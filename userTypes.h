#ifndef LAB_HUFFMAN_CODER_USERTYPES_H

// Huffman tree

typedef struct huffmanNode {
    uint32_t symbol;
    uint32_t frequency;
    struct huffmanNode *left;
    struct huffmanNode *right;
} huffmanNode;

typedef struct {
    uint8_t *code;
    uint8_t len;
} huffmanCode;

// Binary buffer struct

typedef enum {
    r, w, rw
} accessTypes;

typedef struct {
    uint8_t buf;
    uint8_t position;
    const accessTypes type;
    FILE *stream;
} binaryBuffer;

// Chunk buffer struct
// (unused)
typedef struct {
    uint32_t length;
    const uint32_t chunkSize;
    uint32_t chunkCount;
    uint8_t **chunkArr;
} chunkBuffer;


#endif
