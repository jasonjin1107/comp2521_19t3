
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"
#define infinity 1000

int numWithin(Graph g, int src, int dist) {
	// TODO
	Queue newQueue = QueueNew();
	QueueEnqueue(newQueue, src);
	int *visited = calloc(GraphNumVertices(g), sizeof(int));
	int *distance = calloc(GraphNumVertices(g), sizeof(int));
	for (int i = 0; i < GraphNumVertices(g); i++) {
		if (i == src) {
			distance[i] = 0;
		} else {
			distance[i] = infinity;
		}
	}
	visited[src] = 1;
	while (QueueIsEmpty(newQueue) == false) {
		int dropped = QueueDequeue(newQueue);
		for (int i = 0 ; i < GraphNumVertices(g); i++) {
			if (GraphIsAdjacent(g, dropped, i)) {
				if (visited[i] == 0) {
					visited[i] = 1;
					QueueEnqueue(newQueue, i);
				} 
				if (distance[dropped] + 1 < distance[i]) {
					distance[i] = distance[dropped] + 1;
				}
			}
		}
	}
	int total = 0;
	for (int i = 0; i < GraphNumVertices(g); i++) {
		if (distance[i] <= dist) {
			total++;
		}
	}
	QueueFree(newQueue);
	free(visited);
	free(distance);
	return total;
}

