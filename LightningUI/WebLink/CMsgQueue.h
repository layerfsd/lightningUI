/*******************************************************************
 filename: CMsgQueue.h
 purpose:	封装了WINCE下消息队列操作类
 author:	firehood
 created:	2011.03.23
********************************************************************/
#ifndef _MSG_QUEUE_H
#define _MSG_QUEUE_H

#include "MsgQueue.h"

class CMsgQueue
{
public:
	CMsgQueue();
	CMsgQueue(LPCWSTR lpQueueName,DWORD dwSize,ACCESSMODE accessMode);
	~CMsgQueue();
public:
	/**********************************************************************
	函数名：Create
	功能：  创建或打开消息队列	
	参数：
	        [in]lpQueueName:	消息队列名称
	        [in]dwSize:		    每条消息的最大长度
			[in]accessMode      消息队列访问模式 ReadMode：只读  WriteMode：只写
	返回值：
	        成功：TRUE  失败：FALSE
	**********************************************************************/
	BOOL Create(LPCWSTR lpQueueName,DWORD dwSize,ACCESSMODE accessMode);

	/**********************************************************************
	函数名：Read
	功能：  从消息队列中读取一条消息
	参数：
	        [out]lpBuffer:	          存放读取的数据
	        [in] dwSize:		      读取数据的大小
			[out]lpNumberOfBytesRead  实际读取数据的大小
			[in] dwTimeout            读取超时时间(ms) 0 立即返回  INFINITE 永远等待直至消息队列中有数据
	返回值：
	        成功：TRUE  失败：FALSE
	***********************************************************************/
	BOOL Read(LPVOID lpBuffer,DWORD dwSize,LPDWORD lpNumberOfBytesRead,DWORD dwTimeout = 0);

	/**********************************************************************
	函数名：Write
	功能：  向消息队列中写入一条消息	
	参数：
	        [in]lpBuffer:	   待写入的数据
	        [in]dwSize:		   写入数据的大小
	返回值：
	        成功：TRUE  失败：FALSE
	**********************************************************************/
	BOOL Write(LPVOID lpBuffer,DWORD dwSize);

	// 设置消息回调函数
	BOOL SetMsgCallBack(MsgQueueCallBack pCallBackFun, PVOID pParam);
	// 关闭消息队列
	void Close(void);
private:
    // 开启读取消息线程
	BOOL StartRevMsgThread(void);
	// 停止读取消息线程
	BOOL StopRevMsgThread(void);
	// 读取消息线程
	static void WINAPI RevMsgThread(LPVOID pParam);
private:
	HANDLE m_hMsgQueue;
	DWORD m_dwQueueSize;
	MsgQueueCallBack m_MsgCallBack;
    PVOID m_MsgParam;
	HANDLE m_hRevMsgThread;
	BOOL m_bThreadExit;
};
#endif
