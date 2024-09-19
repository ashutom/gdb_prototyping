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

int AMD_Py_IS_TYPE(PyObject *ob, PyTypeObject *type);
int AMD_PyObject_TypeCheck(PyObject *ob, PyTypeObject*);
PyObject* AMD_PyObject_GetAttrString(PyObject *, const char *);
int AMD_PyObject_SetAttrString(PyObject *, const char *, PyObject *);
int AMD_PyObject_HasAttrString(PyObject *, const char *);
PyObject* AMD_PyObject_GetAttr(PyObject * ob, PyObject * at);
int AMD_PyObject_SetAttr(PyObject* a, PyObject* b, PyObject* c);
int AMD_PyObject_HasAttr(PyObject *, PyObject *);
PyObject* AMD_PyObject_GenericGetAttr(PyObject * a, PyObject * b);
int AMD_PyObject_GenericSetAttr(PyObject* a, PyObject* b, PyObject* c);
int AMD_PyObject_IsTrue(PyObject *);
int AMD_PyCallable_Check(PyObject *);
PyObject* AMD_PyObject_CallObject(PyObject *callable, PyObject *args);
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
void AMD_PyErr_Clear(void);
int AMD_PyList_Append(PyObject * ob1, PyObject * ob2);
int AMD_PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item);
int AMD_PyType_Ready(PyTypeObject *);
int AMD_PyModule_AddIntConstant(PyObject *, const char *, long);
int  AMD_PyErr_ExceptionMatches(PyObject *);
int  AMD_PySequence_Check(PyObject *o);
Py_ssize_t AMD_PySequence_Size(PyObject *o);
PyObject * AMD_PySequence_GetItem(PyObject *o, Py_ssize_t i);
int  AMD_PySequence_DelItem(PyObject *o, Py_ssize_t i);
PyObject * AMD_PyBool_FromLong(long);
PyObject * AMD_PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...);
PyObject * AMD_PyTuple_New(Py_ssize_t size);
PyAPI_FUNC(PyObject *) AMD_PyList_AsTuple(PyObject *);

/*Py_INCREF  // its define as static :: static inline void _Py_INCREF(PyObject *op)
Py_DECREF
_Py_NoneStruct  
_Py_FalseStruct
_Py_TrueStruct   //#define Py_True ((PyObject *) &_Py_TrueStruct) ==> PyAPI_DATA(struct _longobject) _Py_TrueStruct;
_Py_NotImplementedStruct
_PyObject_CallMethod_SizeT
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
