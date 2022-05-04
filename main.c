#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>
#include <string.h>

#include "exitCodes.h"
#include "userTypes.h"
#include "binaryBufferUtils.h"
//#include "chunkBufferUtils.h" (unused)
#include "priorityQueueUtils.h"
#include "huffmanUtils.h"
#include "huffmanCoder.h"


int main(int argc, char *args[]) {
    if (argc != 4)
        return 0;
    FILE *inFile = fopen(args[2], "rb");
    FILE *outFile = fopen(args[3], "wb");
    fseek(inFile, 0L, SEEK_END);
    uint32_t endInx = ftell(inFile);
    fseek(inFile, 0L, SEEK_SET);

    if (endInx <= 1) {
        fclose(outFile);
        fclose(inFile);
        return 0;
    }

    exitCodes opCode;

    if (0 == strcmp(args[1], "-c"))
        opCode = huffmanEncodeToBuff(inFile, outFile);
    if (0 == strcmp(args[1], "-d"))
        opCode = huffmanDecodeFromBuff(inFile, outFile);
    if (opCode != SUCCESS)
        printf("Error caught: %s", exitMsg[opCode]);
    fclose(inFile);
    fclose(outFile);
    return 0;
}