#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__

#include <Python.h>
#include "amd_function_pointers_list.h"


#define check_lib_handeler_execption(handle)    if(!handle){   \
                                                      AMD_lib_exception_failure_handeler();  \
                                                }  \

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw, const char *format, 
                                       char **keywords, ...);
PyObject * AMD_PyUnicode_FromFormat(
    const char *format,   /* ASCII-encoded string  */
    ...
);

int  AMD_Py_IS_TYPE(PyObject *ob, PyTypeObject *type);
int  AMD_PyObject_TypeCheck(PyObject *ob, PyTypeObject*);
PyObject* AMD_PyObject_GetAttrString(PyObject *, const char *);
int  AMD_PyObject_SetAttrString(PyObject *, const char *, PyObject *);
int  AMD_PyObject_HasAttrString(PyObject *, const char *);
PyObject* PyObject_GetAttr(PyObject *, PyObject *);
/*PyAPI_FUNC(int) PyObject_SetAttr(PyObject *, PyObject *, PyObject *);
PyAPI_FUNC(int) PyObject_HasAttr(PyObject *, PyObject *);
PyAPI_FUNC(PyObject *) PyObject_SelfIter(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_GenericGetAttr(PyObject *, PyObject *);
PyAPI_FUNC(int) PyObject_GenericSetAttr(PyObject *, PyObject *, PyObject *);
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(int) PyObject_GenericSetDict(PyObject *, PyObject *, void *);
#endif
PyAPI_FUNC(Py_hash_t) PyObject_Hash(PyObject *);
PyAPI_FUNC(Py_hash_t) PyObject_HashNotImplemented(PyObject *);
PyAPI_FUNC(int) PyObject_IsTrue(PyObject *);
PyAPI_FUNC(int) PyObject_Not(PyObject *);
PyAPI_FUNC(int) PyCallable_Check(PyObject *);
PyAPI_FUNC(void) PyObject_ClearWeakRefs(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_CallObject(PyObject *callable,
                                           PyObject *args);
*/
void AMD_PyErr_SetString(PyObject *exception, const char* );
PyObject* AMD_PyUnicode_FromString(const char *ustr);
PyObject* AMD_PyUnicode_Decode(
    const char *s,              /* encoded string */
    Py_ssize_t size,            /* size of buffer */
    const char *encoding,       /* encoding */
    const char *errors          /* error handling */
    );
PyObject* AMD_PyList_New(int size);
PyObject* AMD_PyDict_New();
PyObject* AMD_PyType_GenericNew(PyTypeObject *,PyObject *, PyObject *);

int AMD_PyList_Append(PyObject * ob1, PyObject * ob2);
int AMD_PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item);
/* 
PyType_Ready
_Py_NoneStruct
_Py_FalseStruct
_Py_TrueStruct
_Py_NotImplementedStruct
_PyObject_CallMethod_SizeT
PyModule_AddIntConstant
PyObject_IsTrue
PyObject_HasAttr
PyObject_GetAttr
AMD_PyObject_GetAttrString
PyObject_CallObject
PyObject_IsTrue
PyErr_Clear
PyErr_ExceptionMatches
PySequence_Check
PySequence_Size
PySequence_GetItem
PyModule_Create2
PyImport_GetModuleDict
PyModule_AddIntConstant
_PyObject_CallMethod_SizeT
PySequence_DelItem
Py_INCREF
Py_DECREF

*/

long AMD_PyLong_AsLong(PyObject *ob);
long long AMD_PyLong_AsLongLong(PyObject *ob);
long AMD_PyLong_AsLongAndOverflow(PyObject *ob);


/*Internal functions which need to be supported here because of the tempalated approach*/
void AMD_lib_exception_failure_handeler();
void* AMD_get_lib_handle();
void* check_symbol_resolution(void* functionpointer, char*);

template<typename T>
T* AMD_PyObject_New(PyTypeObject* objptr){
   AMD_get_lib_handle();   
   char str[]="_PyObject_New";
   py_new_func fp =  NULL;
   fp =  (py_new_func) check_symbol_resolution((void*) fp,str);
   PyObject* retval = (*fp)(objptr);
   return (T*) retval;
}


#endif /* __AMD_PYTHON_WRAPPER__ */
