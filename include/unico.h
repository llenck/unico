#ifndef _UNICO_H_INCLUDED
#define _UNICO_H_INCLUDED

#if defined(__x86_64__) || defined(__i386__)
#	define USE_LIBACO 1
//#elif defined(__ARM_ARCH_7A__)
//#	define USE_YACO 1
#else
#	error platform not supported (yet)
#endif

#if USE_LIBACO

#include "aco.h"

typedef aco_share_stack_t* unico_stack;

typedef struct unico_co_state_s unico_co_state;

typedef void (*unico_coro)(unico_co_state*);

struct unico_co_state_s {
	aco_t* main_co;
	aco_t* other_co;
	unico_coro actual_coro;
	void* data; // aco_get_arg is already used, and this is much more convenient anyway
};

extern __thread aco_t* thread_main_co;

void unico_thread_init();
#define unico_thread_free() aco_destroy(thread_main_co)

int unico_create_stack(unico_stack* sp, size_t size);
#define unico_free_stack(s) aco_share_stack_destroy(*(s))

void unico_create_co(unico_co_state* state, unico_stack* stack, unico_coro fn);
#define unico_free_co(state) aco_destroy((state)->other_co)

#define unico_resume(state) aco_resume((state)->other_co)
#define unico_yield(state) aco_yield()

#define unico_exit(state) aco_exit()
#define unico_is_finished(state) ((state)->other_co->is_end)

#endif

#endif
