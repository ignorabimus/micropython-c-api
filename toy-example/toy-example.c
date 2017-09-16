#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#pragma comment(lib, "micropython_d.lib")
#else
#pragma comment(lib, "micropython.lib")
#endif

#include "py/compile.h"
#include "py/runtime.h"
#include "py/gc.h"

int handle_uncaught_exception(mp_obj_base_t *exc);
int do_str(const char *str);

mp_obj_t callback_print(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args)
{
	const char *str = mp_obj_str_get_str(args[0]);
	printf("c_print: %s\n", str);

	return mp_const_none;
}

mp_int_t call_add1(mp_int_t n)
{
	mp_int_t ret;

	nlr_buf_t nlr;
	if (nlr_push(&nlr) == 0) {
		mp_obj_t fun = mp_load_name(qstr_from_str("add1"));
		mp_obj_t arg = mp_obj_new_int(n);
		mp_obj_t o = mp_call_function_1(fun, arg);

		ret = mp_obj_get_int(o);

		nlr_pop();
	} else {
		handle_uncaught_exception(nlr.ret_val);

		return 0;
	}

	return ret;
}

int main(void) {
	const long heap_size = 128 * 1024 * (sizeof(mp_uint_t) / 4);

	char *heap = (char *)malloc(heap_size);
	gc_init(heap, heap + heap_size);
	mp_init();

	// call a C function from Python
	mp_store_name(qstr_from_str("c_print"), mp_obj_new_fun_native(MP_OBJ_NULL, MP_OBJ_NULL, callback_print, m_new0(mp_uint_t, 1)));
	do_str("c_print('Hello world!')\n");

	// call a Python function from C
	do_str("add1 = lambda x:x+1\n");
	printf("call_add1: %d\n", call_add1(10));

	mp_deinit();
	free(heap);

    return 0;
}
