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

#ifndef IPODCONTROL_H
#define IPODCONTROL_H

#include "windows.h"
#include "NullPlayer.h"
#include "NullAuthentication.h"
#include "NullBrowser.h"
#include "NullOSCommunication.h"
#include "iBuilder.h"
#include "iAuthentication.h"
#include "iPlayer.h"
#include "iBrowser.h"
#include "Utils.h"

enum SUPPORT_TYPE {USB, UART};
enum SIGNAL_TYPE {AuthenticationFinished, RecordReady, OpenFinished, ExtendedUIModeReady
, NotifyPlayState, NotifyTrackLength, NotifyTrackTime, NotifyTrackIndex, NofityTrackName
, NofityTrackArtistName, NofityTrackAlbumName, NofityTrackArtwork, NotifyArtworkStatus
, ReturnShuffle, ReturnRepeat, ReadFinished};

using namespace iAP;

class IpodControl
{
public:
	static IpodControl* getIpodControl();
	IpodControl();
	~IpodControl();

	void setUp(SUPPORT_TYPE, AuthenticationListener* ,BrowserListener* ,PlayerListener*,iOSCommunicationListener *, RemoteControlListener*);
	bool constructBuilder();
	void releaseListener();
	void release();

	BOOL plugOutDetect();

	void startAuthentication();

	void browserStart();
	void browserOpen(UINT32);
	void browserUp();
	void browserRoot();
	void browserGetRecords(UINT32, UINT32);
	bool browserIsRoot();
	bool browserIsTopLevel();
	bool browserIsPlayable();
	bool browserIsTrack();
	UINT8 browserGetHierarchyType();

	void playerInit();
	void playerStartPlay(PlaybackID, UINT32);
	void playerPausePlay();
	void playerResumePlay();
	void playerStopPlay();
	void playerNextPlay();
	void playerPreviousPlay();
	void playerFReversePlay();
	void playerFForwordPlay();
	void playerEndFFRewPlay();
	void playerOnTimePlay(UINT32);
	PLAY_STATE playerGetPlayState();
	UINT32 playerGetTimeOffset();
	UINT32 playerGetPlayingTrackIndex();
	void playerGetPlayingTrackIndexFromIpod();
	ShuffleMode playerGetShuffleMode();
	void playerSetShuffleMode(ShuffleMode);
	void playerSwitchShuffleMode();
	RepeatMode playerGetRepeatMode();
	void playerSetRepeatMode(RepeatMode);
	void playerSwitchRepeatMode();
	void playerGetPlayingTrackName(UINT32);
	void playerGetPlayingTrackArtistName(UINT32);
	void playerGetPlayingTrackAlbumName(UINT32);
	void playerGetArtwork(UINT32);
	void playerGetTotalTime();
	void playerGetPlayingTrackNum();
private:
	iAuthentication * getAuthentication();
	iBrowser * getBrowser();
	iPlayer * getPlayer();

	iBuilder * mBuilder;
	iAuthentication * mAuthentication;
	iPlayer * mPlayer;
	iBrowser * mBrowser;
	iRemoteControl *mRemoteControl;
	iOSCommunicationListener * miOSCommunicationListener;
	RemoteControlListener * mRemoteControlListener;
	NullPlayer nullPlayer;
	NullBrowser nullBrowser;
	NullAuthentication nullAuthentication;	
	AuthenticationListener * mAuthenticationListener;
	PlayerListener * mPlayerListener;
	BrowserListener * mBrowserListener;
	
};

#endif