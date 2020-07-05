#ifndef __CUSMEMORYPOOL_H__
#define	__CUSMEMORYPOOL_H__


#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
	MemP_NULL,
	MemP_OK,
	MemP_NOMOREMEMORY,
	
} MemPool_Return_e;

typedef enum
{
	MemP_OStat_Null,
	MemP_OStat_RW,
	MemP_OStat_R,
	MemP_OStat_Hide,
	MemP_OStat_Deleted,
	
}MemP_OStat_e;

typedef enum
{
	MemP_OType_Null,
	MemP_OType_File,
	MemP_OType_Folder,
	MemP_OType_Raw,
	
}MemP_OType_e;

typedef struct
{
	void * Addr;
	uint32_t Size;
	uint32_t LastObjectIdx;
} MemPool_t;

typedef struct
{
	uint16_t DataSize;
	uint8_t Flag;
} MemPool_ObjectHeader_t;


void MemPool_Create(MemPool_t* para_mempool, uint8_t *para_datasrc, uint32_t para_size);
void MemPool_Format(MemPool_t* para_mempool);
MemPool_Return_e MemPool_AddObject(MemPool_t* para_mempool, uint8_t *para_objectdata, uint16_t para_objectsize, uint16_t para_flag);
MemPool_Return_e MemPool_DeleteObject(MemPool_t* para_mempool, MemPool_ObjectHeader_t* para_delobject);
MemPool_ObjectHeader_t * MemPool_GetObject(MemPool_t* para_mempool, uint16_t para_flag);

#endif
