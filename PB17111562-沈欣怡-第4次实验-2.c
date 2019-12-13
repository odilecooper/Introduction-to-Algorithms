#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 2

#define N 20

typedef struct RBNode {
	int low;//key
    int high;
    int max;
	int color;
	int size;
	struct RBNode *left;
	struct RBNode *right;
	struct RBNode *p;
}RBNode;
typedef struct RBTree {
	RBNode* root;
	RBNode* nil;
}RBTree;

RBTree *T;

void UPDATE_SIZE(RBNode *x) {
	int i=1;
	if (x->left != T->nil) i += x->left->size;
	if (x->right != T->nil) i += x->right->size;
	x->size = i;
}
int MAX(int x, int y, int z) {
    if (x > y)
        return (x>z? x:z);
    else return (y>z? y:z);
}
void UPDATE_MAX(RBNode *x) {
    if (x->left==T->nil && x->right==T->nil)
        x->max = x->high;
    else if (x->left == T->nil)
        x->max = (x->high>x->right->max? x->high:x->right->max);
    else if (x->right == T->nil)
        x->max = (x->high>x->left->max? x->high:x->left->max);
    else x->max = MAX(x->high, x->left->max, x->right->max);
}
RBNode * TREE_MINIMUM(RBNode *x) {
	while (x->left != T->nil)
		x = x->left;
	return x;
}
RBNode *SELECT(RBNode *x, int i) {
	int r = x->left->size + 1;
	if (i == r)
		return x;
	else if (i < r)
		return SELECT(x->left, i);
	else return SELECT(x->right, i-r);
}
void LEFT_ROTATE(RBNode *x) {
	RBNode *y = x->right;
	x->right = y->left;
	if (y->left != T->nil)
		y->left->p = x;
	y->p = x->p;
	if (x->p == T->nil)
		T->root = y;
	else if (x == x->p->left)
		x->p->left = y;
	else x->p->right = y;
	y->left = x;
	x->p = y;
    UPDATE_MAX(x);
	UPDATE_SIZE(x);
    UPDATE_MAX(y);
	UPDATE_SIZE(y);
}
void RIGHT_ROTATE(RBNode *x) {
	RBNode *y = x->left;
	x->left = y->right;
	if (y->right != T->nil)
		y->right->p = x;
	y->p = x->p;
	if (x->p == T->nil)
		T->root = y;
	else if (x == x->p->right)
		x->p->right = y;
	else x->p->left = y;
	y->right = x;
	x->p = y;
    UPDATE_MAX(x);
	UPDATE_SIZE(x);
    UPDATE_MAX(y);
	UPDATE_SIZE(y);
}
void RB_INSERT_FIXUP(RBNode *z) {
	RBNode *y;
	while (z->p->color == RED) {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			} else { 
				if (z == z->p->right) {
					z = z->p;
					LEFT_ROTATE(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				RIGHT_ROTATE(z->p->p);
			}
		} else {
			y = z->p->p->left;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			} else {
				if (z == z->p->left) {
					z = z->p;
					RIGHT_ROTATE(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				LEFT_ROTATE(z->p->p);
			}
		}
	}
	T->root->color = BLACK;
}
void RB_INSERT(RBNode *z) {
	RBNode *y = T->nil;
	RBNode *x = T->root;
	while (x != T->nil) {
		y = x;
		if (z->low < x->low)
			x = x->left;
		else x = x->right;
	}
	z->p = y;
	if (y == T->nil)
		T->root = z;
	else if (z->low < y->low)
		y->left = z;
	else y->right = z;
	z->left = T->nil;
	z->right = T->nil;
	z->color = RED;
	RB_INSERT_FIXUP(z);
	x = z->p;
	while (x != T->nil){
		UPDATE_MAX(x);
		UPDATE_SIZE(x);
		x = x->p;
	}
}
void RB_TRANSPLANT(RBNode *u, RBNode *v) {
	if (u->p == T->nil)
		T->root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else u->p->right = v;
	v->p = u->p;
}
void RB_DELETE_FIXUP(RBNode *x) {
	while (x != T->root && x->color == BLACK) {
		if (x == x->p->left) {
			RBNode *w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				LEFT_ROTATE(x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			} else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					RIGHT_ROTATE(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				LEFT_ROTATE(x->p);
				x = T->root;
			}
		} else {
			RBNode *w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				RIGHT_ROTATE(x->p);
				w = x->p->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->p;
			} else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					LEFT_ROTATE(w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				RIGHT_ROTATE(x->p);
				x = T->root;
			}
		}
	}
	x->color = BLACK;
}
void RB_DELETE(RBNode *z){
	RBNode *y = z;
	RBNode *x;
	int y_original_color = y->color;
	RBNode *y_original_p = y->p;
	if (z->left == T->nil) {
		x = z->right;
		RB_TRANSPLANT(z, z->right);
	} else if (z->right == T->nil) {
		x = z->left;
		RB_TRANSPLANT(z, z->left);
	} else {
		y = TREE_MINIMUM(z->right);
		y_original_color = y->color;
		y_original_p = y->p;
		x = y->right;
		if (y->p == z)
			x->p == y;
		else {
			RB_TRANSPLANT(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		RB_TRANSPLANT(z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (y_original_color == BLACK)
		RB_DELETE_FIXUP(x);
	while (x != T->nil) {
		UPDATE_MAX(x);
		UPDATE_SIZE(x);
		x = x->p;
	}
	while (y_original_p != T->nil) {
		UPDATE_MAX(y_original_p);
		UPDATE_SIZE(y_original_p);
		y_original_p = y_original_p->p;
	}
}
void RB_PRINT(RBNode *x, int dep) {
	int i;
	if (x == T->nil) return;
	for (i=dep; i>0; i--)
		printf("------");
	printf("--[%d,%d],%d,%d\n", x->low, x->high, x->max, x->size);
//	if (x->color == RED) printf("R\n");
//	else printf("B\n");
	if (x->left != T->nil) RB_PRINT(x->left, dep+1);
	if (x->right != T->nil) RB_PRINT(x->right, dep+1);
	return;
}
int main() {
//new tree
	RBNode *NIL = (RBNode *)malloc(sizeof(RBNode));
	NIL->color = BLACK;
	NIL->left = NIL;
	NIL->right = NIL;
	NIL->p = NIL;
	NIL->size = 0;
	T = (RBTree *)malloc(sizeof(RBTree));
	T->root = NIL;
	T->nil = NIL;
//insert node
	srand(2);
	int i;
	for (i=0; i<N; i++) {
		RBNode *z = (RBNode *)malloc(sizeof(RBNode));
		z->low = rand() % 250;
        z->high = z->low + rand()%250 + 1;
        z->max = z->high;
		z->size = 1;
		printf("Insert [%d,%d]\n", z->low, z->high);
		z->color = RED;
		RB_INSERT(z);
//		RB_PRINT(T->root, 0);
	}
	RB_PRINT(T->root, 0);
	
	RBNode *n4 = SELECT(T->root, N/4);
	printf("\nDelete node [%d,%d]\n", n4->low, n4->high);
	RB_DELETE(n4);
	RB_PRINT(T->root, 0);
	
	RBNode *n2 = SELECT(T->root, N/2);
	printf("\nDelete node [%d,%d]\n", n2->low, n2->high);
	RB_DELETE(n2);
	RB_PRINT(T->root, 0);
	return 0;
}
