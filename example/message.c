#include <assert.h>
#include <stdio.h>

#include <unico.h>

void printer(unico_co_state* self) {
	printf("starting coroutine...\n");

	while (1) {
		unico_yield(self);
		if (self->data == NULL) break;
		printf("received message: %s\n", (const char*)self->data);
	}

	printf("stopping coroutine...\n");

	unico_exit(self);
}

int main() {
	unico_stack* stk;
	unico_co_state co;

	unico_thread_init();

	assert((stk = unico_create_stack(4096 * 16)) != NULL);

	unico_create_co(&co, stk, printer);

	// co.data is never touched by anything else, so we can use it to pass data back and
	// forth between this function and the coroutine
	co.data = "Hello there!";
	unico_resume(&co);
	co.data = "General Kenobi. You are a bold one.";
	unico_resume(&co);
	co.data = NULL;
	unico_resume(&co);

	assert(unico_is_finished(&co));

	// depending on the platform, the coroutine may have to be destroyed before freeing
	// the stack
	unico_free_co(&co);
	unico_free_stack(stk);

	unico_thread_free();
}
