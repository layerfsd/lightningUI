/*******************************************************************
 filename: CMsgQueue.h
 purpose:	��װ��WINCE����Ϣ���в�����
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
	��������Create
	���ܣ�  ���������Ϣ����	
	������
	        [in]lpQueueName:	��Ϣ��������
	        [in]dwSize:		    ÿ����Ϣ����󳤶�
			[in]accessMode      ��Ϣ���з���ģʽ ReadMode��ֻ��  WriteMode��ֻд
	����ֵ��
	        �ɹ���TRUE  ʧ�ܣ�FALSE
	**********************************************************************/
	BOOL Create(LPCWSTR lpQueueName,DWORD dwSize,ACCESSMODE accessMode);

	/**********************************************************************
	��������Read
	���ܣ�  ����Ϣ�����ж�ȡһ����Ϣ
	������
	        [out]lpBuffer:	          ��Ŷ�ȡ������
	        [in] dwSize:		      ��ȡ���ݵĴ�С
			[out]lpNumberOfBytesRead  ʵ�ʶ�ȡ���ݵĴ�С
			[in] dwTimeout            ��ȡ��ʱʱ��(ms) 0 ��������  INFINITE ��Զ�ȴ�ֱ����Ϣ������������
	����ֵ��
	        �ɹ���TRUE  ʧ�ܣ�FALSE
	***********************************************************************/
	BOOL Read(LPVOID lpBuffer,DWORD dwSize,LPDWORD lpNumberOfBytesRead,DWORD dwTimeout = 0);

	/**********************************************************************
	��������Write
	���ܣ�  ����Ϣ������д��һ����Ϣ	
	������
	        [in]lpBuffer:	   ��д�������
	        [in]dwSize:		   д�����ݵĴ�С
	����ֵ��
	        �ɹ���TRUE  ʧ�ܣ�FALSE
	**********************************************************************/
	BOOL Write(LPVOID lpBuffer,DWORD dwSize);

	// ������Ϣ�ص�����
	BOOL SetMsgCallBack(MsgQueueCallBack pCallBackFun, PVOID pParam);
	// �ر���Ϣ����
	void Close(void);
private:
    // ������ȡ��Ϣ�߳�
	BOOL StartRevMsgThread(void);
	// ֹͣ��ȡ��Ϣ�߳�
	BOOL StopRevMsgThread(void);
	// ��ȡ��Ϣ�߳�
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
