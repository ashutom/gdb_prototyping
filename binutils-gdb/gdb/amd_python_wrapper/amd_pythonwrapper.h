#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__

#include <Python.h>
#include "amd_function_pointers_list.h"


#define check_lib_handeler_execption(handle)    if(!handle){   \
                                                      AMD_lib_exception_failure_handeler();  \
                                                }  \

#define AMD_PyRun_String(str, s, g, l)  AMD_PyRun_StringFlags(str,s,g,l,NULL)
#define AMD_PyRun_InteractiveLoop(f, p) AMD_PyRun_InteractiveLoopFlags(f, p, NULL)

#define AMD_PySlice_Check(op) AMD_Py_IS_TYPE(op, AMD_PySlice_Type)

extern PyObject** AMD_PyExc_RuntimeError;
extern PyObject** AMD_PyExc_ValueError;
extern PyObject** AMD_PyExc_TypeError;
extern PyObject** AMD_PyExc_KeyError;
extern PyObject** AMD_PyExc_StopIteration;
extern PyObject** AMD_PyExc_AttributeError;
extern PyObject** AMD_PyExc_SystemError;
extern PyObject** AMD_PyExc_NotImplementedError;
extern PyObject** AMD_PyExc_IndexError;
extern PyObject** AMD_PyExc_NameError;
extern PyObject** AMD_PyExc_KeyboardInterrupt;
extern PyObject** AMD_PyExc_OverflowError;
extern PyTypeObject* AMD_PyBool_Type;
extern PyTypeObject* AMD_PySlice_Type;
extern PyTypeObject* AMD_PyEllipsis_Type;
extern PyTypeObject* AMD_PyFloat_Type;
extern int* AMD_Py_DontWriteBytecodeFlag;
extern int* AMD_Py_IgnoreEnvironmentFlag;
extern PyObject*  AMD_Py_None;
extern PyObject** AMD_Py_False;
extern PyObject** AMD_Py_True;
extern PyObject** AMD_Py_NotImplemented;


#define AMD_Py_DECREF(ob)  _AMD_Py_DECREF((PyObject*) ob);
#define AMD_Py_RETURN_FALSE return _AMD_Py_RETURN_FALSE_();
#define AMD_Py_RETURN_TRUE return _AMD_Py_RETURN_TRUE_();
#define AMD_Py_RETURN_NONE return _AMD_Py_RETURN_NONE_();
#define AMD_PyBool_Check(x) AMD_Py_IS_TYPE(x,AMD_PyBool_Type)
#define AMD_Py_BuildValue   AMD_Py_BuildValue_SizeT
#define AMD_PyFloat_Check(op) AMD_PyObject_TypeCheck(op,AMD_PyFloat_Type)


typedef char* (*AMD_PyOS_Readlinefp) (FILE *, FILE *, const char *);
void AMD_Assign_AMD_PyOS_Readlinefp(AMD_PyOS_Readlinefp FP);

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw, const char *format, 
                                       char **keywords, ...);
PyObject* AMD_PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...);
PyObject* AMD_PyObject_CallMethodObjArgs(PyObject *obj, PyObject *name, ...);
PyObject* AMD_PyObject_CallFunctionObjArgs(PyObject *callable,...);
//PyObject* AMD_Py_BuildValue(const char *, ...);
PyObject* AMD_Py_BuildValue_SizeT(const char *, ...);
int AMD_PyArg_ParseTuple(PyObject *, const char *, ...);
PyObject* AMD_PyObject_CallObject(PyObject *callable, PyObject *args);
PyObject* AMD_PyObject_GetAttr(PyObject * ob, PyObject * at);
PyObject* AMD_PyObject_GenericGetAttr(PyObject * a, PyObject * b);
int AMD_PyObject_SetAttr(PyObject* a, PyObject* b, PyObject* c);
int AMD_PyObject_HasAttr(PyObject *, PyObject *);
int AMD_PyObject_GenericSetAttr(PyObject* a, PyObject* b, PyObject* c);
int AMD_PyObject_IsTrue(PyObject *);
PyObject* AMD_PyObject_Repr(PyObject *);
PyObject* AMD_PyObject_Str(PyObject *);
Py_ssize_t AMD_PyObject_Size(PyObject *o);
int AMD_PyObject_RichCompareBool(PyObject *, PyObject *, int);

PyObject* AMD_PyUnicode_AsASCIIString(PyObject *unicode);
PyObject* AMD_PyUnicode_FromFormat(
    const char *format,   /* ASCII-encoded string  */
    ...
);
PyObject* AMD_PyUnicode_FromString(const char *ustr);
PyObject* AMD_PyUnicode_Decode(
    const char *s,              /* encoded string */
    Py_ssize_t size,            /* size of buffer */
    const char *encoding,       /* encoding */
    const char *errors          /* error handling */
    );
int AMD_PyUnicode_CompareWithASCIIString(
    PyObject *left,
    const char *right           /* ASCII-encoded string */
    );
PyObject* AMD_PyUnicode_AsEncodedString(
    PyObject *unicode,          /* Unicode object */
    const char *encoding,       /* encoding */
    const char *errors          /* error handling */
    );
int AMD_Py_IS_TYPE(PyObject *ob, PyTypeObject *type);
int AMD_PyObject_TypeCheck(PyObject *ob, PyTypeObject*);
PyObject* AMD_PyObject_GetAttrString(PyObject *, const char *);
int AMD_PyObject_SetAttrString(PyObject *, const char *, PyObject *);
int AMD_PyObject_HasAttrString(PyObject *, const char *);
int AMD_PyCallable_Check(PyObject *);
void AMD_PyErr_SetString(PyObject *exception, const char* );

PyObject* AMD_PyList_New(int size);
int AMD_PyList_Append(PyObject * ob1, PyObject * ob2);
int AMD_PyList_SetItem(PyObject *, Py_ssize_t, PyObject *);
int AMD_PyList_Insert(PyObject *, Py_ssize_t, PyObject *);
Py_ssize_t AMD_PyList_Size(PyObject *);
PyObject* AMD_PyList_AsTuple(PyObject *);
PyObject* AMD_PyList_GetItem(PyObject *, Py_ssize_t);

PyObject* AMD_PyDict_New();
PyObject* AMD_PyType_GenericNew(PyTypeObject *,PyObject *, PyObject *);
void AMD_PyErr_Clear(void);
void _Py_Dealloc(PyObject *);

int AMD_PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item);
int AMD_PyDict_SetItem(PyObject *mp, PyObject *key, PyObject *item);
PyObject* AMD_PyDict_Keys(PyObject *mp);
int AMD_PyDict_Next(PyObject *mp, Py_ssize_t *pos, PyObject **key, PyObject **value);
int AMD_PyDict_DelItemString(PyObject *dp, const char *key);
PyObject* AMD_PyDict_GetItemWithError(PyObject *mp, PyObject *key);

int AMD_PyType_Ready(PyTypeObject *);
int AMD_PyErr_ExceptionMatches(PyObject *);

PyObject* AMD_PyBool_FromLong(long);
PyObject* AMD_PyTuple_New(Py_ssize_t size);
int AMD_PyTuple_SetItem(PyObject *, Py_ssize_t, PyObject *);
Py_ssize_t AMD_PyTuple_Size(PyObject *);
PyObject* AMD_PyTuple_GetItem(PyObject *, Py_ssize_t);

PyObject* AMD_PyObject_GetIter(PyObject *);
PyObject* AMD_PyIter_Next(PyObject *);

PyObject* AMD_PyBytes_FromString(const char *);
char* AMD_PyBytes_AsString(PyObject *);
int AMD_PyBytes_AsStringAndSize(
    PyObject *obj,      /* bytes object */
    char **s,           /* pointer to buffer variable */
    Py_ssize_t *len     /* pointer to length variable or NULL */
    );
PyObject * AMD_PyBytes_FromStringAndSize(const char *, Py_ssize_t);
PyObject * AMD_PyImport_ImportModule( const char *name );
int AMD_PyObject_CheckBuffer(PyObject *obj);
int AMD_PyObject_GetBuffer(PyObject *obj, Py_buffer *view,int flags);
int AMD_PyObject_IsInstance(PyObject *object, PyObject *typeorclass);
PyObject * AMD_PyImport_GetModuleDict(void);
/*
module create is defined with the help of API / ABI version
#define PyModule_Create(module) \
        PyModule_Create2(module, PYTHON_API_VERSION)
        PyAPI_FUNC(PyObject *) PyModule_Create2(struct PyModuleDef*, int apiver);
*/
PyObject* AMD_PyModule_Create(struct PyModuleDef*);
int AMD_PyModule_AddObject(PyObject *mod, const char *, PyObject *value);
int AMD_PyModule_AddStringConstant(PyObject *, const char *, const char *);
PyObject* AMD_PyModule_GetDict(PyObject *);
int AMD_PyModule_AddIntConstant(PyObject *, const char *, long);


void AMD_PyErr_Fetch(PyObject **, PyObject **, PyObject **);
void AMD_PyErr_NormalizeException(PyObject**, PyObject**, PyObject**);
void AMD_PyErr_Restore(PyObject *, PyObject *, PyObject *);
int AMD_PyErr_GivenExceptionMatches(PyObject *, PyObject *);
PyObject* AMD_PyErr_NewException(const char *name, PyObject *base, PyObject *dict);
void AMD_PyErr_SetInterrupt(void);
void AMD_PyErr_Print(void);
PyObject * AMD_PyErr_Format(PyObject *exception,const char *format,...);

void AMD_Py_SetProgramName(const wchar_t *);
PyObject* AMD_PyErr_Occurred(void);
void AMD_PyErr_SetObject(PyObject* ob1, PyObject* ob2);
PyObject* AMD_PyObject_Call(PyObject *callable, PyObject *args, PyObject *kwargs);

/* Releases a Py_buffer obtained from getbuffer ParseTuple's "s*". */
void AMD_PyBuffer_Release(Py_buffer *view);
/* Returns the sequence 'o' as a list on success, and NULL on failure.
   This is equivalent to the Python expression: list(o) */
PyObject * AMD_PySequence_List(PyObject *o);
/* Return the first index for which o[i] == value.
   On error, return -1.
   This is equivalent to the Python expression: o.index(value). */
Py_ssize_t AMD_PySequence_Index(PyObject *o, PyObject *value);
int AMD_PySequence_Check(PyObject *o);
Py_ssize_t AMD_PySequence_Size(PyObject *o);
PyObject* AMD_PySequence_GetItem(PyObject *o, Py_ssize_t i);
int AMD_PySequence_DelItem(PyObject *o, Py_ssize_t i);
/* Return the concatenation of o1 and o2 on success, and NULL on failure.
   This is the equivalent of the Python expression: o1 + o2. */
PyObject* AMD_PySequence_Concat(PyObject *o1, PyObject *o2);


int AMD_PyRun_SimpleString(const char *);
PyGILState_STATE AMD_PyGILState_Ensure(void);
void AMD_PyGILState_Release(PyGILState_STATE);

void* AMD_PyMem_RawMalloc(size_t size);
PyObject* AMD_PyErr_NoMemory(void);
PyObject* AMD_PyMemoryView_FromObject(PyObject *base);
/* Fills in a buffer-info structure correctly for an exporter
   that can only share a contiguous chunk of memory of
   "unsigned bytes" of the given length.
   Returns 0 on success and -1 (with raising an error) on error. */
int AMD_PyBuffer_FillInfo(Py_buffer *view, PyObject *o, void *buf,
                                  Py_ssize_t len, int readonly,
                                  int flags);


long AMD_PyLong_AsLong(PyObject *ob);
long long AMD_PyLong_AsLongLong(PyObject *ob);
long AMD_PyLong_AsLongAndOverflow(PyObject *ob, int* nump);
PyObject* AMD_PyLong_FromLong(long);
PyObject* AMD_PyLong_FromLongLong(long long);
PyObject* AMD_PyLong_FromUnsignedLongLong(unsigned long long);
unsigned long long AMD_PyLong_AsUnsignedLongLong(PyObject *);

void AMD_PyErr_SetNone(PyObject *);
//int  PySlice_Unpack(PyObject *slice, Py_ssize_t *start, Py_ssize_t *stop, Py_ssize_t *step);
//Py_ssize_t PySlice_AdjustIndices(Py_ssize_t length, Py_ssize_t *start, Py_ssize_t *stop, Py_ssize_t step);
int AMD_PyArg_UnpackTuple(PyObject *, const char *, Py_ssize_t, Py_ssize_t, ...);
PyObject* AMD_PyNumber_Long(PyObject *o);

/* Return Python float from C double. */
PyObject* AMD_PyFloat_FromDouble(double);
double AMD_PyFloat_AsDouble(PyObject *);

PyObject * AMD_PySys_GetObject(const char *);
void AMD_PySys_SetPath(const wchar_t *);

int AMD_PyOS_InterruptOccurred(void);
PyObject* AMD_PyImport_AddModule(const char *name );
PyThreadState* AMD_PyEval_SaveThread(void);
void AMD_PyEval_RestoreThread(PyThreadState *);
int AMD_PyImport_ExtendInittab(struct _inittab *newtab);
PyObject* AMD_PyEval_EvalCode(PyObject *, PyObject *, PyObject *);

int AMD_PyRun_InteractiveLoopFlags(FILE *fp, const char *filename,PyCompilerFlags *flags);
PyObject* AMD_PyRun_StringFlags(const char *, int, PyObject *, PyObject *, PyCompilerFlags *);
PyObject* AMD_Py_CompileStringExFlags(const char *str, const char *filename, int start, PyCompilerFlags *flags,int optimize);
void AMD_Py_CompileStringExFlags(const wchar_t *);


int AMD_PySlice_GetIndicesEx(PyObject *r, Py_ssize_t length, Py_ssize_t *start, Py_ssize_t *stop,
                                     Py_ssize_t *step, Py_ssize_t *slicelength);

void AMD_Py_Initialize(void);
void AMD_Py_Finalize(void);

PyObject* AMD_Py_NewRef(PyObject* ob);
inline PyObject* _AMD_Py_RETURN_FALSE_() {
    return AMD_Py_NewRef(*AMD_Py_False);
}
inline PyObject* _AMD_Py_RETURN_TRUE_(){
    return AMD_Py_NewRef(*AMD_Py_True);
}
inline PyObject* _AMD_Py_RETURN_NONE_(){
    return AMD_Py_NewRef(AMD_Py_None);
}

void _AMD_Py_DECREF(PyObject* ob);
int  AMD_PyIter_Check(PyObject *);
Py_ssize_t AMD_PyLong_AsSsize_t(PyObject *);
Py_ssize_t AMD_PyObject_Length(PyObject *o);


/*Internal functions which need to be supported here because of the tempalated approach*/
void AMD_lib_exception_failure_handeler();
void* AMD_get_lib_handle();
void* check_symbol_resolution(void* functionpointer, const char*);
void __attribute__((constructor)) amd_lib_constructor();

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
