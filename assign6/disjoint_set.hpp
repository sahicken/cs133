#pragma once
/*
 * disjoint_set.hpp
 */
#include <cassert>
#include <vector>

/* not_implemented
   Thrown by functions/methods you haven't written yet, this exception signals
   to the test runner to skip any tests involving the relevant functions/
   methods. When you write a function/method, delete the

     throw not_implemented{};

   from it.
*/
struct not_implemented
{
};

class disjoint_set
{
    struct node
    {
        int index;              // Set index
        node *parent = nullptr; // Parent set (-1 for roots)
        int rank = 0;
    };

public:
    /* djsoint_set(n)
       Constructs a disjoint_set object with (initially) n disjoint sets.
    */
    disjoint_set(int elem_count) : total_elems(elem_count)
    {
        forest = new node *[this->elem_count()];
        for (auto i = 0; i < this->elem_count(); ++i)
        {
            forest[i] = new node();
            forest[i]->index = i;
        }
    }

    /* ~disjoint_set()
       Destructor.
    */
    ~disjoint_set()
    {
        for (auto i = 0; i < this->elem_count(); ++i)
            delete forest[i];
        delete[] forest;
    }

    /* disjoint_set d = c;
       Copy constructor.
    */
    disjoint_set(const disjoint_set &original)
    {
        total_elems = original.elem_count();
        forest = new node *[original.elem_count()];
        for (auto i = 0; i < original.elem_count(); ++i)
        {
            forest[i] = new node();
            forest[i]->index = original.rep(i);
        }
    }

    /* d = c;
       Copy-assignment operator.
    */
    disjoint_set &operator=(const disjoint_set &original)
    {
        total_elems = original.elem_count();
        forest = new node *[original.elem_count()];
        for (auto i = 0; i < original.elem_count(); ++i)
        {
            forest[i] = new node();
            forest[i]->index = original.rep(i);
        }

        return *this;
    }

    /* elem_count()
       Returns the total number of elements, as set in the constructor. This
       does not change over the lifetime of the `disjoint_set`.

       Must run in O(1) time.
    */
    int elem_count() const { return total_elems; }

    /* set_count()
       Returns the number of disjoint sets that *currently* exist. Initially,
       this will be the same as `size()`, but it shrinks each time sets are
       merged (eventually to a minimum of 1, if all sets are merged). This is
       essentially the number of nodes whose `parent == -1`.

       Note that the return value from this can never be less than 1 (when all
       sets have been merged together) or greater than the original `elem_count`
       (if no sets have been merged).

       Must run in O(N) time where N = `elem_count()`, but there is an
       optimization which can make this run in O(1) time...
    */
    int set_count() const
    {
        auto count = 0;
        for (auto i = 0; i < this->elem_count(); ++i)
            if (forest[i]->parent == nullptr)
                ++count;
        return count;
    }

    /* singleton()
       Returns true if all elements are in the same set (i.e., if
       `set_count() == 1`).
    */
    bool singleton() const { return set_count() == 1; }

    /* rep(n)
       Returns the "representative" for set n. n must be ≥ 0 and < `elem_count()`.

       This should perform path compression.

       Must run in O(N) time where N is the depth of set n within its tree.
       (The amortized runtime will be O(α(N)) which is essentially O(1).)
    */
    int rep(int n)
    {
        assert(n >= 0 and n < elem_count());

        node *r = forest[n];

        // find the root of e's tree
        while (r->parent != nullptr)
            r = r->parent;

        // path compression
        node *temp = forest[n];
        while (temp != r)
        {
            node *p = temp->parent;
            temp->parent = r;
            temp = p;
        }

        return r->index;
    }

    /* rep(n) const
       The const version of `rep(n)` still returns the representative for n, but
       does NOT do path compression (because, being const, it's not allowed to
       modify the data members).
    */
    int rep(int n) const
    {
        assert(n >= 0 and n < elem_count());

        node *r = forest[n];

        // find the root of e's tree
        while (r->parent != nullptr)
            r = r->parent;

        return r->index;
    }

    /* in_same_set(a,b)
       Returns true if a and b are in the same set. Note that a and b are
       not necessarily the roots of their sets!

       Must run in O(M + N) time, where M and N are the sizes of the sets
       containing a and b, respectively.
    */
    bool in_same_set(int a, int b)
    {
        assert(a >= 0 and a < elem_count());
        assert(b >= 0 and b < elem_count());

        return rep(a) == rep(b);
    }

    /* merge(a,b)
       Merge sets a and b (both must be ≥ 0 and < `size()`). If `a` and `b` are
       already in the same set this should do nothing (but may perform path
       compression).

       This should perform the merge-by-rank optimization.

       Returns true if two sets were merged, and false if not (i.e., if a and
       b were already in the same set).

       Must run in O(M + N) time, where M is the size of a's set and N is the
       size of b's set. (The amortized runtime will be O(α(N)) which is
       essentially O(1).)
    */
    bool merge(int a, int b)
    {
        assert(a >= 0 and a < elem_count());
        assert(b >= 0 and b < elem_count());

        a = rep(a);
        b = rep(b);

        if (a == b)
            return false; // same group
        if (forest[b]->rank > forest[a]->rank)
            std::swap(a, b); // a always has higher rank

        forest[b]->parent = forest[a]; // b becomes child of a
        forest[a]->rank = std::max(forest[a]->rank, forest[b]->rank) + 1;
        return true;
    }

    /* elements(n)
       Returns the vector of elements that are in the same set as n. Note that
       n is not necessarily the root of its set!

       Must run in O(N²) time (where N == elem_count()).
    */
    std::vector<int> elements(int n)
    {
        assert(n >= 0 and n < elem_count());

        std::vector<int> e;
        auto temp = rep(n);
        for (int i = 0; i < elem_count(); ++i)
            if (temp == rep(i))
                e.push_back(i);
        return e;
    }

private:
    // Array of nodes; initialize in your constructor
    node **forest = nullptr;

    // Total number of elements. This should be set in your constructor and then
    // never changed after that.
    int total_elems = -1;

    // This class is defined in the test file; it's a friend so that it can
    // access the private members of `disjoint_set` for easier testing.
    friend class disjoint_set_tester;
};