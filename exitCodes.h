#ifndef LAB_HUFFMAN_CODER_EXITCODES_H

typedef enum {
    ILLEGAL_ARGUMENT_EXCEPTION,
    NULL_POINTER_EXCEPTION,
    INVALID_STREAM_EXCEPTION,
    ILLEGAL_OPERATION_EXCEPTION,
    EMPTY_ARRAY,
    END_OF_FILE,
    OUT_OF_MEMORY,
    SUCCESS
} exitCodes;

char *exitMsg[] = {
  "Illegal argument",
  "Null pointer exception",
  "Stream invalid",
  "Illegal operation exception",
  "Array is empty",
  "EOF",
  "Out of memory",
  "Success"
};

#endif
