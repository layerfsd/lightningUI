
#ifndef INSERT_DATA
#define INSERT_DATA(name, dataClass, dataType, cntArray)
#endif

INSERT_DATA(BTState, DWORD, DT_VALUE, 1)	//0
INSERT_DATA(HFState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)
INSERT_DATA(CallState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)
INSERT_DATA(DUNState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)
INSERT_DATA(CallNumber, TCHAR, DT_ARRAY, MAX_PHONE_NUMBER)	//dialling phone number, write by FSM, read by UI;when incoming or dail by phone(can't get number), write by FSM, read by UI
INSERT_DATA(PairedDeviceList, CBTDevList, DT_LIST, 1)	//5
INSERT_DATA(SearchDeviceList, CBTDevList, DT_LIST, 1)
INSERT_DATA(CallRecordsList, CCallRecList, DT_LIST, 1)
INSERT_DATA(DUNNetworkList, CDUNNetworkList, DT_LIST, 1)
INSERT_DATA(HFSpeakerVolume, DWORD, DT_VALUE, 1)
INSERT_DATA(HFAudioDirection, DWORD, DT_VALUE, 1)	//10
INSERT_DATA(CallInfo1, CallInfoT, DT_VALUE, 1)		//For 3way talking
INSERT_DATA(CallInfo2, CallInfoT, DT_VALUE, 1)		//For 3way talking
INSERT_DATA(TotalNumberOfCalls, DWORD, DT_VALUE, 1)		//For 3way talking
INSERT_DATA(HeldState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)
INSERT_DATA(WaitingNumber, TCHAR, DT_ARRAY, MAX_PHONE_NUMBER)	//15
INSERT_DATA(TPMSState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)//20	//TPMS 
INSERT_DATA(PBAPState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)	//PBAP
INSERT_DATA(ContactsList, CContactList, DT_LIST, 1)				//PBAP
INSERT_DATA(MAPState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)	//MAP
INSERT_DATA(MessagesList, CMessageList, DT_LIST, 1)			//MAP
INSERT_DATA(A2DPState,TCHAR,DT_ARRAY,STATE_STRING_MAXLEN)	//25
INSERT_DATA(AVRCPState,TCHAR,DT_ARRAY,STATE_STRING_MAXLEN)	
INSERT_DATA(AVRCPPlayState,TCHAR,DT_VALUE,1)
INSERT_DATA(AVRCPMediaInfo,AvcmMetaDataInfoT,DT_VALUE,1)
INSERT_DATA(AVRCPSongPlayTime,DWORD,DT_ARRAY,2)	//AVRCP	,the current playing song's length;the playing position in the song 
INSERT_DATA(AVRCPMusicPlayerShuffle,AvcmPlayAppSettingAttValIdT,DT_VALUE,1)//30 //play application settings
INSERT_DATA(AVRCPMusicPlayerRepeat,AvcmPlayAppSettingAttValIdT,DT_VALUE,1)//play application settings
INSERT_DATA(AVRCPVersion,double,DT_VALUE,1)//avrcp version
INSERT_DATA(AVRCPMediaPlayerList,CAvrcpMediaContentList<AvrcpMediaPlayerContentData>,DT_LIST,1)//AvcmMediaPlayerContent media player list(for avrcp 1.4).
INSERT_DATA(AVRCPFolderItemList,CAvrcpMediaContentList<AvrcpFolderItemContentData>,DT_LIST,1)
INSERT_DATA(AVRCPMediaItemList,CAvrcpMediaContentList<AvrcpMediaItemContentData>,DT_LIST,1)//35
INSERT_DATA(SPPState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)	//MAP
INSERT_DATA(HIDState, TCHAR, DT_ARRAY, STATE_STRING_MAXLEN)	//HID
INSERT_DATA(HIDDiscoverable, BOOL, DT_VALUE, 1)// HID service discoverable state
#undef INSERT_DATA