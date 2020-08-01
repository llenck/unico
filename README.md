helper project to build target-specific code for coroutines, since cmake apparently still can't (easily) do architecture specific compilation in 2020. Based on [github.com/hnes/libaco](https://github.com/hnes/libaco) for x86 and [github.com/llenck/yaco](https://github.com/llenck/yaco) for arm. Will mainly be used as a helper for [github.com/llenck/h2o-wrapper](https://github.com/llenck/h2o-wrapper), so there might be a few api breaks every now and then.

If you want to use this, just take a look at the examples to understand the api. There are only two additional things to look out for:
- Since this library uses libaco for x86 support, the coroutines are assymmetric, and trying to create a coroutine while in a coroutine will probably crash
- If you use valgrind to test your code, it will probably complain about invalid reads/writes for every context switch done on armv7; this is because yaco uses the stack pointer in a non-conforming way, so you shouldn't worry about that
