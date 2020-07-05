#include "memorypool.h"




void MemPool_Create(MemPool_t* para_mempool, uint8_t *para_datasrc, uint32_t para_size)
{
	para_mempool->Addr = para_datasrc;
	para_mempool->Size = para_size;
	 MemPool_Format(para_mempool);
}

void MemPool_Format(MemPool_t* para_mempool)
{
	memset(para_mempool->Addr, 0x00, para_mempool->Size);
	para_mempool->LastObjectIdx = 0;
}


MemPool_Return_e MemPool_AddObject(MemPool_t* para_mempool, uint8_t *para_objectdata, uint16_t para_objectsize, uint16_t para_flag)
{
	uint8_t *__NewObject_DataAddr = 0;
	
	MemPool_ObjectHeader_t* __LastObject = (MemPool_ObjectHeader_t*)(para_mempool->Addr + para_mempool->LastObjectIdx);
	MemPool_ObjectHeader_t* __NewObject = 0;
	
	if (__LastObject->DataSize != 0)
	{
		__NewObject = (MemPool_ObjectHeader_t*)((uint32_t)__LastObject + __LastObject->DataSize + sizeof(MemPool_ObjectHeader_t) - 1);
	}
	else
	{
		__NewObject = __LastObject;

	}
	if (((uint32_t)__NewObject + (uint32_t)para_objectsize + sizeof(MemPool_ObjectHeader_t)) > ((uint32_t)para_mempool->Addr + para_mempool->Size))
	{
		return MemP_NOMOREMEMORY;
	}
	__NewObject->DataSize = para_objectsize;
	__NewObject->Flag = para_flag;
	__NewObject_DataAddr = (uint8_t *)((uint32_t)__NewObject + (uint32_t)sizeof(MemPool_ObjectHeader_t) - 1);
	memcpy((void *)__NewObject_DataAddr, para_objectdata, para_objectsize);
	
	
	para_mempool->LastObjectIdx = (uint32_t)__NewObject - (uint32_t)para_mempool->Addr;
	
	return MemP_OK;
}


MemPool_Return_e MemPool_DeleteObject(MemPool_t* para_mempool, MemPool_ObjectHeader_t* para_delobject)
{
	MemPool_ObjectHeader_t* __LastObject = para_mempool->Addr + para_mempool->LastObjectIdx;
	uint32_t __moveaddr = (uint32_t)para_delobject + (uint32_t)para_delobject->DataSize + (uint32_t)sizeof(MemPool_ObjectHeader_t) - 1;
	uint32_t __movelen = (uint32_t)__LastObject + __LastObject->DataSize + sizeof(MemPool_ObjectHeader_t) - __moveaddr;
	memcpy(para_delobject, (void *)__moveaddr, __movelen);
	para_mempool->LastObjectIdx -= (uint32_t)__LastObject - (uint32_t)para_delobject;
	return MemP_OK;
}

MemPool_ObjectHeader_t * MemPool_GetObject(MemPool_t* para_mempool, uint16_t para_flag)
{
	MemPool_ObjectHeader_t* __Temp = para_mempool->Addr;
	bool __FoundObj = false;
	
	while ((uint32_t)__Temp <= ((uint32_t)para_mempool->Addr + para_mempool->LastObjectIdx))
	{
		if (__Temp->Flag == para_flag)
		{
			__FoundObj = true;
			break;
		}
		
		__Temp = (MemPool_ObjectHeader_t*)((uint32_t)__Temp + __Temp->DataSize + sizeof(MemPool_ObjectHeader_t) -  1);
	}
	
	if (!__FoundObj)
	{
		return 0;
	}
	
	return __Temp;
}
