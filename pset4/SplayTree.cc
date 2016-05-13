#include <assert.h>
#include "SplayTree.h"
#include <string>
/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new splay tree holding those
 * elements.
 *
 * Because splay trees rearrange their elements in response to queries, you
 * can safely ignore the assigned probabilities here and just build a BST
 * storing the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
 */
SplayTree::SplayTree(const std::vector<double>& weights)
{
  construct(0, weights.size()-1, nullptr);
}

SplayTree::SplayTree(int min, int max, SplayTree* parent) {
  construct(min, max, parent);
}

void SplayTree::construct(int min, int max, SplayTree* parent)
{
  this->left_child = nullptr;
  this->right_child = nullptr;
  this->parent = parent;

  if (min == max) {
    this->key = min;
  } else {
    this->key = (min + max + 1) / 2;
    int next_right = this->key - 1;
    if (next_right >= min) {
      this->left_child = new SplayTree(min, next_right, this);
    }
    int next_left = this->key + 1;
    if (next_left <= max) {
      this->right_child = new SplayTree(next_left, max, this);
    }
  }
}

/**
 * Frees all memory used by this tree.
 */
SplayTree::~SplayTree() {
  // TODO: Implement this!
}

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool SplayTree::contains(int key)
{
  // start searching from the root
  SplayTree *node = this;

  // iteratively walk down the tree
  while (node) {
    if (node->key == key) {
      return true;
    } else if (node->key > key) {
      node = node->left_child;
    } else {
      node = node->right_child;
    }
  }

  // found a null-path; giving up
  return false;
}

SplayTree *SplayTree::grand_parent()
{
  if (this->parent) {
    return this->parent->parent;
  } else {
    return nullptr;
  }
}

void SplayTree::set_right_child(SplayTree* new_right_child)
{
  if (this->right_child) {
    this->right_child->parent = nullptr;
  }
  this->right_child = new_right_child;
  if (new_right_child) {
    new_right_child->parent = this;
  }
}

void SplayTree::set_left_child(SplayTree* new_left_child)
{
  if (this->left_child) {
    this->left_child->parent = nullptr;
  }
  this->left_child = new_left_child;
  if (new_left_child) {
    new_left_child->parent = this;
  }
}

SplayTree* SplayTree::rotate_left()
{
  assert(this->parent == nullptr);

  SplayTree* new_root = this->right_child;
  this->set_right_child(new_root->left_child);
  new_root->set_left_child(this);

  assert(new_root->parent == nullptr);
  assert(this->parent != nullptr);

  return new_root;
}

SplayTree* SplayTree::rotate_right()
{
  assert(this->parent == nullptr);

  SplayTree* new_root = this->left_child;
  this->set_left_child(new_root->right_child);
  new_root->set_right_child(this);

  assert(new_root->parent == nullptr);
  assert(this->parent != nullptr);

  return new_root;
}

SplayTree* SplayTree::top_down_splay(int search_key)
{
  SplayTree *t = this;
  SplayTree *l = nullptr;
  SplayTree *r = nullptr;

  int count = 0;
  while (t->key && t->key != search_key) {
    count++;
    t->dump_to_dotfile("splaying-t-" + std::to_string(count));
    l->dump_to_dotfile("splaying-l-" + std::to_string(count));
    r->dump_to_dotfile("splaying-r-" + std::to_string(count));
    if (search_key < t->key) {
      if (search_key == t->left_child->key) {
        //link right
        SplayTree* new_t = t->left_child;
        SplayTree* new_r = t;
        if (r) {
//          r->set_left_child(t);
        } else {
//          t->parent = nullptr;
        }
        r = new_r;
        t = new_t;
      } else if (search_key < t->left_child->key) {
        //rotate right
        t = t->rotate_right();

        //link right
        SplayTree* new_t = t->left_child;
        SplayTree* new_r = t;
        if (r) {
          r->set_left_child(t);
        } else {
          t->parent = nullptr;
        }
        r = new_r;
        t = new_t;
      } else if (search_key > t->left_child->key) {
        //link right
        SplayTree* new_t = t->left_child;
        SplayTree* new_r = t;
        if (r) {
          r->set_left_child(t);
        } else {
          t->parent = nullptr;
        }
        r = new_r;
        t = new_t;

        // link left
        new_t = t->right_child;
        SplayTree* new_l = t;
        if (l) {
          l->set_right_child(t);
        } else {
          t->parent = nullptr;
        }
        l = new_l;
        t = new_t;
      }
    } else if (search_key > t->key) {
      if (search_key == t->right_child->key) {
        //link left
        SplayTree* new_t = t->right_child;
        SplayTree* new_l = t;
        if (l) {
          l->set_right_child(t);
        } else {
          t->parent = nullptr;
        }
        l = new_l;
        t = new_t;
      } else if (search_key < t->right_child->key) {
        //rotateleft
        t = t->rotate_left();

        // link left
        SplayTree* new_t = t->right_child;
        SplayTree* new_l = t;
        if (l) {
          l->set_right_child(t);
        } else {
          t->parent = nullptr;
        }
        l = new_l;
        t = new_t;
      } else if (search_key > t->right_child->key) {
        //link left
        SplayTree* new_t = t->right_child;
        SplayTree* new_l = t;
        if (l) {
          l->set_right_child(t);
        } else {
          t->parent = nullptr;
        }
        l = new_l;
        t = new_t;

        // link right
        new_t = t->left_child;
        SplayTree* new_r = t;
        if (r) {
          r->set_left_child(t);
        } else {
          t->parent = nullptr;
        }
        r = new_r;
        t = new_t;
      }
    }
  }
  //assemble
  SplayTree *a = t->left_child;
  SplayTree *b = t->right_child;
  if (l) l->set_right_child(a);
  if (r) r->set_left_child(b);
  t->set_left_child(l);
  t->set_right_child(r);

  return t;
}

// DEBUGGING

#include <iostream>
#include <fstream>

void bst_print_dot_null(int key, int nullcount, std::ofstream& stream);
void bst_print_dot_aux(SplayTree* node, std::ofstream& stream);
void bst_print_dot(SplayTree* tree, std::ofstream& stream);

void SplayTree::dump_to_dotfile(std::string filename)
{
  std::ofstream out_file;
  out_file.open("tree-" + filename + ".dot");
  bst_print_dot(this, out_file);
  out_file.close();
}

void bst_print_dot_null(int key, int nullcount, std::ofstream& stream)
{
  stream << "    null" << nullcount << " [shape=point];" << std::endl;
  stream << "    " << key << " -> null" << nullcount << ";" << std::endl;
//  fprintf(stream, "    null%d [shape=point];\n", nullcount);
//  fprintf(stream, "    %d -> null%d;\n", key, nullcount);
}

void bst_print_dot_aux(SplayTree* node, std::ofstream& stream)
{
  static int nullcount = 0;

  if (node->left_child)
  {
    stream << "    " << node->key <<" -> " <<  node->left_child->key << ";" << std::endl;
    bst_print_dot_aux(node->left_child, stream);
  }
  else
    bst_print_dot_null(node->key, nullcount++, stream);

  if (node->right_child)
  {
    stream << "    " << node->key <<" -> " <<  node->right_child->key << ";" << std::endl;
    bst_print_dot_aux(node->right_child, stream);
  }
  else
    bst_print_dot_null(node->key, nullcount++, stream);
}

void bst_print_dot(SplayTree* tree, std::ofstream& stream)
{
  stream << "digraph BST {" << std::endl;
//  fprintf(stream, "digraph BST {\n");
  stream << "    node [fontname=\"Arial\"];" << std::endl;
//  fprintf(stream, "    node [fontname=\"Arial\"];\n");

  if (!tree)
//    fprintf(stream, "\n");
    stream << std::endl;
  else if (!tree->right_child && !tree->left_child)
//    fprintf(stream, "    %d;\n", tree->key);
    stream << "    " << tree->key << ";" << std::endl;
  else

    bst_print_dot_aux(tree, stream);

//  fprintf(stream, "}\n");
  stream << "}" << std::endl;
}
