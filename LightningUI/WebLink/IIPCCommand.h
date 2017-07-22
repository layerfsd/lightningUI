#ifndef IIPCCommand_H
#define IIPCCommand_H


//--------------------------------------------------------------------------------------------
//  Includes
//--------------------------------------------------------------------------------------------

#define kMsgQName_WeblinkClientMsgQ				_T("WeblinkClientMsgQ")
#define kMsgQName_WeblinkAppMsgQ				_T("WeblinkAppMsgQ")

//---------------------------------------------------------------------------------------------------------------------------
/*!	group	Msg Key fields
*/

#define kMsgKey_cmd				    "cmd"
#define kMsgKey_param				"param"

#define kMsgCMD_ButtonKey			"ButtonKey"
#define kMsgCMD_WindowControl		"WindowControl"
#define kMsgCMD_SetCurrentApp		"SetCurrentApp"
#define kMsgCMD_Init				"Init"
#define kMsgCMD_Connect				"Connect"
#define kMsgCMD_WindowStatus		"WindowStatus"

#define kMsgParam_init				"init"
#define kMsgParam_connect			"connect"
#define kMsgParam_show				"show"
#define kMsgParam_button			"button"
#define kMsgParam_appID				"appID"
#define kMsgParam_appParams			"appParams"

#define kMsgButton_back				"back"
#define kMsgButton_home				"home"

//--------------------------------------------------------------------------------------------  
//  Type defintions
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Inline functions
//--------------------------------------------------------------------------------------------

#endif