#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>

#include "exitCodes.h"
#include "userTypes.h"
#include "binaryBufferUtils.h"
//#include "chunkBufferUtils.h" (unused)
#include "priorityQueueUtils.h"
#include "huffmanUtils.h"
#include "huffmanCoder.h"

#define INPUT_FILE_NAME "in.txt"
#define OUTPUT_FILE_NAME "out.txt"

int main() {
    FILE *inFile = fopen(INPUT_FILE_NAME, "rb");
    FILE *outFile = fopen(OUTPUT_FILE_NAME, "wb");
    fseek(inFile, 0L, SEEK_END);
    uint32_t endInx = ftell(inFile);
    fseek(inFile, 0L, SEEK_SET);

    if (endInx <= 1) {
        fclose(outFile);
        fclose(inFile);
        return 0;
    }
    uint8_t opSmb = getc(inFile);

    exitCodes opCode;
    if (opSmb == 'c')
        opCode = huffmanEncodeToBuff(inFile, outFile);
    if (opSmb == 'd')
        opCode = huffmanDecodeFromBuff(inFile, outFile);
    if (opCode != SUCCESS)
        printf("Error caught: %s", exitMsg[opCode]);

    fclose(inFile);
    fclose(outFile);
    return 0;
}