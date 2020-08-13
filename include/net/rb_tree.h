#ifndef RB_TREE_H
# define RB_TREE_H

# ifdef __cplusplus
extern "C" {
# endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DUPLICATE_DROP_TABLE_SIZE                   20

typedef struct {
  uint8_t mac_addr[6];
  uint16_t sequence_number;
} net_data;

typedef struct {
  net_data value;
  uint32_t time;
  uint32_t status;
} rb_tree_node_data_t;

typedef struct rb_tree_node
{
  rb_tree_node_data_t data;

  uint32_t black_index;
  struct rb_tree_node* parent;
  struct rb_tree_node* left;
  struct rb_tree_node* right;
} rb_tree_node_t;

typedef struct {
  size_t next_free_node;
  rb_tree_node_t rb_tree_nodes[DUPLICATE_DROP_TABLE_SIZE];

  rb_tree_node_t root;
} rb_tree_t;

void rb_tree_init(rb_tree_t* rb_tree);

rb_tree_node_t* get_or_create_rb_tree_node(rb_tree_t* rb_tree, rb_tree_node_data_t* data, bool* is_new);

# ifdef __cplusplus
}
# endif

#endif
