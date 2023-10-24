#pragma once
#include <cassert>
#include <stdexcept>

class splay_tree {
  public:
    struct node 
    {
        node(int key, node* left, node* right, node* parent) : 
            key(key), left(left), right(right), parent(parent)
        { }

        int key;
        node* left;
        node* right;
        node* parent;
    };

    ~splay_tree() { clear(); }

    /* root()
       Returns the root of the tree. 

       Runs in O(1) time.
    */
    node* root() const { return rt; }

    /* size()
       Returns the size (total number of nodes) in the tree. 

       Runs in O(n) time.
    */
    int size() const
    {
        if (empty()) return 0;
        else {
            int sz=0;
            auto n=root();
            if(n->left!=nullptr) size();
            else if(n->right!=nullptr) size();
            else ++sz;
            return sz;
        }

    }

    /* empty()
       Returns true if the tree is empty.

       Runs in O(1) time.
    */
    bool empty() const { return rt==nullptr; }

    /* rotate(c,p)
       Rotate child node c with parent node p. c must be a child of p
       (either c == p->left or c == p->right) and neither c nor p can be 
       nullptr.

       Runs in O(1) time.
    */
    static void rotate(node* c, node* p)
    {
        assert(c != nullptr and p != nullptr);
        assert(c == p->left or  c == p->right);
        assert(c->parent == p);

        // Remove the next line and add your code here.
        // throw std::logic_error("Not implemented");
        // Pivot = Root.OS
        // Root.OS = Pivot.RS
        // Pivot.RS = Root
        // Root = Pivot
        if (c==p->left) {
            // rotate right
            // Let c be p's left child.
            // Set p's left child to be c's right child.
            // [Set c's right-child's parent to p]
            // Set c's right child to be p.
            // [Set p's parent to c]
            p->left=c->right;
            c->right->parent=p;
            c->right=p;
            p->parent=c;
        } else {
            // rotate left
            // Let c be p's right child.
            // Set p's right child to be c's left child.
            // [Set c's left-child's parent to p]
            // Set c's left child to be p.
            // [Set p's parent to c]
            p->right=c->left;
            c->left->parent=p;
            c->left=p;
            p->parent=c;
        }

    }

    /* splay(n)
       Splays n to the root of the tree, returning the new root. n must not
       be nullptr. 

       As with `rotate`, splay is a static member function, so it is not allowed
       to access any member variables (it can call `rotate`, however).

       Runs in O(d) time where d is the depth of node n (amortized, this 
       will be O(log n)).
    */
    static node* splay(node* n)
    {
        assert(n != nullptr);

        // Remove the next line and add your code here.
        throw std::logic_error("Not implemented");
    }

    /* find(k)
       Finds and returns the node containing key k, splaying it to the root.
       If no such node exists, then `find` splay's the parent of the location
       where k *should* be to the root, and then returns the new root. If the 
       tree is empty, returns nullptr. To determine whether a key k exists in 
       the (nonempty) tree, you would check

            k == find(k)->key

       Runs in O(log n) amortized time.
    */
    node* find(int k)
    {
        // Remove the next line and add your code here.
        throw std::logic_error("Not implemented");
    }

    /* insert(k)
       Inserts k into the tree, splaying the new node to the root. If k 
       already exists in the tree, it should be splayed to the root. Returns
       the new root of the tree. 

       Runs in O(log n) amortized time.
    */
    node* insert(int k)
    {
        // Remove the next line and add your code here.
        throw std::logic_error("Not implemented");
    }

    /* remove(k)
       EXTRA CREDIT: Removes the node containing k, and splays its parent to
       the root. If k does not exist in the tree, then nothing is removed,
       but the parent (of where k *should* exist) is still splayed. Returns
       the new root of the tree.

       Runs in O(log n) amortized time.
    */
    node* remove(int k)
    {
        // If you want to do the extra credit problem, remove the next line.
        throw std::logic_error("Not implemented");
    }

    /* set_root(n)
       Replaces the root node with n; this is only used for testing.
    */
    void set_root(node* n) { rt = n; }

    /* clear()
       Delete all nodes in the tree. You should implement the recursive 
       `clear(node*)` version below, and not modify this one.
    */
    void clear() { clear(rt); }

  private:

    /* clear(n)
       Delete n and all its descendants.
    */
    void clear(node* n)
    {
        if(n->left!=nullptr) clear(n->left);
        else if(n->right!=nullptr) clear(n->right);
        else delete n;
    }

    node* rt = nullptr;
};