#pragma ide diagnostic ignored "misc-no-recursion"
#ifndef LAB_HUFFMAN_CODER_HUFFMANUTILS_H

#define MAX_CODE_LENGTH 256

#define copyCode(to, from, length) { memcpy(to.code, from, length); to.len = length; }

void huffmanNodeInitPrefixCodes(huffmanCode *codesList, huffmanNode *parentPtr, uint8_t *parentCode, uint8_t codeLen,
                                binaryBuffer *buf) {
    uint8_t code[256] = {0};
    memcpy(code, parentCode, codeLen);
    if (!parentPtr->left && !parentPtr->right) {
        // Init code
        if (codeLen == 0)
            codeLen += 1;
        copyCode(codesList[parentPtr->symbol], code, codeLen)
        // Writing the code to a file;
        binaryBufferWriteBit(buf, true);
        binaryBufferWriteChar(buf, parentPtr->symbol);
    } else {
        binaryBufferWriteBit(buf, false);
        if (parentPtr->left) {
            code[codeLen] = 0;
            huffmanNodeInitPrefixCodes(codesList, parentPtr->left, code, codeLen + 1, buf);
        }
        if (parentPtr->right) {
            code[codeLen] = 1;
            huffmanNodeInitPrefixCodes(codesList, parentPtr->right, code, codeLen + 1, buf);
        }
    }
}


exitCodes huffmanNodeCreateNode(huffmanNode **node) {
    *node = malloc(sizeof(huffmanNode));
    if (!(*node))
        return OUT_OF_MEMORY;
    (*node)->symbol = UINT_MAX;
    (*node)->frequency = UINT_MAX;
    (*node)->left = NULL;
    (*node)->right = NULL;
    return SUCCESS;
}

exitCodes huffmanNodeRecoverPrefixTree(huffmanNode **parentNullPtr, binaryBuffer *buf) {
    huffmanNodeCreateNode(parentNullPtr);

    bool isExist;
    exitCodes opCode = binaryBufferReadBit(buf, &isExist);
    if (SUCCESS != opCode)
        return opCode;

    if (isExist) {
        uint8_t curSmb;
        opCode = binaryBufferReadChar(buf, &curSmb);
        if (SUCCESS != opCode)
            return opCode;
        (*parentNullPtr)->symbol = curSmb;
        return SUCCESS;
    } else {
        opCode = huffmanNodeRecoverPrefixTree(&(*parentNullPtr)->left, buf);
        if (SUCCESS != opCode)
            return opCode;
        opCode = huffmanNodeRecoverPrefixTree(&(*parentNullPtr)->right, buf);
        if (SUCCESS != opCode)
            return opCode;
    }
    return SUCCESS;
}

exitCodes getCompressedDataBitesCount(uint64_t *outInt, FILE *inStream, uint8_t unreadInBufCount) {
    int32_t curPosInFile = ftell(inStream);
    if (curPosInFile < 0)
        return INVALID_STREAM_EXCEPTION;
    fseek(inStream, -1L, SEEK_END);
    uint8_t trashBitsCount = getc(inStream);
    if (ftell(inStream) < 0)
        return INVALID_STREAM_EXCEPTION;
    *outInt = (ftell(inStream) - curPosInFile - 1) * 8 + unreadInBufCount - trashBitsCount;
    fseek(inStream, curPosInFile, SEEK_SET);
    return SUCCESS;
}

void huffmanNodeFree(huffmanNode *parent) {
    if (parent->left)
        huffmanNodeFree(parent->left);
    if (parent->right)
        huffmanNodeFree(parent->right);
    free(parent);
}

#endif