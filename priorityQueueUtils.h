#ifndef LAB_HUFFMAN_CODER_PRIORITYQUEUE_H

typedef struct priorityQueueList {
    huffmanNode *node;
    struct priorityQueueList *next;
    struct priorityQueueList *previous;
} priorityQueueList;

exitCodes priorityQueueListCreateElement(priorityQueueList **list, huffmanNode *node) {
    *list = malloc(sizeof(priorityQueueList));
    if (!*list)
        return OUT_OF_MEMORY;
    (*list)->node = node;
    (*list)->next = NULL;
    (*list)->previous = NULL;
    return SUCCESS;
}

exitCodes priorityQueueListInsert(priorityQueueList *list, huffmanNode *node) {
    if (!list)
        return ILLEGAL_ARGUMENT_EXCEPTION;
    priorityQueueList *tmp = NULL;
    uint32_t opCode = priorityQueueListCreateElement(&tmp, node);
    if (SUCCESS != opCode)
        return opCode;
    priorityQueueList *cur = list->next;
    if (!cur) {
        list->next = tmp;
        tmp->previous = list;
        return SUCCESS;
    }
    while (cur) {
        if (cur->node->frequency >= tmp->node->frequency) {
            tmp->next = cur;
            tmp->previous = cur->previous;
            cur->previous->next = tmp;
            cur->previous = tmp;
            return SUCCESS;
        }
        if (!cur->next)
            break;
        cur = cur->next;
    }
    cur->next = tmp;
    tmp->previous = cur;
    return SUCCESS;
}

exitCodes priorityQueueListPopNode(priorityQueueList *list, huffmanNode **node) {
    if (!list)
        return ILLEGAL_ARGUMENT_EXCEPTION;
    if (!list->next)
        return EMPTY_ARRAY;
    (*node) = list->next->node;
    priorityQueueList *tmp = list->next;
    list->next = list->next->next;
    if (list->next)
        list->next->previous = list;
    free(tmp);
    return SUCCESS;
}

exitCodes priorityQueueListInit(priorityQueueList **list) {
    return priorityQueueListCreateElement(list, NULL);
}

#endif