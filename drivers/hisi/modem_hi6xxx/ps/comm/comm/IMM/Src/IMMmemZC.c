/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : IMMmemZC.c
  �� �� ��   : ����
  ��    ��   : s00164817
  ��������   : 2011��12��29��
  ����޸�   :
  ��������   : IMM�㿽������ģ��
  �����б�   :
  ˵    ��   : ���뽫��Android�½��б���, ʹ��Cԭʼ������������

  �޸���ʷ   :
  1.��    ��   : 2011��12��29��
    ��    ��   : s00164817
    �޸�����   : �����ļ�

******************************************************************************/



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "IMMmemZC.h"
#include "ImmInterface.h"
#include "v_id.h"
#include "IMMmemRB.h"
#include "IMMmemMntn.h"
#include "pslog.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                 PS_FILE_ID_IMM_ZC_C
/*lint +e767*/
#if (FEATURE_ON == FEATURE_SKB_EXP)

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : IMM_ZcStaticAlloc_Debug
 ��������  : ��A�������ڴ��������ڴ�ķ��亯��
 �������  : ulLen - ������ֽ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : �ɹ�������ָ��IMM_ZC_STRU��ָ��
             ʧ�ܣ�����NULL��
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcStaticAlloc_Debug(unsigned short usFileID, unsigned short usLineNum, unsigned int ulLen)
{
    IMM_ZC_STRU *pstAlloc = VOS_NULL_PTR;


    /* MBB��̬��, ����IMM_ZC�ڴ�(����ڴ�) */
    #if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    pstAlloc = (IMM_ZC_STRU *)dev_alloc_skb_exp_debug(usFileID, usLineNum, ulLen);
    #else
    pstAlloc = (IMM_ZC_STRU *)dev_alloc_skb_exp(ulLen);
    #endif

    return pstAlloc;
}

/*****************************************************************************
 �� �� ��  : IMM_ZcStaticCopy_Debug
 ��������  : IMM_ZC�����ݽṹ�Ŀ��������ƽڵ�����ݿ飩�������ݿ��Linux�Դ����ڴ濽����A�˹����ڴ�
 �������  : pstImmZc - Դ���ݽṹ
 �������  : ��
 �� �� ֵ  : �ɹ�������Ŀ��IMM_ZC_STRU��ָ��
             ʧ�ܣ�����NULL��
 ���ú���  :
 ��������  :
 ����      : A����������ΪMEM_TYPE_SYS_DEFINED�����ݿ飬���ݴ���C��ǰ��һ��Ҫ���ñ��ӿڣ������ݿ�����A�˹����ڴ档
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcStaticCopy_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum, IMM_ZC_STRU* pstImmZc)
{
    IMM_ZC_STRU *pstCopy = VOS_NULL_PTR;

    /* MBB��̬��, ����IMM_ZC�ڴ�(����ڴ�) */
    pstCopy = skb_copy_exp((pstImmZc));

    return pstCopy;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcHeadFree
 ��������  : �ͷ�IMM_ZC_STRU���ƽڵ㣬���ݿ鲻�ͷš�
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcHeadFree(IMM_ZC_STRU* pstImmZc)
{
    /* FEATURE_SKB_EXP�꿪�ؿ��ƣ��꿪�عرճ����½ӿ��ϱ���, Ϊ�պ��� */
#if (FEATURE_ON ==  FEATURE_SKB_EXP)
    kfree_skb_head((pstImmZc));
#endif


    return;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcMapToImmMem
 ��������  : ��IMM_Zc�㿽�����ƽڵ�ת����IMM_Mem���ƽڵ�
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : �ɹ�:ָ��IMM_MEM_STRU��ָ��;ʧ��:NULL��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : s00164817
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_MEM_STRU *IMM_ZcMapToImmMem_Debug(unsigned short usFileID,
        unsigned short usLineNum, IMM_ZC_STRU *pstImmZc)
{
    IMM_MEM_STRU                       *pstImm = NULL;


    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcMapToImmMem pstImmZc ptr is null! \n");
        return NULL;
    }


    if ( MEM_TYPE_USER_DEFINED != pstImmZc->private_mem.enType )
    {
        IMM_LOG1(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcMapToImmMem Mem type %d invalid! \n", pstImmZc->private_mem.enType);
        return NULL;
    }

    if ( NULL == pstImmZc->private_mem.pMem )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcMapToImmMem stPrivateMem Mem ptr is null! \n");
        return NULL;
    }

    if ( NULL == pstImmZc->head )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcMapToImmMem skbuf Mem ptr is null! \n");
        return NULL;
    }

    /* �����ݹҽӵ�IMM_MEM���ƽڵ� */
    pstImm = pstImmZc->private_mem.pMem;
    pstImmZc->private_mem.pMem = NULL;

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return pstImm;
} /* IMM_ZcMapToImmMem */


/*****************************************************************************
 �� �� ��  : IMM_ZcAddMacHead
 ��������  : ��IMM_ZC_STRU �㿽���ṹ����MACͷ
 �������  : IMM_ZC_STRU *pstImmZc
             unsigned char* pucAddData
             VOS_UINT16 usLen
 �������  : ��
 �� �� ֵ  : VOS_OK ���ӳɹ�
             VOS_ERR ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   :
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcAddMacHead (IMM_ZC_STRU *pstImmZc, const unsigned char* pucAddData)
{
    unsigned char                      *pucDestAddr;


    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcAddMacHead pstImmZc ptr is null! \n");
        return VOS_ERR;
    }

    if ( NULL == pucAddData )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcAddMacHead pucData ptr is null! \n");
        return VOS_ERR;
    }


    if( IMM_MAC_HEADER_RES_LEN > (pstImmZc->data - pstImmZc->head) )
    {
        IMM_LOG2(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcAddMacHead invalid data Len! data = 0x%x, head = 0x%x \n",
                    (VOS_INT32)pstImmZc->data, (VOS_INT32)pstImmZc->head);

        return VOS_ERR;
    }

    pucDestAddr = IMM_ZcPush(pstImmZc,IMM_MAC_HEADER_RES_LEN);
    memcpy(pucDestAddr,pucAddData,IMM_MAC_HEADER_RES_LEN);

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcRemoveMacHead
 ��������  : �Ƴ��㿽���ṹMACͷ
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : VOS_OK ���ӳɹ�
             VOS_ERR ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcRemoveMacHead(IMM_ZC_STRU *pstImmZc)
{
    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcRemoveMacHead pstImmZc ptr is null! \n");
        return VOS_ERR;
    }

    if ( IMM_MAC_HEADER_RES_LEN > (pstImmZc->tail - pstImmZc->data) )
    {
        IMM_LOG2(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcRemoveMacHead invalid data Len! tail = 0x%x, data = 0x%x \n",
                    (VOS_INT32)pstImmZc->tail, (VOS_INT32)pstImmZc->data);

        return VOS_ERR;
    }

    IMM_ZcPull(pstImmZc, IMM_MAC_HEADER_RES_LEN);

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcPush_Debug
 ��������  : �������ӵ���Ч���ݿ��ͷ����
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ��׵�ַ����������������֮������ݿ��ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
             �������ӵ���Ч���ݿ��ͷ��֮ǰ,���ñ��ӿ�
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPush_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    pucRet = skb_push_debug(usFileID, usLineNum, (pstImmZc), (ulLen));
#else
    pucRet = skb_push((pstImmZc), (ulLen));
#endif


    return pucRet;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcPull_Debug
 ��������  : ��IMM_ZCָ������ݿ��ͷ��ȡ�����ݡ�
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - ȡ�����ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ��׵�ַ��������ȡ������֮��ĵ�ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPull_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    pucRet = skb_pull_debug(usFileID, usLineNum, pstImmZc, ulLen);
#else
    pucRet = skb_pull(pstImmZc, ulLen);
#endif


    return pucRet;

}


/*****************************************************************************
 �� �� ��  : IMM_ZcPut_Debug
 ��������  : ����������IMM_ZCָ������ݿ��β����
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ�β����ַ����������������֮ǰ�����ݿ�β����ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPut_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    pucRet = skb_put_debug(usFileID, usLineNum, pstImmZc, ulLen);
#else
    pucRet = skb_put(pstImmZc, ulLen);
#endif


    return pucRet;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcReserve_Debug
 ��������  : Ԥ��IMM_ZCָ������ݿ�ͷ���ռ䡣
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - Ԥ������ͷ���Ŀռ�(byte)
 �������  : ��
 �� �� ֵ  : �ޡ�
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룬Ϊͷ��Ԥ���ռ䡣
             ֻ���ڸշ����IMM_ZC,IMM_ZCָ������ݿ黹û��ʹ�ã�
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcReserve_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    skb_reserve_debug(usFileID, usLineNum, pstImmZc, ulLen);
#else
    skb_reserve(pstImmZc, ulLen);
#endif
}


/*****************************************************************************
 �� �� ��  : IMM_ZcGetUsedLen
 ��������  : ��ȡ�㿽���ṹ�����ݿ��ʹ�ó���
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : �ɹ�:���ص�ʹ�õ����ݳ��ȣ�ʧ��:����IMM_INVALID_VALUE.
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcGetUsedLen (const IMM_ZC_STRU *pstImmZc)
{
    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcGetUsedLen pstImmZc ptr is null! \n");
        return IMM_INVALID_VALUE;
    }

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return pstImmZc->len;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcGetUserApp
 ��������  : �õ�UserApp��
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : �õ�UserApp��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short IMM_ZcGetUserApp(IMM_ZC_STRU *pstImmZc)
{
    return ((pstImmZc)->private_mem.usApp);
}


/*****************************************************************************
 �� �� ��  : IMM_ZcSetUserApp
 ��������  : ����UserApp��
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             usApp - �û��Զ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcSetUserApp (IMM_ZC_STRU *pstImmZc, unsigned short usApp)
{
    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_ZcSetUserApp pstImmZc ptr is null! \n");
        return;
    }

    pstImmZc->private_mem.usApp = usApp;

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueHeadInit_Debug
 ��������  : ���г�ʼ����
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueHeadInit_Debug(unsigned short usFileID, unsigned short usLineNum,
        IMM_ZC_HEAD_STRU* pstList)
{
#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    skb_queue_head_init_debug(usFileID, usLineNum, (pstList));
#else
    skb_queue_head_init((pstList));
#endif
}


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueHead_Debug
 ��������  : Ԫ�ز������ͷ����
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
             pstNew  - �����Ԫ�أ���ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueHead_Debug(unsigned short usFileID, unsigned short usLineNum,
                                 IMM_ZC_HEAD_STRU *list, IMM_ZC_STRU *pstNew)
{
#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    skb_queue_head_debug(usFileID, usLineNum, (list), (pstNew));
#else
    skb_queue_head((list), (pstNew));
#endif
}

/*****************************************************************************
 �� �� ��  : IMM_ZcQueueTail_Debug
 ��������  : Ԫ�ز������β�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
             pstNew  - �����Ԫ�أ���ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueTail_Debug(unsigned short usFileID, unsigned short usLineNum,
                                 IMM_ZC_HEAD_STRU *pstList, IMM_ZC_STRU *pstNew)
{
#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    skb_queue_tail_debug(usFileID, usLineNum, (pstList), (pstNew));
#else
    skb_queue_tail((pstList), (pstNew));
#endif
}

/*****************************************************************************
 �� �� ��  : IMM_ZcDequeueHead_Debug
 ��������  : �Ӷ���ͷ�� ȡԪ�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ָ��IMM_ZC_STRU��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcDequeueHead_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    IMM_ZC_STRU *pstHead = NULL;

#if ((FEATURE_ON ==  FEATURE_SKB_EXP) && (FEATURE_IMM_MEM_DEBUG == FEATURE_ON))
    pstHead = skb_dequeue_debug(usFileID, usLineNum,(pstList));
#else
    pstHead = skb_dequeue((pstList));
#endif


    return pstHead;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcDequeueTail_Debug
 ��������  : �Ӷ���β��ȡԪ�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ָ��IMM_ZC_STRU��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcDequeueTail_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    IMM_ZC_STRU *pstTail = NULL;

#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    pstTail = skb_dequeue_tail_debug(usFileID, usLineNum,(pstList));
#else
    pstTail = skb_dequeue_tail((pstList));
#endif


    return pstTail;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueLen_Debug
 ��������  : �õ������е�Ԫ�ص���Ŀ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : �õ������е�Ԫ�ص���Ŀ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int IMM_ZcQueueLen_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    unsigned int ulLen;

#if (FEATURE_IMM_MEM_DEBUG == FEATURE_ON)
    ulLen = skb_queue_len_debug(usFileID, usLineNum, (pstList));
#else
    ulLen = skb_queue_len((pstList));
#endif


    return ulLen;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcDataTransformImmZc_Debug
 ��������  : ���ݿ�Ĺҽӵ�IMM_ZC_STRU�ϡ�
 �������  : unsigned char *pucData    ���ݿ��ڴ��ַ
             unsigned int ulLen      ���ݿ鳤��
             void *pstTtfMem   ���ݿ���ƽڵ�ָ��
 �������  : ��
 �� �� ֵ  : skbuf ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
IMM_ZC_STRU * IMM_ZcDataTransformImmZc_Debug(unsigned short usFileID,
        unsigned short usLineNum, const unsigned char *pucData, unsigned int ulLen, void *pstTtfMem)
{
    IMM_ZC_STRU                        *pstSkb             = NULL;


    /* MBB��̬, �����ݿ�Ĺҽӵ�IMM_ZC_STRU�� */
    unsigned int                       ulDataReservedHead;
    unsigned int                       ulDataReservedTail = 0;
    unsigned int                       ulDataAllign       = 32;

    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstTtfMem )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_DataTranferImmZc pstTtfMem ptr is null! \n");
        return NULL;
    }

    if ( NULL == pucData )
    {
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_DataTranferImmZc pucData ptr is null! \n");
        IMM_RbRemoteFreeMem(pstTtfMem);
        return NULL;
    }

    ulDataReservedHead  = IMM_MAC_HEADER_RES_LEN;
#if((FEATURE_LTE == FEATURE_OFF) && (FEATURE_WIFI == FEATURE_ON ))
    ulDataReservedTail  = DRV_WIFI_DATA_RESERVED_TAIL(IMM_MAC_HEADER_RES_LEN + ulLen);
#else
    ulDataReservedTail  = ((IMM_MAC_HEADER_RES_LEN + ulLen + (ulDataAllign - 1))&(~(ulDataAllign - 1)))
                                - (ulLen+IMM_MAC_HEADER_RES_LEN);
#endif

    pstSkb = dev_alloc_skb_ext(ulLen, (unsigned char *)pucData, ulDataReservedHead, ulDataReservedTail);


    if ( NULL == pstSkb )
    {
        IMM_RbRemoteFreeMem(pstTtfMem);
        IMM_LOG(UEPS_PID_IMM_RB_RECYCLE, IMM_PRINT_ERROR, "IMM_DataTranferImmZc dev_alloc_skb_ext fail! \n");
        return NULL;
    }

    ((IMM_MEM_STRU *)pstSkb->private_mem.pMem)->pstMemBlk = (void *)pstTtfMem;

    IMM_DEBUG_TRACE_FUNC_LEAVE();


    return pstSkb;
}/* IMM_ZcDataTransformImmZc_Debug */

#else

/*****************************************************************************
 �� �� ��  : IMM_ZcStaticAlloc_Debug
 ��������  : ��A�������ڴ��������ڴ�ķ��亯��
 �������  : ulLen - ������ֽ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : �ɹ�������ָ��IMM_ZC_STRU��ָ��
             ʧ�ܣ�����NULL��
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcStaticAlloc_Debug(unsigned short usFileID, unsigned short usLineNum, unsigned int ulLen)
{
    IMM_ZC_STRU *pstAlloc = VOS_NULL_PTR;

    /* ���ܻ���̬��, ����skbϵͳ�ڴ� */
    pstAlloc = (IMM_ZC_STRU *)IMM_ZcLargeMemAlloc(ulLen);

    return pstAlloc;
}

/*****************************************************************************
 �� �� ��  : IMM_ZcStaticCopy_Debug
 ��������  : IMM_ZC�����ݽṹ�Ŀ��������ƽڵ�����ݿ飩�������ݿ��Linux�Դ����ڴ濽����A�˹����ڴ�
 �������  : pstImmZc - Դ���ݽṹ
 �������  : ��
 �� �� ֵ  : �ɹ�������Ŀ��IMM_ZC_STRU��ָ��
             ʧ�ܣ�����NULL��
 ���ú���  :
 ��������  :
 ����      : A����������ΪMEM_TYPE_SYS_DEFINED�����ݿ飬���ݴ���C��ǰ��һ��Ҫ���ñ��ӿڣ������ݿ�����A�˹����ڴ档
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcStaticCopy_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum, IMM_ZC_STRU* pstImmZc)
{
    return NULL;
}

/*****************************************************************************
 �� �� ��  : IMM_ZcHeadFree
 ��������  : �ͷ�IMM_ZC_STRU���ƽڵ㣬���ݿ鲻�ͷš�
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcHeadFree(IMM_ZC_STRU* pstImmZc)
{
    return;
}

/*****************************************************************************
 �� �� ��  : IMM_ZcMapToImmMem
 ��������  : ��IMM_Zc�㿽�����ƽڵ�ת����IMM_Mem���ƽڵ�
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : �ɹ�:ָ��IMM_MEM_STRU��ָ��;ʧ��:NULL��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : s00164817
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_MEM_STRU *IMM_ZcMapToImmMem_Debug(unsigned short usFileID,
        unsigned short usLineNum, IMM_ZC_STRU *pstImmZc)
{
    return NULL;
} /* IMM_ZcMapToImmMem */


/*****************************************************************************
 �� �� ��  : IMM_ZcAddMacHead
 ��������  : ��IMM_ZC_STRU �㿽���ṹ����MACͷ
 �������  : IMM_ZC_STRU *pstImmZc
             unsigned char* pucAddData
             VOS_UINT16 usLen
 �������  : ��
 �� �� ֵ  : VOS_OK ���ӳɹ�
             VOS_ERR ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   :
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcAddMacHead (IMM_ZC_STRU *pstImmZc, const unsigned char* pucAddData)
{
    unsigned char                      *pucDestAddr;

    if ( NULL == pstImmZc )
    {
        vos_printf("IMM_ZcAddMacHead pstImmZc ptr is null! \n");
        return VOS_ERR;
    }

    if ( NULL == pucAddData )
    {
        vos_printf("IMM_ZcAddMacHead pucData ptr is null! \n");
        return VOS_ERR;
    }


    if( IMM_MAC_HEADER_RES_LEN > (pstImmZc->data - pstImmZc->head) )
    {
        vos_printf("IMM_ZcAddMacHead invalid data Len! data = %p, head = %p \n",
                    pstImmZc->data, pstImmZc->head);

        return VOS_ERR;
    }

    pucDestAddr = IMM_ZcPush(pstImmZc, IMM_MAC_HEADER_RES_LEN);
    memcpy(pucDestAddr, pucAddData, IMM_MAC_HEADER_RES_LEN);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcRemoveMacHead
 ��������  : �Ƴ��㿽���ṹMACͷ
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : VOS_OK ���ӳɹ�
             VOS_ERR ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcRemoveMacHead(IMM_ZC_STRU *pstImmZc)
{
    if ( NULL == pstImmZc )
    {
        vos_printf("IMM_ZcRemoveMacHead pstImmZc ptr is null! \n");
        return VOS_ERR;
    }

    if ( IMM_MAC_HEADER_RES_LEN > pstImmZc->len )
    {
        vos_printf("IMM_ZcRemoveMacHead invalid data Len! tail = %p, data = %p, len = %d \n",
                    skb_tail_pointer(pstImmZc), pstImmZc->data, pstImmZc->len);

        return VOS_ERR;
    }

    IMM_ZcPull(pstImmZc, IMM_MAC_HEADER_RES_LEN);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcPush_Debug
 ��������  : �������ӵ���Ч���ݿ��ͷ����
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ��׵�ַ����������������֮������ݿ��ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
             �������ӵ���Ч���ݿ��ͷ��֮ǰ,���ñ��ӿ�
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPush_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

    pucRet = skb_push((pstImmZc), (ulLen));

    return pucRet;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcPull_Debug
 ��������  : ��IMM_ZCָ������ݿ��ͷ��ȡ�����ݡ�
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - ȡ�����ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ��׵�ַ��������ȡ������֮��ĵ�ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPull_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

    pucRet = skb_pull(pstImmZc, ulLen);

    return pucRet;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcPut_Debug
 ��������  : ����������IMM_ZCָ������ݿ��β����
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : ���ص����ݿ�β����ַ����������������֮ǰ�����ݿ�β����ַ��
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룻
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned char* IMM_ZcPut_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    unsigned char* pucRet = NULL;

    pucRet = skb_put(pstImmZc, ulLen);

    return pucRet;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcReserve_Debug
 ��������  : Ԥ��IMM_ZCָ������ݿ�ͷ���ռ䡣
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             ulLen - Ԥ������ͷ���Ŀռ�(byte)
 �������  : ��
 �� �� ֵ  : �ޡ�
 ���ú���  :
 ��������  :
 ����      : ���ӿ�ֻ�ƶ�ָ�룬Ϊͷ��Ԥ���ռ䡣
             ֻ���ڸշ����IMM_ZC,IMM_ZCָ������ݿ黹û��ʹ�ã�
 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcReserve_Debug(unsigned short usFileID, unsigned short usLineNum,
            IMM_ZC_STRU *pstImmZc, unsigned int ulLen)
{
    skb_reserve(pstImmZc, (int)ulLen);

    return;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcGetUsedLen
 ��������  : ��ȡ�㿽���ṹ�����ݿ��ʹ�ó���
 �������  : IMM_ZC_STRU *pstImmZc
 �������  : ��
 �� �� ֵ  : �ɹ�:���ص�ʹ�õ����ݳ��ȣ�ʧ��:����IMM_INVALID_VALUE.
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
unsigned int IMM_ZcGetUsedLen (const IMM_ZC_STRU *pstImmZc)
{
    IMM_DEBUG_TRACE_FUNC_ENTER();

    if ( NULL == pstImmZc )
    {
        vos_printf("IMM_ZcGetUsedLen pstImmZc ptr is null! \r\n");
        return IMM_INVALID_VALUE;
    }

    IMM_DEBUG_TRACE_FUNC_LEAVE();

    return pstImmZc->len;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcGetUserApp
 ��������  : �õ�UserApp��
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : �õ�UserApp��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned short IMM_ZcGetUserApp(IMM_ZC_STRU *pstImmZc)
{
    return IMM_PRIV_CB(pstImmZc)->usApp;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcSetUserApp
 ��������  : ����UserApp��
 �������  : pstImmZc - ָ��IMM_ZC_STRU��ָ��
             usApp - �û��Զ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcSetUserApp (IMM_ZC_STRU *pstImmZc, unsigned short usApp)
{
    if ( NULL == pstImmZc )
    {
        return;
    }

    IMM_PRIV_CB(pstImmZc)->usApp = usApp;

    return;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcDataTransformImmZc_Debug
 ��������  : ���ݿ�Ĺҽӵ�IMM_ZC_STRU�ϡ�
 �������  : unsigned char *pucData    ���ݿ��ڴ��ַ
             unsigned int ulLen      ���ݿ鳤��
             void *pstTtfMem   ���ݿ���ƽڵ�ָ��
 �������  : ��
 �� �� ֵ  : skbuf ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    ��    ��   : s00164817
    �޸�����   : Created
*****************************************************************************/
IMM_ZC_STRU * IMM_ZcDataTransformImmZc_Debug(unsigned short usFileID,
        unsigned short usLineNum, const unsigned char *pucData, unsigned int ulLen, void *pstTtfMem)
{
    return NULL;
}/* IMM_ZcDataTransformImmZc_Debug */


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueHeadInit_Debug
 ��������  : ���г�ʼ����
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueHeadInit_Debug(unsigned short usFileID, unsigned short usLineNum,
        IMM_ZC_HEAD_STRU* pstList)
{
    skb_queue_head_init((pstList));
}


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueHead_Debug
 ��������  : Ԫ�ز������ͷ����
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
             pstNew  - �����Ԫ�أ���ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueHead_Debug(unsigned short usFileID, unsigned short usLineNum,
                                 IMM_ZC_HEAD_STRU *list, IMM_ZC_STRU *pstNew)
{
    skb_queue_head((list), (pstNew));
}

/*****************************************************************************
 �� �� ��  : IMM_ZcQueueTail_Debug
 ��������  : Ԫ�ز������β�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
             pstNew  - �����Ԫ�أ���ָ��IMM_ZC_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
void IMM_ZcQueueTail_Debug(unsigned short usFileID, unsigned short usLineNum,
                                 IMM_ZC_HEAD_STRU *pstList, IMM_ZC_STRU *pstNew)
{
    skb_queue_tail((pstList), (pstNew));
}

/*****************************************************************************
 �� �� ��  : IMM_ZcDequeueHead_Debug
 ��������  : �Ӷ���ͷ�� ȡԪ�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ָ��IMM_ZC_STRU��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcDequeueHead_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    IMM_ZC_STRU *pstHead = NULL;

    pstHead = skb_dequeue((pstList));


    return pstHead;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcDequeueTail_Debug
 ��������  : �Ӷ���β��ȡԪ�� ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : ָ��IMM_ZC_STRU��ָ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
IMM_ZC_STRU* IMM_ZcDequeueTail_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    IMM_ZC_STRU *pstTail = NULL;

    pstTail = skb_dequeue_tail((pstList));


    return pstTail;
}


/*****************************************************************************
 �� �� ��  : IMM_ZcQueueLen_Debug
 ��������  : �õ������е�Ԫ�ص���Ŀ��
 �������  : pstList - ָ��IMM_ZC_HEAD_STRU��ָ��
 �������  : ��
 �� �� ֵ  : �õ������е�Ԫ�ص���Ŀ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��1��
    ��    ��   : y00171741
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int IMM_ZcQueueLen_Debug(unsigned short usFileID, unsigned short usLineNum,
                             IMM_ZC_HEAD_STRU *pstList)
{
    unsigned int ulLen;

    ulLen = skb_queue_len((pstList));

    return ulLen;
}


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
