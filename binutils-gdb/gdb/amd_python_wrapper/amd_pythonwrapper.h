#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__

#include <Python.h>
#include "amd_function_pointers_list.h"

//Declarations of the externs in other translation units
extern PyTypeObject*    _AMD_PyBool_Type;
extern PyTypeObject*    _AMD_PySlice_Type;
extern PyTypeObject*    _AMD_PyEllipsis_Type;
extern PyTypeObject*    _AMD_PyFloat_Type;
extern PyObject**       _AMD_PyExc_RuntimeError;
extern PyObject**       _AMD_PyExc_ValueError;
extern PyObject**       _AMD_PyExc_TypeError;
extern PyObject**       _AMD_PyExc_KeyError;
extern PyObject**       _AMD_PyExc_StopIteration;
extern PyObject**       _AMD_PyExc_AttributeError;
extern PyObject**       _AMD_PyExc_SystemError;
extern PyObject**       _AMD_PyExc_NotImplementedError;
extern PyObject**       _AMD_PyExc_IndexError;
extern PyObject**       _AMD_PyExc_NameError;
extern PyObject**       _AMD_PyExc_KeyboardInterrupt;
extern PyObject**       _AMD_PyExc_OverflowError;
extern PyObject*        _AMD_Py_None;
extern PyObject*        AMD_Py_NotImplementedStructPtr;
extern _longobject*     AMD_Py_FalseStructPtr;
extern _longobject*     AMD_Py_TrueStructPtr;
extern va_arg_pyfunc 			  AMD_PyArg_VaParseTupleAndKeywords;
extern pyunicode_fromformat       AMD_PyUnicode_FromFormat;
extern pyobj_callmethod           AMD_PyObject_CallMethod;
extern pyobj_callmethodobjargs    AMD_PyObject_CallMethodObjArgs;
extern pyobj_callfunctionobjargs  AMD_PyObject_CallFunctionObjArgs;
extern py_buildval                AMD_Py_BuildValue_SizeT;
extern pyarg_unpacktuple          AMD_PyArg_UnpackTuple;
extern pyerr_format               AMD_PyErr_Format;
extern pyarg_parsetuple           AMD_PyArg_ParseTuple;

//Macros for Delinking GDB from Python
#define AMD_PyBool_Check(x)             Py_IS_TYPE(x,&(AMD_PyBool_Type))
#define AMD_PySlice_Check(op)           Py_IS_TYPE(op, &(AMD_PySlice_Type))
#define AMD_PyRun_String(str, s, g, l)  AMD_PyRun_StringFlags(str,s,g,l,NULL)
#define AMD_PyRun_InteractiveLoop(f, p) AMD_PyRun_InteractiveLoopFlags(f, p, NULL)
#define AMD_Py_DECREF(ob)               Py_DECREF(ob);
#define AMD_Py_RETURN_FALSE return      Py_INCREF(AMD_Py_False), AMD_Py_False
#define AMD_Py_RETURN_TRUE return       Py_INCREF(AMD_Py_True), AMD_Py_True
#define AMD_Py_RETURN_NONE return       Py_INCREF(AMD_Py_NotImplemented), AMD_Py_NotImplemented
#define Py_BuildValue_SizeT               _Py_BuildValue_SizeT_
#define AMD_PyFloat_Check(op)           PyObject_TypeCheck(op, (&(AMD_PyFloat_Type)))
#define AMD_Py_False                    ((PyObject *)   &(*AMD_Py_FalseStructPtr))
#define AMD_Py_True                     ((PyObject *)   &(*AMD_Py_TrueStructPtr))
#define AMD_Py_NotImplemented           (&(*AMD_Py_NotImplementedStructPtr))
#define AMD_PyObject_New(type, typeobj) ((type *)_AMD_PyObject_New_(typeobj))

#define AMD_PyExc_RuntimeError          (*_AMD_PyExc_RuntimeError)
#define AMD_PyExc_ValueError            (*_AMD_PyExc_ValueError)
#define AMD_PyExc_TypeError             (*_AMD_PyExc_TypeError)
#define AMD_PyExc_KeyError              (*_AMD_PyExc_KeyError)
#define AMD_PyExc_StopIteration         (*_AMD_PyExc_StopIteration)
#define AMD_PyExc_AttributeError        (*_AMD_PyExc_AttributeError)
#define AMD_PyExc_SystemError           (*_AMD_PyExc_SystemError)
#define AMD_PyExc_NotImplementedError   (*_AMD_PyExc_NotImplementedError)
#define AMD_PyExc_IndexError            (*_AMD_PyExc_IndexError)
#define AMD_PyExc_NameError             (*_AMD_PyExc_NameError)
#define AMD_PyExc_KeyboardInterrupt     (*_AMD_PyExc_KeyboardInterrupt)
#define AMD_PyExc_OverflowError         (*_AMD_PyExc_OverflowError)
#define AMD_PyBool_Type                 (*_AMD_PyBool_Type)
#define AMD_PySlice_Type                (*_AMD_PySlice_Type)
#define AMD_PyEllipsis_Type             (*_AMD_PyEllipsis_Type)
#define AMD_PyFloat_Type                (*_AMD_PyFloat_Type)


#define AMD_Py_None                     (&(*_AMD_Py_None))

//va functions
#define PyArg_VaParseTupleAndKeywords   (*AMD_PyArg_VaParseTupleAndKeywords)
#define PyObject_CallMethod             (*AMD_PyObject_CallMethod)
#define PyObject_CallMethodObjArgs      (*AMD_PyObject_CallMethodObjArgs)
#define PyObject_CallFunctionObjArgs    (*AMD_PyObject_CallFunctionObjArgs)
#define _Py_BuildValue_SizeT_           (*AMD_Py_BuildValue_SizeT)
#define PyArg_ParseTuple                (*AMD_PyArg_ParseTuple)
#define PyArg_UnpackTuple               (*AMD_PyArg_UnpackTuple)
#define PyErr_Format                    (*AMD_PyErr_Format)
#define PyUnicode_FromFormat            (*AMD_PyUnicode_FromFormat)


//From python :  extern __attribute__ ((visibility ("default"))) char* (*PyOS_ReadlineFunctionPointer)(FILE *, FILE *, const char *)
typedef char* (*AMD_PyOS_Readlinefp) (FILE *, FILE *, const char *);

void AMD_Assign_AMD_PyOS_Readlinefp(AMD_PyOS_Readlinefp FP);

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

int AMD_PyErr_GivenExceptionMatches(PyObject *, PyObject *);
PyObject* AMD_PyErr_NewException(const char *name, PyObject *base, PyObject *dict);
void AMD_PyErr_SetInterrupt(void);
void AMD_PyErr_Print(void);


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
PyObject* AMD_PyNumber_Long(PyObject *o);

/* Return Python float from C double. */
PyObject* AMD_PyFloat_FromDouble(double);
double AMD_PyFloat_AsDouble(PyObject *);

PyObject * AMD_PySys_GetObject(const char *);


int AMD_PyOS_InterruptOccurred(void);
PyObject* AMD_PyImport_AddModule(const char *name );
PyThreadState* AMD_PyEval_SaveThread(void);
void AMD_PyEval_RestoreThread(PyThreadState *);
int AMD_PyImport_ExtendInittab(struct _inittab *newtab);
PyObject* AMD_PyEval_EvalCode(PyObject *, PyObject *, PyObject *);

int AMD_PyRun_InteractiveLoopFlags(FILE *fp, const char *filename,PyCompilerFlags *flags);
PyObject* AMD_PyRun_StringFlags(const char *, int, PyObject *, PyObject *, PyCompilerFlags *);
PyObject* AMD_Py_CompileStringExFlags(const char *str, const char *filename, int start, PyCompilerFlags *flags,int optimize);

int AMD_PySlice_GetIndicesEx(PyObject *r, Py_ssize_t length, Py_ssize_t *start, Py_ssize_t *stop,
                                     Py_ssize_t *step, Py_ssize_t *slicelength);

#if PY_VERSION_HEX < 0x030c0000
void AMD_PyErr_Fetch(PyObject **, PyObject **, PyObject **);
void AMD_PyErr_Restore(PyObject *, PyObject *, PyObject *);
void AMD_PyErr_NormalizeException(PyObject**, PyObject**, PyObject**);
#else
//PyErr_GetRaisedException
//PyErr_SetRaisedException
#endif

#if PY_VERSION_HEX < 0x030a0000
extern int*             _AMD_Py_DontWriteBytecodeFlag;
extern int*             _AMD_Py_IgnoreEnvironmentFlag;
#define AMD_Py_IgnoreEnvironmentFlag    (*_AMD_Py_IgnoreEnvironmentFlag)
#define AMD_Py_DontWriteBytecodeFlag    (*_AMD_Py_DontWriteBytecodeFlag)

void AMD_Py_SetProgramName(const wchar_t *);
void AMD_Py_Initialize(void);
void AMD_Py_Finalize(void);
#else
void AMD_PySys_SetPath(const wchar_t *);
int  AMD_PyStatus_Exception(PyStatus err);
void AMD_PyConfig_InitPythonConfig(PyConfig *config);
PyStatus AMD_PyConfig_SetString(PyConfig *config, wchar_t **config_str, const wchar_t *str);
PyStatus AMD_PyConfig_Read(PyConfig *config);
PyStatus AMD_Py_InitializeFromConfig(const PyConfig *config);
void AMD_PyConfig_Clear(PyConfig *);
#endif

#if PY_VERSION_HEX < 0x03090000
  /* PyEval_InitThreads became deprecated in Python 3.9 and will
     be removed in Python 3.11.  Prior to Python 3.7, this call was
     required to initialize the GIL.  */
  PyEval_InitThreads ();
#endif


//void _AMD_Py_DECREF(PyObject* ob);
int  AMD_PyIter_Check(PyObject *);
Py_ssize_t AMD_PyLong_AsSsize_t(PyObject *);
Py_ssize_t AMD_PyObject_Length(PyObject *o);

PyObject* _AMD_PyObject_New_(PyTypeObject *);
PyObject* AMD_PyObject_Type(PyObject *o);

/*Internal functions */
void amd_lib_constructor();
//void __attribute__((constructor)) amd_lib_constructor();


#endif /* __AMD_PYTHON_WRAPPER__ */
