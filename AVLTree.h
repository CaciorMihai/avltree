/*
Author : Cacior Mihai
*/

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

typedef struct node{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root;
	TreeNode *nil;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;

// Creates the tree and the sentinel node.
// Nil node replaces the NULL pointer, all roads lead to nil.
TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){

	TTree *tree = (TTree*)malloc(sizeof(TTree));
	if(!tree){ printf("Memory Error\n");  exit(1); }
	tree->createElement = createElement;
	tree->destroyElement = destroyElement;
	tree->createInfo = createInfo;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;

	TreeNode *nil = (TreeNode*)(malloc(sizeof(TreeNode)));
	if(!nil){ printf("Memory Error\n");  exit(1); }

	nil->pt = nil;
	nil->lt = nil;
	nil->rt = nil;
	nil->next = nil;
	nil->prev = nil;
	nil->end = nil;
	nil->height = 0l;
	nil->elem = NULL;
	nil->info = NULL;
	nil->height = 0l;

	tree->nil = nil;
	tree->root = nil;
	tree->size = 0l;
}

// Initializes a node, returns a pointer to that node.
TreeNode* createTreeNode(TTree *tree, void* value, void* info){
	TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
	if(!node) { printf("Memory Error\n");  exit(1); }

	node->elem = (void*)(tree->createElement)(value);
	node->info = (void*)(tree->createInfo)(info);

	node->pt = tree->nil;
	node->lt = tree->nil;
	node->rt = tree->nil;
	node->next = tree->nil;
	node->prev = tree->nil;
	node->end = node;

	node->height = 0;

	return node;
}

// Frees the memory of a node.
void destroyTreeNode(TTree *tree, TreeNode* node){
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);
	free(node);
}

int isEmpty(TTree* tree){
	if(tree->root == tree->nil){
		return 1;
	}
	return 0;
}

// Searches for value in the tree vrom a certain node x
// Returns a pointer to the node if the value is found, or a pointer to nil if not.
TreeNode* search(TTree* tree, TreeNode* x, void* elem){
	while(x != tree->nil){
		if( tree->compare(x->elem, elem) == -1 ){ // <
			x = x->rt;
		}
		else if( tree->compare(x->elem, elem) == 1 ){ // >
			x = x->lt;
		}
		else if( tree->compare(x->elem, elem) == 0 ){
			break;
		}
	}
	if( x !=  tree->nil){
		return x;
	}
	else{
		return tree->nil;
	}
}

// Returns the left-most node.
TreeNode* minimum(TTree*tree, TreeNode* x){
	while( x->lt != tree->nil){
		x = x->lt;
	}	
	return x;
}

// Returns the right-most node.
TreeNode* maximum(TTree* tree, TreeNode* x){
	while( x->rt != tree->nil){
		x = x->rt;
	}	
	return x;
}

// Returns the node with the smallest value bigger than x's.
TreeNode* successor(TTree* tree, TreeNode* x){
	if(x == tree->nil){
		return tree->nil;
	}
	if(x->rt != tree->nil){
		return minimum(tree, x->rt);
	}
	TreeNode *y = x->pt;
	while(y != tree->nil && x == y->rt){
		x = y;
		y = y->pt;
	}
	return y;
}

// Returns the node with the biggest value smaller than x's.
TreeNode* predecessor(TTree* tree, TreeNode* x){
	if(x == tree->nil){
		return tree->nil;
	}
	if(x->lt != tree->nil){
		return maximum(tree, x->lt);
	}
	TreeNode *y = x->pt;
	while(y != tree->nil && x == y->lt){
		x = y;
		y = y->pt;
	}
	return y;
}

// Rotates the subtree with the root x to the left.
void avlRotateLeft(TTree* tree, TreeNode* x){ 
	TreeNode *y = x->rt;
	x->rt = y->lt;

	if(y->lt != tree->nil)
		y->lt->pt = x; // the parent of the y left subtree is x
	y->pt = x->pt; // update parent for y

	if(x->pt == tree->nil)
		tree->root = y;
	else if(x->pt->lt == x)
		x->pt->lt = y;
	else
		x->pt->rt = y;

	y->lt = x; // x is to the left of y
	x->pt = y; // x parent is y

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;

}

// Rotates the subtree with the root x to the right.
void avlRotateRight(TTree* tree, TreeNode* y){ 
	TreeNode *x = y->lt;

	y->lt = x->rt;
	if(x->rt != tree->nil)
		x->rt->pt = y;

	x->pt = y->pt;

	if(y->pt == tree->nil)
		tree->root = x;
	else if(y->pt->lt == y)
		y->pt->lt = x;
	else
		y->pt->rt = x;

	x->rt = y;
	y->pt = x;

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;
}

// Get AVL balance factor for node x 
int avlGetBalance(TreeNode *y){
	return y->lt->height - y->rt->height;
}

// Rebalances the tree, if necessary.
void avlFixUp(TTree* tree, TreeNode* y){
	while( y != tree->nil ){
		y->height = MAX(y->rt->height, y->lt->height) + 1;
		if( avlGetBalance(y) <= -2){// Right case
			if(avlGetBalance(y->rt) == -1){// Right-Right case
				avlRotateLeft(tree, y);
			}
			else if(avlGetBalance(y->rt) == 1){// Right-Left case
				avlRotateRight(tree, y->rt);
				avlRotateLeft(tree, y);
			}
			else if(avlGetBalance(y->rt) == 0){// Special case
				avlRotateLeft(tree, y);
			}
		}
		if( avlGetBalance(y) >= 2){// Left Case
			if(avlGetBalance(y->lt) == -1){// Left-Right case
				avlRotateLeft(tree, y->lt);
				avlRotateRight(tree, y);
			}
			else if(avlGetBalance(y->lt) == 1){// Left-Left case
				avlRotateRight(tree, y);
			}
			else if(avlGetBalance(y->lt) == 0){// Special case
				avlRotateRight(tree, y);
			}
		}
		y = y->pt;
	}
}

// Finds the position and inserts a node with the given elem and info.
// avlFixUp is called at the end to rebalance the tree.
void insert(TTree* tree, void* elem, void* info) {
	TreeNode *p, *newnod = createTreeNode(tree, elem, info);
	
	p = tree->root;
	if(isEmpty(tree) == 1){
		tree->root = newnod;
		newnod->pt = tree->nil;
	}
	else{
		while(p != tree->nil){
			if( tree->compare(p->elem, newnod->elem) < 0 ){ // <
				if(p->rt == tree->nil){
					p->rt = newnod;
					newnod->pt = p;
					
					newnod->next = p->end->next;
					if(p->end->next != tree->nil){
						p->end->next->prev = newnod;
					}
					p->end->next = newnod;
					newnod->prev = p->end;
					
					p->height = MAX(p->lt->height, 1);
					break;
				}
				p = p->rt;
			}
			else if( tree->compare(p->elem, newnod->elem) > 0 ){ // >
				if(p->lt == tree->nil){
					p->lt = newnod;
					newnod->pt = p;
					
					newnod->prev = p->prev;
					if(p->prev != tree->nil){
						p->prev->next = newnod;
					}
					p->prev = newnod;
					newnod->next = p;
					
					p->height = MAX(p->rt->height, 1);

					break;
				}
				p = p->lt;
			}
			else if( tree->compare(p->elem, newnod->elem) == 0 ){
				newnod->next = p->end->next;
				if(p->end->next != tree->nil){
					p->end->next->prev = newnod;
				}
				p->end->next = newnod;
				newnod->prev = p->end;
				newnod->height = p->height;
				p->end = newnod;
				tree->size ++;
				return;
			}
		}
	}
	tree->size ++;
	avlFixUp(tree, newnod);
}

// Deletes a node and patches up the tree.
void delete(TTree* tree, void* elem){
	TreeNode *p = search(tree, tree->root, elem), *c = tree->nil, *t;
	if( p == tree->nil){
		// Elem was not found
		return;
	}
	if( p->end != p){// Elem is a duplicate
		c = p->end;
		if(c->prev != tree->nil){
			c->prev->next = c->next;
		}
		if(c->next != tree->nil){
			c->next->prev = c->prev;
		}
		p->end = p->end->prev;
		destroyTreeNode(tree, c);
		return;
	}
	if( p->lt == tree->nil && p->rt == tree->nil){ // Node of elem is a leaf node
		if(tree->root == p){ // Check if p is the root
			tree->root = tree->nil;
			destroyTreeNode(tree, p);
			return;
		}
		else{ // If its not the root, find the parent
			c = p->pt;
			if(c->lt == p){
				c->lt = tree->nil;
			}
			else if(c->rt == p){
				c->rt = tree->nil;
			}
		}
	}
	if( p->lt != tree->nil && p->rt == tree->nil){// Node has only the left child
		if(tree->root == p){ // Check if p is the root
			tree->root = p->lt;
			p->lt->pt = tree->nil;
			tree->root->next = tree->nil;
			destroyTreeNode(tree, p);
			return;
		}
		else{ // If its not the root, find the parent
			c = p->pt;
			if(c->lt == p){
				c->lt = p->lt;
			}
			else if(c->rt == p){
				c->rt = p->lt;
			}
			p->lt->pt = c;
		}
	}
	if( p->lt == tree->nil && p->rt != tree->nil){// Node has only the right child
		if(tree->root == p){ // Check if p is the root
			tree->root = p->rt;
			p->rt->pt = tree->nil;
			tree->root->prev = tree->nil;
			tree->size --;
			destroyTreeNode(tree, p);
			return;
		}
		else{ // If its not the root, find the parent
			c = p->pt;
			if(c->lt == p){
				c->lt = p->rt;
			}
			else if(c->rt == p){
				c->rt = p->rt;
			}
			p->rt->pt = c;
		}
	}
	if( p->lt != tree->nil && p->rt != tree->nil){// Node has both children
		t = successor(tree, p);
		t->lt = p->lt;
		t->lt->pt = t;

		if(p->rt != t){
			t->pt->lt = t->rt;
			if(t->rt != tree->nil){
				t->rt->pt = t->pt;
			}
			t->rt = p->rt;
			t->rt->pt = t;
		}
		if(tree->root == p){ // Check if p is the root
			tree->root = t;
			t->pt = tree->nil;
		}
		else{
			c = p->pt;
			if(c->lt == p){
				c->lt = t;
			}
			else if(c->rt == p){
				c->rt = t;
			}
			t->pt = c;
		}
	}
	// Update list links
	if(p->prev != tree->nil){
		p->prev->next = p->next;
	}
	if(p->next != tree->nil){
		p->next->prev = p->prev;
	}
	destroyTreeNode(tree, p);
	tree->size --;
	avlFixUp(tree, c);
}

// Frees the memory of the entire tree.
void destroyTree(TTree* tree){
	TreeNode *p = minimum(tree, tree->root), *c;
	while(p != tree->nil){
		c = p;
		p = p->next;
		destroyTreeNode(tree, c);
	}
	free(tree->nil);
	free(tree);
}

#endif /* AVLTREE_H_ */