#ifndef QUADBIT_H
#define QUADBIT_H

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#include"quadbit_internal.h"

//Assumptions : quadbits are memory efficient
//            : they are fast at searching subsets

//intrusive data structure
struct quadbit_item_t {
	int64_t x;
	int64_t y;
// here you put your data and then cast your data Structure as quabit_item_t
};

void quadbit_init(quadbit_t ** quadbit);

void quadbit_insert(quadbit_t * quadbit, quadbit_item_t * item);

quadbit_item_t *quadbit_search(quadbit_t * quadbit, quadbit_item_t * item);

//pos here is the resolution of the search
// an item is returned if it is the only item in the set.
// In that case parent is NULL.
// pos is from 0 bit to 63bit

quadbit_item_t *quadbit_search_set(quadbit_t * quadbit,
				   quadbit_item_t * item,
				   quadbit_node_t ** parent, int8_t pos);

//TODO memory from the removed node is not freed. It becomes unusuable
////returns the removed quadbit_item
void *quadbit_remove(quadbit_t * quadbit, quadbit_item_t * item);

//no allocation happens, so as to be reusable.
quadbit_item_t *quadbit_iter_first(quadbit_iter_t * iter,
				   quadbit_node_t * parent);
				   //Indicates the end by sending NULL
quadbit_item_t *quadbit_iter_next(quadbit_iter_t * iter);

void print_int64_in_binary(int64_t numb);

void quadbit_print(quadbit_t * quadbit);

void print_item(quadbit_item_t * item);
void print_node(quadbit_node_t * node);

#endif
