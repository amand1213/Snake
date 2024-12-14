#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

QUEUE makeQueue()
{
	QUEUE queue;
	queue.data = malloc(0 * sizeof(COORDINATE)); // Allocate initial memory
	queue.size = 0;
	return queue;
}

int addToQueue(QUEUE* queue, int x1, int y1)
{
	COORDINATE* temp = malloc((queue->size + 1) * sizeof(COORDINATE));
	if (!temp) {
		printf("Memory reallocation failed.\n");
		free(queue->data);
		return 1;
	}

	for (int i = 0; i < queue->size; i++)
	{
		temp[i] = queue->data[i];
	}

	COORDINATE coord;
	coord.x = x1;
	coord.y = y1;
	temp[queue->size] = coord;
	free(queue->data);
	queue->data = temp;
	queue->size = queue->size + 1;
	return 0;
}

COORDINATE getItem(QUEUE* queue) {
	COORDINATE coord;

	if(queue->size <= 1)
	{
		return;
	}

	COORDINATE* temp = malloc((queue->size - 1) * sizeof(COORDINATE));
	if (!temp) {
		printf("Memory reallocation failed.\n");
		free(queue->data);
		return;
	}

	for (int i = 1; i < queue->size; i++)
	{
		temp[i-1] = queue->data[i];
	}
	coord = queue->data[0];
	free(queue->data);
	queue->data = temp;
	queue->size = queue->size - 1;
	return coord;
}

int xyInQueue(QUEUE queue, int x, int y)
{
	COORDINATE coord;
	coord.x = x;
	coord.y = y;

	return coordInQueue(&queue, coord);
}

int coordInQueue(QUEUE* queue, COORDINATE coord) {
	int x = coord.x;
	int y = coord.y;

	for (int i = 0; i < queue->size; i++) {
		int curX = queue->data[i].x;
		int curY = queue->data[i].y;

		if (x == curX && y == curY) {
			return 1;
		}
	}
	return 0;
}

void removeItem(QUEUE* queue, COORDINATE item) 
{
	if (queue->size < 1)
	{
		return;
	}

	COORDINATE* temp = malloc((queue->size - 1) * sizeof(COORDINATE));
	if (!temp) {
		printf("Memory reallocation failed.\n");
		free(queue->data);
		return;
	}

	for (int i = 0; i < queue->size-1; i++)
	{
		if (item.x == queue->data[i].x && item.y == queue->data[i].y) \
		{
			i++;
		}
		temp[i] = queue->data[i];
	}
	free(queue->data);
	queue->data = temp;
	queue->size = queue->size - 1;
}

void print_queue(QUEUE* queue)
{
	printf("[");
	for (int i = 0; i < queue->size; i++)
	{
		printf("(%d,%d)", queue->data[i].x, queue->data[i].y);
		if (i < queue->size-1) {
			printf(",");
		}
	}
	printf("]");
}

void delete_queue(QUEUE* queue)
{
	free(queue->data);
}