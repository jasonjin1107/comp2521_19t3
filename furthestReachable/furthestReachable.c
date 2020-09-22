
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"

#define infinity 1000

void assignDist(int number, int *src) {
	for (int j = 0; j < number; j++) {
		src[j] = infinity;
	}
}

int findMaxIndex(int *dest, Graph g, int source) {
	int max = dest[source];
	for (int i = 0; i < GraphNumVertices(g); i++) {
		if (dest[i] > max && dest[i] != infinity) {
			max = dest[i];
		}	
	}
	for (int k = GraphNumVertices(g) - 1; k >= 0; k--) {
		if (dest[k] == max) {
			return k;
		}
	}
	return 0;	
}

int furthestReachable(Graph g, int src) {
	// TODO
	int *visited = calloc(GraphNumVertices(g), sizeof(int));
	int *dest = calloc(GraphNumVertices(g), sizeof(int));
	assignDist(GraphNumVertices(g), dest);
	dest[src] = 0;
	visited[src] = 1;
	Queue newQueue = QueueNew();
	QueueEnqueue(newQueue, src);
	while (!QueueIsEmpty(newQueue)) {
		int dropped = QueueDequeue(newQueue);
		for (int i = 0; i < GraphNumVertices(g); i++) {
			if (visited[i] == 0 && GraphIsAdjacent(g, i, dropped)) {
				if (1 + dest[dropped] < dest[i]) {
					dest[i] = 1 + dest[dropped];
				}
				visited[i] = 1;
				QueueEnqueue(newQueue, i);
			}
		}
	}
	int index = findMaxIndex(dest, g, src);
	free(visited);
	free(dest);
	QueueFree(newQueue);
	return index;
}

