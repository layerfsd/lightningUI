//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#include "WiFiNetworkList.h"
#include "guts.h"
#include "msxml.h"
#include "string.hxx"
#include "Eapcfg.h"
#include "Netui.h"

#define WIFICPL_CM_PRIORITY (CM_PRIORITY_BACKGROUND_LOWEST + 0x90)

#define DEFAULT_CONNECTION_CONFIG_COUNT 5
#define MAX_TRY_COUNT_FOR_INSUFFICIENT_BUFFER       5

DWORD g_dwSignalStrengthBar = 30;
HANDLE g_hEvent_NetUi_WiFi_Callback = NULL;

/// <file_topic_scope tref="WiFiUX" />
DBGPARAM dpCurSettings =
{
    TEXT("WIFIUX"),
    {
        TEXT("FATAL"),      // ZONE_FATAL
        TEXT("ERROR"),      // ZONE_ERROR
        TEXT("WARN"),       // ZONE_WARN
        TEXT("INFO"),       // ZONE_INFO
        TEXT("TRACE"),      // ZONE_TRACE
        TEXT("VERBOSE"),    // ZONE_VERBOSE
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT(""),
        TEXT("")
    },
#ifdef DEBUG 
    0x0000  // ulZoneMask currently has all defined zones turned off by default
#else
    0x0000  // For non debug, of course nothing is turned on
#endif
};

//////////////////////////////////////////////////////////////////
// Network List
//////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------
    WiFiNetworkList::WiFiNetworkList

    Default constructor.
------------------------------------------------------------------------------*/
WiFiNetworkList::WiFiNetworkList(HWND hwndNotify,WLAN_NOTIFICATION_CALLBACK lpfnCallback,PVOID pContext) :
	m_hwndNotify(hwndNotify),
    m_hMutex(NULL),
    m_fguidInterfaceSet(FALSE),
    m_dwScanCounter(0),
    m_hEvent_ScanComplete(NULL),
    m_pAvailableNetworkList(NULL),
    m_hWlanClient(NULL),
    m_hCMSession(NULL),
    m_fWiFiListenerAlreadyStarted(FALSE),
    m_fNotificationThreadAlreadyStarted(FALSE),
    m_fUIDoneUpdating(TRUE),
    m_hEvent_CMnotification(NULL),
    m_hNotificationThread(NULL),
    m_hEvent_NotificationThreadExit(NULL),
    m_hEvent_NotificationThreadReady(NULL),
    m_hCMListener(NULL),
	m_lpfnCallback(lpfnCallback),
	m_lpfnCallbackContext(pContext),
	m_statusWiFi(WIFI_DISCONNECTED)
{
    m_hMutex = CreateMutex(NULL, FALSE, NULL);

    // Firstly initialize CS
    InitializeCriticalSection(&m_cs);

    // Clear out the GUID and indices array
    ZeroMemory(&m_guidInterface, sizeof(m_guidInterface));

    // Initialize CM APIs and Session
    m_hCMSession = CmCreateSession();
    CmSetPriority(m_hCMSession, WIFICPL_CM_PRIORITY);
    CmApiInit();

    // Now initialize a clinet and get the interface from WLAN
    InitializeWlanClientAndInterface();

    // Start the notification listeners
    StartNotificationListener();

    m_hEvent_ScanComplete = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hEvent_NetUi_WiFi_Callback = CreateEvent(NULL, FALSE, FALSE, NETUI_WIFI_CALLBACK_EVENT_NAME);
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::~WiFiNetworkList

    Destructor.
------------------------------------------------------------------------------*/
WiFiNetworkList::~WiFiNetworkList()
{
    // Clear the network list
    ClearNetworkListInternal(TRUE);

    // Stop all notification listeners
    StopNotificationListener();

    // Uninit with WLAN
    UninitializeWlanClientAndInterface();

    // Close the CM Session
    if (m_hCMSession)
    {
        CmCloseSession(m_hCMSession);
    }

    CloseHandle(m_hEvent_ScanComplete);
    CloseHandle(g_hEvent_NetUi_WiFi_Callback);

    // Lastly delete CS
    DeleteCriticalSection(&m_cs);

    ReleaseMutex(m_hMutex);
    CloseHandle(m_hMutex);

    ClearWiFiNetUiCallbackWindowHandle();
}

/*
    NETWORK LIST NOTIFICATION

    Network List is all things related to the available network list
    for the device to connect to. In order to maintain the latest
    list together with the status of the networks and the device's
    connection state we register to two main notifications:

    1 - Wi-Fi Notifications, which falls under a callback notification
    pattern, where we get notifications for relevant things like
    signal strength, adapter addition/removal, scan complete etc.

    2 - CM Connection Notifications which provide us with the latest
    connection related state of the device.
*/

/*------------------------------------------------------------------------------
    WiFiNetworkList::StartWiFiListener

    Register for ACM and MSM Wi-Fi Notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::StartWiFiListener()
{
    ASSERT(IsLocked());

    HRESULT hr = S_OK;

    // Initialize just in case this is not done already
    // Note that we already have the lock and changing
    // client and interface member variables is safe
    InitializeWlanClientAndInterface();

    if (m_hWlanClient)
    {
        // Register for ACM and MSM notifications
        DWORD dwRet = WlanRegisterNotification( m_hWlanClient,
                                                WLAN_NOTIFICATION_SOURCE_ACM | WLAN_NOTIFICATION_SOURCE_MSM,
                                                FALSE,
                                                s_WlanNotificationCallback,
                                                this,
                                                NULL,
                                                &m_dwPrevWlanRegisterNotifType);
        CBRA(ERROR_SUCCESS == dwRet);
    }
    
Error:
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::StopWiFiListener

    Unregister for ACM and MSM Wi-Fi Notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::StopWiFiListener()
{
    ASSERT(IsLocked());

    // Unregister Wlan notifications
    if (m_hWlanClient)
    {
        WlanRegisterNotification(m_hWlanClient,
                                 WLAN_NOTIFICATION_SOURCE_NONE,
                                 FALSE,          // do not ignore duplications
                                 NULL,           // no callback function is needed
                                 NULL,           // no callback context is needed
                                 NULL,           // reserved
                                 &m_dwPrevWlanRegisterNotifType);
    }

    return;
}

void WiFiNetworkList::StartNotificationThread()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::StartNotificationThread\n"));
    ASSERT(NULL == m_hNotificationThread);
    ASSERT(NULL == m_hEvent_NotificationThreadExit);
    ASSERT(NULL == m_hEvent_NotificationThreadReady);
    ASSERT(IsLocked());

    DWORD result = ERROR_SUCCESS;

    // Create an event to signal the thread to exit
    m_hEvent_NotificationThreadExit = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (! m_hEvent_NotificationThreadExit)
    {
        result = GetLastError();
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StartNotificationThread: Thread exit event creation failed - Error = 0x%X\n", result));
        goto exit;
    }

    // Create an event to signal when the thread is up
    m_hEvent_NotificationThreadReady = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (! m_hEvent_NotificationThreadReady)
    {
        result = GetLastError();
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StartNotificationThread: Thread ready event creation failed - Error = 0x%X\n", result));
        goto exit;
    }

    m_hNotificationThread = CreateThread(NULL, 0, ListenForCMNotificationsThread, (WiFiNetworkList*)this, 0, NULL);
    if (! m_hNotificationThread)
    {
        result = GetLastError();
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StartCMListenerThread: Listener thread creation failed - Error = 0x%X\n", result));
        goto exit;
    }

    HANDLE h = m_hEvent_NotificationThreadReady;
    
    Unlock();
    DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StartNotificationThread: Waiting for thread creation\n"));
    // Wait for the thread to be up
    result = WaitForSingleObject(h, WIFI_WAIT_FOR_THREAD_START);
    if (result == WAIT_OBJECT_0)
    {
        result = ERROR_SUCCESS;
    }
    else
    {
        result = GetLastError();
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StartNotificationThread: Waiting for thread creation failed - Error = 0x%x\n", result));
    }

    Lock();

exit:
    if (ERROR_SUCCESS != result)
    {
        if (m_hEvent_NotificationThreadExit)
        {
            CloseHandle(m_hEvent_NotificationThreadExit);
            m_hEvent_NotificationThreadExit = NULL;
        }
    }

    if (m_hEvent_NotificationThreadReady)
    {
        CloseHandle(m_hEvent_NotificationThreadReady);
        m_hEvent_NotificationThreadReady = NULL;
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::StartNotificationThread: result = 0x%X\n", result));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::StopNotificationThread
    
    This method stops the interface notification thread and waits for it to exit.
------------------------------------------------------------------------------*/
void WiFiNetworkList::StopNotificationThread(void)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::StopNotificationThread\n"));
    ASSERT(IsLocked());
    DWORD result = ERROR_SUCCESS;

    if (m_hNotificationThread)
    {
        // Signal the notification thread to exit
        SetEvent(m_hEvent_NotificationThreadExit);

        // Wait for the thread to exit
        HANDLE h = m_hNotificationThread;
        Unlock();
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StopNotificationThread: Waiting for thread to exit\n"));
        result = WaitForSingleObject(h, 5000);
        Lock();

        // The notification thread has exited or we timed out, either way clean up.
        DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::StopNotificationThread: WaitForSingleObject returned with result = 0x%X\n", result));
        CloseHandle(m_hNotificationThread);
        m_hNotificationThread = NULL;

        CloseHandle(m_hEvent_NotificationThreadExit);
        m_hEvent_NotificationThreadExit = NULL;
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::StopNotificationThread\n"));
}

DWORD WINAPI WiFiNetworkList::ListenForCMNotificationsThread(LPVOID lpv)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::ListenForCMNotificationsThread\n"));
    WiFiNetworkList* pInstance = (WiFiNetworkList*)lpv;
    pInstance->ListenForCMNotifications();
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::ListenForCMNotificationsThread\n"));
    return 0;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::ListenForCMNotifications
    
    This method listens for CM notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::ListenForCMNotifications(void)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::ListenForCMNotifications\n"));
    BOOL fSetThreadReadyEvent = FALSE;
    BOOL LOOP_FOR_EVER = TRUE;
    HANDLE *hWait = NULL;
    DWORD result;
 
    Lock();

    RegisterForCMNotifications();

    hWait = (HANDLE *) LocalAlloc(LMEM_FIXED, (1/*m_wNotificationEventCount*/+1)*sizeof(HANDLE));
    if (hWait == NULL)
    {
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::ListenForNotifications: LocalAlloc failed\n"));
        ASSERT(FALSE);
        goto Error;
    }

    // Signal that the notification thread is up and running
    fSetThreadReadyEvent = TRUE;
    SetEvent(m_hEvent_NotificationThreadReady);

    // Set the event handle array
    hWait[0] = m_hEvent_NotificationThreadExit;
    hWait[1] = m_hEvent_CMnotification;

    while (LOOP_FOR_EVER)
    {
        Unlock();
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::ListenForNotifications: Waiting for multiple objects\n"));
        result = WaitForMultipleObjects((1/*m_wNotificationEventCount*/+1), hWait, FALSE, INFINITE);
        Lock();

        if ((WAIT_OBJECT_0 != result) && (result <= (WAIT_OBJECT_0 + (DWORD)1/*m_wNotificationEventCount*/)))
        {
            DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::ListenForNotifications: Received notification for interface %d\n", result));
            // Received a notification for an interface
            (void) HandleCMNotification();
    }    
        else
        {
            DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkList::ListenForNotifications: Received notification for interface %d\n", result));
            // Exit event or fatal error
            break;
        }
    }    

Error:
    UnregisterFromCMNotifications();

    if (hWait) LocalFree(hWait);

    if (! fSetThreadReadyEvent)
    {
        SetEvent(m_hEvent_NotificationThreadReady);
    }

    Unlock();
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::ListenForCMNotifications\n"));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::HandleCMNotification

    This method handles all CM notifications.
------------------------------------------------------------------------------*/
DWORD WiFiNetworkList::HandleCMNotification()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::HandleCMNotification\n"));

    ASSERT(IsLocked());

    DWORD dwResult = ERROR_SUCCESS;
    HRESULT hr = S_OK;
    CM_NOTIFICATION* pNotif = NULL;
    CM_CONNECTION_DETAILS *pConnDetails = NULL;
    WIFILIST_SYNCBLK(m_hMutex);
    DWORD dwItemIndex = m_rgNetworkItems.size();

    // Get the notification
    CHR(GetNotification(&pNotif));
    CBR(pNotif != NULL);

    // Connection progress handlers
    if (pNotif->Type == CMNT_SESSION_CONNECTION_SELECTION_PROGRESS)
    {
        // Get the index for this network, if it IS a WiFi network
        CBRA(pNotif->SelectionProgress.szConnection != NULL);
        if (pNotif->SelectionProgress.szConnection)
        {
            // Get the connection details from the notification
            CHR(GetConnectionDetails(pNotif, &pConnDetails));
            CBR(pConnDetails != NULL);
        
            // Make sure it IS a WiFi config
            CBR(pConnDetails->Device == CMCD_WIFI);
        
            // Now find the WiFiNetworkItem for which this is related
            dwItemIndex = FindNetworkItemByName(pNotif->SelectionProgress.szConnection);
        }

        // If the network item could not be found, then bail
        CBR(dwItemIndex < m_rgNetworkItems.size());

        // Now that we have the name of the connection, check what action it is
        switch (pNotif->SelectionProgress.Action)
        {
            case CMSA_ACQUIRE_BEGIN:
            {
                DEBUGMSG(ZONE_INFO, (L"[WiFiUX] WiFiNetworkList::HandleCMNotification - Acquire begin\n"));

                break;
            }

            case CMSA_ACQUIRE_END:
            {
                DEBUGMSG(ZONE_INFO, (L"[WiFiUX] WiFiNetworkList::HandleCMNotification - Acquire end\n"));

                // If we got to connected state, handle the state.
                if (pNotif->SelectionProgress.Result == CMRE_SUCCESS)
                {
                    OnConnected(dwItemIndex, CMRE_SUCCESS);
                }
                else
                {
                    //m_rgNetworkItems[dwItemIndex]->SetErrorReason(GetMappedCMError(pNotif->SelectionProgress.Result));
                }
                break;
            }

            default:
                break;
        }
    }
    // State change handlers
    else if (pNotif->Type == CMNT_CONNECTION_STATE ||
        pNotif->Type == CMNT_CONNECTION_STATE_CONNECTED ||
        pNotif->Type == CMNT_CONNECTION_STATE_DISCONNECTED)
    {
        CBRA(pNotif->StateUpdate.szConnection != NULL);

        // Get the index for this network, if it IS a WiFi network
        if (pNotif->StateUpdate.szConnection)
        {
            CHR(GetConnectionDetails(pNotif, &pConnDetails));
            CBR(pConnDetails != NULL);
        
            // Make sure it IS a WiFi config
            CBR(pConnDetails->Device == CMCD_WIFI);
        
            // Now find the WiFiNetworkItem for which this is related
            dwItemIndex = FindNetworkItemByName(pNotif->StateUpdate.szConnection);
        }

        // If the network item could not be found, then bail
        CBR(dwItemIndex < m_rgNetworkItems.size());
        
        switch (pNotif->StateUpdate.State)
        {
            case CMCS_DISCONNECTED:
            {
                m_rgNetworkItems[dwItemIndex]->SetAvailable();
            }
            case CMCS_DISCONNECTED_UNAVAILABLE:
            case CMCS_DISCONNECTED_DISABLED:
            {
                OnDisconnected(dwItemIndex, pNotif->StateUpdate.LastResult);
                break;
            }

            case CMCS_DISCONNECTING:
            {
                OnDisconnecting(dwItemIndex, pNotif->StateUpdate.LastResult);
                break;
            }

            case CMCS_CONNECTING:
            case CMCS_CONNECTING_ASSOCIATING:
            case CMCS_CONNECTING_AUTHENTICATING:
            case CMCS_CONNECTING_WAITING_FOR_IP:
            {
                OnConnecting(dwItemIndex);
                break;
            }

            case CMCS_CONNECTED:
            {
                // Handle connected state appropriately
                // We send in the error code as well, since
                // we need to know if we connected successfully
                // without any errors
                OnConnected(dwItemIndex, pNotif->StateUpdate.LastResult);
                break;
            }
            
            default: 
            {
                break;
            }
        }
    }
    // Now check if the config was really added or deleted...
    // Configuration change handlers
    else if (   (pNotif->Type == CMNT_CONNECTION_CONFIGURATION_ADDED) ||
                (pNotif->Type == CMNT_CONNECTION_CONFIGURATION_DELETED) ||
                (pNotif->Type == CMNT_CONNECTION_CONFIGURATION_UPDATED))
    {
        CBRA(pNotif->ConfigUpdate.szConnection != NULL);

        // Get the index for this network, if it IS a WiFi network
        if (pNotif->ConfigUpdate.szConnection)
        {
            if ((pNotif->Type == CMNT_CONNECTION_CONFIGURATION_ADDED) ||
                (pNotif->Type == CMNT_CONNECTION_CONFIGURATION_UPDATED))
            {
                CHR(GetConnectionDetails(pNotif, &pConnDetails));
                CBR(pConnDetails != NULL);
        
                // Make sure it IS a WiFi config
                CBR(pConnDetails->Device == CMCD_WIFI);
            }
        
            // Now find the WiFiNetworkItem for which this is related
            // Notice that we will try to find for all networks (not just WiFi in case of DELETED... no big deal)
            dwItemIndex = FindNetworkItemByName(pNotif->ConfigUpdate.szConnection);

        }

        // If the network item could not be found, then bail
        CBR(dwItemIndex < m_rgNetworkItems.size());

        // Make sure we have a valid pointer
        CBRA(m_rgNetworkItems[dwItemIndex] != NULL);

        // Reconfigure the internal network data as appropriate
        m_rgNetworkItems[dwItemIndex]->ConfigureInternalWiFiNetworkData(FALSE);

        // Set the state of the item accordingly
        m_rgNetworkItems[dwItemIndex]->UpdateConnectionState();

        // If the item is deleted then I better tell the view to reset the dude on the view.
        // if (pNotif->Type == CMNT_CONNECTION_CONFIGURATION_DELETED)
    }

Error:
    // Free up any allocated memory accordingly and return
    if (pNotif)
    {
        LocalFree(pNotif);
        pNotif = NULL;
    }

    if (pConnDetails)
    {
        LocalFree(pConnDetails);
        pConnDetails = NULL;
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::HandleCMNotification\n"));
    return dwResult;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::GetNotification

    This method gets the CM notifications.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkList::GetNotification(CM_NOTIFICATION **ppNotif)
{
    ASSERT(IsLocked());

    HRESULT hr = S_OK;
    CM_RESULT cmRes = CMRE_SUCCESS;
    CM_NOTIFICATION* pNotification = NULL;
    DWORD cbNotification = sizeof(CM_NOTIFICATION);

    // If the input parameters are invalid, then bail
    CBRA(ppNotif != NULL);

    // Initial allocation
    pNotification = (CM_NOTIFICATION*) LocalAlloc(LPTR, cbNotification);
    CPR(pNotification);
    pNotification->Version = CM_CURRENT_VERSION;

    // Now get the notification on the initially allocated memory
    cmRes = CmGetNotification(m_hCMListener, pNotification, &cbNotification);
    // which will invariably give me insufficient buffer error.
    while (CMRE_INSUFFICIENT_BUFFER == cmRes)
    {
        // Reallocate a larger buffer and try again
        LocalFree(pNotification);
        pNotification = (CM_NOTIFICATION*) LocalAlloc(LPTR, cbNotification);
        CPR(pNotification);
        pNotification->Version = CM_CURRENT_VERSION;

        // Get the notification
        cmRes = CmGetNotification(m_hCMListener, pNotification, &cbNotification);
    }

    // Check for success
    CBR(CMRE_SUCCESS == cmRes);

    // Sanity check on the notification struct
    CBR(NULL != pNotification);

    // Assign the notification to the input and return
    *ppNotif = pNotification;

Exit:
    return hr;

Error:
    // If we came here, then we failed to get notification
    // so clean up the allocated notification memory
    if (pNotification)
    {
        LocalFree(pNotification);
    }

    // Set input to null and return
    *ppNotif = NULL;
    goto Exit;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::RegisterForCMNotifications

    This method registers for Connection related CM notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::RegisterForCMNotifications()
{
    ASSERT(IsLocked());

    HRESULT hr = S_OK;
    CM_NOTIFICATIONS_LISTENER_REGISTRATION cmnlr = {0};
    CM_RESULT cr = CMRE_SUCCESS;

    // Register for all CM Network related notifications.
    // This looks like an over kill, but we need a loooot of
    // notifications and there is no way of specifying (OR-ing)
    // them individually with CM.
    cmnlr.Version = CM_CURRENT_VERSION;
    cmnlr.cRegistration = 1;
    cmnlr.Registration[0].NotificationType = CMNT_ALL;
    cmnlr.Registration[0].Connection.Selection = CMST_CONNECTION_ALL;

    // Create the event on which we should wait for notifications
    if (m_hEvent_CMnotification == NULL)
    {
        m_hEvent_CMnotification = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    CBR(NULL != m_hEvent_CMnotification);

    // Register with CM
    cmnlr.hNotificationsAvailableEvent = m_hEvent_CMnotification;
    cr = CmRegisterNotificationsListener(&cmnlr, sizeof(cmnlr), &m_hCMListener);
    CBR(CMRE_SUCCESS == cr);

    return;

Error:
    // If registration fails for any reason, unregister and bail
    UnregisterFromCMNotifications();
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::UnregisterFromCMNotifications

    This method unregisters from CM notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::UnregisterFromCMNotifications()
{
    ASSERT(IsLocked());

    // If we have a valid handle on which we were
    // listening on for CM notifications, close it
    if (m_hEvent_CMnotification)
    {
        CloseHandle(m_hEvent_CMnotification);
        m_hEvent_CMnotification = NULL;
    }

    // If we had a valid CM Listener handle, close it
    if (m_hCMListener)
    {
        CmUnregisterNotificationsListener(m_hCMListener);
        m_hCMListener = NULL;
    }

    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::GetConnectionDetails

    This method gets the CM connection details from a given notification
    for the connection.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkList::GetConnectionDetails(CM_NOTIFICATION *pNotif, CM_CONNECTION_DETAILS **ppConnDetails)
{
    HRESULT hr = S_OK;
    LPCTSTR pszConnName = NULL;
    CM_CONNECTION_DETAILS *pConnDetails = NULL;
    DWORD cbConnDetails = sizeof(CM_CONNECTION_DETAILS);
    CM_RESULT cmRes = CMRE_SUCCESS;

    // Validate the input
    ASSERT(pNotif != NULL);
    ASSERT(ppConnDetails != NULL);
    ASSERT(pNotif->StateUpdate.szConnection != NULL);
    ASSERT(*ppConnDetails == NULL);
    CBR(pNotif != NULL);
    CBR(ppConnDetails != NULL);
    CBR(pNotif->StateUpdate.szConnection != NULL);

    // Cache the connection name
    pszConnName = pNotif->StateUpdate.szConnection;

    // Now that we know the connection name, get full connection details
    pConnDetails = (CM_CONNECTION_DETAILS*) LocalAlloc(LPTR, cbConnDetails);
    CPR(pConnDetails);
    pConnDetails->Version = CM_CURRENT_VERSION;
    cmRes = CmGetConnectionDetailsByName(pszConnName, pConnDetails, &cbConnDetails);
    while (CMRE_INSUFFICIENT_BUFFER == cmRes)
    {
        // Need more space - reallocate and try again
        LocalFree(pConnDetails);
        pConnDetails = (CM_CONNECTION_DETAILS*) LocalAlloc(LPTR, cbConnDetails);
        CPR(pConnDetails);
        pConnDetails->Version = CM_CURRENT_VERSION;
          
        cmRes = CmGetConnectionDetailsByName(pszConnName, pConnDetails, &cbConnDetails);
    }

    // If we did not get a valid detail, then bail...
    CBR(CMRE_SUCCESS == cmRes);

    // Set the input with the allocated details and return
    *ppConnDetails = pConnDetails;
    return hr;

Error:
    // If we came till here, then for sure something went wrong.
    // If we had allocated memory for details, free it.
    if (NULL != pConnDetails)
    {
        LocalFree(pConnDetails);
    }

    // Set the input detail to null for the user to know and return
    *ppConnDetails = NULL;
    return hr;
}

VOID WiFiNetworkList::OnConnecting(DWORD dwItemIndex)
{
	RETAILMSG(1,(_T(">>>>>WiFiNetworkList::OnConnecting...................idx:%d\n"),dwItemIndex));

    HRESULT hr = S_OK;
    WIFILIST_SYNCBLK(m_hMutex);
    CBRA(dwItemIndex < m_rgNetworkItems.size());

    m_rgNetworkItems[dwItemIndex]->SetConnectionState(CMCS_CONNECTING);
//    m_rgNetworkItems[dwItemIndex]->SetErrorReason(REASON_ERROR_SUCCESS);

	//update status bar:
	BOOL bConnected=FALSE;
	for (DWORD idx=0;idx< m_rgNetworkItems.size();idx++)
	{
		if(m_rgNetworkItems[idx]->GetConnectionState()==CMCS_CONNECTED)
		{
			bConnected=TRUE;
			break;
		}
	}

	if(!bConnected)
	{
		::PostMessage(m_hwndNotify,WM_WIFI_STATUS,WIFI_CONNECTING,NULL);
		m_statusWiFi=WIFI_CONNECTING;
	}

Error:
    return;
}

VOID WiFiNetworkList::OnConnected(DWORD dwItemIndex, CM_RESULT cmResult)
{
	RETAILMSG(1,(_T(">>>>>WiFiNetworkList::OnConnected...................idx:%d\n"),dwItemIndex));

    HRESULT hr = S_OK;
    WIFILIST_SYNCBLK(m_hMutex);
    CBRA(dwItemIndex < m_rgNetworkItems.size());

    // Set the connection state to be most accurate
    m_rgNetworkItems[dwItemIndex]->SetConnectionState(CMCS_CONNECTED);

    // and just re-sort and refresh the list
//    SortNetworkList();

	::PostMessage(m_hwndNotify,WM_WIFI_STATUS,WIFI_CONNECTED,NULL);
	m_statusWiFi=WIFI_CONNECTED;

Error:
    return;
}

VOID WiFiNetworkList::OnDisconnected(DWORD dwItemIndex, CM_RESULT cmResult)
{
	RETAILMSG(1,(_T(">>>>>WiFiNetworkList::OnDisconnected...................idx:%d\n"),dwItemIndex));

    HRESULT hr = S_OK;
    WIFILIST_SYNCBLK(m_hMutex);
    CBRA(dwItemIndex < m_rgNetworkItems.size());

    m_rgNetworkItems[dwItemIndex]->SetConnectionState(CMCS_DISCONNECTED);

	//update status bar:
	BOOL bConnected=FALSE;
	for (DWORD idx=0;idx< m_rgNetworkItems.size();idx++)
	{
		if(m_rgNetworkItems[idx]->GetConnectionState()==CMCS_CONNECTED)
		{
			bConnected=TRUE;
			break;
		}
	}

	if(!bConnected)
	{
		::PostMessage(m_hwndNotify,WM_WIFI_STATUS,WIFI_DISCONNECTED,NULL);
		m_statusWiFi=WIFI_DISCONNECTED;
	}

Error:
    return;
}

VOID WiFiNetworkList::OnDisconnecting(DWORD dwItemIndex, CM_RESULT cmResult)
{
	RETAILMSG(1,(_T(">>>>>WiFiNetworkList::OnDisconnecting...................idx:%d\n"),dwItemIndex));

    HRESULT hr = S_OK;
    WIFILIST_SYNCBLK(m_hMutex);
    CBRA(dwItemIndex < m_rgNetworkItems.size());

    m_rgNetworkItems[dwItemIndex]->SetConnectionState(CMCS_DISCONNECTING);
    // TODO: Do we really care about error condition during -ing states?
    // m_rgNetworkItems[dwItemIndex]->SetErrorReason(GetMappedCMError(cmResult));
//    m_rgNetworkItems[dwItemIndex]->SetErrorReason(REASON_ERROR_SUCCESS);

	//update status bar:
	BOOL bConnected=FALSE;
	for (DWORD idx=0;idx< m_rgNetworkItems.size();idx++)
	{
		if(m_rgNetworkItems[idx]->GetConnectionState()==CMCS_CONNECTED)
		{
			bConnected=TRUE;
			break;
		}
	}

	if(!bConnected)
	{
		::PostMessage(m_hwndNotify,WM_WIFI_STATUS,WIFI_DISCONNECTING,NULL);
		m_statusWiFi=WIFI_DISCONNECTING;
	}

Error:
    return;
}

DWORD WiFiNetworkList::FindNetworkItemByName(TCHAR *pszConnection)
{
    DWORD dwItemIndex=0;

    // For invalid input, just return the size of the vector indicating it was not found
    if (pszConnection == NULL) return m_rgNetworkItems.size();

    // Go over the list of networks already shown and see if we need to add this newly or if we have to remove it...
    for (dwItemIndex=0; dwItemIndex<m_rgNetworkItems.size(); dwItemIndex++)
    {
        TCHAR szSsidFromItem[DOT11_SSID_MAX_LENGTH + 1];
        ZeroMemory(szSsidFromItem, (DOT11_SSID_MAX_LENGTH + 1)*sizeof(TCHAR));
        m_rgNetworkItems[dwItemIndex]->GetSzSSID(szSsidFromItem);

//         if (_tcsncmp(szSsidFromItem, pszConnection, (DOT11_SSID_MAX_LENGTH+1)) == 0)
		if (wcscmp(szSsidFromItem, pszConnection) == 0)
        {
            // Found network...
            break;
        }
    }

    /// Notice that if the network is not found, then the index will be the size of the network items vector
    return dwItemIndex;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::StartNotificationListener

    Register for Wi-Fi call back notifications and CM connection notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::StartNotificationListener()
{
    // This is an entry point for all notification listeners.
    // First we should grab the CS.
    Lock();

    // Start the Wi-Fi Listener
    // Ignore the success/failure since a failure will only imply the stack is not able to
    // send us notifications.
    if (m_fWiFiListenerAlreadyStarted == FALSE)
    {
        StartWiFiListener();
        m_fWiFiListenerAlreadyStarted = TRUE;
    }

    // Start a thread to register and listen to relevant notifications.
    if (m_fNotificationThreadAlreadyStarted == FALSE)
    {
        StartNotificationThread();
        m_fNotificationThreadAlreadyStarted = TRUE;
    }

    // Once we are done spinning the listeners, release the lock
    Unlock();

    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::StopNotificationListener

    Unregister from Wi-Fi call back notifications and CM connection notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::StopNotificationListener()
{
    // First we should grab the CS.
    Lock();

    // Stop the Wi-Fi Listener
    StopWiFiListener();
    m_fWiFiListenerAlreadyStarted = FALSE;

    // Stop the main notification thread
    StopNotificationThread();
    m_fNotificationThreadAlreadyStarted = FALSE;

    // Once we are done spinning the listeners, release the lock
    Unlock();

    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::s_WlanNotificationCallback

    This is a static method which we pass to Wi-Fi stack while
    registering for Wi-Fi notifications.
------------------------------------------------------------------------------*/
void WiFiNetworkList::s_WlanNotificationCallback(PWLAN_NOTIFICATION_DATA pNotifData, PVOID pContext)
{
    HRESULT hr = S_OK;
    WiFiNetworkList *pListener = NULL;

    CBRA(NULL != pNotifData && NULL != pContext);
    pListener = static_cast<WiFiNetworkList*>(pContext);
    pListener->HandleWlanCallbackNotification(pNotifData);
    
	if(pListener->m_lpfnCallback)
	{
		pListener->m_lpfnCallback(pNotifData,pListener->m_lpfnCallbackContext);
	}

Error:
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::HandleWlanCallbackNotification

    Handle the Wi-Fi notifications.
    Notice that this is called on a thread that the Wi-Fi stack controls.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkList::HandleWlanCallbackNotification(PWLAN_NOTIFICATION_DATA pNotifData)
{
    // Synchronize this tread first...
    Lock();

    HRESULT hr = S_OK;
    PWLAN_CONNECTION_NOTIFICATION_DATA pConnNotifData = NULL;

// 	RETAILMSG(1,(_T("WiFiNetworkList::HandleWlanCallbackNotification:NotificationSource:%d:NotificationCode%d\n"),pNotifData->NotificationSource,pNotifData->NotificationCode));

    switch (pNotifData->NotificationSource)
    {
        case WLAN_NOTIFICATION_SOURCE_MSM:
        {
            switch (pNotifData->NotificationCode)
            {
                case wlan_notification_msm_signal_quality_change:
                {
                    // TODO
					//UpdateWiFiNetworkList();
                    break;
                }

                case wlan_notification_msm_associating:
                case wlan_notification_msm_associated:
                case wlan_notification_msm_radio_state_change:
                case wlan_notification_msm_disassociating:
                {
                    // TODO
                    break;
                }
				case wlan_notification_msm_disconnected:
				case wlan_notification_msm_connected:
					UpdateWiFiNetworkList();
					break;

                default:
                    break;
            }
            break;
        }

        case WLAN_NOTIFICATION_SOURCE_ACM:
        {
            switch (pNotifData->NotificationCode)
            {                
                case wlan_notification_acm_scan_complete: // After scan finished, we update the signal quality
                case wlan_notification_acm_scan_fail: // The scan failed somehow, for example, a background scan is already in progress.
                                                      // In this case, we still make the call to OnScanComplete, application can still call GetAvailableNetworkList to get the result of background scan.
                {
                    OnScanComplete();
					UpdateWiFiNetworkList();

                    break;
                }
				case wlan_notification_acm_network_available:
// 					UpdateWiFiNetworkList();

					break;

                case wlan_notification_acm_connection_complete:
                {
                     PWLAN_CONNECTION_NOTIFICATION_DATA pConnectionData = (PWLAN_CONNECTION_NOTIFICATION_DATA)pNotifData->pData;
                     if (pConnectionData && pConnectionData->wlanReasonCode != WLAN_REASON_CODE_SUCCESS)
                     {
                         TCHAR szSsid[DOT11_SSID_MAX_LENGTH + 1] = _T("");

                         CHRA(SsidToString(&(pConnectionData->dot11Ssid), szSsid, ARRAYSIZE(szSsid)));
                         // Reverse find the network index we are looking at here... this is all to do with the name mangling feature
                         DWORD dwItemIndex = FindNetworkItemByName(szSsid); // FindNetworkNameByMangledName(szSsid);
                         if (dwItemIndex < m_rgNetworkItems.size())
                         {
                             m_rgNetworkItems[dwItemIndex]->NotifyFailure();
                         }
                     }
                }
				break;

/*                case wlan_notification_acm_connection_attempt_fail: // connect failed
                {
                    if (pNotifData->dwDataSize < sizeof(WLAN_CONNECTION_NOTIFICATION_DATA))
                    {
                        ASSERT(FALSE);
                        break;
                    }

                    TCHAR szSsid[DOT11_SSID_MAX_LENGTH + 1] = _T("");
                    pConnNotifData = (PWLAN_CONNECTION_NOTIFICATION_DATA)pNotifData->pData;
                    CHRA(SsidToString(&(pConnNotifData->dot11Ssid), szSsid, ARRAYSIZE(szSsid)));
                    if (WLAN_REASON_CODE_MSMSEC_PSK_MISMATCH_SUSPECTED == pConnNotifData->wlanReasonCode)
                    {
                        // Reverse find the network index we are looking at here... this is all to do with the name mangling feature
                        DWORD dwItemIndex = FindNetworkItemByName(szSsid); // FindNetworkNameByMangledName(szSsid);
                        // Now set the error reason for this as the passkey error if it IS a valid network list item
                        if (dwItemIndex < m_rgNetworkItems.size())
                        {
                            m_rgNetworkItems[dwItemIndex]->SetErrorReason(REASON_ERROR_INCORRECT_PASSKEY);
                        }
                    }  
                    else if (ONEX_UI_FAILURE == pConnNotifData->wlanReasonCode) 
                    {  
                        // TODO: Handle this accordingly. We receive ONEX_UI_FAILURE when user cancel an user authentication.
                    }

                    break;
                }               

                case wlan_notification_acm_interface_arrival: // add interface        
                {
                    // Uninit from WLAN what might have been invalid handles
                    UninitializeWlanClientAndInterface();

                    // New arrival, reinitialize the WLAN Client stuff
                    InitializeWlanClientAndInterface();
                        
                    // Refresh interface information
                    UpdateInterfaceInformation();
                    break;

                }
                case wlan_notification_acm_interface_removal: // remove interface
                {
                    // Uninit from WLAN
                    UninitializeWlanClientAndInterface();

                    // Refresh interface information
                    UpdateInterfaceInformation();
                    break;
                }*/
            
                default:
                {
                    break;
                }
            }
            break;
        }    

        default:
            break;
    } 

Error:
    Unlock();
    return hr;
}

/*
    NETWORK LIST HELPER METHODS

    This area includes methods which do things like un/initializing
    a client with the Wi-Fi stack, calling relevant functions in the
    Wi-Fi stack to initiate a scan, updating the interface used, etc..
*/

/*------------------------------------------------------------------------------
    WiFiNetworkList::InitializeWlanClientAndInterface

    Initialize the client with Wi-Fi stack and get the adapter's GUID.
    Currently we support only one adapter!
------------------------------------------------------------------------------*/
void WiFiNetworkList::InitializeWlanClientAndInterface()
{
    DWORD dwError, dwNegotiatedVersion;
    PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;

    if (!m_hWlanClient)
    {
        dwError = WlanOpenHandle(CSP_WIFI_SPECIFIC_INFO_VERSION, NULL, &dwNegotiatedVersion, &m_hWlanClient);
        if (ERROR_SUCCESS != dwError) return;
    }

    if (!m_fguidInterfaceSet)
    {
        dwError = WlanEnumInterfaces(m_hWlanClient, NULL, &pInterfaceList);
        if (ERROR_SUCCESS != dwError) return;
        
        // We only support up to one interface for now
        ASSERT(pInterfaceList->dwNumberOfItems <= 1);
        if (1 > pInterfaceList->dwNumberOfItems) return;
        
        m_guidInterface = pInterfaceList->InterfaceInfo[0].InterfaceGuid;
        m_fguidInterfaceSet = TRUE;
    }
    if (pInterfaceList)
    {
        WlanFreeMemory(pInterfaceList);
    }
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::UninitializeWlanClientAndInterface

    Uninitialize the client with Wi-Fi stack.
------------------------------------------------------------------------------*/
void WiFiNetworkList::UninitializeWlanClientAndInterface()
{
    if (m_hWlanClient)
    {
        WlanCloseHandle(m_hWlanClient, NULL);
        m_hWlanClient = NULL;
    }

    // Clean previous interface information and close the previous handle
    if (m_fguidInterfaceSet)
    {
        m_fguidInterfaceSet = FALSE;
        ZeroMemory(&m_guidInterface, sizeof(m_guidInterface));
    }

    return;
}

/// <summary>
/// Clear connections from list
/// </summary>
/// <returns>
/// Returns HRESULT, S_OK on success, error code otherwise.
/// </returns>
void WiFiNetworkList::ClearNetworkListInternal(BOOL fShutting)
{
    Lock();

    if (m_pAvailableNetworkList)
    {
        WlanFreeMemory(m_pAvailableNetworkList);
        m_pAvailableNetworkList = NULL;
    }

    // Clear all connections
    for (UINT idx = 0; idx < m_rgNetworkItems.size(); idx++)
    {
        delete m_rgNetworkItems[idx];
        m_rgNetworkItems[idx] = NULL;
    }
    m_rgNetworkItems.clear();

    // Clear all connections in the new list, if any
//    for (UINT idx = 0; idx < m_rgNetworkItemsOnReset.size(); idx++)
//    {
//        SAFERELEASE(m_rgNetworkItemsOnReset[idx]);
//    }
//    m_rgNetworkItemsOnReset.clear();
    
    Unlock();

    return;
}

BOOL WiFiNetworkList::StartScan()
{
	DWORD dwError=ERROR_SUCCESS;
    Lock();
    InitializeWlanClientAndInterface();
	if(m_fguidInterfaceSet)
	{
		dwError=WlanScan(m_hWlanClient, &m_guidInterface, NULL, NULL, NULL);
		m_dwScanCounter++;
	}
	else
	{
		dwError=S_FALSE;
	}
    Unlock();
    return (dwError==ERROR_SUCCESS);
}

/*------------------------------------------------------------------------------
    WiFiNetworkList::ShouldShowNetwork

    Determine if PWLAN_AVAILABLE_NETWORK should be shown in the available networks VLV
    We simplify the detection to simply seeing if strProfileName is empty
    and the SSID is non-empty and printable because there's no way to guarantee 
    that a network with name foo from WLAN is the same network as the network with
    name foo from CM. 
    Additionally we make sure the network meets the min signal strength criteria <para />
    Function is static because we only use it in this file.
------------------------------------------------------------------------------*/
BOOL WiFiNetworkList::ShouldShowNetwork(WiFiNetworkItem *pNetworkItem)
{
    HRESULT hr = S_OK;
    TCHAR szSsid[DOT11_SSID_MAX_LENGTH + 1];
    BOOL  fRet = FALSE;

    // Check if the network is pinned
    // Immaterial of what is with this network, if it is pinned, show it
//    if (pNetworkItem->IsPinned())
//    {
//        fRet = TRUE;
//        goto Error;
//    }

    // Check if the network is connected or known. If either, then we should show it!
    if ((pNetworkItem->GetConnectionState() == CMCS_CONNECTED) || (pNetworkItem->IsKnownNetwork() == TRUE))
    {
        fRet = TRUE;
        goto Error;
    }

    // It is neither known nor connected, check if it is a valid network
    PWLAN_AVAILABLE_NETWORK pAvailableNetwork = pNetworkItem->GetStackNetworkData();
    CBR(NULL != pAvailableNetwork);
    if ((0 == pAvailableNetwork->strProfileName[0]) && (0 < pAvailableNetwork->dot11Ssid.uSSIDLength))
    {
        // No saved profile name but SSID buffer contains something - check to see if
        // this is a valid string we can display
        szSsid[0] = 0;
        CHR(SsidToString(&(pAvailableNetwork->dot11Ssid), szSsid, ARRAYSIZE(szSsid)));
        fRet = (0 != szSsid[0]);
    }

    // Now check if the network meets the signal strength criteria
//    if (fRet && (pNetworkItem->GetAverageSignalStrength()< g_dwSignalStrengthBar))
//    {
//        fRet = FALSE;
//    }

Error:
    return fRet;
}
/*------------------------------------------------------------------------------
    WiFiNetworkList::OnScanComplete

    On a scan complete notification add/remove entries and depending on the
    number of times we have finished a scan re-sort the list.
    The idea is, every time a COMEPLTE_REFRESH_INTERVAL scan
    completes, re-sorting the exisiting list and refresh the screen will render a
    more realiztic list.
------------------------------------------------------------------------------*/
void WiFiNetworkList::OnScanComplete()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkList::OnScanComplete\n"));

    if (FALSE==m_fUIDoneUpdating) return;

    if (m_dwScanCounter > 0)
    {
        m_dwScanCounter--;
        SetEvent(m_hEvent_ScanComplete);
    }

//Cleanup:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkList::OnScanComplete\n"));
    return;
}

BOOL WiFiNetworkList::IsScanComplete(DWORD dwTimeOut)
{
    DWORD dwTimeOutMS=INFINITE;

    if (dwTimeOut)
    {
        dwTimeOutMS = dwTimeOut;
    }

    if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent_ScanComplete, dwTimeOutMS))
    {
        ResetEvent(m_hEvent_ScanComplete);
    }
    UpdateWiFiNetworkList();

    return TRUE;
}

VOID WiFiNetworkList::UpdateWiFiNetworkList()
{
    if (m_pAvailableNetworkList)
    {
        WlanFreeMemory(m_pAvailableNetworkList);
        m_pAvailableNetworkList = NULL;
    }

    if (NULL==m_pAvailableNetworkList)
    {
        ClearNetworkListInternal(FALSE);
        if (FAILED(GetAvailableNetworkList(m_hWlanClient, &m_pAvailableNetworkList, &m_guidInterface))) return;
        // Add/remove entries accordingly
        AddNewEntries();
//         RemoveStaleEntries();
//         SortNetworkList();
//         AppendCmConnections();
    }
}

VOID WiFiNetworkList::AddNewEntries()
{

    if (m_pAvailableNetworkList)
    {

        DWORD iListNetwork = 0;
        DWORD iNetwork = 0;
        // Go over each network item and insert new ones accordingly
        for (iNetwork = 0; iNetwork < m_pAvailableNetworkList->dwNumberOfItems; iNetwork++)
        {
            TCHAR szSsidFromStack[DOT11_SSID_MAX_LENGTH + 1];
            ZeroMemory(szSsidFromStack, (DOT11_SSID_MAX_LENGTH + 1) * sizeof(TCHAR));
            SsidToString(&((m_pAvailableNetworkList->Network[iNetwork]).dot11Ssid), szSsidFromStack, ARRAYSIZE(szSsidFromStack));
// 			RETAILMSG(1,(_T(">>>>>CPanelSettingsWireless::UpdateNetworkList:index:%d:SSID:%s\n"),iNetwork,szSsidFromStack));

            // Go over the list of networks already shown and see if we need to add this newly or if we have to remove it...
            for (iListNetwork=0; iListNetwork<m_rgNetworkItems.size(); iListNetwork++)
            {
                TCHAR szSsidFromItem[DOT11_SSID_MAX_LENGTH + 1];
                ZeroMemory(szSsidFromItem, (DOT11_SSID_MAX_LENGTH + 1) * sizeof(TCHAR));
                m_rgNetworkItems[iListNetwork]->GetSzSSID(szSsidFromItem);
                if (wcscmp(szSsidFromItem, szSsidFromStack) == 0)
                {
                    // Exit out of this for loop
                    break;
                }
            }

            // If we ran till the end of the list, then we did not find the item, or we are forcing a re-add... so add it
            if (iListNetwork == m_rgNetworkItems.size())
            {
                // Add it only if it should be...
                WiFiNetworkItem *pNetworkItem = new WiFiNetworkItem((&(m_pAvailableNetworkList->Network[iNetwork])), m_hWlanClient, m_guidInterface, m_hCMSession);
                if (pNetworkItem)
                {
                    if (ShouldShowNetwork(pNetworkItem))
                    {
                        DWORD dwIndex = 0;//GetAndUpdateIndices(pNetworkItem);
                        m_rgNetworkItems.insert(m_rgNetworkItems.begin()+dwIndex, pNetworkItem);
                    }
                    else
                    {
                        SAFE_DELETE(pNetworkItem);
                    }
                }
            }
        }

// 		RETAILMSG(1,(_T(">>>>>WiFiNetworkList::AddNewEntries:total add count:%d\n"),m_rgNetworkItems.size()));

    }

    return;
}

HRESULT WiFiNetworkList::InsertNewEntry(WiFiNetworkItem* pWiFiNetworkItem, BOOL fForceInsert)
{
    HRESULT hr = S_OK;
    DWORD iListNetwork = 0;
    WIFILIST_SYNCBLK(m_hMutex);

    if (pWiFiNetworkItem)
    {
        TCHAR pszSsidNetItem[DOT11_SSID_MAX_LENGTH + 1];
        pWiFiNetworkItem->GetSzSSID(pszSsidNetItem);
        // Go over each network item
        for (iListNetwork = 0; iListNetwork < m_rgNetworkItems.size(); iListNetwork++)
        {
            TCHAR szSsidFromItem[DOT11_SSID_MAX_LENGTH + 1];
            ZeroMemory(szSsidFromItem, (DOT11_SSID_MAX_LENGTH + 1) * sizeof(TCHAR));
            m_rgNetworkItems[iListNetwork]->GetSzSSID(szSsidFromItem);
//             if (_tcsncmp(szSsidFromItem, pszSsidNetItem, (DOT11_SSID_MAX_LENGTH+1)) == 0)
			if (wcscmp(szSsidFromItem, pszSsidNetItem) == 0)
            {
                if (fForceInsert)
                {
                    DWORD dwSignalQuality = m_rgNetworkItems[iListNetwork]->GetNetworkSignalStrength();
                    delete m_rgNetworkItems[iListNetwork];
                    m_rgNetworkItems[iListNetwork] = pWiFiNetworkItem;
                    pWiFiNetworkItem->SetNetworkSignalStrength(dwSignalQuality);
                    pWiFiNetworkItem->InitializeCMsession(m_hCMSession); //share one CM session in the Network list
                    goto Error;
                }
                else
                {   
                    CHR(E_FAIL);
                }
            }
        }

        if (iListNetwork == m_rgNetworkItems.size())
        {
            m_rgNetworkItems.insert(m_rgNetworkItems.end(), pWiFiNetworkItem);
            pWiFiNetworkItem->InitializeCMsession(m_hCMSession); //share one CM session in the Network list
        }
    }

Error:
    return hr;
}

HRESULT WiFiNetworkList::DeleteEntryConfigByName(LPCTSTR pszNetworkName)
{
    HRESULT hr = S_OK;
    DWORD iListNetwork = 0;
    LPTSTR pszName = NULL;
    WIFILIST_SYNCBLK(m_hMutex);

    if (pszNetworkName)
    {
        // Go over each network item
        for (iListNetwork = 0; iListNetwork < m_rgNetworkItems.size(); iListNetwork++)
        {
            CHR(m_rgNetworkItems[iListNetwork]->GetNetworkNameText(&pszName));
            if (!_tcsicmp(pszNetworkName, pszName))
            {
                m_rgNetworkItems[iListNetwork]->RemoveConfiguration();
                delete m_rgNetworkItems[iListNetwork];
                m_rgNetworkItems[iListNetwork] = NULL;
                m_rgNetworkItems.erase(m_rgNetworkItems.begin()+iListNetwork);
                break;
            }
        }
    }

Error:
    return hr;
}

HRESULT WiFiNetworkList::RemoveEntry(WiFiNetworkItem** ppWiFiNetworkItem)
{
    HRESULT hr = S_OK;
    DWORD iListNetwork = 0;
    WIFILIST_SYNCBLK(m_hMutex);

    if (ppWiFiNetworkItem && (*ppWiFiNetworkItem))
    {
        // Go over each network item
        for (iListNetwork = 0; iListNetwork < m_rgNetworkItems.size(); iListNetwork++)
        {
            if (*ppWiFiNetworkItem==m_rgNetworkItems[iListNetwork])
            {
                delete m_rgNetworkItems[iListNetwork];
                m_rgNetworkItems[iListNetwork] = NULL;
                *ppWiFiNetworkItem = NULL;
                m_rgNetworkItems.erase(m_rgNetworkItems.begin()+iListNetwork);
            }
        }
    }

Error:
    return hr;
}

VOID WiFiNetworkList::RemoveStaleEntries()
{

    if (m_pAvailableNetworkList)
    {
        DWORD iListNetwork = 0;
        DWORD iNetwork = 0;
        // Go over the list of networks already shown and see if we need to delete any stale ones
        for (iListNetwork=0; iListNetwork<m_rgNetworkItems.size(); iListNetwork++)
        {
            TCHAR szSsidFromItem[DOT11_SSID_MAX_LENGTH + 1];
            ZeroMemory(szSsidFromItem, (DOT11_SSID_MAX_LENGTH + 1)*sizeof(TCHAR));
            m_rgNetworkItems[iListNetwork]->GetSzSSID(szSsidFromItem);

            // Go over each available network list from stack and remove stale network item from list
            for (iNetwork = 0; iNetwork < m_pAvailableNetworkList->dwNumberOfItems; iNetwork++)
            {
                TCHAR szSsidFromStack[DOT11_SSID_MAX_LENGTH + 1];
                SsidToString(&((m_pAvailableNetworkList->Network[iNetwork]).dot11Ssid), szSsidFromStack, ARRAYSIZE(szSsidFromStack));
//                 if (_tcsncmp(szSsidFromItem, szSsidFromStack, (DOT11_SSID_MAX_LENGTH+1)) == 0)
				if (wcscmp(szSsidFromItem, szSsidFromStack) == 0)
                {
                    // Found network so no need to delete, just exit out of this for loop
                    break;
                }
            }

            // If we ran till the end of the list, then we did not find the item... check if it is stale and if so delete it, otherwise increment the missing count
//            if (iNetwork == m_pAvailableNetworkList->dwNumberOfItems)
//            {
//                RemoveEntryIfNeeded(iListNetwork, FALSE);
//                continue;
//            }
        }
    }


    return;
}

// Algorithm to move things around
//  - Dump the array which contains the index ranges of each category (see "DefaultToIndex" array defined in "common.h")
//  - Clear the new vector of type NetworkItem pointer which will be used to generate the newly sorted list
//  - Assign the old vector to the new vector
//      - This is done so that we can build the sorted list in place
//  - Just clear the old vector but not release the pointers
//  - Run through the new list and get indices accordinlgy and put the in the old cleaned vector
//  - Intentionally do not clear the new vector since the NetworkItem pointers from the new list is put in the old one
//  - Ask the UI main thread to do a reset
VOID WiFiNetworkList::SortNetworkList()
{

    // Dump the array which contains the index ranges of each category
    ZeroMemory(&m_dwNetworkIndicesArray, SORTING_INDICES_COUNT * sizeof(DWORD));

    // Clear the new vector
    for (UINT idx = 0; idx < m_rgNetworkItemsOnReset.size(); idx++)
    {
        //SAFERELEASE(m_rgNetworkItemsOnReset[idx]);
    }
    m_rgNetworkItemsOnReset.clear();

    // Assign the old vector to the new one
    m_rgNetworkItemsOnReset = m_rgNetworkItems;
    
    // Just clear the old vector but not release the pointers...
    m_rgNetworkItems.clear();

    // Go over each entry of the new list, and find the index on it, as you would in the beginning, and refill the old vector
    for (DWORD dwIndex=0; dwIndex<m_rgNetworkItemsOnReset.size(); dwIndex++)
    {
        // Make sure we have a valid pointer in each vector entry. If not, then just ignore that entry
        if (m_rgNetworkItemsOnReset[dwIndex] == NULL) continue;

        // Just set the connection state to be most accurate
        m_rgNetworkItemsOnReset[dwIndex]->UpdateConnectionState();

        // Now get the indices where this should really go to
        DWORD dwNewIndex = GetAndUpdateIndices(m_rgNetworkItemsOnReset[dwIndex]);
        m_rgNetworkItems.insert(m_rgNetworkItems.begin()+dwNewIndex, m_rgNetworkItemsOnReset[dwIndex]);
    }

    // Intentionally do not clear the new vector since the NetworkItem pointers from the new list is put in the old one...
    // But just clear the new vector of its entries...
    m_rgNetworkItemsOnReset.clear();

    return;
}

/// <summary>
/// Find the index for a specific bucket specified by the input flag
/// </summary>
VOID WiFiNetworkList::GetFromAndToIndexForFlag(DWORD dwFlag, DWORD *pdwFrom, DWORD *pdwTo)
{
    if (!pdwFrom || !pdwTo) return;

    DWORD dwIndex=0;

    // Map the flag value to index
    for (dwIndex=0; dwIndex<SORTING_INDICES_COUNT; dwIndex++)
    {
        if (DefaultToIndex[dwIndex] == dwFlag)
        {
            break;
        }
    }

    // Get the "from" and the "to" from the array accordingly
    *pdwFrom = m_dwNetworkIndicesArray[dwIndex];
    *pdwTo = m_dwNetworkIndicesArray[dwIndex+1];
    if (dwIndex == (SORTING_INDICES_COUNT-1))
    {
        *pdwTo = m_rgNetworkItems.size();
    }

    // Now adjust the indices below the index dwIndex
    IncrementIndicesBetween((dwIndex+1), (SORTING_INDICES_COUNT-1));

    return;
}

/// <summary>
/// Find the index where this network should go
/// The algorithm followed is the following:
/// </summary>
DWORD WiFiNetworkList::GetAndUpdateIndices(WiFiNetworkItem *pNetworkItem)
{
    DWORD dwIndex = 0;
    DWORD dwFlag = 0;

    ASSERT(pNetworkItem != NULL);
    // If this is an invalid network item, then return index as 0
    if (pNetworkItem == NULL)
    {
        return dwIndex;
    }

    PWLAN_AVAILABLE_NETWORK pNetworkData = pNetworkItem->GetStackNetworkData();
    BOOL fIsKnownNetwork = pNetworkItem->IsKnownNetwork();
//    DWORD dwAverageSignalStrength = pNetworkItem->GetAverageSignalStrength();

    // If it is a connected network, then the index is 0
    if (pNetworkItem->GetConnectionState() == CMCS_CONNECTED)
    {
        IncrementIndicesBetween(dwIndex+1, (SORTING_INDICES_COUNT-1));
        return dwIndex;
    }

    // Is this a known network?
    if (fIsKnownNetwork == TRUE)
    {
        dwFlag |= WIFI_UX_KNOWN_NETWORK;
    }
    else
    {
        dwFlag |= WIFI_UX_UNKNOWN_NETWORK;
    }

    // What signal strength category does this fall under?
    // The span is starting from the bar till 100, with a jump of delta each time. So I am using 3 and 2 below to calculate the gaps
//    if (dwAverageSignalStrength >= (g_dwSignalStrengthBar + 3*g_dwSignalStrengthDelta))
    {
//        dwFlag |= WIFI_UX_SIGNAL_QUALITY_VERY_HIGH;
    }
//    else if (dwAverageSignalStrength >= (g_dwSignalStrengthBar + 2*g_dwSignalStrengthDelta))
    {
        dwFlag |= WIFI_UX_SIGNAL_QUALITY_HIGH;
    }
//    else if (dwAverageSignalStrength >= (g_dwSignalStrengthBar + g_dwSignalStrengthDelta))
    {
//        dwFlag |= WIFI_UX_SIGNAL_QUALITY_LOW;
    }
//    else if (dwAverageSignalStrength >= (g_dwSignalStrengthBar))
    {
//        dwFlag |= WIFI_UX_SIGNAL_QUALITY_VERY_LOW;
    }
//    else
    {
        // We come here for known networks
//        dwFlag |= WIFI_UX_SIGNAL_QUALITY_VERY_LOW;
    }

    // What authentication category does this fall under?
    if (pNetworkData->bSecurityEnabled == TRUE)
    {
        dwFlag |= WIFI_UX_SECURITY_ENABLED;
    }
    else
    {
        dwFlag |= WIFI_UX_SECURITY_DISABLED;
    }

    // With the flags set now really find the network index
    DWORD dwFrom=0, dwTo=0;
    GetFromAndToIndexForFlag(dwFlag, &dwFrom, &dwTo);

    // Go from dwFrom to dwTo and find the index where this entry fits alphebatically
    ASSERT(dwFrom <= dwTo);
    // Get the SSID of the network
    TCHAR szSsidFromInput[DOT11_SSID_MAX_LENGTH + 1];
    TCHAR szSsidFromList[DOT11_SSID_MAX_LENGTH + 1];
    PWLAN_AVAILABLE_NETWORK pWiFiNetworkdata = NULL;

    if (FAILED(SsidToString(&(pNetworkData->dot11Ssid), szSsidFromInput, ARRAYSIZE(szSsidFromInput))))
    {
        return dwFrom;
    }

    for (dwIndex = dwFrom; dwIndex < dwTo; dwIndex++)
    {
        // Make sure we have a valid network item in the list, if not then jsut return with this index
        if (m_rgNetworkItems[dwIndex] == NULL) return dwIndex;

        // If the current element is a connected one, then just continue
        if (m_rgNetworkItems[dwIndex]->GetConnectionState() == CMCS_CONNECTED) continue;

        // Get the network data
        pWiFiNetworkdata = (m_rgNetworkItems[dwIndex])->GetStackNetworkData();
        // Make sure we have valid network data
        if (pWiFiNetworkdata == NULL) return dwIndex;

        // Get the average signal strength from the list item
//        DWORD dwListItemAverageSignalStrength = m_rgNetworkItems[dwIndex]->GetAverageSignalStrength();

        // Firstly check if the signal strength is greater than the one in the list. If so, then insert it here
//        if (dwAverageSignalStrength > dwListItemAverageSignalStrength)
        {
//            return dwIndex;
        }
        // but if both signal strengths are same, then insert in an alphebatical order
//        else if (dwAverageSignalStrength == dwListItemAverageSignalStrength)
        {
            if (SUCCEEDED(SsidToString(&(pWiFiNetworkdata->dot11Ssid), szSsidFromList, ARRAYSIZE(szSsidFromList))))
            {
                // compare the strings and if the the ssid from pNetworkData is smaller than the one at this index, then send this index
                INT iCmp = _tcsnicmp(szSsidFromList, szSsidFromInput, _tcslen(szSsidFromList));
                if (iCmp >= 0) return dwIndex;
            }
        }
        // else continue
    }

    return dwIndex;
}

DWORD WiFiNetworkList::GetInitialCount()
{
    return m_rgNetworkItems.size();
}

/// <summary>
/// Increment the relevant indices between from and to indices, including from and to
/// </summary>
VOID WiFiNetworkList::IncrementIndicesBetween(DWORD dwFromIndex, DWORD dwToIndex)
{
    ASSERT(dwToIndex < SORTING_INDICES_COUNT);

    // If the indices are screwed, return without doing anything
    if ((dwFromIndex > dwToIndex) || (dwToIndex >= SORTING_INDICES_COUNT))
    {
        return;
    }

    // Increment the index values between and [including at] the indices
    for (DWORD i=dwFromIndex; i<=dwToIndex; i++)
    {
        m_dwNetworkIndicesArray[i] = m_dwNetworkIndicesArray[i] + 1;
    }

    return;
}
HRESULT WiFiNetworkList::GetWiFiNetworkItem(TCHAR *pszSSID, WiFiNetworkItem** ppWiFiNetworkItem)
{
	DWORD dwItemIndex = FindNetworkItemByName(pszSSID); // FindNetworkNameByMangledName(szSsid);
	if (dwItemIndex < m_rgNetworkItems.size())
	{
		return GetWiFiNetworkItem(dwItemIndex,ppWiFiNetworkItem);
	}
	else
	{
		*ppWiFiNetworkItem=NULL;
		return S_FALSE;
	}
}
HRESULT WiFiNetworkList::GetWiFiNetworkItem(DWORD dwIndex, WiFiNetworkItem** ppWiFiNetworkItem)
{
    HRESULT hr = S_OK;
    WIFILIST_SYNCBLK(m_hMutex);

    CPREx(ppWiFiNetworkItem, E_FAIL);
    CBR(dwIndex<m_rgNetworkItems.size());
    *ppWiFiNetworkItem = m_rgNetworkItems[dwIndex];

Error:
    return hr;
}

HRESULT WiFiNetworkList::AppendCmConnections()
{
    HRESULT hr = S_OK;
    CM_RESULT result = CMRE_INSUFFICIENT_BUFFER;
    CM_CONNECTION_NAME_LIST* pCmConnections = NULL;
    DWORD dwSize = sizeof(CM_CONNECTION_NAME_LIST) * DEFAULT_CONNECTION_CONFIG_COUNT;
    WIFI_NETWORKDATA wifiConfigData;
    WLAN_AVAILABLE_NETWORK wifiNetworkData;
    UINT i;
    WiFiNetworkItem* pWiFiNetworkItem = NULL;

    for (i = 0; i < MAX_TRY_COUNT_FOR_INSUFFICIENT_BUFFER && result == CMRE_INSUFFICIENT_BUFFER; i++)
    {
        if (pCmConnections)
        {
            LocalFree(pCmConnections);
            pCmConnections = NULL;
        }

        pCmConnections = (CM_CONNECTION_NAME_LIST*)LocalAlloc(0, dwSize);
        result = CmEnumConnectionsConfigByType(CM_CSP_WIFI_TYPE, pCmConnections, &dwSize);
    }

    if (result != CMRE_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        DEBUGMSG(ZONE_ERROR,(TEXT("WiFiNetworkList::AppendCmConnections failed to enumerate CmConnections 0x%X\r\n"), hr));
        CHR(E_FAIL);
    }

    for (i = 0; i < pCmConnections->cConnection; i++)
    {
        CHR(GetWiFiConfigByConnectionName(pCmConnections->Connection[i].szName, &wifiConfigData));
        // ConvertConfigData2WlanData
        memcpy(&wifiNetworkData.dot11Ssid, &wifiConfigData.ssid, sizeof(DOT11_SSID));
        wifiNetworkData.dot11BssType=wifiConfigData.bsstype;
        wifiNetworkData.dot11DefaultAuthAlgorithm=wifiConfigData.authalgo;
        wifiNetworkData.dot11DefaultCipherAlgorithm=wifiConfigData.ciphalgo;
        wifiNetworkData.wlanSignalQuality = 0;
        pWiFiNetworkItem = new WiFiNetworkItem;
        CPR(pWiFiNetworkItem);
        pWiFiNetworkItem->Initialize(&wifiNetworkData, FALSE);
        // insert new created WiFiNetworkItem 
        CHR(InsertNewEntry(pWiFiNetworkItem, TRUE));
        pWiFiNetworkItem = NULL;
    }

Error:
    SAFE_DELETE(pWiFiNetworkItem);
    if (pCmConnections)
    {
        LocalFree(pCmConnections);
        pCmConnections = NULL;
    }
    return hr;
}

HRESULT WiFiNetworkList::SetWiFiListUiDoneUpdating(BOOL fUIDoneUpdating)
{
    m_fUIDoneUpdating = fUIDoneUpdating;
    return S_OK;
}

HRESULT WiFiNetworkList::SetWiFiNetUiCallbackWindowHandle(HANDLE* phWnd)
{
    HRESULT hr = S_OK;
    HKEY hKey = NULL;
    DWORD dwDisposition = 0;
    LPBYTE pbBuffer = NULL;

    CPREx(phWnd, E_INVALIDARG);

    if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NETUI_REGISTRY_KEY, 0, 0, &hKey) )
    {// key not exist, try creating it
        CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, NETUI_REGISTRY_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, 
                          &hKey, &dwDisposition));
    }
    
    if (hKey)
    {
        pbBuffer = (LPBYTE) (phWnd);
        CBR(RegSetValueEx(hKey, NETUI_CURRENT_WIFICPL_MAINWINDOW_KEY, 0, REG_DWORD, pbBuffer, sizeof(DWORD)) == ERROR_SUCCESS);
    }

Error:
    if (NULL != hKey)
    {
        RegCloseKey(hKey);
    }
    return hr;
}
HRESULT WiFiNetworkList::ClearWiFiNetUiCallbackWindowHandle()
{
    HRESULT hr = S_OK;
    HKEY hKey = NULL;

    CBR( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, NETUI_REGISTRY_KEY, 0, 0, &hKey) );
    if (hKey)
    {
        CBR(ERROR_SUCCESS == RegDeleteValue(hKey, NETUI_CURRENT_WIFICPL_MAINWINDOW_KEY));
    }

Error:
    if (NULL != hKey)
    {
        RegCloseKey(hKey);
    }
    return hr;
}

//////////////////////////////////////////////////////////////////
// WiFi Network Item
//////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------
    WiFiNetworkItem::WiFiNetworkItem

    Default constructor without any input.
------------------------------------------------------------------------------*/
WiFiNetworkItem::WiFiNetworkItem() :
                                m_pWiFiNetworkdata(NULL),
                                m_pConfig(NULL),
                                m_hWlanClient(NULL),
                                m_fguidInterfaceSet(FALSE),
                                m_hCMSession(NULL)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::WiFiNetworkItem\n"));
    m_fSharedWlanClient = FALSE;
    Initialize(NULL, FALSE);
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::WiFiNetworkItem\n"));
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::WiFiNetworkItem

    Constructor to initialize with valid inputs.
------------------------------------------------------------------------------*/
WiFiNetworkItem::WiFiNetworkItem(__in PWLAN_AVAILABLE_NETWORK pWiFiNetworkdata, HANDLE hWlanClient, GUID &guidInterface, CM_SESSION_HANDLE hCMSession) :
                                                                            m_pWiFiNetworkdata(NULL),
                                                                            m_pConfig(NULL),
                                                                            m_hWlanClient(hWlanClient),
                                                                            m_fguidInterfaceSet(TRUE),
                                                                            m_guidInterface(guidInterface),
                                                                            m_hCMSession(hCMSession)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::WiFiNetworkItem: With input parameters\n"));
    m_fSharedWlanClient = TRUE;
    Initialize(pWiFiNetworkdata, FALSE);
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::WiFiNetworkItem: With input parameters\n"));
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::~WiFiNetworkItem

    Destructor.
------------------------------------------------------------------------------*/
WiFiNetworkItem::~WiFiNetworkItem() {
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::~WiFiNetworkItem\n"));
    if (m_pConfig) LocalFree(m_pConfig);

    if (m_pWiFiNetworkdata)
    {
        delete m_pWiFiNetworkdata;
        m_pWiFiNetworkdata = NULL;
    }

    if (!m_fSharedWlanClient)
    {
        WlanCloseHandle(m_hWlanClient, NULL);
    }

    if (m_hEvent_FailedConnect)
    {
        CloseHandle(m_hEvent_FailedConnect);
        m_hEvent_FailedConnect = NULL;
    }

    if (m_hEvent_Connected)
    {
        CloseHandle(m_hEvent_Connected);
        m_hEvent_Connected = NULL;
    }

//    if (m_pProxySettings)
//    {
//        delete m_pProxySettings;
//        m_pProxySettings = NULL;
//    }
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::~WiFiNetworkItem\n"));
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::Initialize

    Initializes the netowrk item.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::Initialize(PWLAN_AVAILABLE_NETWORK pWiFiNetworkdata, BOOL fOverWrite)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::Initialize\n"));
    ZeroMemory(&m_InternalWiFiNetworkData, sizeof(WIFI_NETWORKDATA));
    ZeroMemory(&m_dwPastSignalStrengthReadings, sizeof(DWORD)*SIGNAL_STRENGTH_AVERAGE_COUNT);

    if (pWiFiNetworkdata)
    {
        m_pWiFiNetworkdata = new WLAN_AVAILABLE_NETWORK;
        // If we cannot allocate memory, then bail...
        if (m_pWiFiNetworkdata == NULL) return;
        memcpy(m_pWiFiNetworkdata, pWiFiNetworkdata, sizeof(WLAN_AVAILABLE_NETWORK));
        
        for (WORD i=0; i<SIGNAL_STRENGTH_AVERAGE_COUNT; i++)
        {
            m_dwPastSignalStrengthReadings[i] = m_pWiFiNetworkdata->wlanSignalQuality;
        }
        m_dwSignalStrengthReadingsSum = m_pWiFiNetworkdata->wlanSignalQuality * SIGNAL_STRENGTH_AVERAGE_COUNT;
    }

    InitializeWlanClientAndInterface();

    ConfigureInternalWiFiNetworkData(fOverWrite);

    // Set the connection state during creation
    UpdateConnectionState();

    m_hEvent_Connected = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hEvent_FailedConnect = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hAvailability = CreateEvent(NULL, FALSE, FALSE, NULL);

    m_hConnection = NULL;

    // TODO:
    // Read the stat store to get any error reason, if cached for this network
    // The reasoning behind this is that if CMListener tried to auto connect
    // and failed, it will cache the error state for a particular network it tried to
    // connect last. Reflecting on that, the system tray will display limited
    // connectivity or failed connectivity. Showing a red bang depending on this
    // will help the user act on this particular network. if they chose to.

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::Initialize\n"));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::InitializeWlanClientAndInterface

    If not initialized yet, this method interacts with the
    Wi-Fi stack and initialized the client and the interface.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::InitializeWlanClientAndInterface()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::InitializeWlanClientAndInterface\n"));
    DWORD dwError, dwNegotiatedVersion;
    PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;
    HRESULT hr = S_OK;

    if (!m_hWlanClient)
    {
        dwError = WlanOpenHandle(CSP_WIFI_SPECIFIC_INFO_VERSION, NULL, &dwNegotiatedVersion, &m_hWlanClient);
        m_fSharedWlanClient = FALSE;
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::InitializeWlanClientAndInterface: WlanOpenHandle return value = 0x%X\n", dwError));
        CBR(ERROR_SUCCESS == dwError);
    }

    if (!m_fguidInterfaceSet)
    {
        dwError = WlanEnumInterfaces(m_hWlanClient, NULL, &pInterfaceList);
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::InitializeWlanClientAndInterface: WlanEnumInterfaces return value = 0x%X\n", dwError));
        CBR(ERROR_SUCCESS == dwError);
        
        // We only support up to one interface for now
        ASSERT(pInterfaceList->dwNumberOfItems <= 1);
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::InitializeWlanClientAndInterface: Number of interface list items = %d\n", pInterfaceList->dwNumberOfItems));
        CBR (1 <= pInterfaceList->dwNumberOfItems);
        
        m_guidInterface = pInterfaceList->InterfaceInfo[0].InterfaceGuid;
        m_fguidInterfaceSet = TRUE;
    }

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::InitializeWlanClientAndInterface: HRESULT = 0x%X\n", hr));
    if (pInterfaceList)
    {
        WlanFreeMemory(pInterfaceList);
    }
    return;
}

VOID WiFiNetworkItem::InitializeCMsession(CM_SESSION_HANDLE hCMSession)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::InitializeCMsession\n"));

    if (!m_hCMSession)
    {
        m_hCMSession = hCMSession;
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::InitializeCMsession:\n"));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::ConfigureInternalWiFiNetworkData

    Configures the internar Wi-Fi network data from the
    network data cached while creation of the network item.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::ConfigureInternalWiFiNetworkData(BOOL fOverWrite)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::ConfigureInternalWiFiNetworkData\n"));

    // Get the config from CM, if available
    HRESULT hr = S_OK;
    TCHAR szSsid[DOT11_SSID_MAX_LENGTH + 1];

    // If we have an invalid network data, then bail
    CBR(m_pWiFiNetworkdata != NULL);

    // Set the initial set of variables
    m_InternalWiFiNetworkData.fAvailable = TRUE;
    m_InternalWiFiNetworkData.fExistingConfig = FALSE;
    m_InternalWiFiNetworkData.pbAuthData = NULL;
    m_InternalWiFiNetworkData.dwAuthDataLen = 0;
    m_InternalWiFiNetworkData.pbCredBlob = NULL;
    m_InternalWiFiNetworkData.dwCredBlobLength = 0;
    m_InternalWiFiNetworkData.dwFlags = WIFI_NDF_VALID_AVAILABLE;

    // We know this is an airing network by design so it must be BROADCAST type, thus not hidden
    m_InternalWiFiNetworkData.fHidden = FALSE;

    // Get the network name from SSID
    hr = SsidToString(&(m_pWiFiNetworkdata->dot11Ssid), szSsid, ARRAYSIZE(szSsid));
    CHR(hr);

    // Get existing network data
    CM_RESULT cmr;
    DWORD cbConfig = 1;

    // Make sure that the previous allocation is released.
    if (m_pConfig)
    {
        LocalFree(m_pConfig);
        m_pConfig = NULL;
    }
    m_pConfig = (CM_CONNECTION_CONFIG *)LocalAlloc(LMEM_FIXED, cbConfig);
    CPR(m_pConfig);

    cmr = CmGetConnectionConfig(szSsid, m_pConfig, &cbConfig);
    while (CMRE_INSUFFICIENT_BUFFER == cmr)
    {
        m_pConfig = (CM_CONNECTION_CONFIG *)LocalReAlloc(m_pConfig, cbConfig, LMEM_MOVEABLE);
        CPR(m_pConfig);

        cmr = CmGetConnectionConfig(szSsid, m_pConfig, &cbConfig);
    }

    // If it is not a known network, then continue and set relevant details
    if ((CMRE_SUCCESS != cmr) || (fOverWrite))
    {
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::ConfigureInternalWiFiNetworkData: Network %s is not known to the stack\n", szSsid));
        if (m_pConfig)
        {
            LocalFree(m_pConfig);
            m_pConfig = NULL;
        }

        // Check if 1x is enabled
        switch (m_pWiFiNetworkdata->dot11DefaultAuthAlgorithm)
        {
            case DOT11_AUTH_ALGORITHM_RSNA:
            case DOT11_AUTH_ALGORITHM_WPA:
                // The only "for sure" 1x enabled networks are enterprise networks
                m_InternalWiFiNetworkData.fOneXEnabled = TRUE;
                m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
                break;

            case DOT11_AUTH_ALGO_80211_OPEN:
                if ((DOT11_CIPHER_ALGO_WEP == m_pWiFiNetworkdata->dot11DefaultCipherAlgorithm) &&
                    !(dot11_BSS_type_independent == m_pWiFiNetworkdata->dot11BssType))
                {
                    // This could either be Wep-Open or Wep-Shared which are not 1x enabled or
                    // it could be 80211X which is 1x enabled.
                    // Since we don't know for sure we should leave it ambiguous.
                    m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_INVALID_ONEXENABLED;
                }
                else
                {
                    // Everything else isn't 1x enabled
                    m_InternalWiFiNetworkData.fOneXEnabled = FALSE;
                    m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
                }
                break;
                
            default:
                // Everything else isn't 1x enabled
                m_InternalWiFiNetworkData.fOneXEnabled = FALSE;
                m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
                break;
        }

        m_InternalWiFiNetworkData.authalgo = m_pWiFiNetworkdata->dot11DefaultAuthAlgorithm;
        m_InternalWiFiNetworkData.bsstype = m_pWiFiNetworkdata->dot11BssType;
        m_InternalWiFiNetworkData.ciphalgo = m_pWiFiNetworkdata->dot11DefaultCipherAlgorithm;
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_AUTHALGO | WIFI_NDF_VALID_BSSTYPE | WIFI_NDF_VALID_CIPHALGO;

        memcpy(&(m_InternalWiFiNetworkData.ssid), &(m_pWiFiNetworkdata->dot11Ssid), sizeof(m_InternalWiFiNetworkData.ssid));
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_SSID;

        CHR(SsidToString(&(m_InternalWiFiNetworkData.ssid), m_InternalWiFiNetworkData.szSsid, ARRAYSIZE(m_InternalWiFiNetworkData.szSsid)));
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_SZSSID;

        m_InternalWiFiNetworkData.guidInterface = m_guidInterface;
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_GUIDINTERFACE;
        m_InternalWiFiNetworkData.fExistingConfig = FALSE;
        m_InternalWiFiNetworkData.dwFlags &= ~WIFI_NDF_VALID_EXISTINGCONFIG;
    }
    // This is a known network
    else
    {
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::ConfigureInternalWiFiNetworkData: Network %s is known to the stack\n", szSsid));
        // Update WiFi network data with data from existing network data
        m_InternalWiFiNetworkData.fExistingConfig = TRUE;
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_EXISTINGCONFIG;

        // UpdateWiFiNetworkDataFromExistingConfig
        CHR(GetWiFiConfigByConnectionName(szSsid, &m_InternalWiFiNetworkData));
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_AUTHALGO | WIFI_NDF_VALID_BSSTYPE | WIFI_NDF_VALID_CIPHALGO;
        CHR(SsidToString(&(m_InternalWiFiNetworkData.ssid), m_InternalWiFiNetworkData.szSsid, ARRAYSIZE(m_InternalWiFiNetworkData.szSsid)));
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_SZSSID;
        m_InternalWiFiNetworkData.dwFlags |= ((m_InternalWiFiNetworkData.fOneXEnabled) ? WIFI_NDF_VALID_ONEXENABLED : WIFI_NDF_INVALID_ONEXENABLED);
        m_InternalWiFiNetworkData.guidInterface = m_guidInterface;
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_GUIDINTERFACE;
    }

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::ConfigureInternalWiFiNetworkData: HRESULT = 0x%X\n", hr));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::GetStackNetworkData

    Just return the stack network data cached in the object.
------------------------------------------------------------------------------*/
PWLAN_AVAILABLE_NETWORK WiFiNetworkItem::GetStackNetworkData()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::GetStackNetworkData\n"));
    PWLAN_AVAILABLE_NETWORK pWiFiNetworkData;
    pWiFiNetworkData = m_pWiFiNetworkdata;
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::GetStackNetworkData\n"));
    return pWiFiNetworkData;
}
bool WiFiNetworkItem::Disconnect()
{
	RETAILMSG(1, (L"[WiFiUX] ++WiFiNetworkItem::Disconnect: Parameter-less connect\n"));
	HRESULT hr = S_OK;
	CM_RESULT cmr;

	// If it is already connected, then don't do anything. notice that we do not make hr failure intentionally
	if (GetConnectionState() != CMCS_CONNECTED) goto Error;

	if (m_hConnection)
	{
		cmr = CmReleaseConnection(m_hConnection);
		if (cmr != CMRE_SUCCESS) 
			hr=S_FALSE;
	}

	m_hConnection = NULL;

Error:
	RETAILMSG(1, (L"[WiFiUX] --WiFiNetworkItem::Disconnect: Parameter-less connect - HRESULT = 0x%X\n", hr));
	return SUCCEEDED(hr);
}
/*------------------------------------------------------------------------------
    WiFiNetworkItem::Connect

    Parameter-less connect.
    This method [re]configures the network and calls relevant
    method to acquire the connection via CM on this network.
------------------------------------------------------------------------------*/
bool WiFiNetworkItem::Connect(BOOL bConfig)
{
   RETAILMSG(1,, (L"[WiFiUX] ++WiFiNetworkItem::Connect: Parameter-less connect\n"));
    HRESULT hr = S_OK;

    // If it is already connected, then don't do anything. notice that we do not make hr failure intentionally
    if (GetConnectionState() == CMCS_CONNECTED) goto Error;

	if(bConfig)
	{
    // Only if this is an unknown network, configure it
//     if (FALSE == m_InternalWiFiNetworkData.fExistingConfig)
    {
		RETAILMSG(1, (L"[WiFiUX] ++WiFiNetworkItem::Connect: ConfigureWiFiNetworkProfile\n"));

        CHR(ConfigureWiFiNetworkProfile(NULL, NULL, NULL, NULL, NULL));
    }

    // If we are here, then we should mark this dude as a known network
    m_InternalWiFiNetworkData.fExistingConfig = TRUE;
	}

//    if (IsAvailableNetwork(&m_InternalWiFiNetworkData))
    if ((WIFI_NDF_VALID_AVAILABLE & m_InternalWiFiNetworkData.dwFlags) && (m_InternalWiFiNetworkData.fAvailable))
    {
        CHR(ConnectWiFiNetwork());
    }

Error:
    RETAILMSG(1, (L"[WiFiUX] --WiFiNetworkItem::Connect: Parameter-less connect - HRESULT = 0x%X\n", hr));
    return SUCCEEDED(hr);
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::Connect

    Connect with passkey.
    This method [re]configures the network and calls relevant
    method to acquire the connection via CM on this network.
------------------------------------------------------------------------------*/
bool WiFiNetworkItem::Connect(PCWSTR pszPasskey, BOOL fHidden)
{
	RETAILMSG(1, (L"[WiFiUX] ++WiFiNetworkItem::Connect: Connect with passkey:%s\n"),pszPasskey);
    HRESULT hr = S_OK;
    size_t secKeyLen = 0;

    if (pszPasskey != NULL)
    {   // Cache the passkey and ensure passphrase flag is enabled
        wcsncpy_s(m_InternalWiFiNetworkData.szPassphrase, PWLEN, pszPasskey, _tcslen(pszPasskey));
        m_InternalWiFiNetworkData.dwFlags &= ~WIFI_NDF_VALID_ENCRYPTEDKEY;
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_KEY;
        CHR(StringCchLength(pszPasskey, MAX_PATH, &secKeyLen));
        CHR(CheckWepKey(&m_InternalWiFiNetworkData, pszPasskey, secKeyLen));
    }
	else
	{
	}

    if (fHidden)
    {
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_HIDDEN;
    }

    CBR(Connect());

Error:
    RETAILMSG(1, (L"[WiFiUX] --WiFiNetworkItem::Connect: Connect with passkey: HRESULT = 0x%X\n", hr));
    return SUCCEEDED(hr);
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::Connect

    Connect with username and password.
    This method [re]configures the network and calls relevant
    method to acquire the connection via CM on this network.
------------------------------------------------------------------------------*/
bool WiFiNetworkItem::Connect(PCWSTR pszUsername, PCWSTR pszPassword, BOOL fHidden)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::Connect: Connect with username and password: Username = %s\n", pszUsername));
    HRESULT hr = S_OK;

    // For an invalid username or password, bail
    CBR(pszPassword != NULL);
    CBR(pszUsername != NULL);    

    // Cache the username coming in from the UX
    // Notice that this could be a concatination of username and domainname
    // So if it WERE to be DN/UN or UN@DN, then we would have UNLEN + 1 (for @ or /) + DNLEN + 1 (string ender)
    TCHAR szUNDN[(UNLEN + 1 + DNLEN + 1)];
    ZeroMemory(&szUNDN, ((UNLEN + 1 + DNLEN + 1)*sizeof(TCHAR)));

    // Make a copy from the input
    wcsncpy_s(szUNDN, ((UNLEN) + 1 + (DNLEN)), pszUsername, _tcslen(pszUsername));

    // Make sure we send a valid string
    szUNDN[(UNLEN + DNLEN + 1)] = L'\0';

    // Break the UN and DN accordingly. If there were failures, then bail... no point in connecting
    CHR(SetUsernameAndDomain(szUNDN));

    // Cache the passkey and set the relevant flag for UN/DN - which is WIFI_NDF_VALID_EAPTYPE
    wcsncpy_s(m_InternalWiFiNetworkData.szPassphrase, PWLEN, pszPassword, _tcslen(pszPassword));
    m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_EAPTYPE;

    if (fHidden)
    {
        m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_HIDDEN;
    }

    // Now call connect on the configuration we have
    CBR(Connect());

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::Connect: Connect with username and password: HRSULT = 0x%X\n", hr));
    return SUCCEEDED(hr);
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::SetUsernameAndDomain

    This breaks the username entered into UN and DN
    and caches the values locally.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::SetUsernameAndDomain(TCHAR *pszUNDN)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::SetUsernameAndDomain: Input = %s\n", pszUNDN));
    HRESULT hr = S_OK;
    ce::tstring strDomain, strUsername;

    // Validate the input first
    CBRA(pszUNDN != NULL);

    // Assert that this is 1x network
    ASSERT(m_InternalWiFiNetworkData.fOneXEnabled == TRUE);
    CBR(m_InternalWiFiNetworkData.fOneXEnabled == TRUE);

    // Get the last occurance of the special character ' / ' or ' \ ' or ' @ ' till UNLEN+DNLEN+2 characters or  EOS
    // First preference comes for the special character ' @ '
    TCHAR *pszSubString = _tcsrchr(pszUNDN, L'@');
    // If we found an occurance, then whatever is after this character is DN and what is before is UN
    if (pszSubString)
    {
        ASSERT(pszSubString >= pszUNDN);
        CBR(pszSubString >= pszUNDN);
        WORD wUNLength = (WORD)(pszSubString - pszUNDN);
        CBR(wUNLength <= UNLEN);

        strUsername.assign(pszUNDN, wUNLength);
        strDomain.assign(pszSubString+1, DNLEN);
    }
    else
    {
        // If we come till here, then we did not find ' @ ', so look for ' / ' or ' \ '
        pszSubString = _tcsrchr(pszUNDN, L'/');
        // If this character was not found, then look for ' \ ' immediately since handling both cases is exactly the same
        if (pszSubString == NULL)
        {
            pszSubString = _tcsrchr(pszUNDN, L'\\');
        }
        // If we found an occurance, then whatever is after this character is UN and what is before is DN
        if (pszSubString)
        {
            ASSERT(pszSubString >= pszUNDN);
            CBR(pszSubString >= pszUNDN);
            WORD wDNLength = (WORD)(pszSubString - pszUNDN);
            CBR(wDNLength <= DNLEN);

            strDomain.assign(pszUNDN, wDNLength);
            strUsername.assign(pszSubString+1, UNLEN);
        }
        else
        {
            // If we came till here, then we have no domain name
            // We do not care about domain info, so just copy the entire string as the UN
            strUsername.assign(pszUNDN, UNLEN);
        }
    }

    // Everything looks fine so far, cache the DN
    CHR(StringCchCopy(m_InternalWiFiNetworkData.szDomain, ARRAYSIZE(m_InternalWiFiNetworkData.szDomain), strDomain));
    // Now cache the UN
    CHR(StringCchCopy(m_InternalWiFiNetworkData.szUsername, ARRAYSIZE(m_InternalWiFiNetworkData.szUsername), strUsername));

    DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::SetUsernameAndDomain: UN = %s; DN = %s\n", m_InternalWiFiNetworkData.szUsername, m_InternalWiFiNetworkData.szDomain));

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::SetUsernameAndDomain: HRESULT = 0x%X\n", hr));
    return hr;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::WaitForAvailability

    Basically this method waits for the event which represents the
    network state to not be in UNAVAILABLE or DISABLED. This event
    is set either by the list object when CM gives it the state
    change notificaiton that the network moved away from one of these
    states, or if the network is already not in this state, then
    the connect method internally sets this event right away.
------------------------------------------------------------------------------*/
DWORD WiFiNetworkItem::WaitForAvailability()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::WaitForAvailability\n"));
    DWORD dwWaitError = ERROR_SUCCESS;

    // If not in connecting state, then we need not do anything
    if (m_fConnecting == FALSE) return dwWaitError;

    dwWaitError = WaitForSingleObject(m_hAvailability, WIFI_WAIT_FOR_CONFIG_CHANGE);
    if (WAIT_OBJECT_0 == dwWaitError)
    {
        DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::WaitForAvailability: Received event that this network is connectable\n"));
        dwWaitError = ERROR_SUCCESS;
    }
    else if (WAIT_TIMEOUT == dwWaitError)
    {
        // Waiting for more than WIFI_WAIT_FOR_CONFIG_CHANGE is ridiculous...
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::WaitForAvailability: Timed out, timeout = %d\n", WIFI_WAIT_FOR_CONFIG_CHANGE));
        dwWaitError = ERROR_TIMEOUT;
        ASSERT(FALSE);
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::WaitForAvailability - Return value = 0x%X\n", dwWaitError));
    return dwWaitError;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::SetAvailable

    Basically this method sets the event which represents the
    network state to not be in UNAVAILABLE or DISABLED. This event
    is set either by the list object when CM gives it the state
    change notificaiton that the network moved away from one of these
    states, or if the network is already not in this state, then
    the connect method internally sets this event right away.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::SetAvailable()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::SetAvailable\n"));
    // If not in connecting state, then we need not do anything
    if (m_fConnecting == FALSE) 
    {
        DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::SetAvailable: Not in connecting mode\n"));
        return;
    }

    SetEvent(m_hAvailability);
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::SetAvailable\n"));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::RemoveConfiguration

    This method calls into CM to remove the configuration.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::RemoveConfiguration()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::RemoveConfiguration\n"));
    // Best effort. Don't check for anything
    if (m_InternalWiFiNetworkData.szSsid)
    {
        DEBUGMSG(ZONE_INFO, (L"[WiFiUX] WiFiNetworkItem::RemoveConfiguration: Network item = %s\n", m_InternalWiFiNetworkData.szSsid));
        CmDeleteConnectionConfig(m_InternalWiFiNetworkData.szSsid);
    }

    // Mark this guys as an unknown network
    m_InternalWiFiNetworkData.fExistingConfig = FALSE;

    SetConnectionState(CMCS_DISCONNECTED);

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::RemoveConfiguration\n"));
    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::ConnectWiFiNetwork

    This creates a thread which in turn calls CM APIs to connect.
    This method also checks for the connection state of the network
    and immediately unblocks the acquire connection thread, if the
    network is not in unavailable or disabled state. This way we
    prevent acquire thread not waiting too long, if we don't have to.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::ConnectWiFiNetwork()
{
    RETAILMSG(1, ((L"[WiFiUX] ++WiFiNetworkItem::ConnectWiFiNetwork,%s\n"),m_InternalWiFiNetworkData.szSsid));
    HRESULT hr = S_OK;
    HANDLE hCMAcquireConnectionThread = NULL;

    CBR(m_hCMSession != NULL);

    // Set this item to be as connecting, so that we can wait till this connection becomes available for acquiring in the other thread
    m_fConnecting = TRUE;

    hCMAcquireConnectionThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)s_CMAcquireConnectionThread, this, 0, NULL);
    CBR((hCMAcquireConnectionThread != NULL));

    // If the network is already in the connected mode, then set the event for availability, this way we don't wait for WIFI_WAIT_FOR_CONFIG_CHANGE seconds
    UpdateConnectionState();
    CM_CONNECTION_STATE cmcs = GetConnectionState();
    DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::ConnectWiFiNetwork: Conneciton state = %d\n", cmcs));
    if ((cmcs != CMCS_DISCONNECTED_UNAVAILABLE) && (cmcs != CMCS_DISCONNECTED_DISABLED))
    {
        SetAvailable();
    }

Error:
    if (hCMAcquireConnectionThread) CloseHandle(hCMAcquireConnectionThread);
    RETAILMSG(1, (L"[WiFiUX] --WiFiNetworkItem::ConnectWiFiNetwork: HRESULT = 0x%X\n", hr));
    return hr;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::s_CMAcquireConnectionThread

    Spins a thread to initiate a CmAcquireConnection for the selected network.
------------------------------------------------------------------------------*/
DWORD WiFiNetworkItem::s_CMAcquireConnectionThread(WiFiNetworkItem *pNetworkItem)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::s_CMAcquireConnectionThread\n"));
    if (NULL == pNetworkItem)
    {
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::s_CMAcquireConnectionThread: Invalid input network item\n"));
        return (DWORD)-1;
    }

    // If we did get the right event, then acquire connection
    pNetworkItem->CMAcquireConnectionThread();

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::s_CMAcquireConnectionThread\n"));
    return (DWORD)0;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::CMAcquireConnectionThread

    This thread basically waits for an event indicating that
    the network "might" be available to be connected to.
    Once this happens, it runs throught he candidate connections
    in CM, and gets a handle to this network's connection.
    After this, it calls CmAcquireConnection to get the
    connection.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::CMAcquireConnectionThread()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::CMAcquireConnectionThread\n"));

    HRESULT hr = E_FAIL;
    CM_RESULT cmr = CMRE_SUCCESS;

    if (m_hConnection)
    {
        //cmr = CmReleaseConnection(m_hConnection);
    }

    // Even before doing anything, just wait for the configuration change event
    // This is going to be a best effort. If we wait and timeout, even then we 
    // try to go and find-acquire connection...
    WaitForAvailability();

    m_hConnection = NULL;

    // Go over all candidate connections.
    // Get a handle to the one we are interested and
    // If there is a connection which is in connected mode, then release that connection
    for (cmr = CmGetFirstCandidateConnection(m_hCMSession, NULL, CMSO_DEFAULT, &m_hConnection);
            cmr == CMRE_SUCCESS;
            cmr = CmGetNextCandidateConnection(m_hCMSession, &m_hConnection))
    {
        CM_CONNECTION_DETAILS* pDetails = NULL;
        DWORD cbDetails = sizeof(DWORD);

        for (int i = 0; i < MAX_TRY_COUNT_FOR_INSUFFICIENT_BUFFER; i += 1)
        {
            pDetails = reinterpret_cast<CM_CONNECTION_DETAILS*>(new BYTE[cbDetails]);
            cmr = CmGetConnectionDetailsByHandle(m_hConnection, pDetails, &cbDetails);

            if (cmr == CMRE_SUCCESS) break;

            // Prepare for next iteration.
            if (pDetails != NULL)
            {
                delete [] pDetails;
                pDetails = NULL;
            }
        }

        CBR(pDetails != NULL);

        // If this is not a Wi-Fi type, then ignore...
        if (pDetails->Device != CMCD_WIFI)
        {
            // Go for the next iteration.
            if (pDetails != NULL)
            {
                delete [] pDetails;
                pDetails = NULL;
            }
            continue;
        }

        // Check if this is the connection we are interested in
        if ((m_InternalWiFiNetworkData.szSsid != NULL) && (_tcscmp(pDetails->szName, m_InternalWiFiNetworkData.szSsid) == 0))
        {
            // If it is then acquire and then immediately release it. This way CM will use this connection going forward
            DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::CMAcquireConnectionThread: Found network %s to be connectable\n", pDetails->szName));
            cmr = CmAcquireConnection(m_hConnection);
            if (cmr == CMRE_SUCCESS)
            {
                DEBUGMSG(ZONE_INFO|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::CMAcquireConnectionThread: Calling release on CM for network %s, immediately, to cleanup any dangling handles\n", pDetails->szName));
                //cmr = CmReleaseConnection(hConnection);
            }

            // Immaterial of this call failed/succeeded, cleanup and exit this loop
            if (pDetails)
            {
                delete [] pDetails;
                pDetails = NULL;
            }

            break;
        }

        // Cleanup for the next connection
        if (pDetails)
        {
            delete [] pDetails;
            pDetails = NULL;
        }
    }

    hr = S_OK;

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::CMAcquireConnectionThread: HRESULT = 0x%X\n", hr));

    // Set this item back to not connecting
    m_fConnecting = FALSE;

    return hr;
}
/*------------------------------------------------------------------------------
    WiFiNetworkItem::GetConnectionState

    Get the cached connection state of the network item.
------------------------------------------------------------------------------*/
CM_CONNECTION_STATE WiFiNetworkItem::GetConnectionState()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::GetConnectionState\n"));
    CM_CONNECTION_STATE cmcs;
    cmcs = m_CMConnectionState;
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::GetConnectionState: Connection state for %s is %d\n", m_InternalWiFiNetworkData.szSsid, cmcs));
    return cmcs;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::UpdateConnectionState

    Talk with CM and cache the connection state into member variable.
    This method does not call into notifying anyone .
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::UpdateConnectionState()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::UpdateConnectionState\n"));

    HRESULT                 hr                  = S_OK;
    CM_RESULT               cmRes               = CMRE_SUCCESS;
    CM_CONNECTION_DETAILS*  pConnDetails        = NULL;
    DWORD                   cbConnDetails       = sizeof(CM_CONNECTION_DETAILS);
    m_CMConnectionState                            = CMCS_DISCONNECTED;

    pConnDetails = (CM_CONNECTION_DETAILS*)LocalAlloc(LPTR, cbConnDetails);
    CPR(pConnDetails);

    //get connection details per connection
    cmRes = CmGetConnectionDetailsByName(m_InternalWiFiNetworkData.szSsid, pConnDetails, &cbConnDetails);

    while (cmRes == CMRE_INSUFFICIENT_BUFFER)
    {
        LocalFree(pConnDetails);

        pConnDetails = (CM_CONNECTION_DETAILS*) LocalAlloc(LPTR, cbConnDetails);
        CPR(pConnDetails);

        pConnDetails->Version = CM_CURRENT_VERSION;
        
        cmRes = CmGetConnectionDetailsByName(m_InternalWiFiNetworkData.szSsid, pConnDetails, &cbConnDetails);
    }

    CBR(cmRes == CMRE_SUCCESS);

    CBR(pConnDetails->Device == CMCD_WIFI);

    m_CMConnectionState = pConnDetails->State;

Error:
    if (pConnDetails)
    {
        LocalFree(pConnDetails);
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::UpdateConnectionState: Connection state = %d; HRESULT = 0x%X\n", m_CMConnectionState, hr));
    return hr;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::SetConnectionState

    Cache the input connection state into member variable.
    This calls into notifying any (can I say UI in model!? :)) listeners
    for this update.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::SetConnectionState(CM_CONNECTION_STATE cmcsNetwork)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::SetConnectionState with CMCS input: Connection state = %d\n", cmcsNetwork));

    m_CMConnectionState = cmcsNetwork;

    if (CMCS_CONNECTED==m_CMConnectionState)
    {
        SetEvent(m_hEvent_Connected);
    }

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::SetConnectionState with CMCS input\n"));
    return;
}

BOOL WiFiNetworkItem::IsConnected(DWORD dwTimeOut)
{
    DWORD dwTimeOutMS=INFINITE;
    HANDLE hEvents[2];
    DWORD dwWaitResult;
    hEvents[0] = m_hEvent_Connected;
    hEvents[1] = m_hEvent_FailedConnect;

    if (dwTimeOut)
    {
        dwTimeOutMS = dwTimeOut;
    }

    do
    {
        dwWaitResult = WaitForMultipleObjects(2/*nCount*/, hEvents, FALSE/*bWaitAll*/, dwTimeOutMS);
        if (WAIT_OBJECT_0 == dwWaitResult)
        {
            ResetEvent(m_hEvent_Connected);
            dwTimeOutMS = 1000;
        }
        else if (WAIT_OBJECT_0 + 1 == dwWaitResult)
        {
            ResetEvent(m_hEvent_FailedConnect);
            dwTimeOutMS = 1000;
        }
        else
        {
            dwTimeOutMS >>= 1;
        }
    } while (TRUE==m_fConnecting);

    return (CMCS_CONNECTED==m_CMConnectionState);
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::GetSzSSID

    If we have a valid input, then copy the cached SSID and return back
    otherwise just return back without doing anything.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::GetSzSSID(TCHAR *pszSsid)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::GetSzSSID\n"));

    if (pszSsid == 0) 
    {
        DEBUGMSG(ZONE_ERROR|ZONE_VERBOSE, (L"[WiFiUX] WiFiNetworkItem::GetSzSSID: Invalid input\n"));
        goto Error;
    }

    // Get the szSSID from the network data
    SsidToString(&(m_pWiFiNetworkdata->dot11Ssid), pszSsid, (DOT11_SSID_MAX_LENGTH + 1));

Error:
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::GetSzSSID\n"));

    return;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::ConfigureWiFiNetworkProfile

    This method configures the network.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::ConfigureWiFiNetworkProfile(
    BOOL* pfSecTypeOneX, 
    BSTR pszKey, 
    BYTE bEapType, 
    LPCTSTR pszCertName, 
    BOOL* pfValidateServer
)
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::ConfigureWiFiNetworkProfile\n"));
    HRESULT hr = S_OK;

    UpdateInternalWiFiNetworkData(pfSecTypeOneX, pszKey, bEapType, pszCertName, pfValidateServer);
    CHR(ConfigureWiFiNetwork(&m_InternalWiFiNetworkData));

Error:
    if (FAILED(hr))
    {
        RemoveConfiguration();
    }
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::ConfigureWiFiNetworkProfile: HRESULT = 0x%X\n", hr));
    return hr;
}

HRESULT WiFiNetworkItem::UpdateInternalWiFiNetworkData(
    BOOL* pfSecTypeOneX, 
    BSTR pszKey, 
    BYTE bEapType, 
    LPCTSTR pszCertName, 
    BOOL* pfValidateServer
)
{
    HRESULT hr = S_OK;
    size_t secKeyLen;

    if (pfSecTypeOneX)
    {
        if (*pfSecTypeOneX)
        {
            m_InternalWiFiNetworkData.dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
            m_InternalWiFiNetworkData.fOneXEnabled = TRUE;
        }
    }
    if (pszKey)
    {
        CHR(StringCchLength(pszKey, MAX_PATH, &secKeyLen));
        if (secKeyLen > 0)
        {
            wcsncpy_s(m_InternalWiFiNetworkData.szPassphrase, PWLEN, pszKey, _tcslen(pszKey));
            CHR(CheckWepKey(&m_InternalWiFiNetworkData, pszKey, secKeyLen));
        }
    }
    if (bEapType)
    {
        m_InternalWiFiNetworkData.eapmethodtype.eapType.type = bEapType;
        if (pszCertName || pfValidateServer)
        {
            CHR(InsertAuthDataByCertName(&m_InternalWiFiNetworkData, pszCertName, pfValidateServer));
        }
    }

Error:
    return hr;
}
/*------------------------------------------------------------------------------
    WiFiNetworkItem::GetNetworkNameText

    Gets called by the Splash render redirector for list items.
    Returning relavent propery string, looked up by ID.
------------------------------------------------------------------------------*/
HRESULT WiFiNetworkItem::GetNetworkNameText(LPTSTR* ppWiFiNetworkName)
{
    HRESULT hr = S_OK;

    CPREx(ppWiFiNetworkName, E_INVALIDARG);
    CBR( (m_InternalWiFiNetworkData.dwFlags & (WIFI_NDF_VALID_SSID|WIFI_NDF_VALID_SZSSID)) );
    *ppWiFiNetworkName = m_InternalWiFiNetworkData.szSsid;

Error:
    return hr;
}


/*------------------------------------------------------------------------------
    WiFiNetworkItem::NotifyFailure

    Notify the connection thread the connection was failed.
------------------------------------------------------------------------------*/
VOID WiFiNetworkItem::NotifyFailure()
{
    SetEvent(m_hEvent_FailedConnect);
}
/*------------------------------------------------------------------------------
    WiFiNetworkItem::IsKnownNetwork

    Look for cached value to determine if this is a known network.
------------------------------------------------------------------------------*/
bool WiFiNetworkItem::IsKnownNetwork()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::IsKnownNetwork\n"));
    bool fRet = false;

    fRet = (m_InternalWiFiNetworkData.fExistingConfig ? true : false);

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::IsKnownNetwork: Network known = %d\n", fRet));
    return fRet;
}

/*------------------------------------------------------------------------------
    WiFiNetworkItem::GetActualNetworkType

    Returns the actual network type.
------------------------------------------------------------------------------*/
int WiFiNetworkItem::GetActualNetworkType()
{
    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] ++WiFiNetworkItem::GetActualNetworkType\n"));
    int iRet = WIFI_KT_UNKNOWN;

    iRet = GetKeyType(&m_InternalWiFiNetworkData);

    DEBUGMSG(ZONE_VERBOSE, (L"[WiFiUX] --WiFiNetworkItem::GetActualNetworkType: Actual network type = %d\n", iRet));
    return iRet;
}

BOOL WiFiNetworkItem::IsAuthenticated()
{
    if ((DOT11_AUTH_ALGO_80211_OPEN==m_pWiFiNetworkdata->dot11DefaultAuthAlgorithm) && (DOT11_CIPHER_ALGO_NONE==m_pWiFiNetworkdata->dot11DefaultCipherAlgorithm))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL WiFiNetworkItem::Is8021xNetwork()
{
    return IsOneXNetwork(&m_InternalWiFiNetworkData);
}

WIFI_NETWORKDATA* WiFiNetworkItem::GetNetworkConfigData()
{
    return (&m_InternalWiFiNetworkData);
}

DWORD WiFiNetworkItem::GetNetworkSignalStrength()
{
    return m_pWiFiNetworkdata->wlanSignalQuality;
}

HRESULT WiFiNetworkItem::SetNetworkSignalStrength(DWORD dwSignalQuality)
{
    m_pWiFiNetworkdata->wlanSignalQuality = dwSignalQuality;
    return S_OK;
}

HRESULT WiFiNetworkItem::GetIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo)
{
    HRESULT hr = S_OK;
    WSADATA wsaData;
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    ULONG ulSizeAdapterInfo = 0;
    PIP_ADAPTER_INFO pOriginalPtr = NULL;
    ULONG                    ulBufferLength, ulFlags;
    PIP_ADAPTER_ADDRESSES    pAdapterAddresses = NULL;
    WLAN_INTERFACE_INFO interfaceInfo = {0};
    WCHAR szInterfaceName[MAX_PATH];
    ce::tstring strAdapterName;
    PFIXED_INFO         pNetworkParams = NULL;
    PIP_ADDR_STRING pDnsServer = NULL;
    ULONG               ulBufferSize = 0;

    CBREx((WSAStartup(MAKEWORD(1,1), &wsaData) == 0), E_FAIL);
    ulFlags = GAA_FLAG_SKIP_ANYCAST              | 
              GAA_FLAG_SKIP_MULTICAST            | 
              GAA_FLAG_INCLUDE_ALL_INTERFACES    | 
              GAA_FLAG_INCLUDE_ALL_COMPARTMENTS;

    GetAdaptersAddresses(AF_UNSPEC, ulFlags, NULL, NULL, &ulBufferLength);
    pAdapterAddresses = (PIP_ADAPTER_ADDRESSES)malloc(ulBufferLength);
    CPR(pAdapterAddresses);
    CBR(NO_ERROR == GetAdaptersAddresses(AF_UNSPEC, ulFlags, NULL, pAdapterAddresses, &ulBufferLength));

    if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo( pAdapterInfo, &ulSizeAdapterInfo ))
    {
        pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo);
        CPR(pAdapterInfo);
        pOriginalPtr = pAdapterInfo;
        CBR(ERROR_SUCCESS == GetAdaptersInfo( pAdapterInfo, &ulSizeAdapterInfo));
    }

    CHR(GetInterfaceInfo(m_hWlanClient, &interfaceInfo));
    strAdapterName.assign(interfaceInfo.strInterfaceDescription);
    do
    {
        MultiByteToWideChar(CP_ACP, 0, pAdapterInfo->AdapterName, -1, szInterfaceName, MAX_PATH);
        if (!strAdapterName.compare(szInterfaceName)) break;
        pAdapterInfo = pAdapterInfo->Next;
    } while (pAdapterInfo);
    CPREx(pAdapterInfo, E_FAIL);

    PIP_ADDR_STRING pAddressList = &(pAdapterInfo->IpAddressList);
    CHR(StringCchPrintf(pIpPropInfo->pszIpAddr, IPADDRESS_STRING_MAXLENGTH, TEXT("%hs"), pAddressList->IpAddress.String));
    CHR(StringCchPrintf(pIpPropInfo->pszSubnetMask, IPADDRESS_STRING_MAXLENGTH, TEXT("%hs"), pAddressList->IpMask.String));

    PIP_ADDR_STRING pGatewayList = &(pAdapterInfo->GatewayList);
    if (pGatewayList->IpAddress.String[0] != 0)
    {
        CHR(StringCchPrintf(pIpPropInfo->pszGateway, IPADDRESS_STRING_MAXLENGTH, TEXT("%hs"), pGatewayList->IpAddress.String));
    }

    // Get Network params struct for DNS servers
    CBR(ERROR_BUFFER_OVERFLOW == GetNetworkParams(pNetworkParams, &ulBufferSize));
    pNetworkParams = (PFIXED_INFO)malloc(ulBufferSize);
    CPR(pNetworkParams);
    CBR(ERROR_SUCCESS==GetNetworkParams(pNetworkParams, &ulBufferSize));
    // Get DNS Servers
    pDnsServer = &(pNetworkParams->DnsServerList);
    if (pDnsServer->IpAddress.String[0] != 0)
    {
        CHR(StringCchPrintf(pIpPropInfo->pszDNSAddr, IPADDRESS_STRING_MAXLENGTH, TEXT("%hs"), pDnsServer->IpAddress.String));
    }

Error:
    if (pOriginalPtr)  
        free(pOriginalPtr);
    if (pAdapterAddresses)
        free(pAdapterAddresses);
    if (pNetworkParams)
        free(pNetworkParams);
    WSACleanup();
    return hr;
}

HRESULT WiFiNetworkItem::GetAdptIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo)
{
    HRESULT hr = S_OK;
    WLAN_INTERFACE_INFO interfaceInfo = {0};
    EthernetLan* pAdptLan = NULL;
    NETWORK_ADPT_INFO adptIpPropInfo = {0};

    CHR(GetInterfaceInfo(m_hWlanClient, &interfaceInfo));
    pAdptLan = new EthernetLan(interfaceInfo.strInterfaceDescription);
    CPREx(pAdptLan, E_FAIL);
    CBR(pAdptLan->GetAdapterIPProperties(&adptIpPropInfo));

    IPAddrToStr(pIpPropInfo->pszIpAddr, adptIpPropInfo.IPAddr);
    IPAddrToStr(pIpPropInfo->pszSubnetMask, adptIpPropInfo.SubnetMask);
    IPAddrToStr(pIpPropInfo->pszGateway, adptIpPropInfo.Gateway);
    IPAddrToStr(pIpPropInfo->pszDNSAddr, adptIpPropInfo.DNSAddr);
    IPAddrToStr(pIpPropInfo->pszDNSAltAddr, adptIpPropInfo.DNSAltAddr);
    IPAddrToStr(pIpPropInfo->pszWINSAddr, adptIpPropInfo.WINSAddr);
    IPAddrToStr(pIpPropInfo->pszWINSAltAddr, adptIpPropInfo.WINSAltAddr);
    pIpPropInfo->fUseDHCP = adptIpPropInfo.fUseDHCP;
    memcpy(pIpPropInfo->szAdapterName, adptIpPropInfo.szAdapterName, MAX_PATH);

    if (pIpPropInfo->fUseDHCP)
    {
        GetIpProperties(pIpPropInfo);
    }

Error:
    if (pAdptLan)
    {
        delete pAdptLan;
    }
    return hr;
}

BOOL WiFiNetworkItem::IsConfigDeleteable()
{
    if (IsKnownNetwork())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

HRESULT GetInterfaceInfo(__in HANDLE hClient, __deref_out PWLAN_INTERFACE_INFO pInterfaceInfo)
{
    HRESULT hr = S_OK;
    DWORD dwError;
    PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;

    CPREx(pInterfaceInfo, E_INVALIDARG);

    dwError = WlanEnumInterfaces(hClient, NULL, &pInterfaceList);
    CBREx(ERROR_SUCCESS == dwError, HRESULT_FROM_WIN32(dwError));

    // We only support up to one interface for now
    ASSERT(pInterfaceList->dwNumberOfItems <= 1);
    CBREx(1 <= pInterfaceList->dwNumberOfItems, E_NOINTERFACE);

    memcpy(pInterfaceInfo, &(pInterfaceList->InterfaceInfo[0]), sizeof(WLAN_INTERFACE_INFO));

Error:
    if (pInterfaceList)
    {
        WlanFreeMemory(pInterfaceList);
    }
    return hr;
}

HRESULT SetAdptIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo)
{
    HRESULT hr = S_OK;
    WLAN_INTERFACE_INFO interfaceInfo = {0};
    EthernetLan* pAdptLan = NULL;
    NETWORK_ADPT_INFO adptIpPropInfo = {0};

    adptIpPropInfo.szAdapterName = SysAllocString(pIpPropInfo->szAdapterName);
    adptIpPropInfo.fUseDHCP = pIpPropInfo->fUseDHCP;
    CBR(StringToAddr(pIpPropInfo->pszIpAddr, &adptIpPropInfo.IPAddr));
    CBR(StringToAddr(pIpPropInfo->pszSubnetMask, &adptIpPropInfo.SubnetMask));
    CBR(StringToAddr(pIpPropInfo->pszGateway, &adptIpPropInfo.Gateway));
    CBR(StringToAddr(pIpPropInfo->pszDNSAddr, &adptIpPropInfo.DNSAddr));

    pAdptLan = new EthernetLan(pIpPropInfo->szAdapterName);
    CPREx(pAdptLan, E_FAIL);
    adptIpPropInfo.fSaved = 1;
    CBR(pAdptLan->SetAdapterIPProperties(&adptIpPropInfo));

Error:
    if (pAdptLan)
    {
        delete pAdptLan;
    }
    if (adptIpPropInfo.szAdapterName)
    {
        SysFreeString(adptIpPropInfo.szAdapterName);
    }
    return hr;
}

HRESULT GetWiFiPowerState(BOOL *pWiFiOn)
{
    HRESULT hr = S_OK;
    BOOL fWiFiOn = FALSE;
    DWORD dwError;
    DWORD dwNegotiatedVersion;
    HANDLE hClient = NULL;
    WLAN_INTERFACE_INFO interfaceInfo = {0};
    ce::tstring strAdapterName;
    HKEY hKey = NULL;
    DWORD dwDisposition = 0;
    DWORD dwRegValue = 0;
    LPBYTE pbBuffer = NULL;
    DWORD dwType = REG_DWORD;
    DWORD dwLen = sizeof(DWORD);

    CPREx(pWiFiOn, E_INVALIDARG);

    // first check stored power state
    dwError = WlanOpenHandle(CSP_WIFI_SPECIFIC_INFO_VERSION, NULL, &dwNegotiatedVersion, &hClient);
    CBREx(ERROR_SUCCESS == dwError, HRESULT_FROM_WIN32(dwError));
    if (SUCCEEDED(GetInterfaceInfo(hClient, &interfaceInfo)))
    {
        fWiFiOn = TRUE;
        CBR(strAdapterName.assign(interfaceInfo.strInterfaceDescription));
        if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_cszWiFiPowerRegKeyPath, 0, KEY_ALL_ACCESS, &hKey))
        {// key not exist, try creating it
            CBR(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, g_cszWiFiPowerRegKeyPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, 
                              &hKey, &dwDisposition));
            if (hKey)
            {
                dwRegValue = fWiFiOn;
                pbBuffer = (LPBYTE) &(dwRegValue);
                CBR(RegSetValueEx(hKey, strAdapterName, 0, dwType, pbBuffer, sizeof(DWORD)) == ERROR_SUCCESS);
            }
        }

        // second check device power state, need OEM to customize here
        // and sync with stored power state

        // last, get real power state from stored place
        if (hKey)
        {
            CBR(ERROR_SUCCESS == RegQueryValueEx(hKey, strAdapterName, NULL, &dwType, (LPBYTE)&(dwRegValue), &dwLen));
            CBR(REG_DWORD == dwType);
            if (0==dwRegValue)
            {
                fWiFiOn = FALSE;
            }
            else
            {
                fWiFiOn = TRUE;
            }
        }
    }

    *pWiFiOn = fWiFiOn;

Error:
    if (hKey)
    {
        RegCloseKey(hKey);
    }
    WlanCloseHandle(hClient, NULL);
    return hr;
}

HRESULT SetWiFiPowerState(BOOL fWiFiOn, WiFiNetworkList* pWiFiList)
{
    HRESULT hr = S_OK;
    DWORD dwError;
    DWORD dwNegotiatedVersion;
    HANDLE hClient = NULL;
    WLAN_INTERFACE_INFO interfaceInfo = {0};
    WCHAR szAdapterName[MAX_PATH] = {0};
    DWORD cbAdapterName = ARRAYSIZE(szAdapterName);
    HKEY hKey = NULL;
    HKEY hWiFiBindKey = NULL;
    DWORD dwBindKeyType = REG_MULTI_SZ;
    WCHAR  wszWiFiBindName[MAX_PATH] = {0};
    WCHAR  wszWiFiBindValue[MAX_PATH] = {0};
    const WCHAR wszBindKeyName[] = L"UpperBind";
    const WCHAR wszNullBindKeyValue[] = L"";
    DWORD dwWiFiBindValueLen = sizeof(wszWiFiBindValue)/sizeof(char);
    DWORD dwRegValue = 0;
    LPBYTE pbBuffer = NULL;
    HANDLE  hNdis = INVALID_HANDLE_VALUE;
    WCHAR  wszWiFiPmName[MAX_PATH] = {0};

    dwError = WlanOpenHandle(CSP_WIFI_SPECIFIC_INFO_VERSION, NULL, &dwNegotiatedVersion, &hClient);

    CBR(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_cszWiFiPowerRegKeyPath, 0, 0, &hKey));
    if ((NULL==hClient) && SUCCEEDED(GetInterfaceInfo(hClient, &interfaceInfo)))
    {
        CHR(StringCchCopy( szAdapterName, ARRAYSIZE(szAdapterName), interfaceInfo.strInterfaceDescription));
    }
    else
    {
        CBR(ERROR_SUCCESS==RegEnumValue(hKey, 0, szAdapterName, &cbAdapterName, NULL, NULL, NULL, NULL));
    }

    hNdis = CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, OPEN_ALWAYS, 0, NULL);
    CBR(hNdis != INVALID_HANDLE_VALUE);
    // set real device power state
    _stprintf_s(wszWiFiBindName, MAX_PATH, L"Comm\\%s\\Parms", szAdapterName);
    CBRA(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszWiFiBindName, 0, 0, &hWiFiBindKey));
    _stprintf_s(wszWiFiPmName, MAX_PATH, L"%s\\%s", PMCLASS_NDIS_MINIPORT, szAdapterName);
    if (fWiFiOn)
    {
        if (ERROR_SUCCESS == RegQueryValueEx(hWiFiBindKey, wszBindKeyName, NULL, &dwBindKeyType, (LPBYTE)&(wszWiFiBindValue), &dwWiFiBindValueLen))
        {
            CBRA(ERROR_SUCCESS == RegDeleteValue(hWiFiBindKey, wszBindKeyName));
        }

        CEDEVICE_POWER_STATE dps = D4;
        CBRA(ERROR_SUCCESS==GetDevicePower(wszWiFiPmName, POWER_NAME, &dps));
        CBRA(ERROR_SUCCESS==SetDevicePower(wszWiFiPmName, POWER_NAME, D0));
        if (D4!=dps)
        {
            CBR(0!=DeviceIoControl(hNdis, IOCTL_NDIS_BIND_ADAPTER, (LPVOID)szAdapterName, sizeof(szAdapterName), NULL, 0, NULL, NULL));
        }
    }
    else
    {
        if (ERROR_SUCCESS != RegQueryValueEx(hWiFiBindKey, wszBindKeyName, NULL, &dwBindKeyType, (LPBYTE)&(wszWiFiBindValue), &dwWiFiBindValueLen))
        {
            pbBuffer = (LPBYTE) &(wszNullBindKeyValue);
            CBR(ERROR_SUCCESS == RegSetValueEx(hWiFiBindKey, wszBindKeyName, 0, dwBindKeyType, pbBuffer, 0));
        }

        CBR(0!=DeviceIoControl(hNdis, IOCTL_NDIS_UNBIND_ADAPTER, (LPVOID)szAdapterName, sizeof(szAdapterName), NULL, 0, NULL, NULL));
        dwError = SetDevicePower(wszWiFiPmName, POWER_NAME, D4);
    }
    CBRA(ERROR_SUCCESS==dwError);

    // set internal cached power state
    dwRegValue = fWiFiOn;
    pbBuffer = (LPBYTE) &(dwRegValue);
    CBR(RegSetValueEx(hKey, szAdapterName, 0, REG_DWORD, pbBuffer, sizeof(DWORD)) == ERROR_SUCCESS);

Error:
    if (hKey)
    {
        RegCloseKey(hKey);
    }
    if (hWiFiBindKey)
    {
        RegCloseKey(hWiFiBindKey);
    }
    WlanCloseHandle(hClient, NULL);
    CloseHandle(hNdis);
    return hr;
}

// If function succeeds then caller must WlanFreeMemory(pAvailableNetworkList)
HRESULT GetAvailableNetworkList(__in HANDLE hClient, __deref_out PWLAN_AVAILABLE_NETWORK_LIST *ppAvailableNetworkList, __out GUID *pguidInterface)
{
    HRESULT hr = S_OK;
    DWORD dwError;
    PWLAN_AVAILABLE_NETWORK_LIST pAvailableNetworkList = NULL;
    WLAN_INTERFACE_INFO interfaceInfo = {0};

    CBRAEx(ppAvailableNetworkList && pguidInterface, E_INVALIDARG);

    CHR(GetInterfaceInfo(hClient, &interfaceInfo));

    *pguidInterface = interfaceInfo.InterfaceGuid;
    
    dwError = WlanGetAvailableNetworkList(hClient,
        &(interfaceInfo.InterfaceGuid),
        NULL,
        NULL,
        &pAvailableNetworkList);
    CBREx(ERROR_SUCCESS == dwError, HRESULT_FROM_WIN32(dwError));

    *ppAvailableNetworkList = pAvailableNetworkList;

    // The caller owns this memory now
    pAvailableNetworkList = NULL;

Error:
    WlanFreeMemory(pAvailableNetworkList);
    return hr;
}

HRESULT GetAllCertNames(LPTSTR* ppszAllCertNamesArray, DWORD* pCntCertNames)
{
    HRESULT hr = S_OK;
    HCERTSTORE hMyStore = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    DWORD dwCertNameLen = 0;
    int nCerts = 0;

    CPREx(ppszAllCertNamesArray, E_INVALIDARG);
    CBREx(*pCntCertNames > 0, E_INVALIDARG);

    hMyStore = CertOpenStore(CERT_STORE_PROV_SYSTEM_W, CRYPT_ASN_ENCODING, 
                            0, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
    CBR(NULL!=hMyStore);

    while ((nCerts < *pCntCertNames)
            && (pCertContext = CertEnumCertificatesInStore(hMyStore,  pCertContext)))
    {
        CHR(GetCertName(pCertContext, &ppszAllCertNamesArray[nCerts++], &dwCertNameLen));
    }

    if (nCerts<*pCntCertNames)
    {
        *pCntCertNames = nCerts;
    }

Error:
    if (pCertContext)
    {
        CertFreeCertificateContext(pCertContext);
    }
    if ( hMyStore != NULL )
    {
        CertCloseStore( hMyStore, 0 );
    }
    return hr;
}

HRESULT GetCertName(PCCERT_CONTEXT pUserCert, LPTSTR* ppszCertName, DWORD* pCertNameLen)
{
    HRESULT hr = S_OK;
    WCHAR szName[MAX_PATH] = {0};
    DWORD cchName = 0;
    size_t cchNameLen;

    CPREx(pUserCert, E_INVALIDARG);
    if (ppszCertName)
    {
        cchName = CertGetNameStringW(pUserCert, CERT_NAME_FRIENDLY_DISPLAY_TYPE, 0, NULL, szName, MAX_PATH   );
        CHR(StringCchLength(szName, sizeof(szName), &cchNameLen));
        *pCertNameLen = cchNameLen+1;
        if (*pCertNameLen > 0)
        {
            *ppszCertName = NULL;
            *ppszCertName = SysAllocStringLen(*ppszCertName, *pCertNameLen);
            CPR(*ppszCertName);
            CHR( StringCchCopy(*ppszCertName, *pCertNameLen, szName) );
        }
    }

Error:
    if (FAILED(hr) && (NULL==*ppszCertName))
    {
        SysFreeString(*ppszCertName);
        *ppszCertName = NULL;
    }
    return hr;
}

HRESULT GetCertByName(LPCTSTR pszCertName, PCCERT_CONTEXT* ppUserCert)
{
    HRESULT hr = S_OK;
    HCERTSTORE hMyStore = NULL;
    LPTSTR pszName = NULL;
    DWORD dwNameLen = 0;
    PCCERT_CONTEXT pCertContext = NULL;

    CBREx((NULL!=pszCertName) && (NULL!=ppUserCert), E_INVALIDARG);

    hMyStore = CertOpenStore(CERT_STORE_PROV_SYSTEM_W, CRYPT_ASN_ENCODING, 
                            0, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
    CBR(NULL!=hMyStore);

    while (pCertContext = CertEnumCertificatesInStore(hMyStore,  pCertContext))
    {
        CHR(GetCertName(pCertContext, &pszName, &dwNameLen));
        if (!_tcsicmp(pszCertName, pszName))
        {
            break;
        }
        SysFreeString(pszName);
        pszName = NULL;
    }
    if (pCertContext)
    {
        *ppUserCert = pCertContext;
    }

Error:
    if ( hMyStore != NULL )
    {
        CertCloseStore( hMyStore, 0 );
    }
    if (pszName)
    {
        SysFreeString(pszName);
    }
    return hr;
}

DWORD
RegGetDwordValue(
    IN  HKEY   hKey,
    IN  PWSTR wszSubKeyName,
    IN  PWSTR wszValueName,
    OUT PDWORD      pdwValue)
{
    HKEY                hk;
    DWORD               dwSize;
    DWORD               dwType;
    DWORD               dwResult;
    DWORD               dwValue;

    dwResult = RegOpenKeyEx(hKey, wszSubKeyName, 0, KEY_ALL_ACCESS, &hk);
    if (ERROR_SUCCESS == dwResult)
    {
        dwSize = sizeof(DWORD);
        dwResult = RegQueryValueEx(hk, wszValueName, NULL, &dwType, (BYTE *)&dwValue, &dwSize);
        if (ERROR_SUCCESS == dwResult)
        {
            if (REG_DWORD == dwType)
                *pdwValue = dwValue;
            else
                dwResult = ERROR_BADDB;
        }
        RegCloseKey(hk);
    }

    return dwResult;
}

void
PeapInitializeDefaultConnectionProperties(
    OUT PEAP_CONN_PROP *pConnProp)
//
//  Called to initialize PEAP connection properties data structure
//  with default settings.
//
{
    DWORD dwValidateServerCert;
    PEAP_ENTRY_CONN_PROPERTIES EntryProp;

    pConnProp->dwVersion = 1;
    pConnProp->dwSize = sizeof(PEAP_CONN_PROP) + sizeof(PEAP_ENTRY_CONN_PROPERTIES);
    pConnProp->EapTlsConnProp.dwVersion = EAPTLS_CONN_PROPS_V1;
    pConnProp->EapTlsConnProp.dwSize = sizeof(EAPTLS_CONN_PROPERTIES_V1);
    pConnProp->EapTlsConnProp.fFlags = EAPTLS_CONN_FLAG_NO_CLIENT_CERT | EAPTLS_CONN_FLAG_NO_VALIDATE_NAME;

    //
    // If HKEY_LOCAL_MACHINE\Comm\EAP\Extension\25\ValidateServerCert == dword:0, then
    // by default server cert validation is disabled.
    //
    dwValidateServerCert = 1;
    EntryProp.dwEapTypeId = PPP_EAP_MSCHAPv2;
    (void)RegGetDwordValue(HKEY_LOCAL_MACHINE, EAPPEAP_KEY_25, EAPTLS_VAL_CHECK_SERVER_CERT, &dwValidateServerCert);
    (void)RegGetDwordValue(HKEY_LOCAL_MACHINE, EAPPEAP_KEY_25, EAPTLS_VAL_TUNNELED_EXTENSION_TYPE, &EntryProp.dwEapTypeId);

    if (dwValidateServerCert == 0)
        pConnProp->EapTlsConnProp.fFlags |= EAPTLS_CONN_FLAG_NO_VALIDATE_CERT;

    pConnProp->dwNumPeapTypes = 1;

    //
    // Setup the first peap entry conn prop
    //
    EntryProp.dwVersion = 1;
    EntryProp.dwSize = sizeof(PEAP_ENTRY_CONN_PROPERTIES);
    memcpy((PBYTE)&pConnProp->EapTlsConnProp + pConnProp->EapTlsConnProp.dwSize, (PBYTE)&EntryProp, sizeof(EntryProp));
}

HRESULT InsertAuthDataByCertName(__in WIFI_NETWORKDATA *pWiFiNetworkData, __in LPCTSTR pszCertName, __in BOOL* pfValidateServer)
{
    HRESULT hr = S_OK;
    PCCERT_CONTEXT pUserCert = NULL;
    EAPTLS_CONN_PROPERTIES_V1* pEapTlsConnProp = NULL;
//    DWORD dwConnFlags = 0;

    CPREx(pWiFiNetworkData, E_INVALIDARG);

    if (NULL!=pfValidateServer)
    {
//        dwConnFlags = (pWiFiNetworkData->eapmethodtype.eapType.type == PPP_EAP_PEAP) ? EAPTLS_CONN_FLAG_NO_CLIENT_CERT : 0;

        pEapTlsConnProp = (EAPTLS_CONN_PROPERTIES_V1 *)LocalAlloc(LPTR, sizeof(EAPTLS_CONN_PROPERTIES_V1));
        CPR(pEapTlsConnProp);
        pEapTlsConnProp->dwVersion = EAPTLS_CONN_PROPS_V1;
        pEapTlsConnProp->dwSize = sizeof(EAPTLS_CONN_PROPERTIES_V1);
        pEapTlsConnProp->fFlags = EAPTLS_CONN_FLAG_NO_VALIDATE_NAME;

        if (*pfValidateServer)
        {
            pEapTlsConnProp->fFlags &= ~EAPTLS_CONN_FLAG_NO_VALIDATE_CERT;
        }
        else
        {
            pEapTlsConnProp->fFlags |= EAPTLS_CONN_FLAG_NO_VALIDATE_CERT;
        }

        pEapTlsConnProp->UserHash.cbHash = 0;
        if (pszCertName)
        {
            CHR(GetCertByName(pszCertName, &pUserCert));
            if (pUserCert)
            {
                DWORD dwHashSize = MAX_HASH_SIZE;
                if (CertGetCertificateContextProperty(pUserCert, CERT_HASH_PROP_ID, &pEapTlsConnProp->UserHash.pbHash, &dwHashSize))
                {
                    pEapTlsConnProp->UserHash.cbHash = dwHashSize;
                    pEapTlsConnProp->dwNumHashes = 1;
                }
            }
        }
        if (0 == pEapTlsConnProp->UserHash.cbHash)
        {
            memset(pEapTlsConnProp->UserHash.pbHash, 0, MAX_HASH_SIZE);
            pEapTlsConnProp->fFlags |= EAPTLS_CONN_FLAG_NO_CLIENT_CERT;
            pEapTlsConnProp->dwNumHashes = 0;
            pEapTlsConnProp->bData[0] = 0;
        }

        if (pWiFiNetworkData->pbAuthData)
        {
            LocalFree(pWiFiNetworkData->pbAuthData);
            pWiFiNetworkData->pbAuthData = NULL;
            pWiFiNetworkData->dwAuthDataLen = 0;
        }

        if (pWiFiNetworkData->eapmethodtype.eapType.type == PPP_EAP_PEAP)
        {
            // No PEAP connection data provided, create default data
            DWORD dwSizeOfConnectionDataIn = sizeof(PEAP_CONN_PROP) + sizeof(PEAP_ENTRY_CONN_PROPERTIES)+ sizeof(WCHAR);
            PEAP_CONN_PROP* pPeapConnProp = NULL;

            pPeapConnProp = (PPEAP_CONN_PROP)LocalAlloc(LPTR, dwSizeOfConnectionDataIn);
            CPR(pPeapConnProp);
            PeapInitializeDefaultConnectionProperties(pPeapConnProp);
            pPeapConnProp->EapTlsConnProp.fFlags = pEapTlsConnProp->fFlags;

            PEAP_ENTRY_CONN_PROPERTIES EntryProp;
            PBYTE pEntryProp = (PBYTE)&EntryProp;
            EAPTLS_CONN_PROPERTIES_V1* pTmpEapTlsConnProp = NULL;
            pTmpEapTlsConnProp = &pPeapConnProp->EapTlsConnProp;
            pEntryProp = (PBYTE) &pPeapConnProp->EapTlsConnProp + pTmpEapTlsConnProp->dwSize;

            LONGLONG dwEntryPropSize = pPeapConnProp->dwSize;
            dwEntryPropSize -= pTmpEapTlsConnProp->dwSize;
            dwEntryPropSize -= offsetof(PEAP_CONN_PROP, EapTlsConnProp);

            LONGLONG dwSizeEapConnPropOut = sizeof(PEAP_CONN_PROP);
            dwSizeEapConnPropOut -= sizeof(EAPTLS_CONN_PROPERTIES_V1);
            dwSizeEapConnPropOut += pEapTlsConnProp->dwSize;
            dwSizeEapConnPropOut += dwEntryPropSize;
            PEAP_CONN_PROP* pEapConnPropOut = NULL;
            pEapConnPropOut = (PEAP_CONN_PROP *)LocalAlloc(0, dwSizeEapConnPropOut);
            
            pEapConnPropOut->dwVersion = pPeapConnProp->dwVersion;
            pEapConnPropOut->dwSize = dwSizeEapConnPropOut;
            pEapConnPropOut->dwNumPeapTypes = pPeapConnProp->dwNumPeapTypes;

            // copy in the EAPTLS structure
            memcpy(&pEapConnPropOut->EapTlsConnProp, pEapTlsConnProp, pEapTlsConnProp->dwSize);
    
            //append the PEAP_ENTRY_CONN_PROPERTIES
            memcpy( (PBYTE) &pEapConnPropOut->EapTlsConnProp + pEapTlsConnProp->dwSize,
                           pEntryProp,
                           dwEntryPropSize);

            CBR((pWiFiNetworkData->pbAuthData = (BYTE*)LocalAlloc(LPTR, pEapConnPropOut->dwSize)) != NULL);

            pWiFiNetworkData->dwAuthDataLen = pEapConnPropOut->dwSize;
            memcpy(pWiFiNetworkData->pbAuthData, pEapConnPropOut, pEapConnPropOut->dwSize);

            pWiFiNetworkData->dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
            pWiFiNetworkData->fOneXEnabled = TRUE;
        }
        else if (pWiFiNetworkData->eapmethodtype.eapType.type == PPP_EAP_TLS)
        {
            CBR((pWiFiNetworkData->pbAuthData = (BYTE*)LocalAlloc(LPTR, pEapTlsConnProp->dwSize)) != NULL);

            pWiFiNetworkData->dwAuthDataLen = pEapTlsConnProp->dwSize;
            memcpy(pWiFiNetworkData->pbAuthData, pEapTlsConnProp, pEapTlsConnProp->dwSize);

            pWiFiNetworkData->dwFlags |= WIFI_NDF_VALID_ONEXENABLED;
            pWiFiNetworkData->fOneXEnabled = TRUE;
        }
    }

Error:
    if (pUserCert)
        CertFreeCertificateContext(pUserCert);
    if (pEapTlsConnProp)
        LocalFree(pEapTlsConnProp);
    return hr;
}

HRESULT GetCertNameFromAuthData(
    __in WIFI_NETWORKDATA *pWiFiNetworkData, 
    __out LPTSTR* ppszCertName, 
    __out BOOL* pfValidateServer
)
{
    HRESULT hr = S_OK;
    HCERTSTORE hMyStore = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    DWORD dwCertNameLen = 0;
    int nCerts = 0;
    EAPTLS_CONN_PROPERTIES_V1* pEapTlsConnProp = NULL;
    EAPTLS_CONN_PROPERTIES_V1* pProfileEapTlsConnProp = NULL;

    CPREx(pWiFiNetworkData, E_INVALIDARG);
    CBRAEx(pWiFiNetworkData->fOneXEnabled, E_UNEXPECTED);

    CBREx(((pWiFiNetworkData->dwAuthDataLen > 0) && (NULL!=pWiFiNetworkData->pbAuthData)), E_UNEXPECTED);

    pEapTlsConnProp = (EAPTLS_CONN_PROPERTIES_V1 *)LocalAlloc(LPTR, sizeof(EAPTLS_CONN_PROPERTIES_V1));
    CPR(pEapTlsConnProp);

    if (pWiFiNetworkData->eapmethodtype.eapType.type == PPP_EAP_PEAP)
    {
        PEAP_CONN_PROP *pPeapConnProp = (PEAP_CONN_PROP *)pWiFiNetworkData->pbAuthData;
        pProfileEapTlsConnProp = &pPeapConnProp->EapTlsConnProp;
        CPREx(pProfileEapTlsConnProp, E_UNEXPECTED);
    }
    else if (pWiFiNetworkData->eapmethodtype.eapType.type == PPP_EAP_TLS)
    {
        pProfileEapTlsConnProp = (EAPTLS_CONN_PROPERTIES_V1*)pWiFiNetworkData->pbAuthData;
        CPREx(pProfileEapTlsConnProp, E_UNEXPECTED);
    }

    hMyStore = CertOpenStore(CERT_STORE_PROV_SYSTEM_W, CRYPT_ASN_ENCODING, 
                            0, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
    CBR(NULL!=hMyStore);

    CRYPT_HASH_BLOB HashBlob;
    HashBlob.cbData = pProfileEapTlsConnProp->UserHash.cbHash;
    HashBlob.pbData = pProfileEapTlsConnProp->UserHash.pbHash;
    pCertContext = CertFindCertificateInStore(hMyStore, X509_ASN_ENCODING,
                        0, CERT_FIND_HASH, &HashBlob, NULL);
    if (pCertContext)
    {
        // get Cert name
        CHR(GetCertName(pCertContext, ppszCertName, &dwCertNameLen));
    }

    // get validate server info
    if (pfValidateServer)
    {
        *pfValidateServer = (0 == (pProfileEapTlsConnProp->fFlags & EAPTLS_CONN_FLAG_NO_VALIDATE_CERT)) ? TRUE : FALSE;
    }

Error:
    if (pCertContext)
    {
        CertFreeCertificateContext(pCertContext);
    }
    if ( hMyStore != NULL )
    {
        CertCloseStore( hMyStore, 0 );
    }
    if (pEapTlsConnProp)
    {
        LocalFree(pEapTlsConnProp);
    }
    return hr;
}

EAP_TYPE GetEapMethodType(__in WIFI_NETWORKDATA *pData)
{
    EAP_TYPE eapType;
    if (pData->eapmethodtype.eapType.type)
    {
        eapType.type = pData->eapmethodtype.eapType.type;
        eapType.dwVendorType = pData->eapmethodtype.eapType.dwVendorType;
        eapType.dwVendorId = pData->eapmethodtype.eapType.dwVendorId;
    }
    else
    {
        eapType.type = PPP_EAP_PEAP;
        eapType.dwVendorType = 0;
        eapType.dwVendorId = 0;
        pData->eapmethodtype.eapType.type = PPP_EAP_PEAP;
        pData->eapmethodtype.eapType.dwVendorType = 0;
        pData->eapmethodtype.eapType.dwVendorId = 0;
    }
    return eapType;
}

DWORD GetValidateServerCertCheckFromEAP(DWORD dwEapType)
{
    DWORD dwValue = 0;
    HKEY hKey = NULL;
    WCHAR szEapKeyPath[MAX_PATH] = {0};
    DWORD dwType = REG_DWORD;
    DWORD dwLen = sizeof(DWORD);

    if (FAILED(StringCchPrintf(szEapKeyPath, ARRAYSIZE(szEapKeyPath), 
        TEXT("Comm\\EAP\\Extension\\%d"), dwEapType)))
    {
        dwValue = 0;
    }
    else
    {
        if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, szEapKeyPath, 0, KEY_ALL_ACCESS, &hKey))
        {
            if (hKey)
            {
                if (ERROR_SUCCESS != RegQueryValueEx(hKey, EAPTLS_VAL_CHECK_SERVER_CERT, NULL, &dwType, (LPBYTE)&(dwValue), &dwLen))
                {
                    dwValue = 0;
                }
            }
        }
        if (hKey)
        {
            RegCloseKey(hKey);
        }
    }
    return dwValue;
}

ADVANCED_SPINNER_TYPE GetSpinnerType(__in const WIFI_NETWORKDATA *pData)
{
    ADVANCED_SPINNER_TYPE ast = AST_COUNT;

    switch (pData->authalgo)
    {
        case DOT11_AUTH_ALGO_RSNA:
        case DOT11_AUTH_ALGO_WPA:
            ast = AST_EAPTYPE;
            break;
            
        case DOT11_AUTH_ALGO_80211_OPEN:
            if (DOT11_CIPHER_ALGO_WEP == pData->ciphalgo)
            {
                if (IsOneXNetwork(pData))
                {
                    // 80211x has EAP type options
                    ast = AST_EAPTYPE;
                }
                else
                {
                    // Wep-Open has Key index options
                    ast = AST_KEYINDEX;
                }
            }
            break;
            
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
            if (DOT11_CIPHER_ALGO_WEP == pData->ciphalgo)
            {
                ast = AST_KEYINDEX;
            }
            break;
                    
        default:
            // Other networks don't support advanced options
            ASSERT(AST_COUNT == ast);
            break;
    }

    return ast;
}

#define INVALID_SIZE ((size_t)~0)

/// <summary>
///   Convert DOT11_SSID struct to string format
/// </summary>
/// <param name="pSsid">Pointer to DOT11_SSID struct to convert</param>
/// <param name="pszSsid">Pointer to string to receive converted value</param>
/// <param name="cchSsid">Buffer size of pszSsid in TCHARs</param>
/// <returns>
///   Standard HRESULT
/// </returns>
/// <remarks>
///   We first try to convert using UTF-8 then fallback to ANSI if UTF-8 conversion fails.
/// </remarks>
HRESULT SsidToString(__in DOT11_SSID *pSsid, __out_ecount(cchSsid) TCHAR *pszSsid, UINT cchSsid)
{
    HRESULT hr = S_OK;
    UINT cchWritten = 0;

    CBREx(pSsid && pszSsid, E_INVALIDARG);

	memset(pszSsid,0,cchSsid*sizeof(TCHAR));
    // We actually have an array of UCHARs in pSsid but in practice these are typically filled with CHAR values
    // so let's treat them as such.
//     cchWritten = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pSsid->ucSSID, pSsid->uSSIDLength, pszSsid, cchSsid);
//     if (0 == cchWritten)
    {
        // UTF-8 conversion failed so try ANSI
        cchWritten = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pSsid->ucSSID, pSsid->uSSIDLength, pszSsid, cchSsid);
        CBREx(0 != cchWritten, HRESULT_FROM_WIN32(GetLastError()));
    }

    // NULL terminate the string
    CBRA(cchWritten < cchSsid);
    pszSsid[cchWritten] = 0;

	/*
	size_t size = FIELD_OFFSET(WLAN_RAW_DATA,DataBlob) + pSsid->uSSIDLength * sizeof(pSsid->ucSSID[0]);
	PWLAN_RAW_DATA prdBuffer = (PWLAN_RAW_DATA)malloc(size);
	prdBuffer->dwDataSize = pSsid->uSSIDLength * sizeof(pSsid->ucSSID[0]);
	memcpy(prdBuffer->DataBlob,pSsid->ucSSID,prdBuffer->dwDataSize);

	WCHAR outBuf[DOT11_SSID_MAX_LENGTH+1];
	memset(outBuf,0,sizeof(outBuf));

	if (prdBuffer == NULL
		|| prdBuffer->DataBlob == NULL
		|| prdBuffer->dwDataSize == 0
		|| prdBuffer->DataBlob[0] == (BYTE)0)
	{
		StringCchCopyW(pszSsid, cchSsid, L"<NULL>");
	}
	else
	{
		int ssidLen = static_cast<int>(prdBuffer->dwDataSize);
		if (ssidLen > DOT11_SSID_MAX_LENGTH)
			ssidLen = DOT11_SSID_MAX_LENGTH;
		int wchars = MultiByteToWideChar(CP_ACP, 0,
			(LPCSTR)(prdBuffer->DataBlob), ssidLen,
			outBuf, DOT11_SSID_MAX_LENGTH+1);
		if (0 >= wchars || wchars > DOT11_SSID_MAX_LENGTH)
			StringCchCopyW(pszSsid, cchSsid, L"<INVALID SSID>");
		else
		{
			outBuf[wchars] = L'\0';
			StringCchCopyW(pszSsid, cchSsid, outBuf);
		}
	}

	free(prdBuffer);
*/
Error:
    return hr;
}

HRESULT StringToSsid(__in_ecount(DOT11_SSID_MAX_LENGTH+1) TCHAR *pszSsid, __out DOT11_SSID *pssid)
{
    HRESULT hr = S_OK;
    int cchWritten;
    size_t cchSsid;

    CBRAEx(pszSsid && pssid, E_INVALIDARG);

    cchSsid = _tcslen(pszSsid);
    CBR(cchSsid <= DOT11_SSID_MAX_LENGTH);

    // Try to convert using UTF-8
    cchWritten = WideCharToMultiByte(CP_UTF8, 0, pszSsid, cchSsid, (LPSTR)&(pssid->ucSSID), ARRAYSIZE(pssid->ucSSID), NULL, NULL);
    CBREx(0 < cchWritten, HRESULT_FROM_WIN32(GetLastError()));

    // NB: If UTF8 conversion fails we shouldn't just convert using ANSI because
    // it probably means that we'd lose some characters from pszSsid.

    pssid->uSSIDLength = cchWritten;
    
Error:
    return hr;
}

wchar_t ConvertIntegerToHexWChar(unsigned int src) throw ()
{
   assert(src < 0x10);

   static const wchar_t conversionMap[] =
   {
      L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
      L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
   };

   return conversionMap[src];
}

inline size_t BufferToBinHexLength(
                 const void*,
                 size_t length
                 ) throw ()
{
   // Two characters per octet + terminator.
   return (length * 2) + 1;
}

inline bool IsHexDigit(wchar_t src) throw ()
{
   return (src >= L'0' && src <= L'9') ||
          (src >= L'A' && src <= L'F') ||
          (src >= L'a' && src <= L'f');
}

unsigned char ConvertHexWCharToInteger(wchar_t src) throw ()
{
   assert(IsHexDigit(src));

   if ((src >= L'0') && (src <= L'9'))
   {
      return static_cast<unsigned char>(src - L'0');
   }

   if ((src >= L'A') && (src <= L'F'))
   {
      return static_cast<unsigned char>(src - (L'A' - 10));
   }

   return static_cast<unsigned char>(src - (L'a' - 10));
}

// Converts a buffer to formatted octets. 'dst' must point to sufficient
// storage as determined by BufferToBinHexLength.
void ConvertBufferToBinHex(
        __in_bcount(srcLen) const void* src,
        size_t srcLen,
        __inout_ecount(dstLen) wchar_t* dst,
        size_t dstLen
        ) throw ()
{
   assert((src != 0) || (srcLen == 0));
   assert((dst != 0) && (dstLen != 0));

   const unsigned char* next = static_cast<const unsigned char*>(src);
   const unsigned char* end = next + srcLen;
   wchar_t* dstEnd = dst + dstLen;

   for ( ; (next < end) && (dst < dstEnd - 1); ++next)
   {
      *dst = ConvertIntegerToHexWChar((*next) >> 4);
      ++dst;
      *dst = ConvertIntegerToHexWChar((*next) & 0xF);
      ++dst;
   }

   // Add the null terminator.
   if (dst < dstEnd)
   {
      *dst = L'\0';
   }
   else
   {
      *(dstEnd - 1) = L'\0';
   }
}

size_t BinHexToBufferLength(
          const wchar_t* src,
          size_t srcLen
          ) throw ()
{
   assert((src != 0) || (srcLen == 0));

   const wchar_t* end = src + srcLen;

   for ( ; src < end; ++src)
   {
      if (!IsHexDigit(*src))
      {
         return INVALID_SIZE;
      }
   }

   // Divide by two rounding up in case we have an odd number of nibbles.
   return (srcLen + 1) / 2;
}

void ConvertBinHexToBuffer(
        const wchar_t* src,
        size_t srcLen,
        void* dst
        ) throw ()
{
   assert((src != 0) || (srcLen == 0));
   assert((dst != 0) || (srcLen == 0));
   assert(BinHexToBufferLength(src, srcLen) != INVALID_SIZE);

   unsigned char* nextDst = static_cast<unsigned char*>(dst);

   const wchar_t* nextSrc = src;
   const wchar_t* endSrc = src + srcLen;

   // If the given string is a odd length string, then special case the first
   // nibble.
   if ((srcLen % 2) != 0)
   {
      *nextDst = ConvertHexWCharToInteger(*nextSrc);
      ++nextSrc;
      ++nextDst;
   }

   for ( ; nextSrc < endSrc; ++nextSrc, ++nextDst)
   {
      *nextDst = ConvertHexWCharToInteger(*nextSrc) << 4;
      ++nextSrc;
      *nextDst |= ConvertHexWCharToInteger(*nextSrc);
   }
}

enum NodeType
{
    Node_WLANProfile = 0,
    Node_name = 1,
    Node_SSIDConfig = 2,
    Node_SSID = 3,
    Node_nonBroadcast = 4,
    Node_connectionType = 5,
    Node_connectionMode = 6,
    Node_MSM = 7,
    Node_security = 8,
    Node_authEncryption = 9,
    Node_authentication = 10,
    Node_encryption = 11,
    Node_useOneX = 12,
    Node_sharedKey = 13,
    Node_keyType = 14,
    Node_protected = 15,
    Node_keyMaterial = 16,
    Node_documentRoot = 17,
    Node_OneX = 18,
    Node_EapHostConfig = 19,
    Node_EapMethod = 20,
    Node_eapCommonType = 21,
    Node_eapCommonAuthorId = 22,
    Node_ConfigBlob = 23,
    Node_EAPConfig = 24,
    Node_CredentialsBlob = 25,
    Node_InvalidNode,
};

template<typename Value>
struct NameValuePair
{
    WCHAR szName[255];
    Value value;
};

#define c_profileNodePairCount 26
static NameValuePair<NodeType> s_profileNodePairs[c_profileNodePairCount] = 
{
    {L"WLANProfile",    Node_WLANProfile},
    {L"name",           Node_name},
    {L"SSIDConfig",     Node_SSIDConfig},
    {L"SSID",           Node_SSID},
    {L"nonBroadcast",   Node_nonBroadcast},
    {L"connectionType", Node_connectionType},
    {L"connectionMode", Node_connectionMode},
    {L"MSM",            Node_MSM},
    {L"security",       Node_security},
    {L"authEncryption", Node_authEncryption},
    {L"authentication", Node_authentication},
    {L"encryption",     Node_encryption},
    {L"useOneX",        Node_useOneX},
    {L"sharedKey",      Node_sharedKey},
    {L"keyType",        Node_keyType},
    {L"protected",      Node_protected},
    {L"keyMaterial",    Node_keyMaterial},
    {L"#document",      Node_documentRoot},
    {L"OneX",           Node_OneX},
    {L"EapHostConfig",  Node_EapHostConfig},
    {L"EapMethod",      Node_EapMethod},
    {L"ConfigBlob",     Node_ConfigBlob},
    {L"Type",           Node_eapCommonType},
    {L"AuthorId",       Node_eapCommonAuthorId},
    {L"EAPConfig",      Node_EAPConfig},
    {L"CredentialsBlob",  Node_CredentialsBlob}
};

static NameValuePair<DWORD> s_authAlgorithmPairs[] =
{
    {L"open",           DOT11_AUTH_ALGO_80211_OPEN},
    {L"shared",         DOT11_AUTH_ALGO_80211_SHARED_KEY},
    {L"WPA",            DOT11_AUTH_ALGO_WPA},
    {L"WPAPSK",         DOT11_AUTH_ALGO_WPA_PSK},
    {L"WPA2",           DOT11_AUTH_ALGO_RSNA},
    {L"WPA2PSK",        DOT11_AUTH_ALGO_RSNA_PSK},
};

static NameValuePair<DWORD> s_cipherAlgorithmPairs[] =
{
    {L"none",           DOT11_CIPHER_ALGO_NONE},
    {L"WEP",            DOT11_CIPHER_ALGO_WEP},
    {L"TKIP",           DOT11_CIPHER_ALGO_TKIP},
    {L"AES",            DOT11_CIPHER_ALGO_CCMP},
};

static NameValuePair<DWORD> s_useOneXPairs[] =
{
    {L"true",           TRUE},
    {L"false",          FALSE},
};

static NameValuePair<DWORD> s_connectionTypePairs[] =
{
    {L"ESS",            dot11_BSS_type_infrastructure},
    {L"IBSS",           dot11_BSS_type_independent},
};

static NameValuePair<DWORD> s_keyProtectPairs[] =
{
    {L"true",           WIFI_NDF_VALID_ENCRYPTEDKEY/*NWCTL_WEPK_HEXTEXT*/},
    {L"false",          0},
};

static NameValuePair<DWORD> s_keyTypePairs[] =
{
    {L"passPhrase",     WIFI_NDF_VALID_PASSPHRASE/*NWCTL_WEPK_PASSPHRASE*/},
    {L"networkKey",     0},
};

static NameValuePair<DWORD> s_nonBroadcastPairs[] =
{
    {L"true",           WIFI_NDF_VALID_HIDDEN},
    {L"false",          0},
};

HRESULT SyncXmlNode(WIFI_NETWORKDATA *pNetworkData, BOOL fUpdateXmlDirection, IXMLDOMNode *pNode, NodeType parentNodeType, BOOL *pfShouldRemove);

/// <summary>
/// Expand a xml node, sync value for each child.
/// </summary>
/// <param name="pConfig">The PNW_WLAN_CONFIG profile.</param>
/// <param name="fUpdateXmlDirection">The Sync direction.</param>
/// <param name="pNode">The xml node to expand.</param>
/// <returns>ERROR_SUCCESS if the operation is successful. Otherwise error code returned.</returns>
HRESULT ExpandNode(WIFI_NETWORKDATA *pNetworkData, BOOL fUpdateXmlDirection, IXMLDOMNode *pNode, NodeType nodeType)
{
    IXMLDOMNodeList* pChildren = NULL;
    LONG lCount = 0;
    HRESULT hr = S_OK;

    CPREx(pNode, E_INVALIDARG);

    CHR( pNode->get_childNodes(&pChildren) );

    CHR( pChildren->get_length(&lCount) );

    for (int i = 0; i < lCount;)
    {
        IXMLDOMNode *pChild = NULL;
        BOOL fShouldRemove = FALSE;
        CHR( pChildren->get_item(i, &pChild) );

        CHR( SyncXmlNode(pNetworkData, fUpdateXmlDirection, pChild, nodeType, &fShouldRemove) );
        if (fShouldRemove)
        {
            IXMLDOMNode *pRemovedChild = NULL;
            if (SUCCEEDED(hr = pNode->removeChild(pChild, &pRemovedChild)))
            {
                pRemovedChild->Release();
            }
        }

        pChild->Release();

        if (fShouldRemove)
        {
            lCount--;
        }
        else
        {
            i++;
        }
    }

Error:
    if (pChildren)
    {
        pChildren->Release();
    }
    return hr;
}

HRESULT SyncXmlEnumValue(IXMLDOMNode* pNode, NameValuePair<DWORD>* pEnums, DWORD dwCount, DWORD* pdwValue, BOOL fUpdateXmlDirection)
{
    HRESULT hr = S_OK;
    BSTR bstrInnerText = NULL;
    if (fUpdateXmlDirection)
    {
        for (int i = 0; i < dwCount; i++)
        {
            if (*pdwValue == pEnums[i].value)
            {
                bstrInnerText = SysAllocString(pEnums[i].szName);
                CPR(bstrInnerText);

                CHR( pNode->put_text(bstrInnerText) );
                break;
            }
        }
    }
    else
    {
        CHR( pNode->get_text(&bstrInnerText) );

        for (int i = 0; i < dwCount; i++)
        {
            if (!wcscmp(pEnums[i].szName, bstrInnerText))
            {
                *pdwValue = pEnums[i].value;
                break;
            }
        }
    }

Error:
    SysFreeString(bstrInnerText);
    return hr;
}

HRESULT SyncXmlFlagValue(IXMLDOMNode* pNode, NameValuePair<DWORD>* pEnums, DWORD* pdwValue, BOOL fUpdateXmlDirection)
{
    HRESULT hr = S_OK;
    BSTR bstrInnerText = NULL;
    DWORD dwMask = pEnums[0].value;
    if (fUpdateXmlDirection)
    {
        bstrInnerText = (*pdwValue & dwMask) ? SysAllocString(pEnums[0].szName) : SysAllocString(pEnums[1].szName);
        CPR(bstrInnerText);
        CHR( pNode->put_text(bstrInnerText) );
    }
    else
    {
        CHR( pNode->get_text(&bstrInnerText) );

        if (!wcscmp(pEnums[0].szName, bstrInnerText))
        {
            *pdwValue |= dwMask;
        }
        else
        {
            *pdwValue &= ~dwMask;
        }
    }
Error:
    SysFreeString(bstrInnerText);
    return hr;
}

/// <summary>
/// Sync a xml node value with the corresponding field of PNW_WLAN_CONFIG.
/// </summary>
/// <param name="pConfig">The PNW_WLAN_CONFIG profile.</param>
/// <param name="fUpdateXmlDirection">The Sync direction.</param>
/// <param name="pNode">The xml node to sync.</param>
/// <param name="pfShouldRemove">The BOOL indicates whether the node should be removed from tree.</param>
/// <returns>ERROR_SUCCESS if the operation is successful. Otherwise error code returned.</returns>
HRESULT SyncXmlNode(WIFI_NETWORKDATA *pNetworkData, BOOL fUpdateXmlDirection, IXMLDOMNode *pNode, NodeType parentNodeType, BOOL *pfShouldRemove = NULL)
{
    HRESULT hr = S_OK;
    NodeType nodeType = Node_InvalidNode;
    BSTR bstrName = NULL;
    BSTR bstrInnerText = NULL;
    ce::wstring nodeName;

    CPREx(pNode, E_INVALIDARG);

    if (pfShouldRemove)
    {
        *pfShouldRemove = FALSE;
    }

    pNode->get_nodeName(&bstrName);
    nodeName.assign(bstrName);
    size_t pos = nodeName.find(L":");
    if (pos != ce::wstring::npos)
    {
        nodeName.erase(0, pos + 1);
        nodeName.trim(L" ");
    }

    for (DWORD idx=0; idx<c_profileNodePairCount; idx++)
    {
        if (!nodeName.compare(s_profileNodePairs[idx].szName))
        {
            nodeType = s_profileNodePairs[idx].value;
            break;
        }
    }
//    CBR(nodeType!=Node_InvalidNode);
    switch (nodeType)
    {
    case Node_WLANProfile:
    case Node_MSM:
    case Node_authEncryption:
    case Node_SSIDConfig:
    case Node_SSID:
    case Node_security:
    case Node_documentRoot:
    case Node_EapMethod:
    case Node_EAPConfig:
    case Node_EapHostConfig:
        CHR(ExpandNode(pNetworkData, fUpdateXmlDirection, pNode, nodeType));
        break;
    case Node_connectionMode:
        // Do nothing since we use default value.
        break;
    case Node_sharedKey:
        // Cut this node
        if (fUpdateXmlDirection &&
            (((pNetworkData->authalgo == DOT11_AUTH_ALGO_80211_OPEN || pNetworkData->authalgo == DOT11_AUTH_ALGO_80211_SHARED_KEY) && 
             (pNetworkData->ciphalgo != DOT11_CIPHER_ALGO_WEP || (pNetworkData->dwFlags & WIFI_NDF_VALID_KEY) == 0)) ||
            pNetworkData->authalgo == DOT11_AUTH_ALGO_WPA ||
            pNetworkData->authalgo == DOT11_AUTH_ALGO_RSNA))
        {
            if (pfShouldRemove)
            {
                *pfShouldRemove = TRUE;
            }
            else
            {
                CHR(E_INVALIDARG);
            }

            break;
        }

        CHR(ExpandNode(pNetworkData, fUpdateXmlDirection, pNode, nodeType));
        break;

    case Node_OneX:
        // Cut this node.
        if (fUpdateXmlDirection && !pNetworkData->fOneXEnabled)
        {
            if (pfShouldRemove)
            {
                *pfShouldRemove = TRUE;
            }
            else
            {
                CHR(E_INVALIDARG);
            }

            break;
        }

        CHR(ExpandNode(pNetworkData, fUpdateXmlDirection, pNode, nodeType));
        break;
    case Node_eapCommonType:
        if (fUpdateXmlDirection)
        {
            WCHAR szType[5] = {0}; // 5 is enough for a byte string.
            _itow_s(pNetworkData->eapmethodtype.eapType.type, szType, 5/*size*/, 10/*radix*/);
            bstrInnerText = SysAllocString(szType);
            CPR(bstrInnerText);
            CHR( pNode->put_text(bstrInnerText) );
        }
        else
        {
            CHR( pNode->get_text(&bstrInnerText) );

            // The range of the value has already been checked by Native WLan. We don't check this.
            pNetworkData->eapmethodtype.eapType.type = (BYTE)_wtoi(bstrInnerText);
        }
        break;
    case Node_eapCommonAuthorId:
        if (fUpdateXmlDirection)
        {
            WCHAR szType[32] = {0}; // 32 is enough for a DWORD string.
            _itow_s(pNetworkData->eapmethodtype.dwAuthorId, szType, 32/*size*/, 10/*radix*/);
            bstrInnerText = SysAllocString(szType);
            CPR(bstrInnerText);
            CHR( pNode->put_text(bstrInnerText) );
        }
        else
        {
            CHR( pNode->get_text(&bstrInnerText) );

            // The range of the value has already been checked by Native WLan. We don't check this.
            pNetworkData->eapmethodtype.dwAuthorId = (DWORD)_wtoi(bstrInnerText);
        }
        break;
    case Node_CredentialsBlob:
        if (fUpdateXmlDirection)
        {
            if (pNetworkData->pbCredBlob&& pNetworkData->dwCredBlobLength)
            {
                DWORD dwHexLen = (DWORD)BufferToBinHexLength(pNetworkData->pbCredBlob, pNetworkData->dwCredBlobLength);
                bstrInnerText = SysAllocStringLen(L"", dwHexLen + 1);
                CPR(bstrInnerText);
                ConvertBufferToBinHex(pNetworkData->pbCredBlob, pNetworkData->dwCredBlobLength, bstrInnerText, dwHexLen + 1);
            }
            else
            {
                bstrInnerText = SysAllocString(L"");
                CPR(bstrInnerText);
            }

            CHR( pNode->put_text(bstrInnerText) );
        }
        else
        {
            CHR( pNode->get_text(&bstrInnerText) );

            DWORD dwBufferLen = BinHexToBufferLength(bstrInnerText, SysStringLen(bstrInnerText));
            if (dwBufferLen != INVALID_SIZE && dwBufferLen)
            {
                pNetworkData->pbCredBlob = (BYTE*)LocalAlloc(0, dwBufferLen);
                pNetworkData->dwCredBlobLength = dwBufferLen;
                CPR(pNetworkData->pbCredBlob);

                ConvertBinHexToBuffer(bstrInnerText, SysStringLen(bstrInnerText), pNetworkData->pbCredBlob);
            }
            else
            {
                pNetworkData->pbCredBlob = NULL;
                pNetworkData->dwCredBlobLength = 0;
            }
        }
        break;
    case Node_ConfigBlob:
        if (fUpdateXmlDirection)
        {
            if (pNetworkData->pbAuthData/* && pNetworkData->dwAuthDataLen*/)
            {
                DWORD dwHexLen = (DWORD)BufferToBinHexLength(pNetworkData->pbAuthData, pNetworkData->dwAuthDataLen);
                bstrInnerText = SysAllocStringLen(L"", dwHexLen + 1);
                CPR(bstrInnerText);
                ConvertBufferToBinHex(pNetworkData->pbAuthData, pNetworkData->dwAuthDataLen, bstrInnerText, dwHexLen + 1);
            }
            else
            { // default
                bstrInnerText = SysAllocString(L"010000004800000001000000010000002C0000000E000000000000000000000000000000000000000000000000000000000000000000000001000000100000001A000000C73B6A42");
                CPR(bstrInnerText);
            }

            CHR( pNode->put_text(bstrInnerText) );
        }
        else
        {
            CHR( pNode->get_text(&bstrInnerText) );

            DWORD dwBufferLen = BinHexToBufferLength(bstrInnerText, SysStringLen(bstrInnerText));
            if (dwBufferLen != INVALID_SIZE && dwBufferLen)
            {
                pNetworkData->pbAuthData = (BYTE*)LocalAlloc(0, dwBufferLen);
                pNetworkData->dwAuthDataLen = dwBufferLen;
                CPR(pNetworkData->pbAuthData);

                ConvertBinHexToBuffer(bstrInnerText, SysStringLen(bstrInnerText), pNetworkData->pbAuthData);
            }
            else
            {
                pNetworkData->pbAuthData = NULL;
                pNetworkData->dwAuthDataLen = 0;
            }
        }
        break;
    case Node_name:
        {
            // we only sync the SSID name, the profile name will be auto generated by Connection Manager. We don't care, it's not equal to SSID name now.
            // hence skip profile name
            if (parentNodeType != Node_SSID)
            {
                break;
            }
            if (fUpdateXmlDirection)
            {
                DWORD cchSize = DOT11_SSID_MAX_LENGTH+1;
                WCHAR szSsidName[DOT11_SSID_MAX_LENGTH+1];
                CHR(SsidToString(&(pNetworkData->ssid), szSsidName, cchSize));
                bstrInnerText = SysAllocString(szSsidName);
                CPR(bstrInnerText);
                CHR( pNode->put_text(bstrInnerText) );
            }
            else
            {
                CHR( pNode->get_text(&bstrInnerText) );
                CHR( StringToSsid(bstrInnerText, &pNetworkData->ssid) );
            }

            break;
        }
    case Node_connectionType:
        CHR( SyncXmlEnumValue(pNode, s_connectionTypePairs, _countof(s_connectionTypePairs), (DWORD*)&pNetworkData->bsstype, fUpdateXmlDirection) );
        break;
    case Node_nonBroadcast:
        CHR( SyncXmlFlagValue(pNode, s_nonBroadcastPairs, &pNetworkData->dwFlags, fUpdateXmlDirection) );
        break;
    case Node_authentication:
        CHR( SyncXmlEnumValue(pNode, s_authAlgorithmPairs, _countof(s_authAlgorithmPairs), (DWORD*)&pNetworkData->authalgo, fUpdateXmlDirection) );
        break;
    case Node_encryption:
        CHR( SyncXmlEnumValue(pNode, s_cipherAlgorithmPairs, _countof(s_cipherAlgorithmPairs), (DWORD*)&pNetworkData->ciphalgo, fUpdateXmlDirection) );
        break;
    case Node_useOneX:
        CHR( SyncXmlEnumValue(pNode, s_useOneXPairs, _countof(s_useOneXPairs), (DWORD*)&pNetworkData->fOneXEnabled, fUpdateXmlDirection) );
        break;
    case Node_keyMaterial:
        if (fUpdateXmlDirection)
        {
            bstrInnerText = SysAllocString(pNetworkData->szPassphrase);
            CPR(bstrInnerText);
            CHR( pNode->put_text(bstrInnerText) );
        }
        else
        {
            CHR( pNode->get_text(&bstrInnerText) );
            StringCchCopy(pNetworkData->szPassphrase, PWLEN+1, bstrInnerText);
        }
        break;
    case Node_protected:
        CHR( SyncXmlFlagValue(pNode, s_keyProtectPairs, &pNetworkData->dwFlags, fUpdateXmlDirection) );
        break;
    case Node_keyType:
        CHR( SyncXmlFlagValue(pNode, s_keyTypePairs, &pNetworkData->dwFlags, fUpdateXmlDirection) );
        break;
    default:
        break;
    }

Error:
    SysFreeString(bstrName);
    SysFreeString(bstrInnerText);
    return hr;
}

WCHAR g_szProfileTemplate[] = L"<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\
<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
    <name>SampleProfile</name>\
    <SSIDConfig>\
        <SSID>\
            <name>SampleProfile</name>\
        </SSID>\
        <nonBroadcast>false</nonBroadcast>\
    </SSIDConfig>\
    <connectionType>ESS</connectionType>\
    <connectionMode>manual</connectionMode>\
    <MSM>\
        <security>\
            <authEncryption>\
                <authentication>WPAPSK</authentication>\
                <encryption>TKIP</encryption>\
                <useOneX>false</useOneX>\
            </authEncryption>\
            <sharedKey>\
                <keyType>passPhrase</keyType>\
                <protected>false</protected>\
                <keyMaterial>SampleKey</keyMaterial>\
            </sharedKey>\
            <OneX xmlns=\"http://www.microsoft.com/networking/OneX/v1\">\
                <authMode>user</authMode>\
                <EAPConfig>\
                  <EapHostConfig xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\" \
                    xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\">\
                    <EapMethod>\
                      <eapCommon:Type>25</eapCommon:Type> \
                      <eapCommon:AuthorId>0</eapCommon:AuthorId> \
                    </EapMethod>\
                    <ConfigBlob>0201</ConfigBlob>\
                  </EapHostConfig>\
                </EAPConfig> \
            </OneX> \
         </security>\
    </MSM>\
</WLANProfile>";

/// <summary>
/// Sync the xml profile with the PNW_WLAN_CONFIG profile.
/// </summary>
/// <param name="pConfig">The PNW_WLAN_CONFIG profile.</param>
/// <param name="fUpdateXmlDirection">The Sync direction.</param>
/// <param name="pstrXml">The xml profile.</param>
/// <returns>ERROR_SUCCESS if the operation is successful. Otherwise error code returned.</returns>
HRESULT SyncProfile(WIFI_NETWORKDATA *pNetworkData, BOOL fUpdateXmlDirection, BSTR* pstrXml)
{
    BSTR bstrXml = NULL;
    IUnknown *pUnk = NULL;
    IXMLDOMDocument* pDOMDoc = NULL;
    VARIANT_BOOL fSuccess = 0;
    HRESULT hr = S_OK;

    CPREx(pNetworkData, E_INVALIDARG);
    CPREx(pstrXml, E_INVALIDARG);

    bstrXml = fUpdateXmlDirection ? SysAllocString(g_szProfileTemplate) : SysAllocString(*pstrXml);
    if (!bstrXml)
    {
        hr = GetLastError();
        CBREx(hr != ERROR_SUCCESS, E_OUTOFMEMORY);
    }

    hr = CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER, 
                          __uuidof(IUnknown), (LPVOID*)&pUnk );
    CHR(hr);

    hr = pUnk->QueryInterface(__uuidof(IXMLDOMDocument), (LPVOID*)&pDOMDoc);
    CHR(hr);

    hr = pDOMDoc->loadXML(bstrXml, &fSuccess);
    CHR(hr);
    if (!fSuccess)
    {
        CHR(E_FAIL);
    }

    CHR( SyncXmlNode(pNetworkData, fUpdateXmlDirection, pDOMDoc, Node_InvalidNode) );

    if (fUpdateXmlDirection)
    {
        CHR(pDOMDoc->get_xml(pstrXml));
    }

Error:
    SysFreeString(bstrXml);
    if (pDOMDoc)
        pDOMDoc->Release();
    if (pUnk)
        pUnk->Release();
    return hr;
}

WCHAR g_szEapProfileTemplate[] = L"<?xml version=\"1.0\" ?> \
 <EapHostUserCredentials xmlns=\"http://www.microsoft.com/provisioning/EapHostUserCredentials\" \
   xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\" >\
   <EapMethod>\
     <Type>%d</Type> \
     <AuthorId>%d</AuthorId> \
   </EapMethod>\
   <CredentialsBlob>%s</CredentialsBlob>\
 </EapHostUserCredentials>";

BOOL GetEapUserCredProfileXml(__in WIFI_NETWORKDATA *pWiFiNetworkData, __out LPWSTR *ppstrProfileXml)
{
    HRESULT hr = S_OK;
    DWORD dwEapUserDataLen = 0;
    DWORD dwEapUserStruLen = 0;
//    PPM_EAP_USER_CRED_PROFILE pEapUserCred = NULL;
    DWORD dwPsWordLen = 0;
    DWORD dwIndex = 0;
    DWORD dwUserNameLen = 0;
    DWORD dwDomainLen = 0;
    EAP_TYPE eapType;
    BSTR bstrXml = NULL;
    BSTR bstrCredText = NULL;
    IXMLDOMDocument* pDOMDoc = NULL;
    VARIANT_BOOL fSuccess = 0;

    // Make sure the input is valid
    // Make sure we have a valid pointer to take out...
    CPRA(ppstrProfileXml);

    // Make sure Wi-Fi Network Data is valid
    CPRA(pWiFiNetworkData);

    // Now get the EAP Method type
    eapType = GetEapMethodType(pWiFiNetworkData);

    // Only PEAP and TLS are supported as of now... extend this as needed later
    CBRA(eapType.type == PPP_EAP_PEAP || eapType.type == PPP_EAP_TLS);

    // Make sure that the value at the input is null
    CBRA((*ppstrProfileXml) == NULL);

    // Now for the particular EAP Type, create the profile...
    // The stack is expecting the blob to be:
    //[Username_strlen_DWORD|Username|Password_strlen|Password|Domain_length|Domain]
    
    // Calculte Username length. Username MUST be available
    CBRA((pWiFiNetworkData->szUsername) != NULL);
    dwUserNameLen = (_tcslen(pWiFiNetworkData->szUsername) + 1) * sizeof(TCHAR);
    CBRA(dwUserNameLen <= ((UNLEN+1) * sizeof(TCHAR)));
    dwEapUserDataLen = dwUserNameLen + sizeof(DWORD);
    
    // Calculate Domain length. Domain might not be available, so act accordingly
    if (pWiFiNetworkData->szDomain)
    {
        dwDomainLen = (_tcslen(pWiFiNetworkData->szDomain) + 1) * sizeof(TCHAR);
        CBRA(dwDomainLen <= ((DNLEN+1) * sizeof(TCHAR)));
        dwEapUserDataLen += dwDomainLen + sizeof(DWORD);
    }
    
    // Calculate Password length. Password might not be available, so act accordingly
    if (pWiFiNetworkData->szPassphrase)
    {
        // Password is relevant only for PEAP
        if (eapType.type == PPP_EAP_PEAP)
        {
           dwPsWordLen += (_tcslen(pWiFiNetworkData->szPassphrase) + 1) * sizeof(TCHAR);
           CBRA(dwPsWordLen <= (PWLEN * sizeof(TCHAR)));
        }

        dwEapUserDataLen += dwPsWordLen + sizeof(DWORD);
    }

    // Now allocate enough memory to construct the blob which needs to be sent to generate the XML
    pWiFiNetworkData->pbCredBlob = NULL;
    pWiFiNetworkData->pbCredBlob = (BYTE*)LocalAlloc(LPTR, dwEapUserDataLen);
    CPRA(pWiFiNetworkData->pbCredBlob);
    pWiFiNetworkData->dwCredBlobLength = dwEapUserDataLen;

    // Start at the beginning
    dwIndex = 0;

    // Username
    memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], &dwUserNameLen, sizeof(DWORD));
    dwIndex += sizeof(DWORD);
    memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], pWiFiNetworkData->szUsername, dwUserNameLen);
    dwIndex += dwUserNameLen;

    // Password, if it exists
    memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], &dwPsWordLen, sizeof(DWORD));
    dwIndex += sizeof(DWORD);
    
    if (eapType.type == PPP_EAP_PEAP)
    {
       memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], pWiFiNetworkData->szPassphrase, dwPsWordLen);
       dwIndex += dwPsWordLen;
    }

    // Domain, if it exists
    memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], &dwDomainLen, sizeof(DWORD));
    dwIndex += sizeof(DWORD);

    if (pWiFiNetworkData->szDomain)
    {
        memcpy(&pWiFiNetworkData->pbCredBlob[dwIndex], pWiFiNetworkData->szDomain, dwDomainLen);
    }

    // Call the helper to generate the credential xml
    DWORD dwHexLen = (DWORD)BufferToBinHexLength(pWiFiNetworkData->pbCredBlob, pWiFiNetworkData->dwCredBlobLength);
    bstrCredText = SysAllocStringLen(L"", dwHexLen + 1);
    CPR(bstrCredText);
    ConvertBufferToBinHex(pWiFiNetworkData->pbCredBlob, pWiFiNetworkData->dwCredBlobLength, bstrCredText, dwHexLen + 1);

    DWORD dwXmlLen = _tcslen(g_szEapProfileTemplate)+5+32+dwHexLen+1; // 5 for Type, 32 for AuthorId
    bstrXml = SysAllocStringLen(L"", dwXmlLen);
    CPR(bstrXml);
    swprintf_s(bstrXml, dwXmlLen, g_szEapProfileTemplate, pWiFiNetworkData->eapmethodtype.eapType.type, pWiFiNetworkData->eapmethodtype.eapType.dwVendorId, bstrCredText);

    *ppstrProfileXml = bstrXml;

Error:
    if (pDOMDoc)
        pDOMDoc->Release();

    if (FAILED(hr) && (*ppstrProfileXml))
    {
        WlanFreeMemory(*ppstrProfileXml);
        *ppstrProfileXml = NULL;
    }

    SysFreeString(bstrCredText);
    return SUCCEEDED(hr);
}

HRESULT CreateWiFiProfileXml(__in WIFI_NETWORKDATA *pWiFiNetworkData, __deref_out TCHAR **ppszProfileXml, __deref_out TCHAR **ppszEapCredProfileXml)
{
    HRESULT hr = S_OK;

    if ((WIFI_NDF_VALID_EAPTYPE & pWiFiNetworkData->dwFlags) &&
        (AST_EAPTYPE == GetSpinnerType(pWiFiNetworkData)))
    {
        // Make sure this IS a OneX Profile
        //CBRA(pOneXProfile);

        // Get the EAP User XML for this network data
        CBRA(GetEapUserCredProfileXml(pWiFiNetworkData, ppszEapCredProfileXml));
        CBRA(*ppszEapCredProfileXml != NULL);
//        pWiFiNetworkData->pbAuthData = L"010000004800000001000000010000002C0000000E000000000000000000000000000000000000000000000000000000000000000000000001000000100000001A000000C73B6A42";
//        pWiFiNetworkData->dwAuthDataLen = 144;
    }

    CHR(SyncProfile(pWiFiNetworkData, TRUE, ppszProfileXml));

Error:
    return hr;
}

HRESULT GetWiFiConfigByConnectionName(LPCTSTR pConnectionName, WIFI_NETWORKDATA *pWiFiData)
{
    HRESULT hr = S_OK;
    DWORD cbSize = 255;
    CM_CONNECTION_CONFIG* pConfig = NULL;
    CM_RESULT result = CMRE_INSUFFICIENT_BUFFER;
    WCHAR* szXml = NULL;
    UINT i;
    for (i = 0; i < MAX_TRY_COUNT_FOR_INSUFFICIENT_BUFFER && result == CMRE_INSUFFICIENT_BUFFER; i++)
    {
        LocalFree(pConfig);

        pConfig = (CM_CONNECTION_CONFIG*)LocalAlloc(0, cbSize);
        CPR(pConfig);
        result = CmGetConnectionConfig(pConnectionName, pConfig, &cbSize);
    }

    CBREx(CMRE_SUCCESS==result, E_FAIL);

    CHR(AllocAndGetXmlFromConnectionConfig(pConfig, &szXml));

    CHR( SyncProfile(pWiFiData, FALSE/*updateXmlDirection*/, &szXml) );

Error:
    LocalFree(pConfig);
    LocalFree(szXml);
    return hr;
}

// Simplified version of CmUpdateConnectionConfig that doesn't require a
// CM_CONFIG_CHANGE_HANDLE.
// NB: We use this function to ensure we never leak the handle by guaranteeing
// that CmUpdateConnectionConfig is called if we successfully get a handle.
// We have to do this because for some reason the CM APIs don't provide a means
// to close the handle and cancel the update once you've called CmGetToUpdateConnectionConfig.
// Ideally we should be able to call CmGetToUpdateConnectionConfig in ConfigureWiFiNetwork
// and if something goes wrong before calling CmUpdateConnectionConfig then we
// could just CloseHandle(hConfig).  As is though, we will end up allocating
// memory for pConfig twice because we have to use CmGetConnectionConfig to
// avoid getting a handle then call CmGetToUpdateConnectionConfig later when
// we're actually ready to update the connection.
CM_RESULT CmUpdateConnectionConfigByName(__in const WCHAR* const pszConnection,
    CM_CONFIG_OPTION cmco,
    __in_bcount(cbConfig) CM_CONNECTION_CONFIG* pConfig,
    DWORD cbConfig)
{
    CM_RESULT cmr = CMRE_SUCCESS;
    HRESULT hr = S_OK;
    CM_CONFIG_CHANGE_HANDLE hConfig;
    CM_CONNECTION_CONFIG *pThrowAway = NULL;
    DWORD cbThrowAway = 0;
    
    // CmGetToUpdateConnectionConfig asserts if you pass in a NULL pointer for pConfig
    cbThrowAway = 1;
    pThrowAway = (CM_CONNECTION_CONFIG *)LocalAlloc(LMEM_FIXED, cbThrowAway);
    CPR(pThrowAway);
    
    cmr = CmGetToUpdateConnectionConfig(pszConnection, pThrowAway, &cbThrowAway, &hConfig);
    ASSERT(CMRE_INSUFFICIENT_BUFFER == cmr);
    while (CMRE_INSUFFICIENT_BUFFER == cmr)
    {
        VBR(NULL == LocalFree(pThrowAway));
        pThrowAway = (CM_CONNECTION_CONFIG *)LocalAlloc(LMEM_FIXED, cbThrowAway);
        CPR(pThrowAway);

        pThrowAway->Version = CM_CURRENT_VERSION;
        cmr = CmGetToUpdateConnectionConfig(pszConnection, pThrowAway, &cbThrowAway, &hConfig);
    }
    if (CMRE_SUCCESS != cmr)
    {
        ASSERT(FALSE);
        goto Error;
    }

    cmr = CmUpdateConnectionConfig(hConfig, cmco, pConfig, cbConfig);
    ASSERT(CMRE_SUCCESS == cmr);

Error:
    VBR(NULL == LocalFree(pThrowAway));

    if (FAILED(hr))
    {
        ASSERT(E_OUTOFMEMORY == hr);
        cmr = CMRE_OUT_OF_MEMORY;
    }
    
    return cmr;
}

HRESULT AllocAndGetXmlFromConnectionConfig(
    const CM_CONNECTION_CONFIG *pConfig,
    LPWSTR *pszXml)
{
    HRESULT hr = S_OK;
    WCHAR *szXml = NULL;

    CPREx(pConfig, E_INVALIDARG);
    CPREx(pszXml, E_INVALIDARG);

    CspWiFiSpecificInfo *pCspWiFiInfo = (CspWiFiSpecificInfo*)&pConfig->TypeSpecificInfo[0];

    // Extract the profile XML and make a copy
    // We only get the connection configuration xml, ignore eap user credential xml.
    // cbUserDataOffset should be always the connection config xml length, even if there is no user data in the config.
    // Terminate with NULL.
    //

    // Make sure no overflow, prevent prefast warning.
    CBREx(pCspWiFiInfo->cbUserDataOffset + sizeof(WCHAR) >= pCspWiFiInfo->cbUserDataOffset, E_OUTOFMEMORY);

    szXml = (LPWSTR)LocalAlloc(0, pCspWiFiInfo->cbUserDataOffset + sizeof(WCHAR));
    CPR(szXml);

    // DevConfigXml should be NULL terminated
    memcpy(szXml, &pCspWiFiInfo->DevConfigXml[0], pCspWiFiInfo->cbUserDataOffset);
    // when there is eap user data, the xml is not NULL terminated, appending the NULL.
    szXml[pCspWiFiInfo->cbUserDataOffset/sizeof(WCHAR)] = 0;

    *pszXml = szXml;

Error:
    if (FAILED(hr))
    {
        if (szXml != NULL)
            LocalFree(szXml);
    }
    return hr;
}

// NB: Returns WIFI_HRESULT
HRESULT ConfigureWiFiNetwork(__inout WIFI_NETWORKDATA *pWiFiNetworkData)
{
    HRESULT hr = S_FALSE;
    BOOL fExistingConfig = FALSE;
    CM_CONNECTION_CONFIG *pConfig = NULL;
    DWORD cbConfig = 0;
    CM_CONNECTION_CONFIG *pExistingConfig = NULL;
    DWORD cbExistingConfig = 0;
    CM_CONNECTION_DETAILS *pDetails = NULL;
    DWORD cbDetails = 0;
    CM_RESULT cmr;
    CspWiFiSpecificInfo *pInfo = NULL;
//    PM_PROFILE *pProfile = NULL;      
//    PM_EAP_USER_CRED_PROFILE *pEapUserCredProfile = NULL;
    WIFI_NETWORKDATA stExistingWiFiNetworkData = {0};
    TCHAR *pszProfileXml = NULL;
    TCHAR *pszEapCredProfileXml = NULL;
    UINT cbXml = 0;
    UINT cbProfileXml = 0;
    UINT cbCredXml = 0;
    UINT cbInfo = 0;

    CPRAEx(pWiFiNetworkData, E_INVALIDARG);

    // Check to see if this connection already exists
    ASSERT(WIFI_NDF_VALID_SZSSID & pWiFiNetworkData->dwFlags);
    cbDetails = sizeof(CM_CONNECTION_DETAILS);
    pDetails = (CM_CONNECTION_DETAILS *)LocalAlloc(LMEM_FIXED, cbDetails);
    CPR(pDetails);
    cmr = CmGetConnectionDetailsByName(pWiFiNetworkData->szSsid, pDetails, &cbDetails);
    while (CMRE_INSUFFICIENT_BUFFER == cmr)
    {
        VBR(NULL == LocalFree(pDetails));
        pDetails = (CM_CONNECTION_DETAILS *)LocalAlloc(LMEM_FIXED, cbDetails);
        CPR(pDetails);

        cmr = CmGetConnectionDetailsByName(pWiFiNetworkData->szSsid, pDetails, &cbDetails);
    }
    if (CMRE_SUCCESS == cmr)
    {
        fExistingConfig = TRUE;
    }

    if (fExistingConfig)
    {
        // CmGetConnectionConfig asserts if you pass in a NULL pointer for pExistingConfig
        cbExistingConfig = 1;
        pExistingConfig = (CM_CONNECTION_CONFIG *)LocalAlloc(LMEM_FIXED, cbExistingConfig);
        CPR(pExistingConfig);
        
        cmr = CmGetConnectionConfig(pWiFiNetworkData->szSsid, pExistingConfig, &cbExistingConfig);
        ASSERT(CMRE_INSUFFICIENT_BUFFER == cmr);
        while (CMRE_INSUFFICIENT_BUFFER == cmr)
        {
            VBR(NULL == LocalFree(pExistingConfig));
            pExistingConfig = (CM_CONNECTION_CONFIG *)LocalAlloc(LMEM_FIXED, cbExistingConfig);
            CPR(pExistingConfig);

            pExistingConfig->Version = CM_CURRENT_VERSION;
            cmr = CmGetConnectionConfig(pWiFiNetworkData->szSsid, pExistingConfig, &cbExistingConfig);
        }

        CBRA(CMRE_SUCCESS == cmr);

        CHR(GetWiFiConfigByConnectionName(pWiFiNetworkData->szSsid, &stExistingWiFiNetworkData));
        // Update WiFi network data with data from existing network data
//        VHR(UpdateWiFiNetworkDataFromExistingProfile(pProfile, pEapUserCredProfile, pWiFiNetworkData));
        // re-create the profile and eap cred (if any) xmls
        CreateWiFiProfileXml(pWiFiNetworkData, &pszProfileXml, &pszEapCredProfileXml);

        pExistingConfig->fEnabled = TRUE;

        // By default AutoConnect will be set
//        pWiFiNetworkData->fAutoConnect = true;
//        pWiFiNetworkData->dwFlags |= WIFI_NDF_VALID_AUTOCONNECT;
    }
    else
    {
        // Create profile Xml from scratch
        CreateWiFiProfileXml(pWiFiNetworkData, &pszProfileXml, &pszEapCredProfileXml);
    }

    if (pszEapCredProfileXml)
    {
        cbCredXml = (_tcslen(pszEapCredProfileXml) + 1) * sizeof(TCHAR);
    }

    CBRA(pszProfileXml != NULL);
    if (pszProfileXml)
    {
        cbProfileXml = sizeof(TCHAR) * (_tcslen(pszProfileXml) + 1);
    }
    cbXml = cbCredXml + cbProfileXml;
    cbInfo = offsetof(CspWiFiSpecificInfo, DevConfigXml[0]) + cbXml;
    cbConfig = CmGetConnectionConfigNeededSize(cbInfo);

    pConfig = (CM_CONNECTION_CONFIG *)LocalAlloc(LPTR, cbConfig);
    CPR(pConfig);

    pInfo = (CspWiFiSpecificInfo *)LocalAlloc(LPTR, cbInfo);
    CPR(pInfo);

    if (pExistingConfig)
    {
        // Copy only the header. The type specific info will be copied later
        memcpy(pConfig, pExistingConfig, CmGetConnectionConfigNeededSize(0));
    }
    else
    {
        pConfig->Version = CM_CURRENT_VERSION;
        VHR(StringCchCopy(pConfig->szDescription, ARRAYSIZE(pConfig->szDescription), pWiFiNetworkData->szSsid));
        pConfig->Type = CM_CSP_WIFI_TYPE;
        pConfig->Characteristic[CMCH_BANDWIDTH_KBITPS] = CM_UNKNOWN_CHARACTERISTIC_VALUE;
        pConfig->Characteristic[CMCH_SUPPORTS_WAKE_ON_INCOMING] = CM_UNKNOWN_CHARACTERISTIC_VALUE;
        pConfig->Characteristic[CMCH_ROAMING] = CM_UNKNOWN_CHARACTERISTIC_VALUE;
        pConfig->ConnectBehavior = CMCB_ALWAYS_CONNECTED;
        pConfig->AutoReconnect.ReconnectAlg = CMTR_LINEAR;
        pConfig->AutoReconnect.DelaySec = 120;
        pConfig->AutoReconnect.Attempts = 3;
        pConfig->AutoReconnect.AttemptsExceededDelaySec = 120;
        pConfig->fEnabled = TRUE;
    }

    pInfo->Version = CSP_WIFI_SPECIFIC_INFO_VERSION;

    // All WiFi connections must be interface specific
    pInfo->InterfaceSpecific = TRUE;
    pInfo->InterfaceGuid = pWiFiNetworkData->guidInterface;

    pInfo->cbDevConfig = cbXml;

    // Copy the profile first
    pInfo->cbUserDataOffset = pInfo->cbDevConfig;
    memcpy(&(pInfo->DevConfigXml), pszProfileXml, cbProfileXml);

    // If we have Eap profile xml, then copy that at the end of the dev config and set cbUserDataOffset
    // to be the end of the profile data
    if (pszEapCredProfileXml)
    {
        memcpy(&(pInfo->DevConfigXml[cbProfileXml]), pszEapCredProfileXml, cbCredXml);
        pInfo->cbUserDataOffset = cbProfileXml;
    }

    memcpy(&(pConfig->TypeSpecificInfo), pInfo, cbInfo);
    pConfig->cbTypeSpecificInfo = offsetof(CspWiFiSpecificInfo, DevConfigXml[0]) + (pInfo->cbDevConfig);

    if (fExistingConfig)
    {
        cmr = CmUpdateConnectionConfigByName(pWiFiNetworkData->szSsid, CMCO_IMMEDIATE_APPLY, pConfig, cbConfig);
        CBRA(CMRE_SUCCESS == cmr);
    }
    else
    {
        cmr = CmAddConnectionConfig(pWiFiNetworkData->szSsid, pConfig, cbConfig);
        CBRA(CMRE_SUCCESS == cmr);
    }

    // NB: Technically we can now set pWiFiNetworkData->fExistingConfig = TRUE
    // but the only consumers of that field really just want to know if the
    // initial launch of the wizard was to edit an existing configuration

Error:
    if (pConfig)
    {
        VBR(NULL == LocalFree(pConfig));
    }
    if (pExistingConfig)
    {
        VBR(NULL == LocalFree(pExistingConfig));
    }
    if (pDetails)
    {
        VBR(NULL == LocalFree(pDetails));
    }
    if (pInfo)
    {
        VBR(NULL == LocalFree(pInfo));
    }
    if (pszProfileXml)
    {
        SysFreeString(pszProfileXml);//WpFreeMemory(pszProfileXml);
    }
    if (pszEapCredProfileXml)
    {
        SysFreeString(pszEapCredProfileXml);//WpFreeMemory(pszEapCredProfileXml);
    }

    if (SUCCEEDED(hr))
    {
        hr = S_FALSE;//WIFI_S_CONFIGURED;
    }
    else
    {
        hr = E_UNEXPECTED;//WIFI_E_NOTCONFIGURED;
    }
        
    return hr;
}

BOOL IsOneXNetwork(__in const WIFI_NETWORKDATA *pNetworkData)
{
    HRESULT hr = S_OK;

    // NB: We don't assert on the WIFI_NDF_VALID_ONEXENABLED check because
    // we don't know if the network is OneXEnabled when configuring airing Wep-Open

    CBRA(pNetworkData);
    CBR(WIFI_NDF_VALID_ONEXENABLED & pNetworkData->dwFlags);
    CBR(pNetworkData->fOneXEnabled);

Error:    
    return SUCCEEDED(hr);
}

WIFI_KEYTYPE GetKeyType(__in const WIFI_NETWORKDATA *pNetworkData)
{
    WIFI_KEYTYPE keytype = WIFI_KT_UNKNOWN;

    ASSERT(pNetworkData);
    switch (pNetworkData->authalgo)
    {
        case DOT11_AUTH_ALGO_80211_OPEN:
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
            switch (pNetworkData->ciphalgo)
            {
                case DOT11_CIPHER_ALGO_NONE:
                    // We only expect to see ALGO_NONE with 80211_OPEN
                    keytype = WIFI_KT_NONE;
                    break;
                    
                case DOT11_CIPHER_ALGO_WEP:
                    if (IsOneXNetwork(pNetworkData))
                    {
                        keytype = WIFI_KT_NONE;
                    }
                    else
                    {
                        keytype = WIFI_KT_PASSPHRASE_WEP;
                    }
                    break;

                default:
                    // Not supported
                    ASSERT(FALSE);
                    break;
            }
            break;

        case DOT11_AUTH_ALGO_WPA_NONE:
        case DOT11_AUTH_ALGO_RSNA_PSK:
        case DOT11_AUTH_ALGO_WPA_PSK:
            switch (pNetworkData->ciphalgo)
            {
                case DOT11_CIPHER_ALGO_CCMP:
                case DOT11_CIPHER_ALGO_TKIP:
                    keytype = WIFI_KT_PASSPHRASE_WPA;
                    break;

                default:
                    // Not supported
                    ASSERT(FALSE);
                    break;
            }
            break;
                
        case DOT11_AUTH_ALGO_RSNA:
        case DOT11_AUTH_ALGO_WPA:
            switch (pNetworkData->ciphalgo)
            {
                case DOT11_CIPHER_ALGO_CCMP:
                case DOT11_CIPHER_ALGO_TKIP:
                    keytype = WIFI_KT_DOMAINUSERPASSWORD;
                    break;

                default:
                    // Not supported
                    ASSERT(FALSE);
                    break;
            }
            break;
            
        default:
            // What auth algorithm is this?
            ASSERT(FALSE);
            break;
    }

    return keytype;
}

HRESULT CheckWepKey(__in const PWLAN_AVAILABLE_NETWORK pNetworkData, LPCTSTR pszKey, DWORD dwKeyLength)
{
    HRESULT hr = S_OK;
    BOOL fXFormat = FALSE;
    DWORD i;

    CPREx(pszKey, E_INVALIDARG);
    CBR(dwKeyLength > 0);

    if (pNetworkData->dot11DefaultAuthAlgorithm == DOT11_AUTH_ALGO_WPA_PSK   ||
        pNetworkData->dot11DefaultAuthAlgorithm == DOT11_AUTH_ALGO_RSNA_PSK  ||
        (pNetworkData->dot11DefaultAuthAlgorithm == DOT11_AUTH_ALGO_WPA_NONE &&
        pNetworkData->dot11DefaultCipherAlgorithm == DOT11_CIPHER_ALGO_TKIP ||
        pNetworkData->dot11DefaultCipherAlgorithm == DOT11_CIPHER_ALGO_CCMP))
    {    //  User can only enter either 64 hex entries, or 8/63 any ASCII entries
        if (WIFI_KEYLENGTH_WPAPSK_HEX == dwKeyLength)
        {
            fXFormat = TRUE;
        }
        else if (dwKeyLength >= WIFI_KEYLENGTH_WPAPSK_MINCHAR && dwKeyLength <= WIFI_KEYLENGTH_WPAPSK_MAXCHAR)
        {
            pNetworkData->dwFlags |= WIFI_NDF_VALID_PASSPHRASE;
        }
        else
        {
            CHR(E_FAIL);
        }
    }
    else if (pNetworkData->dot11DefaultCipherAlgorithm == DOT11_CIPHER_ALGO_WEP)
    {    //  User can only enter either 10/26 hex entries, or 5/13 any ASCII entries
        if ((WIFI_KEYLENGTH_WEP_HEX1 == dwKeyLength) || (WIFI_KEYLENGTH_WEP_HEX2 == dwKeyLength))
        {
            fXFormat = TRUE;
        }
        else if ((WIFI_KEYLENGTH_WEP_CHAR1 != dwKeyLength) && (WIFI_KEYLENGTH_WEP_CHAR2 != dwKeyLength))
        {
            CHR(E_FAIL);
        }
    }

    if (fXFormat)
    {
        for (i = 0; i < dwKeyLength; i++)
        {
            CBR(isxdigit(pszKey[i]));
        }
    }

Error:
    return hr;
}

HRESULT CheckWepKey(__in WIFI_NETWORKDATA* pNetworkData, LPCTSTR pszKey, DWORD dwKeyLength)
{
    HRESULT hr = S_OK;
    BOOL fXFormat = FALSE;
    DWORD i;

    CPREx(pszKey, E_INVALIDARG);
    CBR(dwKeyLength > 0);

    if (pNetworkData->authalgo == DOT11_AUTH_ALGO_WPA_PSK   ||
        pNetworkData->authalgo == DOT11_AUTH_ALGO_RSNA_PSK  ||
        (pNetworkData->authalgo == DOT11_AUTH_ALGO_WPA_NONE &&
        pNetworkData->ciphalgo == DOT11_CIPHER_ALGO_TKIP ||
        pNetworkData->ciphalgo == DOT11_CIPHER_ALGO_CCMP))
    {    //  User can only enter either 64 hex entries, or 8/63 any ASCII entries
        if (WIFI_KEYLENGTH_WPAPSK_HEX == dwKeyLength)
        {
            fXFormat = TRUE;
        }
        else if (dwKeyLength >= WIFI_KEYLENGTH_WPAPSK_MINCHAR && dwKeyLength <= WIFI_KEYLENGTH_WPAPSK_MAXCHAR)
        {
            pNetworkData->dwFlags |= WIFI_NDF_VALID_PASSPHRASE;
        }
        else
        {
            CHR(E_FAIL);
        }
    }
    else if (pNetworkData->ciphalgo == DOT11_CIPHER_ALGO_WEP)
    {    //  User can only enter either 10/26 hex entries, or 5/13 any ASCII entries
        if ((WIFI_KEYLENGTH_WEP_HEX1 == dwKeyLength) || (WIFI_KEYLENGTH_WEP_HEX2 == dwKeyLength))
        {
            fXFormat = TRUE;
        }
        else if ((WIFI_KEYLENGTH_WEP_CHAR1 != dwKeyLength) && (WIFI_KEYLENGTH_WEP_CHAR2 != dwKeyLength))
        {
            CHR(E_FAIL);
        }
    }

    if (fXFormat)
    {
        for (i = 0; i < dwKeyLength; i++)
        {
            CBR(isxdigit(pszKey[i]));
        }
    }

Error:
    return hr;
}

