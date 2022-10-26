#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;  //enum을 쓰면 앞에서 부터 차례대로, 0, 1 값으로 정의
// 열거형을 정의해도 메모리는 할당되지 않는다. 열거된 유형의 변수가 정의된 경우, 해당 변수에 대해 메모리가 할당된다.

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key; 
  struct node_t *parent, *left, *right; 
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
