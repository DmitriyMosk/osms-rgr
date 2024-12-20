#include "errors.hpp" 

void __err_reading_input_fault() { 
    std::cerr << ERR_READING_INPUT_FAULT << std::endl; 
}

void __err_unknown_modulation_type() { 
    std::invalid_argument(ERR_UNKNOWN_MODULATION_TYPE);
}

void __err_arg_incorrect_sigma() { 
    std::invalid_argument(ERR_ARG_INCORRECT_SINR);
} 

void __err_arg_incorrect_io_buffer_size() { 
    std::invalid_argument(ERR_ARG_INCORRECT_IO_BUFFER_SIZE);
} 

void __err_fstream_fault() { 
    std::cerr << ERR_FSTREAM_FAULT << std::endl;
}

void __err_buffer_overflow() { 
    std::cerr << ERR_BUFFER_OVERFLOW << std::endl;
}