#pragma once
#include "coord.h"

struct queue {
	COORDINATE* data;
	int size;
};

typedef struct queue QUEUE;

QUEUE makeQueue();
int addToQueue(QUEUE* queue, int x, int y);
void print_queue(QUEUE* queue);
void delete_queue(QUEUE* queue);
COORDINATE getItem(QUEUE* queue);