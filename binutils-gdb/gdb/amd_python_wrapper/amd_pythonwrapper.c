#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>
#include "amd_pythonwrapper.h"
#include "amd_function_pointers_list.h"


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

int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw,
                                       const char *format, char **keywords, ...){    
    void* handle = AMD_get_lib_handle();
    check_lib_handeler_execption(handle);
    // Look up the symbol
    va_arg_pyfunc PyArg_VaParseTupleAndKeywords=NULL;
    char funname[] = "PyArg_VaParseTupleAndKeywords";
    PyArg_VaParseTupleAndKeywords=(va_arg_pyfunc) check_symbol_resolution((void*) PyArg_VaParseTupleAndKeywords, funname); 
    /*va_arg_pyfunc PyArg_VaParseTupleAndKeywords = (va_arg_pyfunc)dlsym(handle, "PyArg_VaParseTupleAndKeywords");
    if (!PyArg_VaParseTupleAndKeywords) {  
        fprintf(stderr, "%s\n", dlerror());  
        AMD_close_lib_handle();  
        exit(EXIT_FAILURE);  
    }*/

    //Use the function  
    va_list ap;

    va_start (ap, keywords);

    /*int result = (*PyArg_VaParseTupleAndKeywords)(args, kw, format, const_cast<char **> (keywords), ap);  */
    int result = (*PyArg_VaParseTupleAndKeywords)(args, kw, format, keywords, ap);  
    printf("Call status of the API :[ %s ]  is [ %d ]\n",funname, result);
  
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
   void* handle = AMD_get_lib_handle();
   if(!handle) 
      AMD_lib_exception_failure_handeler();
   
   char str[]="PyErr_SetString";
   pyerror_setstring fp =  NULL;
   fp =  (pyerror_setstring) check_symbol_resolution((void*) fp,str);
   (*fp) (exception, Errstr); //execute
}

PyObject* AMD_PyUnicode_FromString(const char *ustr){
   void* handle = AMD_get_lib_handle();
   if(!handle) 
      AMD_lib_exception_failure_handeler();
   
   char str[]="PyUnicode_FromString";
   py_convert_str_to_pyobj fp =  NULL;
   fp =  (py_convert_str_to_pyobj) check_symbol_resolution((void*) fp,str);
   return (PyObject*) (*fp) (ustr); //execute
}
    

