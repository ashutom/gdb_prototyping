#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>
#include "amd_pythonwrapper.h"
#include "amd_function_pointers_list.h"

static const int AMD_APIVER = 1013;

/*#define GET_CORRECT_FUNCTION_POINTER(fun_name_in_lib,funtype)              \
   do{                                                                     \
         char funname[]="fun_name_in_lib";                                 \
         funtype fp =  NULL;                                               \
         fp = (funtype) get_fun_pointer_from_handle((void*) fp,funname);   \
}while(0);*/

/*This shall have to be made dynamic using the configuration file later */
const char* LIBRARY_WITH_PATH = "/usr/lib64/libpython3.9.so.1.0";
/* LIB handle */
static void * PY_LIB_HANDLE=NULL;

static void AMD_close_lib_handle(){
       if(PY_LIB_HANDLE){
    		//Close the library
    		dlclose(PY_LIB_HANDLE);
		PY_LIB_HANDLE=NULL;
	}
}

void AMD_lib_exception_failure_handeler(){
        fprintf(stderr, "%s\n", dlerror());  
        AMD_close_lib_handle();  
        exit(EXIT_FAILURE);
}

void* AMD_get_lib_handle(){
	if(!PY_LIB_HANDLE){
                fprintf(stdout, "%s\n", "Ashutosh 's Implementation of Python API <<<<<<<<<<<<<<<<<<<<<<<<");  
        	PY_LIB_HANDLE = dlopen(LIBRARY_WITH_PATH, RTLD_LAZY);
		if (!PY_LIB_HANDLE) {
            AMD_lib_exception_failure_handeler();
        }
	}
	return PY_LIB_HANDLE;
}

void* check_symbol_resolution(void* functionpointer, char* str){
    functionpointer=NULL;
    functionpointer= dlsym(PY_LIB_HANDLE, str);
    if(!functionpointer){
        fprintf(stderr, "%s , %s, %s \n", dlerror(), "Fatal!!  Symbol not found for : ", str);
        AMD_close_lib_handle();  
        exit(EXIT_FAILURE);        
    }
    return functionpointer;
}

static void* get_fun_pointer_from_handle(void* funp, char* funname){
   AMD_get_lib_handle();// create handle if not created
   funp =  check_symbol_resolution(funp,funname);
   return funp;
}


int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw,
                                       const char *format, char **keywords, ...){    
    void* handle = AMD_get_lib_handle();
    check_lib_handeler_execption(handle);
    // Look up the symbol
    va_arg_pyfunc AMD_PyArg_VaParseTupleAndKeywords=NULL;
    char funname[] = "PyArg_VaParseTupleAndKeywords";
    AMD_PyArg_VaParseTupleAndKeywords=(va_arg_pyfunc) check_symbol_resolution((void*) AMD_PyArg_VaParseTupleAndKeywords, funname); 
    /*va_arg_pyfunc AMD_PyArg_VaParseTupleAndKeywords = (va_arg_pyfunc)dlsym(handle, "AMD_PyArg_VaParseTupleAndKeywords");
    if (!AMD_PyArg_VaParseTupleAndKeywords) {  
        fprintf(stderr, "%s\n", dlerror());  
        AMD_close_lib_handle();  
        exit(EXIT_FAILURE);  
    }*/

    //Use the function  
    va_list ap;

    va_start (ap, keywords);

    /*int result = (*AMD_PyArg_VaParseTupleAndKeywords)(args, kw, format, const_cast<char **> (keywords), ap);  */
    int result = (*AMD_PyArg_VaParseTupleAndKeywords)(args, kw, format, keywords, ap);  
    printf("Call status of the API :[ %s ]  is [ %d ]\n",funname, result);
  
    va_end (ap);
  
    return result;
}
PyObject * AMD_PyUnicode_FromFormat(const char *format,...){
    char funname[] = "PyUnicode_FromFormat";
    pyunicode_fromformat fp=NULL;
    fp=(pyunicode_fromformat) get_fun_pointer_from_handle((void*) fp,funname);

    va_list ap;
    va_start (ap, format);
    PyObject * result = (*fp)(format,ap);
    va_end (ap);
    return result;  
}
PyObject * AMD_PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...){
    char funname[] = "_PyObject_CallMethod_SizeT";
    pyobj_callmethod fp=NULL;
    fp=(pyobj_callmethod) get_fun_pointer_from_handle((void*) fp,funname);

    va_list ap;
    va_start (ap, format);
    PyObject * result = (*fp)(obj,name,format,ap);
    va_end (ap);
    return result;
}
PyObject* AMD_PyObject_CallMethodObjArgs(PyObject *obj, PyObject *name, ...){
    char funname[] = "PyObject_CallMethodObjArgs";
    pyobj_callmethodobjargs fp=NULL;
    fp=(pyobj_callmethodobjargs) get_fun_pointer_from_handle((void*) fp,funname);

    va_list ap;
    va_start (ap, name);
    PyObject* result = (*fp)(obj,name,ap);
    va_end (ap);
    return result;
}
PyObject*  AMD_PyObject_CallFunctionObjArgs(PyObject *callable,...){
    char funname[] = "PyObject_CallFunctionObjArgs";
    pyobj_callfunctionobjargs fp=NULL;
    fp=(pyobj_callfunctionobjargs) get_fun_pointer_from_handle((void*) fp,funname);

    va_list ap;
    va_start (ap, callable);
    PyObject* result = (*fp)(callable,ap);
    va_end (ap);
    return result;
}

inline int AMD_Py_IS_TYPE(PyObject *ob, PyTypeObject *type){
    return ob->ob_type == type;
}

int  AMD_PyObject_TypeCheck(PyObject *ob, PyTypeObject* type){
    int result = 0;
    result = AMD_Py_IS_TYPE(ob,type);
    if(0 == result){
        void* handle = AMD_get_lib_handle();
        check_lib_handeler_execption(handle);
        // Look up the symbol
        pytypeissubtype Py_pytypeissubtype=NULL;
        char funname[] = "PyType_IsSubtype";
        Py_pytypeissubtype=(pytypeissubtype) check_symbol_resolution((void*) Py_pytypeissubtype, funname); 
        //Use the function  
        result = (*Py_pytypeissubtype) (ob->ob_type,type);  
        printf("Call status of the API :[ %s ]  is [ %d ]\n",funname, result);
    }
  
    return result ;
}


void AMD_PyErr_SetString(PyObject *exception, const char* Errstr){
   char funname[]="PyErr_SetString";
   pyerror_setstring fp =  NULL;
   fp =  (pyerror_setstring) get_fun_pointer_from_handle((void*) fp,funname);
   //GET_CORRECT_FUNCTION_POINTER(PyErr_SetString,pyerror_setstring)
   (*fp) (exception, Errstr); //execute
}

PyObject* AMD_PyUnicode_FromString(const char *ustr){
   char funname[]="PyUnicode_FromString";
   py_convert_str_to_pyobj fp =  NULL;
   fp = (py_convert_str_to_pyobj) get_fun_pointer_from_handle((void*) fp,funname);
   return (PyObject*) (*fp) (ustr); //execute
}
    
long AMD_PyLong_AsLong(PyObject *ob){
   char funname[]="PyLong_AsLong";
   py_convert_pyobj_to_long fp =  NULL;
   fp = (py_convert_pyobj_to_long) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob); //execute
}
long long AMD_PyLong_AsLongLong(PyObject *ob){
   char funname[]="PyLong_AsLongLong";
   py_convert_pyobj_to_long_long fp =  NULL;
   fp = (py_convert_pyobj_to_long_long) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob); //execute

}
long AMD_PyLong_AsLongAndOverflow(PyObject *ob){
   char funname[]="PyLong_AsLongAndOverflow";
   py_convert_pyobj_to_long_overflow fp =  NULL;
   fp = (py_convert_pyobj_to_long_overflow) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob); //execute

}
PyObject* AMD_PyList_New(int size){
   char funname[]="PyList_New";
   pylist_new fp =  NULL;
   fp = (pylist_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (size); //execute   
}
PyObject* AMD_PyDict_New(){
   char funname[]="PyDict_New";
   pydict_new fp =  NULL;
   fp = (pydict_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (); //execute   
}
int AMD_PyList_Append(PyObject * ob1, PyObject * ob2){
   char funname[]="PyList_Append";
   pylist_append fp =  NULL;
   fp = (pylist_append) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob1,ob2); //execute   
}
int AMD_PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item){
   char funname[]="PyDict_SetItemString";
   pydict_setitemstring fp =  NULL;
   fp = (pydict_setitemstring) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (dp,key,item); //execute   
}
PyObject* AMD_PyType_GenericNew(PyTypeObject * o,PyObject * t, PyObject * e){
   char funname[]="PyType_GenericNew";
   pytype_genericnew fp =  NULL;
   fp = (pytype_genericnew) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (o,t,e); //execute    
}
PyObject* AMD_PyUnicode_Decode(const char *s,Py_ssize_t size,const char *encoding,const char *errors){
   char funname[]="PyUnicode_Decode";
   pyunicode_decode fp =  NULL;
   fp = (pyunicode_decode) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (s,size,encoding,errors);
}
PyObject* AMD_PyObject_GetAttrString(PyObject * ob, const char * str){
   char funname[]="PyObject_GetAttrString";
   pyobject_getattrstr fp =  NULL;
   fp = (pyobject_getattrstr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,str);
}
int  AMD_PyObject_SetAttrString(PyObject * ob, const char * str, PyObject * ob2){
   char funname[]="PyObject_SetAttrString";
   pyobj_setattstr fp =  NULL;
   fp = (pyobj_setattstr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,str,ob2);
}
int  AMD_PyObject_HasAttrString(PyObject * ob, const char * str){
   char funname[]="PyObject_HasAttrString";
   pyobj_hasattstr fp =  NULL;
   fp = (pyobj_hasattstr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,str);
}
PyObject* AMD_PyObject_GetAttr(PyObject * ob, PyObject * at){
   char funname[]="PyObject_GetAttr";
   pytobj_getattr fp =  NULL;
   fp = (pytobj_getattr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,at);    
}
int AMD_PyObject_SetAttr(PyObject* a, PyObject* b, PyObject* c){
   char funname[]="PyObject_SetAttr";
   pyobj_setattr fp =  NULL;
   fp = (pyobj_setattr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b,c);
}
int AMD_PyObject_HasAttr(PyObject* a, PyObject* b){
   char funname[]="PyObject_HasAttr";
   pylist_append fp =  NULL;
   fp = (pylist_append) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b);
}
PyObject* AMD_PyObject_GenericGetAttr(PyObject * a, PyObject * b){
   char funname[]="PyObject_GenericGetAttr";
   pytobj_getattr fp =  NULL;
   fp = (pytobj_getattr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b);
}
int AMD_PyObject_GenericSetAttr(PyObject* a, PyObject* b, PyObject* c){
   char funname[]="PyObject_GenericSetAttr";
   pyobj_setattr fp =  NULL;
   fp = (pyobj_setattr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b,c);
}
int AMD_PyObject_IsTrue(PyObject* a){
   char funname[]="PyObject_IsTrue";
   pyobj_istrue fp =  NULL;
   fp = (pyobj_istrue) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a);
}
int AMD_PyCallable_Check(PyObject* a){
   char funname[]="PyCallable_Check";
   pyobj_istrue fp =  NULL;
   fp = (pyobj_istrue) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a);
}
PyObject * AMD_PyObject_CallObject(PyObject *callable, PyObject *args){
   char funname[]="PyObject_CallObject";
   pytobj_getattr fp =  NULL;
   fp = (pytobj_getattr) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (callable,args);
}
int AMD_PyType_Ready(PyTypeObject* objtype){
   char funname[]="PyType_Ready";
   pytype_ready fp =  NULL;
   fp = (pytype_ready) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (objtype);
}
int AMD_PyModule_AddIntConstant(PyObject * ob, const char * str, long val){
   char funname[]="PyModule_AddIntConstant";
   pytmodule_addintconstant fp =  NULL;
   fp = (pytmodule_addintconstant) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,str,val);
}
void AMD_PyErr_Clear(void){
   char funname[]="PyErr_Clear";
   pyerr_clear fp =  NULL;
   fp = (pyerr_clear) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) ();   
}
int  AMD_PyErr_ExceptionMatches(PyObject * ob){
   char funname[]="PyErr_ExceptionMatches";
   pyobj_istrue fp =  NULL;
   fp = (pyobj_istrue) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);   
}
int AMD_PySequence_Check(PyObject *ob){
   char funname[]="PySequence_Check";
   pyobj_istrue fp =  NULL;
   fp = (pyobj_istrue) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);   
}
Py_ssize_t AMD_PySequence_Size(PyObject *ob){
   char funname[]="PySequence_Size";
   pysequence_size fp =  NULL;
   fp = (pysequence_size) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);    
}
PyObject * AMD_PySequence_GetItem(PyObject *ob, Py_ssize_t i){
   char funname[]="PySequence_GetItem";
   pysequence_getitem fp =  NULL;
   fp = (pysequence_getitem) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,i);   
}
int  AMD_PySequence_DelItem(PyObject *ob, Py_ssize_t i){
   char funname[]="PySequence_DelItem";
   pysequence_delitem fp =  NULL;
   fp = (pysequence_delitem) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,i);
}
PyObject * AMD_PyBool_FromLong(long ln){
   char funname[]="PyBool_FromLong";
   pybool_fromlong fp =  NULL;
   fp = (pybool_fromlong) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ln);
}
PyObject* AMD_PyTuple_New(Py_ssize_t size){
   char funname[]="PyTuple_New";
   pytuple_new fp =  NULL;
   fp = (pytuple_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (size);
}
PyObject * AMD_PyList_AsTuple(PyObject * ob){
   char funname[]="PyTuple_New";
   pylist_astuple fp =  NULL;
   fp = (pylist_astuple) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);
}
int AMD_PyTuple_SetItem(PyObject* ob, Py_ssize_t sz, PyObject * ob2){
   char funname[]="PyTuple_SetItem";
   pytuple_setitem fp =  NULL;
   fp = (pytuple_setitem) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,sz,ob2);
}
PyObject* AMD_PyObject_GetIter(PyObject * ob){
   char funname[]="PyObject_GetIter";
   pylist_astuple fp =  NULL;
   fp = (pylist_astuple) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);
}
PyObject* AMD_PyIter_Next(PyObject * ob){
   char funname[]="PyIter_Next";
   pylist_astuple fp =  NULL;
   fp = (pylist_astuple) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob);   
}
PyObject* AMD_PyUnicode_AsASCIIString(PyObject *unicode){
   char funname[]="PyUnicode_AsASCIIString";
   pylist_astuple fp =  NULL;
   fp = (pylist_astuple) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (unicode);
}
int AMD_PyBytes_AsStringAndSize(PyObject *obj, char **s, Py_ssize_t *len){
   char funname[]="PyUnicode_AsASCIIString";
   pybytes_asstringandsize fp =  NULL;
   fp = (pybytes_asstringandsize) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (obj,s,len);
}
PyObject * AMD_PyBytes_FromStringAndSize(const char *s, Py_ssize_t len){
   char funname[]="PyBytes_FromStringAndSize";
   pybytes_fromstringandsize fp =  NULL;
   fp = (pybytes_fromstringandsize) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (s,len); //execute
}
PyObject * AMD_PyImport_ImportModule(const char *name ){
   char funname[]="PyImport_ImportModule";
   py_convert_str_to_pyobj fp =  NULL;
   fp = (py_convert_str_to_pyobj) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (name); //execute
}
int AMD_PyObject_CheckBuffer(PyObject *obj){
   char funname[]="PyObject_CheckBuffer";
   pyobj_istrue fp =  NULL;
   fp = (pyobj_istrue) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (obj); //execute   
}
PyAPI_FUNC(int) AMD_PyObject_GetBuffer(PyObject *obj, Py_buffer *view,int flags){
   char funname[]="PyObject_GetBuffer";
   pyobj_getbuffer fp =  NULL;
   fp = (pyobj_getbuffer) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (obj,view,flags); //execute  
}
Py_ssize_t AMD_PyList_Size(PyObject * obj){
   char funname[]="PyList_Size";
   pylist_size fp =  NULL;
   fp = (pylist_size) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (obj); //execute
}
int AMD_PyObject_IsInstance(PyObject *object, PyObject *typeorclass){
   char funname[]="PyObject_IsInstance";
   pylist_append fp =  NULL;
   fp = (pylist_append) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (object,typeorclass); //execute   
}
/*
module create is defined with the help of API / ABI version
#define PyModule_Create(module) \
        PyModule_Create2(module, PYTHON_API_VERSION)
        PyAPI_FUNC(PyObject *) PyModule_Create2(struct PyModuleDef*, int apiver);
*/
PyObject* AMD_PyModule_Create(struct PyModuleDef* pmd){
   char funname[]="PyModule_Create2";
   pymodue_create2 fp =  NULL;
   fp = (pymodue_create2) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (pmd,AMD_APIVER); //execute 
}
PyObject * AMD_PyImport_GetModuleDict(void){
   char funname[]="PyImport_GetModuleDict";
   pydict_new fp =  NULL;
   fp = (pydict_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (); //execute   
}
void AMD_PyErr_Fetch(PyObject ** a, PyObject ** b, PyObject ** c){
   char funname[]="PyErr_Fetch";
   pyerr_fetch fp =  NULL;
   fp = (pyerr_fetch) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b,c); //execute
}
void AMD_PyErr_NormalizeException(PyObject ** a, PyObject ** b, PyObject ** c){
   char funname[]="PyErr_NormalizeException";
   pyerr_fetch fp =  NULL;
   fp = (pyerr_fetch) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b,c); //execute
}
void AMD_PyErr_Restore(PyObject * a, PyObject * b, PyObject * c){
   char funname[]="PyErr_Restore";
   pyerr_restore fp =  NULL;
   fp = (pyerr_restore) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (a,b,c); //execute
}
void AMD_PyBuffer_Release(Py_buffer *view){
   char funname[]="PyBuffer_Release";
   pybuffer_release fp =  NULL;
   fp = (pybuffer_release) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (view); //execute   
}
PyObject * AMD_PySequence_List(PyObject *o){
   char funname[]="PySequence_List";
   pylist_astuple fp =  NULL;
   fp = (pylist_astuple) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (o); //execute      
}
PyObject * AMD_PyList_GetItem(PyObject * ob, Py_ssize_t sz){
   char funname[]="PyList_GetItem";
   pysequence_getitem fp =  NULL;
   fp = (pysequence_getitem) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (ob,sz); //execute     
}
Py_ssize_t AMD_PySequence_Index(PyObject *o, PyObject *value){
   char funname[]="PySequence_Index";
   pysequence_index fp =  NULL;
   fp = (pysequence_index) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (o,value); //execute
}
void* AMD_PyMem_RawMalloc(size_t size){
   char funname[]="PyMem_RawMalloc";
   pymem_rawmalloc fp =  NULL;
   fp = (pymem_rawmalloc) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (size); //execute   
}
PyObject* AMD_PyErr_NoMemory(void){
   char funname[]="PyErr_NoMemory";
   pydict_new fp =  NULL;
   fp = (pydict_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (); //execute      
}
PyObject* AMD_PyRun_SimpleString(const char * str){
   char funname[]="PyRun_SimpleStringFlags";
   pydict_new fp =  NULL;
   fp = (pydict_new) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (); //execute      
}
PyGILState_STATE AMD_PyGILState_Ensure(void){
   char funname[]="PyGILState_Ensure";
   pygilstate_ensure fp =  NULL;
   fp = (pygilstate_ensure) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (); //execute
}
void AMD_PyGILState_Release(PyGILState_STATE state){
   char funname[]="PyGILState_Release";
   pygilstate_release fp =  NULL;
   fp = (pygilstate_release) get_fun_pointer_from_handle((void*) fp,funname);
   return (*fp) (state); //execute
}