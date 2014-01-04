#include<stdio.h>
#include<stdlib.h>

#include"quadbit.h"

void main()
{

	quadbit_t *quadbit;
	quadbit_init(&quadbit);

	quadbit_item_t *item = malloc(sizeof(quadbit_item_t));
	item->x = 0x8000000000000000 >> 9;
	item->y = 0x8000000000000000 >> 9;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 0x8000000000000000 >> 8;
	item->y = 0x8000000000000000 >> 6;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 0x8000000000000000 >> 45;
	item->y = 0x8000000000000000 >> 6;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 0x8000000000000000 >> 8;
	item->y =
	    (0x8000000000000000 >> 3) + (0x8000000000000000 >> 6) +
	    (0x8000000000000000 >> 7);
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 423456472;
	item->y = 7686724;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 9;
	item->y = 34;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 87426841;
	item->y = 742345;
	quadbit_insert(quadbit, item);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 0x8000000000000000 >> 45;
	item->y = 0x8000000000000000 >> 6;

	quadbit_item_t *sitem = quadbit_search(quadbit, item);

	printf("\nsearched_item: x = %llu , y = %llu\n\n", sitem->x, sitem->y);

	item = malloc(sizeof(quadbit_item_t));
	item->x = 0;
	item->y = 0x8000000000000000 >> 3;

	quadbit_node_t *parent;
	sitem = quadbit_search_set(quadbit, item, &parent, 3);

	if (parent) {
		quadbit_iter_t iter;

		sitem = quadbit_iter_first(&iter, parent);
		while (sitem) {
			printf("searched_item: x = %llu , y = %llu\n", sitem->x,
			       sitem->y);
			sitem = quadbit_iter_next(&iter);
		}

	} else {

		if (sitem) {
			printf("only_one_item: x = %llu , y = %llu\n", sitem->x,
			       sitem->y);

		}

	}

}
