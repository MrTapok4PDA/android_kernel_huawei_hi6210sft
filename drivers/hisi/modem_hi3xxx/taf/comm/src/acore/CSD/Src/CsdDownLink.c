/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : CsdDownLink.c
  �� �� ��   : ����
  ��    ��   : w00199382
  ��������   : 2011��12��7��
  ����޸�   :
  ��������   : CSD�������ݴ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��12��7��
    ��    ��   : w00199382
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CsdDownLink.h"
#include "CsdDebug.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767 */
#define    THIS_FILE_ID                 PS_FILE_ID_CSD_DOWN_LINK_C
/*lint -e767 */

#if( FEATURE_ON == FEATURE_CSD )
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
extern VOS_UINT32 AT_SendCsdZcDataToModem(
    VOS_UINT8                           ucIndex,
    IMM_ZC_STRU                        *pstDataBuf
);


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : CSD_DL_ProcIsr
 ��������  : DICC�����ж���Ӧ��������DICC_Initʱע��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��14��
    ��    ��   : w00199382
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID CSD_DL_ProcIsr(VOS_VOID)
{
    VOS_SEM                             hDLDataSem = VOS_NULL_PTR;

    hDLDataSem  = CSD_GetDownLinkDataSem();

#if 0
    /*DICC�ر��ж�*/
    DICC_DisableIsr(ACPU_PID_CSD, DICC_SERVICE_TYPE_CSD_DATA, DICC_CPU_ID_ACPU);
#endif
    /*�ͷ�����ȡ�����ź���*/
    VOS_SmV(hDLDataSem);
}

/*****************************************************************************
 �� �� ��  : CSD_DL_SendData
 ��������  : ��ͨ���л�ȡ���ݲ�����
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��7��
    ��    ��   : w00199382
    �޸�����   : �����ɺ���
  2.��    ��   : 2012��05��21��
    ��    ��   : f00179208
    �޸�����   : ���ⵥ��:DTS2012052101051,C�˹�����������Ҫ������ʵ��ַת��
  3.��    ��   : 2012��8��31��
    ��    ��   : l60609
    �޸�����   : AP������Ŀ���޸�IMM�ӿ�
  4.��    ��   : 2013��05��28��
    ��    ��   : f00179208
    �޸�����   : V3R3 PPP PROJECT
*****************************************************************************/
VOS_VOID CSD_DL_SendData(VOS_VOID)
{
    IMM_ZC_STRU                        *pstCsdDLMem;
    CST_CSD_DATA_IND_STRU               stDLData;
    VOS_UINT32                          ulDICCNodeCnt;
    VOS_UINT32                          ulRslt;
    VOS_CHAR                           *ImmZcData;


    for ( ; ; )
    {
        /*��ȡDICCͨ�����������ݵĸ���*/
        ulDICCNodeCnt = DICC_GetChannelCurrDataCnt(ACPU_PID_CSD,
                                                   DICC_CHAN_ID_DL_CSD_DATA_CHAN,
                                                   DICC_CPU_ID_ACPU);

        if ( DICC_INVALID_VALUE == ulDICCNodeCnt )
        {
            CSD_ERROR_LOG1(ACPU_PID_CSD,
                           "CSD_DL_SendData :: DICC_GetChannelCurrDataCnt is Wrong",
                           ulDICCNodeCnt);
            break;
        }

        if (0 != ulDICCNodeCnt )
        {
           /*��DICCͨ�����Ƴ�����*/
            ulRslt      = DICC_RemoveChannelData(ACPU_PID_CSD,
                                                 DICC_CHAN_ID_DL_CSD_DATA_CHAN,
                                                 (VOS_UINT8 *)(&stDLData),
                                                 DICC_CPU_ID_ACPU);

            if (DICC_OK != ulRslt)
            {
                CSD_ERROR_LOG1(ACPU_PID_CSD,
                              "CSD_DL_SendData:: enMsgId Illage",
                              ulRslt);
                break;
            }

            if (VOS_NULL_PTR == stDLData.pGarbage)
            {
                CSD_ERROR_LOG(ACPU_PID_CSD,
                              "CSD_DL_SendData:: stDLData.pGarbage Is Null");
                break;
            }

            CSD_DBG_DL_RECV_PKT_NUM(1);

            /*����sk_buffer�ڴ�*/
            pstCsdDLMem = IMM_ZcStaticAlloc(stDLData.usLen);

            if (VOS_NULL_PTR == pstCsdDLMem)
            {
                CSD_ERROR_LOG1(ACPU_PID_CSD,
                              "CSD_DL_SendData:: pstCsdDLMem Is Null IMM_ZcStaticAlloc Fail",
                              pstCsdDLMem);

                /*֪ͨcCpu�ͷ�TTFmem*/
                IMM_RemoteFreeTtfMem(stDLData.pGarbage);

                /*�˴���continue���ܵ�����ѭ��*/
                break;

            }

            /*�˲��費��������ƫ������βָ��*/
            /* Modified by l60609 for AP������Ŀ ��2012-08-31 Begin */
            ImmZcData = (VOS_CHAR *)IMM_ZcPut(pstCsdDLMem, stDLData.usLen);
            /* Modified by l60609 for AP������Ŀ ��2012-08-31 End */

            /* Modified by f00179208 for DTS2012052101051, 2012-05-21 Begin */
            PS_MEM_CPY(ImmZcData, (VOS_UINT8 *)TTF_PHY_TO_VIRT((VOS_VOID *)(stDLData.pucData)), stDLData.usLen);
            /* Modified by f00179208 for DTS2012052101051, 2012-05-21 End */

            /*�������ݵ�����,��һ������ΪpppidĿǰ��ʹ�ã�����ʧ��AT���ͷ��ڴ棬
            ���Դ˴�����Ҫ�����ͷ�a���ڴ�*/
            /* Modified by f00179208 for V3R3 PPP RPOJECT 2013-05-28, Begin */
            ulRslt      = AT_SendCsdZcDataToModem(CSD_UL_GetAtClientIndex(), pstCsdDLMem);
            /* Modified by f00179208 for V3R3 PPP RPOJECT 2013-05-28, End */

            if (VOS_OK != ulRslt)
            {
                CSD_DBG_DL_SEND_FAIL_NUM(1);

                CSD_ERROR_LOG(ACPU_PID_CSD,
                              "CSD_DL_SendData:: AT_SendZcDataToModem  Fail");

                /*֪ͨcCpu�ͷ�TTFmem*/
                IMM_RemoteFreeTtfMem(stDLData.pGarbage);

                /*�˴���continue���ܵ�����ѭ��*/
                break;
            }
            CSD_DBG_DL_SEND_PKT_NUM(1);

            /*֪ͨcCpu�ͷ�TTFmem*/
            IMM_RemoteFreeTtfMem(stDLData.pGarbage);

        }
        else
        {

            /*ͨ�������ݷ�����*/
            CSD_NORMAL_LOG1(ACPU_PID_CSD,
                            "CSD_DL_SendData Queue is Null",
                            ulDICCNodeCnt);

            break;
        }

        CSD_NORMAL_LOG1(ACPU_PID_CSD,
                        "CSD_DL_SendData Done",
                        ulDICCNodeCnt);

    }

}
/*****************************************************************************
 �� �� ��  : CSD_DL_ClearData
 ��������  : ���δ��ͨ�绰ʱDICC������������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��14��
    ��    ��   : w00199382
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID CSD_DL_ClearData(VOS_VOID)
{
    VOS_UINT32                          ulDICCNodeCnt;
    VOS_UINT32                          ulRslt;
    CST_CSD_DATA_IND_STRU               stDLData;


    /*��ȡDICCͨ�����������ݵĸ���*/
    ulDICCNodeCnt   = DICC_GetChannelCurrDataCnt(ACPU_PID_CSD,
                                                 DICC_CHAN_ID_DL_CSD_DATA_CHAN,
                                                 DICC_CPU_ID_ACPU);
    if ( DICC_INVALID_VALUE == ulDICCNodeCnt )
    {
        CSD_ERROR_LOG1(ACPU_PID_CSD,
                       "CSD_DL_ClearData :: DICC_GetChannelCurrDataCnt is Wrong",
                       ulDICCNodeCnt);
        return;
    }

    while ((0 != ulDICCNodeCnt))
    {

        /*��DICCͨ�����Ƴ�����*/
        ulRslt      = DICC_RemoveChannelData(ACPU_PID_CSD,
                                             DICC_CHAN_ID_DL_CSD_DATA_CHAN,
                                             (VOS_UINT8 *)(&stDLData),
                                             DICC_CPU_ID_ACPU);

        if (DICC_OK != ulRslt)
        {
            CSD_ERROR_LOG1(ACPU_PID_CSD,
                          "CSD_DL_ClearData:: DICC_RemoveChannelData Fail",
                          ulRslt);
            break;
        }

        ulDICCNodeCnt-- ;

        /*֪ͨcCpu�ͷ�TTFmem*/
        IMM_RemoteFreeTtfMem(stDLData.pGarbage);
    }

    CSD_NORMAL_LOG(ACPU_PID_CSD,"CSD_DL_ClearData Done");
}
/*****************************************************************************
 �� �� ��  : CSD_DL_ProcData
 ��������  : ���������Դ���
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��7��
    ��    ��   : w00199382
    �޸�����   : �����ɺ���
  2.��    ��   : 2012��12��13��
    ��    ��   : L00171473
    �޸�����   : DTS2012121802573, TQE����
*****************************************************************************/
VOS_VOID CSD_DL_ProcDataTask(VOS_VOID)
{
    VOS_SEM                             hDownLinkSem;

    hDownLinkSem = CSD_GetDownLinkDataSem();

    for ( ; ; )
    {

        /* ��ȡ����ȡ�����ź��� */
        if (VOS_OK != VOS_SmP(hDownLinkSem, 0 ))
        {
            CSD_NORMAL_LOG(ACPU_PID_CSD,
                "CSD_DL_ProcDataTask:: VOS_SmP pulDownLinkSem then continue !");

            /* Modified by l00171473 for DTS2012121802573, 2012-12-12, begin */

#ifdef __PC_UT__
            break;
#else
            continue;
#endif
            /* Modified by l00171473 for DTS2012121802573, 2012-12-12, end */
        }

        /*��ȡ��ǰ�Ƿ�Ҷϵ绰*/
        if (AT_CSD_CALL_STATE_ON != CSD_GetCallState())
        {
            /*�ͷ�����*/
            CSD_DL_ClearData();
#if 0
            /*ʹ���ж�*/
            DICC_EnableIsr(ACPU_PID_CSD,
                           DICC_SERVICE_TYPE_CSD_DATA,
                           DICC_CPU_ID_ACPU);
#endif

            CSD_ERROR_LOG(ACPU_PID_CSD,
                          "CSD_DL_ProcDataTask:: AT_CSD_CALL_STATE_OFF Receive ISR");

            /* Modified by l00171473 for DTS2012121802573, 2012-12-12, begin */

#ifdef __PC_UT__
            break;
#else
            continue;
#endif
            /* Modified by l00171473 for DTS2012121802573, 2012-12-12, end */

        }

        CSD_DL_SendData();
#if 0
        /* ʹ���ж� */
        DICC_EnableIsr(ACPU_PID_CSD,
                       DICC_SERVICE_TYPE_CSD_DATA,
                       DICC_CPU_ID_ACPU);
#endif
#ifdef __PC_UT__
            break;
#endif
    }
}

#endif /*FEATURE_CSD*/






#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif