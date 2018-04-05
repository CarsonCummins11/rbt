/*
Carson Cummins
2018
This emulates a binary search tree

 */
#include <iostream>
#include<fstream>
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
void case2(node*);
void case3(node*);
node* parent(node*);
node* grandparent(node*);
node* uncle(node*);
void repair_tree(node*);
node* sibling(node*);
void rotate_left(node* n);

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
    printf("%d\n", root->data);
 
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
//add to then end of the bst regardless of node value
void add (node*& head, node* toadd){
	 if (!head){ 
    head = toadd;
	 }
  else if (toadd->data < head->data){
   add(head->left,toadd);
  }else{
    add(head->right,toadd);
  }
}
node* grandparent(node* n){
	node* p = parent(n);
	if(p){
		return parent(p);
	}
	return NULL;
}
void rotate_left(node* n){
	node* newn = n->right;
	n->right = newn->left;
	newn->left = n;
	newn->parent = n->parent;
	n->parent = newn;
}
void rotate_right(node* n){
	node * newn = n->left;
	n->left = newn->right;
	newn->right = n;
	newn->parent = n->parent;
	n->parent = newn;
}
void insert_r(node* & root, node* n){
	if(root&&n->data<root->data){
	if(root->left){	
		insert_r(root->left,n);
		return;
	}else{
		root->left=n;
	}
}else if(root!=NULL){
	if(root->right){
		insert_r(root->right,n);
		return;
	}else{
		root->right = n;
	}
}
n->parent = root;
n->left = NULL;
n->right = NULL;
n->red = true; 
}
void insert(node*& root, node* n){
	insert_r(root,n);
	repair_tree(n);
	root = n;
	while(parent(root)){
		root = parent(root);
	}
}

void repair_tree(node* n){
	if(!parent(n)){
		case1(n);
	}else if(!parent(n)->red){
		return;
	}else if(uncle(n)->red){
		case2(n);
	}else{
		case3(n);
	}
}
void case1(node* n){
	if(!parent(n)){
		n->red = false;
	}
}
void case2(node* n){
	parent(n)->red = false;
	uncle(n)->red = false;
	grandparent(n)->red = true;
	repair_tree(grandparent(n));
}
void case3(node* n){
	node* p = parent(n);
	node* g = grandparent(g);
	if(n==g->left->right){
	rotate_left(p);
	n = n->left;
	}else if(n == g->right->left){
		rotate_right(p);
		n = n->right;
	}
	if(n==p->left){
		rotate_right(g);
	}else{
		rotate_left(g);
		p->red = false;
		g->red = true;
	}
}
//remove first occurence of that number
void remove(node*& head, int dat){
	if(dat==head->data){
	if(head->left&&head->right){
		node* succ = head->right;
		node* psucc = head;
		if(!succ->left){
			head->data = succ->data;
			head->right = succ->right;
			return;
		}
		while(succ->left){
			psucc = succ;
			succ = succ->left;
		}
		head->data = succ->data;
		psucc->left = succ->right;
	}else if(!(head->left||head->right)){
		head=NULL;
	}else if(head->right&&!head->left){
		head = head->right;
	}else if(head->left&&!head->right){
		head = head->left;
	}
	}else{
		if(dat<head->data){
			remove(head->left,dat);
		}else{
			remove(head->right,dat);
		}
	}
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
			cout << "Quit (Q), Insert (I), Remove(R)" << endl;
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
			}else if(c=='R'){
				cout << "input the node to delete" << endl;
				int inn;
				cin>>inn;
				remove(head,inn);
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
	while(infile >> x){
		node* ad = new node;
			ad->data = x;
			ad->left = NULL;
			ad->right = NULL;
			insert(head,ad);
	}
		print(head,0);
		while(true){
			cout << "Quit (Q), Insert (I), Remove(R)" << endl;
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
			add(head,ad);
			print(head,0);
			}else if(c=='R'){
				cout << "input the node to delete" << endl;
				int inn;
				cin>>inn;
				remove(head,inn);
				print(head,0);
			}else{
				cout << "not a valid command" << endl;
			}
		}

	}
}
