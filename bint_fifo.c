#include "bint_fifo.h"
#include <stdlib.h>

BINTQueue* createQueue(int loop) {
    BINTQueue* q = (BINTQueue*)malloc(loop * sizeof(BINTQueue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(BINTQueue* q, BINT data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

BINT dequeue(BINTQueue* q) {
    if (isempty(q)) {
        // Returning a dummy value, in a real-world application you would handle this more gracefully
        BINT dummy = {false, 0, NULL};
        return dummy;
    }

    Node* temp = q->front;
    BINT data = temp->data;

    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
    return data;
}

BINT peek(BINTQueue* q) {
    if (isempty(q)) {
        // Returning a dummy value, in a real-world application you would handle this more gracefully
        BINT dummy = {false, 0, NULL};
        return dummy;
    }
    return q->front->data;
}

bool isempty(BINTQueue* q) {
    return q->front == NULL;
}

void freeQueue(BINTQueue* q) {
    while (!isempty(q)) {
        dequeue(q);
    }
    free(q);
}
