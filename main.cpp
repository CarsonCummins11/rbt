/*
Carson Cummins
2018
This emulates a binary search tree

 */
#include <iostream>
#include<fstream>
#include<stdlib.h>
using namespace std;
//node structure
struct node{
	int data;
	node* left; 
	node* right;
	node* parent;
	bool red;
};
void case1(node*);
void case2(node*&,node*);
void case3(node*&,node*);
node* parent(node*);
node* grandparent(node*);
node* uncle(node*);
void repair_tree(node*&,node*);
node* sibling(node*);
void rotate_left(node*&,node* n);

//print the heap
//thanks https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
// I had my own but it made no sense so I thought this one looked better
void print(node * root, int space){
	 // Base case
    if (root == NULL){
        return;
	}
 
    // Increase distance between levels
    space += 10;
 
    // Process right child first
    print(root->right, space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" "); 
    printf("%d", root->data);
	if(root->red)
	{
		cout << "R\n";
	}
	else
	{
		cout << "B\n";
	}
 
    // Process left child
    print(root->left, space);
}
node* parent(node* n){
	return n->parent;
}
node* uncle(node* n){
	node * p = parent(n);
	node * g = grandparent(n);
	if(!g){
		return NULL;
	}
	return sibling(p);
}
node* sibling(node* n){
	node * p = parent(n);
	if(p == NULL){
		return NULL;
	}
	if(n==p->left){
		return p->right;
	}else{
		return p->left;
	}
}
node* grandparent(node* n){
	node* p = parent(n);
	if(p){
		return parent(p);
	}
	return NULL;
}
void rotate_left(node*& head, node* n){
	node* newn = n->right;
	if(newn){
	n->right = newn->left;
	newn->left=n;
	newn->parent = n->parent;
	n->parent = newn;
	if(newn->parent&&newn->parent->left==n){
		newn->parent->left=newn;
	}
	else if(newn->parent&&newn->parent->right==n){
		newn->parent->right=newn;
	}
	if(n==head){
		head=newn;
		head->red=false;
	}
	}
}
void rotate_right(node*& head,node* n){
	node * newn = n->left;
	if(newn){
		if(n->parent&&n==n->parent->left){
			n->parent->left = n->left;
		}else if(n->parent&&n==n->parent->right){
			n->parent->right = n->left;
		}
	n->left = newn->right;
	if(newn->right){
		newn->right->parent = n;
	}
	newn->right = n;
	newn->parent = n->parent;
	n->parent = newn;
	
	if(n==head){
		head = newn;
		head->red = false;
	}
	}
}
void insert_r(node* & root, node* n){
	if(!root){
		root=n;
		n->parent = NULL;
		n->left = NULL;
		n->right = NULL;
		n->red = true;
	}else if(root&&n->data<root->data){
	if(root->left){	
		insert_r(root->left,n);
		return;
	}else{
		root->left=n;
		n->parent = root;
		n->left = NULL;
		n->right = NULL;
		n->red = true;
	}
}else{
	if(root->right){
		insert_r(root->right,n);
		return;
	}else{
		root->right = n;
		n->parent = root;
		n->left = NULL;
		n->right = NULL;
		n->red = true;
	}
}
}
node* insert(node*& root, node* n){
	insert_r(root,n);
	repair_tree(root,n);
	root = n;
	while(parent(root)){
		root = parent(root);
	}
	return root;
}

void repair_tree(node*&head,node* n){
	
	if(!parent(n)){
		case1(n);
	}else if(!parent(n)->red){
		return;
	}else if(uncle(n)&&uncle(n)->red){
		case2(head,n);
	}else{
		case3(head,n);
	}
}
void case1(node* n){
	if(!parent(n)){
		n->red = false;
	}
}
void case2(node*&head,node* n){
	parent(n)->red = false;
	uncle(n)->red = false;
	grandparent(n)->red = true;
	repair_tree(head,grandparent(n));
}
void case3(node*& head,node* n){
	node* p = parent(n);
	node* g = grandparent(n);
	if(g->left&&n==g->left->right){
	rotate_left(head,p);
	n = n->left;
	}else if(g->right&&n == g->right->left){
		rotate_right(head,p);
		n = n->right;
	}
	 p = parent(n);
	 g = grandparent(n);
	if(n==p->left){
		rotate_right(head,g);
	}else{
		rotate_left(head,g);
	}
	p->red = false;
	g->red = true;
}
//take in data and process
int main(){
	cout << "1: enter numbers, 2: enter file path" <<endl;
	int choice;
	cin>>choice;
	if(choice == 1){
	  //user chose to input numms
	  node * head = NULL;
			while(true){
			cout << "Quit (Q), Insert (I)" << endl;
			char c;
			cin >> c;
			if(c=='Q'){
				break;
			}else if(c=='I'){
				cout << "input #" << endl;
			int inn;
			cin >> inn;
			node* ad = new node;
			ad->data = inn;
			ad->left = NULL;
			ad->right = NULL;
			insert(head,ad);
			print(head,0);
			}else{
				cout << "not a valid command" << endl;
			}
		}
	}else if(choice ==2){
	  //user chose file
	ifstream infile;
	int length = 1;
	cout << "input path"<<endl;
	char* path = new char;
	cin>>path;
	infile.open(path);
	node * head = NULL;
	int x;
	const int MAXSIZE = 5;
	char thisVal[MAXSIZE];
	while(infile.getline(thisVal,MAXSIZE,',')){
		x = atoi(thisVal);
		node* ad = new node;
			ad->data = x;
			ad->left = NULL;
			ad->right = NULL;
			head = insert(head,ad);
	}
		print(head,0);
		while(true){
			cout << "Quit (Q), Insert (I)" << endl;
			char c;
			cin >> c;
			if(c=='Q'){
				break;
			}else if(c=='I'){
				cout << "input #" << endl;
			int inn;
			cin >> inn;
			node* ad = new node;
			ad->data = inn;
			ad->left = NULL;
			ad->right = NULL;
			head = insert(head,ad);
			print(head,0);
			}else{
				cout << "not a valid command" << endl;
			}
		}

	}
}
