#ifndef _AMD_FUNCTION_POINTER_LIST_H_
#define _AMD_FUNCTION_POINTER_LIST_H_



/*List of function poitners which needs to be looked from the library*/
typedef int (*va_arg_pyfunc) (PyObject *args, PyObject *kw, const char *format, char **keywords, ...);
typedef PyObject* (*py_new_func) (PyTypeObject* objptr);



#endif //_AMD_FUNCTION_POINTER_LIST_H_