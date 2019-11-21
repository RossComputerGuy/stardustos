/**
	* StardustOS liblist - (C) 2019 Tristan Ross
	*/
#include <errno.h>
#include <liblist.h>
#include <stdlib.h>

size_t liblist_indexof(list_t* list, void* value) {
	list_node_t* node = NULL;
	size_t i = 0;
	SLIST_FOREACH(node, list, list) {
		if (node->value == value) return i;
		i++;
	}
	return -1;
}

size_t liblist_length(list_t* list) {
	list_node_t* node = NULL;
	size_t i = 0;
	SLIST_FOREACH(node, list, list) {
		i++;
	}
	return i;
}

void* liblist_get(list_t* list, size_t i) {
	list_node_t* node = NULL;
	size_t index = 0;
	SLIST_FOREACH(node, list, list) {
		if (index == i) return node->value;
		index++;
	}
	return NULL;
}

int liblist_add(list_t* list, void* value) {
	if (liblist_indexof(list, value) != -1) return -EEXIST;
	list_node_t* node = malloc(sizeof(list_node_t));
	if (node == NULL) return -ENOMEM;
	node->value = value;
	SLIST_INSERT_HEAD(list, node, list);
	return 0;
}

void liblist_remove(list_t* list, void* value) {
	list_node_t* node = NULL;
	SLIST_FOREACH(node, list, list) {
		if (node->value != value) continue;
		SLIST_REMOVE(list, node, list_node_t, list);
		free(node);
		break;
	}
}

void liblist_destroy(list_t* list) {
	list_node_t* node = SLIST_FIRST(list);
	list_node_t* next = node;
	while (next != NULL) {
		node = next;
		next = SLIST_NEXT(node, list);
		free(node);
	}
}