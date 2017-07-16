/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : RnicProcMsg.c
  �� �� ��   : ����
  ��    ��   : f00179208
  ��������   : 2011��12��06��
  ����޸�   :
  ��������   : RNIC����Ϣ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��12��06��
    ��    ��   : f00179208
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RnicCtx.h"
#include "RnicProcMsg.h"
#include "AtRnicInterface.h"
#include "v_typdef.h"
#include "RnicTimerMgmt.h"
#include "mdrv.h"
#include "RnicEntity.h"
#include "AdsDeviceInterface.h"
#include "RnicLog.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "RnicDebug.h"

/* Added by f00179208 for CCPU RESET, 2013-04-15, Begin */
#include "AcpuReset.h"
/* Added by f00179208 for CCPU RESET, 2013-04-15, End */

/* Added by f00179208 for VTLTE, 2014-07-31, Begin */
#include "ImsaRnicInterface.h"
/* Added by f00179208 for VTLTE, 2014-07-31, End */

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
#include "SdioInterface.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RNIC_PROCMSG_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
/* RNICģ�鶨ʱ����ʱ��Ϣ����������Ӧ�� */
const RNIC_RCV_TI_EXPRIED_PROC_STRU g_astRnicTiExpriedProcTab[]=
{
    /* ��ϢID */                            /* ��ʱ����ʱ�������� */
    {TI_RNIC_DSFLOW_STATS_0,                RNIC_RcvTiDsflowStatsExpired},
    {TI_RNIC_DSFLOW_STATS_1,                RNIC_RcvTiDsflowStatsExpired},
    {TI_RNIC_DSFLOW_STATS_2,                RNIC_RcvTiDsflowStatsExpired},
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    {TI_RNIC_DSFLOW_STATS_3,                RNIC_RcvTiDsflowStatsExpired},
    {TI_RNIC_DSFLOW_STATS_4,                RNIC_RcvTiDsflowStatsExpired},
#endif
    {TI_RNIC_DEMAND_DIAL_DISCONNECT,        RNIC_RcvTiDemandDialDisconnectExpired},
    {TI_RNIC_DEMAND_DIAL_PROTECT,           RNIC_RcvTiDemandDialProtectExpired}
 };
/* Added by f00179208 for DSDA Phase I, 2012-11-22, End */


/* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
extern VOS_UINT32 OM_AcpuTraceMsgHook(VOS_VOID* pMsg);
/* Added by A00165503 for DTS2012060502819, 2012-06-06, End */

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_MNTN_TraceDialConnEvt
 ��������  : ��ά�ɲ⹳��: ������������¼�
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��6��6��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��6��18��
    ��    ��   : A00165503
    �޸�����   : DTS2012061800997: �޸İ��貦�ſ�ά�ɲ⹳����ʽ
  3.��    ��   : 2012��12��11��
    ��    ��   : l00167671
    �޸�����   : DTS2012121802573, TQE����
*****************************************************************************/
VOS_VOID RNIC_MNTN_TraceDialConnEvt(VOS_VOID)
{
    RNIC_MNTN_DIAL_CONN_EVT_STRU       *pstDialEvt = VOS_NULL_PTR;

    /* ������Ϣ */
    pstDialEvt = (RNIC_MNTN_DIAL_CONN_EVT_STRU*)PS_ALLOC_MSG_WITH_HEADER_LEN(
                        ACPU_PID_RNIC,
                        sizeof(RNIC_MNTN_DIAL_CONN_EVT_STRU));
    if (VOS_NULL_PTR == pstDialEvt)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_MNTN_TraceDialEvent: Memory alloc failed.");
        return;
    }

    /* ��д��Ϣ */
    pstDialEvt->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstDialEvt->ulReceiverPid   = ACPU_PID_RNIC;
    pstDialEvt->enMsgId         = ID_RNIC_MNTN_EVT_DIAL_CONNECT;

    /* Modified by A00165503 for DTS2012061800997, 2012-06-18, Begin */
    /* ������ά�ɲ���Ϣ */
    /* Modified by l00167671 for DTS2012121802573, 2012-12-11, begin */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDialEvt))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_MNTN_TraceDialConnEvt():WARNING:SEND MSG FIAL");
    }
     /* Modified by l00167671 for DTS2012121802573, 2012-12-11, end */
    /* Modified by A00165503 for DTS2012061800997, 2012-06-18, End */

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_MNTN_TraceDialDisconnEvt
 ��������  : ��ά�ɲ⹳��: ������ŶϿ��¼�
 �������  : ulPktNum      - ���ŶϿ�ʱ�յ������ݰ�����
             ulUsrExistFlg - �û�����״̬(USB����WIFI)
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��6��6��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��6��18��
    ��    ��   : A00165503
    �޸�����   : DTS2012061800997: �޸İ��貦�ſ�ά�ɲ⹳����ʽ
  3.��    ��   : 2012��12��11��
    ��    ��   : l00167671
    �޸�����   : DTS2012121802573, TQE����
*****************************************************************************/
VOS_VOID RNIC_MNTN_TraceDialDisconnEvt(
    VOS_UINT32                          ulPktNum,
    VOS_UINT32                          ulUsrExistFlg
)
{
    RNIC_MNTN_DIAL_DISCONN_EVT_STRU    *pstDialEvt = VOS_NULL_PTR;

    /* ������Ϣ */
    pstDialEvt = (RNIC_MNTN_DIAL_DISCONN_EVT_STRU*)PS_ALLOC_MSG_WITH_HEADER_LEN(
                        ACPU_PID_RNIC,
                        sizeof(RNIC_MNTN_DIAL_DISCONN_EVT_STRU));
    if (VOS_NULL_PTR == pstDialEvt)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_MNTN_TraceDialEvent: Memory alloc failed.");
        return;
    }

    /* ��д��Ϣͷ */
    pstDialEvt->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstDialEvt->ulReceiverPid   = ACPU_PID_RNIC;
    pstDialEvt->enMsgId         = ID_RNIC_MNTN_EVT_DIAL_DISCONNECT;

    /* ��д��Ϣ���� */
    pstDialEvt->ulPktNum        = ulPktNum;
    pstDialEvt->ulUsrExistFlg   = ulUsrExistFlg;

    /* Modified by A00165503 for DTS2012061800997, 2012-06-18, Begin */
    /* ������ά�ɲ���Ϣ */
    /* Modified by l00167671 for DTS2012121802573, 2012-12-11, begin */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDialEvt))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_MNTN_TraceDialDisconnEvt():WARNING:SEND MSG FIAL");
    }
    /* Modified by l00167671 for DTS2012121802573, 2012-12-11, end */
    /* Modified by A00165503 for DTS2012061800997, 2012-06-18, End */

    return;
}
/* Added by A00165503 for DTS2012060502819, 2012-06-06, End */

/*****************************************************************************
 �� �� ��  : RNIC_SendDialInfoMsg
 ��������  : RNIC���͸��Լ��Ĳ���ģʽ��Ϣ���ڿ�ά�ɲ�
 �������  : RNIC_DEMAND_DIAL_INFO_MSG_ID_ENUM_UINT32                enMsgId
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��2��1��
    ��    ��   : w00199382
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RNIC_SendDialInfoMsg(
    RNIC_MSG_ID_ENUM_UINT32            enMsgId
)
{
    RNIC_NOTIFY_MSG_STRU               *pstDialInfo;
    RNIC_DIAL_MODE_STRU                *pstDialMode;


    /* �ڴ���� */
    pstDialInfo = (RNIC_NOTIFY_MSG_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                                                      sizeof(RNIC_NOTIFY_MSG_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstDialInfo)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDialInfoMsg: Malloc failed!");
        return VOS_ERR;
    }

    pstDialMode                         = RNIC_GetDialModeAddr();

    /* �����Ϣ */
    pstDialInfo->ulSenderCpuId          = VOS_LOCAL_CPUID;
    pstDialInfo->ulSenderPid            = ACPU_PID_RNIC;
    pstDialInfo->ulReceiverCpuId        = VOS_LOCAL_CPUID;
    pstDialInfo->ulReceiverPid          = ACPU_PID_RNIC;
    pstDialInfo->enMsgId                = enMsgId;

    PS_MEM_CPY(&(pstDialInfo->stDialInfo), pstDialMode, sizeof(RNIC_DIAL_MODE_STRU));

    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDialInfo))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDialInfoMsg: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_SendDialEvent
 ��������  : RNICͨ��netlink֪ͨAPP���Ż�Ͽ�
 �������  : ulDeviceId :�豸��
             ulEventId  :�¼�ID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��13��
   ��    ��   : S62952
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_SendDialEvent(
    VOS_UINT32                          ulDeviceId,
    VOS_UINT32                          ulEventId
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulSize;
    DEVICE_EVENT                        stEvent;

    ulSize                              = sizeof(DEVICE_EVENT);
    stEvent.device_id                   = (DEVICE_ID)ulDeviceId;
    stEvent.event_code                  = (VOS_INT)ulEventId;
    stEvent.len                         = 0;

    /* �ϱ����貦���¼�*/
    ulRet = (VOS_UINT32)device_event_report(&stEvent, (VOS_INT)ulSize);

    if (VOS_OK != ulRet)
    {
        RNIC_WARNING_LOG(ACPU_PID_RNIC, "RNIC_SendDialEvent: can't add event");
        return VOS_ERR;
    }

    RNIC_INFO_LOG2(ACPU_PID_RNIC, "RNIC_SendDialEvent Done ulEventId  ulDeviceId",ulEventId,ulDeviceId);

    return VOS_OK;

}

/* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_GetDsflowTimerIdByNetId
 ��������  : ͨ������ID�õ�����ͳ�ƶ�ʱ��ID
 �������  : ucRmNetId :����ID
 �������  : ��
 �� �� ֵ  : RNIC_TIMER_ID_ENUM_UINT16 ��ʱ��ID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��11��28��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
RNIC_TIMER_ID_ENUM_UINT16 RNIC_GetDsflowTimerIdByNetId(VOS_UINT8 ucRmNetId)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;

    switch (ucRmNetId)
    {
        case RNIC_RM_NET_ID_0 :
            enTimerId = TI_RNIC_DSFLOW_STATS_0;
            break;

        case RNIC_RM_NET_ID_1 :
            enTimerId = TI_RNIC_DSFLOW_STATS_1;
            break;

        case RNIC_RM_NET_ID_2 :
            enTimerId = TI_RNIC_DSFLOW_STATS_2;
            break;

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
        case RNIC_RM_NET_ID_3 :
            enTimerId = TI_RNIC_DSFLOW_STATS_3;
            break;

        case RNIC_RM_NET_ID_4 :
            enTimerId = TI_RNIC_DSFLOW_STATS_4;
            break;
#endif

        default :
            enTimerId = TI_RNIC_TIMER_BUTT;
            break;
    }

    return enTimerId;
}

/*****************************************************************************
 �� �� ��  : RNIC_GetNetIdByTimerId
 ��������  : ���ݶ�ʱ��ID��ȡ����ID
 �������  : ucRmNetId :����ID
 �������  : ��
 �� �� ֵ  : VOS_UINT8 ����ID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��11��28��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT8 RNIC_GetNetIdByTimerId(VOS_UINT32 ulMsgId)
{
    VOS_UINT8                           ucRmNedId;

    switch (ulMsgId)
    {
        case TI_RNIC_DSFLOW_STATS_0 :
            ucRmNedId = RNIC_RM_NET_ID_0;
            break;

        case TI_RNIC_DSFLOW_STATS_1 :
            ucRmNedId = RNIC_RM_NET_ID_1;
            break;

        case TI_RNIC_DSFLOW_STATS_2 :
            ucRmNedId = RNIC_RM_NET_ID_2;
            break;

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
        case TI_RNIC_DSFLOW_STATS_3 :
            ucRmNedId = RNIC_RM_NET_ID_3;
            break;

        case TI_RNIC_DSFLOW_STATS_4 :
            ucRmNedId = RNIC_RM_NET_ID_4;
            break;
#endif

        case TI_RNIC_DEMAND_DIAL_DISCONNECT :
        case TI_RNIC_DEMAND_DIAL_PROTECT :
            ucRmNedId = RNIC_RM_NET_ID_0;
            break;

        default :
            ucRmNedId = RNIC_RM_NET_ID_BUTT;
            break;
    }

    return ucRmNedId;
}

/*****************************************************************************
 �� �� ��  : RNIC_BuildRabIdByModemId
 ��������  : ����ModemId����Rabid
 �������  : enModemId : Modem ID
             ucRabId   : RabId
 �������  : pucRabId  : ���ɵ�RabId
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_BuildRabIdByModemId(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucRabId,
    VOS_UINT8                          *pucRabId
)
{
    if (MODEM_ID_0 == enModemId)
    {
        /* Modem0��RABID�ĸ���λ��00��ʾ */
        *pucRabId = ucRabId;
    }
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    else if (MODEM_ID_1 == enModemId)
    {
        /* Modem1��RABID�ĸ���λ��01��ʾ */
        *pucRabId = ucRabId | RNIC_RABID_TAKE_MODEM_1_MASK;
    }
#endif
    else
    {
        /* �Ȳ���Modem0Ҳ����Modem1�ģ�����ʧ�� */
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_SaveNetIdByRabId
 ��������  : ����rabid�洢����ID
 �������  : enModemId : Modem ID
             ucRmNetId : ����ID
             ucRabId   : RabId
 �������  :
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��12��10��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_SaveNetIdByRabId(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucRabId,
    VOS_UINT8                           ucRmNetId
)
{
    RNIC_RABID_INFO_STAU               *pstRabIdInfo;

    if ((ucRabId < RNIC_RAB_ID_MIN)
     || (ucRabId > RNIC_RAB_ID_MAX))
    {
        return VOS_ERR;
    }

    /* ��ȡָ��Modem��RABID��Ϣ */
    pstRabIdInfo = RNIC_GET_SPEC_MODEM_RABID_INFO(enModemId);

    pstRabIdInfo->aucRmNetId[ucRabId - RNIC_RAB_ID_OFFSET] = ucRmNetId;

    return VOS_OK;

}
/* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

/*****************************************************************************
 �� �� ��  : RNIC_RcvIpv4PdpActInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_IPV4_PDP_ACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��1��31��
   ��    ��   : w00199382
   �޸�����   : ���ű�����ʱ����ʱɾ��
 3.��    ��   : 2012��6��6��
   ��    ��   : A00165503
   �޸�����   : DTS2012060502819: �����ʽ�������貦��, ����Ƶ���ϱ��¼�
 4.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtIpv4PdpActInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_IPV4_PDP_ACT_IND_STRU      *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

    pstRcvInd                           = (AT_RNIC_IPV4_PDP_ACT_IND_STRU *)pstMsg;

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    ucRmNetId                           = pstRcvInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstRcvInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv4PdpActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv4PdpActInd, RabId:%d", pstRcvInd->ucRabId);
        return VOS_ERR;
    }

    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;
    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv4PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv4PdpInfo.ucRabId     = pstRcvInd->ucRabId;
    pstPdpAddr->stIpv4PdpInfo.ulIpv4Addr  = pstRcvInd->ulIpv4Addr;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
    /* ֹͣ���貦�ű�����ʱ�� */
    if (RNIC_RM_NET_ID_0 == ucRmNetId)
    {
        RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_PROTECT);
    }
    /* Added by A00165503 for DTS2012060502819, 2012-06-06, End */

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_RcvIpv6PdpActInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_IPV6_PDP_ACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtIpv6PdpActInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_IPV6_PDP_ACT_IND_STRU      *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

    pstRcvInd                           = (AT_RNIC_IPV6_PDP_ACT_IND_STRU *)pstMsg;

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    ucRmNetId                           = pstRcvInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstRcvInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv6PdpActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv6PdpActInd, RabId:%d", pstRcvInd->ucRabId);
        return VOS_ERR;
    }

    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;
    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv6PdpInfo.ucRabId     = pstRcvInd->ucRabId;
    VOS_MemCpy(pstPdpAddr->stIpv6PdpInfo.aucIpv6Addr,
               pstRcvInd->aucIpv6Addr,
               RNICITF_MAX_IPV6_ADDR_LEN);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    return VOS_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_RcvIpvv46PdpActInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_IPV4V6_PDP_ACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��17��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��6��6��
   ��    ��   : A00165503
   �޸�����   : DTS2012060502819: �����ʽ�������貦��, ����Ƶ���ϱ��¼�
 3.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtIpv4v6PdpActInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_IPV4V6_PDP_ACT_IND_STRU    *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

    pstRcvInd                           = (AT_RNIC_IPV4V6_PDP_ACT_IND_STRU *)pstMsg;

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    ucRmNetId                           = pstRcvInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstRcvInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv4v6PdpActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtIpv4v6PdpActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
    pstSpecNetCardCtx->enModemType          = RNIC_MODEM_TYPE_INSIDE;
    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv4v6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv4v6PdpInfo.ucRabId     = pstRcvInd->ucRabId;
    pstPdpAddr->stIpv4PdpInfo.ucRabId       = pstRcvInd->ucRabId;
    pstPdpAddr->stIpv6PdpInfo.ucRabId       = pstRcvInd->ucRabId;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
    /* ֹͣ���貦�ű�����ʱ�� */
    RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_PROTECT);
    /* Added by A00165503 for DTS2012060502819, 2012-06-06, End */

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    return VOS_OK;

}


/*****************************************************************************
 �� �� ��  : RNIC_RcvPdpDeactInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_PDP_DEACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��6��16��
   ��    ��   : z60575
   �޸�����   : DTS2012061502207: �ϱ�����Ͽ�ʱ��ʱ����ͶƱ��˯�ߣ�����Ӧ��
                ������Ϣǰ�ֽ�����˯
 3.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtPdpDeactInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_PDP_DEACT_IND_STRU         *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpCtxAddr;
    RNIC_UL_CTX_STRU                   *pstUlCtxAddr;
    RNIC_DL_CTX_STRU                   *pstDlCtxAddr;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRmNetId;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

    pstRcvInd                           = (AT_RNIC_PDP_DEACT_IND_STRU *)pstMsg;

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    ucRmNetId                           = pstRcvInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpCtxAddr                       = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ��ȡ�����������ĵ�ַ */
    pstUlCtxAddr                        = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtxAddr                        = RNIC_GetDlCtxAddr(ucRmNetId);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    /* �����IPV4 PDPȥ���� */
    if ((pstPdpCtxAddr->stIpv4PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus))
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdpCtx(&pstPdpCtxAddr->stIpv4PdpInfo);

        /* Modified by f00179208 for DSDA Phase I, 2012-12-10, Begin */
        /* ������0�ϲ��а��貦�ŵĹ��� */
        if (RNIC_RM_NET_ID_0 == ucRmNetId)
        {
            /* ֹͣ���貦�ŶϿ���ʱ�� */
            RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT);

            /* ��ղ��ŶϿ���ʱ����ʱͳ�� */
            RNIC_ClearTiDialDownExpCount();
        }
        /* Modified by f00179208 for DSDA Phase I, 2012-12-10, End */
    }

    if ((pstPdpCtxAddr->stIpv6PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus))
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdpCtx(&pstPdpCtxAddr->stIpv6PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv4v6PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ���IPV4V6 PDP��������Ϣ */
        /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
        RNIC_InitIpv4v6PdpCtx(&pstPdpCtxAddr->stIpv4v6PdpInfo, ucRmNetId);
        /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */
    }

    /* PDP��δ�����ʱ�� */
    if ((RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
        /* ֹͣ����ͳ�ƶ�ʱ�� */
        RNIC_StopTimer(enTimerId);
        /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

        /* ������������� */
        RNIC_InitUlCtx(pstUlCtxAddr);
        RNIC_InitDlCtx(pstDlCtxAddr);

        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
        pstSpecNetCardCtx->enModemType = RNIC_MODEM_TYPE_INSIDE;
        /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */
    }

    /* Add By z60575 for DTS2012061502207, 2012-6-15 begin */
#if (FEATURE_ON == FEATURE_LTE)
    /* �ڶϿ����ųɹ�ʱ���ͶƱ˯�ߣ��Ա��ܽ�����˯ */
    DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_RNIC);
#endif
    /* Add By z60575 for DTS2012061502207, 2012-6-15 end */

    /* Added by f00179208 for DSDA Phase I, 2012-12-10, Begin */
    /* ���RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->ucRabId,
                                        RNIC_RM_NET_ID_BUTT))
    {
        return VOS_ERR;
    }
    /* Added by f00179208 for DSDA Phase I, 2012-12-10, End */

    return VOS_OK;
}

/* Added by f00179208 for DSDA Phase I, 2012-12-10, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_RcvAtMtuChangeInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_MTU_CHANGE_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��21��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtMtuChangeInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_MTU_CHANGE_IND_STRU        *pstRcvInd;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    RNIC_NETCARD_DEV_INFO_STRU         *pstNetDevInfo;

    pstRcvInd                           = (AT_RNIC_MTU_CHANGE_IND_STRU *)pstMsg;

    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(pstRcvInd->ucRmNetId);

    pstNetDevInfo = pstSpecNetCardCtx->pstNetDevInfo;

    pstNetDevInfo->pstNetDev->mtu = (VOS_UINT)pstRcvInd->ulMtuValue;

    return VOS_OK;
}
/* Added by f00179208 for DSDA Phase I, 2012-12-10, End */

/*****************************************************************************
 �� �� ��  : RNIC_RcvAtDialModeReq
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_DIAL_MODE_REQ�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��21��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtDialModeReq(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_DIAL_MODE_REQ_STRU         *pstRcvInd;
    RNIC_DIAL_MODE_STRU                *pstDialMode;
    RNIC_AT_DIAL_MODE_CNF_STRU         *pstSndMsg;

    /* �ڴ���� */
    pstSndMsg = (RNIC_AT_DIAL_MODE_CNF_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_AT_DIAL_MODE_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstSndMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDialModeReq: Malloc failed!");
        return VOS_ERR;
    }

    pstRcvInd                           = (AT_RNIC_DIAL_MODE_REQ_STRU *)pstMsg;

    /* ��ȡ���貦�ŵ�ģʽ�Լ�ʱ���ĵ�ַ */
    pstDialMode                         = RNIC_GetDialModeAddr();
    pstSndMsg->clientId                 = pstRcvInd->clientId;
    pstSndMsg->ulDialMode               = pstDialMode->enDialMode;
    pstSndMsg->ulIdleTime               = pstDialMode->ulIdleTime;
    pstSndMsg->ulEventReportFlag        = pstDialMode->enEventReportFlag;

    /* ͨ��ID_RNIC_AT_DIAL_MODE_CNF��Ϣ���͸�ATģ�� */
    /* �����Ϣ */
    pstSndMsg->ulSenderCpuId            = VOS_LOCAL_CPUID;
    pstSndMsg->ulSenderPid              = ACPU_PID_RNIC;
    pstSndMsg->ulReceiverCpuId          = VOS_LOCAL_CPUID;
    pstSndMsg->ulReceiverPid            = WUEPS_PID_AT;
    pstSndMsg->enMsgId                  = ID_RNIC_AT_DIAL_MODE_CNF;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstSndMsg))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDialModeReq: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_RcvDsflowInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_DSFLOW_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32 VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtDsflowInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_DSFLOW_IND_STRU            *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    RNIC_AT_DSFLOW_RSP_STRU            *pstDsflowRsp;

    /* �ڴ���� */
    pstDsflowRsp = (RNIC_AT_DSFLOW_RSP_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_AT_DSFLOW_RSP_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstDsflowRsp)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDsflowInd: Malloc failed!");
        return VOS_ERR;
    }

    pstRcvInd                               = (AT_RNIC_DSFLOW_IND_STRU *)pstMsg;
    pstDsflowRsp->clientId                  = pstRcvInd->clientId;

    /* Modified by f00179208 for DSDA Phase I, 2012-12-05, Begin */
    pstPdpAddr                              = RNIC_GetPdpCtxAddr(pstRcvInd->enRnicRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-12-05, End */

    /* ��ƷҪ��δ���Ϻţ�����Ϊ0 */
    pstDsflowRsp->stRnicDataRate.ulDLDataRate = 0;
    pstDsflowRsp->stRnicDataRate.ulULDataRate = 0;

    /* PDP�����ʱ�򣬻�ȡ��ǰ������������ */
    if ((RNIC_PDP_REG_STATUS_ACTIVE == pstPdpAddr->stIpv4PdpInfo.enRegStatus)
     || (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpAddr->stIpv6PdpInfo.enRegStatus)
     || (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* Modified by f00179208 for DSDA Phase I, 2012-12-05, Begin */
        pstDsflowRsp->stRnicDataRate.ulDLDataRate = RNIC_GetCurrentDlRate(pstRcvInd->enRnicRmNetId);
        pstDsflowRsp->stRnicDataRate.ulULDataRate = RNIC_GetCurrentUlRate(pstRcvInd->enRnicRmNetId);
        /* Modified by f00179208 for DSDA Phase I, 2012-12-05, End */
    }

    /* ͨ��ID_RNIC_AT_DSFLOW_RSP��Ϣ���͸�ATģ�� */
    /* �����Ϣ */
    pstDsflowRsp->ulSenderCpuId               = VOS_LOCAL_CPUID;
    pstDsflowRsp->ulSenderPid                 = ACPU_PID_RNIC;
    pstDsflowRsp->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstDsflowRsp->ulReceiverPid               = WUEPS_PID_AT;
    pstDsflowRsp->enMsgId                     = ID_RNIC_AT_DSFLOW_RSP;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDsflowRsp))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDsflowInd: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}

/* Added by z00214637 for V3R3 Share-PDP Project, 2013-6-5, begin */
/*****************************************************************************
 �� �� ��  : RNIC_RcvAtPdnInfoCfgInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_PDN_INFO_CFG_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32 VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��4��
    ��    ��   : z00214637
    �޸�����   : V3R3 Share-PDP��Ŀ����
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtPdnInfoCfgInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstRnicPdnCfgInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    ADS_FILTER_IP_ADDR_INFO_STRU        stFilterIpAddr;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;

    PS_MEM_SET(&stFilterIpAddr, 0, sizeof(ADS_FILTER_IP_ADDR_INFO_STRU));
    pstRnicPdnCfgInd = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)pstMsg;
    ucRmNetId        = pstRnicPdnCfgInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);
    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id���Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstRnicPdnCfgInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRnicPdnCfgInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd, RabId:%d", pstRnicPdnCfgInd->ucRabId);
        return VOS_ERR;
    }

    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;
    /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */

    if (VOS_TRUE == pstRnicPdnCfgInd->bitOpIpv4PdnInfo)
    {
        /* ����PDP��������Ϣ */
        pstPdpAddr->stIpv4PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
        pstPdpAddr->stIpv4PdpInfo.ucRabId     = pstRnicPdnCfgInd->ucRabId;
        pstPdpAddr->stIpv4PdpInfo.ulIpv4Addr  = pstRnicPdnCfgInd->stIpv4PdnInfo.ulPdnAddr;

        /* ���¹���IP��ַ��Ϣ */
        stFilterIpAddr.bitOpIpv4Addr          = VOS_TRUE;
        PS_MEM_CPY(stFilterIpAddr.aucIpv4Addr,
                   (VOS_UINT8 *)&pstRnicPdnCfgInd->stIpv4PdnInfo.ulPdnAddr,
                   ADS_IPV4_ADDR_LEN);
    }

    if (VOS_TRUE == pstRnicPdnCfgInd->bitOpIpv6PdnInfo)
    {
        /* ����PDP��������Ϣ */
        pstPdpAddr->stIpv6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
        pstPdpAddr->stIpv6PdpInfo.ucRabId     = pstRnicPdnCfgInd->ucRabId;
        PS_MEM_CPY(pstPdpAddr->stIpv6PdpInfo.aucIpv6Addr,
                   pstRnicPdnCfgInd->stIpv6PdnInfo.aucPdnAddr,
                   RNICITF_MAX_IPV6_ADDR_LEN);

        /* ���¹���IP��ַ��Ϣ */
        stFilterIpAddr.bitOpIpv6Addr          = VOS_TRUE;
        PS_MEM_CPY(stFilterIpAddr.aucIpv6Addr,
                   pstRnicPdnCfgInd->stIpv6PdnInfo.aucPdnAddr,
                   ADS_IPV6_ADDR_LEN);
    }

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ע�����й��˻ص�������ADS����ע��ĺ��������������� */
    ADS_DL_RegFilterDataCallback(ucRabid, &stFilterIpAddr, (RCV_DL_DATA_FUNC)RNIC_RcvAdsDlData);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_RcvAtPdnInfoRelInd
 ��������  : RNIC�յ�AT��ϢID_AT_RNIC_PDN_INFO_REL_REQ�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32 VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��4��
    ��    ��   : z00214637
    �޸�����   : V3R3 Share-PDP��Ŀ����
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtPdnInfoRelInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_PDN_INFO_REL_IND_STRU      *pstRnicPdnRelInd;
    RNIC_PDP_CTX_STRU                  *pstPdpCtxAddr;
    RNIC_UL_CTX_STRU                   *pstUlCtxAddr;
    RNIC_DL_CTX_STRU                   *pstDlCtxAddr;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRmNetId;

    pstRnicPdnRelInd = (AT_RNIC_PDN_INFO_REL_IND_STRU *)pstMsg;

    ucRmNetId                           = pstRnicPdnRelInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpCtxAddr                       = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ��ȡ�����������ĵ�ַ */
    pstUlCtxAddr                        = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtxAddr                        = RNIC_GetDlCtxAddr(ucRmNetId);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ����PDP��������Ϣ */
    if ((pstPdpCtxAddr->stIpv4PdpInfo.ucRabId == pstRnicPdnRelInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus))
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdpCtx(&pstPdpCtxAddr->stIpv4PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv6PdpInfo.ucRabId == pstRnicPdnRelInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus))
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdpCtx(&pstPdpCtxAddr->stIpv6PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv4v6PdpInfo.ucRabId == pstRnicPdnRelInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ���IPV4V6 PDP��������Ϣ */
        RNIC_InitIpv4v6PdpCtx(&pstPdpCtxAddr->stIpv4v6PdpInfo, ucRmNetId);
    }

    /* PDP��δ�����ʱ�� */
    if ((RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ֹͣ����ͳ�ƶ�ʱ�� */
        RNIC_StopTimer(enTimerId);

        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, begin */
        pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;
        /* Modified by l60609 for L-C��������Ŀ, 2014-1-14, end */
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */

        /* ������������� */
        RNIC_InitUlCtx(pstUlCtxAddr);
        RNIC_InitDlCtx(pstDlCtxAddr);
    }

    /* ���RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRnicPdnRelInd->ucRabId,
                                        RNIC_RM_NET_ID_BUTT))
    {
        return VOS_ERR;
    }

    /* ȥע�����й��˻ص����� */
    ADS_DL_DeregFilterDataCallback(pstRnicPdnRelInd->ucRabId);

    return VOS_OK;
}
/* Added by z00214637 for V3R3 Share-PDP Project, 2013-6-5, end */

/*****************************************************************************
 �� �� ��  : RNIC_RcvTiAppDsflowExpired
 ��������  : ����ͳ�ƶ�ʱ����ʱ����������RNIC�����е�ǰ������
 �������  : pstMsg:��ʱ����Ϣ�׵�ַ
             enRmNetId:����ID
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��3��02��
   ��    ��   : f00179208
   �޸�����   : ���ⵥ;DTS2012032406513,���������У���ѯ����Ϊ0
 3.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32  RNIC_RcvTiDsflowStatsExpired(
    MsgBlock                           *pstMsg,
    VOS_UINT8                           ucRmNetId
)
{
    VOS_UINT32                          ulTaBytes;
    VOS_UINT32                          ulRate;
    RNIC_UL_CTX_STRU                   *pstUlCtx;
    RNIC_DL_CTX_STRU                   *pstDlCtx;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    /* Added by f00179208 for DSDA Phase I, 2012-11-22, End */

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    /* ��ȡ�����������ĵ�ַ */
    pstUlCtx                            = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtx                            = RNIC_GetDlCtxAddr(ucRmNetId);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* Added by f00179208 for DTS2012032406513��2012-03-24 Begin */
    RNIC_StopTimer(enTimerId);
    /* Added by f00179208 for DTS2012032406513��2012-03-24 End */

    /* ��ȡ2����������� */
    ulTaBytes = pstDlCtx->stDLDataStats.ulDLPeriodRcvBytes;

    /* ��ƷҪ��ÿ2���Ӽ���һ��,��λΪ:byte/s */
    ulRate = ulTaBytes>>1;
    RNIC_SetCurrentDlRate(ulRate, ucRmNetId);

    /* ��ȡ2����������� */
    ulTaBytes = pstUlCtx->stULDataStats.ulULPeriodSndBytes;

    /* ��ƷҪ��ÿ2���Ӽ���һ��,��λΪ:byte/s */
    ulRate = ulTaBytes>>1;
    RNIC_SetCurrentUlRate(ulRate, ucRmNetId);

    /* ÿ������ͳ�����ڽ�������Ҫ������ͳ��Byte����� */
    pstDlCtx->stDLDataStats.ulDLPeriodRcvBytes = 0;
    pstUlCtx->stULDataStats.ulULPeriodSndBytes = 0;

    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_RcvTiAppDemandDialDownExpired
 ��������  : ���ŶϿ���ʱ����ʱ������֪ͨӦ�öϿ�����
 �������  : pstMsg:��ʱ����Ϣ�׵�ַ
             enRmNetId - ����ID
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��3��02��
   ��    ��   : w00199382
   �޸�����   : DTS201203025734
 3.��    ��   : 2012��6��6��
   ��    ��   : A00165503
   �޸�����   : DTS2012060502819: �����ʽ�������貦��, ����Ƶ���ϱ��¼�
 4.��    ��   : 2012��6��15��
   ��    ��   : z60575
   �޸�����   : DTS2012061502207: �ϱ�����Ͽ�ʱ��ʱ����ͶƱ��˯�ߣ�����Ӧ��
                ������Ϣǰ�ֽ�����˯
 5.��    ��   : 2012��11��23��
   ��    ��   : f00179208
   �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvTiDemandDialDisconnectExpired(
    MsgBlock                           *pstMsg,
    VOS_UINT8                           ucRmNetId
)
{
    VOS_UINT32                          ulPktsNum;
    VOS_UINT32                          ulCount;
    VOS_UINT32                          ulUserExistFlg;
    VOS_UINT32                          ulExpiredCount;
    RNIC_DIAL_MODE_STRU                *pstDialMode;
    RNIC_UL_CTX_STRU                   *pstUlCtx;

    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
    /* ��ȡ�����������ĵ�ַ */
    pstUlCtx                            = RNIC_GetUlCtxAddr(ucRmNetId);
    /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

    /* ��ȡ���貦�ŵ�ģʽ�Լ�ʱ���ĵ�ַ */
    pstDialMode                         = RNIC_GetDialModeAddr();

    /* ���û����õ�ʱ������ʱ��ʱ���ȷ� */
    ulExpiredCount                      = (pstDialMode->ulIdleTime * TI_RNIC_UNIT) / TI_RNIC_DEMAND_DIAL_DISCONNECT_LEN;

    /* Added by w00199382 for DTS201203025734��2012-03-02,  Begin */
    RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT);
    /* Added by w00199382 for DTS201203025734��2012-03-02,  End */

    /* �����ǰ���ֶ�����,��ֱ�ӷ���,����Ҫ֪ͨ */
    if (AT_RNIC_DIAL_MODE_DEMAND_CONNECT != pstDialMode->enDialMode)
    {
        RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_RcvTiDemandDialDisconnectExpired: Dialmode is manual!");
        return VOS_OK;
    }

    /* ��ȡ�ڶ�ʱ��������������ͳ�Ƶ����ݰ����Լ���ǰ�û�����״̬ */
    ulPktsNum = pstUlCtx->stULDataStats.ulULPeriodSndPkts;
    ulUserExistFlg  = DRV_AT_GET_USER_EXIST_FLAG();

    /* ���貦�ŶϿ��߼�����:
         ���û������������ݰ�:
             ����������
         �������:
             ������++
       ���³����޷�ʶ��:
          ��ǰ����USB�����û������ݷ��ͣ��������������·�������Ҳ����Ϊ
          �����û������������ݰ��������¼�����
    */
    if ((0 != ulPktsNum)
     && (VOS_TRUE == ulUserExistFlg))
    {
        RNIC_ClearTiDialDownExpCount();
    }
    else
    {
        RNIC_IncTiDialDownExpCount();
    }

    /* �ж�ͳ�Ƽ������Ƿ���ڵ����û����öϿ�����ʱ���붨ʱ��ʱ���ȷ� */
    ulCount = RNIC_GetTiDialDownExpCount();

    if ((ulCount >= ulExpiredCount)
       &&(RNIC_ALLOW_EVENT_REPORT == pstDialMode->enEventReportFlag))
    {
        /*֪ͨӦ�öϿ����� */
        if (VOS_OK == RNIC_SendDialEvent(DEVICE_ID_WAN, RNIC_DAIL_EVENT_DOWN))
        {
            /* Add By z60575 for DTS2012061502207, 2012-6-15 begin */
            /* �ϱ��Ͽ������¼���ͶƱ������˯�ߣ��ڶϿ����ųɹ�ʱ��� */
#if (FEATURE_ON == FEATURE_LTE)
            DRV_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_RNIC);
#endif
            /* Add By z60575 for DTS2012061502207, 2012-6-15 end */
            /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
            RNIC_DBG_SEND_APP_DIALDOWN_SUCC_NUM(1, ucRmNetId);
            /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */

            RNIC_WARNING_LOG(ACPU_PID_RNIC, "RNIC_ProcUlIpv4DataInPdpDeactive:Send Act PDP Msg to APP");
        }
        else
        {
            /* Modified by f00179208 for DSDA Phase I, 2012-11-22, Begin */
            RNIC_DBG_SEND_APP_DIALDOWN_FAIL_NUM(1, ucRmNetId);
            /* Modified by f00179208 for DSDA Phase I, 2012-11-22, End */
        }

        /* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
        RNIC_MNTN_TraceDialDisconnEvt(ulPktsNum, ulUserExistFlg);
        /* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
    }

    RNIC_StartTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT, TI_RNIC_DEMAND_DIAL_DISCONNECT_LEN);

    return VOS_OK;
}

/* Added by A00165503 for DTS2012060502819, 2012-06-06, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_RcvTiDemandDialProtectExpired
 ��������  : ���ű�����ʱ����ʱ����, ��ʽֹͣ��ʱ��, ��ʱ��״̬��Ϊֹͣ
 �������  : pstMsg  - ��ʱ����Ϣ
             enRmNetId - ����ID
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��6��6��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RNIC_RcvTiDemandDialProtectExpired(
    MsgBlock                           *pstMsg,
    VOS_UINT8                           ucRmNetId
)
{
    /* ֹͣ���貦�ű�����ʱ�� */
    RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_PROTECT);

    return VOS_OK;
}
/* Added by A00165503 for DTS2012060502819, 2012-06-06, End */

/* Added by f00179208 for DSDA Phase I, 2012-12-10, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_GetTiExpiredFuncByMsgId
 ��������  : ��ȡ��Ϣ��������
 �������  : ulMsgId            - ��ϢID
 �������  : ��
 �� �� ֵ  : pTiExpriedProcFunc - ��ʱ����ʱ����������ַ
             VOS_NULL_PTR       - ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��11��28��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���

*****************************************************************************/
RNIC_RCV_TI_EXPRIED_PROC_FUNC RNIC_GetTiExpiredFuncByMsgId(VOS_UINT32 ulMsgId)
{
    VOS_UINT8                           i;
    VOS_UINT32                          ulTableSize;

    /* ��g_astRnicTiExpriedProcTab�л�ȡ�������� */
    ulTableSize = sizeof(g_astRnicTiExpriedProcTab)/sizeof(RNIC_RCV_TI_EXPRIED_PROC_STRU);

    /* g_astRnicTiExpriedProcTab�����������Ϣ�����ķַ� */
    for (i = 0; i < ulTableSize; i++)
    {
        if (g_astRnicTiExpriedProcTab[i].ulMsgId== ulMsgId)
        {
            return g_astRnicTiExpriedProcTab[i].pTiExpriedProcFunc;
        }
    }

    return VOS_NULL_PTR;
}
/* Added by f00179208 for DSDA Phase I, 2012-12-10, End */

/* Added by f00179208 for CCPU RESET, 2013-04-15, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_RcvCcpuResetStartInd
 ��������  : RNIC�յ�RNIC��ϢID_CCPU_RESET_RNIC_START_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2013��04��15��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2014��02��14��
   ��    ��   : m00217266
   �޸�����   : ����L-C��������Ŀ�����ӵ�ȫ�ֱ����ĳ�ʼ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvCcpuResetStartInd(
    MsgBlock                           *pstMsg
)
{
    VOS_UINT8                           ucIndex;
    RNIC_CTX_STRU                      *pstRnicCtx;

    pstRnicCtx                          = RNIC_GetRnicCtxAddr();

    /* ֹͣ���������Ķ�ʱ�� */
    RNIC_StopAllTimer();

    for (ucIndex = 0 ; ucIndex < RNIC_NET_ID_MAX_NUM ; ucIndex++)
    {
        /* ��ʼ��RNIC���������� */
        RNIC_InitUlCtx(&(pstRnicCtx->astSpecCtx[ucIndex].stUlCtx));

        /* ��ʼ��RNIC���������� */
        RNIC_InitDlCtx(&(pstRnicCtx->astSpecCtx[ucIndex].stDlCtx));

        /* ��ʼ��RNIC PDP������ */
        RNIC_InitPdpCtx(&(pstRnicCtx->astSpecCtx[ucIndex].stPdpCtx), ucIndex);

        /* ��ʼ������״̬ */
        pstRnicCtx->astSpecCtx[ucIndex].enFlowCtrlStatus = RNIC_FLOW_CTRL_STATUS_STOP;

        /* ��ʼ������ͳ�ƶ�ʱ�� */
        pstRnicCtx->astSpecCtx[ucIndex].enTiDsFlowStats  = TI_RNIC_DSFLOW_STATS_0 + ucIndex;

        /* ��ʼ��ģ��ID */
        /* ��ʼ����ʱ����Ѿ�ȷ��������������Ӧ�ĸ�MODEM */
        pstRnicCtx->astSpecCtx[ucIndex].enModemId        = RNIC_GET_MODEM_ID_BY_NET_ID(ucIndex);

        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        /* Added by m00217266 for L-C��������Ŀ, 2014-1-27, begin */
        pstRnicCtx->astSpecCtx[ucIndex].enModemType      = RNIC_MODEM_TYPE_INSIDE;
        /* Added by m00217266 for L-C��������Ŀ, 2014-1-27, end */
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */
    }

    /* ��ʼ��RABID��Ϣ */
    for (ucIndex = 0 ; ucIndex < RNIC_MODEM_ID_MAX_NUM ; ucIndex++)
    {
        RNIC_InitRabidInfo(&pstRnicCtx->astRabIdInfo[ucIndex]);
    }

    /* Added by m00217266 for L-C��������Ŀ, 2014-1-27, begin */
    /* ��ʼ��PDNID��Ϣ */
    RNIC_InitPdnIdInfo(&(pstRnicCtx->stPdnIdInfo));
    /* Added by m00217266 for L-C��������Ŀ, 2014-1-27, end */

    /* ��ʼ��RNIC��ʱ�������� */
    RNIC_InitAllTimers(pstRnicCtx->astTimerCtx);

    /* ��ʼ������ģʽ��Ϣ */
    RNIC_ResetDialMode(&(pstRnicCtx->stDialMode));

    /* ��ʼ�����ŶϿ���ʱ����ʱ��������ͳ�� */
    RNIC_ClearTiDialDownExpCount();

    /* ��ʼ��TIMER4���ѱ�־ */
    RNIC_SetTimer4WakeFlg(VOS_FALSE);

    /* �ͷ��ź�����ʹ�õ���API����������� */
    VOS_SmV(RNIC_GetResetSem());

    /* Added by m00217266 for L-C��������Ŀ, 2014-2-17, begin */
    /* �˴��Ƿ�Ҫ���sdio����ע�ắ�� */
    /* Added by m00217266 for L-C��������Ŀ, 2014-2-17, end */

    return VOS_OK;
}
/* Added by f00179208 for CCPU RESET, 2013-04-15, End */

/* Added by f00179208 for VTLTE, 2014-07-31, Begin */
#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
 �� �� ��  : RNIC_RcvImsaPdnActInd
 ��������  : RNIC�յ�IMSA��ϢID_IMSA_RNIC_PDN_ACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2014��07��31��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvImsaPdnActInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_ACT_IND_STRU         *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;

    pstRcvInd                           = (IMSA_RNIC_PDN_ACT_IND_STRU *)pstMsg;

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId                           = RNIC_RM_NET_ID_VT;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);
    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstRcvInd->stPdnInfo.ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaPdnActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return VOS_ERR;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->stPdnInfo.ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaPdnActInd, RabId:%d", pstRcvInd->stPdnInfo.ucRabId);
        return VOS_ERR;
    }

    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;

    /* ����PDP��������Ϣ */

    /* IPV4���� */
    if (VOS_TRUE == pstRcvInd->stPdnInfo.bitOpIpv4PdnInfo)
    {
        pstPdpAddr->stIpv4PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
        pstPdpAddr->stIpv4PdpInfo.ucRabId     = pstRcvInd->stPdnInfo.ucRabId;
    }

    /* IPV6���� */
    if (VOS_TRUE == pstRcvInd->stPdnInfo.bitOpIpv6PdnInfo)
    {
        pstPdpAddr->stIpv6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
        pstPdpAddr->stIpv6PdpInfo.ucRabId     = pstRcvInd->stPdnInfo.ucRabId;
    }

    /* IPV4V6���� */
    if (VOS_TRUE == (pstRcvInd->stPdnInfo.bitOpIpv4PdnInfo & pstRcvInd->stPdnInfo.bitOpIpv6PdnInfo))
    {
        pstPdpAddr->stIpv4v6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
        pstPdpAddr->stIpv4v6PdpInfo.ucRabId     = pstRcvInd->stPdnInfo.ucRabId;
    }

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_RcvImsaPdnDeactInd
 ��������  : RNIC�յ�IMSA��ϢID_IMSA_RNIC_PDN_DEACT_IND�Ĵ���
 �������  : pMsg:��Ϣ�׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2014��07��31��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvImsaPdnDeactInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_DEACT_IND_STRU       *pstRcvInd;
    RNIC_PDP_CTX_STRU                  *pstPdpCtxAddr;
    RNIC_UL_CTX_STRU                   *pstUlCtxAddr;
    RNIC_DL_CTX_STRU                   *pstDlCtxAddr;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx;
    VOS_UINT8                           ucRmNetId;

    pstRcvInd                           = (IMSA_RNIC_PDN_DEACT_IND_STRU *)pstMsg;

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId                           = RNIC_RM_NET_ID_VT;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpCtxAddr                       = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ��ȡ�����������ĵ�ַ */
    pstUlCtxAddr                        = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtxAddr                        = RNIC_GetDlCtxAddr(ucRmNetId);

    /* �����IPV4 PDPȥ���� */
    if ((pstPdpCtxAddr->stIpv4PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus))
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdpCtx(&pstPdpCtxAddr->stIpv4PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv6PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus))
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdpCtx(&pstPdpCtxAddr->stIpv6PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv4v6PdpInfo.ucRabId == pstRcvInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ���IPV4V6 PDP��������Ϣ */
        RNIC_InitIpv4v6PdpCtx(&pstPdpCtxAddr->stIpv4v6PdpInfo, ucRmNetId);
    }

    /* ����������PDP��ȥ�����ʱ����ո���������������Ϣ */
    if ((RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        RNIC_InitUlCtx(pstUlCtxAddr);
        RNIC_InitDlCtx(pstDlCtxAddr);
        pstSpecNetCardCtx->enModemType = RNIC_MODEM_TYPE_INSIDE;
    }

    /* ���RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstRcvInd->ucRabId,
                                        RNIC_RM_NET_ID_BUTT))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
#endif
/* Added by f00179208 for VTLTE, 2014-07-31, End */


/* Modified by l60609 for L-C��������Ŀ, 2014-01-06, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_ProcInsideModemIpv4ActInd
 ��������  : ��modem ipv4����
 �������  : RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��27��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcInsideModemIpv4ActInd(
    RNIC_RMNET_CONFIG_REQ_STRU         *pstPdpStatusInd
)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr          = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstPdpStatusInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcInsideModemIpv4ActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstPdpStatusInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcInsideModemIpv4ActInd, RabId:%d", pstPdpStatusInd->ucRabId);
        return;
    }

    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv4PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv4PdpInfo.ucRabId     = pstPdpStatusInd->ucRabId;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ֹͣ���貦�ű�����ʱ�� */
    if (RNIC_RM_NET_ID_0 == ucRmNetId)
    {
        RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_PROTECT);
    }

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcInsideModemIpv6ActInd
 ��������  : ��modem ipv6����
 �������  : RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��27��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcInsideModemIpv6ActInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRabid;
    VOS_UINT8                           ucRmNetId;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr          = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;


    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucRabid                             = RNIC_RAB_ID_INVALID;

    /* ����modem id������id�����Rabid */
    if (VOS_OK != RNIC_BuildRabIdByModemId(pstSpecNetCardCtx->enModemId,
                                           pstPdpStatusInd->ucRabId,
                                           &ucRabid))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcInsideModemIpv6ActInd, modemId:%d", pstSpecNetCardCtx->enModemId);
        return;
    }

    /* �洢RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstPdpStatusInd->ucRabId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcInsideModemIpv6ActInd, RabId:%d", pstPdpStatusInd->ucRabId);
        return;
    }

    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_INSIDE;

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv6PdpInfo.ucRabId     = pstPdpStatusInd->ucRabId;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    ADS_DL_RegDlDataCallback(ucRabid, RNIC_RcvAdsDlData, ucRmNetId);

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcRnicPdpActInd
 ��������  : ����PDP������Ϣ
 �������  : RNIC_PDP_STATUS_IND_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��7��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcRnicPdpActInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    /* Ĭ��ipv4��ipv6����ɹ��������·� */
    if (RNIC_MODEM_TYPE_INSIDE == pstPdpStatusInd->enModemType)
    {
        if (RNIC_IP_TYPE_IPV4 == pstPdpStatusInd->enIpType)
        {
            /* balong modem ipv4���� */
            RNIC_ProcInsideModemIpv4ActInd(pstPdpStatusInd);
        }

        if (RNIC_IP_TYPE_IPV6 == pstPdpStatusInd->enIpType)
        {
            /* balong modem ipv6���� */
            RNIC_ProcInsideModemIpv6ActInd(pstPdpStatusInd);
        }
    }

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    if (RNIC_MODEM_TYPE_OUTSIDE == pstPdpStatusInd->enModemType)
    {
        if (RNIC_IP_TYPE_IPV4 == pstPdpStatusInd->enIpType)
        {
            /* ����modem ipv4����*/
            RNIC_ProcOutsideModemIpv4ActInd(pstPdpStatusInd);
        }

        if (RNIC_IP_TYPE_IPV6 == pstPdpStatusInd->enIpType)
        {
            /* ����modem ipv6����*/
            RNIC_ProcOutsideModemIpv6ActInd(pstPdpStatusInd);
        }
    }
#endif

    return;
}

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
/*****************************************************************************
 �� �� ��  : RNIC_SaveNetIdByPdnId
 ��������  : ����pdn id��������id
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��7��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RNIC_SaveNetIdByPdnId(
    VOS_UINT8                           ucPdnId,
    VOS_UINT8                           ucRmNetId
)
{
    RNIC_PDNID_INFO_STAU               *pstPdnIdInfo;

    if ((ucPdnId < RNIC_PDN_ID_MIN)
     || (ucPdnId > RNIC_PDN_ID_MAX))
    {
        return VOS_ERR;
    }

    /* ��ȡָ��Modem��RABID��Ϣ */
    pstPdnIdInfo = RNIC_GET_SPEC_MODEM_PDNID_INFO();

    pstPdnIdInfo->aucRmNetId[ucPdnId - RNIC_PDN_ID_OFFSET] = ucRmNetId;

    return VOS_OK;

}


/*****************************************************************************
 �� �� ��  : RNIC_ProcOutsideModemIpv4ActInd
 ��������  : ��modem ipv4����
 �������  : RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��27��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcOutsideModemIpv4ActInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           ucRmNetId;
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr          = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucPdnId                             = pstPdpStatusInd->ucPdnId;

    /* �洢PDNID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByPdnId(ucPdnId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcOutsideModemIpv4ActInd, PdnId:%d", ucPdnId);
        return;
    }

    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_OUTSIDE;

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv4PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv4PdpInfo.ucPdnId     = ucPdnId;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    SDIO_DL_RegDataCallback(ucPdnId, (RCV_C_DL_DATA_FUNC)RNIC_RcvSdioDlData);

    return;
}


/*****************************************************************************
 �� �� ��  : RNIC_ProcOutsideModemIpv6ActInd
 ��������  : ��modem ipv6����
 �������  : RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��27��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcOutsideModemIpv6ActInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           ucRmNetId;
    RNIC_PDP_CTX_STRU                  *pstPdpAddr          = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpAddr                          = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    ucPdnId                             = pstPdpStatusInd->ucPdnId;

    /* �洢PDNID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByPdnId(ucPdnId,
                                        ucRmNetId))
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcOutsideModemIpv6ActInd, PdnId:%d", ucPdnId);
        return;
    }

    pstSpecNetCardCtx->enModemType        = RNIC_MODEM_TYPE_OUTSIDE;

    /* ����PDP��������Ϣ */
    pstPdpAddr->stIpv6PdpInfo.enRegStatus = RNIC_PDP_REG_STATUS_ACTIVE;
    pstPdpAddr->stIpv6PdpInfo.ucPdnId     = ucPdnId;

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* ��������ͳ�ƶ�ʱ�� */
    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    /* ע�����з��ͺ�����ADS����ע��ĺ��������������� */
    SDIO_DL_RegDataCallback(ucPdnId, (RCV_C_DL_DATA_FUNC)RNIC_RcvSdioDlData);

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ClearIpv4PdpCtxInSwitch
 ��������  : �����������л����������������ipv4 pdp������
 �������  : VOS_UINT8  ucRmNetId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��19��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ClearIpv4PdpCtxInSwitch(
    VOS_UINT8                           ucRmNetId
)
{
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           ucRabId;
    VOS_UINT16                          usModemId;
    RNIC_PDP_CTX_STRU                  *pstPdpCtx           = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucPdnId           = RNIC_PDN_ID_INVALID;
    ucRabId           = RNIC_RAB_ID_INVALID;
    pstPdpCtx         = RNIC_GET_SPEC_NET_CTX(ucRmNetId);
    pstSpecNetCardCtx = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);
    usModemId         = pstSpecNetCardCtx->enModemId;

    /* ����PDP�������ͣ����PDN ID �� RAB ID��Ӧ������id��Ϣ */
    /* ��������modem��Ҫ�ÿ��������ݷ���ָ�� */
    if (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtx->stIpv4PdpInfo.enRegStatus)
    {
        if (RNIC_PDN_ID_INVALID != pstPdpCtx->stIpv4PdpInfo.ucPdnId)
        {
            ucPdnId = pstPdpCtx->stIpv4PdpInfo.ucPdnId;

            if (VOS_OK != RNIC_SaveNetIdByPdnId(ucPdnId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv4PdpCtxInSwitch, Pdnid:%d", ucPdnId);
            }

            SDIO_DL_RegDataCallback(ucPdnId, (RCV_C_DL_DATA_FUNC)VOS_NULL_PTR);
        }

        if (RNIC_RAB_ID_INVALID != pstPdpCtx->stIpv4PdpInfo.ucRabId)
        {
            ucRabId = pstPdpCtx->stIpv4PdpInfo.ucRabId;

            if (VOS_OK != RNIC_SaveNetIdByRabId(usModemId, ucRabId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv4PdpCtxInSwitch, ucRabId:%d", ucRabId);
            }
        }
    }

    /* ���ip������ */
    RNIC_InitIpv4PdpCtx(&(pstPdpCtx->stIpv4PdpInfo));
    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ClearIpv6PdpCtxInSwitch
 ��������  : �����������л����������������ipv6 pdp������
 �������  : VOS_UINT8  ucRmNetId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��19��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ClearIpv6PdpCtxInSwitch(
    VOS_UINT8                           ucRmNetId
)
{
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           ucRabId;
    VOS_UINT16                          usModemId;
    RNIC_PDP_CTX_STRU                  *pstPdpCtx           = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucPdnId           = RNIC_PDN_ID_INVALID;
    ucRabId           = RNIC_RAB_ID_INVALID;
    pstPdpCtx         = RNIC_GET_SPEC_NET_CTX(ucRmNetId);
    pstSpecNetCardCtx = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);
    usModemId         = pstSpecNetCardCtx->enModemId;

    /* ����PDP�������ͣ����PDN ID �� RAB ID��Ӧ������id��Ϣ */
    /* ��������modem��Ҫ�ÿ��������ݷ���ָ�� */
    if (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtx->stIpv6PdpInfo.enRegStatus)
    {
        if (RNIC_PDN_ID_INVALID != pstPdpCtx->stIpv6PdpInfo.ucPdnId)
        {
            ucPdnId = pstPdpCtx->stIpv6PdpInfo.ucPdnId;

            if (VOS_OK != RNIC_SaveNetIdByPdnId(ucPdnId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv6PdpCtxInSwitch, Pdnid:%d", ucPdnId);
            }

            SDIO_DL_RegDataCallback(ucPdnId, (RCV_C_DL_DATA_FUNC)VOS_NULL_PTR);
        }

        if (RNIC_RAB_ID_INVALID != pstPdpCtx->stIpv6PdpInfo.ucRabId)
        {
            ucRabId = pstPdpCtx->stIpv6PdpInfo.ucRabId;

            if (VOS_OK != RNIC_SaveNetIdByRabId(usModemId, ucRabId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv6PdpCtxInSwitch, ucRabId:%d", ucRabId);
            }
        }
    }

    /* ���ip������ */
    RNIC_InitIpv6PdpCtx(&(pstPdpCtx->stIpv6PdpInfo));
    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ClearIpv4v6PdpCtxInSwitch
 ��������  : �����������л����������������ipv4v6 pdp������
 �������  : VOS_UINT8 ucRmNetId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��19��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ClearIpv4v6PdpCtxInSwitch(
    VOS_UINT8                           ucRmNetId
)
{
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           ucRabId;
    VOS_UINT16                          usModemId;
    RNIC_PDP_CTX_STRU                  *pstPdpCtx           = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucPdnId           = RNIC_PDN_ID_INVALID;
    ucRabId           = RNIC_RAB_ID_INVALID;
    pstPdpCtx         = RNIC_GET_SPEC_NET_CTX(ucRmNetId);
    pstSpecNetCardCtx = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);
    usModemId         = pstSpecNetCardCtx->enModemId;

    /* ����PDP�������ͣ����PDN ID �� RAB ID��Ӧ������id��Ϣ */
    /* ��������modem��Ҫ�ÿ��������ݷ���ָ�� */
    if (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtx->stIpv4v6PdpInfo.enRegStatus)
    {
        if (RNIC_PDN_ID_INVALID != pstPdpCtx->stIpv4v6PdpInfo.ucPdnId)
        {
            ucPdnId = pstPdpCtx->stIpv4v6PdpInfo.ucPdnId;

            if (VOS_OK != RNIC_SaveNetIdByPdnId(ucPdnId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv4v6PdpCtxInSwitch, Pdnid:%d", ucPdnId);
            }

            SDIO_DL_RegDataCallback(ucPdnId, (RCV_C_DL_DATA_FUNC)VOS_NULL_PTR);
        }

        if (RNIC_RAB_ID_INVALID != pstPdpCtx->stIpv4v6PdpInfo.ucRabId)
        {
            ucRabId = pstPdpCtx->stIpv4v6PdpInfo.ucRabId;

            if (VOS_OK != RNIC_SaveNetIdByRabId(usModemId, ucRabId, RNIC_RM_NET_ID_BUTT))
            {
                RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ClearIpv4v6PdpCtxInSwitch, ucRabId:%d", ucRabId);
            }
        }
    }

    /* ���ip������ */
    RNIC_InitIpv4v6PdpCtx(&(pstPdpCtx->stIpv4v6PdpInfo), ucRmNetId);
    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ClearSwitchPdpCtx
 ��������  : �����������л����������pdp������
 �������  : ucRmNetId
 �������  : ��
 �� �� ֵ  : VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��10��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ClearPdpCtxInSwitch(
    VOS_UINT8                           ucRmNetId
)
{
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    pstSpecNetCardCtx = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ����PDP�������ͣ��ֱ����PDN ID �� RAB ID��Ӧ������id��Ϣ */
    /* ��������modem��Ҫ�ÿ��������ݷ���ָ�� */
    /* ���ipv4 pdp������ */
    RNIC_ClearIpv4PdpCtxInSwitch(ucRmNetId);

    /* ���ipv6 pdp������ */
    RNIC_ClearIpv6PdpCtxInSwitch(ucRmNetId);

    /* ���ipv4v6 pdp������ */
    RNIC_ClearIpv4v6PdpCtxInSwitch(ucRmNetId);

    /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
    /* ����modem type */
    pstSpecNetCardCtx->enModemType = RNIC_MODEM_TYPE_INSIDE;
    /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcRnicPdpHandoverInd
 ��������  : ����PDP handover��Ϣ
 �������  : RNIC_PDP_STATUS_IND_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��6��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcRnicPdpHandoverInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    VOS_UINT8                           ucRmNetId;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* handover�������̷�Ϊ������
       1���״�handover���req��Ϣ����֮ǰpdp�����ĺ�������Ϣ
       2�����汾��req��PDP�����ģ������̴�����PDP��������һ��*/

    /* step1:�״�handover���pdp�����ĺ�������Ϣ */
    if (pstPdpStatusInd->enModemType != pstSpecNetCardCtx->enModemType)
    {
        RNIC_ClearPdpCtxInSwitch(ucRmNetId);
    }

    /* step2:����PDP������ */
    RNIC_ProcRnicPdpActInd(pstPdpStatusInd);

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcOutsideModemDeactInd
 ��������  : �����ⲿmodem PDPȥ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��8��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcOutsideModemDeactInd(
    RNIC_RMNET_CONFIG_REQ_STRU         *pstPdpStatusInd
)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRmNetId;
    RNIC_PDP_CTX_STRU                  *pstPdpCtxAddr       = VOS_NULL_PTR;
    RNIC_UL_CTX_STRU                   *pstUlCtxAddr        = VOS_NULL_PTR;
    RNIC_DL_CTX_STRU                   *pstDlCtxAddr        = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpCtxAddr                       = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ��ȡ�����������ĵ�ַ */
    pstUlCtxAddr                        = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtxAddr                        = RNIC_GetDlCtxAddr(ucRmNetId);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* �����IPV4 PDPȥ���� */
    if ((pstPdpCtxAddr->stIpv4PdpInfo.ucPdnId == pstPdpStatusInd->ucPdnId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus))
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdpCtx(&pstPdpCtxAddr->stIpv4PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv6PdpInfo.ucPdnId == pstPdpStatusInd->ucPdnId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus))
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdpCtx(&pstPdpCtxAddr->stIpv6PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv4v6PdpInfo.ucPdnId == pstPdpStatusInd->ucPdnId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ���IPV4V6 PDP��������Ϣ */
        RNIC_InitIpv4v6PdpCtx(&pstPdpCtxAddr->stIpv4v6PdpInfo, ucRmNetId);
    }

    /* PDP��δ�����ʱ�� */
    if ((RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ֹͣ����ͳ�ƶ�ʱ�� */
        RNIC_StopTimer(enTimerId);

        /* ������������� */
        RNIC_InitUlCtx(pstUlCtxAddr);
        RNIC_InitDlCtx(pstDlCtxAddr);

        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        pstSpecNetCardCtx->enModemType = RNIC_MODEM_TYPE_INSIDE;
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */
    }

    /* ���ע���SDIO�Ļص����� */
    SDIO_DL_RegDataCallback(pstPdpStatusInd->ucPdnId, (RCV_C_DL_DATA_FUNC)VOS_NULL_PTR);

    /* ���PDNID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByPdnId(pstPdpStatusInd->ucPdnId,
                                        RNIC_RM_NET_ID_BUTT))
    {
        return;
    }

    return;
}

#endif

/*****************************************************************************
 �� �� ��  : RNIC_ProcInsideModemDeactInd
 ��������  : �����ڲ�modem PDPȥ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��2��8��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcInsideModemDeactInd(
    RNIC_RMNET_CONFIG_REQ_STRU         *pstPdpStatusInd
)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRmNetId;
    RNIC_PDP_CTX_STRU                  *pstPdpCtxAddr       = VOS_NULL_PTR;
    RNIC_UL_CTX_STRU                   *pstUlCtxAddr        = VOS_NULL_PTR;
    RNIC_DL_CTX_STRU                   *pstDlCtxAddr        = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx   = VOS_NULL_PTR;

    ucRmNetId                           = pstPdpStatusInd->ucRmNetId;

    /* ��ȡPDP�����ĵ�ַ */
    pstPdpCtxAddr                       = RNIC_GetPdpCtxAddr(ucRmNetId);
    pstSpecNetCardCtx                   = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* ��ȡ�����������ĵ�ַ */
    pstUlCtxAddr                        = RNIC_GetUlCtxAddr(ucRmNetId);
    pstDlCtxAddr                        = RNIC_GetDlCtxAddr(ucRmNetId);

    /* ��������ID��ȡ����ͳ�ƵĶ�ʱ��ID */
    enTimerId = RNIC_GetDsflowTimerIdByNetId(ucRmNetId);

    /* �����IPV4 PDPȥ���� */
    if ((pstPdpCtxAddr->stIpv4PdpInfo.ucRabId == pstPdpStatusInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus))
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdpCtx(&pstPdpCtxAddr->stIpv4PdpInfo);

        /* ������0�ϲ��а��貦�ŵĹ��� */
        if (RNIC_RM_NET_ID_0 == ucRmNetId)
        {
            /* ֹͣ���貦�ŶϿ���ʱ�� */
            RNIC_StopTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT);

            /* ��ղ��ŶϿ���ʱ����ʱͳ�� */
            RNIC_ClearTiDialDownExpCount();
        }
    }

    if ((pstPdpCtxAddr->stIpv6PdpInfo.ucRabId == pstPdpStatusInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus))
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdpCtx(&pstPdpCtxAddr->stIpv6PdpInfo);
    }

    if ((pstPdpCtxAddr->stIpv4v6PdpInfo.ucRabId == pstPdpStatusInd->ucRabId)
     && (RNIC_PDP_REG_STATUS_ACTIVE == pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ���IPV4V6 PDP��������Ϣ */
        RNIC_InitIpv4v6PdpCtx(&pstPdpCtxAddr->stIpv4v6PdpInfo, ucRmNetId);
    }

    /* PDP��δ�����ʱ�� */
    if ((RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv6PdpInfo.enRegStatus)
     && (RNIC_PDP_REG_STATUS_ACTIVE != pstPdpCtxAddr->stIpv4v6PdpInfo.enRegStatus))
    {
        /* ֹͣ����ͳ�ƶ�ʱ�� */
        RNIC_StopTimer(enTimerId);

        /* ������������� */
        RNIC_InitUlCtx(pstUlCtxAddr);
        RNIC_InitDlCtx(pstDlCtxAddr);

        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        pstSpecNetCardCtx->enModemType = RNIC_MODEM_TYPE_INSIDE;
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* �ڶϿ����ųɹ�ʱ���ͶƱ˯�ߣ��Ա��ܽ�����˯ */
    DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_RNIC);
#endif

    /* ���RABID��Ӧ������ID */
    if (VOS_OK != RNIC_SaveNetIdByRabId(pstSpecNetCardCtx->enModemId,
                                        pstPdpStatusInd->ucRabId,
                                        RNIC_RM_NET_ID_BUTT))
    {
        return;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcRnicPdpDeactInd
 ��������  : ����PDPȥ������Ϣ
 �������  : RNIC_PDP_STATUS_IND_STRU           *pstPdpStatusInd
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��6��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_ProcRnicPdpDeactInd(
    RNIC_RMNET_CONFIG_REQ_STRU           *pstPdpStatusInd
)
{
    /* ��ն�Ӧ������PDP������ */
    if (RNIC_MODEM_TYPE_INSIDE == pstPdpStatusInd->enModemType)
    {
        /* ����ڲ�modem PDPȥ������������� */
        RNIC_ProcInsideModemDeactInd(pstPdpStatusInd);
    }
#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    if (RNIC_MODEM_TYPE_OUTSIDE == pstPdpStatusInd->enModemType)
    {
        /* ����ⲿmodem PDPȥ������������� */
        RNIC_ProcOutsideModemDeactInd(pstPdpStatusInd);
    }
#endif
    return;
}


/*****************************************************************************
 �� �� ��  : RNIC_RcvRnicRmnetConfigReq
 ��������  : ����ID_RNIC_RMNET_CONFIG_REQ��Ϣ
 �������  : MsgBlock                           *pstMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��6��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���
  2.��    ��   : 2014��6��3��
    ��    ��   : m00217266
    �޸�����   : DTS2014052902453
*****************************************************************************/
VOS_UINT32 RNIC_RcvRnicRmnetConfigReq(
    MsgBlock                           *pstMsg
)
{
    VOS_UINT8                           ucRmNetId;
    RNIC_RMNET_CONFIG_REQ_STRU         *pstRmnetConfigReq = VOS_NULL_PTR;
    RNIC_SPEC_CTX_STRU                 *pstSpecNetCardCtx = VOS_NULL_PTR;

    pstRmnetConfigReq       = (RNIC_RMNET_CONFIG_REQ_STRU *)pstMsg;
    ucRmNetId               = pstRmnetConfigReq->ucRmNetId;
    pstSpecNetCardCtx       = RNIC_GetSpecNetCardCtxAddr(ucRmNetId);

    /* pdp����ɹ���rnic����up */
    if (RNIC_RMNET_STATUS_UP == pstRmnetConfigReq->enRmnetStatus)
    {
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, begin */
        /* RNIC_MODEM_TYPE_BUTT�޸�ΪRNIC_MODEM_TYPE_INSIDE����ҪΪ������v3r3���貦�Ź��� */
        if ((pstSpecNetCardCtx->enModemType == pstRmnetConfigReq->enModemType)
         || (RNIC_MODEM_TYPE_INSIDE == pstSpecNetCardCtx->enModemType))
        {
            RNIC_ProcRnicPdpActInd(pstRmnetConfigReq);
        }
        /* Added by m00217266 for DTS2014052902453, 2014-6-3, end */
    }
#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    /* ����modem ps��handover��rnic����switch */
    else if (RNIC_RMNET_STATUS_SWITCH == pstRmnetConfigReq->enRmnetStatus)
    {
        RNIC_ProcRnicPdpHandoverInd(pstRmnetConfigReq);
    }
#endif
    /* pdpȥ����ɹ���rnic����down */
    else
    {
        if (pstSpecNetCardCtx->enModemType == pstRmnetConfigReq->enModemType)
        {
            RNIC_ProcRnicPdpDeactInd(pstRmnetConfigReq);
        }
    }

    return VOS_OK;
}
/* Modified by l60609 for L-C��������Ŀ, 2014-01-06, End */


/*****************************************************************************
 �� �� ��  : Rnic_RcvAtMsg
 ��������  : RNIC�յ�AT��Ϣ�ķַ�
 �������  : pMsg:AT�ķ�������Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK, VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���
 2.��    ��   : 2012��1��31��
   ��    ��   : w00199382
   �޸�����   : ID_AT_RNIC_DIAL_MODE_IND ID_AT_RNIC_DIAL_MODE_REQɾ��
  3.��    ��   : 2012��11��23��
    ��    ��   : f00179208
    �޸�����   : DSDA Phase I: ����MTU������Ϣ
  4.��    ��   : 2013��6��4��
    ��    ��   : z00214637
    �޸�����   : V3R3 Share-PDP��Ŀ�޸�
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtMsg(MsgBlock *pstMsg)
{
    MSG_HEADER_STRU                    *pstMsgHeader;

    pstMsgHeader = (MSG_HEADER_STRU *)pstMsg;

    switch(pstMsgHeader->ulMsgName)
    {
        case ID_AT_RNIC_IPV4_PDP_ACT_IND:
            RNIC_RcvAtIpv4PdpActInd(pstMsg);
            break;

        case ID_AT_RNIC_IPV6_PDP_ACT_IND:
            RNIC_RcvAtIpv6PdpActInd(pstMsg);
            break;

        case ID_AT_RNIC_IPV4V6_PDP_ACT_IND:
            RNIC_RcvAtIpv4v6PdpActInd(pstMsg);
            break;

        case ID_AT_RNIC_PDP_DEACT_IND:
            RNIC_RcvAtPdpDeactInd(pstMsg);
            break;

        case ID_AT_RNIC_DSFLOW_IND:
            RNIC_RcvAtDsflowInd(pstMsg);
            break;

        case ID_AT_RNIC_DIAL_MODE_REQ:
            RNIC_RcvAtDialModeReq(pstMsg);
            break;

        /* Added by f00179208 for DSDA Phase I, 2012-12-10, Begin */
        case ID_AT_RNIC_MTU_CHANGE_IND:
            RNIC_RcvAtMtuChangeInd(pstMsg);
            break;
        /* Added by f00179208 for DSDA Phase I, 2012-12-10, End */

        /* Added by z00214637 for V3R3 Share-PDP Project, 2013-6-5, begin */
        case ID_AT_RNIC_PDN_INFO_CFG_IND:
            RNIC_RcvAtPdnInfoCfgInd(pstMsg);
            break;

        case ID_AT_RNIC_PDN_INFO_REL_IND:
            RNIC_RcvAtPdnInfoRelInd(pstMsg);
            break;
        /* Added by z00214637 for V3R3 Share-PDP Project, 2013-6-5, end */

        default:
            RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvAtMsg:MsgId", pstMsgHeader->ulMsgName);
            break;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_ProcTimerMsg
 ��������  : RNIC�յ���ʱ����Ϣ��ʱ�ķַ�
 �������  : pMsg:��ʱ����ʱ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK, VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ     :
  1.��    ��   : 2011��12��06��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���
  2.��    ��   : 2012��1��31��
    ��    ��   : w00199382
    �޸�����   : ���ű�����ʱ����ʱɾ��
  3.��    ��   : 2012��11��23��
    ��    ��   : f00179208
    �޸�����   : DSDA Phase I: RNIC��ʵ��
*****************************************************************************/
VOS_UINT32 RNIC_RcvTimerMsg(MsgBlock *pstMsg)
{
    /* Modified by f00179208 for DSDA Phase I, 2012-12-10, Begin */
    REL_TIMER_MSG                      *pstRcvMsg;
    VOS_UINT8                           ucRmNetId;
    RNIC_RCV_TI_EXPRIED_PROC_FUNC       pTiExpriedProcFunc;
    VOS_UINT32                          ulRst;

    pTiExpriedProcFunc = VOS_NULL_PTR;
    pstRcvMsg = (REL_TIMER_MSG *)pstMsg;

    /* ���ݶ�ʱ��ID��ȡ����ID */
    ucRmNetId = RNIC_GetNetIdByTimerId(pstRcvMsg->ulName);

    /* ���Ҷ�ʱ����ʱ����������Ϣ�������� */
    pTiExpriedProcFunc = RNIC_GetTiExpiredFuncByMsgId(pstRcvMsg->ulName);
    if (VOS_NULL_PTR != pTiExpriedProcFunc)
    {
        ulRst = pTiExpriedProcFunc(pstMsg, ucRmNetId);
        if (VOS_ERR == ulRst)
        {
            RNIC_INFO_LOG(ACPU_PID_RNIC, "RNIC_RcvTimerMsg:proc func error! \r\n");
            return VOS_ERR;
        }
    }
    else
    {
        RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_RcvTimerMsg:pstRcvMsg->ulName\r\n", pstRcvMsg->ulName);
    }
    /* Modified by f00179208 for DSDA Phase I, 2012-12-10, End */

    return VOS_OK;

}

/* Added by f00179208 for CCPU RESET, 2013-04-15, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_RcvRnicMsg
 ��������  : RNIC�յ�RNIC��Ϣ�ķַ�
 �������  : pstMsg:RNIC��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK, VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ     :
  1.��    ��   : 2013��04��15��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvRnicMsg(MsgBlock *pstMsg)
{
    MSG_HEADER_STRU                    *pstMsgHeader;

    pstMsgHeader = (MSG_HEADER_STRU *)pstMsg;

    switch(pstMsgHeader->ulMsgName)
    {
        case ID_RNIC_CCPU_RESET_START_IND:
            RNIC_RcvCcpuResetStartInd(pstMsg);
            break;

        case ID_RNIC_CCPU_RESET_END_IND:

            /* do nothing */
            RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_RcvRnicMsg: rcv ID_CCPU_RNIC_RESET_END_IND");
            break;

        /* Modified by l60609 for L-C��������Ŀ, 2014-01-06, Begin */
        case ID_RNIC_RMNET_CONFIG_REQ:
            RNIC_RcvRnicRmnetConfigReq(pstMsg);
            break;

        /* Modified by l60609 for L-C��������Ŀ, 2014-01-06, End */

        default:
            RNIC_NORMAL_LOG1(ACPU_PID_RNIC, "RNIC_RcvRnicMsg: rcv error msg id %d\r\n", pstMsgHeader->ulMsgName);
            break;
    }

    return VOS_OK;
}
/* Added by f00179208 for CCPU RESET, 2013-04-15, Begin */

/* Added by f00179208 for VTLTE, 2014-07-31, Begin */
#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
 �� �� ��  : RNIC_RcvImsaMsg
 ��������  : RNIC�յ�IMSA��Ϣ�ķַ�
 �������  : pstMsg:IMSA��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK, VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ     :
  1.��    ��   : 2014��07��31��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_RcvImsaMsg(MsgBlock *pstMsg)
{
    MSG_HEADER_STRU                    *pstMsgHeader;

    pstMsgHeader = (MSG_HEADER_STRU *)pstMsg;

    switch(pstMsgHeader->ulMsgName)
    {
        case ID_IMSA_RNIC_PDN_ACT_IND:
            RNIC_RcvImsaPdnActInd(pstMsg);
            break;

        case ID_IMSA_RNIC_PDN_DEACT_IND:
            RNIC_RcvImsaPdnDeactInd(pstMsg);
            break;

        default:
            RNIC_NORMAL_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaMsg: rcv error msg id %d\r\n", pstMsgHeader->ulMsgName);
            break;
    }

    return VOS_OK;
}
#endif
/* Added by f00179208 for VTLTE, 2014-07-31, End */

/*****************************************************************************
 �� �� ��  : RNIC_PidMsgProc
 ��������  : RNIC���ݽ���task��Ϣ��������
 �������  : pMsg:��������Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32:VOS_OK, VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   ��    ��   : f00179208
   �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RNIC_ProcMsg (MsgBlock *pstMsg)
{
    if (VOS_NULL_PTR == pstMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcMsg: Msg is invalid!");
        return VOS_ERR;
    }

    /* ��Ϣ�ķַ����� */
    switch (pstMsg->ulSenderPid)
    {
        case WUEPS_PID_AT:

            /* ����AT��Ϣ */
            RNIC_RcvAtMsg(pstMsg);
            break;

        case VOS_PID_TIMER:

            /* ���ն�ʱ����ʱ��Ϣ */
            RNIC_RcvTimerMsg(pstMsg);
            break;

        /* Added by f00179208 for CCPU RESET, 2013-04-15, Begin */
        case ACPU_PID_RNIC:

            /* ����RNIC����Ϣ*/
            RNIC_RcvRnicMsg(pstMsg);
            break;
        /* Added by f00179208 for CCPU RESET, 2013-04-15, End */

        /* Added by f00179208 for VTLTE, 2014-07-31, Begin */
#if (FEATURE_ON == FEATURE_IMS)
        case PS_PID_IMSA:

            /* ����IMSA����Ϣ*/
            RNIC_RcvImsaMsg(pstMsg);
            break;
#endif
        /* Added by f00179208 for VTLTE, 2014-07-31, End */

        default:
            RNIC_INFO_LOG1(ACPU_PID_RNIC, "RNIC_ProcMsg:SendPid", pstMsg->ulSenderPid);
            break;
    }

    return VOS_OK;
}

/* Modified by m00217266 for L-C��������Ŀ, 2014-01-06, Begin */
/*****************************************************************************
 �� �� ��  : RNIC_SndRnicRmnetConfigReq
 ��������  : RNIC�ڲ�����PDP ״̬��Ϣ
 �������  :
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��18��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RNIC_SndRnicRmnetConfigReq(
    RNIC_RMNET_CONFIG_STRU             *pstConfigInfo
)
{
    RNIC_RMNET_CONFIG_REQ_STRU         *pstSndMsg = VOS_NULL_PTR;

    /* �ڴ���� */
    pstSndMsg = (RNIC_RMNET_CONFIG_REQ_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_RMNET_CONFIG_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstSndMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SndRnicRmnetConfigMsg: Malloc failed!");
        return VOS_ERR;
    }

    /* �����Ϣͷ */
    pstSndMsg->ulSenderCpuId            = VOS_LOCAL_CPUID;
    pstSndMsg->ulSenderPid              = ACPU_PID_RNIC;
    pstSndMsg->ulReceiverCpuId          = VOS_LOCAL_CPUID;
    pstSndMsg->ulReceiverPid            = ACPU_PID_RNIC;
    pstSndMsg->enMsgId                  = ID_RNIC_RMNET_CONFIG_REQ;

    /* �����Ϣ */
    pstSndMsg->enModemType              = pstConfigInfo->enModemType;
    pstSndMsg->enRmnetStatus            = pstConfigInfo->enRmnetStatus;
    pstSndMsg->enIpType                 = pstConfigInfo->enIpType;
    pstSndMsg->ucPdnId                  = pstConfigInfo->ucPdnId;
    pstSndMsg->ucRabId                  = pstConfigInfo->ucRabId;
    pstSndMsg->ucRmNetId                = pstConfigInfo->ucRmNetId;
    pstSndMsg->usModemId                = pstConfigInfo->usModemId;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstSndMsg))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SndRnicRmnetConfigMsg: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_SendConfigInfoMsg
 ��������  : ��ά�ɲ⣬��¼�ⲿ���õ�ĳ�ʼ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��28��
    ��    ��   : m00217266
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID RNIC_MNTN_SndRmnetConfigInfoMsg(
    RNIC_RMNET_CONFIG_STRU             *pstConfigInfo
)
{
    RNIC_RMNET_CONFIG_REQ_STRU          *pstSndMsg = VOS_NULL_PTR;

    /* �ڴ���� */
    pstSndMsg = (RNIC_RMNET_CONFIG_REQ_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_RMNET_CONFIG_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstSndMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SndRnicRmnetConfigMsg: Malloc failed!");
        return;
    }

    /* �����Ϣ */
    pstSndMsg->enModemType              = pstConfigInfo->enModemType;
    pstSndMsg->enRmnetStatus            = pstConfigInfo->enRmnetStatus;
    pstSndMsg->enIpType                 = pstConfigInfo->enIpType;
    pstSndMsg->ucPdnId                  = pstConfigInfo->ucPdnId;
    pstSndMsg->ucRabId                  = pstConfigInfo->ucRabId;
    pstSndMsg->ucRmNetId                = pstConfigInfo->ucRmNetId;
    pstSndMsg->usModemId                = pstConfigInfo->usModemId;

    pstSndMsg->ulSenderCpuId            = VOS_LOCAL_CPUID;
    pstSndMsg->ulSenderPid              = ACPU_PID_RNIC;
    pstSndMsg->ulReceiverCpuId          = VOS_LOCAL_CPUID;
    pstSndMsg->ulReceiverPid            = ACPU_PID_RNIC;
    pstSndMsg->enMsgId                  = ID_RNIC_MNTN_RMNET_CONFIG_INFO;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstSndMsg))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SndRnicRmnetConfigMsg: Send msg failed!");
        return;
    }

    return;
}

/* Modified by m00217266 for L-C��������Ŀ, 2014-01-06, End */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
