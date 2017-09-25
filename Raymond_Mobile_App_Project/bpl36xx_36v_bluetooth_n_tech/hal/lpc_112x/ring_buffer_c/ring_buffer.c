/*
* @brief Common ring buffer support functions
*
* @note
* Copyright(C) NXP Semiconductors, 2012
* All rights reserved.
*
* @par
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* @par
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
*/

#include "ring_buffer.h"

#ifdef RINGBUF_IRQ_SAFE
#include <cmsis.h>
#define INIT_CRITICAL_RBF() uint32_t priMask = __get_PRIMASK()
#define ENTER_CRITICAL_RBF() __set_PRIMASK(1)
#define LEAVE_CRITICAL_RBF() __set_PRIMASK(priMask)
#else
#define INIT_CRITICAL_RBF()
#define ENTER_CRITICAL_RBF()
#define LEAVE_CRITICAL_RBF()

#endif

/**
 * @brief	Create and initialize a ring buffer
 * @param	pRB	: pointer to ring buffer instance
 * @param	pBuffer: pointer to the buffer for ring buffer data
 * @param   size: The size of buffer pointed by pBuffer
 * @return	>=0:Success ; <0:Failed
 */
int32_t RingBuf_Init(ring_buffer_t* pRB, uint8_t* buffer, uint32_t size )
{
	pRB->pBuf	 = (uint8_t*)buffer;
	pRB->size		 = size;
	pRB->rNdx		 = 0;
	pRB->wNdx		 = 0;
	pRB->cnt = 0;
	return 0;
}

int32_t RingBuf_Deinit(ring_buffer_t* pRB )
{
	pRB = pRB;;
	return 0;
}

/**
 * @brief	Get free bytes of ring buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @return	>=0:Free bytes ; <0:Failed
 */
int32_t RingBuf_GetFreeBytes(ring_buffer_t* pRB )
{
	return pRB->size - pRB->cnt;
}

/**
 * @brief	Get used bytes of ring buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @return	>=0:Used bytes ; <0:Failed
 */
int32_t RingBuf_GetUsedBytes(ring_buffer_t* pRB)
{
	return pRB->cnt;
}

/**
 * @brief	Write new data to buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	pcData: point to data array that will be written to ring buffer
 * @param   dataBytes: bytes to write
 * @return	>=0:Bytes written ; <0:Failed
 * @remark  This function updates the ring buffer
 */
int32_t RingBuf_Write(ring_buffer_t* pRB, const uint8_t* data, uint32_t dataBytes)
{
	uint32_t writeToEnd, bytesToCopy;
	INIT_CRITICAL_RBF();
	ENTER_CRITICAL_RBF();
	/* Calculate the maximum amount we can copy */
	writeToEnd = pRB->size - pRB->wNdx;
	bytesToCopy = MIN(dataBytes, pRB->size - pRB->cnt);
	
	if (bytesToCopy != 0)
	{
		/* Copy as much as we can until we fall off the end of the buffer */
		memcpy(&pRB->pBuf[pRB->wNdx], data, MIN(bytesToCopy, writeToEnd));

		/* Check if we have more to copy to the front of the buffer */
		if (writeToEnd < bytesToCopy)
		{
			memcpy(pRB->pBuf, data + writeToEnd, bytesToCopy - writeToEnd);
		}

		/* Update the wNdx */
		
		pRB->wNdx = (pRB->wNdx + bytesToCopy) % pRB->size;
		pRB->cnt += bytesToCopy;
	}
	LEAVE_CRITICAL_RBF();
	return bytesToCopy;
}

/**
 * @brief	Write 1 new byte data to buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	pcData: point to data byte that will be written to ring buffer
 * @return	1:success; otherwise failed
 * @remark  This function updates the ring buffer. Optimized for byte-by-byte write
 */
int32_t RingBuf_Write1Byte(ring_buffer_t* pRB, const uint8_t *pcData)
{
	uint32_t ret = 0;
	INIT_CRITICAL_RBF();
	ENTER_CRITICAL_RBF();
	if (pRB->cnt < pRB->size)
	{
		pRB->pBuf[pRB->wNdx] = pcData[0];
		pRB->wNdx = (pRB->wNdx + 1) % pRB->size;
		pRB->cnt++;
		ret = 1;
	}
	LEAVE_CRITICAL_RBF();
	return ret;
}

int32_t _prvRingBuf_Read(ring_buffer_t* pRB, uint8_t* data, uint32_t dataBytes, uint32_t isToFree)
{
	uint32_t readToEnd, bytesToCopy;
	INIT_CRITICAL_RBF();
	ENTER_CRITICAL_RBF();
	readToEnd = pRB->size - pRB->rNdx;
	bytesToCopy = MIN(dataBytes, pRB->cnt);
	if (bytesToCopy != 0)
	{
		memcpy(data, &pRB->pBuf[pRB->rNdx], MIN(bytesToCopy, readToEnd));
		
		if (readToEnd < bytesToCopy)
			memcpy(data + readToEnd, &pRB->pBuf[0], bytesToCopy - readToEnd);

		if (isToFree)
		{
			pRB->rNdx = (pRB->rNdx + bytesToCopy) % pRB->size;
			pRB->cnt -= bytesToCopy;	
		}
	}
	LEAVE_CRITICAL_RBF();
	
	return bytesToCopy;
}

/**
 * @brief	Read (copy and remove) data from ring buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	pData : pointer to data array that receives read data
 * @param   dataBytes: bytes to copy
 * @return	>=0:Bytes read ; <0:Failed
 * @remark  This function updates the ring buffer. 
 */
int32_t RingBuf_Read(ring_buffer_t* pRB, uint8_t* data, uint32_t dataBytes)
{
	return _prvRingBuf_Read(pRB, data, dataBytes, 1);
}

/**
 * @brief	Copy but does NOT remove data from ring buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	pData : pointer to data array that receives read data
 * @param   dataBytes: bytes to read
 * @return	>=0:Read bytes ; <0:Failed
 */
int32_t RingBuf_Copy(ring_buffer_t* pRB, uint8_t* data, uint32_t dataBytes)
{
	return _prvRingBuf_Read(pRB, data, dataBytes, 0);
}

/**
 * @brief	Read (copy and remove) 1 oldest byte data from buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	pData: point to data byte that will receive the oldest byte
 * @return	1:success ; otherwise failed
 * @remark  This function updates the ring buffer. Optimized for byte-by-byte read
 */
int32_t RingBuf_Read1Byte(ring_buffer_t* pRB, uint8_t *pData)
{
	uint32_t ret = 0;
	INIT_CRITICAL_RBF();
	ENTER_CRITICAL_RBF();
	if (pRB->cnt != 0)
	{
		pData[0] = pRB->pBuf[pRB->rNdx];
		pRB->rNdx = (pRB->rNdx + 1) % pRB->size;
		pRB->cnt--;
		ret = 1;
	}
	LEAVE_CRITICAL_RBF();
	return ret;
}

/**
 * @brief	Get data pointer to oldest byte in ring buffer, and contigous byte count 
 * @param	pRB	: pointer to the ring buffer instance
 * @param	ppData : pointer to pointer variable that will be updated to point to oldest byte
 * @param   contiguous_bytes: Congigous bytes until roll back
 * @return	>=0:Contiuous bytes until roll back or whole data (if roll back won't happen) ; <0:Failed
 * @remak   Use this function if performance is critical since it does NOT copy data
 *          Use RingBuf_Free() to free (remove) data after use
 */
int32_t RingBuf_Peek(ring_buffer_t* pRB, uint8_t **ppData)
{
	uint32_t readToEnd = pRB->size - pRB->rNdx;
	uint32_t contiguousBytes;
	*ppData = &(pRB->pBuf[pRB->rNdx]);
	contiguousBytes = MIN(readToEnd, (readToEnd + pRB->wNdx) % pRB->size);
	return contiguousBytes;
}

/**
 * @brief	Free (remove) data from ring buffer
 * @param	pRB	: pointer to the ring buffer instance
 * @param	bytesToFree : Bytes to free (remove)
 * @return	remove bytes
 * @remak  Use this function to free data after data get from RingBuf_Peek() is used
 */
int32_t RingBuf_Free(ring_buffer_t* pRB, uint32_t bytesToFree)
{
	INIT_CRITICAL_RBF();
	ENTER_CRITICAL_RBF();
	
	if(bytesToFree > pRB->cnt)
		{
		bytesToFree = pRB->cnt;
		}
	pRB->rNdx = (pRB->rNdx + bytesToFree) % pRB->size;
	pRB->cnt -= bytesToFree;
	
	LEAVE_CRITICAL_RBF();
	return bytesToFree;

}

