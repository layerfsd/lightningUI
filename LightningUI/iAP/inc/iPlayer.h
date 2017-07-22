/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company iAP Application

 Copyright (c) 2012  Cambridge Silicon Radio Limited, a CSR plc group
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
#pragma once

#include "../../stdafx.h"
#include "string"
#include "vector"
#include "iAPShuffleMode.h"
#include "iAPRepeatMode.h"

namespace iAP{

class PlayerListener;

typedef enum PLAY_STATE{
	PLAY_STATE_STOPPED	= 0x00,
	PLAY_STATE_PLAYING	= 0x01,
	PLAY_STATE_PAUSED		= 0x02,
	PLAY_STATE_FFW			= 0x03,
	PLAY_STATE_REW			= 0x04,
	PLAY_STATE_INVALID	= 0xFF,
}PlayState;

#pragma pack(1)
typedef struct _TrackArtworkDataHeader{
	UINT8 pixelFormatCode;
	UINT16 WidthInPixels;
	UINT16 HeightInPixels;
	UINT16 XTopLeft;
	UINT16 YTopLeft;
	UINT16 XBottomRight;
	UINT16 YBottomRight;
	UINT32 rowSizeInBytes;
}TrackArtworkDataHeader, *PTrackArtworkDataHeader;
#pragma pack()

class Artwork{
public:
	TrackArtworkDataHeader artworkDataHeader;
	std::vector<UINT8> artworkData;
};

class Artwork_N{
public:
	TrackArtworkDataHeader artworkDataHeader;
	UINT32 artworkLength;
	UINT8* artworkDatabuffer;
};

class Lyric{
public:
	UINT32 lyricLenth;
	std::vector<UINT8> lyricData;
};

class Lyric_N{
public:
	UINT32 lyricLenth;
	UINT8* lyricDatabuffer;
};

typedef enum _PlaybackID{
	PLAYBACK_ID_TRACK				= 0x00,
	PLAYBACK_ID_AUDIOBOOK	=0x01,
}PlaybackID;

class iPlayer{
public:
	iPlayer();
	virtual ~iPlayer();
	/**
	Init player, set notification request bits and get artwork format	  
	*/
	virtual void init()=0;
	/**
	Start play music, after start playback, time offset, artwork, play state, id3 will be returned automatically when changed by player listener.
	@param  id Play item type
	@param  index Item index that in the current playlist.
	*/
	virtual void startPlay(PlaybackID id, UINT32 index)=0;
	/**
	Resume playback when in pause mode	  
	*/
	virtual void resumePlay()=0;
	/**
	Pause playback when in play mode  
	*/
	virtual void pausePlay()=0;
	/**
	Stop playback 	  
	*/
	virtual void stopPlay()=0;
	/**
	Change to play next item in the playlist
	*/
	virtual void nextPlay()=0;
	/**
	Change to play previous item in the playlist
	*/
	virtual void prevPlay()=0;
	/**
	Change to play in fast forward mode
	*/
	virtual void fastForwardPlay()=0;
	/**
	Change to play in reverse mode
	*/
	virtual void reversePlay()=0;
	/**
	Change to play from ff/reverse mode to normal playback
	*/
	virtual void endFFRewPlay()=0;
	/**
	Change to play from a specific time.

	@param  secToStart time in second where play start at
	*/
	virtual void onTimePlay(UINT32 secToStart)=0;
	/**
	Get play status.
	@return play status
	*/
	virtual PlayState getPlayState() const=0;
	/**
	Set play status.
	@param  val play status that wanted to be
	*/
	virtual void setPlayState(PlayState val)=0;
	/**
	Get current time offset.
	@return play time offset in second
	*/
	virtual UINT32 getTimeOffset() const=0;
	/**
	Get Playing Track Index from DB.
	@return current playing track index
	*/
	virtual UINT32 getPlayingTrackIndex() const=0;
	/**
	Get Shuffle Mode.
	@return current shuffle mode
	*/
	virtual ShuffleMode getShuffleMode()=0;
	/**
	Set Shuffle Mode.
	@param newShuffleMode new shuffle mode that will be set
	*/
	virtual void setShuffleMode(ShuffleMode &newShuffleMode)=0;
	/**
	Get Repeat Mode.
	@return current repeat mode
	*/
	virtual RepeatMode getRepeatMode()=0;
	/**
	Set Repeat Mode.
	@param newRepeatMode new repeat mode that will be set
	*/
	virtual void setRepeatMode(RepeatMode &newRepeatMode)=0;
	/**
	Get Artwork Format ID, get specific artwork format ID, in order to get specific size of artwork
	@param formatIndex index in the artwork format table
	@return selected artwork format id which corresponds to specific formatIndex
	*/
	virtual UINT16 getArtworkFormatId(UINT formatIndex = 0) const=0;
	/**
	Get Artwork image width and height
	@param *imageWidth image width of specific formatIndex
	@param *imageHeight image height of specific formatIndex
	@param formatIndex specific index to get artwork width & height
	*/
	virtual void getArtworkImageWidthandHeight(UINT16* imageWidth, UINT16* imageHeight, UINT formatIndex = 0) const=0;
	/**
	Get Playing Track Name, track name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get name
	*/
	virtual void getPlayingTrackName(UINT32 playingTrackIndex)=0;
	/**
	Get Playing Track Artist Name, artist name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get artist name
	*/
	virtual void getPlayingTrackArtistName(UINT32 playingTrackIndex)=0;
	/**
	Get Playing Track Album Name,album name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get album name
	*/
	virtual void getPlayingTrackAlbumName(UINT32 playingTrackIndex)=0;
	/**
	Get Playing Track Artwork,	artwork data will be returned by player listener
	@param trackIndex indicate the track index that requires artwork
	@param formatID specified formatID for artwork, formatID can be get by using getArtworkFormatId function
	*/
	virtual void getArtwork(UINT32 trackIndex, UINT16 formatID)=0;
	virtual void getArtworkStatus(UINT32 trackIndex)=0;
	/**
	Get Playing Lyric
	@param trackIndex indicate the track index that requires lyric
	*/
	virtual void getLyric(UINT32 trackIndex)=0;
	/**
	Get Playing Track Index from iPod.
	@return current playing track index from iPod
	*/
	virtual void getPlayingTrackIndexFromIpod()=0;
	/**
	Get Track total time from iPod.
	@return current playing track total time from iPod
	*/
	virtual void getTotalTime()=0;
	/**
	Get Playing track number
	*/
	virtual void GetNumPlayingTracks()=0;
	/**
	Set Current playing track
	*/
	virtual void SetCurrentPlayingTrack( UINT32 trackIndex )=0;
	/**
	Register listener for notification returned by iPod
	@param listener listener to be used by passing return value.
	*/
	virtual void registerListener(PlayerListener* listener)=0;
};

class PlayerListener{
public:
	PlayerListener(){};
	virtual ~PlayerListener(){};
	/**
	Notify play state to whom register player listener
	@param playState play state
	*/
	virtual void onNotifyPlayState(UINT32 playState) = 0;

	/**
	Notify current track total playback time to whom register player listener
	@param trackLengthinSec total playback time in second
	*/
	virtual void onNotifyTrackLength(UINT32 trackLengthinSec) = 0;

	/**
	Notify current track playback time to whom register player listener
	@param trackTimeinSec playback time in second
	*/
	virtual void onNotifyTrackTime(UINT32 trackTimeinSec) = 0;
	/**
	Notify current playing track index to whom register player listener
	@param trackIndex playing track index
	*/
	virtual void onNotifyTrackIndex(UINT32 trackIndex) = 0;
	/**
	Notify playing track name to whom register player listener
	@param playingTrackName requested playing track name
	*/
	virtual void onNofityTrackName(std::string& playingTrackName) = 0;
	/**
	Notify playing artist name to whom register player listener
	@param playingTrackName requested playing artist name
	*/
	virtual void onNofityTrackArtistName(std::string& trackArtistName) = 0;
	/**
	Notify playing album name to whom register player listener
	@param playingTrackName requested playing album name
	*/
	virtual void onNofityTrackAlbumName(std::string& trackAlbumName) = 0;
	/**
	Notify track artwork data to whom register player listener
	@param artwork artwork data buffer
	*/
	virtual void onNofityTrackArtwork(Artwork* artwork) = 0;
	/**
	Notify artwork status to whom register player listener
	@param isArtworkExist artwork status
	*/
	virtual void onNotifyArtworkStatus(BOOL isArtworkExist) = 0;
	/**
	Notify lyric to whom register player listener
	@param lyric lyric buffer
	*/
	virtual void onNotifyTrackLyric(Lyric* lyric) = 0;	
	/**
	Notify shuffle state to whom register player listener
	@param shuffleMode shuffle mode
	*/
	virtual void onReturnShuffle(ShuffleMode &shuffleMode) = 0;
	/**
	Notify repeat state to whom register player listener
	@param repeatMode repeat mode
	*/
	virtual void onReturnRepeat(RepeatMode &repeatMode) = 0;
	/**
	Notify number of current playing track
	*/
	virtual void onNofityPlayingTrackNum( UINT32 trackNum ) = 0;
};

}//namespace iAP