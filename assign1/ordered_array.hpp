#pragma once
#include <stdexcept> // For out_of_range

class ordered_array {
  public:
    /* constructor
       Construct a new ordered_array with the given capacity (maximum size).
       The size of a new ordered_array should be 0.
    */
    ordered_array(int c); 

    // destructor
    ~ordered_array();

    /* size()
       Returns the size (number of elements in the array).
    */
    int size() const;

    /* capacity()
       Returns the maximum size of the array.
    */
    int capacity() const;

    /* insert(e)
       Insert e into the array. Note that it is OK to insert duplicates; if n 
       copies of a value are inserted into the array then n copies should appear
       in the array.

       If size() == capacity() then this does nothing.

       If e == -2147483648 then this does nothing (i.e., -2147483648 is not a
       valid value to insert).
    */
    void insert(int e);

    /* remove(e)
       Remove e from the array, if it exists. (If it does not exist, the
       array should be unchanged.) If multiple copies of e are present, only
       one should be removed.

       If e = -2147483648 then this does nothing.
    */
    void remove(int e);

    /* exists(e)
       Returns true if e is present at least once in the array.

       If e == -2147483648 then this returns false.
    */
    bool exists(int e) const;

    /* at(i)
       Returns the value in the array at index i, which should be >= 0 and <
       size(). 

       If i < 0 or i >= size(), then at(i) should throw a std::out_of_range
       exception. (This is what std::vector does in this situation.)

       Note that at() should *never* return -2147483648 (because insert() should
       never insert it).
    */
    int at(int i) const;

  private:
    // You can add any private members you need.
    int sz;
    int cap;
    int* data;
};
