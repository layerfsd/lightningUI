#include "AppConfigProxy.h"
#include "../Include/CVTE_IoCtl.h"
#include "../Include/CVTE_AppProxy.h"

CAppConfigProxy::CAppConfigProxy(void)
{
	m_hOPD = NULL;
	m_hOPD = AppProxyOpen();

	if(m_hOPD == NULL)
	{
		RETAILMSG(1, (TEXT("APP:ERROR:Oemsector: [CAppConfigProxy] CreateFile[OPD1] Failed.\r\n")));
		m_bOPDReady = FALSE;
	}
	else
	{
		m_bOPDReady = TRUE;
		memset(&m_OemSectorParam, 0, sizeof(m_OemSectorParam));
	}
}

CAppConfigProxy::~CAppConfigProxy(void)
{
	m_bOPDReady = FALSE;
}

BOOL CAppConfigProxy::SetOemSector()
{
	if(!m_bOPDReady)
		return FALSE;

	BOOL bValueChanged = FALSE;

	//Refresh the oemsector param which will be saved in the TOC.
	if(m_OemSectorParam.LCDGamma != GetCurParam()->iLCDGammaCurrent)
	{
		bValueChanged = TRUE;
		m_OemSectorParam.LCDGamma =  GetCurParam()->iLCDGammaCurrent;
		RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [SetOemSector]  LCDGamma[%d] ##############\r\n"),m_OemSectorParam.LCDGamma));
	}
//		if(m_OemSectorParam.RVChannel != (BYTE)GetCurParam()->dwCameraType)
//		{
//			bValueChanged = TRUE;
//			m_OemSectorParam.RVChannel = (BYTE)GetCurParam()->dwCameraType;
//		}
	if(m_OemSectorParam.ColorSignal != GetCurParam()->iRearStandard)
	{
		bValueChanged = TRUE;
		m_OemSectorParam.ColorSignal =  GetCurParam()->iRearStandard;
	    RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [SetOemSector]  ColorSignal[%d] ##############\r\n"),m_OemSectorParam.ColorSignal));
	}

	if(m_OemSectorParam.Backlight != GetCurParam()->dwBacklight)
	{
		bValueChanged = TRUE;
		m_OemSectorParam.Backlight =  GetCurParam()->dwBacklight;
		RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [SetOemSector]  Backlight[%d] ##############\r\n"),m_OemSectorParam.Backlight));
	}

	if(m_OemSectorParam.PowerStatus != GetCurParam()->iPowerStatusBeforeACCOff)
	{
		bValueChanged = TRUE;
		m_OemSectorParam.PowerStatus =  GetCurParam()->iPowerStatusBeforeACCOff;
		RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [SetOemSector]  PowerStatus[%d] ##############\r\n"),m_OemSectorParam.PowerStatus));
	}

	if(!bValueChanged)
		return FALSE;

	//Get Current OEM 
	OEMSECTOR   oemsector;
	memset(&oemsector, 0, sizeof(oemsector));
	if(!AppProxyGetWholeOem(m_hOPD, &oemsector, sizeof(oemsector)))
	{
		RETAILMSG(1, (TEXT("APP:ERR:Oemsector: Get whole oemsector failed. err=%d\r\n"), GetLastError()));
		return FALSE;
	}

	oemsector.LCDGamma = m_OemSectorParam.LCDGamma;
	oemsector.ColorSignal = m_OemSectorParam.ColorSignal;
	oemsector.Backlight = m_OemSectorParam.Backlight;
	oemsector.PowerStatus = m_OemSectorParam.PowerStatus;

	if(!AppProxySetWholeOem(m_hOPD, &oemsector, sizeof(oemsector)))
	{
		RETAILMSG(1, (TEXT("APP:ERR:Oemsector: Set whole oemsector failed. err=%d\r\n"), GetLastError()));
		return FALSE;
	}

	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [SetOemSector] Set whole oemsector successfully. \r\n")));
	return TRUE;
}

BOOL CAppConfigProxy::GetOemSector()
{
	if(!m_bOPDReady)
		return FALSE;

	if(!AppProxyGetWholeOem(m_hOPD, &m_OemSectorParam, sizeof(m_OemSectorParam)))
	{
		RETAILMSG(1, (TEXT("APP:ERROR:Oemsector: [GetOemSector] Get whole oemsector failed. err=%d\r\n"), GetLastError()));
		return FALSE;
	}

	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: VersionID    [%S]\r\n"), m_OemSectorParam.VersionID));
	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: PowerStatus  [%d]\r\n"), m_OemSectorParam.PowerStatus));
	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: RVChannel    [%d]\r\n"), m_OemSectorParam.RVChannel));
	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: LCDGamma     [%d]\r\n"), m_OemSectorParam.LCDGamma));
	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: ColorSignal  [%d]\r\n"), m_OemSectorParam.ColorSignal));
	RETAILMSG(1, (TEXT("APP:MSG:Oemsector: [GetOemSector]: Backlight    [%d]\r\n"), m_OemSectorParam.Backlight));

	//Refresh the Reg param.
	//do not update gamma from TOC, use registry default value
	//GetCurParam()->iLCDGammaCurrent = m_OemSectorParam.LCDGamma;

	GetCurParam()->dwCameraType = m_OemSectorParam.RVChannel;
	GetCurParam()->iRearStandard = m_OemSectorParam.ColorSignal;
    //GetCurParam()->iLCDGammaCurrent = m_OemSectorParam.LCDGamma;
	GetCurParam()->dwBacklight = m_OemSectorParam.Backlight;

	return TRUE;
}

OEMSECTOR * CAppConfigProxy::GetOemSectorParam()
{
	return &m_OemSectorParam;
}

BOOL CAppConfigProxy::SaveAppConfig()
{
	SaveConfig();
	SetOemSector();
	return TRUE;
}


