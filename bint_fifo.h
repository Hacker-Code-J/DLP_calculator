#ifndef BINT_FIFO_H
#define BINT_FIFO_H

#include <stdint.h>

#include "setup.h"
#include "BigInteger.h"

typedef struct Node {
    BINT data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} BINTQueue;

BINTQueue* createQueue(int loop);
void enqueue(BINTQueue* q, BINT data);
BINT dequeue(BINTQueue* q);
BINT peek(BINTQueue* q);
bool isempty(BINTQueue* q);
void freeQueue(BINTQueue* q);

#endif // BINT_FIFO_H