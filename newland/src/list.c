/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/alloc.h>
#include <newland/list.h>
#include <errno.h>

size_t list_indexof(list_t* list, void* value) {
  list_node_t* node = NULL;
  size_t i = 0;
  SLIST_FOREACH(node, list, list) {
    if (node->value == value) return i;
    i++;
  }
  return -1;
}

size_t list_length(list_t* list) {
  list_node_t* node = NULL;
  size_t i = 0;
  SLIST_FOREACH(node, list, list) {
    i++;
  }
  return i;
}

void* list_get(list_t* list, size_t i) {
  list_node_t* node = NULL;
  size_t index = 0;
  SLIST_FOREACH(node, list, list) {
    if (index == i) return node->value;
    index++;
  }
  return NULL;
}

int list_add(list_t* list, void* value) {
  if (list_indexof(list, value) != -1) return -EEXIST;
  list_node_t* node = kmalloc(sizeof(list_node_t));
  if (node == NULL) return -ENOMEM;
  node->value = value;
  SLIST_INSERT_HEAD(list, node, list);
  return 0;
}

void list_remove(list_t* list, void* value) {
  list_node_t* node = NULL;
  SLIST_FOREACH(node, list, list) {
    if (node->value != value) continue;
    SLIST_REMOVE(list, node, list_node_t, list);
    kfree(node);
    break;
  }
}