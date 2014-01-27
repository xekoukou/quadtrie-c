
#ifndef QUADBIT_INTERVAL_H
#define QUADBIT_INTERVAL_H

#include<stdint.h>
#include<stdlib.h>

//Assumptions : quadbits are memory efficient
//            : they are fast at searching subsets

#define bucket_size 64

struct quadbit_node_t {

	int8_t pos;
	uint8_t which;		//used for many things:
// first bit shows whether the different bit is in the x(0) or y(1) position
//the second and third show whether the child[1],child[0] are nodes or items  

	void *child[2];		//0 for 0 bit 1 for 1 bit
};

typedef struct quadbit_node_t quadbit_node_t;

typedef struct quadbit_item_t quadbit_item_t;

struct quadbit_t {

	quadbit_node_t *bucket;	//this is used to minimize the number of mallocs
	uint8_t bucket_pos;
	uint8_t is_root_item;
	void *root;
};
typedef struct quadbit_t quadbit_t;

struct quadbit_iter_t {

	quadbit_node_t *i[64];
	uint8_t level;
	uint8_t flip[64];
};
typedef struct quadbit_iter_t quadbit_iter_t;

inline void quadbit_new_bucket(quadbit_t * quadbit);

uint8_t bit_diff_pos(int64_t a, int64_t b);

quadbit_node_t *quadbit_new_node(quadbit_t * quadbit);

#include"quadbit.h"

inline quadbit_node_t *quadbit_insert_search_set(quadbit_t * quadbit,
						 quadbit_item_t * item,
						 quadbit_node_t * node,
						 int8_t pos);

#endif
