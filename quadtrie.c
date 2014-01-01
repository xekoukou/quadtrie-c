#include<stdint.h>
//Assumptions : quadbits are memory efficient
//            : they are fast at searching subsets

#define bucket_size 64

struct quadbit_node_t
{

  uint8_t pos;
  uint8_t which;		//used for many things:
// first bit shows whether the different bit is in the x(0) or y(1) position
//the second and third show whether the child[0],child[1] are nodes or items  

  void *child[2];		//0 for 0 bit 1 for 1 bit
}

typedef struct quadbit_node_t quadbit_node_t;

struct quadbit_item_t
{
  uint64_t x;
  uint64_t y;
  void *pointer;
}

struct quadbit_t
{

  quabbit_node_t *bucket;	//this is used to minimize the number of mallocs
  uint8_t bucket_pos;
  uint8_t is_root_item;
  quadbit_node_t *root;
}

typedef struct quadbit_t quadbit_t;

inline void
quadbit_new_bucket (quadbit_t * quadbit)
{

  quadbit->bucket = malloc (sizeof (quadbit_node_t) * bucket_size);
  quadbit->bucket_pos = 0;
}

uint8_t
bit_diff_pos (uint64_t a, uint64_t b)
{

  uint64_t c = a ^ b;
  uint8_t result = 0;
  while (c)
    {
      c = c >> 1;
      result++;
    }
  return result;
}

void
quadbit_init (quadbit_t ** quadbit)
{
  *quadbit = malloc (sizeof (quadbit_t));
  quadbit_new_bucket (*quadbit);
  (*quadbit)->bucket_pos = 0;

  (*quadbit)->root = NULL;
}

quadbit_node_t *
quaddbit_new_node (quadbit_t * quadbit)
{

  quadbit_node_t *result = quadbit->bucket + quadbit->bucket_pos;
  quadbit->bucket_pos++;
  if (quadbit->bucket_pos = bucket_size)
    {
      quadbit_new_bucket (quadbit);
    }

  return result;
}

int
quadbit_insert (quadbit_t * quadbit, quadbit_item_t * item)
{

  if (quadbit->root == NULL)
    {

      quadbit->is_root_item = 1;
      quadbit->root = item;

    }
  else
    {

      if (quadbit->is_root_item)
	{

	  uint8_t posX = bit_diff_pos (quadbit->root->x, item->x);
	  uint8_t posY = bit_diff_pos (quadbit->root->y, item->y);

	  quadbit_node_t *node = quadbit_new_node (quadbit);

	  if (posX < posY)
	    {
	      quadbit->which = 64 + 32;

	      uint64_t mask = 1 << posX;

	      if (item->x & mask)
		{

		  node->child[1] = item;
		  node->child[0] = quadbit->root;

		}
	      else
		{
		  node->child[0] = item;
		  node->child[1] = quadbit->root;

		}

	    }
	  else
	    {
	      quadbit->which = 128 + 64 + 32;

	      uint64_t mask = 1 << posY;

	      if (item->y & mask)
		{

		  node->child[1] = item;
		  node->child[0] = quadbit->root;

		}
	      else
		{
		  node->child[0] = item;
		  node->child[1] = quadbit->root;

		}

	    }

	  quadbit->root = node;
	  quadbit->is_root_item = 0;

	}
      else
	{

	}

    }

}
