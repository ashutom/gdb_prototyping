#ifndef _AMD_FUNCTION_POINTER_LIST_H_
#define _AMD_FUNCTION_POINTER_LIST_H_



/*List of function poitners which needs to be looked from the library*/
typedef int (*va_arg_pyfunc) (PyObject *args, PyObject *kw, const char *format, char **keywords, ...);
typedef PyObject* (*pyobj_callmethod) (PyObject *obj, const char *name, const char *format, ...);
typedef PyObject* (*pyobj_callmethodobjargs) (PyObject *obj, PyObject *name, ...);
typedef PyObject* (*pyobj_callfunctionobjargs) (PyObject *callable,...);
typedef PyObject* (*py_new_func) (PyTypeObject* objptr);
typedef int (*pytypeissubtype) (PyTypeObject *, PyTypeObject *);
typedef void (*pyerror_setstring) (PyObject *exception, const char *string  );
typedef PyObject* (*py_convert_str_to_pyobj) (const char* str);
typedef long (*py_convert_pyobj_to_long) (PyObject *);
typedef long long (*py_convert_pyobj_to_long_long) (PyObject *);
typedef long (*py_convert_pyobj_to_long_overflow) (PyObject *);
typedef PyObject* (*pylist_new) (Py_ssize_t size);
typedef PyObject* (*pydict_new) (void);
typedef PyObject* (*pytype_genericnew) (PyTypeObject *, PyObject *, PyObject *);
typedef int (*pylist_append) (PyObject *, PyObject *);
typedef int (*pydict_setitemstring) (PyObject *dp, const char *key, PyObject *item);
typedef PyObject* (*pyunicode_decode) (const char *s,Py_ssize_t size,const char *encoding,const char *errors);
typedef PyObject* (*pyunicode_fromformat) (const char *format,...);
typedef PyObject* (*pyobject_getattrstr) (PyObject *, const char *);
typedef int  (*pyobj_setattstr) (PyObject *, const char *, PyObject *);
typedef int  (*pyobj_hasattstr) (PyObject *, const char *);
typedef PyObject* (*pytobj_getattr) (PyObject *, PyObject *);
typedef int  (*pyobj_setattr) (PyObject *, PyObject *, PyObject *);
typedef int (*pyobj_istrue) (PyObject* a);
typedef int (*pytype_ready) (PyTypeObject* objtype);
typedef int (*pytmodule_addintconstant) (PyObject *, const char *, long);
typedef void (*pyerr_clear) (void);
typedef Py_ssize_t (*pysequence_size) (PyObject *ob);
typedef PyObject * (*pysequence_getitem) (PyObject *o, Py_ssize_t i);
typedef int  (*pysequence_delitem) (PyObject *o, Py_ssize_t i);
typedef PyObject* (*pybool_fromlong) (long ln);
typedef PyObject* (*pytuple_new) (Py_ssize_t size);
typedef PyObject* (*pylist_astuple) (PyObject * ob);
typedef int (*pytuple_setitem) (PyObject *, Py_ssize_t, PyObject *);
typedef int (*pybytes_asstringandsize) (PyObject *obj, char **s, Py_ssize_t *len);
typedef PyObject* (*pybytes_fromstringandsize) (const char *s, Py_ssize_t len);
typedef int (*pyobj_getbuffer) (PyObject *obj, Py_buffer *view,int flags);
typedef Py_ssize_t (*pylist_size) (PyObject *);
typedef PyObject* (*pymodue_create2) (struct PyModuleDef*,int apiver);
typedef void (*pyerr_fetch) (PyObject **, PyObject **, PyObject **);
typedef void (*pyerr_restore) (PyObject *, PyObject *, PyObject *);
typedef void (*pybuffer_release) (Py_buffer *view);
typedef Py_ssize_t (*pysequence_index) (PyObject *o, PyObject *value);
typedef void* (*pymem_rawmalloc) (size_t size);
typedef int   (*pyrun_simplestringFlags) (const char *, PyCompilerFlags *);
typedef PyGILState_STATE (*pygilstate_ensure) (void);
typedef void (*pygilstate_release) (PyGILState_STATE);











#endif //_AMD_FUNCTION_POINTER_LIST_H_