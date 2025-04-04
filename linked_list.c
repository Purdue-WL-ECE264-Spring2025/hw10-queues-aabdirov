#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    node->value = value;
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    node->value = value;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        struct list_node *cur = list->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
}

size_t remove_from_head(struct linked_list *list) {
    assert(list->head != NULL);

    struct list_node *temp = list->head;
    size_t val = temp->value;

    list->head = list->head->next;
    free(temp);
    return val;
}

size_t remove_from_tail(struct linked_list *list) {
    assert(list->head != NULL);

    struct list_node *cur = list->head;
    if (cur->next == NULL) {
        size_t val = cur->value;
        free(cur);
        list->head = NULL;
        return val;
    }

    while (cur->next->next != NULL) {
        cur = cur->next;
    }

    size_t val = cur->next->value;
    free(cur->next);
    cur->next = NULL;
    return val;
}

void dump_list(FILE *fp, struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur != NULL) {
        fprintf(fp, "%zu -> ", cur->value);
        cur = cur->next;
    }
    fprintf(fp, "NULL\n");
}

void free_list(struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur != NULL) {
        struct list_node *next = cur->next;
        free(cur);
        cur = next;
    }
}
