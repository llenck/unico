#include "unico.h"

#if USE_LIBACO

__thread aco_t* thread_main_co = NULL;

static void libaco_arg_wrapper() {
	unico_co_state* state = aco_get_arg();
	state->actual_coro(state);
}

void unico_thread_init() {
	aco_thread_init(NULL);
	thread_main_co = aco_create(NULL, NULL, 0, NULL, NULL);
}

void unico_create_co(unico_co_state* state, unico_stack* stack, unico_coro fn) {
	state->main_co = thread_main_co;
	state->actual_coro = fn;
	state->other_co = aco_create(thread_main_co, stack, 0, libaco_arg_wrapper, state);

	aco_resume(state->other_co);
}

#endif
