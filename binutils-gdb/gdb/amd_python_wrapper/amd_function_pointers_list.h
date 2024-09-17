#ifndef _AMD_FUNCTION_POINTER_LIST_H_
#define _AMD_FUNCTION_POINTER_LIST_H_



/*List of function poitners which needs to be looked from the library*/
typedef int (*va_arg_pyfunc) (PyObject *args, PyObject *kw, const char *format, char **keywords, ...);
typedef PyObject* (*py_new_func) (PyTypeObject* objptr);
typedef int (*pytypeissubtype) (PyTypeObject *, PyTypeObject *);
typedef void (*pyerror_setstring) (PyObject *exception, const char *string  );
typedef PyObject* (*py_convert_str_to_pyobj) (const char* str);

#endif //_AMD_FUNCTION_POINTER_LIST_H_