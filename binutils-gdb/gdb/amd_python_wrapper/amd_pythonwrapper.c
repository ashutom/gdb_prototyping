#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "amd_pythonwrapper.h"
#include "amd_function_pointers_list.h"

/*
 The init part of the code is clearly the critical section if operated in multiple threads
 Since init would start at lod time hence that would single thread and executed under the constructor API
 Hence not using pthread_mutex anywhere 
*/

static const int AMD_APIVER = 1013;
static bool GLOBAL_INIT=false;
static std::unordered_map<std::string,void*>* AMD_FunctionPointerTablePtr=nullptr;
/*This shall have to be made dynamic using the configuration file later */
static const char* LIBRARY_WITH_PATH = "/usr/lib64/libpython3.9.so.1.0";
/* LIB handle */
static void * PY_LIB_HANDLE=NULL;

//Data Objects from lib
PyObject** AMD_PyExc_RuntimeError=nullptr;
PyObject** AMD_PyExc_ValueError=nullptr;
PyObject** AMD_PyExc_TypeError=nullptr;
PyObject** AMD_PyExc_KeyError=nullptr;
PyObject** AMD_PyExc_StopIteration=nullptr;
PyObject** AMD_PyExc_AttributeError=nullptr;
PyObject** AMD_PyExc_SystemError=nullptr;
PyObject** AMD_PyExc_NotImplementedError=nullptr;
PyObject** AMD_PyExc_IndexError=nullptr;
PyObject** AMD_PyExc_NameError=nullptr;
PyObject** AMD_PyExc_KeyboardInterrupt=nullptr;
PyObject** AMD_PyExc_OverflowError=nullptr;
PyTypeObject* AMD_PyBool_Type=nullptr;
PyTypeObject* AMD_PySlice_Type=nullptr;
PyTypeObject* AMD_PyEllipsis_Type=nullptr;
PyTypeObject* AMD_PyFloat_Type=nullptr;
int* AMD_Py_DontWriteBytecodeFlag=nullptr;
int* AMD_Py_IgnoreEnvironmentFlag=nullptr;
PyObject*  AMD_Py_None=nullptr;

_longobject*  AMD_Py_FalseStructPtr=nullptr;
_longobject*  AMD_Py_TrueStructPtr=nullptr;
PyObject*  AMD_Py_NotImplementedStructPtr=nullptr;

// Debug functions begin :: TODO : move these to a seperate file
static bool DEBUG_START=false;
static bool SKIP_START=false;
static const char* DEBUG_VAR=  "AMD_GDB_DEBUG";
static const char* SKIP_VAR=   "AMD_GDB_DEBUG_SKIP";
static void SetEnviornment();
static void printObjectType(PyObject* obj);
#define check_set_and_print_debug_value(value_var_name,env_var_name,debug_value_for_setting)     do{     \
    const char* value_var_name = getenv(env_var_name);                                                   \
    if (value_var_name != NULL) {                                                                        \
        printf("Value of %s is: %s\n", "#env_var_name", value_var_name);                                 \
        debug_value_for_setting=true;                                                                    \
    } else {                                                                                             \
        printf("Environment variable %s is not set.\n", "#env_var_name");                                \
    }                                                                                                    \
}while(0);                                                                                               \

#define AMD_TRACE_API   do{                                                                              \
   if(DEBUG_START){                                                                                      \
      printf(" In AMD API : [%s] \n", __FUNCTION__);                                                     \
   }                                                                                                     \
}while(0);                                                                                               \

static void SetEnviornment() {
   AMD_TRACE_API
    // Use getenv to retrieve the value of the environment variable
    check_set_and_print_debug_value(AMD_GDB_DEBUG_VALUE,DEBUG_VAR,DEBUG_START);
    check_set_and_print_debug_value(AMD_GDB_DEBUG_SKIP_VALUE,SKIP_VAR,SKIP_START);
}  

static void printObjectType(PyObject* obj) {
   AMD_TRACE_API
   if(DEBUG_START){
      PyObject* objType = AMD_PyObject_Type(obj);  
      if (objType != NULL) {  
         const char* typeName = Py_TYPE(objType)->tp_name;  
         printf("Type of object: %s\n", typeName);  
         Py_DECREF(objType);  
      } else {  
         printf("Failed to get object type.\n");  
      }  

   }
}


// Debug functions end :: TODO : move these to a seperate file


static void AMD_close_lib_handle(){
   AMD_TRACE_API
   if(PY_LIB_HANDLE){
      //Close the library
      dlclose(PY_LIB_HANDLE);
      PY_LIB_HANDLE=NULL;
   }
}

static void AMD_lib_exception_failure_handeler(){
   AMD_TRACE_API
   fprintf(stderr, "%s\n", dlerror());  
   AMD_close_lib_handle();  
   exit(EXIT_FAILURE);
}

static void* AMD_get_lib_handle(){
   AMD_TRACE_API
	if(!PY_LIB_HANDLE){
                fprintf(stdout, "%s\n", "Ashutosh 's Implementation of Python API <<<<<<<<<<<<<<<<<<<<<<<<");  
        	//PY_LIB_HANDLE = dlopen(LIBRARY_WITH_PATH, RTLD_LAZY);
         //Switched to RTLD_NOW because we wanted to resolve all the symbols in the starting
         //when we are making funtion pointer table
         PY_LIB_HANDLE = dlopen(LIBRARY_WITH_PATH, RTLD_NOW); 
		if (!PY_LIB_HANDLE) {
            AMD_lib_exception_failure_handeler();
        }
	}
	return PY_LIB_HANDLE;
}

static void* AMD_check_symbol_resolution(void* functionpointer, const char* str){
   AMD_TRACE_API
   functionpointer=NULL;
   functionpointer= dlsym(PY_LIB_HANDLE, str);
   if(!functionpointer){
      fprintf(stderr, "%s , %s, %s \n", dlerror(), "Fatal!!  Symbol not found for : ", str);
      AMD_close_lib_handle();  
      exit(EXIT_FAILURE);        
   }
   return functionpointer;
}

static void* get_fun_pointer_from_handle(void* funp, const char* funname){
   AMD_TRACE_API
   AMD_get_lib_handle();// create handle if not created
   funp =  AMD_check_symbol_resolution(funp,funname);
   return funp;
}

static inline void CheckNASSIGN_FP_To_Table_From_LIB_Using_Handle(void* f,const char* str){
   AMD_TRACE_API
   if(AMD_FunctionPointerTablePtr->count(std::string(str))>0){
      fprintf(stderr, "%s, %s\n", "Alaready present in table hence overriding pointer for", str);
   }
   (*AMD_FunctionPointerTablePtr).insert(std::make_pair(str,AMD_check_symbol_resolution(f,str)));   
}


static void* get_fun_pointer_from_table(const char* funname){
   AMD_TRACE_API
   void* funp=nullptr;
   if(AMD_FunctionPointerTablePtr && AMD_FunctionPointerTablePtr->count(funname)<=0){
      AMD_FunctionPointerTablePtr->insert(std::make_pair(funname,get_fun_pointer_from_handle(funp,funname)));
   }
   return (*AMD_FunctionPointerTablePtr)[funname];
}

static void Initialize_Fun_Pointer_Table(){
   AMD_TRACE_API
   if(nullptr==AMD_FunctionPointerTablePtr){
      AMD_FunctionPointerTablePtr = new std::unordered_map<std::string,void*>;
   }

   void* placeholder=nullptr;
   std::vector<std::string> functionnames = {
      "PyExc_RuntimeError", "PyArg_VaParseTupleAndKeywords", "PyUnicode_FromFormat", "_PyObject_CallMethod_SizeT",
      "PyObject_CallMethodObjArgs", "PyObject_CallFunctionObjArgs", "_Py_BuildValue_SizeT", "PyArg_UnpackTuple",
      "PyErr_Format", "_PyArg_ParseTuple_SizeT", "PyType_IsSubtype", "PyErr_SetString", "PyUnicode_FromString",
      "PyLong_AsLong", "PyLong_AsLongLong", "PyLong_AsLongAndOverflow", "PyList_New", "PyList_SetItem",
      "PyList_Insert", "PyList_Append", "PyDict_New", "PyDict_SetItemString", "PyType_GenericNew",
      "PyUnicode_Decode", "PyObject_GetAttrString",  "PyObject_SetAttrString",  "PyObject_HasAttrString",
      "PyObject_GetAttr",  "PyObject_SetAttr",  "PyObject_HasAttr",  "PyObject_GenericGetAttr",  "PyObject_GenericSetAttr",
      "PyObject_IsTrue",  "PyCallable_Check",  "PyObject_CallObject",  "PyType_Ready",  "PyModule_AddIntConstant",
      "PyErr_Clear",  "PyErr_ExceptionMatches",  "PySequence_Check",  "PySequence_Size",  "PySequence_GetItem",
      "PySequence_DelItem",  "PyBool_FromLong",  "PyTuple_New",  "PyList_AsTuple",  "PyTuple_SetItem",  "PyObject_GetIter",
      "PyIter_Next",  "PyUnicode_AsASCIIString",  "PyBytes_AsStringAndSize",  "PyBytes_FromStringAndSize",
      "PyImport_ImportModule",  "PyObject_CheckBuffer",  "PyObject_GetBuffer",  "PyList_Size",  "PyObject_IsInstance",
      "PyModule_Create2",  "PyImport_GetModuleDict",  "PyErr_Fetch",  "PyErr_NormalizeException",  "PyErr_Restore",
      "PyBuffer_Release",  "PySequence_List",  "PyList_GetItem",  "PySequence_Index",  "PyMem_RawMalloc",
      "PyErr_NoMemory",  "PyRun_SimpleStringFlags",  "PyGILState_Ensure",  "PyGILState_Release",  "PyDict_SetItem",
      "PyDict_Keys",  "PyDict_Next",  "PyDict_DelItemString",  "PyDict_GetItemWithError",  "PyMemoryView_FromObject",
      "PyBuffer_FillInfo",  "PyTuple_Size",  "PyTuple_GetItem",  "PyObject_Repr",  "PyObject_Str",  "PyObject_Size",
      "PyLong_AsSsize_t",  "PyObject_Length",  "PyObject_RichCompareBool",  "PyLong_FromLong",  "PyLong_FromLongLong",
      "PyLong_FromUnsignedLongLong",  "PyLong_AsUnsignedLongLong",  "PyUnicode_CompareWithASCIIString",
      "PyUnicode_AsEncodedString",  "PyBytes_FromString",  "PyBytes_AsString",  "PyErr_SetNone",  "PyNumber_Long",
      "PyModule_AddObject",  "PyModule_AddStringConstant",  "PyModule_GetDict",  "PyErr_GivenExceptionMatches",
      "PyErr_NewException",  "PyErr_SetInterrupt",  "PyErr_Print",  "PyFloat_FromDouble",  "PyFloat_AsDouble",
      "PySequence_Concat",  "PySys_GetObject",  "PySys_SetPath",  "PyOS_InterruptOccurred",  "PyImport_AddModule",
      "PyImport_ExtendInittab",  "PyEval_EvalCode",  "PyEval_SaveThread",  "PyEval_RestoreThread",
      "PyRun_InteractiveLoopFlags",  "PyRun_StringFlags", "Py_CompileStringExFlags", "Py_Initialize",  "Py_Finalize",
      "PyErr_Occurred",  "PyErr_SetObject",  "PyObject_Call",  "Py_SetProgramName", "PySlice_GetIndicesEx",
      "Py_DecRef",  "PyIter_Check",  "_Py_Dealloc", "_PyObject_New", "PyObject_Type"
   };
   //lets assing
   for(int i=0;i<functionnames.size();i++){
      CheckNASSIGN_FP_To_Table_From_LIB_Using_Handle(placeholder,functionnames[i].c_str());
   }   
}


static void Initialize_AMD_PyAPI_DATA(){
   AMD_TRACE_API
   void* placeholder=nullptr;

   AMD_PyExc_RuntimeError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_RuntimeError");
   AMD_PyExc_ValueError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_ValueError");
   AMD_PyExc_TypeError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_TypeError");
   AMD_PyExc_KeyError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_KeyError");
   AMD_PyExc_StopIteration = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_StopIteration");
   AMD_PyExc_AttributeError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_AttributeError");
   AMD_PyExc_SystemError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_SystemError");
   AMD_PyExc_NotImplementedError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_NotImplementedError");
   AMD_PyExc_IndexError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_IndexError");
   AMD_PyExc_NameError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_NameError");
   AMD_PyExc_KeyboardInterrupt = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_KeyboardInterrupt");
   AMD_PyExc_OverflowError = (PyObject**)AMD_check_symbol_resolution(placeholder,"PyExc_OverflowError");

   AMD_Py_TrueStructPtr = (_longobject*)AMD_check_symbol_resolution(placeholder,"_Py_TrueStruct");
   AMD_Py_FalseStructPtr = (_longobject*)AMD_check_symbol_resolution(placeholder,"_Py_FalseStruct");
   AMD_Py_NotImplementedStructPtr = (PyObject*)AMD_check_symbol_resolution(placeholder,"_Py_NotImplementedStruct");

   AMD_Py_None = (PyObject*)AMD_check_symbol_resolution(placeholder,"_Py_NoneStruct");

   AMD_PyBool_Type = (PyTypeObject*)AMD_check_symbol_resolution(placeholder,"PyBool_Type");
   AMD_PySlice_Type = (PyTypeObject*)AMD_check_symbol_resolution(placeholder,"PySlice_Type");
   AMD_PyEllipsis_Type = (PyTypeObject*)AMD_check_symbol_resolution(placeholder,"PyEllipsis_Type");
   AMD_PyFloat_Type = (PyTypeObject*)AMD_check_symbol_resolution(placeholder,"PyFloat_Type");

   AMD_Py_DontWriteBytecodeFlag = (int*)AMD_check_symbol_resolution(placeholder,"Py_DontWriteBytecodeFlag");
   AMD_Py_IgnoreEnvironmentFlag = (int*)AMD_check_symbol_resolution(placeholder,"Py_IgnoreEnvironmentFlag");
}

void amd_lib_constructor() {
   AMD_TRACE_API
   SetEnviornment();
   printf("AMD Constructor called\n");
   AMD_get_lib_handle();
   Initialize_AMD_PyAPI_DATA();
   Initialize_Fun_Pointer_Table();

   if(false == SKIP_START){
      printObjectType(*AMD_PyExc_RuntimeError);
   }else{
      printObjectType((PyObject*)AMD_PyExc_RuntimeError);
   }
   fprintf(stdout, "%s\n", " Trying to print : AMD_PyExc_RuntimeError <<<<<<<<<<<<<<<<<<<<<<<<");
   PyObject* AMD_PyExc_RuntimeError_forprinting = *AMD_PyExc_RuntimeError;
   PyTypeObject* type_for_printing=AMD_PyExc_RuntimeError_forprinting->ob_type;
   printf("Trying to print : type_for_printing->tp_name  : is [ %s ] <<<<<<<<<<<<<<< \n", type_for_printing->tp_name);

   GLOBAL_INIT=true;
}  

void AMD_Assign_AMD_PyOS_Readlinefp(AMD_PyOS_Readlinefp FP){
   AMD_TRACE_API
   AMD_get_lib_handle();
   AMD_PyOS_Readlinefp* fp=nullptr;
   char symbolname[]="PyOS_ReadlineFunctionPointer";
   fp = (AMD_PyOS_Readlinefp*) dlsym(PY_LIB_HANDLE, symbolname);
   if(!fp){
        fprintf(stderr, "%s , %s, %s \n", dlerror(), "Fatal!!  Symbol not found for : ", symbolname);
        AMD_close_lib_handle();  
        exit(EXIT_FAILURE);        
   }
   *fp=FP;
}

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw,
                                       const char *format, char **keywords, ...){
   AMD_TRACE_API
   va_arg_pyfunc fp=(va_arg_pyfunc) get_fun_pointer_from_table("PyArg_VaParseTupleAndKeywords");
   //Use the function  
   va_list ap;
   va_start (ap, keywords);
   int result = (*fp)(args, kw, format, keywords, ap);  
   printf("Call status of the API :[ %s ]  is [ %d ]\n",__FUNCTION__, result);
   va_end (ap);
   return result;
}
PyObject * AMD_PyUnicode_FromFormat(const char *format,...){
   AMD_TRACE_API
   pyunicode_fromformat fp=(pyunicode_fromformat) get_fun_pointer_from_table("PyUnicode_FromFormat");
   va_list ap;
   va_start (ap, format);
   PyObject * result = (*fp)(format,ap);
   va_end (ap);
   return result;  
}
PyObject * AMD_PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...){
   AMD_TRACE_API
   pyobj_callmethod fp=(pyobj_callmethod) get_fun_pointer_from_table("_PyObject_CallMethod_SizeT");

   va_list ap;
   va_start (ap, format);
   PyObject * result = (*fp)(obj,name,format,ap);
   va_end (ap);
   return result;
}
PyObject* AMD_PyObject_CallMethodObjArgs(PyObject *obj, PyObject *name, ...){
   AMD_TRACE_API
   pyobj_callmethodobjargs fp=(pyobj_callmethodobjargs) get_fun_pointer_from_table("PyObject_CallMethodObjArgs");
   va_list ap;
   va_start (ap, name);
   PyObject* result = (*fp)(obj,name,ap);
   va_end (ap);
   return result;
}
PyObject*  AMD_PyObject_CallFunctionObjArgs(PyObject *callable,...){
   AMD_TRACE_API
   pyobj_callfunctionobjargs fp=(pyobj_callfunctionobjargs) get_fun_pointer_from_table("PyObject_CallFunctionObjArgs");

   va_list ap;
   va_start (ap, callable);
   PyObject* result = (*fp)(callable,ap);
   va_end (ap);
   return result;
}
PyObject* AMD_Py_BuildValue_SizeT(const char * str, ...){
   AMD_TRACE_API
   py_buildval fp =(py_buildval) get_fun_pointer_from_table("_Py_BuildValue_SizeT");
   va_list ap;
   va_start (ap, str);
   PyObject* result = (*fp)(str,ap);
   va_end (ap);
   return result;
}
int AMD_PyArg_UnpackTuple(PyObject * ob, const char * action, Py_ssize_t min, Py_ssize_t max, ...){
   AMD_TRACE_API
   pyarg_unpacktuple fp=(pyarg_unpacktuple) get_fun_pointer_from_table("PyArg_UnpackTuple");
   va_list ap;
   va_start (ap, max);
   int result = (*fp)(ob,action,min,max,ap);
   va_end (ap);
   return result;   
}
PyObject* AMD_PyErr_Format(PyObject *exception,const char *format,...){
   AMD_TRACE_API
   pyerr_format fp=(pyerr_format) get_fun_pointer_from_table("PyErr_Format");
   va_list ap;
   va_start (ap, format);
   PyObject* result = (*fp)(exception,format,ap);
   va_end (ap);
   return result; 
}

int AMD_PyArg_ParseTuple(PyObject * obj, const char * cstr, ...){
   AMD_TRACE_API
   pyarg_parsetuple fp = (pyarg_parsetuple) get_fun_pointer_from_table("PyArg_ParseTuple");    
   va_list ap;
   va_start (ap, cstr);
   int result = (*fp)(obj,cstr,ap);
   va_end (ap);
   return result;
}

inline int AMD_Py_IS_TYPE(PyObject *ob, PyTypeObject *type){
   AMD_TRACE_API
   return ob->ob_type == type;
}

int  AMD_PyObject_TypeCheck(PyObject *ob, PyTypeObject* type){
   AMD_TRACE_API
   int result = 0;
   result = AMD_Py_IS_TYPE(ob,type);
   if(0 == result){
      // Look up the symbol
      pytypeissubtype Py_pytypeissubtype=(pytypeissubtype)get_fun_pointer_from_table("PyType_IsSubtype");
      //Use the function  
      result = (*Py_pytypeissubtype) (ob->ob_type,type);  
      printf("Call status of the API :[ %s ]  is [ %d ]\n",__FUNCTION__, result);
   }
  
   return result ;
}


void AMD_PyErr_SetString(PyObject *exception, const char* Errstr){
   AMD_TRACE_API
   pyerror_setstring fp = (pyerror_setstring) get_fun_pointer_from_table("PyErr_SetString");
   //GET_CORRECT_FUNCTION_POINTER(PyErr_SetString,pyerror_setstring)
   (*fp) (exception, Errstr); //execute
}

PyObject* AMD_PyUnicode_FromString(const char *ustr){
   AMD_TRACE_API
   py_convert_str_to_pyobj fp =(py_convert_str_to_pyobj) get_fun_pointer_from_table("PyUnicode_FromString");
   return (PyObject*) (*fp) (ustr); //execute
}
    
long AMD_PyLong_AsLong(PyObject *ob){
   AMD_TRACE_API
   py_convert_pyobj_to_long fp =(py_convert_pyobj_to_long) get_fun_pointer_from_table("PyLong_AsLong");
   return (*fp) (ob); //execute
}
long long AMD_PyLong_AsLongLong(PyObject *ob){
   AMD_TRACE_API
   py_convert_pyobj_to_long_long fp =(py_convert_pyobj_to_long_long) get_fun_pointer_from_table("PyLong_AsLongLong");
   return (*fp) (ob); //execute
}
long AMD_PyLong_AsLongAndOverflow(PyObject *ob, int* nump){
   AMD_TRACE_API
   py_long_as_long_and_overflow fp =(py_long_as_long_and_overflow) get_fun_pointer_from_table("PyLong_AsLongAndOverflow");
   return (*fp) (ob,nump); //execute
}
PyObject* AMD_PyList_New(int size){
   AMD_TRACE_API
   pylist_new fp =(pylist_new) get_fun_pointer_from_table("PyList_New");
   return (*fp) (size); //execute   
}
int AMD_PyList_SetItem(PyObject * ob, Py_ssize_t size, PyObject * ob2){
   AMD_TRACE_API
   pytuple_setitem fp =(pytuple_setitem) get_fun_pointer_from_table("PyList_SetItem");
   return (*fp) (ob,size,ob2); //execute
}
int AMD_PyList_Insert(PyObject * ob, Py_ssize_t size, PyObject * ob2){
   AMD_TRACE_API
   pytuple_setitem fp =(pytuple_setitem) get_fun_pointer_from_table("PyList_Insert");
   return (*fp) (ob,size,ob2); //execute
}
int AMD_PyList_Append(PyObject * ob1, PyObject * ob2){
   AMD_TRACE_API
   pylist_append fp =(pylist_append) get_fun_pointer_from_table("PyList_Append");
   return (*fp) (ob1,ob2); //execute   
}
PyObject* AMD_PyDict_New(){
   AMD_TRACE_API
   pydict_new fp =(pydict_new) get_fun_pointer_from_table("PyDict_New");
   return (*fp) (); //execute   
}
int AMD_PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item){
   AMD_TRACE_API
   pydict_setitemstring fp =(pydict_setitemstring) get_fun_pointer_from_table("PyDict_SetItemString");
   return (*fp) (dp,key,item); //execute   
}
PyObject* AMD_PyType_GenericNew(PyTypeObject * o,PyObject * t, PyObject * e){
   AMD_TRACE_API
   pytype_genericnew fp =(pytype_genericnew) get_fun_pointer_from_table("PyType_GenericNew");
   return (*fp) (o,t,e); //execute    
}
PyObject* AMD_PyUnicode_Decode(const char *s,Py_ssize_t size,const char *encoding,const char *errors){
   AMD_TRACE_API
   pyunicode_decode fp =(pyunicode_decode) get_fun_pointer_from_table("PyUnicode_Decode");
   return (*fp) (s,size,encoding,errors);
}
PyObject* AMD_PyObject_GetAttrString(PyObject * ob, const char * str){
   AMD_TRACE_API
   pyobject_getattrstr fp =(pyobject_getattrstr) get_fun_pointer_from_table("PyObject_GetAttrString");
   return (*fp) (ob,str);
}
int  AMD_PyObject_SetAttrString(PyObject * ob, const char * str, PyObject * ob2){
   AMD_TRACE_API
   pyobj_setattstr fp =(pyobj_setattstr) get_fun_pointer_from_table("PyObject_SetAttrString");
   return (*fp) (ob,str,ob2);
}
int  AMD_PyObject_HasAttrString(PyObject * ob, const char * str){
   AMD_TRACE_API
   pyobj_hasattstr fp =(pyobj_hasattstr) get_fun_pointer_from_table("PyObject_HasAttrString");
   return (*fp) (ob,str);
}
PyObject* AMD_PyObject_GetAttr(PyObject * ob, PyObject * at){
   AMD_TRACE_API
   pytobj_getattr fp =(pytobj_getattr) get_fun_pointer_from_table("PyObject_GetAttr");
   return (*fp) (ob,at);    
}
int AMD_PyObject_SetAttr(PyObject* a, PyObject* b, PyObject* c){
   AMD_TRACE_API
   pyobj_setattr fp =(pyobj_setattr) get_fun_pointer_from_table("PyObject_SetAttr");
   return (*fp) (a,b,c);
}
int AMD_PyObject_HasAttr(PyObject* a, PyObject* b){
   AMD_TRACE_API
   pylist_append fp =(pylist_append) get_fun_pointer_from_table("PyObject_HasAttr");
   return (*fp) (a,b);
}
PyObject* AMD_PyObject_GenericGetAttr(PyObject * a, PyObject * b){
   AMD_TRACE_API
   pytobj_getattr fp =(pytobj_getattr) get_fun_pointer_from_table("PyObject_GenericGetAttr");
   return (*fp) (a,b);
}
int AMD_PyObject_GenericSetAttr(PyObject* a, PyObject* b, PyObject* c){
   AMD_TRACE_API
   pyobj_setattr fp =(pyobj_setattr) get_fun_pointer_from_table("PyObject_GenericSetAttr");
   return (*fp) (a,b,c);
}
int AMD_PyObject_IsTrue(PyObject* a){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PyObject_IsTrue");
   return (*fp) (a);
}
int AMD_PyCallable_Check(PyObject* a){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PyCallable_Check");
   return (*fp) (a);
}
PyObject * AMD_PyObject_CallObject(PyObject *callable, PyObject *args){
   AMD_TRACE_API
   pytobj_getattr fp =(pytobj_getattr) get_fun_pointer_from_table("PyObject_CallObject");
   return (*fp) (callable,args);
}
int AMD_PyType_Ready(PyTypeObject* objtype){
   AMD_TRACE_API
   pytype_ready fp =(pytype_ready) get_fun_pointer_from_table("PyType_Ready");
   return (*fp) (objtype);
}
int AMD_PyModule_AddIntConstant(PyObject * ob, const char * str, long val){
   AMD_TRACE_API
   pytmodule_addintconstant fp =(pytmodule_addintconstant) get_fun_pointer_from_table("PyModule_AddIntConstant");
   return (*fp) (ob,str,val);
}
void AMD_PyErr_Clear(void){
   AMD_TRACE_API
   pyerr_clear fp =(pyerr_clear) get_fun_pointer_from_table("PyErr_Clear");
   return (*fp) ();   
}
int  AMD_PyErr_ExceptionMatches(PyObject * ob){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PyErr_ExceptionMatches");
   return (*fp) (ob);   
}
int AMD_PySequence_Check(PyObject *ob){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PySequence_Check");
   return (*fp) (ob);   
}
Py_ssize_t AMD_PySequence_Size(PyObject *ob){
   AMD_TRACE_API
   pysequence_size fp =(pysequence_size) get_fun_pointer_from_table("PySequence_Size");
   return (*fp) (ob);    
}
PyObject * AMD_PySequence_GetItem(PyObject *ob, Py_ssize_t i){
   AMD_TRACE_API
   pysequence_getitem fp =(pysequence_getitem) get_fun_pointer_from_table("PySequence_GetItem");
   return (*fp) (ob,i);   
}
int  AMD_PySequence_DelItem(PyObject *ob, Py_ssize_t i){
   AMD_TRACE_API
   pysequence_delitem fp =(pysequence_delitem) get_fun_pointer_from_table("PySequence_DelItem");
   return (*fp) (ob,i);
}
PyObject * AMD_PyBool_FromLong(long ln){
   AMD_TRACE_API
   pybool_fromlong fp =(pybool_fromlong) get_fun_pointer_from_table("PyBool_FromLong");
   return (*fp) (ln);
}
PyObject* AMD_PyTuple_New(Py_ssize_t size){
   AMD_TRACE_API
   pytuple_new fp =(pytuple_new) get_fun_pointer_from_table("PyTuple_New");
   return (*fp) (size);
}
PyObject * AMD_PyList_AsTuple(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyList_AsTuple");
   return (*fp) (ob);
}
int AMD_PyTuple_SetItem(PyObject* ob, Py_ssize_t sz, PyObject * ob2){
   AMD_TRACE_API
   pytuple_setitem fp =(pytuple_setitem) get_fun_pointer_from_table("PyTuple_SetItem");
   return (*fp) (ob,sz,ob2);
}
PyObject* AMD_PyObject_GetIter(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyObject_GetIter");
   return (*fp) (ob);
}
PyObject* AMD_PyIter_Next(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyIter_Next");
   return (*fp) (ob);   
}
PyObject* AMD_PyUnicode_AsASCIIString(PyObject *unicode){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyUnicode_AsASCIIString");
   return (*fp) (unicode);
}
int AMD_PyBytes_AsStringAndSize(PyObject *obj, char **s, Py_ssize_t *len){
   AMD_TRACE_API
   pybytes_asstringandsize fp =(pybytes_asstringandsize) get_fun_pointer_from_table("PyBytes_AsStringAndSize");
   return (*fp) (obj,s,len);
}
PyObject * AMD_PyBytes_FromStringAndSize(const char *s, Py_ssize_t len){
   AMD_TRACE_API
   pybytes_fromstringandsize fp =(pybytes_fromstringandsize) get_fun_pointer_from_table("PyBytes_FromStringAndSize");
   return (*fp) (s,len); //execute
}
PyObject * AMD_PyImport_ImportModule(const char *name ){
   AMD_TRACE_API
   py_convert_str_to_pyobj fp =(py_convert_str_to_pyobj) get_fun_pointer_from_table("PyImport_ImportModule");
   return (*fp) (name); //execute
}
int AMD_PyObject_CheckBuffer(PyObject *obj){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PyObject_CheckBuffer");
   return (*fp) (obj); //execute   
}
PyAPI_FUNC(int) AMD_PyObject_GetBuffer(PyObject *obj, Py_buffer *view,int flags){
   AMD_TRACE_API
   pyobj_getbuffer fp =(pyobj_getbuffer) get_fun_pointer_from_table("PyObject_GetBuffer");
   return (*fp) (obj,view,flags); //execute  
}
Py_ssize_t AMD_PyList_Size(PyObject * obj){
   AMD_TRACE_API
   pylist_size fp =(pylist_size) get_fun_pointer_from_table("PyList_Size");
   return (*fp) (obj); //execute
}
int AMD_PyObject_IsInstance(PyObject *object, PyObject *typeorclass){
   AMD_TRACE_API
   pylist_append fp =(pylist_append) get_fun_pointer_from_table("PyObject_IsInstance");
   return (*fp) (object,typeorclass); //execute   
}
/*
module create is defined with the help of API / ABI version
#define PyModule_Create(module) \
        PyModule_Create2(module, PYTHON_API_VERSION)
        PyAPI_FUNC(PyObject *) PyModule_Create2(struct PyModuleDef*, int apiver);
*/
PyObject* AMD_PyModule_Create(struct PyModuleDef* pmd){
   AMD_TRACE_API
   pymodue_create2 fp =(pymodue_create2) get_fun_pointer_from_table("PyModule_Create2");
   return (*fp) (pmd,AMD_APIVER); //execute 
}
PyObject * AMD_PyImport_GetModuleDict(void){
   AMD_TRACE_API
   pydict_new fp =(pydict_new) get_fun_pointer_from_table("PyImport_GetModuleDict");
   return (*fp) (); //execute   
}
void AMD_PyErr_Fetch(PyObject ** a, PyObject ** b, PyObject ** c){
   AMD_TRACE_API
   pyerr_fetch fp =(pyerr_fetch) get_fun_pointer_from_table("PyErr_Fetch");
   return (*fp) (a,b,c); //execute
}
void AMD_PyErr_NormalizeException(PyObject ** a, PyObject ** b, PyObject ** c){
   AMD_TRACE_API
   pyerr_fetch fp =(pyerr_fetch) get_fun_pointer_from_table("PyErr_NormalizeException");
   return (*fp) (a,b,c); //execute
}
void AMD_PyErr_Restore(PyObject * a, PyObject * b, PyObject * c){
   AMD_TRACE_API
   pyerr_restore fp =(pyerr_restore) get_fun_pointer_from_table("PyErr_Restore");
   return (*fp) (a,b,c); //execute
}
void AMD_PyBuffer_Release(Py_buffer *view){
   AMD_TRACE_API
   pybuffer_release fp =(pybuffer_release) get_fun_pointer_from_table("PyBuffer_Release");
   return (*fp) (view); //execute   
}
PyObject * AMD_PySequence_List(PyObject *o){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PySequence_List");
   return (*fp) (o); //execute      
}
PyObject * AMD_PyList_GetItem(PyObject * ob, Py_ssize_t sz){
   AMD_TRACE_API
   pysequence_getitem fp =(pysequence_getitem) get_fun_pointer_from_table("PyList_GetItem");
   return (*fp) (ob,sz); //execute     
}
Py_ssize_t AMD_PySequence_Index(PyObject *o, PyObject *value){
   AMD_TRACE_API
   pysequence_index fp =(pysequence_index) get_fun_pointer_from_table("PySequence_Index");
   return (*fp) (o,value); //execute
}
void* AMD_PyMem_RawMalloc(size_t size){
   AMD_TRACE_API
   pymem_rawmalloc fp =(pymem_rawmalloc) get_fun_pointer_from_table("PyMem_RawMalloc");
   return (*fp) (size); //execute   
}
PyObject* AMD_PyErr_NoMemory(void){
   AMD_TRACE_API
   pydict_new fp =(pydict_new) get_fun_pointer_from_table("PyErr_NoMemory");
   return (*fp) (); //execute      
}
int AMD_PyRun_SimpleString(const char * str){
   AMD_TRACE_API
   pyrun_simplestringFlags fp =(pyrun_simplestringFlags) get_fun_pointer_from_table("PyRun_SimpleStringFlags");
   return (*fp) (str,NULL); //execute      
}
PyGILState_STATE AMD_PyGILState_Ensure(void){
   AMD_TRACE_API
   pygilstate_ensure fp =(pygilstate_ensure) get_fun_pointer_from_table("PyGILState_Ensure");
   return (*fp) (); //execute
}
void AMD_PyGILState_Release(PyGILState_STATE state){
   AMD_TRACE_API
   pygilstate_release fp =(pygilstate_release) get_fun_pointer_from_table("PyGILState_Release");
   return (*fp) (state); //execute
}
int AMD_PyDict_SetItem(PyObject *mp, PyObject *key, PyObject *item){
   AMD_TRACE_API
   pyobj_setattr fp =(pyobj_setattr) get_fun_pointer_from_table("PyDict_SetItem");
   return (*fp) (mp,key,item); //execute
}
PyObject* AMD_PyDict_Keys(PyObject *mp){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyDict_Keys");
   return (*fp) (mp); //execute   
}
int AMD_PyDict_Next(PyObject *mp, Py_ssize_t *pos, PyObject **key, PyObject **value){
   AMD_TRACE_API
   pydict_next fp =(pydict_next) get_fun_pointer_from_table("PyDict_Next");
   return (*fp) (mp,pos,key,value); //execute
}
int AMD_PyDict_DelItemString(PyObject *dp, const char *key){
   AMD_TRACE_API
   pyobj_hasattstr fp =(pyobj_hasattstr) get_fun_pointer_from_table("PyDict_DelItemString");
   return (*fp) (dp,key); //execute
}
PyObject* AMD_PyDict_GetItemWithError(PyObject *mp, PyObject *key){
   AMD_TRACE_API
   pytobj_getattr fp =(pytobj_getattr) get_fun_pointer_from_table("PyDict_GetItemWithError");
   return (*fp) (mp,key); //execute
}
PyObject* AMD_PyMemoryView_FromObject(PyObject *base){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyMemoryView_FromObject");
   return (*fp) (base); //execute
}
int AMD_PyBuffer_FillInfo(Py_buffer *view, PyObject *o, void *buf, Py_ssize_t len, int readonly, int flags){
   AMD_TRACE_API
   pybuffer_fillinfo fp =(pybuffer_fillinfo) get_fun_pointer_from_table("PyBuffer_FillInfo");
   return (*fp) (view,o,buf,len,readonly,flags); //execute
}
Py_ssize_t AMD_PyTuple_Size(PyObject * ob){
   AMD_TRACE_API
   pysequence_size fp =(pysequence_size) get_fun_pointer_from_table("PyTuple_Size");
   return (*fp) (ob); //execute
}
PyObject* AMD_PyTuple_GetItem(PyObject * ob, Py_ssize_t i){
   AMD_TRACE_API
   pysequence_getitem fp =(pysequence_getitem) get_fun_pointer_from_table("PyTuple_GetItem");
   return (*fp) (ob,i); //execute
}
PyObject* AMD_PyObject_Repr(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyObject_Repr");
   return (*fp) (ob); //execute   
}
PyObject* AMD_PyObject_Str(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyObject_Str");
   return (*fp) (ob); //execute
}
Py_ssize_t AMD_PyObject_Size(PyObject *ob){
   AMD_TRACE_API
   pysequence_size fp =(pysequence_size) get_fun_pointer_from_table("PyObject_Size");
   return (*fp) (ob); //execute
}
Py_ssize_t AMD_PyLong_AsSsize_t(PyObject* ob){
   AMD_TRACE_API
   pysequence_size fp =(pysequence_size) get_fun_pointer_from_table("PyLong_AsSsize_t");
   return (*fp) (ob); //execute       
}
Py_ssize_t AMD_PyObject_Length(PyObject *ob){
   AMD_TRACE_API
   pysequence_size fp =(pysequence_size) get_fun_pointer_from_table("PyObject_Length");
   return (*fp) (ob); //execute
}
int AMD_PyObject_RichCompareBool(PyObject * ob, PyObject * ob2, int v){
   AMD_TRACE_API
   pyobj_richcomparebool fp =(pyobj_richcomparebool) get_fun_pointer_from_table("PyObject_RichCompareBool");
   return (*fp) (ob,ob2,v); //execute
}
PyObject* AMD_PyLong_FromLong(long v){
   AMD_TRACE_API
   pybool_fromlong fp =(pybool_fromlong) get_fun_pointer_from_table("PyLong_FromLong");
   return (*fp) (v); //execute
}
PyObject* AMD_PyLong_FromLongLong(long long v){
   AMD_TRACE_API
   pylong_fromlonglong fp =(pylong_fromlonglong) get_fun_pointer_from_table("PyLong_FromLongLong");
   return (*fp) (v); //execute
}
PyObject* AMD_PyLong_FromUnsignedLongLong(unsigned long long v){
   AMD_TRACE_API
   pylong_fromunsignedlonglong fp =(pylong_fromunsignedlonglong) get_fun_pointer_from_table("PyLong_FromUnsignedLongLong");
   return (*fp) (v); //execute
}
unsigned long long AMD_PyLong_AsUnsignedLongLong(PyObject * ob){
   AMD_TRACE_API
   pylong_asunsignedlonglong fp =(pylong_asunsignedlonglong) get_fun_pointer_from_table("PyLong_AsUnsignedLongLong");
   return (*fp) (ob); //execute
}
int AMD_PyUnicode_CompareWithASCIIString(PyObject *left, const char *right){
   AMD_TRACE_API
   pyobj_hasattstr fp =(pyobj_hasattstr) get_fun_pointer_from_table("PyUnicode_CompareWithASCIIString");
   return (*fp) (left,right); //execute
}
PyObject* AMD_PyUnicode_AsEncodedString(PyObject *unicode,const char *encoding,const char *errors){
   AMD_TRACE_API
   pyunicode_asencodedstr fp =(pyunicode_asencodedstr) get_fun_pointer_from_table("PyUnicode_AsEncodedString");
   return (*fp) (unicode,encoding,errors); //execute
}
PyObject* AMD_PyBytes_FromString(const char * str){
   AMD_TRACE_API
   py_convert_str_to_pyobj fp =(py_convert_str_to_pyobj) get_fun_pointer_from_table("PyBytes_FromString");
   return (*fp) (str); //execute
}
char* AMD_PyBytes_AsString(PyObject * ob){
   AMD_TRACE_API
   pybytes_asstring fp =(pybytes_asstring) get_fun_pointer_from_table("PyBytes_AsString");
   return (*fp) (ob); //execute
}
void AMD_PyErr_SetNone(PyObject * ob){
   AMD_TRACE_API
   pyerr_setnone fp =(pyerr_setnone) get_fun_pointer_from_table("PyErr_SetNone");
   return (*fp) (ob); //execute   
}
PyObject* AMD_PyNumber_Long(PyObject *ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyNumber_Long");
   return (*fp) (ob); //execute   
}
int AMD_PyModule_AddObject(PyObject *mod, const char * str, PyObject *value){
   AMD_TRACE_API
   pydict_setitemstring fp =(pydict_setitemstring) get_fun_pointer_from_table("PyModule_AddObject");
   return (*fp) (mod,str,value); //execute   
}
int AMD_PyModule_AddStringConstant(PyObject* ob, const char *str , const char * fr){
   AMD_TRACE_API
   pymodule_addstringconstant fp =(pymodule_addstringconstant) get_fun_pointer_from_table("PyModule_AddStringConstant");
   return (*fp) (ob,str,fr); //execute   
}
PyObject* AMD_PyModule_GetDict(PyObject * ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("PyModule_GetDict");
   return (*fp) (ob); //execute   
}
int AMD_PyErr_GivenExceptionMatches(PyObject * ob, PyObject * v){
   AMD_TRACE_API
   pylist_append fp =(pylist_append) get_fun_pointer_from_table("PyErr_GivenExceptionMatches");
   return (*fp) (ob,v); //execute
}
PyObject* AMD_PyErr_NewException(const char *name, PyObject *base, PyObject *dict){
   AMD_TRACE_API
   pyerr_newexception fp =(pyerr_newexception) get_fun_pointer_from_table("PyErr_NewException");
   return (*fp) (name,base,dict); //execute
}
void AMD_PyErr_SetInterrupt(void){
   AMD_TRACE_API
   pyerr_clear fp =(pyerr_clear) get_fun_pointer_from_table("PyErr_SetInterrupt");
   return (*fp) (); //execute    
}
void AMD_PyErr_Print(void){
   AMD_TRACE_API
   pyerr_clear fp =(pyerr_clear) get_fun_pointer_from_table("PyErr_Print");
   return (*fp) (); //execute   
}
PyObject* AMD_PyFloat_FromDouble(double val){
   AMD_TRACE_API
   pyfloat_fromdouble fp =(pyfloat_fromdouble) get_fun_pointer_from_table("PyFloat_FromDouble");
   return (*fp) (val); //execute
}
double AMD_PyFloat_AsDouble(PyObject * ob){
   AMD_TRACE_API
   pyfloat_asdouble fp =(pyfloat_asdouble) get_fun_pointer_from_table("PyFloat_AsDouble");
   return (*fp) (ob); //execute
}
PyObject* AMD_PySequence_Concat(PyObject *o1, PyObject *o2){
   AMD_TRACE_API
   pytobj_getattr fp =(pytobj_getattr) get_fun_pointer_from_table("PySequence_Concat");
   return (*fp) (o1,o2); //execute   
}
PyObject * AMD_PySys_GetObject(const char * str){
   AMD_TRACE_API
   py_convert_str_to_pyobj fp =(py_convert_str_to_pyobj) get_fun_pointer_from_table("PySys_GetObject");
   return (*fp) (str); //execute   
}
void AMD_PySys_SetPath(const wchar_t * str){
   AMD_TRACE_API
   pysys_setpath fp =(pysys_setpath) get_fun_pointer_from_table("PySys_SetPath");
   return (*fp) (str); //execute   
}
int AMD_PyOS_InterruptOccurred(void){
   AMD_TRACE_API
   pyos_interruptoccurred fp =(pyos_interruptoccurred) get_fun_pointer_from_table("PyOS_InterruptOccurred");
   return (*fp) (); //execute 
}
PyObject* AMD_PyImport_AddModule(const char *name ){
   AMD_TRACE_API
   py_convert_str_to_pyobj fp =(py_convert_str_to_pyobj) get_fun_pointer_from_table("PyImport_AddModule");
   return (*fp) (name); //execute
}
int AMD_PyImport_ExtendInittab(struct _inittab *newtab){
   AMD_TRACE_API
   pyimport_extendinitab fp =(pyimport_extendinitab) get_fun_pointer_from_table("PyImport_ExtendInittab");
   return (*fp) (newtab); //execute
}
PyObject* AMD_PyEval_EvalCode(PyObject * o1, PyObject *o2, PyObject * o3){
   AMD_TRACE_API
   pyeval_evalcode fp =(pyeval_evalcode) get_fun_pointer_from_table("PyEval_EvalCode");
   return (*fp) (o1,o2,o3); //execute
}
PyThreadState* AMD_PyEval_SaveThread(void){
   AMD_TRACE_API
   pyeval_savethread fp =(pyeval_savethread) get_fun_pointer_from_table("PyEval_SaveThread");
   return (*fp) (); //execute
}
void AMD_PyEval_RestoreThread(PyThreadState * th){
   AMD_TRACE_API
   pyeval_restorethread fp =(pyeval_restorethread) get_fun_pointer_from_table("PyEval_RestoreThread");
   return (*fp) (th); //execute
}
int AMD_PyRun_InteractiveLoopFlags(FILE *filep, const char *filename, PyCompilerFlags *flags){
   AMD_TRACE_API
   pyrun_interactiveloopflags fp =(pyrun_interactiveloopflags) get_fun_pointer_from_table("PyRun_InteractiveLoopFlags");
   return (*fp) (filep,filename, flags); //execute
}
PyObject* AMD_PyRun_StringFlags(const char *  name, int v, PyObject * o1, PyObject * o2,PyCompilerFlags * flags ){
   AMD_TRACE_API
   pyrun_stringflags fp =(pyrun_stringflags) get_fun_pointer_from_table("PyRun_StringFlags");
   return (*fp) (name,v,o1,o2,flags); //execute
}
PyObject* AMD_Py_CompileStringExFlags(const char *str, const char *filename, int start, PyCompilerFlags *flags,int optimize){
   AMD_TRACE_API
   py_compilestringexflags fp =(py_compilestringexflags) get_fun_pointer_from_table("Py_CompileStringExFlags");
   return (*fp) (str,filename,start,flags,optimize); //execute
}
void AMD_Py_Initialize(void){
   AMD_TRACE_API
   pyerr_clear fp =(pyerr_clear) get_fun_pointer_from_table("Py_Initialize");
   return (*fp) (); //execute
}
void AMD_Py_Finalize(void){
   AMD_TRACE_API
   pyerr_clear fp =(pyerr_clear) get_fun_pointer_from_table("Py_Finalize");
   return (*fp) (); //execute
}
PyObject * AMD_PyErr_Occurred(void){
   AMD_TRACE_API
   pydict_new fp =(pydict_new) get_fun_pointer_from_table("PyErr_Occurred");
   return (*fp) (); //execute
}
void AMD_PyErr_SetObject(PyObject* ob1, PyObject* ob2){
   AMD_TRACE_API
   pyerr_setobject fp =(pyerr_setobject) get_fun_pointer_from_table("PyErr_SetObject");
   return (*fp) (ob1,ob2); //execute
}
PyObject* AMD_PyObject_Call(PyObject *callable, PyObject *args, PyObject *kwargs){
   AMD_TRACE_API
   pyeval_evalcode fp =(pyeval_evalcode) get_fun_pointer_from_table("PyObject_Call");
   return (*fp) (callable,args,kwargs); //execute
}
void AMD_Py_SetProgramName(const wchar_t * name){
   AMD_TRACE_API
   pysys_setpath fp =(pysys_setpath) get_fun_pointer_from_table("Py_SetProgramName");
   return (*fp) (name); //execute
}

int AMD_PySlice_GetIndicesEx(PyObject *r, Py_ssize_t length, Py_ssize_t *start, Py_ssize_t *stop,
                                     Py_ssize_t *step, Py_ssize_t *slicelength){
   AMD_TRACE_API
   pyslice_getindicesex fp =(pyslice_getindicesex) get_fun_pointer_from_table("PySlice_GetIndicesEx");
   return (*fp) (r,length,start,stop,step,slicelength); //execute
}

void _AMD_Py_DECREF(PyObject* ob){
   AMD_TRACE_API
   pyerr_setnone fp =(pyerr_setnone) get_fun_pointer_from_table("Py_DecRef");
   return (*fp) (ob); //execute   
}

int  AMD_PyIter_Check(PyObject* ob){
   AMD_TRACE_API
   pyobj_istrue fp =(pyobj_istrue) get_fun_pointer_from_table("PyIter_Check");
   return (*fp) (ob); //execute    
}
void  _Py_Dealloc(PyObject* ob){
   AMD_TRACE_API
   pyerr_setnone fp = (pyerr_setnone) get_fun_pointer_from_table("_Py_Dealloc");
   return (*fp) (ob); //execute    
}

PyObject * _AMD_PyObject_New_(PyTypeObject * ob){
   AMD_TRACE_API
   py_new_func fp = (py_new_func) get_fun_pointer_from_table("_PyObject_New");
   return (*fp) (ob); //execute
}

PyObject* AMD_PyObject_Type(PyObject *ob){
   AMD_TRACE_API
   pylist_astuple fp = (pylist_astuple) get_fun_pointer_from_table("PyObject_Type");
   return (*fp)(ob);
}

#if 0
void AMD_Py_CompileStringExFlags(const wchar_t * str){
   AMD_TRACE_API
   pysys_setpath fp =(pysys_setpath) get_fun_pointer_from_table("Py_CompileStringExFlags");
   return (*fp) (str); //execute   
}

PyObject* AMD_Py_NewRef(PyObject* ob){
   AMD_TRACE_API
   pylist_astuple fp =(pylist_astuple) get_fun_pointer_from_table("Py_NewRef");
   return (*fp) (ob); //execute   
}

#endif
