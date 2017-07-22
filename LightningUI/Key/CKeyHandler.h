#ifndef	__CKEYHANDLER_h__
#define	__CKEYHANDLER_h__

#include <windows.h>
#include <CMthread.h>
#include <CSync.h>
#include <map>
#include <../gui/common/key.h>

using namespace std;

typedef void (*KeyHandlerCallback_f)(	unsigned int key,
										unsigned int event,
										void* context );

typedef struct _KeyHandlerParam
{
	unsigned int 			Key;
	unsigned int			v_Key;
	unsigned int 			Flag;
	unsigned int 			EventMask;
	unsigned int 			PressHoldIntervalTime;
	unsigned int 			LongPressIntervalTime;
	unsigned int 			DoublePressIntervalTime;
	KeyHandlerCallback_f	FuncCallback;
	void * 					FuncContext;
}KeyHandlerParam;

class CKeyHandler;
class CKeysHandler;

typedef map  < unsigned int, CKeyHandler > CKeyHandler_Map;
typedef pair < unsigned int, CKeyHandler > CKeyHandler_Pair;

class CKeyHandler
{
public:
	CKeyHandler(KeyHandlerParam& param, HWND hWnd, UINT Msg);
	~CKeyHandler();

	void KeyDown();
	void KeyUp(BOOL bCancle=FALSE);
	
private:
	void PostMsg( unsigned int event );

	static void CALLBACK PressHoldTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK LongPressTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK DoublePressTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	void PressHoldTimer();
	void LongPressTimer();
	void DoublePressTimer();
	
	KeyHandlerParam m_param;
	HWND m_hWnd;
	UINT m_Msg;

	MMRESULT m_PressHoldTimer;
	MMRESULT m_LongPressTimer;
	MMRESULT m_DoublePressTimer;

	BOOL m_Down;
	BOOL m_SendDoubleEvent;
};

class CKeysHandler
{
public:
	static CKeysHandler* GetInstance();
	void Initialize(HWND hWnd, UINT Msg);
	void AddKeys(KeyHandlerParam* inParam, size_t count);
	void AddorReplaceKey(KeyHandlerParam &param);
	void RemoveKey(unsigned int key);

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key, BOOL bCancle=FALSE);
	
private:
	CKeysHandler();
    CKeysHandler(const CKeysHandler &);
    CKeysHandler& operator = (const CKeysHandler &);
    ~CKeysHandler();
	static const CKeysHandler* m_pInstance;

	CKeyHandler_Map m_KeyMap;
	HWND m_hWnd;
	UINT m_Msg;
};

#endif
