#include <Windows.h>
#include "IPCApp.h"

CIPCApp& CIPCApp::GetInstance() 
{
	static CIPCApp s_Instance;

	return s_Instance;
}

CIPCApp::CIPCApp(void)
: m_pNotification( NULL ),
m_windowshowstatus(false)
{
}

CIPCApp::~CIPCApp(void)
{
}

bool CIPCApp::Init(IIPCNotification* pNotification)
{
	if ( !m_AppMsgQ.Create(kMsgQName_WeblinkAppMsgQ, MAX_MESSAGE_SIZE, WriteMode) )
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Create 'AppMsgQ' Message Queue!\n" )) );
		return false;
	}

	if ( !m_ClientMsgQ.Create(kMsgQName_WeblinkClientMsgQ, MAX_MESSAGE_SIZE, ReadMode) )
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Create 'ClientMsgQ' Message Queue!\n" ) ));
		return false;
	}

	if (!m_ClientMsgQ.SetMsgCallBack( ClientMsgQCallBackProc, this))
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to set 'ClientMsgQ' callback function!\n" ) ));
		return false;
	}

	m_pNotification = pNotification;

	return true;
}

BOOL CIPCApp::ShowWindow(bool show)
{
	if ( m_windowshowstatus )
	{
//			if (show)
//			{
//				OnSetCurrentApp("com.sygic.incar://", "");
//			}
		OnWindowControl(show);
	}

	return m_windowshowstatus?TRUE:FALSE;
}

void CIPCApp::OnWindowControl(bool show)
{
	cJSON * json = NULL;
	cJSON * param = NULL;
	char * s = NULL;

	param = cJSON_CreateObject();

	cJSON_AddBoolToObject(param, kMsgParam_show, show);

	json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, kMsgKey_cmd, kMsgCMD_WindowControl);
	cJSON_AddItemToObject(json, kMsgKey_param, param);

	s = cJSON_Print(json);
	m_AppMsgQ.Write(s, strlen(s)+1);

	if (json)
	{
		cJSON_Delete(json);
		json = NULL;
	}
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void CIPCApp::OnSetCurrentApp(const char * appID, const char * appParams)
{
	cJSON * json = NULL;
	cJSON * param = NULL;
	char * s = NULL;

	param = cJSON_CreateObject();

	cJSON_AddStringToObject(param, kMsgParam_appID, appID);
	cJSON_AddStringToObject(param, kMsgParam_appParams, appParams);

	json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, kMsgKey_cmd, kMsgCMD_SetCurrentApp);
	cJSON_AddItemToObject(json, kMsgKey_param, param);

	s = cJSON_Print(json);
	m_AppMsgQ.Write(s, strlen(s)+1);

	if (json)
	{
		cJSON_Delete(json);
		json = NULL;
	}
	if (s)
	{
		free(s);
		s = NULL;
	}
}


void CIPCApp::OnBack()
{
	cJSON * json = NULL;
	cJSON * param = NULL;
	char * s = NULL;

	param = cJSON_CreateObject();

	cJSON_AddStringToObject(param, kMsgParam_button, kMsgButton_back);

	json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, kMsgKey_cmd, kMsgCMD_ButtonKey);
	cJSON_AddItemToObject(json, kMsgKey_param, param);

	s = cJSON_Print(json);
	m_AppMsgQ.Write(s, strlen(s)+1);

	if (json)
	{
		cJSON_Delete(json);
		json = NULL;
	}
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void CIPCApp::OnHome()
{
	cJSON * json = NULL;
	cJSON * param = NULL;
	char * s = NULL;

	param = cJSON_CreateObject();

	cJSON_AddStringToObject(param, kMsgParam_button, kMsgButton_home);

	json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, kMsgKey_cmd, kMsgCMD_ButtonKey);
	cJSON_AddItemToObject(json, kMsgKey_param, param);

	s = cJSON_Print(json);
	m_AppMsgQ.Write(s, strlen(s)+1);

	if (json)
	{
		cJSON_Delete(json);
		json = NULL;
	}
	if (s)
	{
		free(s);
		s = NULL;
	}
}

BOOL CIPCApp::ClientMsgQCallBackProc(PVOID Param, PVOID pData, DWORD dwSize)
{
	CIPCApp* pClient = (CIPCApp*)Param;
	return pClient->ClientMsgQCallBack(pData, dwSize);
}

BOOL CIPCApp::ClientMsgQCallBack(PVOID pData, DWORD dwSize)
{
	BOOL bRet = FALSE;
	cJSON * json = NULL;
	cJSON * cmd = NULL;
	cJSON * param = NULL;

	json = cJSON_Parse((char *)pData);
	if (json == NULL)
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse pData!\n" )) );
		goto exit;
	}
	cmd = cJSON_GetObjectItem(json, kMsgKey_cmd);
	if (cmd == NULL)
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse cmd!\n" )) );
		goto exit;
	}
	if (cmd->type != cJSON_String)
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse cmd's type!\n" )) );
		goto exit;
	}
	param = cJSON_GetObjectItem(json, kMsgKey_param);
	if (param == NULL)
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse param!\n" )) );
		goto exit;
	}
	if (param->type != cJSON_Object)
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse param's type!\n" )) );
		goto exit;
	}

	if( 0 ) {}
	else if( _stricmp( cmd->valuestring, kMsgCMD_Init )	== 0 ) CMDProcessInit( param );
	else if( _stricmp( cmd->valuestring, kMsgCMD_Connect )	== 0 ) CMDProcessConnect( param );
	else if( _stricmp( cmd->valuestring, kMsgCMD_WindowStatus )	== 0 ) CMDProcessWindowStatus( param ); 
	else { printf( "[CIPCApp] Bad CMD: '%s'\n", cmd->valuestring ); goto exit; }

	bRet = TRUE;

exit:
	if (json)
	{
		cJSON_Delete(json);
		json = NULL;
	}
	return bRet;
}

void CIPCApp::CMDProcessInit( cJSON * param )
{
    cJSON * init = NULL;

	if ( param == NULL )
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse param!\n" )) );
		goto exit;
	}
    init = cJSON_GetObjectItem(param, kMsgParam_init);
	if ( init == NULL )
	{
		printf("[CIPCApp] Unable to Parse init! %s\n", cJSON_GetErrorPtr());
		goto exit;
	}
    if ( init->type == cJSON_True )
    {
        if (m_pNotification)
		{
        	m_pNotification->OnInit(true);
		}
    }
    else if ( init->type == cJSON_False ) 
    {
        if (m_pNotification)
		{
        	m_pNotification->OnInit(false);
		}
    }
	else
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse init's type!\n" )) );
	}
    
exit:
    return;
}

void CIPCApp::CMDProcessConnect( cJSON * param )
{
	cJSON * connect = NULL;

	if ( param == NULL )
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse param!\n" )) );
		goto exit;
	}
    connect = cJSON_GetObjectItem(param, kMsgParam_connect);
	if ( connect == NULL )
	{
		printf("[CIPCApp] Unable to Parse connect! %s\n", cJSON_GetErrorPtr());
		goto exit;
	}
    if ( connect->type == cJSON_True )
    {
		if (m_pNotification)
		{
        	m_pNotification->OnConnect(true);
		}
    }
    else if ( connect->type == cJSON_False ) 
    {
        if (m_pNotification)
		{
        	m_pNotification->OnConnect(false);
		}
    }
	else
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse connect's type!\n" )) );
	}
    
exit:
    return;

}

void CIPCApp::CMDProcessWindowStatus( cJSON * param )
{
	cJSON * show = NULL;

	if ( param == NULL )
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse param!\n" )) );
		goto exit;
	}
    show = cJSON_GetObjectItem(param, kMsgParam_show);
	if ( show == NULL )
	{
		printf("[CIPCApp] Unable to Parse show! %s\n", cJSON_GetErrorPtr());
		goto exit;
	}
    if ( show->type == cJSON_True )
    {
		m_windowshowstatus = true;
		m_pNotification->OnWindowStatus(true);
    }
    else if ( show->type == cJSON_False ) 
    {
        m_windowshowstatus = false;
		m_pNotification->OnWindowStatus(false);
    }
	else
	{
		RETAILMSG( 1, (_T( "[CIPCApp] Unable to Parse show's type!\n" )) );
	}
    
exit:
    return;
}

