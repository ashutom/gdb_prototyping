#ifndef __AMD_PYTHON_WRAPPER__ 
#define __AMD_PYTHON_WRAPPER__


#include <stdio.h>  
#include <stdlib.h>  
#include <dlfcn.h>  
  

const char* LIBRARY_WITH_PATH = "/usr/lib64/libpython3.9.so.1.0";


int (*function_name)(int);


void construct_function_list() {
    
    //fun->function_name = (int (*)(int))dlsym(dlopen(LIBRARY_WITH_PATH, RTLD_LAZY), "function_name");
}


int amd_main() {  
    // Open the shared library  
    void* handle = dlopen(LIBRARY_WITH_PATH, RTLD_LAZY);  
    if (!handle) {  
        fprintf(stderr, "%s\n", dlerror());  
        exit(EXIT_FAILURE);  
    }  
  
    // Look up the symbol  
    typedef int (*func_t)(int);  
    func_t my_func = (func_t)dlsym(handle, "function_name");  
    if (!my_func) {  
        fprintf(stderr, "%s\n", dlerror());  
        dlclose(handle);  
        exit(EXIT_FAILURE);  
    }  
  
    // Use the function  
    int result = my_func(42);  
    printf("Result: %d\n", result);  
  
    // Close the library  
    dlclose(handle);  
  
    return 0;  
}  


#endif /* __AMD_PYTHON_WRAPPER__ */
