#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__

#include <Python.h>
#include "amd_function_pointers_list.h"

/*#define AMD_PyObject_New(type, typeobj) PyObject_New(type, typeobj) */
//#define AMD_PyObject_New(type, typeobj) PyObject_New(type, typeobj) 

#define check_lib_handeler_execption(handle)    if(!handle){   \
                                                      AMD_lib_exception_failure_handeler();  \
                                                }  \

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw, const char *format, 
                                       char **keywords, ...);

/*PyObject* AMD_PyObject_New(PyTypeObject*);*/

/*Internal functions which need to be supported here because of the tempalated approach*/
void AMD_lib_exception_failure_handeler();
void* AMD_get_lib_handle();
void* check_symbol_resolution(void* functionpointer, char*);

template<typename T>
T* AMD_PyObject_New(PyTypeObject* objptr){
   void* handle = AMD_get_lib_handle();
   if(!handle) 
      AMD_lib_exception_failure_handeler();
   
   char str[]="_PyObject_New";
   py_new_func fp =  NULL;
   fp =  (py_new_func) check_symbol_resolution((void*) fp,str);
   PyObject* retval = (*fp)(objptr);
   return (T*) retval;
}



#endif /* __AMD_PYTHON_WRAPPER__ */
