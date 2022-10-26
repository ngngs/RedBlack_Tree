#include "rbtree.h"

#include <stdlib.h>


// RB tree 구조체 생성 : 여러 개의 tree를 생성할 수 있어야 하며 각각 다른 내용들을 저장할 수 있어야 합니다.
rbtree *new_rbtree(void) {        
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  nil_node->key = 0;
  nil_node->color = 1; //red = 0 , black = 1
  nil_node->parent = NULL;
  nil_node->left = NULL;
  nil_node->right = NULL;

  p->nil = nil_node;
  p->root = p->nil;

  return p;
}

void del_node(node_t *t, node_t *nil){
  if (t != nil){
    del_node(t->left, nil);
    del_node(t->right, nil);
    free(t);
    t = NULL;
  }
}

//  RB tree 구조체가 차지했던 메모리 반환 : 해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  del_node(t->root, t->nil); // root~nil(nil은 미포함)에 할당된 포인터가 해제
  free(t->nil); // rbtree t의 nil에 할당된 포인터가 해제된다
  free(t);  // rbtree t에 할당된 포인터가 해제된다
}

// Left-Rotate
void rotate_left(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) y->left->parent = x;
  
  y->parent = x->parent;
  if (x->parent == t->nil) t->root = y;
  else if (x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->left = x;
  x->parent = y;
}

// Right-Rotate
void rotate_right(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil) y->right->parent = x;
  
  y->parent = x->parent;
  if (x->parent == t->nil) t->root = y;
  else if (x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;

  y->right = x;
  x->parent = y;
}

// insert fix-up
void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t *y;
  while (z->parent->color == 0){
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      if (y->color == 0){
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      }
      else{ 
        if (z==z->parent->right){
          z = z->parent;
          rotate_left(t, z);
        }
        z->parent->color = 1;
        z->parent->parent->color = 0;
        rotate_right(t, z->parent->parent);
        }
    }
    else { // (z->parent == z->parent->parent->right)
      y = z->parent->parent->left;
      if (y->color == 0){
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      }
      else {
        if (z==z->parent->left){
          z = z->parent;
          rotate_right(t, z);
        }
        z->parent->color = 1;
        z->parent->parent->color = 0;
        rotate_left(t, z->parent->parent);
      }
    }
  }
  t->root->color = 1;
}


//  key 추가 : 구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;

  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  z->key = key;
  z->color = 0; // 들어온 값은 무조건 red색칠
  z->left = NULL;
  z->right = NULL;

  while (x != t->nil){
    y = x;
    if (z->key < x->key){
      x = x->left;
    }
    else x = x->right;
  }
  z->parent = y;
  if (y == t->nil) t->root = z;
  else if (z->key < y->key) y->left = z;
  else y->right = z;
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = 0;
  rbtree_insert_fixup(t, z);

  return t->root;
}

// RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환, 해당하는 node가 없으면 NULL 반환
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *this_node = t->root;

  while (this_node != t->nil){
    if (key == this_node->key) return this_node;
    else if (key < this_node->key) this_node = this_node->left;
    else this_node = this_node->right;
  }
  return NULL; // find 실패하면 계속 return되던 null값을 갖고 탈출
}

// RB tree 중 최소 값을 가진 node pointer 반환
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *min = t->root;
  while(min->left != t->nil){
    min = min->left;
  }

  return min;
}

// 최대값을 가진 node pointer 반환
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *max = t->root;
  while(max->right != t->nil){
    max = max->right;
  }

  return max;
}


//erase 구현을 위한 transplant(이식함수)
void transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil) t->root = v;
  else if (u==u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

node_t *rbtree_minimum(const rbtree *t, node_t *x) {
  while(x->left != t->nil){
    x = x->left;
  }
  return x;
}

// delete fixup
void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while (x != t->root && x->color == 1){
    if (x == x->parent->left){
      w = x->parent->right;
      if (w->color == 0){
        w->color = 1;
        x->parent->color = 0;
        rotate_left(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == 1 && w->right->color == 1){
        w->color = 0;
        x = x->parent;
      }
      else{
        if (w->right->color == 1){
          w->left->color = 1;
          w->color = 0;
          rotate_right(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = 1;
        w->right->color = 1;
        rotate_left(t, x->parent);
        x = t->root;
      }
    }
    else{ // x == x->parent->right 인 경우
      w = x->parent->left;
      if (w->color == 0){
        w->color = 1;
        x->parent->color = 0;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == 1 && w->left->color == 1){
        w->color = 0;
        x = x->parent;
      }
      else {
        if (w->left->color == 1){
          w->right->color = 1;
          w->color = 0;
          rotate_left(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = 1;
        w->left->color = 1;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = 1;
}


// RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  node_t *x;
  color_t y_origin_color = y->color;

  if (p->left == t->nil){
    x = p->right;
    transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    transplant(t, p, p->left);
  }
  else{
    y = rbtree_minimum(t, p->right); 
    y_origin_color = y->color;
    x = y->right;
    if (y->parent == p) x->parent = y;
    else {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_origin_color == 1) rbtree_delete_fixup(t, x);
  free(p);
  return 0;
}

int inorder(const rbtree *t, node_t *p, key_t *arr, const size_t n, int i){
  if (i < n){
    if (p == t->nil){
      return i;
    }
    i = inorder(t, p->left, arr, n, i);
    arr[i++] = p->key;
    i = inorder(t, p->right, arr, n, i);
    return i;
  }
  return 0;
}

// RB tree의 내용을 key 순서대로 주어진 array로 변환
// array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
// array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder(t, t->root, arr, n, 0);
  return 0;
}
