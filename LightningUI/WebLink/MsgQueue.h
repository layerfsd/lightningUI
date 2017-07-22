#ifndef	__MsgQueue_h__
#define	__MsgQueue_h__

#if defined(__cplusplus)
extern "C"
{
#endif

// ��Ϣ���з���ģʽ
enum ACCESSMODE
{
	ReadMode = 0,
	WriteMode
};

#define MAX_MESSAGE_SIZE    0x4000
#define MAX_IAP_MESSAGE_SIZE    0x4000

// ������Ϣ�ص�����
typedef BOOL (CALLBACK *MsgQueueCallBack)(PVOID Param, PVOID pData, DWORD dwSize);

typedef struct __MsgQueue MsgQueue,*MsgQueueRef;

typedef BOOL
	( *MsgQueueRead_f)(
	    MsgQueueRef queue,
		LPVOID lpBuffer,
		DWORD dwSize,
		LPDWORD lpNumberOfBytesRead,
		DWORD dwTimeout);

typedef BOOL
	( *MsgQueueWrite_f)(
	    MsgQueueRef queue,
		LPVOID lpBuffer,
		DWORD dwSize);

typedef BOOL
	( *MsgQueueSetCallBack_f)(
	    MsgQueueRef queue,
		MsgQueueCallBack pCallBackFun,
		PVOID pParam);

struct __MsgQueue
{
	void* context;
    MsgQueueRead_f Read_f;
	MsgQueueWrite_f	Write_f;
    MsgQueueSetCallBack_f SetCallBack_f;
};

extern MsgQueueRef MsgQueueInit(LPCWSTR lpQueueName, DWORD dwSize, enum ACCESSMODE accessMode);
extern void MsgQueueDeinit(MsgQueueRef queue);

#if defined(__cplusplus)
}
#endif


#endif
