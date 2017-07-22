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
#include "IapPlayerListener.h"
#include "../GUI/Common/message_map.h"

IapPlayerListener::IapPlayerListener(HWND hNotifyWnd,LPARAM lparamUser) : 
m_hNotifyWnd(hNotifyWnd),
m_lparamUser(lparamUser)
{
	LOG(LOG_FUNC,__FUNCTION__);
}

IapPlayerListener::~IapPlayerListener()
{
	LOG(LOG_FUNC,__FUNCTION__);
}

void IapPlayerListener::onNotifyPlayState(UINT32 playState)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNotifyPlayState( %d )",playState);
	//mIapPlayer->sendSignal(NotifyPlayState, playState, 0, "");
	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_PLAY_STATE,playState);

}

void IapPlayerListener::onNotifyTrackLength(UINT32 trackLengthinSec)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNotifyTrackLength( %d )",trackLengthinSec);
	//mIapPlayer->sendSignal(NotifyTrackLength, trackLengthinSec, 0, "");

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_LENGTH,trackLengthinSec);

}

void IapPlayerListener::onNotifyTrackTime(UINT32 trackTimeinSec)
{
	LOG(0/*LOG_FUNC*/,"IapPlayerListener::onNotifyTrackTime(%d)",trackTimeinSec);
	//mIapPlayer->sendSignal(NotifyTrackTime, trackTimeinSec, 0, "");

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_TIME,trackTimeinSec);

}

void IapPlayerListener::onNotifyTrackIndex(UINT32 trackIndex)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNotifyTrackIndex( %d )", trackIndex);
	//mIapPlayer->sendSignal(NotifyTrackIndex, trackIndex, 0, "");

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_INDEX,trackIndex);

}

void IapPlayerListener::onNofityTrackName(std::string &playingTrackName)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNofityTrackName()");
	//mIapPlayer->sendSignal(NofityTrackName, 0, 0, QString::fromUtf8(playingTrackName.c_str()));

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_NAME,(LPARAM)playingTrackName.c_str());

}

void IapPlayerListener::onNofityTrackArtistName(std::string &trackArtistName)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNofityTrackArtistName()");
	//mIapPlayer->sendSignal(NofityTrackArtistName, 0, 0, QString::fromUtf8(trackArtistName.c_str()));

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_ARTIST,(LPARAM)trackArtistName.c_str());

}

void IapPlayerListener::onNofityTrackAlbumName(std::string &trackAlbumName)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNofityTrackAlbumName()");
	//mIapPlayer->sendSignal(NofityTrackAlbumName, 0, 0, QString::fromUtf8(trackAlbumName.c_str()));

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_ALBUM,(LPARAM)trackAlbumName.c_str());

}

void IapPlayerListener::onNofityTrackArtwork(iAP::Artwork *artwork)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNofityTrackArtwork( %x )", artwork);
	//mIapPlayer->notifyArtwork(artwork);

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_ARTWORK,(LPARAM)artwork);

}

void IapPlayerListener::onNotifyArtworkStatus(BOOL isArtworkExist)
{
	LOG(LOG_FUNC,"IapPlayerListener::onNotifyArtworkStatus( %d )", isArtworkExist);

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_ARTWORK_STATUS,isArtworkExist);

}

void IapPlayerListener::onReturnShuffle(iAP::ShuffleMode &shuffleMode)
{
	LOG(LOG_FUNC,__FUNCTION__);
	if (ShuffleMode::ShuffleOff() == shuffleMode)
	{
		//mIapPlayer->notifyShuffleMode(shuffle_off);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_SHUFFLE,shuffle_off);
	}
	else if (ShuffleMode::ShuffleTracks() == shuffleMode)
	{
		//mIapPlayer->notifyShuffleMode(shuffle_tracks);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_SHUFFLE,shuffle_tracks);
	}
	else if (ShuffleMode::ShuffleAlbums() == shuffleMode)
	{
		//mIapPlayer->notifyShuffleMode(shuffle_albums);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_SHUFFLE,shuffle_albums);
	}


}

void IapPlayerListener::onReturnRepeat(iAP::RepeatMode &repeatMode)
{
	LOG(LOG_FUNC,__FUNCTION__);
	if (RepeatMode::RepeatOff() == repeatMode)
	{
		//mIapPlayer->notifyRepeatMode(repeat_off);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_REPEAT,repeat_off);
	}
	else if (RepeatMode::RepeatOneTrack() == repeatMode)
	{
		//mIapPlayer->notifyRepeatMode(repeat_one);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_REPEAT,repeat_one);
	}
	else if (RepeatMode::RepeatAllTracks() == repeatMode)
	{
		//mIapPlayer->notifyRepeatMode(repeat_all);
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_REPEAT,repeat_all);
	}

}

void IapPlayerListener::onNotifyTrackLyric(Lyric* lyric)
{
	RETAILMSG(1,(_T("###<%s>###\r\n"),_T(__FUNCTION__)));

}

void IapPlayerListener::onNofityPlayingTrackNum( UINT32 trackNum )
{
	RETAILMSG(1,(_T("###<%s> trackNum=%d\r\n"),_T(__FUNCTION__),trackNum));

	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_TRACK_NUM,trackNum);
}