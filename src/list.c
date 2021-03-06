/*
 * Copyright (c) 2017-2018 Bailey Thompson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include "list.h"

struct _list {
    size_t data_size;
    int space;
    struct node *head;
    struct node *tail;
};

struct node {
    struct node *prev;
    void *data;
    struct node *next;
};

/**
 * Initializes a doubly-linked list.
 *
 * @param data_size The size of data to store.
 *
 * @return The doubly-linked list, or NULL if memory could not be allocated.
 */
list list_init(const size_t data_size)
{
    struct _list *const init = malloc(sizeof(struct _list));
    if (!init) {
        return NULL;
    }
    init->data_size = data_size;
    init->space = 0;
    init->head = NULL;
    init->tail = NULL;
    return init;
}

/**
 * Gets the amount of elements in the doubly-linked list.
 *
 * @param me The doubly-linked list to check.
 *
 * @return The amount of elements.
 */
int list_size(list me)
{
    return me->space;
}

/**
 * Determines if the doubly-linked list is empty.
 *
 * @param me The doubly-linked list to check.
 *
 * @return If the list is empty.
 */
bool list_is_empty(list me)
{
    return list_size(me) == 0;
}

/**
 * Copies the nodes of the doubly-linked list to an array.
 *
 * @param arr The array to copy the list to.
 * @param me  The list to copy to the array.
 */
void list_copy_to_array(void *const arr, list me)
{
    struct node *traverse = me->head;
    int offset = 0;
    while (traverse) {
        memcpy(arr + offset, traverse->data, me->data_size);
        offset += me->data_size;
        traverse = traverse->next;
    }
}

/*
 * Get the node at index starting from the head.
 */
static struct node *list_get_node_from_head(list me, const int index)
{
    struct node *traverse = me->head;
    for (int i = 0; i < index; i++) {
        traverse = traverse->next;
    }
    return traverse;
}

/*
 * Get the node at index starting from tail.
 */
static struct node *list_get_node_from_tail(list me, const int index)
{
    struct node *traverse = me->tail;
    for (int i = me->space - 1; i > index; i--) {
        traverse = traverse->prev;
    }
    return traverse;
}

/*
 * Get the node at the specified index.
 */
static struct node *list_get_node_at(list me, const int index)
{
    if (index <= me->space / 2) {
        return list_get_node_from_head(me, index);
    } else {
        return list_get_node_from_tail(me, index);
    }
}

/**
 * Adds data at the first index in the doubly-linked list.
 *
 * @param me   The list to add data to.
 * @param data The data to add to the list.
 *
 * @return 0       No error.
 *         -ENOMEM Out of memory.
 */
int list_add_first(list me, void *const data)
{
    struct node *const traverse = me->head;
    struct node *const add = malloc(sizeof(struct node));
    if (!add) {
        return -ENOMEM;
    }
    add->data = malloc(me->data_size);
    if (!add->data) {
        free(add);
        return -ENOMEM;
    }
    add->prev = NULL;
    memcpy(add->data, data, me->data_size);
    add->next = traverse;
    if (traverse) {
        traverse->prev = add;
    }
    me->head = add;
    if (!me->tail) {
        me->tail = traverse;
    }
    me->space++;
    return 0;
}

/**
 * Adds data at a specified index in the doubly-linked list.
 *
 * @param me    The list to add data to.
 * @param index The index to add the data at.
 * @param data  The data to add to the list.
 *
 * @return 0       No error.
 *         -ENOMEM Out of memory.
 *         -EINVAL Invalid argument.
 */
int list_add_at(list me, const int index, void *const data)
{
    if (index < 0 || index > me->space) {
        return -EINVAL;
    }
    if (index == 0) {
        return list_add_first(me, data);
    }
    if (index == me->space) {
        return list_add_last(me, data);
    }
    // The new node will go right before this node.
    struct node *const traverse = list_get_node_at(me, index);
    struct node *const add = malloc(sizeof(struct node));
    if (!add) {
        return -ENOMEM;
    }
    add->data = malloc(me->data_size);
    if (!add->data) {
        free(add);
        return -ENOMEM;
    }
    add->prev = traverse->prev;
    memcpy(add->data, data, me->data_size);
    add->next = traverse;
    traverse->prev->next = add;
    traverse->prev = add;
    me->space++;
    return 0;
}

/**
 * Adds data at the last index in the doubly-linked list.
 *
 * @param me   The list to add data to.
 * @param data The data to add to the list.
 *
 * @return 0       No error.
 *         -ENOMEM Out of memory.
 */
int list_add_last(list me, void *const data)
{
    struct node *const traverse = me->tail;
    struct node *const add = malloc(sizeof(struct node));
    if (!add) {
        return -ENOMEM;
    }
    add->data = malloc(me->data_size);
    if (!add->data) {
        free(add);
        return -ENOMEM;
    }
    add->prev = traverse;
    memcpy(add->data, data, me->data_size);
    add->next = NULL;
    if (traverse) {
        traverse->next = add;
    }
    me->tail = add;
    me->space++;
    return 0;
}

/*
 * Determines if the input is illegal.
 */
static bool list_is_illegal_input(list me, const int index)
{
    return index < 0 || index >= me->space || me->space == 0;
}

/**
 * Removes the first piece of data from the doubly-linked list.
 *
 * @param me The list to remove data from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_remove_first(list me)
{
    return list_remove_at(me, 0);
}

/**
 * Removes data from the doubly-linked list at the specified index.
 *
 * @param me    The list to remove data from.
 * @param index The index to remove from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_remove_at(list me, const int index)
{
    if (list_is_illegal_input(me, index)) {
        return -EINVAL;
    }
    struct node *const traverse = list_get_node_at(me, index);
    if (index == 0) {
        traverse->next->prev = NULL;
        me->head = traverse->next;
    } else if (index == me->space - 1) {
        traverse->prev->next = NULL;
        me->tail = traverse->prev;
    } else {
        traverse->prev->next = traverse->next;
        traverse->next->prev = traverse->prev;
    }
    free(traverse->data);
    free(traverse);
    me->space--;
    return 0;
}

/**
 * Removes the last piece of data from the doubly-linked list.
 *
 * @param me The list to remove data from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_remove_last(list me)
{
    return list_remove_at(me, me->space - 1);
}

/**
 * Sets the data at the first index in the doubly-linked list.
 *
 * @param me   The list to set data for.
 * @param data The data to set in the list.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_set_first(list me, void *const data)
{
    return list_set_at(me, 0, data);
}

/**
 * Sets the data at the specified index in the doubly-linked list.
 *
 * @param me    The list to set data for.
 * @param index The index to set data in the list.
 * @param data  The data to set in the list.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_set_at(list me, const int index, void *const data)
{
    if (list_is_illegal_input(me, index)) {
        return -EINVAL;
    }
    struct node *const traverse = list_get_node_at(me, index);
    memcpy(traverse->data, data, me->data_size);
    return 0;
}

/**
 * Sets the data at the last index in the doubly-linked list.
 *
 * @param me   The list to set data for.
 * @param data The data to set in the list.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_set_last(list me, void *const data)
{
    return list_set_at(me, me->space - 1, data);
}

/**
 * Gets the data at the first index in the doubly-linked list.
 *
 * @param data The data to get.
 * @param me   The list to get data from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_get_first(void *const data, list me)
{
    return list_get_at(data, me, 0);
}

/**
 * Gets the data at the specified index in the doubly-linked list.
 *
 * @param data  The data to get.
 * @param me    The list to get data from.
 * @param index The index to get data from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_get_at(void *const data, list me, const int index)
{
    if (list_is_illegal_input(me, index)) {
        return -EINVAL;
    }
    struct node *const traverse = list_get_node_at(me, index);
    memcpy(data, traverse->data, me->data_size);
    return 0;
}

/**
 * Gets the data at the last index in the doubly-linked list.
 *
 * @param data The data to get.
 * @param me   The list to get data from.
 *
 * @return 0       No error.
 *         -EINVAL Invalid argument.
 */
int list_get_last(void *const data, list me)
{
    return list_get_at(data, me, me->space - 1);
}

/**
 * Clears all elements from the doubly-linked list.
 *
 * @param me The list to clear.
 */
void list_clear(list me)
{
    struct node *traverse = me->head;
    while (traverse) {
        struct node *const temp = traverse;
        traverse = traverse->next;
        free(temp->data);
        free(temp);
    }
    me->head = NULL;
    me->space = 0;
    me->tail = NULL;
}

/**
 * Destroys the doubly-linked list.
 *
 * @param me The list to destroy.
 *
 * @return NULL
 */
list list_destroy(list me)
{
    list_clear(me);
    free(me);
    return NULL;
}
