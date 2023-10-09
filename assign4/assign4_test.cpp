/*
 * assign4_test.cc
 * Assignment 4 (BST) test runner.
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <vector>

using std::cout;

// Set to false to disable cycle checking
const bool cycle_check = true;

std::vector<unsigned> make_random_permutation(
    std::size_t len,
    int seed = 1) 
{
    std::default_random_engine generator(seed);
    std::vector<unsigned> ret(len, 0);

    // Initialize vector to 0...len-1
    for(std::size_t i = 0; i < len; ++i) 
        ret.at(i) = i;

    std::shuffle(ret.begin(), ret.end(), generator);

    return ret;

}

#ifndef CHECK_SOLUTION

#include "splaytree.hpp"

#else

#include "assign4_soln.hpp"

#endif

/******************************************************************************
 Tree structure checking
 ******************************************************************************/

// Safe find, that does not modify the tree structure. Returns true/false,
// but does not return the node.
bool safe_find(splay_tree& t, int value) {
    splay_tree::node* root = t.root();

    while(root != nullptr) {
        if(root->key == value)
            return true;
        else if(value < root->key)
            root = root->left;
        else // value < root->key
            root = root->right;
    }
    return false;
}



int count_nodes(splay_tree::node* root) {
    if(!root)
        return 0;
    else
        return 1 + count_nodes(root->left) + count_nodes(root->right);
}

int tree_height(splay_tree::node* root) {
    if(!root)
        return 0;
    else
        return 1 + std::max(tree_height(root->left), tree_height(root->right));
}

bool safe_empty(splay_tree& t)
{
    return t.root() == nullptr;
}

/* find_true_root(n,nodes)
   Tries to find the true root of the tree, starting at node n.

   If the tree contains a cycle along the path to the root, then n itself will
   be returned, otherwise, the root of the tree (the node along the ->parent
   path with ->parent == nullptr) will be returned.
*/
splay_tree::node* find_true_root(
    splay_tree::node* n, 
    std::set<splay_tree::node*>& nodes
)
{
    if(nodes.count(n) != 0)
        return n; // Cycle!
    else if(n->parent == nullptr)
        return n;
    else {
        nodes.insert(n);
        return find_true_root(n->parent, nodes);
    }
}

/* find_true_root(t)
   Tries to find the true root of the tree t. If there is a cycle along the path
   to the root, n itself will be returned.
*/
splay_tree::node* find_true_root(splay_tree::node* n)
{
    std::set<splay_tree::node*> nodes;
    return find_true_root(n, nodes);
}

/* insert_safe(n,key)
   Insert `key` into the tree rooted at n, returning the root of the "new"
   tree. *Does not* splay n to the root. This is used to construct sample
   trees for testing.
*/
splay_tree::node* insert_safe(splay_tree::node* n, int key)
{
    if(n == nullptr)
        return new splay_tree::node{key, nullptr, nullptr, nullptr};
    else if(key == n->key)
        return n;
    else if(key < n->key) {
        n->left = insert_safe(n->left, key);
        n->left->parent = n;
        return n;
    }
    else { // key > n->key
        n->right = insert_safe(n->right, key);
        n->right->parent = n;
        return n;
    }    
}

/* trees_equal(a,b)
   Returns true if the trees rooted at a and b are equal.
*/
bool trees_equal(const splay_tree::node* a, const splay_tree::node* b)
{
    if(a == nullptr and b == nullptr)
        return true;
    else if(a == nullptr or b == nullptr)
        return false; // One empty, the other not
    else if(a->key == b->key) {
        return trees_equal(a->left, b->left) and
               trees_equal(a->right, b->right);
    }
    else
        return false;
}

bool trees_equal(const splay_tree& a, const splay_tree& b)
{
    return trees_equal(a.root(), b.root());
}

/* build_tree_safe(values)
   Constructs a tree by inserting the given values *without* splaying, using
   our own insert code. This is guaranteed to return a valid tree, suitable for
   use as input to later user code.
*/
splay_tree build_tree_safe(std::vector<int> values)
{
    splay_tree::node* t = nullptr;
    for(int x : values)
        t = insert_safe(t, x);

    splay_tree tree;
    tree.set_root(t);

    return tree;
}

/* Tree printing
   -------------

   This uses a level-order traversal to print the tree. Nodes are printed in
   the form

                                   -.
         VALUE                    VALUE
       [p=PARENT]               [p=PARENT]
        .--^--.                  <Cycle>

   The note <Cycle> is added to any node that has already been printed 
   previously.

   We save a copy of each node in the traversal structure.

   TODO: Finish this function!
*/
void print_levelorder(splay_tree& t)
{
    struct traversal_node
    {
        traversal_node(splay_tree::node* n, int level) :
            n(n), level(level)
        { }

        splay_tree::node* n;
        int level;

        splay_tree::node* cycle_with = nullptr;
    };

    std::set< splay_tree::node* > nodes_seen;

    std::vector< std::vector< traversal_node >> traversal;

    traversal.resize(tree_height(t.root())); 

    std::queue< traversal_node > q;

    q.push( traversal_node{ t.root(), 0 } );

    while(not q.empty()) {
        traversal_node tn = q.front();
        q.pop();

        // TODO: detect cycle

        // Push children, if they exist.
        if(tn.n != nullptr) {

            q.push( { tn.n->left,  tn.level + 1 } );
            q.push( { tn.n->right, tn.level + 1 } );
        }

        traversal.at(tn.level).push_back(tn); 
    }
}

// Pretty-print a tree. This does cycle-checking at the same time, so that if
// there's a cycle in the tree we won't get stuck in a loop.
void print(
    const splay_tree::node* root, 
    int level, 
    int parents, 
    bool left_child, 
    std::set<splay_tree::node*>& nodes
) {

    if(level == 0)
        cout << "--- Tree structure ---\n";

    // Print indent for node
    for(int i = 0; i < level-1; ++i)
        if(parents & (1 << i))
            cout << " │ ";
        else 
            cout << "   ";

    if(level > 0)
        cout << (left_child ? " ├─" : " └─");

    if(root == nullptr) {
        cout << "(null)" << std::endl;
    }
    else if(cycle_check && nodes.count(const_cast<splay_tree::node*>(root)) > 0) {
        // Already printed this node somewhere else
        cout << "CYCLE (" << root->key << ")" << std::endl;
    }
    else {
        nodes.insert(const_cast<splay_tree::node*>(root)); // Visit root

        // Print children
        cout.width(3);
        cout << root->key; 
        if(root->parent != nullptr)
            cout << " [p = " << root->parent->key << "]";
        cout << std::endl;

        // Print children
        if(root->left || root->right) {
            // We only print both children if one of them is non-null.
            // If both are null we don't print anything, to avoid making a huge
            // mess.

            // We print the children in the order right, left so that you can
            // turn your head (or your screen) to the left and the tree will
            // be correct.
            print(root->right, level+1, parents | (1 << level), true, nodes);
            print(root->left, level+1, parents, false, nodes);
        }
    }
}

/* print(t)
   Pretty-print the tree t. This does cycle-detection so that if the tree 
   contains a cycle we don't go into an infinite loop.
*/
void print(const splay_tree& t) {
    std::set<splay_tree::node*> nodes;

    print(t.root(), 0, 0, true, nodes);    
}

void print(const splay_tree::node* n)
{
    std::set<splay_tree::node*> nodes;

    print(n, 0, 0, true, nodes);
}

/* check_for_cycles(n)
   Traverse the tree (preorder) starting at n, checking for cycles of nodes.
   Note that this does not check for parent-pointer cycles, only child-pointer
   cycles. Use `find_true_root` to find parent-pointer cycles.
*/
bool check_for_cycles(
    splay_tree::node* n, 
    std::set<splay_tree::node*>& nodes
) {
    if(n == nullptr)
        return true; // No cycles in an empty tree

    if(!cycle_check)
        return true; // No cycles

    if(nodes.count(n) > 0)
        return false;
    else {
        nodes.insert(n); // Mark n as seen

        // Explore left and right subtrees
        bool ret = true;
        if(n->left)
            ret = ret && check_for_cycles(n->left, nodes);
        if(n->right)
            ret = ret && check_for_cycles(n->right, nodes);

        return ret;
    }
}

bool check_for_cycles(splay_tree::node* n) {
    if(n == nullptr)
        return true; // No cycles in an empty tree

    if(!cycle_check)
        return true;

    std::set<splay_tree::node*> nodes;

    if(!check_for_cycles(n, nodes)) {
        cout << "FAILED: tree structure contains a cycle.\n";
        return false;
    }
    else
        return true;
}

// Check the pointer structure of the tree (parent/child) to make sure it is
// correct. 
bool check_tree_pointers(splay_tree::node* root, bool is_root = true) {
    if(!root)
        return true;
    else {
        if(is_root && root->parent != nullptr) {
            cout << "FAILED: root->parent should always be null.\n";
            return false;            
        }

        // Child child nodes (if they exist) to make sure their parents
        // point back to root.
        if(root->left) {
            if(root->left->parent != root) {
                cout << "FAILED: found node " << root->left->key 
                     << " with incorrect parent pointer.\n";
                return false;
            }
            if(root->left->key >= root->key) {
                cout << "FAILED: found node " << root->left->key
                     << " which is on the wrong side of parent.\n";
                return false;
            }
        }

        if(root->right) {
            if(root->right->parent != root) {
                cout << "FAILED: found node " << root->right->key 
                     << " with incorrect parent pointer.\n";
                return false;
            }
            if(root->right->key <= root->key) {
                cout << "FAILED: found node " << root->right->key
                     << " which is on the wrong side of parent.\n";
                return false;
            }            
        }
        
        if(root->right && root->left) {
            // Both children, if they exist, have valid parent pointers.
            // So now we check both subtrees recursively.
            return check_tree_pointers(root->left,  false) && 
                   check_tree_pointers(root->right, false);
        }

        return true;
    }
}

bool check_tree_values(splay_tree::node* root, 
                       int low = std::numeric_limits<int>::min(),
                       int high = std::numeric_limits<int>::max()) {
    if(!root)
        return true;
    else if(root->key <= low) {
        cout << "FAILED: found node " << root->key << " improperly placed.\n";
        return false;
    }
    else if(root->key >= high) {
        cout << "FAILED: found node " << root->key << " improperly placed.\n";
        return false;   
    }
    else { // root->key is in the correct range
        return check_tree_values(root->left, low, root->key) &&
               check_tree_values(root->right, root->key, high);
    }

}

bool check_tree(splay_tree::node* root) {
    if(root && root->parent != nullptr) {
        cout << "FAILED: Root of tree must have null parent pointer";
        cout << " (root->parent->key = " << root->parent->key << ")\n";
        return false;
    }
    
    return check_for_cycles(root) && 
           check_tree_pointers(root) && 
           check_tree_values(root);    
}

bool check_tree(splay_tree& t)
{
    return check_tree(t.root());
}

/******************************************************************************
 Tree testing
 ******************************************************************************/

template<typename Func>
struct scope_exit {
    scope_exit(Func f) : exit(f)
    {}

    ~scope_exit() {
        exit();
    }

    Func exit;
};

template<typename Func>
scope_exit<Func> make_scope_exit(Func f) {
    return scope_exit<Func>(f);
}

// To test the tree functions, we generate a random permutation of the integers
// from -20 to 20 and insert them into the tree. Then, we generate another
// permutation and find them in that order. Finally, we generate another
// permutation and remove them in that order. After every operation, we perform
// a full check of the tree structure. The test stops if the tree structure is
// not valid at any point.

bool test_rotate() {

    cout << "Testing rotation...";

    /* Input tree (no root)

         1           parent
          \  
           2         child

       After rotation:

         2
        /
       1 
    */ 
    splay_tree test1 = build_tree_safe({1, 2});
    splay_tree::node* root = test1.root();
    try {
        splay_tree::rotate(root->right, root);

        // In this case only, we have to reassign the root.
        test1.set_root(root->parent);
    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.\n";
        return false;
    }

    if(not trees_equal(test1, build_tree_safe({2, 1}))) {
        cout << "Result of child-parent rotation (with no subtrees) is incorrect:\n";

        cout << "Expected: \n";
        print(build_tree_safe({2, 1}));

        cout << "Actual result: \n";
        print(test1);

        return false;
    }

    /* Input tree:
       1
        \
         2         Parent
          \
           3       Child

       Expected result:

       1
        \
         3
        /
       2
    */  
    splay_tree test2 = build_tree_safe({1,2,3});
    splay_tree test2_expected = build_tree_safe({1,3,2});
    root = test2.root();
    splay_tree::rotate(root->right->right, root->right);

    if(not trees_equal(test2, test2_expected)) {
        cout << "Result of child-parent rotation (with no subtrees) is incorrect:\n";

        cout << "Expected:\n";
        print(test2_expected);

        cout << "Actual result:\n";
        print(test2);
        return false;
    }

    /* Input tree

                10
               /
              5       parent
             / \
     child  2   7
           / \
          1   3

       Expected result:

                10
               /
              2
             / \
            1   5
               / \
              3   7 
    */
    splay_tree test3 = build_tree_safe({10, 5, 2, 7, 1, 3});
    splay_tree test3_expected = build_tree_safe({10, 2, 1, 5, 3, 7});
    root = test3.root();

    splay_tree::rotate(root->left->left, root->left);

    if(not trees_equal(test3, test3_expected)) {
        cout << "Result of child-parent rotation (with subtrees) is incorrect:\n";

        cout << "Expected:\n";
        print(test3_expected);

        cout << "Actual result:\n";
        print(test3);
        return false;
    }

    /* Input tree

            10
           /
          1            parent
         / \  
        0   5          child
           / \
          2   7 

       Expected result:

            10
           /
          5
         / \
        1   7
       / \
      0   2
    */
    splay_tree test4 = build_tree_safe({10, 1, 0, 5, 2, 7});
    splay_tree test4_expected = build_tree_safe({10, 5, 1, 7, 0, 2});
    root = test4.root();

    splay_tree::rotate(root->left->right, root->left);

    if(not trees_equal(test4, test4_expected)) {
        cout << "Result of child-parent rotation (with subtrees) is incorrect:\n";

        cout << "Expected:\n";
        print(test4_expected);

        cout << "Actual result:\n";
        print(test4);
        return false;
    }        

    cout << "OK!\n";
    return true;
}

/* test_splay()
   Test the splay operation specifically. This exists to make sure students
   are not just rotate a node with its parent repeatedly (which would still
   make the node become the root).
*/
bool test_splay()
{
    cout << "Testing splay...";

    #define CHECK_SPLAY(case)                                                \
        if(not trees_equal(test1, test1_expected) ) {                        \
            cout << "Result of splay (" #case " case) is not correct:\n";    \
            cout << "Expected:\n";                                           \
            print(test1_expected);                                           \
            cout << "Actual result:\n";                                      \
            print(test1);                                                    \
            return false;                                                    \
        }         

    // All told, there are 6 different cases we want to test for (left/right 
    // for each of zig, zig-zag, zig-zig).

    // Zig on the right
    {
        splay_tree test1 = build_tree_safe({1,2});
        splay_tree test1_expected = build_tree_safe({2,1});

        test1.set_root( test1.splay(test1.root()->right) );

        CHECK_SPLAY(zig);
    }

    // Zig on the left
    {
        splay_tree test1 = build_tree_safe({2,1});
        splay_tree test1_expected = build_tree_safe({1,2});

        test1.set_root( test1.splay(test1.root()->left) );

        CHECK_SPLAY(zig);
    }

    // Zig-Zig on the right
    {
        splay_tree test1 = build_tree_safe({1, 2, 3}); 
        splay_tree test1_expected = build_tree_safe({3, 2, 1});

        test1.set_root( test1.splay( test1.root()->right->right ));

        CHECK_SPLAY(zig-zig-right);
    }  

    // Zig-Zig on the left
    {
        splay_tree test1 = build_tree_safe({3, 2, 1}); 
        splay_tree test1_expected = build_tree_safe({1, 2, 3});

        test1.set_root( test1.splay( test1.root()->left->left ));

        CHECK_SPLAY(zig-zig-left);
    } 

    // Zig-zag on the left-right
    {
        splay_tree test1 = build_tree_safe({3, 1, 2}); 
        splay_tree test1_expected = build_tree_safe({2, 1, 3});

        test1.set_root( test1.splay( test1.root()->left->right ));

        CHECK_SPLAY(zig-zag-left-right);
    }   

    // Zig-zag on the right-left
    {
        splay_tree test1 = build_tree_safe({1, 3, 2}); 
        splay_tree test1_expected = build_tree_safe({2, 3, 1});

        test1.set_root( test1.splay( test1.root()->right->left ));

        CHECK_SPLAY(zig-zag-right-left);
    }             

    cout << "OK!\n";
    return true;
}

bool test_tree() {
    splay_tree t; // Empty tree

    bool result = true;

    cout << "Testing empty()...";
    cout.flush();
    try {
        t.empty();
    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.\n";
        result = result and false;
    }
    cout << "OK!\n";

    cout << "Testing .size()...";
    try {
        t.size();
    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.\n";
        result = result and false;
    }
    cout << "OK!\n";

    // Generate test data
    std::vector<unsigned> test = make_random_permutation(41, 12);

    // Insert a random permutation
    cout << "Testing tree insertion...";
    cout.flush();
    try {
        int c = 0; // Expected number of nodes
        for(unsigned u : test) {
            const int i = static_cast<int>(u);
            cout << u << " ";

            t.insert(i);
            c++;
            if(count_nodes(t.root()) == 0) {
                cout << "FAILED: tree is empty after insert.\n";
                return false;
            }

            if(count_nodes(t.root()) != c) {
                cout << "FAILED: wrong number of nodes (expected " << c << ")\n";
                print(t);
                return false;
            }

            if(!check_tree(t)) {
                print(t);
                return false; // Stop if the check fails.
            }
            
            if(t.root()->key != i) {
                cout << "FAILED: After inserting " << i << " it should be splayed to the root\n";
                print(t);
                return false;
            } 
                   
        }
        cout << std::endl;

        // check_tree checks for cycles, so it's safe to count the number of nodes
        // here. 
        int cn = count_nodes(t.root());
        if(cn != 41) {
            cout << "FAILED: tree does not have the correct number of nodes. ";
            cout << "(expected 41, found " << cn << ")\n";
            print(t);
            return false;
        }
        else {
            cout << "OK!\n";
            print(t);
        }        
    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.\n";
        result = result and false;
    }

    // Find a random permutation
    cout << "Testing find()...";
    cout.flush();
    try {
        for(unsigned u : test) {
            const int i = static_cast<int>(u);  
            cout << i << " ";
            
            splay_tree::node* n = t.find(i);
            if(n == nullptr or n->key != i) {
                cout << "FAILED: find() couldn't find " << i << "\n";
                return false;
            }

            if(t.root() == nullptr) {
                cout << "FAILED: find() makes the tree empty!\n";
                return false;
            }
            
            if(t.root()->key != i) {
                cout << "FAILED: find() did not splay target to the root.\n";
                print(t);
                return false;
            }
            
            if(!check_tree(t)) {
                print(t);
                return false;
            }        
        }
        cout << "OK, final tree: \n";
        print(t);

    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.\n";
        result = result and false; 
    }

    // Remove a random permutation
    cout << "Testing tree removal... ";
    cout.flush();
    try {
        for(unsigned u : test) {
            const int i = static_cast<int>(u);

            t.remove(i);

            if(!check_tree(t)) {
                cout << "Tree check failed: ";
                print(t);
                return false;
            }
            if(safe_find(t,i)) {
                cout << "FAILED: removed element " << i << " is still present in the tree\n";
                print(t);
                return false;
            }        
        }

        if(not t.empty()) {
            cout << "FAILED: Tree not empty after removing all elements.\n";    
            print(t);
            return false;
        }

        cout << "Success!";
    }
    catch(std::logic_error&) {
        cout << "Not implemented, skipping.";
        // Note: don't update result here
    }

    cout << "\n";

    return result;
}

int main() {
    cout << "---- Beginning tree tests ----\n";
    bool result = true;

    // We do this to avoid short-circuiting, so that later tests will run even
    // if earlier ones fail.
    result = result and test_rotate();
    result = result and test_splay();
    result = result and test_tree();

    if(result) {
        cout << "---- All tests successful ----\n";
	return 0;
    }

    return 1;
}
