#include "PaneliPodWrapper.h"
#include "../../resource.h"

CPaneliPodWrapper::CPaneliPodWrapper(void)
{
	m_ppMain = NULL;
	m_ppLibrary = NULL;

	m_bDeviceDonnected = FALSE;

	m_repeatMode = repeat_off;
	m_shuffleMode = shuffle_off;

	m_dwCurTrackLength = 0;
	m_dwCurTrackTime = 0;

	m_dwCurTrackIndex = -1;//0;
	m_dwTotalTracks = -1;//0;
	m_browser_level = 0;

	m_bSpeechOn = FALSE;
	m_bPhoneCallActive = FALSE;

	m_playState = PLAY_STATE_INVALID;
	m_playStateBeforeSpeech = PLAY_STATE_INVALID;
	m_playStateBeforePhonecall = PLAY_STATE_INVALID;
	m_bHasAlbumArt = FALSE;
	m_bRefreshPlayinfoAfterConnect = FALSE;
	m_dwRefreshPlayinfoRef = 0;
	m_dwRefreshPlayinfoTick = 0;

	memset(m_strTrackName,0,MAX_STRING);
	memset(m_strArtist,0,MAX_STRING);
	memset(m_strAlbum,0,MAX_STRING);
	m_bTrackNameValid = FALSE;
	m_bArtistValid = FALSE;
	m_bAlbumValid = FALSE;

	mIpodControl = NULL;
	mAuthenticationListener = NULL;
	mBrowserListener = NULL;
	mPlayerListener = NULL;
	miOSCommunicationListener = NULL;
	mRemoteControlListener = NULL;

	m_bPlayingBeforeSpeech = FALSE;
}
CPaneliPodWrapper::~CPaneliPodWrapper(void)
{

}

void CPaneliPodWrapper::Render()
{
	if(!BeginRender())return;


	EndRender();
}

BOOL CPaneliPodWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	IniAPPlayer();

	return TRUE;
}
void CPaneliPodWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	if(!lpDeviceName)
		return;

	RETAILMSG(DEBUG_IPOD, (TEXT("CPanelIPod::OnDeviceArrivedReady :%s\r\n"), lpDeviceName));

	if(wcsstr(lpDeviceName,_T("LNG")))//
	{
		//plugInDetection();
		if(m_ppMain)
			m_ppMain->SetConnectStatus(TRUE);

		if(UI_CLASS_IPOD != GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
		{
			SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN);
		}
	}
}
void CPaneliPodWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_IPOD, (TEXT("CPanelIPod::OnDeviceRemovedReady :%s\r\n"), lpDeviceName));

	if(wcsstr(lpDeviceName,_T("LNG")))//
	{
		plugOutDetection();

		if(m_ppMain)
			m_ppMain->SetConnectStatus(FALSE);

		//if(UI_CLASS_IPOD == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
		if(m_pSkinManager)
		{
			//20160428
			//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
			//SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);

			//reset media widget:
			UINT media_type=MEDIA_TYPE_INVALID;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
			if(media_type == MEDIA_TYPE_IPOD)
			{
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)MEDIA_TYPE_INVALID);

				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,NULL);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,NULL);
			}
			m_bHasAlbumArt=FALSE;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ALBUMART,UIOPRT_SETVALUE,&m_bHasAlbumArt);
		}
	}
}
void CPaneliPodWrapper::OniPodCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IPOD_CMD_SELECT_INDEX:
		{
			OnSelectItem(lParam);
		}
		break;
	case IPOD_CMD_BROWSEUP:
		{
			OnBrowserUp();
		}
		break;
	case IPOD_CMD_SET_POSITION:
		{

		}
		break;
	case IPOD_CMD_SET_PLAYMODE:
		{
			if(mIpodControl && m_bDeviceDonnected)
			{
				switch (lParam)
				{
				case IPOD_PLAYMODE_OFF:
					mIpodControl->playerSetRepeatMode(RepeatMode::RepeatOff());
					mIpodControl->playerSetShuffleMode(ShuffleMode::ShuffleOff());
					break;
				case IPOD_PLAYMODE_REPEAT_ALL:
					mIpodControl->playerSetRepeatMode(RepeatMode::RepeatAllTracks());
					mIpodControl->playerSetShuffleMode(ShuffleMode::ShuffleOff());
					break;
				case IPOD_PLAYMODE_REPEAT_ONE:
					mIpodControl->playerSetRepeatMode(RepeatMode::RepeatOneTrack());
					mIpodControl->playerSetShuffleMode(ShuffleMode::ShuffleOff());
					break;
				case IPOD_PLAYMODE_SHUFFLE:
					mIpodControl->playerSetRepeatMode(RepeatMode::RepeatOff());
					mIpodControl->playerSetShuffleMode(ShuffleMode::ShuffleTracks());
					break;
				}
			}
		}
		break;
	case IPOD_CMD_PLAY_CONTROL:
		{
			if(m_bDeviceDonnected)
			{
				switch (lParam)
				{
				case IPOD_CONTROL_STOP:
					Stop();
					break;
				case IPOD_CONTROL_PAUSE:
					Pause();
					break;
				case IPOD_CONTROL_PLAY:
					Play();
					break;
				case IPOD_CONTROL_NEXT:
					PlayNext();
					break;
				case IPOD_CONTROL_PREVIOUS:
					PlayPrevious();
					break;
				}
			}
			else
			{
				plugInDetection();
				if(!m_bDeviceDonnected)
				{
					//20160428
					//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);

					//quit ipod:
					if(UI_CLASS_IPOD == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
					{
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
					}
				}
			}
		}
		break;
	}
}
void CPaneliPodWrapper::OniPodMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_IPOD,(_T("CPanelIPod::OniPodMsg  wParam:%d,lParam:%d\r\n"),wParam,lParam));

	switch (wParam)
	{
	case IPOD_AUTHENTICATION:
		{
			BOOL bStatus = lParam;
			if(bStatus)
			{
				if(mIpodControl)
				{
					RETAILMSG(DEBUG_IPOD,(_T("====>:browserStart\r\n")));

					mIpodControl->browserStart();
				}
			}
		}
		break;
	case IPOD_PLAY_STATE:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_PLAY_STATE %d\r\n"),lParam));

			if (m_playState != lParam)
			{
				m_playState = (PLAY_STATE)lParam;
				m_ppMain->SetPlayState(lParam == PLAY_STATE_PLAYING);

				if(m_playState==PLAY_STATE_PLAYING)
				{
					//reset playinfo:
					m_bTrackNameValid = FALSE;
					m_bArtistValid = FALSE;
					m_bAlbumValid = FALSE;
					m_dwRefreshPlayinfoRef = 0;

					//Play();
					MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,300);			
				}		
			}
		}
		break;
	case IPOD_REPEAT:
		{
			m_repeatMode = repeat_mode(lParam);

			RETAILMSG(DEBUG_IPOD,(_T("====>:repeat_mode %d\r\n"),lParam));

			//
			if(m_repeatMode == repeat_one)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_REPEAT_ONE);
			else if(m_shuffleMode != shuffle_off)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_SHUFFLE);
			else if(m_repeatMode == repeat_all)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_REPEAT_ALL);
			else
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_OFF);
		}
		break;
	case IPOD_SHUFFLE:
		{
			m_shuffleMode = shuffle_mode(lParam);

			RETAILMSG(DEBUG_IPOD,(_T("====>:shuffle_mode %d\r\n"),lParam));

			//
			if(m_repeatMode == repeat_one)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_REPEAT_ONE);
			else if(m_shuffleMode != shuffle_off)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_SHUFFLE);
			else if(m_repeatMode == repeat_all)
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_REPEAT_ALL);
			else
				m_ppMain->SetPlayMode(IPOD_PLAYMODE_OFF);
		}
		break;
	case IPOD_TRACK_NAME:
		{
			if(lParam && strcmp(m_strTrackName,(char*)lParam))
			{
				wchar_t temp[MAX_STRING]={0};
				UTF8TOUnicode((char*)lParam,-1,temp,MAX_STRING);
				m_ppMain->SetTitle(temp);

				RETAILMSG(DEBUG_IPOD,(_T("====>:SetTitle %s\r\n"),temp));
				strcpy_s(m_strTrackName,MAX_STRING,(char*)lParam);
				m_ppMain->UpdateWidget();
				m_bTrackNameValid = TRUE;
			}
		}
		break;
	case IPOD_TRACK_ARTIST:
		{
			if(lParam && strcmp(m_strArtist,(char*)lParam))
			{
				wchar_t temp[MAX_STRING]={0};
				UTF8TOUnicode((char*)lParam,-1,temp,MAX_STRING);
				m_ppMain->SetArtist(temp);
				RETAILMSG(DEBUG_IPOD,(_T("====>:SetArtist %s\r\n"),temp));
				strcpy_s(m_strArtist,MAX_STRING,(char*)lParam);
				m_ppMain->UpdateWidget();
				m_bArtistValid = TRUE;
			}
		}
		break;
	case IPOD_TRACK_ALBUM:
		{
			if(lParam && strcmp(m_strAlbum,(char*)lParam))
			{
				wchar_t temp[MAX_STRING]={0};
				UTF8TOUnicode((char*)lParam,-1,temp,MAX_STRING);
				m_ppMain->SetAlbum(temp);
				RETAILMSG(DEBUG_IPOD,(_T("====>:SetAlbum %s\r\n"),temp));
				strcpy_s(m_strAlbum,MAX_STRING,(char*)lParam);
				m_bAlbumValid = TRUE;
			}
		}
		break;
	case IPOD_EXTENDEDUIMODEREADY:
		{
			BOOL bStatus = lParam;
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_EXTENDEDUIMODEREADY: 0x%x\r\n"),lParam));
			if(bStatus)
			{
				if(mIpodControl)
				{
					mIpodControl->playerInit();
					mIpodControl->browserRoot();

					/*
					Sleep(1000);
					mIpodControl->playerInit();

					Sleep(1000);
// 					mIpodControl->playerResumePlay();

// 					mIpodControl->playerGetPlayingTrackIndexFromIpod();
// 					mIpodControl->playerGetPlayingTrackIndex();
// 					mIpodControl->playerGetRepeatMode();
// 					mIpodControl->playerGetShuffleMode();
// 					Sleep(100);
					mIpodControl->playerSetRepeatMode(RepeatMode::RepeatAllTracks());
					mIpodControl->playerSetShuffleMode(ShuffleMode::ShuffleOff());

					Sleep(200);
					RETAILMSG(DEBUG_IPOD,(_T("====>:ipod ResumePlay\r\n")));
					mIpodControl->playerResumePlay();

					mIpodControl->playerGetPlayingTrackIndexFromIpod();
					//mIpodControl->playerGetPlayingTrackIndex();
					*/
					
				}
			}
		}
		break;
	case IPOD_TRACK_NUM:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_TRACK_NUM: %d\r\n"),lParam));
			if(m_dwTotalTracks != lParam)
			{
				m_dwTotalTracks = lParam;

				wchar_t temp[32]={0};
				if(m_dwTotalTracks != -1)
					swprintf_s(temp,32,_T("%d/%d"),m_dwCurTrackIndex+1,m_dwTotalTracks);
				else
					swprintf_s(temp,32,_T("%d"),m_dwCurTrackIndex+1);

				m_ppMain->SetNowPlayingTitle(temp);
			}
		}
		break;
	case IPOD_TRACK_INDEX:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_TRACK_INDEX: %d\r\n"),lParam));

			UINT32 trackIndex = lParam;

			//reset playinfo:
			m_bTrackNameValid = FALSE;
			m_bArtistValid = FALSE;
			m_bAlbumValid = FALSE;
			m_dwRefreshPlayinfoRef = 0;

			if(trackIndex<0x80000000 && m_dwCurTrackIndex!=trackIndex)
			{
				m_dwCurTrackIndex = trackIndex;

				if(!m_bRefreshPlayinfoAfterConnect)
				{
					m_bRefreshPlayinfoAfterConnect = TRUE;
					if(GetTickCount()-m_dwRefreshPlayinfoTick > 2000)
					{
						m_dwRefreshPlayinfoTick = GetTickCount();
						//mIpodControl->playerGetPlayingTrackNum();
						//mIpodControl->playerGetTotalTime();
						mIpodControl->playerGetPlayingTrackName(trackIndex);
						mIpodControl->playerGetPlayingTrackAlbumName(trackIndex);
						mIpodControl->playerGetPlayingTrackArtistName(trackIndex);	
						mIpodControl->playerGetArtwork(trackIndex);
					}
				}

				//
				wchar_t temp[32]={0};
				if(m_dwTotalTracks != -1)
					swprintf_s(temp,32,_T("%d/%d"),m_dwCurTrackIndex+1,m_dwTotalTracks);
				else
					swprintf_s(temp,32,_T("%d"),m_dwCurTrackIndex+1);

				m_ppMain->SetNowPlayingTitle(temp);

				//
				DWORD dwData1=((BYTE(trackIndex))<<24)+((trackIndex>>8)<<16)+((BYTE(m_dwTotalTracks))<<8)+((BYTE(m_dwTotalTracks>>8))<<0);
				::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,dwData1,(0x0<<16)+0x12);
			}
		}
		break;
	case IPOD_OPENFINISH:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_OPENFINISH: 0x%x\r\n"),lParam));

			UINT32 numberofRecords = lParam;
			BOOL bPlayable = mIpodControl->browserIsPlayable();

// 			if(bPlayable)
// 				m_dwTotalTracks = lParam;

			if (numberofRecords != 0)
				mIpodControl->browserGetRecords(0, numberofRecords);

			m_ppLibrary->onOpenFinish(numberofRecords,bPlayable);

			//start playback:
			mIpodControl->playerResumePlay();
			mIpodControl->playerGetPlayingTrackIndexFromIpod();
		}
		break;
	case IPOD_TRACK_LENGTH:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_TRACK_LENGTH: %d\r\n"),lParam));

			if(lParam<0x400000)
				m_dwCurTrackLength = lParam;
			else
				m_dwCurTrackLength = 0;
		}
		break;
	case IPOD_TRACK_TIME:
		{
			RETAILMSG(DEBUG_IPOD,(_T("====>:IPOD_TRACK_TIME: %d\r\n"),lParam));

			m_dwCurTrackTime = lParam;
			
			//refresh playinfo:
			if(!m_pSkinManager->GetGUI()->IsAnimating())
			{
				if(GetTickCount()-m_dwRefreshPlayinfoTick > 2000)
				{
					m_dwRefreshPlayinfoTick = GetTickCount();

					if((m_dwRefreshPlayinfoRef < 3))
					{
						RETAILMSG(DEBUG_IPOD,(_T("\r\n####refresh playinfo, idx=%d, cnt=%d####\r\n\r\n"),m_dwCurTrackIndex,m_dwRefreshPlayinfoRef));
						m_dwRefreshPlayinfoRef++;

						mIpodControl->playerGetPlayingTrackNum();

						mIpodControl->playerGetTotalTime();

						if(!m_bTrackNameValid)
							mIpodControl->playerGetPlayingTrackName(m_dwCurTrackIndex);

						if(!m_bAlbumValid)
							mIpodControl->playerGetPlayingTrackAlbumName(m_dwCurTrackIndex);

						if(!m_bArtistValid)
							mIpodControl->playerGetPlayingTrackArtistName(m_dwCurTrackIndex);

						mIpodControl->playerGetArtwork(m_dwCurTrackIndex);
					}
					else
					{
// 						mIpodControl->playerGetPlayingTrackIndexFromIpod();
// 						mIpodControl->playerGetPlayingTrackName(m_dwCurTrackIndex);
// 						//mIpodControl->playerGetPlayingTrackNum();
// 
// 						ShuffleMode shuff=mIpodControl->playerGetShuffleMode();
// 						RepeatMode repeat=mIpodControl->playerGetRepeatMode();
// 						RETAILMSG(DEBUG_IPOD,(_T("\r\n####refresh playinfo, shuff=%d, repeat=%d####\r\n\r\n"),shuff.get(),repeat.get()));
					}
				}
			}

			if(m_dwCurTrackLength)
			{
				m_ppMain->SetProgressValue(m_dwCurTrackTime*512/m_dwCurTrackLength);
				m_ppMain->SetElapsedTime(m_dwCurTrackTime);
				m_ppMain->SetRemainTime(m_dwCurTrackLength - m_dwCurTrackTime);
			}
			else
			{
				m_ppMain->SetProgressValue(0);
				m_ppMain->SetElapsedTime(0);
				m_ppMain->SetRemainTime(0);
			}
		}
		break;
	case IPOD_TRACK_ARTWORK:
		{
			Artwork* artwork = (Artwork*)lParam;
			if(artwork)
			{
				RETAILMSG(DEBUG_IPOD,(_T("##artwork cx:%d,cy:%d, pixelFormatCode:%d,rowSizeInBytes:%d\r\n"),
					artwork->artworkDataHeader.WidthInPixels,artwork->artworkDataHeader.HeightInPixels,artwork->artworkDataHeader.pixelFormatCode,artwork->artworkDataHeader.rowSizeInBytes));
				int cx=min(artwork->artworkDataHeader.WidthInPixels,512);
				int cy=min(artwork->artworkDataHeader.HeightInPixels,512);
				m_pSkinManager->ReloadTextureFromBuff(TEXID_003,&artwork->artworkData[0],0,cx,cy,max(2,artwork->artworkDataHeader.rowSizeInBytes/artwork->artworkDataHeader.WidthInPixels));

				m_ppMain->SetArtWorkStatus(TRUE);
				m_bHasAlbumArt = TRUE;
				if(m_pSkinManager)
				{
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ALBUMART,UIOPRT_SETVALUE,&m_bHasAlbumArt);
				}
			}
		}
		break;
	case IPOD_TRACK_ARTWORK_STATUS:
		{
			RETAILMSG(DEBUG_IPOD,(_T("##artwork status : %d\r\n"),lParam));
			if(!lParam)
			{
				m_ppMain->SetArtWorkStatus(lParam);

				//reset:
				if(m_bHasAlbumArt)
				{
					RETAILMSG(DEBUG_IPOD,(_T("##ipod:: reload album art\r\n")));
					m_pSkinManager->ReloadTextureFromSkin(TEXID_003,m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName.String());
				}

				m_bHasAlbumArt=FALSE;
				if(m_pSkinManager)
				{
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ALBUMART,UIOPRT_SETVALUE,&m_bHasAlbumArt);
				}
			}
		}
		break;
	case IPOD_RECORDREADY_ROOT:
		{
			ItemInfoRoot *pItem=(ItemInfoRoot*)lParam;
			if(pItem)
				m_ppLibrary->onRecordReady(pItem->index,pItem->recordId);
		}
		break;
	case IPOD_RECORDREADY_TOPLEVEL:
		{
			ItemInfoTopLevel *pItem=(ItemInfoTopLevel*)lParam;
			if(pItem)
				m_ppLibrary->onRecordReady(pItem->index,pItem->recordId);
		}
		break;
	case IPOD_RECORDREADY_RECORD:
		{
			ItemInfoRecord *pItem=(ItemInfoRecord*)lParam;
			if(pItem)
				m_ppLibrary->onRecordReady(pItem->index,pItem->record);
		}
		break;
	case IPOD_READFINISH:
		{
			if(m_browser_level == 0)//root
			{
				if (!mIpodControl->browserIsPlayable())
				{
					//clear list:
					m_ppLibrary->ClearList();

					mIpodControl->browserOpen(0);
					m_browser_level += 1;

					m_ppLibrary->SetBrowserLevel(m_browser_level);
				}
			}
		}
		break;
	}
}
void CPaneliPodWrapper::LinkPages(CPaneliPodMain *pMain,CPaneliPodLibrary *pLibrary)
{
	m_ppMain = pMain;
	m_ppLibrary = pLibrary;
}

void CALLBACK CPaneliPodWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPaneliPodWrapper *panel=(CPaneliPodWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPaneliPodWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BTN_HOME:
		break;
	}
}

void CPaneliPodWrapper::IniAPPlayer()
{
	mIpodControl = new IpodControl();
	mAuthenticationListener = new IapAuthenticationListener(m_hMainWnd,LPARAM(this));
	mBrowserListener = new IapBrowserListener(m_hMainWnd,LPARAM(this));
	mPlayerListener = new IapPlayerListener(m_hMainWnd,LPARAM(this));
	miOSCommunicationListener = new IapIOSCommunicationListener();
	mRemoteControlListener = new IapRemoteControlListener(m_hMainWnd,LPARAM(this));
	mIpodControl->setUp(USB, mAuthenticationListener, mBrowserListener, mPlayerListener, miOSCommunicationListener,mRemoteControlListener);

}
void CPaneliPodWrapper::DeiniAPPlayer()
{
	if(mIpodControl)
		mIpodControl->release();

	if(miOSCommunicationListener)
		delete miOSCommunicationListener;
	if(mAuthenticationListener)
		delete mAuthenticationListener;
	if(mBrowserListener)
		delete mBrowserListener;
	if(mPlayerListener)
		delete mPlayerListener;
	if(mIpodControl)
		delete mIpodControl;
}

void CPaneliPodWrapper::plugInDetection()
{
	if(!m_bDeviceDonnected)
	{
		if (mIpodControl->constructBuilder())
		{
			m_bDeviceDonnected = true;
			mIpodControl->startAuthentication();
			if(m_ppMain)
				m_ppMain->SetConnectStatus(TRUE);
		}
		else
		{
			RETAILMSG(DEBUG_IPOD,(_T("CPanelIPod::plugInDetection  FAILED!!\r\n")));
		}
	}

}

void CPaneliPodWrapper::plugOutDetection()
{
	if (mIpodControl->plugOutDetect())
	{
		m_bDeviceDonnected = false;
		//b_resumePlay_on_uiModeReady = true;
		mIpodControl->releaseListener();

		m_browser_level = 0;
		m_ppLibrary->SetBrowserLevel(0);

		m_dwCurTrackLength = 0;
		m_dwCurTrackTime = 0;

		m_dwCurTrackIndex = -1;//0;
		m_dwTotalTracks = -1;//0;

		m_repeatMode = repeat_off;
		m_shuffleMode = shuffle_off;

		m_playState = PLAY_STATE_INVALID;
		m_playStateBeforeSpeech = PLAY_STATE_INVALID;
		m_playStateBeforePhonecall = PLAY_STATE_INVALID;
		m_bHasAlbumArt = FALSE;
		m_bRefreshPlayinfoAfterConnect = FALSE;
		m_dwRefreshPlayinfoRef = 0;
		m_dwRefreshPlayinfoTick = 0;

		memset(m_strTrackName,0,MAX_STRING);
		memset(m_strArtist,0,MAX_STRING);
		memset(m_strAlbum,0,MAX_STRING);
		m_bTrackNameValid = FALSE;
		m_bArtistValid = FALSE;
		m_bAlbumValid = FALSE;

		Stop();
	}
}


BOOL CPaneliPodWrapper::IsPlaying()
{
	return (m_playState == PLAY_STATE_PLAYING || m_playState == PLAY_STATE_FFW || m_playState == PLAY_STATE_REW);
}

void CPaneliPodWrapper::Play()
{
	if(!m_bSpeechOn && !m_bPhoneCallActive)
	{
		if(mIpodControl)
		{
			MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,300);
			RETAILMSG(DEBUG_IPOD, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
			switch (mIpodControl->playerGetPlayState())
			{
			case PLAY_STATE_FFW:
			case PLAY_STATE_REW:
				mIpodControl->playerEndFFRewPlay();
				break;
			case PLAY_STATE_PAUSED:
				mIpodControl->playerResumePlay();
				break;
			case PLAY_STATE_STOPPED:
				mIpodControl->playerResumePlay();
				break;
			default:
				mIpodControl->playerResumePlay();
				break;
			}
		}
	}
}
void CPaneliPodWrapper::Pause()
{
	if(mIpodControl)
	{
		RETAILMSG(DEBUG_IPOD, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
		switch (mIpodControl->playerGetPlayState())
		{
		case PLAY_STATE_REW:
		case PLAY_STATE_FFW:
			mIpodControl->playerEndFFRewPlay();
			break;
		default:
			mIpodControl->playerPausePlay();
			break;
		}
	}
}

void CPaneliPodWrapper::Stop()
{
	RETAILMSG(DEBUG_IPOD, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	m_ppMain->SetArtWorkStatus(FALSE);

	m_ppMain->SetProgressValue(0);
	m_ppMain->SetElapsedTime(0);
	m_ppMain->SetRemainTime(0);

	m_ppMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
	m_ppMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
	m_ppMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
	m_ppMain->SetNowPlayingTitle(_T(""));

	m_ppMain->SetPlayState(FALSE);
}

void CPaneliPodWrapper::PlayPrevious()
{
	if(mIpodControl)
	{
		mIpodControl->playerPreviousPlay();
		m_ppMain->PlayInfoFlyOut(0);
	}
}
void CPaneliPodWrapper::PlayNext()
{
	if(mIpodControl)
	{
		mIpodControl->playerNextPlay();
		m_ppMain->PlayInfoFlyOut(1);
	}
}
void  CPaneliPodWrapper::PlayFF()
{
	if(mIpodControl)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
		mIpodControl->playerFForwordPlay();
	}
}
void  CPaneliPodWrapper::PlayRW()
{
	if(mIpodControl)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
		mIpodControl->playerFReversePlay();
	}
}

void  CPaneliPodWrapper::EndFFRPlay()
{
	if(mIpodControl)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
		mIpodControl->playerEndFFRewPlay();
	}
}
void	CPaneliPodWrapper::Repeat()
{
	if(mIpodControl)
	{
		mIpodControl->playerSwitchRepeatMode();
	}
}
void CPaneliPodWrapper::Shuffle()
{
	if(mIpodControl)
	{
		mIpodControl->playerSwitchShuffleMode();
	}
}
void CPaneliPodWrapper::SetPlayMode(UINT idMode)
{

}
void CPaneliPodWrapper::OnSpeechStart()
{
	m_bSpeechOn = TRUE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_IPOD)
	{
		m_playStateBeforeSpeech = m_playState;
		Pause();
	}
#endif
}
void CPaneliPodWrapper::OnSpeechTerminate()
{
	m_bSpeechOn = FALSE;
#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_IPOD)
	{
		if(m_playStateBeforeSpeech==PLAY_STATE_PLAYING)
		{
			Play();
		}
	}
#endif
}

void CPaneliPodWrapper::OnSRStart()
{
	//m_bSROn = TRUE;
	m_bPlayingBeforeSpeech = (m_playState==PLAY_STATE_PLAYING)?TRUE:FALSE;

	RETAILMSG(DEBUG_IPOD,(_T("MSG:[%s]: m_bPlayingBeforeSpeech:%d\r\n"),TEXT(__FUNCTION__),m_bPlayingBeforeSpeech));

#if USE_WAV_ONCHIP
	{
		if (m_bPlayingBeforeSpeech)
		{
			Pause();
		}
	}
#endif
}
void CPaneliPodWrapper::OnSRTerminate()
{
	RETAILMSG(DEBUG_IPOD,(_T("MSG:[%s]: m_bPlayingBeforeSpeech:%d\r\n"),TEXT(__FUNCTION__),m_bPlayingBeforeSpeech));

	//m_bSROn = FALSE;
#if USE_WAV_ONCHIP
	{
		if (m_bPlayingBeforeSpeech)
		{
			SwitchAudioSRC(SRC_AUDIO_ARM);
			Play();
		}
	}
#endif
}

void CPaneliPodWrapper::OnSRCChanged(UINT idSRC)
{
	if(GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_IPOD)
	{
		if(m_playState==PLAY_STATE_PLAYING)
		{
			Pause();
		}
	}
}
void CPaneliPodWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_IPOD)
	{
		if(wParam)//active
		{
			m_playStateBeforePhonecall = m_playState;
			Pause();
		}
		else
		{
			if(m_playStateBeforePhonecall==PLAY_STATE_PLAYING)
			{
				Play();
			}
		}
	}
}
void CPaneliPodWrapper::OnBrowserUp()
{
	if(mIpodControl && m_browser_level>0)
	{
		//clear list:
		m_ppLibrary->ClearList();

		m_browser_level -= 1;
		mIpodControl->browserUp();

		m_ppLibrary->SetBrowserLevel(m_browser_level);
	}
}
void CPaneliPodWrapper::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	if(m_ppMain && MEDIA_TYPE_IPOD==wParam)
		m_ppMain->SetConnectStatus(lParam);

	if(m_ppLibrary)
		m_ppLibrary->OnAvailableSourceChanged(wParam,lParam);
}
void CPaneliPodWrapper::OnSelectItem(int index)
{
	RETAILMSG(1/*DEBUG_IPOD*/,(_T("............CPaneliPodWrapper OnSelectItem: %d, browserGetHierarchyType: %d............\r\n"),index,mIpodControl->browserGetHierarchyType()));

	if (!mIpodControl->browserIsPlayable())
	{
		//clear list:
		m_ppLibrary->ClearList();

		m_browser_level += 1;
		m_ppLibrary->SetBrowserLevel(m_browser_level);

		mIpodControl->browserOpen(index);
	}
	else
	{
		if (mIpodControl->browserGetHierarchyType() == 2)
		{
			// 			setNoFullScreenWindow();
			// 			ui.buttonListVideoSwitch->setIcon(QIcon(":/buttons/Resources/icon_buttons/ic_btn_playlist.png"));
			// 			videoController_.startVideoDisplay(false);
			// 			b_inVideoMode = true;
			// 			ui.listWidget->setEnabled(false);
			// 			enableTabButtons(false);
			// 			ui.buttonListVideoSwitch->setEnabled(true);
		}
		else
		{
			// 			videoController_.stopVideoDisplay();
			// 			b_inVideoMode = false;
			// 			ui.buttonListVideoSwitch->setEnabled(false);
			// 			ui.buttonListVideoSwitch->setIcon(QIcon(":/buttons/Resources/icon_buttons/ic_btn_playlist.png"));
		}
		mIpodControl->playerStartPlay(mIpodControl->browserIsTrack()?PLAYBACK_ID_TRACK:PLAYBACK_ID_AUDIOBOOK, index);
	}
}

#if CVTE_EN_KEY_PROXY
BOOL CPaneliPodWrapper::OnKey(UINT idKey,UINT idStatus)
{
	static BOOL bLongPressFFR = FALSE;
	RETAILMSG(DEBUG_IPOD,(_T("--CPaneliPodWrapper::OnKey: key=0x%x, Status=0x%x--\r\n"),idKey,idStatus));

	switch (idKey)
	{
	case KEY_NEXT:
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			PlayNext();
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			bLongPressFFR = TRUE;
			PlayFF();
		}
		else if ( idStatus == EV_KEY_UP)
		{
			if (bLongPressFFR)
			{
				bLongPressFFR = FALSE;
				EndFFRPlay();
			}
		}
		break;
	case KEY_PREVIOUS:
		
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			PlayPrevious();
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			bLongPressFFR = TRUE;
			PlayRW();
		}
		else if ( idStatus == EV_KEY_UP)
		{
			if (bLongPressFFR)
			{
				bLongPressFFR = FALSE;
				EndFFRPlay();
			}
		}
		break;
	case KEY_RESUME:
	case KEY_PLAY:
		if ( idStatus == EV_KEY_UP )
		{
			SwitchAudioSRC(SRC_AUDIO_ARM);
			Play();
		}
		break;
	case KEY_PAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			Pause();
		}
		break;
	case KEY_PLAYPAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			if(m_playState==PLAY_STATE_PLAYING)
			{
				Pause();
			}
			else
			{
				SwitchAudioSRC(SRC_AUDIO_ARM);
				Play();
			}
		}
		break;
	case KEY_STOP:
		if ( idStatus == EV_KEY_UP )
		{
			Pause();
		}
		break;
	default:
		break;
	}
	return TRUE;
}
#else
BOOL CPaneliPodWrapper::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_IPOD,(_T("--CPaneliPodWrapper::OnKey:%x--\r\n"),idKey));


	switch (idKey)
	{
	case KEY_MCU_TRACK_NEXT:
	case KEY_MEDIA_NEXT:
		PlayNext();
		break;
	case KEY_MCU_TRACK_PREV:
	case KEY_MEDIA_PREVIOUS:
		PlayPrevious();
		break;
	case KEY_MEDIA_PLAY:
		if(GetAudioSRC() != SRC_AUDIO_ARM)
		{
			SwitchAudioSRC(SRC_AUDIO_ARM);
		}
		Play();
		break;
	case KEY_MEDIA_STOP:
		Pause();
		break;
	case KEY_MEDIA_PAUSE:
		Pause();
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
			if(m_playState==PLAY_STATE_PLAYING)
			{
				Pause();
			}
			else
			{
				Play();
			}
		}
		break;
	}

	return TRUE;
}
#endif

void CPaneliPodWrapper::OnSystemPowerStatePrepare(UINT idState)
{
	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_IDLE:
		{
			Pause();
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}
}
void CPaneliPodWrapper::OnSystemPowerStateDone(UINT idState)
{

}
