/******************************************************************************
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_auth.h
Description:    Ѷ�������׼�-��Ȩ.
                ��д˵��:
                ISS  : iFLYTEK Speech Suite,Ѷ�������׼�;
                auth : Authorization,��Ȩ;
Author:         iFLYTEK
Version:        1.0
Date:           2013-12-04
History:        2013-12-04  ����
*******************************************************************************/
#ifndef __ISS_AUTH_H__
#define __ISS_AUTH_H__

#include "iss_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
Function:       ISSSetMachineCode
Description:    ���������.
                �ڵ���ISSGetActiveKey��ȡ������ǰ���ڵ���ISSActivate����ǰ��
                �ڵ���ISSSRCreate����ʶ����ǰ, ����Ҫ���ٵ���һ�δ˽ӿ�.
Calls:          ��
Called By:      �û�����
Return:         ISSErrID    �����׼���Ȩ����Ĵ�����.
                ISS_SUCCESS:���������ɹ�;
                ISS_ERROR_INVALID_PARA:��Ч�Ĳ���ֵ;
                ISS_ERROR_FAIL:���������ʧ��;
Others:         ÿ���ͻ��˱������벻ͬ�Ļ�����,������ĳ��Ȳ��ܳ���1024.
                ÿ��������ÿ�����ֻ�ܷ����ƶ˽���1000��ʶ��.
                ���������Ψһ, ���ڱ�ʶ���Ի���Դ.
*******************************************************************************/
ISSErrID ISSAPI ISSSetMachineCode(
    const char* szMachineCode      /* [In] ������ */
);

#ifdef ISS_SOFTWARE_ENCRYPTION

/*******************************************************************************
Function:       ISSGetActiveKey
Description:    ��ȡ�����룬ֻ��Ҫ������״̬�³ɹ�����һ��.
                �ڵ���ISSGetActiveKey��ȡ������ǰ, �����Ѿ�����ISSSetMachineCode
                ���û�����.
Calls:          ��
Called By:      �û�����
Return:         ISSErrID    �����׼���Ȩ����Ĵ�����.
                ISS_SUCCESS:��ȡ������ɹ�;
                ISS_ERROR_FAIL:��ȡ������ʧ��;
                ISS_ERROR_MACHINE_CODE_NOT_SET:��δ���û�����;
                ISS_ERROR_INVALID_MACHINE_CODE:�˻�����δ����Ȩ;
                ISS_ERROR_ACTIVATE_TO_MANY_TIMES:�λ�������Ȩ����;
                ISS_ERROR_NET_XXXX:�������;
Others:         ͬ���ӿ�, ����᷵�� ������.
*******************************************************************************/
ISSErrID ISSAPI ISSGetActiveKey(
    const char*     szResourceDir       /* [In] ��ԴĿ¼,һ�������:
                                                ��windows���Ļ�����ΪGBK����,
                                                ��linux������ΪUTF-8���� */
);

/*******************************************************************************
Function:       ISSActivate
Description:    ���뼤����Ŀ¼,����. 
                �ڵ���ISSSRCreate����ʶ����ǰ���ڵ���ISSTTSInitRes����
                �����ϳ���Դ���ǰ, ����Ҫ���ٵ���һ�δ˽ӿ�.
                �ڵ���ISSActivate����ǰ, �����Ѿ�����ISSSetMachineCode���û�����.
Calls:          ��
Called By:      �û�����
Return:         ISSErrID    �����׼���Ȩ����Ĵ�����.
                ISS_SUCCESS:��Ȩ����ɹ�;
                ISS_ERROR_INVALID_PARA:��Ч�Ĳ���ֵ;
                ISS_ERROR_MACHINE_CODE_NOT_SET:��δ���û�����;
                ISS_ERROR_INVALID_ACTIVE_KEY:��Ч�ļ�����;
Others:         
*******************************************************************************/
ISSErrID ISSAPI ISSActivate(
    const char*  szResourceDir          /* [In] ��ԴĿ¼ */
);

#endif

#ifdef __cplusplus
}
#endif

#endif  /* __ISS_AUTH_H__ */