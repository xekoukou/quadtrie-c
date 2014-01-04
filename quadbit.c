#include<stdint.h>
#include"quadbit_internal.h"

inline void quadbit_new_bucket(quadbit_t * quadbit)
{

	quadbit->bucket = malloc(sizeof(quadbit_node_t) * bucket_size);
	quadbit->bucket_pos = 0;
}

uint8_t bit_diff_pos(uint64_t a, uint64_t b)
{

	uint64_t c = a ^ b;
	uint8_t result = 0;
	while (c) {
		c = c >> 1;
		result++;
	}
	return 64 - result;
}

void quadbit_init(quadbit_t ** quadbit)
{
	*quadbit = malloc(sizeof(quadbit_t));
	quadbit_new_bucket(*quadbit);
	(*quadbit)->bucket_pos = 0;

	(*quadbit)->root = NULL;
}

quadbit_node_t *quadbit_new_node(quadbit_t * quadbit)
{

	quadbit_node_t *result = quadbit->bucket + quadbit->bucket_pos;
	quadbit->bucket_pos++;
	if (quadbit->bucket_pos == bucket_size) {
		quadbit_new_bucket(quadbit);
	}

	return result;
}

void quadbit_insert(quadbit_t * quadbit, quadbit_item_t * item)
{

	if (quadbit->root == NULL) {

		quadbit->is_root_item = 1;
		quadbit->root = item;
		return;
	}

	if (quadbit->is_root_item) {
		quadbit_item_t *c = quadbit->root;

		uint8_t posX = bit_diff_pos(c->x, item->x);
		uint8_t posY = bit_diff_pos(c->y, item->y);

		quadbit_node_t *node = quadbit_new_node(quadbit);

		if (posX < posY) {
			node->pos = posX;
			node->which = 64 + 32;

			uint64_t mask = 0x8000000000000000 >> posX;

			if (item->x & mask) {

				node->child[1] = item;
				node->child[0] = c;

			} else {
				node->child[0] = item;
				node->child[1] = c;

			}

		} else {
			node->pos = posY;
			node->which = 128 + 64 + 32;

			uint64_t mask = 0x8000000000000000 >> posY;

			if (item->y & mask) {

				node->child[1] = item;
				node->child[0] = c;

			} else {
				node->child[0] = item;
				node->child[1] = c;

			}

		}

		quadbit->root = node;
		quadbit->is_root_item = 0;
		return;
	}

	quadbit_node_t *p = quadbit->root;
	quadbit_item_t *c;
	uint8_t is_item;
	uint8_t which;

	while (1) {
		if (p->which & 128) {
			if (item->y & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;
			}
		} else {
			if (item->x & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;

			}
		}
		c = p->child[which];
		if (p->which & (32 * (which + 1))) {
			is_item = 1;
		} else {
			is_item = 0;
		}

		if (!is_item) {
			p = (quadbit_node_t *) c;
		} else {
			uint8_t posX = bit_diff_pos(c->x,
						    item->x);
			uint8_t posY = bit_diff_pos(c->y,
						    item->y);

			if (p->pos > ((posX < posY) ? posX : posY)) {

				quadbit_node_t *node =
				    quadbit_new_node(quadbit);

				if (posX < posY) {
					node->pos = posX;

					uint64_t mask =
					    0x8000000000000000 >> posX;

					if (item->x & mask) {

						node->which = 64;

						node->child[1]
						    = item;
						node->child[0]
						    =
						    quadbit_insert_search_set
						    (quadbit, item, node, posX);

					} else {
						node->which = 32;
						node->child[0]
						    = item;
						node->child[1]
						    =
						    quadbit_insert_search_set
						    (quadbit, item, node, posX);

					}

				} else {
					node->pos = posY;
					uint64_t mask =
					    0x8000000000000000 >> posY;

					if (item->y & mask) {
						node->which = 128 + 64;

						node->child[1]
						    = item;
						node->child[0]
						    =
						    quadbit_insert_search_set
						    (quadbit, item, node, posY);

					} else {
						node->which = 128 + 32;
						node->child[0]
						    = item;
						node->child[1]
						    =
						    quadbit_insert_search_set
						    (quadbit, item, node, posY);

					}
				}

				break;

			} else {

				quadbit_node_t *node =
				    quadbit_new_node(quadbit);

				if (posX < posY) {
					node->pos = posX;
					node->which = 64 + 32;

					uint64_t mask =
					    0x8000000000000000 >> posX;

					if (item->x & mask) {

						node->child[1]
						    = item;
						node->child[0]
						    = c;

					} else {
						node->child[0]
						    = item;
						node->child[1]
						    = c;

					}

				} else {
					node->pos = posY;
					node->which = 128 + 64 + 32;

					uint64_t mask =
					    0x8000000000000000 >> posY;

					if (item->y & mask) {

						node->child[1]
						    = item;
						node->child[0]
						    = c;

					} else {
						node->child[0]
						    = item;
						node->child[1]
						    = c;

					}

				}

				p->child[which] = node;
				//tell the parent that it has a node not an item
				p->which = p->which & (~(32 * (which + 1)));
				break;
			}
		}

	}

}

quadbit_item_t *quadbit_search(quadbit_t * quadbit, quadbit_item_t * item)
{

	if (quadbit->root == NULL) {

		return NULL;
	}

	if (quadbit->is_root_item) {
		quadbit_item_t *c = quadbit->root;

		uint8_t posX = bit_diff_pos(c->x, item->x);
		uint8_t posY = bit_diff_pos(c->y, item->y);

		if (((posX == 64) && (posY == 64))) {
			return quadbit->root;
		} else {
			return NULL;
		}

	}
	quadbit_node_t *p = quadbit->root;
	quadbit_item_t *c;
	uint8_t is_item;
	uint8_t which;

	while (1) {
		if (p->which & 128) {
			if (item->y & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;
			}
		} else {
			if (item->x & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;

			}
		}
		c = p->child[which];
		if (p->which & (32 * (which + 1))) {
			is_item = 1;
		} else {
			is_item = 0;
		}
		if (!is_item) {
			p = (quadbit_node_t *) c;
		} else {

			uint8_t posX = bit_diff_pos(c->x, item->x);
			uint8_t posY = bit_diff_pos(c->y, item->y);

			if (((posX == 64) && (posY == 64))) {
				return c;
			} else {
				return NULL;
			}

		}
	}
}

//pos here is the resolution of the search
// an item is returned if it is the only item in the set.
// In that case parent is NULL.

quadbit_item_t *quadbit_search_set(quadbit_t * quadbit, quadbit_item_t * item,
				   quadbit_node_t ** parent, uint8_t pos)
{
	*parent = NULL;

	if (quadbit->root == NULL) {

		*parent = NULL;
		return NULL;
	}

	if (quadbit->is_root_item) {
		quadbit_item_t *c = quadbit->root;

		uint8_t posX = bit_diff_pos(c->x, item->x);
		uint8_t posY = bit_diff_pos(c->y, item->y);

		if ((posX > pos) && (posY > pos)) {
			return quadbit->root;
		} else {
			return NULL;
		}

	}

	quadbit_node_t *p = quadbit->root;
	quadbit_item_t *c;
	uint8_t is_item;
	uint8_t which;

	uint8_t once = 1;
	// needs to check at least one item so as to make sure this is the correnct set
	if (p->pos > pos) {
		*parent = quadbit->root;
		once = 0;
		//TODO find the first item on the bottom
	}
	while (1) {
		if (p->which & 128) {
			if (item->y & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;
			}
		} else {
			if (item->x & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;

			}
		}
		c = p->child[which];
		if (p->which & (32 * (which + 1))) {
			is_item = 1;
		} else {
			is_item = 0;
		}
		if (!is_item) {
			p = (quadbit_node_t *) c;

			if (once && (((quadbit_node_t *) p)->pos > pos)) {
				*parent = p;
				once = 0;
			}

		} else {

			uint8_t posX = bit_diff_pos(c->x, item->x);
			uint8_t posY = bit_diff_pos(c->y, item->y);

			if ((posX > pos) && (posY > pos)) {
				if (*parent) {
					return NULL;
				} else {
					return c;
				}
			} else {
				return NULL;
			}

		}
	}
}

//returns the child of the inserted node
//node is the inserted node
inline quadbit_node_t *quadbit_insert_search_set(quadbit_t * quadbit,
						 quadbit_item_t * item,
						 quadbit_node_t * node,
						 uint8_t pos)
{

	//we already know that there is a node and that there is an item that is part of of the set

	quadbit_node_t *p = quadbit->root;
	quadbit_node_t *c;
	uint8_t is_item;
	uint8_t which;

	if (p->pos >= pos) {
		quadbit->root = node;
		return p;
	}

	while (1) {
		if (p->which & 128) {
			if (item->y & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;
			}
		} else {
			if (item->x & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;

			}
		}
		c = p->child[which];

		//we will never reach an item.

		if ((c)->pos >= pos) {
			p->child[which] = node;
			return c;

		}
		p = (quadbit_node_t *) c;

	}
}

//TODO memory from the removed node is not freed. It becomes unusuable
//returns the removed quadbit_item
void *quadbit_remove(quadbit_t * quadbit, quadbit_item_t * item)
{

	if (quadbit->root == NULL) {

		return NULL;
	}

	if (quadbit->is_root_item) {
		quadbit_item_t *c = quadbit->root;

		uint8_t posX = bit_diff_pos(c->x, item->x);
		uint8_t posY = bit_diff_pos(c->y, item->y);

		if (((posX == 64) && (posY == 64))) {
			quadbit->root = NULL;
			return c;
		} else {
			return NULL;
		}

	}
	quadbit_node_t *p = quadbit->root;

	quadbit_item_t *c;
	uint8_t is_item;
	uint8_t which;

	//granpa is quadbit?
	quadbit_node_t *gpp;
	uint8_t gwhich;

	if (p->which & 128) {
		if (item->y & (0x8000000000000000 >> p->pos)) {
			which = 1;
		} else {
			which = 0;
		}
	} else {
		if (item->x & (0x8000000000000000 >> p->pos)) {
			which = 1;
		} else {
			which = 0;

		}
	}
	c = p->child[which];
	if (p->which & (32 * (which + 1))) {
		is_item = 1;
	} else {
		is_item = 0;
	}
	if (!is_item) {
		gpp = p;
		gwhich = which;
		p = (quadbit_node_t *) c;

	} else {

		uint8_t posX = bit_diff_pos(c->x, item->x);
		uint8_t posY = bit_diff_pos(c->y, item->y);

		if (((posX == 64) && (posY == 64))) {
			quadbit->is_root_item = 1;
			quadbit->root = p->child[!which];
			return c;
		} else {
			return NULL;
		}

	}

	while (1) {
		if (p->which & 128) {
			if (item->y & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;
			}
		} else {
			if (item->x & (0x8000000000000000 >> p->pos)) {
				which = 1;
			} else {
				which = 0;

			}
		}
		c = p->child[which];
		if (p->which & (32 * (which + 1))) {
			is_item = 1;
		} else {
			is_item = 0;
		}
		if (!is_item) {
			gpp = p;
			gwhich = which;
			p = (quadbit_node_t *) c;
		} else {

			uint8_t posX = bit_diff_pos(c->x, item->x);
			uint8_t posY = bit_diff_pos(c->y, item->y);

			if (((posX == 64) && (posY == 64))) {
				gpp->child[gwhich] = p->child[!which];
				if (p->which & (32 * ((!which) + 1))) {
					is_item = 1;
				} else {
					is_item = 0;
				}

				uint8_t cmask = 32 * ((!gwhich) + 1);
				gpp->which = gpp->which + (cmask * is_item);

				return c;
			} else {
				return NULL;
			}

		}
	}

}

//no allocation happens, so as to be reusable.
quadbit_item_t *quadbit_iter_first(quadbit_iter_t * iter,
				   quadbit_node_t * parent)
{

	iter->i[0] = parent;
	iter->level = 0;
	iter->flip[0] = 0;

	uint8_t i = 0;
	while (1) {

		if (iter->i[i]->which & (32 * (iter->flip[i] + 1))) {
			iter->level = i;
			return iter->i[i]->child[iter->flip[i]];
		} else {
			iter->i[i + 1] = iter->i[i]->child[iter->flip[i]];
			iter->flip[i + 1] = 0;
			i++;
		}

	}
}

quadbit_item_t *quadbit_iter_next(quadbit_iter_t * iter)
{

	while (iter->flip[iter->level] == 1) {

		if (iter->level == 0) {
			return NULL;
		}

		iter->level--;
	}

	iter->flip[iter->level] = 1;

	uint8_t i = iter->level;
	while (1) {

		if (iter->i[i]->which & (32 * (iter->flip[i] + 1))) {
			iter->level = i;
			return iter->i[i]->child[iter->flip[i]];
		} else {
			iter->i[i + 1] = iter->i[i]->child[iter->flip[i]];
			iter->flip[i + 1] = 0;
			i++;
		}

	}

}
