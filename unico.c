#include "unico.h"

#if USE_LIBACO

__thread aco_t* thread_main_co = NULL;

void unico_thread_init() {
	aco_thread_init(NULL);
	thread_main_co = aco_create(NULL, NULL, 0, NULL, NULL);
}

// for libaco, unico_stack is a typedef for aco_share_stack_t*
int unico_create_stack(unico_stack* stack, size_t size) {
	*stack = aco_share_stack_new(size);

	return *stack == NULL? -1 : 0;
}

static void libaco_arg_wrapper() {
	unico_co_state* state = aco_get_arg();
	state->actual_coro(state);
}

void unico_create_co(unico_co_state* state, unico_stack* stack, unico_coro fn) {
	state->main_co = thread_main_co;
	state->actual_coro = fn;
	state->other_co = aco_create(thread_main_co, *stack, 0, libaco_arg_wrapper, state);

	aco_resume(state->other_co);
}

#elif USE_YACO

// all implemented as macros in unico.h

#endif
