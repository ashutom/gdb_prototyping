#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>
#include "amd_pythonwrapper.h"
#include </usr/include/python3.9/Python.h>


const char* LIBRARY_WITH_PATH = "/usr/lib64/libpython3.9.so.1.0";


typedef int (*va_arg_pyfunc) (PyObject *args, PyObject *kw, const char *format, char **keywords, ...);



int  AMD_PyArg_VaParseTupleAndKeywords(PyObject *args, PyObject *kw,
                                       const char *format, char **keywords, ...){

    
    fprintf(stderr, "%s\n", "Ashutosh 's Implementation of PyArg_VaParseTupleAndKeywords <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");  
    void* handle = dlopen(LIBRARY_WITH_PATH, RTLD_LAZY);  
    if (!handle) {  
        fprintf(stderr, "%s\n", dlerror());  
        exit(EXIT_FAILURE);  
    }  
  
    // Look up the symbol
    va_arg_pyfunc PyArg_VaParseTupleAndKeywords = (va_arg_pyfunc)dlsym(handle, "PyArg_VaParseTupleAndKeywords");  
    if (!PyArg_VaParseTupleAndKeywords) {  
        fprintf(stderr, "%s\n", dlerror());  
        dlclose(handle);  
        exit(EXIT_FAILURE);  
    }  
  
    // Use the function  
    va_list ap;

    va_start (ap, keywords);

    //int result = (*PyArg_VaParseTupleAndKeywords)(args, kw, format, const_cast<char **> (keywords), ap);  
    int result = (*PyArg_VaParseTupleAndKeywords)(args, kw, format, keywords, ap);  
    printf("Call status of the API :  [ %d ]\n", result);  
  
    va_end (ap);
    // Close the library  
    dlclose(handle);  
  
    return result;
}  

