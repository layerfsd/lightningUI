/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2011  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international 
 treaties.  You may not reverse engineer, decompile or disassemble this 
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and 
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION, 
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING 
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY 
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without 
 Cambridge Silicon Radio Limited's express written permission.   Use of any 
 portion of the contents of this Software is subject to and restricted by your 
 signed written agreement with Cambridge Silicon Radio Limited. 
******************************************************************************/

#include "../stdafx.h"
#include "IpodControl.h"
#include "..\Config\customer.h"

IpodControl::IpodControl():
	mBuilder(NULL),mPlayer(NULL),mBrowser(NULL),mAuthentication(NULL),miOSCommunicationListener(NULL),
	mAuthenticationListener(NULL),mPlayerListener(NULL),mBrowserListener(NULL),mRemoteControl(NULL)
{
	LOG(LOG_FUNC,__FUNCTION__);
}

IpodControl::~IpodControl()
{
	LOG(LOG_FUNC,__FUNCTION__);
}

void IpodControl::setUp(SUPPORT_TYPE type, 
						AuthenticationListener* authenticationListener,
						BrowserListener* browserListener,
						PlayerListener* playerListener,
						iOSCommunicationListener * communicationListener,
						RemoteControlListener *remoteListener)
{
	LOG(LOG_FUNC,__FUNCTION__);
	mAuthenticationListener = authenticationListener;
	mPlayerListener = playerListener;
	mBrowserListener = browserListener;
	miOSCommunicationListener = communicationListener;
	mRemoteControlListener = remoteListener;

	if (NULL == mBuilder){
		mBuilder = iBuilder::create();
		if (NULL == mBuilder)
			return;
#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U705PM)
		mBuilder->configI2CDevice(_T("I2C2:"), 0x23/*0x21*/, 0x22/*0x20*/, 100000);
#else
		mBuilder->configI2CDevice(_T("I2C1:"), 0x23/*0x21*/, 0x22/*0x20*/, 100000);
#endif
		if (type == USB)
			mBuilder->configUsbDevice(_T("LNG1:"));
		else
			mBuilder->configUartDevice(_T("COM1:"),19200, 8, ONESTOPBIT,NOPARITY);
	}
}

iAuthentication * IpodControl::getAuthentication()
{
	if (NULL != mBrowser)
		return mAuthentication;
	else
		return &nullAuthentication;
}

iPlayer * IpodControl::getPlayer()
{
	if (NULL != mPlayer)
		return mPlayer;
	else
		return &nullPlayer;
}

iBrowser * IpodControl::getBrowser()
{
	if (NULL != mBrowser)
		return mBrowser;
	
	else	
		return &nullBrowser;
}

bool IpodControl::constructBuilder()
{
	if (NULL != mBuilder
		&& mBuilder->construct(mAuthenticationListener, mBrowserListener, mPlayerListener, miOSCommunicationListener,mRemoteControlListener))
	{
		mAuthentication = mBuilder->getAuthentication();
		mBrowser = mBuilder->getBrowser();
		mPlayer = mBuilder->getPlayer();
		mRemoteControl = mBuilder->getRemoteControl();
		return true;
	}else
		return false;
}

void IpodControl::releaseListener()
{
	LOG(LOG_FUNC,__FUNCTION__);
	mAuthentication = NULL;
	mBrowser = NULL;
	mPlayer = NULL;
	mRemoteControlListener = NULL;
}

void IpodControl::release()
{
	LOG(LOG_FUNC,__FUNCTION__);
	releaseListener();
	if (mBuilder != NULL)
		mBuilder->destruct();
	delete mBuilder;
}

BOOL IpodControl::plugOutDetect()
{
	return mBuilder->plugOutDetection();
}

void IpodControl::startAuthentication()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getAuthentication()->startAuthentication();
}

void IpodControl::browserStart()
{
	getBrowser()->start();
}

void IpodControl::browserOpen(UINT32 index)
{
	LOG(LOG_FUNC,"IpodControl::browserOpen( %d )",index);
	getBrowser()->open(index);
}

void IpodControl::browserUp()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getBrowser()->up();
}

void IpodControl::browserRoot()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getBrowser()->root();
}

void IpodControl::browserGetRecords(UINT32 startIndex, UINT32 count=1)
{
	LOG(LOG_FUNC,"IpodControl::browserGetRecords(startIndex = %d, count = %d )",startIndex, count);
	getBrowser()->getRecords(startIndex, count);
}

bool IpodControl::browserIsRoot()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getBrowser()->isRoot() && getBrowser()->isTopLevel();
}

bool IpodControl::browserIsTopLevel()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getBrowser()->isTopLevel();
}

bool IpodControl::browserIsPlayable()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getBrowser()->isPlayable();
}

bool IpodControl::browserIsTrack()
{
	return getBrowser()->isTrack();
}

UINT8 IpodControl::browserGetHierarchyType()
{
	return getBrowser()->getHierarchyType();
}


void IpodControl::playerInit()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->init();
}

void IpodControl::playerStartPlay(PlaybackID id, UINT32 index)
{
	LOG(LOG_FUNC,"IpodControl::playerStartPlay(PlaybackID = %d, UINT32 = %d)",id,index)
	getPlayer()->startPlay(id, index);
}

void IpodControl::playerPausePlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->pausePlay();
}

void IpodControl::playerResumePlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->resumePlay();
}

void IpodControl::playerStopPlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->stopPlay();
}

void IpodControl::playerNextPlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->nextPlay();
}

void IpodControl::playerPreviousPlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->prevPlay();
}

void IpodControl::playerFReversePlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->reversePlay();
}

void IpodControl::playerFForwordPlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->fastForwardPlay();
}

void IpodControl::playerEndFFRewPlay()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->endFFRewPlay();
}

void IpodControl::playerOnTimePlay(UINT32 secToStart)
{
	LOG(LOG_FUNC,"IpodControl::playerOnTimePlay( %d )",secToStart);
	getPlayer()->onTimePlay(secToStart);
}

PLAY_STATE IpodControl::playerGetPlayState()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getPlayer()->getPlayState();
}

UINT32 IpodControl::playerGetTimeOffset()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getPlayer()->getTimeOffset();
}

UINT32 IpodControl::playerGetPlayingTrackIndex()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getPlayer()->getPlayingTrackIndex();
}

void IpodControl::playerGetPlayingTrackIndexFromIpod()
{
	getPlayer()->getPlayingTrackIndexFromIpod();
}

ShuffleMode IpodControl::playerGetShuffleMode()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getPlayer()->getShuffleMode();
}

void IpodControl::playerSetShuffleMode(ShuffleMode mode)
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->setShuffleMode(mode);
}

void IpodControl::playerSwitchShuffleMode()
{
	LOG(LOG_FUNC,__FUNCTION__);
	ShuffleMode currentShuffleMode = getPlayer()->getShuffleMode();
	ShuffleMode newShuffleMode;

// 	if(ShuffleMode::ShuffleOff() == currentShuffleMode)
// 		newShuffleMode = ShuffleMode::ShuffleTracks();
// 	else if(ShuffleMode::ShuffleTracks() == currentShuffleMode)
// 		newShuffleMode = ShuffleMode::ShuffleAlbums();
// 	else if(ShuffleMode::ShuffleAlbums() == currentShuffleMode)
// 		newShuffleMode = ShuffleMode::ShuffleOff();

	if(ShuffleMode::ShuffleOff() == currentShuffleMode)
		newShuffleMode = ShuffleMode::ShuffleTracks();
	else if(ShuffleMode::ShuffleTracks() == currentShuffleMode)
		newShuffleMode = ShuffleMode::ShuffleOff();
	else 
		newShuffleMode = ShuffleMode::ShuffleTracks();


	getPlayer()->setShuffleMode(newShuffleMode);
}

RepeatMode IpodControl::playerGetRepeatMode()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return getPlayer()->getRepeatMode();
}

void IpodControl::playerSetRepeatMode(RepeatMode mode)
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->setRepeatMode(mode);
}

void IpodControl::playerSwitchRepeatMode()
{
	LOG(LOG_FUNC,__FUNCTION__);
	RepeatMode currentRepeatMode = getPlayer()->getRepeatMode();
	RepeatMode newRepeatMode;
// 	if(RepeatMode::RepeatOff() == currentRepeatMode)
// 		newRepeatMode = RepeatMode::RepeatOneTrack();
// 	else if(RepeatMode::RepeatOneTrack() == currentRepeatMode)
// 		newRepeatMode = RepeatMode::RepeatAllTracks();
// 	else if(RepeatMode::RepeatAllTracks() == currentRepeatMode)
// 		newRepeatMode = RepeatMode::RepeatOff();

	if(RepeatMode::RepeatOneTrack() == currentRepeatMode)
		newRepeatMode = RepeatMode::RepeatAllTracks();
	else if(RepeatMode::RepeatAllTracks() == currentRepeatMode)
		newRepeatMode = RepeatMode::RepeatOneTrack();
	else
		newRepeatMode = RepeatMode::RepeatOneTrack();

	getPlayer()->setRepeatMode(newRepeatMode);
}

void IpodControl::playerGetPlayingTrackName(UINT32 playingTrackIndex)
{
	LOG(LOG_FUNC,"IpodControl::playerGetPlayingTrackName( %d )", playingTrackIndex);
	getPlayer()->getPlayingTrackName(playingTrackIndex);
}

void IpodControl::playerGetPlayingTrackArtistName(UINT32 playingTrackIndex)
{
	LOG(LOG_FUNC,"IpodControl::playerGetPlayingTrackArtistName( %d )",playingTrackIndex);
	getPlayer()->getPlayingTrackArtistName(playingTrackIndex);
}

void IpodControl::playerGetPlayingTrackAlbumName(UINT32 playingTrackIndex)
{
	LOG(LOG_FUNC,"IpodControl::playerGetPlayingTrackAlbumName( %d )",playingTrackIndex);
	getPlayer()->getPlayingTrackAlbumName(playingTrackIndex);
}

void IpodControl::playerGetArtwork(UINT32 trackIndex)
{
	UINT16 FormatID;
	LOG(LOG_FUNC,"IpodControl::playerGetArtwork( %d )",trackIndex);
    FormatID = getPlayer()->getArtworkFormatId();
	if (FormatID == 0xffff)
	{
        LOG(LOG_FUNC,"IpodControl::ArtworkFormatID not ready!");
		return;
	}
	getPlayer()->getArtwork(trackIndex, getPlayer()->getArtworkFormatId());
}

void IpodControl::playerGetTotalTime()
{
	LOG(LOG_FUNC,__FUNCTION__);
	getPlayer()->getTotalTime();
}
void IpodControl::playerGetPlayingTrackNum()
{
	getPlayer()->GetNumPlayingTracks();
}
IpodControl* IpodControl::getIpodControl()
{
	LOG(LOG_FUNC,__FUNCTION__);
	return new IpodControl();
}