/************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : SSA_Decode.c
  Author       : ---
  Version      : V200R001
  Date         : 2008-08-16
  Description  : ��C�ļ�������MMAģ���Զ��ʵ��
  Function List:
        ---
        ---
        ---
  History      :
  1. Date:2005-08-16
     Author: ---
     Modification:Create
  2. Date:2005-08-16
     Author: ---
     Modification:Add function ... ���ⵥ��:

  3.��    ��   : 2006��4��4��
    ��    ��   : liuyang id:48197
    �޸�����   : ���ⵥ��:A32D01738
  4.Date        : 2007-11-07
    Author      : F62575
    Modification: ���ⵥA32D13382(�ĵ�У�Է�������)
  5.Date        : 2008-12-06
    Author      : s62952
    Modification: ���ⵥAT2D07296
************************************************************************/

/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Taf_Tafm_Remote.h"
#include "TafAppSsa.h"
#include "TafClientApi.h"

/* Modified by o00132663 for At Project, 2011-10-3, Begin */

/* #include "Ssa_Define.h" */
/* #include "MnComm.h" */
/* #include "Taf_Common.h" */
#include "MnCommApi.h"

/* Modified by o00132663 for At Project, 2011-10-3, End */

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767 �޸���:�޽� 107747;������:���ٻ�65952;ԭ��:Log��ӡ*/
#define    THIS_FILE_ID        PS_FILE_ID_SSA_APP_REMOTE_C
/*lint +e767 �޸���:�޽� 107747;������:sunshaohua*/

/*****************************************************************************
 Prototype      : Taf_RegisterSSReq
 Description    : APP/AT����RegisterSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_RegisterSSReq ( MN_CLIENT_ID_T               ClientId,
                                       MN_OPERATION_ID_T           OpId,
                                       TAF_SS_REGISTERSS_REQ_STRU  *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_REGISTERSS_MSG,
                                   pPara,
                                   sizeof(TAF_SS_REGISTERSS_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}


/*****************************************************************************
 Prototype      : Taf_EraseSSReq
 Description    : APP/AT����EraseSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_EraseSSReq ( MN_CLIENT_ID_T              ClientId,
                                    MN_OPERATION_ID_T          OpId,
                                    TAF_SS_ERASESS_REQ_STRU    *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                               OpId,
                               TAF_MSG_ERASESS_MSG,
                               pPara,
                               sizeof(TAF_SS_ERASESS_REQ_STRU),
                               I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

/*****************************************************************************
 Prototype      : Taf_ActivateSSReq
 Description    : APP/AT����ActivateSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_ActivateSSReq(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_SS_ACTIVATESS_REQ_STRU         *pPara

)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_ACTIVATESS_MSG,
                                   pPara,
                                   sizeof(TAF_SS_ACTIVATESS_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

/*****************************************************************************
 Prototype      : TAF_DeativateSSReq
 Description    : APP/AT����DeactivateSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2007-11-07
    Author      : F62575
    Modification: ���ⵥA32D13382(�ĵ�У�Է�������)
  4.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_DeactivateSSReq ( MN_CLIENT_ID_T                ClientId,
                                         MN_OPERATION_ID_T             OpId,
                                         TAF_SS_DEACTIVATESS_REQ_STRU  *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_DEACTIVATESS_MSG,
                                   pPara,
                                   sizeof(TAF_SS_DEACTIVATESS_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

/*****************************************************************************
 Prototype      : Taf_InterrogateSSReq
 Description    : APP/AT����InterrogateSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_InterrogateSSReq ( MN_CLIENT_ID_T                    ClientId,
                                           MN_OPERATION_ID_T               OpId,
                                           TAF_SS_INTERROGATESS_REQ_STRU   *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_INTERROGATESS_MSG,
                                   pPara,
                                   sizeof(TAF_SS_INTERROGATESS_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

/*****************************************************************************
 Prototype      : Taf_RegisterPasswordReq
 Description    : APP/AT����RegisterPassword���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_RegisterPasswordReq ( MN_CLIENT_ID_T           ClientId,
                                                MN_OPERATION_ID_T       OpId,
                                                TAF_SS_REGPWD_REQ_STRU  *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_REGPWD_MSG,
                                   pPara,
                                   sizeof(TAF_SS_REGPWD_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

/*****************************************************************************
 Prototype      : Taf_ProcessUnstructuredSSReq
 Description    : APP/AT����ProcessUntructuredSS���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2008-12-06
    Author      : s62952
    Modification: ���ⵥAT2D07296
  4.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_ProcessUnstructuredSSReq ( MN_CLIENT_ID_T                   ClientId,
                                                        MN_OPERATION_ID_T              OpId,
                                                        TAF_SS_PROCESS_USS_REQ_STRU    *pPara)
{
    TAF_SS_PROCESS_USS_REQ_STRU    *pstSsReq;

    pstSsReq = pPara;

    if((TAF_SS_7bit_LANGUAGE_UNSPECIFIED == pstSsReq->DatacodingScheme)
        ||(TAF_SS_8bit == pstSsReq->DatacodingScheme)
        ||(TAF_SS_UCS2 == pstSsReq->DatacodingScheme)
        || (TAF_SS_7BIT_DATA_CODING == pstSsReq->DatacodingScheme))
    {
        /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
        return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_PROCESS_USS_MSG,
                                   pstSsReq,
                                   sizeof(TAF_SS_PROCESS_USS_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
        /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
    }
    else
    {
        return TAF_FAILURE;
    }
}


/* Deleted by s00217060 for �����ϱ�AT�������������C��, 2013-5-6, begin */
/* Deleted by s00217060 for �����ϱ�AT�������������C��, 2013-5-6, end */


/*****************************************************************************
 Prototype      : Taf_EraseCCEntryReq
 Description    : APP/AT����EraseCCEntry-Request���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
VOS_UINT32 TAF_EraseCCEntryReq(MN_CLIENT_ID_T            ClientId,
                                   MN_OPERATION_ID_T                OpId,
                                   TAF_SS_ERASECC_ENTRY_REQ_STRU   *pPara)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_ERASECCENTRY_MSG,
                                   pPara,
                                   sizeof(TAF_SS_ERASECC_ENTRY_REQ_STRU),
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}

#if 0
/*****************************************************************************
 Prototype      : Taf_ProcessUSSDataReq
 Description    : APP/AT����ProcessUnstructuredSS-Data���������д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
*****************************************************************************/
VOS_UINT32 TAF_ProcessUSSDataReq(VOS_UINT16                          ClientId,
                                 VOS_UINT8                                 OpId,
                                 TAF_SS_PROCESS_USSDATA_REQ_STRU       *pPara)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_PROCESS_USSDATA_MSG,
                                   pPara,
                                   sizeof(TAF_SS_PROCESS_USSDATA_REQ_STRU),
                                   WUEPS_PID_SS);
}
#endif

/*****************************************************************************
 Prototype      : TAF_SsReleaseComplete
 Description    : APP/AT����ReleaseComplete�������ͷŵ�ǰ����ռ�õ���·,���д�˱������
 Input          : ClientId - APP/AT��ʶ
                  OpId     - ������ʶ
                  pPara    - ��������
 Output         : --
 Return Value   : �������
 Calls          : ---
 Called By      : --

 History        : ---
  1.Date        : 2005-10-15
    Author      : ---
    Modification: Created function
  2.Date        : 2007-10-16
    Author      : F62575
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  3.Date        : 2013-5-17
    Author      : W000176964
    Modification: SS FDN&Call Control��Ŀ:�޸���Ϣ����PIDΪTAF
*****************************************************************************/
TAF_UINT32 TAF_SsReleaseComplete ( MN_CLIENT_ID_T      ClientId,
                                             MN_OPERATION_ID_T  OpId)
{
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, begin */
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_RLEASE_MSG,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_WUEPS_PID_TAF);
    /* Modified by w00176964 for SS FDN&Call Control, 2013-5-17, end */
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif