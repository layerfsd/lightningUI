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

#ifndef IAPPLAYERLISTENER_H
#define IAPPLAYERLISTENER_H

#include "iPlayer.h"
#include "Utils.h"

enum repeat_mode
{
	repeat_off,
	repeat_one,
	repeat_all
};

enum shuffle_mode
{
	shuffle_off,
	shuffle_albums,
	shuffle_tracks
};

using namespace iAP;

class IapPlayerListener : public PlayerListener
{
public:
	IapPlayerListener(HWND hNotifyWnd,LPARAM lparamUser);
	~IapPlayerListener();
	virtual void onNotifyPlayState(UINT32 playState);
	virtual void onNotifyTrackLength(UINT32 trackLengthinSec);
	virtual void onNotifyTrackTime(UINT32 trackTimeinSec);
	virtual void onNotifyTrackIndex(UINT32 trackIndex);
	virtual void onNofityTrackName(std::string& playingTrackName);
	virtual void onNofityTrackArtistName(std::string& trackArtistName);
	virtual void onNofityTrackAlbumName(std::string& trackAlbumName);
	virtual void onNofityTrackArtwork(Artwork* artwork);
	virtual void onNotifyArtworkStatus(BOOL isArtworkExist);
	virtual void onReturnShuffle(ShuffleMode &shuffleMode);
	virtual void onReturnRepeat(RepeatMode &repeatMode);

	/**
	Notify lyric to whom register player listener
	@param lyric lyric buffer
	*/
	virtual void onNotifyTrackLyric(Lyric* lyric);	

	/**
	Notify number of current playing track
	*/
	virtual void onNofityPlayingTrackNum( UINT32 trackNum );

private:
	HWND m_hNotifyWnd;
	LPARAM m_lparamUser;

};

#endif