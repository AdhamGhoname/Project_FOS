
#include <inc/lib.h>
// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

#define NUM_OF_HEAP_PAGES (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE
uint32 all_heap_pages[NUM_OF_HEAP_PAGES];

void UH_Allocation(uint32 selected_allocation_start_va, uint32 rounded_size);
void DeAllocation(uint32 selected_allocation_start_va, uint32 rounded_size);
void* WORSTFIT(uint32 required_pages);
void* FindAllocationAddress(uint32 rounded_size);

uint32 heap_allocations_addresses[NUM_OF_HEAP_PAGES];
uint32 heap_allocations_sizes[NUM_OF_HEAP_PAGES];
uint32 after_last_allocation_address ;

//2017 Bonus
int shared_objects_IDs[NUM_OF_HEAP_PAGES];
//=============

int allocation_available = 0;
/*Worstfit Strategy*/
void* WORSTFIT(uint32 required_pages)
{
	uint32 maxFit = 0;
	uint32 max_allocation_start_va = 0;
	uint32 start_va = USER_HEAP_START;
	uint32 pages_found = 0;
	uint32 allocation_start_va = 0;
	allocation_available = 0;

	for(; start_va < USER_HEAP_MAX; start_va += PAGE_SIZE)
	{
		if(all_heap_pages[(start_va - USER_HEAP_START)/PAGE_SIZE] == 0)
		{
			pages_found++;
			if(allocation_start_va == 0)
			{
				allocation_start_va = start_va;
			}
		}
		else
		{
			if(start_va > USER_HEAP_START)
			{
				//check if the last page is empty or not to compare with the max fit
				if(all_heap_pages[((start_va-PAGE_SIZE)-USER_HEAP_START)/PAGE_SIZE] == 0)
				{
					//compare with the last maxfit found
					if(pages_found >= required_pages && pages_found > maxFit)
					{
						max_allocation_start_va = allocation_start_va;
						maxFit = pages_found;
						allocation_available = 1;
					}
				}
				//reset noofpages to count from new free range and compare with the last minfit
				allocation_start_va = 0;
				pages_found = 0;
			}
		}
	}
	if(pages_found >= required_pages && pages_found > maxFit)
	{
		maxFit = pages_found;
		allocation_available = 1;
		max_allocation_start_va = allocation_start_va;
	}
	return (void*) max_allocation_start_va;
}

/*Find Allocation Address*/
void* FindAllocationAddress(uint32 rounded_size)
{
	uint32 required_pages = rounded_size / PAGE_SIZE;
	uint32 selected_allocation_start_va = 0;

	//cprintf("allocating %x.........\n", rounded_size);
	{
		if (sys_isUHeapPlacementStrategyNEXTFIT())
		{
			//selected_allocation_start_va = (uint32) NEXTFIT(required_pages);
		}
		else if (sys_isUHeapPlacementStrategyBESTFIT())
		{
			//selected_allocation_start_va = (uint32) BESTFIT(required_pages);
		}
		else if (sys_isUHeapPlacementStrategyWORSTFIT())
		{
			selected_allocation_start_va = (uint32) WORSTFIT(required_pages);
		}
		else if (sys_isUHeapPlacementStrategyFIRSTFIT())
		{
			//selected_allocation_start_va = (uint32) FIRSTFIT(required_pages);
		}
	}
	return (void*) selected_allocation_start_va ;
}

/*Allocation & Deallocation functions*/
void UH_Allocation(uint32 start_va, uint32 rounded_size)
{
	uint32 va2 = start_va;
	for( ; va2 < (start_va + rounded_size) ; va2+=PAGE_SIZE)
	{
		all_heap_pages[(va2 - USER_HEAP_START) / PAGE_SIZE] = 1;
	}
	//if(returnAddress != NULL) va_user_free_mem = ((uint32)returnAddress + size);

	unsigned int heap_allocation_index = (start_va-USER_HEAP_START)/PAGE_SIZE;
	heap_allocations_addresses[heap_allocation_index] = start_va;
	heap_allocations_sizes[heap_allocation_index] = rounded_size;

	after_last_allocation_address = start_va + rounded_size;
	if (after_last_allocation_address == USER_HEAP_MAX)
		after_last_allocation_address = USER_HEAP_START;

	//cprintf("alloc start address = %x, after_last_allocation_address = %x\n", selected_allocation_start_va, after_last_allocation_address);
}

void DeAllocation(uint32 start_va, uint32 rounded_size)
{
	int index_heap_alloc_sizes = (start_va - USER_HEAP_START)/PAGE_SIZE;

	heap_allocations_sizes[index_heap_alloc_sizes] = 0;
	heap_allocations_addresses[index_heap_alloc_sizes] = 0;
	uint32 va2 = start_va;
	//cprintf("freeing from %x to %x\n",va2 , ((uint32)virtual_address + size) );
	for( ; va2 < (start_va + rounded_size) ; va2+=PAGE_SIZE)
	{
		all_heap_pages[(va2 - USER_HEAP_START) / PAGE_SIZE] = 0;
	}
}

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

void* malloc(uint32 size)
{
	//TODO: [PROJECT 2020 - [5] User Heap] malloc() [User Side]
	// Write your code here, remove the panic and write your code
	panic("malloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement FIRST FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	return 0;
}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//TODO: [PROJECT 2020 - [6] Shared Variables: Creation] smalloc() [User Side]
	// Write your code here, remove the panic and write your code
	panic("smalloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement FIRST FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_createSharedObject(...) to invoke the Kernel for allocation of shared variable
	//		sys_createSharedObject(): if succeed, it returns the ID of the created variable. Else, it returns -ve
	//	4) If the Kernel successfully creates the shared variable, return its virtual address
	//	   Else, return NULL

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//TODO: [PROJECT 2020 - [6] Shared Variables: Get] sget() [User Side]
	// Write your code here, remove the panic and write your code
	panic("sget() is not implemented yet...!!");

	// Steps:
	//	1) Get the size of the shared variable (use sys_getSizeOfSharedObject())
	//	2) If not exists, return NULL
	//	3) Implement FIRST FIT strategy to search the heap for suitable space
	//		to share the variable (should be on 4 KB BOUNDARY)
	//	4) if no suitable space found, return NULL
	//	 Else,
	//	5) Call sys_getSharedObject(...) to invoke the Kernel for sharing this variable
	//		sys_getSharedObject(): if succeed, it returns the ID of the shared variable. Else, it returns -ve
	//	6) If the Kernel successfully share the variable, return its virtual address
	//	   Else, return NULL
	//

	//This function should find the space for sharing the variable
	// ******** ON 4KB BOUNDARY ******************* //

	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2020 - [5] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	panic("free() is not implemented yet...!!");

	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details

}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=============
// [1] sfree():
//=============
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//TODO: [PROJECT 2020 - BOUNS3] Free Shared Variable [User Side]
	// Write your code here, remove the panic and write your code
	panic("sfree() is not implemented yet...!!");

	//	1) you should find the ID of the shared variable at the given address
	//	2) you need to call sys_freeSharedObject()

}


//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2020 - BONUS1] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

}
