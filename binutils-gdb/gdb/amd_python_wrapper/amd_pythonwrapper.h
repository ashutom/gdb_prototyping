#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__

#include <Python.h>
#include "amd_function_pointers_list.h"


#define check_lib_handeler_execption(handle)    if(!handle){   \
                                                      AMD_lib_exception_failure_handeler();  \
                                                }  \

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw, const char *format, 
                                       char **keywords, ...);
PyObject* AMD_PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...);
PyObject* AMD_PyObject_CallMethodObjArgs(PyObject *obj, PyObject *name, ...);
PyObject* AMD_PyObject_CallFunctionObjArgs(PyObject *callable,...);
PyObject* AMD_PyObject_CallObject(PyObject *callable, PyObject *args);

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
int AMD_PyErr_ExceptionMatches(PyObject *);
int AMD_PySequence_Check(PyObject *o);
Py_ssize_t AMD_PySequence_Size(PyObject *o);
PyObject* AMD_PySequence_GetItem(PyObject *o, Py_ssize_t i);
int AMD_PySequence_DelItem(PyObject *o, Py_ssize_t i);
PyObject* AMD_PyBool_FromLong(long);
PyObject* AMD_PyTuple_New(Py_ssize_t size);
PyObject* AMD_PyList_AsTuple(PyObject *);
int AMD_PyTuple_SetItem(PyObject *, Py_ssize_t, PyObject *);
PyObject* AMD_PyObject_GetIter(PyObject *);
PyObject* AMD_PyIter_Next(PyObject *);
PyObject* AMD_PyUnicode_AsASCIIString(PyObject *unicode);
int AMD_PyBytes_AsStringAndSize(
    PyObject *obj,      /* bytes object */
    char **s,           /* pointer to buffer variable */
    Py_ssize_t *len     /* pointer to length variable or NULL */
    );
PyObject * AMD_PyBytes_FromStringAndSize(const char *, Py_ssize_t);
PyObject * AMD_PyImport_ImportModule( const char *name );
int AMD_PyObject_CheckBuffer(PyObject *obj);
int AMD_PyObject_GetBuffer(PyObject *obj, Py_buffer *view,int flags);
Py_ssize_t AMD_PyList_Size(PyObject *);
int AMD_PyObject_IsInstance(PyObject *object, PyObject *typeorclass);
PyObject * AMD_PyImport_GetModuleDict(void);
/*
module create is defined with the help of API / ABI version
#define PyModule_Create(module) \
        PyModule_Create2(module, PYTHON_API_VERSION)
        PyAPI_FUNC(PyObject *) PyModule_Create2(struct PyModuleDef*, int apiver);
*/
PyObject* AMD_PyModule_Create(struct PyModuleDef*);
void AMD_PyErr_Fetch(PyObject **, PyObject **, PyObject **);
void AMD_PyErr_NormalizeException(PyObject**, PyObject**, PyObject**);
void AMD_PyErr_Restore(PyObject *, PyObject *, PyObject *);

/* Releases a Py_buffer obtained from getbuffer ParseTuple's "s*". */
void AMD_PyBuffer_Release(Py_buffer *view);
/* Returns the sequence 'o' as a list on success, and NULL on failure.
   This is equivalent to the Python expression: list(o) */
PyObject * AMD_PySequence_List(PyObject *o);
PyObject * AMD_PyList_GetItem(PyObject *, Py_ssize_t);
/* Return the first index for which o[i] == value.
   On error, return -1.
   This is equivalent to the Python expression: o.index(value). */
Py_ssize_t AMD_PySequence_Index(PyObject *o, PyObject *value);


int AMD_PyRun_SimpleString(const char *);
PyGILState_STATE AMD_PyGILState_Ensure(void);
void AMD_PyGILState_Release(PyGILState_STATE);

void* AMD_PyMem_RawMalloc(size_t size);
PyObject* AMD_PyErr_NoMemory(void);

long AMD_PyLong_AsLong(PyObject *ob);
long long AMD_PyLong_AsLongLong(PyObject *ob);
long AMD_PyLong_AsLongAndOverflow(PyObject *ob);


PyAPI_FUNC(void) PyErr_SetNone(PyObject *);




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
