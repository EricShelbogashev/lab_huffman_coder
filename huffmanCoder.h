#ifndef LAB_HUFFMAN_CODER_HUFFMANCODER_H

exitCodes huffmanEncodeToBuff(FILE *inStream, FILE *outStream) {
    int_fast32_t startPos = ftell(inStream  );
    fseek(inStream, 0L, SEEK_END);
    uint64_t length = ftell(inStream) - startPos;
    fseek(inStream, startPos, SEEK_SET);
    uint32_t probTable[256];
    for (int i = 0; i < 256; ++i)
        probTable[i] = 0;
    uint32_t nodeArrSize = 0;

    // Frequency initialize
    uint8_t tmp;
    for (int i = 0; i < length; ++i) {
        tmp = getc(inStream);
        if (probTable[tmp] == 0)
            nodeArrSize += 1;
        probTable[tmp] += 1;
    }

    // Инициализация списка с кодами
    huffmanCode codesList[UCHAR_MAX];
    uint8_t *memSet = malloc(MAX_CODE_LENGTH * nodeArrSize * sizeof(uint8_t));

    if (!memSet) {
        return OUT_OF_MEMORY;
    }

    // Инициализация массива для дерева
    priorityQueueList *nodePriorQue = NULL;
    priorityQueueListInit(&nodePriorQue);
    huffmanNode *tmpNode = NULL;
    uint32_t nodeInx = 0;
    uint8_t memInx = 0;
    for (int i = 0; i < 256; ++i) {
        if (probTable[i] != 0) {
            huffmanNodeCreateNode(&tmpNode);
            tmpNode->symbol = i;
            tmpNode->frequency = probTable[i];
            tmpNode->left = NULL;
            tmpNode->right = NULL;
            priorityQueueListInsert(nodePriorQue, tmpNode);
            codesList[i].code = &memSet[MAX_CODE_LENGTH * memInx * sizeof(uint8_t)];
            nodeInx++;
            memInx++;
        }
    }

    // построение дерева - алгоритм Хаффмана
    huffmanNode *a = NULL, *b = NULL, *c = NULL;
    for (int i = 0; i < nodeArrSize - 1; ++i) {
        priorityQueueListPopNode(nodePriorQue, &a);
        priorityQueueListPopNode(nodePriorQue, &b);
        huffmanNodeCreateNode(&c);
        c->left = a;
        c->right = b;
        c->frequency = a->frequency + b->frequency;
        priorityQueueListInsert(nodePriorQue, c);
    }
    // Инициализация буффера для вывода в файл
    binaryBuffer *binBuf = NULL;
    binaryBufferInit(&binBuf, outStream, w);

    huffmanNode *parentNode = NULL;
    priorityQueueListPopNode(nodePriorQue, &parentNode);
    uint8_t parentCode = 0;
    uint8_t codeLen = 0;

    //Получение кодов через дерево
    huffmanNodeInitPrefixCodes(codesList, parentNode, &parentCode, codeLen, binBuf);

    uint8_t curSmb;
    huffmanCode curCode;
    fseek(inStream, startPos, SEEK_SET);
    for (int i = 0; i < length; ++i) {
        curSmb = getc(inStream);
        curCode = codesList[curSmb];
        for (int j = 0; j < curCode.len; ++j) {
            binaryBufferWriteBit(binBuf, curCode.code[j]);
        }
    }

    free(memSet);
    binaryBufferClose(binBuf);
    return SUCCESS;
}

exitCodes huffmanDecodeFromBuff(FILE *inStream, FILE *outStream) {
    huffmanNode *tree = NULL;
    binaryBuffer *binBuf = NULL;
    binaryBufferInit(&binBuf, inStream, r);

    huffmanNodeRecoverPrefixTree(&tree, binBuf);
    uint64_t numOfBitsToRead;
    exitCodes opCode = getCompressedDataBitesCount(&numOfBitsToRead, inStream, binaryBufferGetUnreadCount(binBuf));
    if (SUCCESS != opCode) {
        binaryBufferClose(binBuf);
        return opCode;
    }
    bool curBit;
    huffmanNode *curNode = tree;
    while (numOfBitsToRead > 0) {
        opCode = binaryBufferReadBit(binBuf, &curBit);
        numOfBitsToRead -= 1;
        if (SUCCESS != opCode)
            return opCode;
        if (curNode->symbol == UINT_MAX)
            curNode = curBit ? curNode->right : curNode->left;
        if (curNode->left == NULL && curNode->right == NULL) {
            putc((int8_t) curNode->symbol, outStream);
            curNode = tree;
        }
    }

    huffmanNodeFree(tree);
    binaryBufferClose(binBuf);
    return SUCCESS;
}

#endif