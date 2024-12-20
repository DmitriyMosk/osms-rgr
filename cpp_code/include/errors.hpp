#include <iostream> 

#ifndef LIB_HELPERS_ERRORS
#define LIB_HELPERS_ERRORS

    #define ERR_READING_INPUT_FAULT "Error reading input!"
    void __err_reading_input_fault(); 

    #define ERR_UNKNOWN_MODULATION_TYPE "Error set modulation type!"
    void __err_unknown_modulation_type(); 

    #define ERR_ARG_INCORRECT_SINR "Error set sinr must be [0:1] !"
    void __err_arg_incorrect_sigma(); 

    #define ERR_FSTREAM_FAULT "Error opening file"
    void __err_fstream_fault(); 

    #define ERR_ARG_INCORRECT_IO_BUFFER_SIZE "Xyu"
    void __err_arg_incorrect_io_buffer_size();

    #define ERR_BUFFER_OVERFLOW "Ggg" 
    void __err_buffer_overflow(); 
    
#endif