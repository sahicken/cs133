/*
 * list_tests.cc
 * Test the doubly-linked list implementation
 */
#include <iostream>
#include <string>
#include <sstream>


/* ===========================================================================
   BEGIN STANDARD PREAMBLE
   =========================================================================*/
   
/* ASSIGNMENT_URL
   Update this to the URL of the assignment. This will be used to generate links to 
   the test descriptions.
*/
#define ASSIGNMENT_URL "https://staffwww.fullcoll.edu/aclifton/cs133/newassign2.html"

#include <random>
#include <vector>
#include <functional>
#include <algorithm>
#include <climits>

/* CHECK(cond,msg)
   Check if cond == true; if so, print

       FAILED TEST #n: msg
       (See https://url for details)

   and then return false.

   
*/
#ifndef LIST_ALL_CHECKS
#define CHECK(cond,msg)                                                      \
    do {                                                                     \
        if(cond) {                                                           \
            const int n = __COUNTER__;                                       \
            std::cout << "\n\u001b[31;1mTEST #" << n << " "                  \
                         "(" << __FILE__ << ":" << __LINE__ << ")" <<        \
                         " FAILED:\u001b[0m " << msg << '\n';                \
            std::cout << "(See \u001b[32;1m" << ASSIGNMENT_URL <<            \
                         "#test" << n << "\u001b[0m for info.)\n";           \
            return false;                                                    \
        }                                                                    \
    } while(false);                              

#else
// This part is used to list the checks. This will probably only work on GCC/Clang, 
// but students should never need to use this.
#define DO_PRAGMA(x)       _Pragma( #x )
#define PRINT(m)           DO_PRAGMA(GCC warning #m)
#define CHECK(cond, msg)                                                     \
	do {                                                                     \
		PRINT(CHECK msg)                                                     \
	} while(false);

#endif

void OK() { std::cout << "\u001b[32m" << "OK" << "\u001b[0m" << "\n"; } 

/* make_random_vector(len)
   Returns a vector<int> of random values, where each entry is between 0 and
   INT_MAX. The optional second parameter lets you specify the seed to be used 
   for the RNG.
*/
std::vector<int> make_random_vector(
    std::size_t len,
    int seed = 1) 
{
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution;
    auto gen = std::bind(distribution, generator);

    // Fill with random values
    std::vector<int> ret(len, 0);
    for(std::size_t i = 0; i < len; ++i)
        ret.at(i) = gen() % 100;

    return ret;
}

/* make_random_permutation(len)
   Returns a vector of length len containing a random permutation of the 
   integers 0...len-1. This can, of course, be used to randomly permute any
   vector of length len.
*/
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

/* ===========================================================================
   END STANDARD PREAMBLE
   =========================================================================*/

#include "dlist.hpp"

// We include dlist twice, to check for functions in header files (which are not
// allowed).
#include "dlist.hpp"

// Prints the contents of the list in the forward direction; You can use limit to only print the
// first n elements.
char print_fwd (dlist& l, int limit = INT_MAX)
{
	using std::cout; 
	
	if(l.head() == nullptr) 
		cout << "<empty list>";
	else {
		// If the list does not end with nullptr, this will fail.
		for(dlist::node* n = l.head(); 
		    n != nullptr and limit > 0; 
		    n = n->next, --limit) 
		{
			if(n != l.head())
				cout << " -> ";

			cout.width(3);
			cout << n->value;
		}
	}

	return '\0';
}

char print_rev(dlist& l, int limit = INT_MAX)
{
	using std::cout;

	if(l.head() == nullptr) 
		cout << "<empty list>";
	else {
		for(dlist::node* n = l.tail(); 
		    n != nullptr and limit > 0; 
		    n = n->prev, --limit) 
		{
			if(n != l.tail())
				cout << " <- ";

			cout.width(3);
			cout << n->value;
		}
	}

	return '\0';
}

// Returns true if the list a is acyclic, in both the forward and reverse directions
bool acyclic(dlist& l)
{
	std::vector<dlist::node*> nodes;

	for(dlist::node* n = l.head(); n != nullptr; n = n->next) {

		// Check to see if n is already in the vector
		for(dlist::node* m : nodes)
			if(m == n)
				return false; // Same node seen twice

		// Otherwise, add to the collection of "seen" nodes and continue
		nodes.push_back(n);
	}

	nodes.clear();

	for(dlist::node* n = l.tail(); n != nullptr; n = n->prev) {

		for(dlist::node* m : nodes)
			if(m == n)
				return false;

		nodes.push_back(n);
	}

	return true;
}

/* verify(l)
   Check the structure of the list.
*/
bool verify(dlist& l) {
	// Empty list
	if(l.head() == nullptr and l.tail() == nullptr) {
		CHECK(l.size() != 0,
			"Empty list (head == tail == nullptr) but list.size() = " << l.size());

		// I can't think of any other way the empty list could go wrong.
		return true;
	}

	CHECK(l.head() == nullptr and l.tail() != nullptr, 
		"head is nullptr but tail is not (if one is nullptr, both should be)");

	CHECK(l.head() != nullptr and l.tail() == nullptr,
		"tail is nullptr but head is not (if one is nullptr, both should be)");

	// These check for head != tail (which should indicate a list with > 1 elements)
	// but head->next or tail->prev == nullptr.
	CHECK(l.head()->next == nullptr and l.head() != l.tail(),
		"head->next == nullptr, but head != tail.");

	CHECK(l.tail()->prev == nullptr and l.head() != l.tail(),
		"tail->prev == nullptr, but head != tail.");

	// Single-element list, because head == tail.
    if(l.head() == l.tail()) {

		CHECK(l.size() != 1,
			"head == tail (!= nullptr), but size() != 1.\n" <<
			"List: " << print_fwd(l));

		// We only need to check this for head, because head == tail.
		CHECK(l.head()->next != nullptr,
			"head == tail (!= nullptr), but node->next != nullptr");

		CHECK(l.head()->prev != nullptr,
			"head == tail (!= nullptr), but node->prev != nullptr");

		// Again, don't need to check for tail, because head == tail.
	
        return true; 
    }
    else { // head != tail, should be a list of size > 1

		CHECK(l.size() <= 1,
			"head != tail, but list.size() = " << l.size());

        // Check outward-pointing pointers for nullness
        CHECK(l.head()->prev != nullptr, "head->prev != nullptr");
        CHECK(l.tail()->next != nullptr, "tail->next != nullptr");

        // In a multi-element list, head->next should be non-null,
        // as should tail->prev.

        CHECK(l.head()->next == nullptr,
        	"head != tail, but head->next == nullptr");

        CHECK(l.tail()->prev == nullptr,
        	"head != tail, but tail->prev == nullptr");

        // Check all internal pointers to make sure they line up.

        dlist::node*c = l.head();

        // Because head != tail, we know this will be true at least once. 
        while(c != l.tail()) {
			CHECK(c == nullptr, "Found a non-tail node with n->next == nullptr");

			// Only if we have moved beyond the head do we check these, because they refer to c->prev
			if(c != l.head()) {
				CHECK(c->prev == nullptr, "Found a non-head node with n->prev == nullptr");

				// If we did this at the head, we'd get a nullptr dereference
				CHECK(c->prev->next != c,
					"n->prev->next does not point back to n.");
			}

			CHECK(c->next->prev != c,
				"n->next->prev does not point back to n.");        

			CHECK(c->next == c->prev,
				"Found a node " << c->value << " where next == prev");

			CHECK(c->prev == c,
				"Found a node " << c->value << " where n->prev == n");

			CHECK(c->next == c,
				"Found a node " << c->value << " where n->next == n");

            c = c->next;
        }

        // Make sure we can traverse the list from both directions
        for(dlist::node* n = l.head(); n != l.tail(); n = n->next) {
			CHECK(n == nullptr,
				"Could not traverse the list from head to tail");
        }
        
        for(dlist::node* n = l.tail(); n != l.head(); n = n->prev) {
        	CHECK(n == nullptr,
        		"Could not traverse the list from tail to head");
        } 

		CHECK(not acyclic(l),
			"List has a cycle");

        return true; // Everything OK!
    }
}

// Returns true if l contains the same elements as v.
// This assumes that the list is traversable in the forward direction, ending with
// a nullptr.
bool contains_elements(dlist& l, std::vector<int> v)
{
	size_t i = 0;
	dlist::node* n = l.head();

	while(i != v.size() and n != nullptr) {

		if(v.at(i) != n->value)
			return false;

		++i;
		n = n->next;
	}

	// If they both don't end at the same point, then that's a fail.
	if((i == v.size()) != (n == nullptr))
		return false; 

	return true;
}

// If the previous function returned false, this can be used to print the
// pair of elements where the vector and the list differ.
void show_mismatched_elements(dlist& l, std::vector<int> v)
{
	using std::cout; 
	size_t i = 0;
	dlist::node* n = l.head();

	while(i != v.size() and n != nullptr) {

		if(v.at(i) != n->value)
			cout << "list.at(" << i << ") = " << n->value << ", should be " << v.at(i); 

		++i;
		n = n->next;
	}

	// If they both don't end at the same point, then that's a fail.
	if(i == v.size() and n != nullptr) {
		cout << "list.at(" << i << ") = " << n->value << ", should be the end of the list";
	}
	else if(i < v.size() and n == nullptr) {
		cout << "end-of-list, should be " << v.at(i);
	}	
}

std::ostream& operator<< (std::ostream& out, std::vector<int> v)
{
	for(std::size_t i = 0; i < v.size(); ++i) {
		out.width(3);
		out << v.at(i) << (i < v.size() - 1 ? ",   " : ""); // Align with list printing
	}

	return out;
}

/*****************************************************************************
 * Test functions
 *****************************************************************************/

bool test_empty() {
    std::cout << "Checking empty list... ";
    std::cout.flush();
    dlist e;
    
    if(!verify(e))
        return false;

	CHECK(not e.empty(),
		"empty list is not .empty()");

	OK();

    return true;
}

bool test_insert() {
    using std::cout;

	// What should the list contain?
	std::vector<int> contents;

    cout << "Checking .insert() into empty list... ";
    std::cout.flush();
    dlist e;
    e.insert(e.head(), 100);
    contents.push_back(100);
    
    if(!verify(e))
        return false;

    CHECK(e.size() != 1,
    	"size() != 1 after inserting one element into an empty list");

	CHECK(e.at(0)->value != 100,
		"After inserting 100 into an empty list, l.at(0) = " << e.at(0)->value);
		
    OK();

    cout << "Checking .insert() method at head() by inserting 0, 1, ..., 10; ";
    std::cout.flush();
    dlist l1;
    contents.clear();
    for(int i = 0; i < 10; ++i) {

        l1.insert(l1.head(), i);

        // vector::insert inserts before, while our insert inserts after, which 
        // requires a bit of an adjustment. 
        if(i == 0)
        	contents.push_back(i);
        else
        	contents.insert(contents.begin() + 1, i);
        
        if(!verify(l1))
            return false;

		CHECK(l1.size() != i + 1,
			"After inserting " << i + 1 << " elements, list.size() = " << l1.size());

		CHECK(not contains_elements(l1, contents),
			"After inserting " << i << ", the contents of the list are incorrect\n" <<
			"List:           " << print_fwd(l1) << "\n" 
			"Should contain: " << contents);       
    }
    
    OK();

    std::cout << "Checking .insert() at tail() by inserting 0, 1, ..., 9, 10; "; 
    std::cout.flush();   
    dlist l2;
    contents.clear();
    for(int i = 0; i < 10; ++i) {

        l2.insert(l2.tail(), i);
        contents.push_back(i);
        
        if(!verify(l2))
            return false;

		CHECK(l2.size() != i + 1,
			"After inserting " << i + 1 << " elements, list.size() = " << l2.size());

		CHECK(not contains_elements(l2, contents),
			"After inserting " << i << ", the contents of the list are incorrect\n" <<
			"List: " << print_fwd(l2));
			
    }
    
    OK();

    std::cout << "Checking .insert() in between 5 and 6 of 0, 1, ..., 5, 6, ..., 10; ";
    std::cout.flush();
    dlist l3;
    contents.clear();
    dlist::node* m = nullptr;
    for(int i = 0; i < 11; ++i) {
		// We know this is safe, because we've already tested insert at the tail.
        l3.insert(l3.tail(), i); 
        contents.push_back(i);
        if(i == 5) 
            m = l3.tail(); // Save pointer to node 5, we'll insert after this later.      
    }    

    // Actually insert some values in the middle
    for(int i = 100; i < 120; ++i) {

        l3.insert(m, i);

        // Again, we have to adjust for the fact that list inserts after
        contents.insert(contents.begin() + (5+1), i);
        
        if(!verify(l3))
            return false;

        CHECK(not contains_elements(l3, contents),
        	"After inserting " << i << ", the contents of the list are incorrect: \n" <<
        	"List:           " << print_fwd(l3) << "\n" <<
        	"Should contain: " << contents);
    }

    CHECK(l3.size() != 11 + 20,
    	"After inserting 31 elements, list.size() = " << l3.size());
    	
    OK();

    return true;
}

bool test_remove() {
    using std::cout;
    using std::endl;

    dlist::node* n = nullptr;
    dlist l;
    std::vector<int> contents;

    cout << "Checking remove from empty list... ";
    cout.flush();
    dlist e;
    e.remove(nullptr); // This will most likely crash if it fails at all
    if(!verify(e))
        return false;
        
    OK();

    // We know that insert works at this point, so we use it to construct
    // the list, and we don't bother to verify.
    for(int i = 0; i < 30; ++i) {
        l.insert(l.tail(), i);
        contents.push_back(i);
        if(i == 15)
            n = l.tail(); // Save pointer
    }

    cout << "Checking remove 10 elems. from the head of 0, 1, 2, ..., 29; ";
    cout.flush();
    for(int i = 0; i < 10; ++i) {

		int value = l.head()->value;
		(void)value;
     
        l.remove(l.head());
        contents.erase(contents.begin());
        
        if(!verify(l))
            return false;

        CHECK(not contains_elements(l, contents),
        	"After removing " << value << " at head, list contents are incorrect:\n" <<
        	"List:           " << print_fwd(l) << "\n" <<
        	"Should contain: " << contents);
    }
    
    OK();

    cout << "Checking remove 10 elems. from the tail of 10, 11, ..., 28, 29; ";
    cout.flush();
    for(int i = 0; i < 10; ++i) {

		int value = l.tail()->value; 
		(void)value;
        
        l.remove(l.tail());
        contents.pop_back();
        
        if(!verify(l))
            return false;

        CHECK(not contains_elements(l, contents),
        	"After removing " << value << " at tail, list contents are incorrect:\n" <<
        	"List:           " << print_fwd(l) << "\n" <<
        	"Should contain: " << contents);     
    }
    
    OK();

    cout << "Checking remove from the middle by removing 15... ";
    int value = n->value;
    (void)value;
    l.remove(n);
    contents.erase(contents.begin() + 5);
    
    if(!verify(l))
        return false;

    CHECK(not contains_elements(l, contents),
      	"After removing " << value << " in the middle, list contents are incorrect:\n" <<
      	"List:           " << print_fwd(l) << "\n" <<
      	"Should contain: " << contents); 	

	OK();

	cout << "Removing remaining elements... ";
	for(int i = 0; i < 9; ++i)
		l.remove(l.head());

	if(!verify(l))
		return false;

	CHECK(not l.empty(),
		"After removing all elements, list is not empty()\n" <<
		"List: " << print_fwd(l) << "\n");
        
    OK();
    return true;
}

bool test_push_back() {
    using std::cout;
    using std::endl;

    dlist e;

    cout << "Checking push_back with 0, 1, ..., 19; ";
    cout.flush();
    for(int i = 0; i < 20; ++i) {

        e.push_back(i);
        if(!verify(e))
            return false;

		CHECK(e.tail()->value != i,
			"Last push_back'd value = " << i << ", but tail = " << e.tail()->value);

		CHECK(e.size() != i + 1,
			"After push_back'ing " << i + 1 << " values, size is " << e.size());
    }

    CHECK(not contains_elements(e, {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}),
    	"After push_back'ing 0, 1, 2, ..., 18, 19, list does not contain correct elements:\n"
    	"List: " << print_fwd(e));

    OK();
    return true;
}

bool test_push_front() {
    using std::cout;
    using std::endl;

    dlist e;

    cout << "Checking push_front with 0, 1, 2, ..., 18, 19; ";
    cout.flush();
    for(int i = 0; i < 20; ++i) {
        e.push_front(i);
        
        if(!verify(e))
            return false;

		CHECK(e.head()->value != i,
			"After push_front'ing " << i << ", head->value = " << e.head()->value);

		CHECK(e.size() != i+1,
			"After push_front'ing " << i+1 << " values, size() = " << e.size());
    }

	CHECK(not contains_elements(e, {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0}),
		"After push_front'ing 0, 1, 2, ..., 18, 19, list does not contain correct elements: \n"
		"List: " << print_fwd(e));
    
    OK();
    return true;
}

bool test_pop_front() {
    using std::cout;
    using std::endl;
    dlist e;

    cout << "Checking pop_front from empty list... ";
    cout.flush();
    dlist l;
    l.pop_front();
    if(!verify(l))
        return false;
        
    OK();

    // Construct list using insert. 0-9
    for(int i = 0; i < 10; ++i)
        e.insert(e.tail(), i);

    cout << "Checking pop_front from 0, 1, ..., 8, 9; ";
    cout.flush();
    for(int i = 0; i < 10; ++i) {
        e.pop_front();
        if(!verify(e))
            return false;

        CHECK(e.size() != 9 - i,
        	"List should have " << 9-i << " elements, but size() = " << e.size());        	
    }

	CHECK(not e.empty(),
		"After pop_front'ing all elements, the list is not empty().\n" <<
		"List: " << print_fwd(e));
    
    OK();

    return true;
}

bool test_pop_back() {
    using std::cout;
    using std::endl;
    dlist e;

    cout << "Checking pop_back from empty list... ";
    cout.flush();
    dlist l;
    l.pop_back();
    if(!verify(l))
        return false;
        
    OK();

    // Construct list using insert. 0-9
    for(int i = 0; i < 10; ++i)
        e.insert(e.tail(), i);

    cout << "Checking pop_back from 0, 1, ..., 8, 9; ";
    cout.flush();
    for(int i = 0; i < 10; ++i) {
        e.pop_back();
        
        if(!verify(e))
            return false;

        CHECK(e.size() != 9-i,
        	"List should have " << 9 - i << " elements, but size() = " << e.size());    
    }
    
    OK();
    return true;
}

bool test_equal() {
    using std::cout;

    dlist a, b, c, e;

    cout << "Checking == operator... ";
    cout.flush();

    for(int i = 0; i < 10; ++i) {
        a.push_back(i);
        b.push_back(i);
        c.push_back(i + 10);
    }

	CHECK(not (a == b),
		"Two equal lists are not ==.\n" <<
		"List A: " << print_fwd(a) << "\n" <<
		"List B: " << print_fwd(b));

	CHECK(a == c,
		"Two non-equal lists count as ==.\n" <<
		"List A: " << print_fwd(a) << "\n" << 
		"List B: " << print_fwd(c));

	CHECK(a == e,
		"A non-empty list is == to an empty list.\n" <<
		"List A: " << print_fwd(a) << "\n" << 
		"List B: " << print_fwd(e));

	CHECK(e == a,
		"An empty list is == to a non-empty list.\n" <<
		"List A: " << print_fwd(e) << "\n" << 
		"List B: " << print_fwd(a));			

    // Test with lists of different lengths
    // Note that a and b are equal, except that b has an extra element!
    c.push_back(12); 
    b.push_back(12); 

	CHECK(a == c || a == b,
		"Two non-equal lists count as ==.\n" <<
		"List A: " << print_fwd(a) << "\n" << 
		"List B: " << print_fwd(a == c ? c : b));		

    // Check for shorter lists
    c.pop_back(); c.pop_back(); b.pop_back(); b.pop_back();

	CHECK(a == c || a == b,
		"Two non-equal lists count as ==.\n" <<
		"List A: " << print_fwd(a) << "\n" << 
		"List B: " << print_fwd(a == c ? c : b));	

    CHECK(not (a == a) or not (b == b) or not (c == c) or not (e == e), 
    	"A list must be == to itself.");

    OK();

    return true;    
}

bool test_concat() {
    using std::cout;

    dlist a, b;

    cout << "Checking + operator on {0, 1, ..., 9} + {10, 11, .., 19}... ";
    cout.flush();

    for(int i = 0; i < 10; ++i) {
        a.push_back(i);
        b.push_back(i+10);
    }

	// This shouldn't be necessary, but just to be on the safe side.
    if(!verify(a) || !verify(b))
        return false;

    dlist c = a + b;

	CHECK(a.tail()->next == b.head(),
		"a + b must not modify a or b (a's tail points to b)");
    
    if(!verify(c))
        return false;

    // TODO: check to make sure c does not share structure with a or b.
    // This could manifest when a or b is empty (resulting in c sharing with 
    // the other list), or if neither is empty, by just modifying the tail->next
    // of a. Either way, it's incorrect.

    // Check to make sure all elements are present
    int i = 0;
    for(dlist::node* n = c.head(); n != nullptr; n = n->next) {
		CHECK(n->value != i,
			"Elements resulting from + are incorrect. Expected 0, 1, ..., 18, 19.\n" <<
			"Found: " << print_fwd(c));

		++i;
    }

    OK();
    
    cout << "Checking operator + with empty lists... ";
    cout.flush();

    // Check concat with the empty list
    dlist e;
    dlist a2 = a + e;

    if(!verify(a2))
        return false;

    CHECK(a2.head() == a.head() || a2.tail() == a.tail(),
        "Result of <empty list> + b must not share nodes with b.\n");

    i = 0;
    for(dlist::node* n = a2.head(); n != nullptr; n = n->next) {
    	CHECK(n->value != i,
    		"After concatenating with the empty list, the result has the wrong elements.\n" <<
    		"Result: " << print_fwd(a2));
    	++i;
    }

    a2 = e + a;
    i = 0;
    for(dlist::node* n = a2.head(); n != nullptr; n = n->next) {
    	CHECK(n->value != i,
    		"After concatenating with the empty list, the result has the wrong elements.\n" <<
    		"Result: " << print_fwd(a2));
    	++i;
    }

    CHECK(a2.head() == a.head() || a2.tail() == a.tail(),
        "Result of <empty list> + b must not share nodes with b.");

    dlist e2 = e + e;
    if(!verify(e2))
        return false;

	CHECK(not e2.empty(),
		"Concatenating two empty lists results in a non-empty list.\n" <<
		"Result: " << print_fwd(e2));

    OK();

    return true;
}

bool test_reverse() {
    using std::cout;

    dlist a, e;

    cout << "Checking reverse() of empty list... ";
    cout.flush();

    dlist er = reverse(e);
    if(!verify(er))
        return false;

	CHECK(not er.empty(),
		"reverse() of the empty list should be empty, but it isn't.\n" <<
		"List: " << print_fwd(er));

	OK();
	
	cout << "Checking reverse of 0, 1, ..., 8, 9; ";
	cout.flush();

    for(int i = 0; i < 10; ++i)
        a.push_back(i);

    dlist ar = reverse(a);

    int i = 0, j = 9;
    for(dlist::node* m = a.head(), *n = ar.head(); 
        m != nullptr && n != nullptr; 
        m = m->next, n = n->next) {

		CHECK(m->value != i,
			"reverse() must not modify its argument." << 
			"Modified input list: " << print_fwd(a));
			
		++i;

		CHECK(n->value != j,
			"Elements of the reverse()'d list are incorrect: \n" <<
			"List: " << print_fwd(ar));

		--j;
    }

    OK();
    
    cout << "Checking double-reverse... ";
    cout.flush();

	CHECK(!(reverse(reverse(a)) == a),
		"reverse(reverse(a)) != a");

	OK();

    return true;
}

int main() {

    std::cout << "\u001b[37;1m*** Starting dlist tests ***\u001b[0m" << std::endl;

	// 
    if(not test_empty()      or
       not test_insert()     or
       not test_remove()     or
       not test_push_back()  or
       not test_push_front() or
       not test_pop_front()  or
       not test_pop_back()   or
       not test_equal()      or
       not test_concat()     or 
       not test_reverse()) {
        // At least one test did not pass.
    	return 1;   	
    }
    else {
    	// All tests passed
        std::cout << "\u001b[32;1m*** All tests passed! ***\u001b[0m" << std::endl;   
    	return 0;
   	}
}