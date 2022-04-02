#ifndef LAB_HUFFMAN_CODER_CHUNKBUFFER_H

exitCodes chunkBufferAddChunk(chunkBuffer *buf) {
    uint8_t **tmp = realloc(buf->chunkArr, buf->chunkSize * (buf->chunkCount + 1));
    if (!tmp)
        return NULL_POINTER_EXCEPTION;
    uint8_t *tmp1 = malloc(buf->chunkSize);
    if (!tmp1) {
        free(tmp);
        return OUT_OF_MEMORY;
    }
    buf->chunkArr = tmp;
    buf->chunkArr[buf->chunkCount] = tmp1;
    buf->chunkCount += 1;
    return SUCCESS;
}

exitCodes chunkBufferPutSym(chunkBuffer *buf, uint8_t data) {
    if (buf->chunkCount * buf->chunkSize >= buf->length + 1) {
        uint8_t **tmp = realloc(buf->chunkArr, buf->chunkSize * (buf->chunkCount + 1));
        if (tmp) {
            buf->chunkArr = tmp;
            buf->chunkCount += 1;
        } else return OUT_OF_MEMORY;
    }
    buf->chunkArr[buf->length / buf->chunkSize][buf->length % buf->chunkSize] = data;
    buf->length += 1;
    return SUCCESS;
}

exitCodes chunkBufferGetFromBuffByInx(chunkBuffer *buf, uint32_t inx, uint8_t *outSmb) {
    if (buf->length > inx) {
        *outSmb = buf->chunkArr[inx / buf->chunkSize][inx % buf->chunkSize];
        return SUCCESS;
    }
    return ILLEGAL_ARGUMENT_EXCEPTION;
}

exitCodes chunkBufferFillBuffFromStream(chunkBuffer *buf, uint32_t chunkSize, FILE *inStream) {
    uint8_t *chunkTmp;
    uint32_t readNum;
    while (!feof(inStream)) {
        chunkBufferAddChunk(buf);
        chunkTmp = buf->chunkArr[buf->chunkCount - 1];
        readNum = fread(chunkTmp, sizeof(uint8_t), chunkSize, inStream);
        buf->length += readNum;
    }
    return SUCCESS;
}

exitCodes chunkBufferInit(chunkBuffer **nullBuf, uint32_t chunkSize) {
    chunkBuffer tmp = {0, chunkSize, 0, NULL};
    *nullBuf = malloc(sizeof(chunkBuffer));
    if (!*nullBuf)
        return OUT_OF_MEMORY;
    memcpy(*nullBuf, &tmp, sizeof(chunkBuffer));
    return SUCCESS;
}

exitCodes chunkBufferClose(chunkBuffer *buf) {
    for (int i = 0; i < buf->chunkCount; ++i) {
        free(buf->chunkArr[i]);
    }
    free(buf->chunkArr);
    free(buf);
    return SUCCESS;
}

#endif
