#ifndef _UNICO_H_INCLUDED
#define _UNICO_H_INCLUDED

#if defined(__x86_64__) || defined(__i386__)
#	define USE_LIBACO 1
#elif defined(__ARM_ARCH_7A__)
#	define USE_YACO 1
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
#define unico_yield(state) do { (void)(state); aco_yield(); } while (0)

#define unico_exit(state) do { (void) (state); aco_exit(); } while (0)
#define unico_is_finished(state) ((state)->other_co->is_end)

#elif USE_YACO

#include "yaco.h"

typedef struct yaco_stack unico_stack;

// yaco_coro_state already has a data field, so we don't have to define our own struct
typedef struct yaco_coro_state unico_co_state;

#define unico_thread_init() ((void)0)
#define unico_thread_free() ((void)0)

#define unico_create_stack(s, n) yaco_create_stack(s, n)
#define unico_free_stack(s) yaco_destroy_stack(s)

#define unico_create_co(state, stack, fn) yaco_create(state, stack, fn)
#define unico_free_co(state) ((void)(state))

#define unico_resume(state) yaco_switch(state)
#define unico_yield(state) yaco_switch(state)

#define unico_exit(state) yaco_exit(state)
#define unico_is_finished(state) yaco_is_finished(state)

#endif // USE_YACO

#endif // _UNICO_H_INCLUDED
