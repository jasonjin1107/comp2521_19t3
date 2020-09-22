
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Stack.h"

bool hasCycle(Graph g) {
	// TODO
	Stack newStack = StackNew();
	int *visited = calloc(GraphNumVertices(g), sizeof(int));
	int *appear = calloc(GraphNumVertices(g), sizeof(int));
	visited[0] = 1;
	StackPush(newStack, 0);
	while (StackIsEmpty(newStack) == false) {
		int dropped = StackPop(newStack);
		appear[dropped]++;
		for (int i = 0; i < GraphNumVertices(g); i++) {
			if (GraphIsAdjacent(g, i, dropped)) {
				if (visited[i] == 0) {
					visited[i] = 1;
					StackPush(newStack, i);
				}
				

			}
		}
	}

	for (int i =0; i < GraphNumVertices(g); i++) {
		printf("%d ", appear[i]);
		if (appear[i] > 1) {
			return true;
		}
	}
	return false;
}

