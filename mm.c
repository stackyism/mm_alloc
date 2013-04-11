/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "alone",
    /* First member's full name */
    "Saagar Takhi",
    /* First member's email address */
    "201101203@daiict.ac.in",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4 //constant word-size 4
#define DSIZE 8 //constant double word-size 8
#define CHUNKSIZE (1<<12) //size of large chunk of heap of size 4096
#define OVERHEAD 8 //the extra space needed by header and footer

#define PACK(size, alloc) ((size)|(alloc)) //creating a pack of size and allocated bit
#define GET(pointer) (*(size_t *)(pointer)) // Reading a word from where the pointer points
#define PUT(pointer,value) (*(size_t *)(pointer) = (value)) // Writing the value to memory where pointer points
#define GET_SIZE(pointer) (GET(pointer) & ~0x7) // calculating the size from where the pointer points
#define GET_ALLOC(pointer) (GET(pointer) & 0x1) // checking the status of allocation field of block
#define HDRP(block_p) ((char *)(block_p) - WSIZE) // getting the pointer to the header of the block pointed by block_p
#define FTRP(block_p) ((char *)(block_p) + GET_SIZE(HDRP(block_p)) - DSIZE) // getting the pointer to the footer of the block pointed by block_p
#define NEXT_BLKP(block_p) ((char *)(block_p) + GET_SIZE(((char *)(block_p)-WSIZE))) // getting pointer to the next block to the pointed block by block_p
#define PREV_BLKP(block_p) ((char *)(block_p)-GET_SIZE(((char *)(block_p)-DSIZE))) //getting pointer to the previous block to the pointed block by block_p
#define MAX(x, y) ((x) > (y)? (x) : (y))

static void *extend_heap(size_t);


static char *heap_p; // pointer to the first block of the heap created

/* 
 * mm_init - initialize the malloc package.
 */

int mm_init(void)
{
	heap_p = mem_sbrk(4*WSIZE);
	if(heap_p != NULL)
	{
		PUT(heap_p, 0); // 4-byte alignment padding at the start of the heap
		PUT(heap_p + WSIZE, PACK(OVERHEAD, 1)); // 4-byte Prologue HEADER
		PUT(heap_p + DSIZE, PACK(OVERHEAD, 1)); // 4-byte Prologue FOOTER
		PUT(heap_p + WSIZE + DSIZE, PACK(0, 1)); // 4-byte Epilogue of size 0
		heap_p = heap_p + DSIZE; //take the pointer to the Prologue
	
		
	
		/*void * extend_heap = extend_heap(CHUNKSIZE/WSIZE);
		if(extend_heap == NULL)
		{
			return -1;
		}
		else
		{
			return 0;
		}*/
		
	}
	else
	{
		return -1;
	}
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;
	
	if(words % 2 == 0)
	{
		size = words * WSIZE;
	}
	else
	{
		size = (words+1)*SIZE;
	}
	
	bp = mem_sbrk(size);
	
	int block_p = (int) bp;
	
	if(block_p < 0)
	{
		return NULL;
	}
	else
	{
	
	}


}


