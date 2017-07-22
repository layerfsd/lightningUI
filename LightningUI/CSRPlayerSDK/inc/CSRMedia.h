/***************************************************************************
*                                                                         *
*                   SiRF Technology, Inc. Platform Software               *
*                                                                         *
*    Copyright (c) 2010 by SiRF Technology, Inc.  All rights reserved.    *
*                                                                         *
*    This Software is protected by United States copyright laws and       *
*    international treaties.  You may not reverse engineer, decompile     *
*    or disassemble this Software.                                        *
*                                                                         *
*    WARNING:                                                             *
*    This Software contains SiRF Technology, Inc.'s confidential and      *
*    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
*    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
*    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
*    Software without SiRF Technology, Inc.'s  express written            *
*    permission.   Use of any portion of the contents of this Software    *
*    is subject to and restricted by your written agreement with          *
*    SiRF Technology, Inc.                                                *
*                                                                         *
***************************************************************************/

#ifndef __CSR_MEDIA_H__
#define __CSR_MEDIA_H__

#define CM_INVALID_TIME	(-1)

typedef enum _CM_STATE
{
	CM_STATE_VOID_PENDING,
	CM_STATE_NULL,
	CM_STATE_READY,
	CM_STATE_PAUSED,
	CM_STATE_PLAYING
} CM_STATE;

typedef enum _CM_EVENT_TYPE
{
	CM_EVENT_UNKNOWN,
	CM_EVENT_COMPLETE,
	CM_EVENT_ERROR,
	CM_EVENT_WARNING,
	CM_EVENT_INFO,
	CM_EVENT_TAG,
	CM_EVENT_BUFFERING,
	CM_EVENT_STATE_CHANGED,
	CM_EVENT_STRUCTURE_CHANGE,
	CM_EVENT_DURATION,
	CM_EVENT_ASYNC_START,
	CM_EVENT_ASYNC_DONE,
	CM_EVENT_REQUEST_STATE,
	CM_EVENT_VIDEO_SIZE
} CM_EVENT_TYPE;

typedef enum _CM_VALUE_TYPE
{
	CM_VALUE_TYPE_UNKNOWN,
	CM_VALUE_TYPE_UINT32,	//	The value is a 4-byte UINT32 value.
	CM_VALUE_TYPE_UINT64,	//	The value is a 8-byte UINT64 value.
	CM_VALUE_TYPE_DOUBLE,	//	The value is a 8-byte double value.
	CM_VALUE_TYPE_STRING,	//	The value is a null-terminated Unicode string.
	CM_VALUE_TYPE_BINARY	//	The property is an array of bytes.
} CM_VALUE_TYPE;

typedef enum _CM_META_TYPE
{
	CM_META_DATA_FILE,		//	file (global) meta-data
	CM_META_DATA_AUDIO,
	CM_META_DATA_VIDEO,
	CM_META_DATA_TAG
} CM_META_TYPE;

typedef enum _CM_STREAM_TYPE
{
	CM_STREAM_AUDIO,
	CM_STREAM_VIDEO,
	CM_STREAM_TEXT
} CM_STREAM_TYPE;

typedef struct _CM_EVENT_BASE
{
	CM_EVENT_TYPE	nType;
	DWORD			dwParam;
	LPOLESTR		pszText;
} CM_EVENT_BASE;


// {A9991BC9-8888-40d3-B46E-193EC71A7B0C}
static const IID IID_ICSRPlayer = 
{ 0xa9991bc9, 0x8888, 0x40d3, { 0xb4, 0x6e, 0x19, 0x3e, 0xc7, 0x1a, 0x7b, 0xc } };
interface __declspec(uuid("A9991BC9-8888-40d3-B46E-193EC71A7B0C"))
ICSRPlayer : public IUnknown
{
	/*	Open a media file to playback
	Param:
		pszFileName[in]:	Full path filename to playback.
	*/
	virtual HRESULT __stdcall OpenFile(LPCOLESTR pszFileName) = 0;

	virtual CM_STATE __stdcall GetState() = 0;
	virtual HRESULT	__stdcall Play() = 0;
	virtual HRESULT	__stdcall Pause() = 0;

	/*	Get duration in milliseconds, return CM_INVALID_TIME if unavailable */
	virtual DWORD __stdcall GetDuration() = 0;

	/*	Get playing position in milliseconds, return CM_INVALID_TIME if unavailable */
	virtual DWORD __stdcall GetPosition() = 0;

	/*	Set playing position in milliseconds */
	virtual HRESULT	__stdcall SetPosition(DWORD dwPos) = 0;
};

// {2D10E7F9-BAE3-429f-88DA-F236B8DA14F4}
static const IID IID_ICSREventSource = 
{ 0x2d10e7f9, 0xbae3, 0x429f, { 0x88, 0xda, 0xf2, 0x36, 0xb8, 0xda, 0x14, 0xf4 } };
interface __declspec(uuid("2D10E7F9-BAE3-429f-88DA-F236B8DA14F4"))
ICSREventSource : public IUnknown
{
	/*	Set event notify window handle and message to receive player events
		Param:
			hWnd[in]:		Handle to the window, or NULL to stop receiving event messages.
			uMsg[in]:		Window message to be passed as the notification, wParam of the uMsg message will be a pointer of CM_EVENT_BASE or subclasses.
			dwInstance[in]:	Value to be passed as the lParam parameter for the uMsg message.
	*/
	virtual HRESULT __stdcall SetNotifyWindow(HWND hWnd, UINT uMsg, DWORD dwInstance) = 0;
};

// {E4FFD26E-E0A4-4f2f-8B9B-6131CD70835A}
static const IID IID_ICSRVideo = 
{ 0xe4ffd26e, 0xe0a4, 0x4f2f, { 0x8b, 0x9b, 0x61, 0x31, 0xcd, 0x70, 0x83, 0x5a } };
interface __declspec(uuid("E4FFD26E-E0A4-4f2f-8B9B-6131CD70835A"))
ICSRVideo : public IUnknown
{
	/*	Set video renderer window handlle */
	virtual HRESULT	__stdcall SetVideoWindow(HWND hWnd) = 0;

	/*	Set video overlay layer */
	virtual HRESULT	__stdcall SetOverlayLayer(LONG nIndex) = 0;

	/*	Force keep the aspect ratio, if change it in playing time, call UpdateVideo() to update the video  */
	virtual HRESULT	__stdcall KeepAspectRatio(BOOL bKeepRatio) = 0;

	/*	Return video size if available */
	virtual HRESULT __stdcall GetVideoSize(LONG* pWidth, LONG* pHeight) = 0;

	/*	Redraw video after video window position is changed */
	virtual HRESULT __stdcall UpdateVideo() = 0;
};

// {A099A86D-77E9-4db2-9473-9765A55015E3}
static const IID IID_ICSRWaveoutAudio = 
{ 0xa099a86d, 0x77e9, 0x4db2, { 0x94, 0x73, 0x97, 0x65, 0xa5, 0x50, 0x15, 0xe3 } };
interface __declspec(uuid("A099A86D-77E9-4db2-9473-9765A55015E3"))
ICSRWaveoutAudio : public IUnknown
{
	/*	Set the device ID of a waveform output device (for waveOutOpen) */
	virtual HRESULT __stdcall SetDeviceID(DWORD dwDeviceID) = 0;

	/*	Set the volume of a waveform output device (same as waveOutSetVolume) */
	virtual HRESULT	__stdcall SetVolume(DWORD dwVolume) = 0;

	/*	Get the volume of a waveform output device (same as waveOutGetVolume) */
	virtual HRESULT __stdcall GetVolume(DWORD* pdwVolume) = 0;
};

// {07955DCF-068C-411c-A6DE-576A77F3B844}
static const IID IID_ICSRInspector = 
{ 0x7955dcf, 0x68c, 0x411c, { 0xa6, 0xde, 0x57, 0x6a, 0x77, 0xf3, 0xb8, 0x44 } };
interface __declspec(uuid("07955DCF-068C-411c-A6DE-576A77F3B844"))
ICSRInspector : public IUnknown
{
	/*	Open a media file to inspect media info
		Param:
			pszFileName[in]:	Full path filename to inspect media info.
	*/
	virtual HRESULT __stdcall InspectFile(LPCOLESTR pszFileName, BOOL bExtractVideoThumbnail) = 0;

	/*	Wait for completion
		Param:
			dwTimeout[in]:	Time-out interval, in milliseconds. Pass zero to return immediately. Pass the value INFINITE to block indefinitely.
			hEventCancel[in]: An event to cancel when waiting, if this event is signaled, return E_ABORT. Pass NULL if don't want cancel.
		Return:
			S_OK:			Completed Ok.
			S_FALSE:		Time-out expired, but some meta-data may be available
			E_ABORT:		The hEventCancel is signaled.
			E_FAIL:			Match some errors.
	*/
	virtual HRESULT __stdcall WaitForCompletion(DWORD dwTimeout, HANDLE hEventCancel) = 0;
};

// {8A84D905-DF11-4388-82D2-6893DFB10436}
static const IID IID_ICSRMetaData = 
{ 0x8a84d905, 0xdf11, 0x4388, { 0x82, 0xd2, 0x68, 0x93, 0xdf, 0xb1, 0x4, 0x36 } };
interface __declspec(uuid("8A84D905-DF11-4388-82D2-6893DFB10436"))
ICSRMetaData : public IUnknown
{
	/*	Return meta-data field count */
	virtual UINT __stdcall GetFieldCount() = 0;

	/*	Enumerate meta-data fields by index
		Param:
			nIndex[in]:		Field index from 0 to count - 1.
			pszName[out]:	Pointer to a wide-character null-terminated string containing the attribute name. Pass NULL to retrieve the size of the string, which will be returned in pnName.
			pnName[in,out]: Pointer to a DWORD containing the size of pszName, in wide characters. This size includes the terminating null character.
			pType[out]:		Type of data used for the attribute. For more information about the types of data supported, see CM_VALUE_TYPE.
			pValue[out]:	Pointer to an array of bytes containing the attribute value. Pass NULL to retrieve the size of the attribute value, which will be returned in pnValue.
			pnValue[in,out]:Pointer to a DWORD containing the length, in bytes, of the attribute value pointed to by pValue.
	*/
	virtual HRESULT __stdcall GetFieldByIndex(UINT nIndex, LPOLESTR pszName, DWORD* pnName, CM_VALUE_TYPE* pType, BYTE* pValue, DWORD* pnValue) = 0;

	/*	Return meta-data field by name
		Param:
			pszName[int]:	Field name to query.
			pType[out]:		Type of data used for the attribute. For more information about the types of data supported, see CM_VALUE_TYPE.
			pValue[out]:	Pointer to an array of bytes containing the attribute value. Pass NULL to retrieve the size of the attribute value, which will be returned in pnValue.
			pnValue[in,out]:Pointer to a DWORD containing the length, in bytes, of the attribute value pointed to by pValue.
	*/
	virtual HRESULT __stdcall GetFieldByName(LPCOLESTR pszName, CM_VALUE_TYPE* pType, BYTE* pValue, DWORD* pnValue) = 0;
};

// {10185792-E872-4666-9EA0-66FF53BE2B1F}
static const IID IID_ICSRMediaInfo = 
{ 0x10185792, 0xe872, 0x4666, { 0x9e, 0xa0, 0x66, 0xff, 0x53, 0xbe, 0x2b, 0x1f } };
interface __declspec(uuid("10185792-E872-4666-9EA0-66FF53BE2B1F"))
ICSRMediaInfo : public IUnknown
{
	/*	Query meta-data by meta-type, ppMetaData[out]: Pointer to return the meta-data instance. */
	virtual HRESULT __stdcall GetMetaData(CM_META_TYPE nMetaType, ICSRMetaData** ppMetaData) = 0;
};

// {082C5C4C-237B-4dd2-BC7C-448A6A493D90}
static const IID IID_ICSRStreamInfo = 
{ 0x82c5c4c, 0x237b, 0x4dd2, { 0xbc, 0x7c, 0x44, 0x8a, 0x6a, 0x49, 0x3d, 0x90 } };
interface __declspec(uuid("082C5C4C-237B-4dd2-BC7C-448A6A493D90"))
ICSRStreamInfo : public IUnknown
{
	/*	Return stream count by type */
	virtual UINT __stdcall GetStreamCount(CM_STREAM_TYPE nStreamType) = 0;

	/*	Query stream information by stream type and stream index, ppMetaData[out]: Pointer to return the meta-data instance. */
	virtual HRESULT __stdcall GetStreamInfo(CM_STREAM_TYPE nStreamType, UINT nIndex, ICSRMetaData** ppMetaData) = 0;

	/*	Return current stream index by type */
	virtual UINT __stdcall GetCurrentStream(CM_STREAM_TYPE nStreamType) = 0;

	/*	Set current stream index by type */
	virtual HRESULT __stdcall SetCurrentStream(CM_STREAM_TYPE nStreamType, UINT nIndex) = 0;
};

// {BB822C8B-8348-4558-B219-AEF71A4B045A}
static const IID IID_ICSRMedia = 
{ 0xbb822c8b, 0x8348, 0x4558, { 0xb2, 0x19, 0xae, 0xf7, 0x1a, 0x4b, 0x4, 0x5a } };
interface __declspec(uuid("BB822C8B-8348-4558-B219-AEF71A4B045A"))
ICSRMedia : public IUnknown
{
	/*	Free a CSR player event received from player.
		Param:
			pEvent[in]:			Pointer to a CM_EVENT_BASE received from player.
	*/
	virtual HRESULT __stdcall FreeEvent(CM_EVENT_BASE* pEvent) = 0;

	/*	Create a CSR player instance.
		Param:
			ppPlayer[out]:		Pointer to return the player instance.
	*/
	virtual HRESULT __stdcall CreatePlayer(ICSRPlayer** ppPlayer) = 0;

	/*	Create an inspector instance for media info
		Param:
			ppInspector[out]:	Pointer to return the inspector instance.
	*/
	virtual HRESULT __stdcall CreateInspector(ICSRInspector** ppInspector) = 0;
};

#ifdef __cplusplus
extern "C"
{
#endif

/*	Create a CSR media instance.
	Param:
		ppMedia[out]:	Pointer to return the media instance.
*/
HRESULT __stdcall CSRCreateMedia(ICSRMedia** ppMedia);

#ifdef __cplusplus
}
#endif

#endif /* __CSR_MEDIA_H__ */
