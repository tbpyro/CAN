/*
 * syscalls.c
 *
 *  Created on: May 28, 2012
 *      Author: narf
 */
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include "stm32f4xx.h"

/* set by linker -> value is undefined only the address of the variable is useful */
extern void __heap_start__;
extern void __heap_end__;

extern void *_sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		// start address of heap
		heap = (unsigned char *) &__heap_start__;
	}
	prev_heap = heap;

	if ((heap + incr) >= (unsigned char *) &__heap_end__) {
		return 0;
	}
	heap += incr;
	return (void *) prev_heap;
}



int lock = 0;

void __malloc_lock(struct _reent *reent){
	__disable_irq();
	++lock;
}

void __malloc_unlock(struct _reent *reent){
	--lock;
	if(lock == 0){
		__enable_irq();
	}
}


