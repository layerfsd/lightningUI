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
#include "iPlayer.h"

using namespace iAP;

class NullPlayer :
	public iPlayer
{
public:

	NullPlayer(void)
	{
	}

	virtual ~NullPlayer(void)
	{
	}
	virtual void init(){};
	/**
	Start play music, after start playback, time offset, artwork, play state, id3 will be returned automatically when changed by player listener.
	@param  id Play item type
	@param  index Item index that in the current playlist.
	*/
	virtual void startPlay(PlaybackID id, UINT32 index){};
	/**
	Resume playback when in pause mode	  
	*/
	virtual void resumePlay(){};
	/**
	Pause playback when in play mode  
	*/
	virtual void pausePlay(){};
	/**
	Stop playback 	  
	*/
	virtual void stopPlay(){};
	/**
	Change to play next item in the playlist
	*/
	virtual void nextPlay(){};
	/**
	Change to play previous item in the playlist
	*/
	virtual void prevPlay(){};
	/**
	Change to play in fast forward mode
	*/
	virtual void fastForwardPlay(){};
	/**
	Change to play in reverse mode
	*/
	virtual void reversePlay(){};
	/**
	Change to play from ff/reverse mode to normal playback
	*/
	virtual void endFFRewPlay(){};
	/**
	Change to play from a specific time.

	@param  secToStart time in second where play start at
	*/
	virtual void onTimePlay(UINT32 secToStart){};
	/**
	Get play status.
	@return play status
	*/
	virtual PlayState getPlayState() const{return PLAY_STATE_INVALID;};
	/**
	Set play status.
	@param  val play status that wanted to be
	*/
	virtual void setPlayState(PlayState val){};
	/**
	Get current time offset.
	@return play time offset in second
	*/
	virtual UINT32 getTimeOffset() const{return 0;};
	/**
	Get Playing Track Index from DB.
	@return current playing track index
	*/
	virtual UINT32 getPlayingTrackIndex() const{return 0;};
	/**
	Get Shuffle Mode.
	@return current shuffle mode
	*/
	virtual ShuffleMode getShuffleMode(){return ShuffleMode::ShuffleOff();};
	/**
	Set Shuffle Mode.
	@param newShuffleMode new shuffle mode that will be set
	*/
	virtual void setShuffleMode(ShuffleMode &newShuffleMode){};
	/**
	Get Repeat Mode.
	@return current repeat mode
	*/
	virtual RepeatMode getRepeatMode(){return RepeatMode::RepeatOff();};
	/**
	Set Repeat Mode.
	@param newRepeatMode new repeat mode that will be set
	*/
	virtual void setRepeatMode(RepeatMode &newRepeatMode){};
	/**
	Get Artwork Format ID, get specific artwork format ID, in order to get specific size of artwork
	@param formatIndex index in the artwork format table
	@return selected artwork format id which corresponds to specific formatIndex
	*/
	virtual UINT16 getArtworkFormatId(UINT formatIndex = 0) const{return 0;};
	/**
	Get Artwork image width and height
	@param *imageWidth image width of specific formatIndex
	@param *imageHeight image height of specific formatIndex
	@param formatIndex specific index to get artwork width & height
	*/
	virtual void getArtworkImageWidthandHeight(UINT16* imageWidth, UINT16* imageHeight, UINT formatIndex = 0) const{};
	/**
	Get Playing Track Name, track name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get name
	*/
	virtual void getPlayingTrackName(UINT32 playingTrackIndex){};
	/**
	Get Playing Track Artist Name, artist name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get artist name
	*/
	virtual void getPlayingTrackArtistName(UINT32 playingTrackIndex){};
	/**
	Get Playing Track Album Name,album name will be returned by player listener
	@param playingTrackIndex indicate playing track index that function wants to get album name
	*/
	virtual void getPlayingTrackAlbumName(UINT32 playingTrackIndex){};
	/**
	Get Playing Track Artwork,	artwork data will be returned by player listener
	@param trackIndex indicate the track index that requires artwork
	@param formatID specified formatID for artwork, formatID can be get by using getArtworkFormatId function
	*/
	virtual void getArtwork(UINT32 trackIndex, UINT16 formatID){};

	virtual void getArtworkStatus(UINT32 trackIndex){};

	/**
	Get Playing Lyric
	@param trackIndex indicate the track index that requires lyric
	*/
	virtual void getLyric(UINT32 trackIndex){};

	/**
	Get Playing Track Index from iPod.
	@return current playing track index from iPod
	*/
	virtual void getPlayingTrackIndexFromIpod(){};
	/**
	Get Track total time from iPod.
	@return current playing track total time from iPod
	*/
	virtual void getTotalTime(){};

	/**
	Get Playing track number
	*/
	virtual void GetNumPlayingTracks(){};
	/**
	Set Current playing track
	*/
	virtual void SetCurrentPlayingTrack( UINT32 trackIndex ){};

	/**
	Register listener for notification returned by iPod
	@param listener listener to be used by passing return value.
	*/
	virtual void registerListener(PlayerListener* listener){};

};
