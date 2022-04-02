#ifndef LAB_HUFFMAN_CODER_BINARYBUFFERUTILS_H

#define binaryBufferValidCheck(buf) { if (!buf) return NULL_POINTER_EXCEPTION; if (!buf->stream) return INVALID_STREAM_EXCEPTION; }

#define binaryBufferUnloadIfFull(buf) { if (buf->position == 8) { putc(buf->buf, buf->stream); buf->position = 0; } }

#define binaryBufferLoadIfEmpty(buf) { uint32_t tmp; if (buf->position == 0) { tmp = getc(buf->stream); if (tmp == EOF) return END_OF_FILE; else { buf->buf = tmp; buf->position = 8; } } }

#define binaryBufferWriteAccessCheck(buf) { if (buf->type == r) return ILLEGAL_OPERATION_EXCEPTION; }

#define binaryBufferReadAccessCheck(buf) { if (buf->type == w) return ILLEGAL_OPERATION_EXCEPTION; }

exitCodes binaryBufferWriteBit(binaryBuffer *buf, bool value) {
    binaryBufferValidCheck(buf)
    binaryBufferWriteAccessCheck(buf)

    if (value != true && value != false)
        return ILLEGAL_ARGUMENT_EXCEPTION;

    binaryBufferUnloadIfFull(buf)

    buf->buf <<= 1;
    buf->buf |= value;
    buf->position += 1;

    return SUCCESS;
}

exitCodes binaryBufferWriteChar(binaryBuffer *buf, uint8_t value) {
    binaryBufferValidCheck(buf)
    binaryBufferWriteAccessCheck(buf)

    for (int i = 7; i >= 0; --i) {
        binaryBufferUnloadIfFull(buf)
        buf->buf <<= 1;
        buf->buf |= ((1 << i) & value) >> i;
        buf->position += 1;
    }

    return SUCCESS;
}

exitCodes binaryBufferReadBit(binaryBuffer *buf, bool *value) {
    binaryBufferValidCheck(buf)
    binaryBufferReadAccessCheck(buf)

    binaryBufferLoadIfEmpty(buf)
    buf->position -= 1;
    *value = ((1 << buf->position) & buf->buf) >> buf->position;

    return SUCCESS;
}

uint8_t binaryBufferGetUnreadCount(binaryBuffer *buf) {
    // Warning bufMayBeNull
    return buf->position;
}

exitCodes binaryBufferReadChar(binaryBuffer *buf, uint8_t *value) {
    binaryBufferValidCheck(buf)
    binaryBufferReadAccessCheck(buf)

    for (int i = 0; i < 8; ++i) {
        binaryBufferLoadIfEmpty(buf)
        buf->position -= 1;
        *value <<= 1;
        *value |= ((1 << buf->position) & buf->buf) >> buf->position;
    }

    return SUCCESS;
}

exitCodes binaryBufferInit(binaryBuffer **nullBuf, FILE *stream, accessTypes type) {
    if (type != r && type != w && type != rw)
        return ILLEGAL_ARGUMENT_EXCEPTION;

    binaryBuffer tmp = {0, 0, type, stream};
    *nullBuf = malloc(sizeof(binaryBuffer));

    if (!*nullBuf)
        return OUT_OF_MEMORY;

    memcpy(*nullBuf, &tmp, sizeof(binaryBuffer));
    (*nullBuf)->buf = 0;

    return SUCCESS;
}

exitCodes binaryBufferClose(binaryBuffer *buf) {
    binaryBufferValidCheck(buf)

    if (buf->type == w) {
        binaryBufferUnloadIfFull(buf)
        uint8_t trashCount = 0;

        while (buf->position > 0) {
            buf->buf <<= 1;
            buf->position += 1;
            binaryBufferUnloadIfFull(buf)
            trashCount++;
        }
        putc(trashCount, buf->stream);
    }

    free(buf);
    return SUCCESS;
}

#endif