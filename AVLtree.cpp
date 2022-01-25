#include <iostream>
#include <stack>
#include <queue>

using namespace std ;

class Node {
  public :
  Node *l_child ;
  int data ;
  Node *r_child ;
  int height ;
};

class AVL {
  private :
    Node *root ;
    int rootState ;
  public :
    AVL() {
        root = nullptr ;
        rootState = 0 ;
    }
    
    void rInorder() {
        cout<<endl ;
        cout<<"Inorder : ";
        rInorder(getRoot()) ;
    }
    
    Node* rInsert(Node *p , int key) ;
    Node* Delete(Node *p, int key) ;
    int   NodeHeight(Node *p) ;
    int   BalanceFactor(Node *p) ;
    Node* LLRotation(Node *p) ;
    Node* LRRotation(Node *p) ;
    Node* RLRotation(Node *p) ;
    Node* RRRotation(Node *p) ;
    Node* getRoot() ;
    void  setRoot(int key) ;
    void  rInorder(Node *p) ;
    Node* InSuccessor(Node *p) ;
    Node* InPredecessor(Node *p) ;
    
};

void AVL::rInorder(Node *p) {
    if (p) {
        rInorder(p->l_child) ;
        cout<<p->data<<"  " ;
        rInorder(p->r_child) ;
    }
}

Node* AVL::getRoot() {
    return root ;
}

void AVL::setRoot(int key) {
    if(!rootState) {
        root = rInsert(root,key) ;
        rootState = 1 ;
    }
}

int AVL::NodeHeight(Node *p) {
    int hl , hr ;
    hl = p && p->l_child ? p->l_child -> height : 0 ;
    hr = p && p->r_child ? p->r_child -> height : 0 ;
    return hl > hr ? hl + 1 : hr + 1 ;
}

int AVL::BalanceFactor(Node *p) {
    int hl , hr ;
    hl = p && p->l_child ? p->l_child -> height : 0 ;
    hr = p && p->r_child ? p->r_child -> height : 0 ;
    return hl - hr ;
}

Node* AVL::RRRotation(Node *p) {
    Node *pr , *prl ;
    
    pr  = p  -> r_child ; 
    prl = pr -> l_child ;
    
    pr -> l_child = p   ;
    p  -> r_child = prl ;
    // Update height
    p   -> height = NodeHeight(p) ;
    pr  -> height = NodeHeight(pr) ;
    // set new root if root was displaced 
    if( root == p ) {
        root = pr ;
    }
    
    return pr ;
}

Node* AVL::RLRotation(Node *p) {
     Node *pr , *prl ; 
    
    pr  = p  -> r_child ;
    prl = pr -> l_child ;
    
    pr  -> l_child = prl -> r_child ;
    p   -> r_child = prl -> l_child ;
    prl -> r_child = pr ;
    prl -> l_child = p  ;
    
    pr -> height = NodeHeight(pr) ;
    p  -> height = NodeHeight(p)  ;
    prl-> height = NodeHeight(prl);
    
    if( root == p ) {
        root = prl ;
    }
    
    return prl ;
}

Node* AVL::LRRotation(Node *p) {
    Node *pl , *plr ; 
    
    pl  = p  -> l_child ;
    plr = pl -> r_child ;
    
    pl  -> r_child = plr -> l_child ;
    p   -> l_child = plr -> r_child ;
    plr -> l_child = pl ;
    plr -> r_child = p  ;
    
    pl -> height = NodeHeight(pl) ;
    p  -> height = NodeHeight(p)  ;
    plr-> height = NodeHeight(plr);
    
    if( root == p ) {
        root = plr ;
    }
    
    return plr ;
}

Node* AVL::LLRotation(Node *p) {
    Node *pl , *plr ;
    
    pl  = p  -> l_child ; 
    plr = pl -> r_child ;
    
    pl -> r_child = p   ;
    p  -> l_child = plr ;
    // Update height
    p   -> height = NodeHeight(p) ;
    pl  -> height = NodeHeight(pl) ;
    // set new root if root was displaced 
    if( root == p ) {
        root = pl ;
    }
    
    return pl ;
}

Node* AVL::rInsert(Node *p, int key) {
    Node* t;
    //------ check for first node ------
    if (p == nullptr){
        t = new Node;
        t->data = key;
        t->height = 1 ;
        t->l_child = nullptr;
        t->r_child = nullptr;
        return t ;
    }
    //------ for rest nodes ------
    if (key < p->data){
        p->l_child = rInsert(p->l_child, key);
    } else if (key > p->data){
        p->r_child = rInsert(p->r_child, key);
    }
    p->height = NodeHeight(p) ;
    
    if (BalanceFactor(p)==2 && BalanceFactor(p->l_child)==1) {
        return LLRotation(p) ;
    }
    else if (BalanceFactor(p)==2 && BalanceFactor(p->l_child)==-1) {
        return LRRotation(p) ;
    }
    else if (BalanceFactor(p)==-2 && BalanceFactor(p->r_child)==-1) {
        return RRRotation(p) ;
    }
    if (BalanceFactor(p)==-2 && BalanceFactor(p->r_child)==1) {
        return RLRotation(p) ;
    }
    
    
    return p;  
    
}

Node* AVL::InPredecessor(Node *p) {
    while (p && p->r_child != nullptr){
        p = p->r_child;
    }
    return p;
}
 
Node* AVL::InSuccessor(Node *p) {
    while (p && p->l_child != nullptr){
        p = p->l_child;
    }
    return p;
}

// ------ Delete function ------
Node* AVL::Delete(Node *p, int key) {
    if (p == nullptr){
        return nullptr;
    }
 
    if (p->l_child == nullptr && p->r_child == nullptr){
        if (p == root){
            root = nullptr;
        }
        delete p;
        return nullptr;
    }
 
    if (key < p->data){
        p->l_child = Delete(p->l_child, key);
    } else if (key > p->data){
        p->r_child = Delete(p->r_child, key);
    } else {
        Node* q;
        if (NodeHeight(p->l_child) > NodeHeight(p->r_child)){
            q = InPredecessor(p->l_child);
            p->data = q->data;
            p->l_child = Delete(p->l_child, q->data);
        } else {
            q = InSuccessor(p->r_child);
            p->data = q->data;
            p->r_child = Delete(p->r_child, q->data);
        }
    }
 
    // Update height
    p->height = NodeHeight(p);
 
    // Balance Factor and Rotation
    if (BalanceFactor(p) == 2 && BalanceFactor(p->l_child) == 1) {  // L1 Rotation
        return LLRotation(p);
    } else if (BalanceFactor(p) == 2 && BalanceFactor(p->l_child) == -1){  // L-1 Rotation
        return LRRotation(p);
    } else if (BalanceFactor(p) == -2 && BalanceFactor(p->r_child) == -1){  // R-1 Rotation
        return RRRotation(p);
    } else if (BalanceFactor(p) == -2 && BalanceFactor(p->r_child) == 1){  // R1 Rotation
        return RLRotation(p);
    } else if (BalanceFactor(p) == 2 && BalanceFactor(p->l_child) == 0){  // L0 Rotation
        return LLRotation(p);
    } else if (BalanceFactor(p) == -2 && BalanceFactor(p->r_child) == 0){  // R0 Rotation
        return RRRotation(p);
    }
 
    return p;
}

int main() {
    AVL avl;
    cout<<avl.getRoot()<<endl;
    avl.setRoot(10); // ------ set first node ------
    avl.rInsert(avl.getRoot(),20);
    cout<<avl.getRoot()->data<<endl;
    avl.rInsert(avl.getRoot(),30);
    cout<<avl.getRoot()->data<<endl;
    avl.rInsert(avl.getRoot(),25);
    cout<<avl.getRoot()->data<<endl;
    avl.rInsert(avl.getRoot(),28);
    cout<<avl.getRoot()->data<<endl;
    avl.rInsert(avl.getRoot(),27);
    cout<<avl.getRoot()->data<<endl;
    avl.rInsert(avl.getRoot(),5);
    cout<<avl.getRoot()->data<<endl;
    
    avl.rInorder() ;
    
    avl.Delete(avl.getRoot(),25) ;
    avl.rInorder() ;
    cout<<endl<<avl.getRoot()->data<<endl;
    
    avl.Delete(avl.getRoot(),27) ;
    avl.rInorder() ;
    cout<<endl<<avl.getRoot()->data<<endl;
    
    return 0 ;
}


