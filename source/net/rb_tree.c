#include "rb_tree.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void rotate_left(rb_tree_node_t** root_ptr) {
  rb_tree_node_t* root = *root_ptr;

  rb_tree_node_t* new_root = root->right;

  root->right = new_root->left;
  new_root->left = root;

  if (root->right)
    root->right->parent = root;

  new_root->parent = root->parent;
  root->parent = new_root;

  *root_ptr = new_root;
}

static void rotate_right(rb_tree_node_t** root_ptr) {
  rb_tree_node_t* root = *root_ptr;
  rb_tree_node_t* new_root = root->left;

  root->left = new_root->right;
  new_root->right = root;

  if (root->left)
    root->left->parent = root;

  new_root->parent = root->parent;
  root->parent = new_root;

  *root_ptr = new_root;
}

static void fix_insert_violation(rb_tree_node_t** node_ptr) {
  rb_tree_node_t* node = *node_ptr;
  rb_tree_node_t* parent = node->parent;
  bool is_root = (NULL == parent->parent);
  if (is_root)
  {
    node->black_index = 1;
  }
  else if (0 == parent->black_index)
  {
    bool on_parents_left = (parent->left == node);
    rb_tree_node_t* grand_parent = parent->parent;
    bool on_grand_parents_left = (grand_parent->left == parent);
    rb_tree_node_t* uncle = (on_grand_parents_left)? grand_parent->right: grand_parent->left;

    rb_tree_node_t* ancestor = grand_parent->parent;
    bool on_ancestors_left = (ancestor->left == grand_parent);
    rb_tree_node_t** grand_parent_ptr = (on_ancestors_left? &(ancestor->left): &(ancestor->right));

    if (NULL == uncle || 0 < uncle->black_index) {
      rb_tree_node_t** parent_ptr = (on_grand_parents_left? &(grand_parent->left): &(grand_parent->right));

      if (on_grand_parents_left && !on_parents_left)
        rotate_left(parent_ptr);
      if (!on_grand_parents_left && on_parents_left)
        rotate_right(parent_ptr);
      (*parent_ptr)->black_index = 1;

      if (on_grand_parents_left)
        rotate_right(grand_parent_ptr);
      else
        rotate_left(grand_parent_ptr);
      grand_parent->black_index = 0;

    }
    else
    {
      parent->black_index = 1;
      uncle->black_index = 1;
      grand_parent->black_index = 0;
      fix_insert_violation(grand_parent_ptr);
    }
  }
}

static void replace_successor(rb_tree_node_t* node, rb_tree_node_t* successor) {

  rb_tree_node_t* next = successor->right;
  if (successor->parent->left == successor)
    successor->parent->left = next;
  else
    successor->parent->right = next;
  if (next) 
    next->parent = successor->parent;

  rb_tree_node_t* node_left = node->left;
  rb_tree_node_t* node_right = node->right;
  successor->left = node_left;
  if (node_left)
    node_left->parent = successor;
  successor->right = node_right;
  if (node_right)
    node_right->parent = successor;


  rb_tree_node_t* node_parent = node->parent;
  if (node_parent->right == node)
    node_parent->right = successor;
  else 
    node_parent->left = successor;

  successor->parent = node_parent;
  successor->black_index = node->black_index;
}

static void replace_child(rb_tree_node_t* node, rb_tree_node_t* child) {
  rb_tree_node_t* node_parent = node->parent;
  if (node_parent->left == node)
    node_parent->left = child;
  else
    node_parent->right = child;
  child->parent = node_parent;
  if (child->black_index == 0 && node_parent->black_index == 0)
    child->black_index = 1;
  else
    child->black_index = node->black_index;
}

static rb_tree_node_t* get_successor(rb_tree_node_t* node) {
  if (NULL == node->left)
    return node;
  return get_successor(node->left);
}

static void remove_barren_node(rb_tree_node_t* node) {
  rb_tree_node_t* node_parent = node->parent;
  if (node_parent->left == node)
    node_parent->left = NULL;
  else
    node_parent->right = NULL;
}

static void rotate(rb_tree_node_t** root_ptr, bool left) {
  if (left)
    rotate_left(root_ptr);
  else
    rotate_right(root_ptr);
}

static void fix_delete_violation(rb_tree_node_t* parent, bool left) {
  rb_tree_node_t* parent_parent = parent->parent;
  if (NULL == parent_parent)
    return;

  rb_tree_node_t* node = left? parent->left: parent->right;
  if (NULL != node && node->black_index == 0)
  {
    node->black_index = 1;
    return;
  }

  rb_tree_node_t* sibling = left? parent->right: parent->left;
  bool parent_left = parent_parent->left == parent;
  rb_tree_node_t** parent_ptr = parent_left? &(parent_parent->left): &(parent_parent->right);
  if (sibling->black_index == 0) {
    rotate(parent_ptr, left);
    rb_tree_node_t* grand_father = *parent_ptr;
    rb_tree_node_t** father_ptr = left? &(grand_father->left): &(grand_father->right);
    uint32_t tmp = grand_father->black_index;
    grand_father->black_index = parent->black_index;
    parent->black_index = tmp;
    fix_delete_violation(parent, left);
  } else if (
    ((NULL == sibling->left) || (sibling->left->black_index == 1)) && 
    ((NULL == sibling->right) || (sibling->right->black_index == 1))
  ) {
    sibling->black_index = 0;
    fix_delete_violation(parent_parent, parent_left);
  } else if (
    (left && ((NULL == sibling->right) || (sibling->right->black_index == 1))) ||
    (!left && ((NULL == sibling->left) || (sibling->left->black_index == 1)))
  ) {
    rb_tree_node_t** sibling_ptr = left? &(parent->right): &(parent->left);
    rotate(sibling_ptr, !left);
    rb_tree_node_t* new_sibling = *sibling_ptr;
    uint32_t tmp = new_sibling->black_index;
    new_sibling->black_index = sibling->black_index;
    sibling->black_index = tmp;
    fix_delete_violation(parent, left);
  } else {
    rotate(parent_ptr, left);
    rb_tree_node_t* grand_father = *parent_ptr;
    grand_father->black_index = parent->black_index;
    parent->black_index = 1;
    if (left)
      grand_father->right->black_index = 1;
    else
      grand_father->left->black_index = 1;
  }
}

void delete_rb_tree_node(rb_tree_node_t* node) {
  rb_tree_node_t* node_left = node->left;
  rb_tree_node_t* node_right = node->right;

  if ((NULL == node_left) && (NULL == node_right)) {
    bool extra_black = node->black_index;
    rb_tree_node_t* parent = node->parent;
    bool left = parent->left == node;
    remove_barren_node(node);
    if (extra_black) {
      fix_delete_violation(parent, left);
    }
  } else if (NULL == node_left) {
    replace_child(node, node_right);
  } else if (NULL == node_right) {
    replace_child(node, node_left);
  } else {
    rb_tree_node_t* successor = get_successor(node_right);
    rb_tree_node_t* successor_parrent = successor->parent;
    bool is_child = successor->parent == node;
    bool extra_black = successor->black_index != 0;
    replace_successor(node, successor);

    if (extra_black) {
      bool left = !is_child;
      rb_tree_node_t* fix_parent = is_child? successor: successor_parrent;
      fix_delete_violation(fix_parent, left);
    }
  }
}

static void init_rb_tree_node(rb_tree_node_t* node, rb_tree_node_data_t* data)
{
  memcpy(&node->data, data, sizeof(rb_tree_node_data_t));

  node->left = NULL;
  node->right = NULL;

  node->black_index = 0;
}

rb_tree_node_t* get_new_rb_tree_node(
  rb_tree_t* rb_tree, rb_tree_node_data_t* data
) {
  size_t next_free_node = rb_tree->next_free_node;
  rb_tree->next_free_node++;
  if (rb_tree->next_free_node >= DUPLICATE_DROP_TABLE_SIZE)
    rb_tree->next_free_node = 0;

  rb_tree_node_t* node = &(rb_tree->rb_tree_nodes[next_free_node]);
  if (NULL != node->parent)
    delete_rb_tree_node(node);
  init_rb_tree_node(node, data);
  return node;
}

static int rb_tree_node_cmp(const rb_tree_node_data_t* data1, const rb_tree_node_data_t* data2) {
  return memcmp(&(data1->value), &(data2->value), sizeof(data1->value));
}

static rb_tree_node_t* find_rb_tree_node(
  rb_tree_t* rb_tree, rb_tree_node_t* node, rb_tree_node_data_t* data
) {
  if (NULL == node)
    return node;

  int cmp_result = rb_tree_node_cmp(&(node->data), data);
  if (cmp_result < 0)
    return find_rb_tree_node(rb_tree, node->left, data);
  if (cmp_result > 0)
    return find_rb_tree_node(rb_tree, node->right, data);
  return node;
}

static rb_tree_node_t* insert_rb_tree_node(
  rb_tree_t* rb_tree, rb_tree_node_t* node, rb_tree_node_t* new_node
) {
  rb_tree_node_t** next_node = NULL;
  int cmp_result = rb_tree_node_cmp(&(node->data), &(new_node->data));
  if (cmp_result < 0)
    next_node = &(node->left);
  else if (cmp_result > 0)
    next_node = &(node->right);

  if (NULL == *next_node) {
    *next_node = new_node;
    new_node->parent = node;
    fix_insert_violation(next_node);
    return new_node;
  } else {
    return insert_rb_tree_node(rb_tree, *next_node, new_node);
  }
}

static rb_tree_node_t* find_or_insert_rb_tree_node(
  rb_tree_t* rb_tree, rb_tree_node_data_t* data, bool* is_new
) {
  rb_tree_node_t* node = find_rb_tree_node(rb_tree, rb_tree->root.left, data);
  if (NULL != node) {
    *is_new = false;
    return node;
  }
  rb_tree_node_t* new_node = get_new_rb_tree_node(rb_tree, data);
  *is_new = true;
  return insert_rb_tree_node(rb_tree, rb_tree->root.left, new_node);
}

rb_tree_node_t* get_or_create_rb_tree_node(
  rb_tree_t* rb_tree, rb_tree_node_data_t* data, bool* is_new
) {
  if (NULL == rb_tree->root.left)
  {
    rb_tree_node_t* new_node = get_new_rb_tree_node(rb_tree, data);
    init_rb_tree_node(new_node, data);
    new_node->black_index = 1;
    new_node->parent = &(rb_tree->root);
    rb_tree->root.left = new_node;
    *is_new = true;
    return new_node;
  }
  return find_or_insert_rb_tree_node(rb_tree, data, is_new);
}

void rb_tree_init(rb_tree_t* rb_tree)
{
  rb_tree->next_free_node = 0;
  rb_tree->root.parent = NULL;
  rb_tree->root.left = NULL;
  rb_tree->root.right = NULL;
}
