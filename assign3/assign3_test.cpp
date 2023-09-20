/*
 * assign3_test.cpp
 * Test runner for assignment 3
 */
#include <iostream>
#include <algorithm>
#include <functional>
#include <random>
#include <vector>

using namespace std;

std::default_random_engine generator(17);
std::uniform_int_distribution<int> distribution(0,9);
auto rnd = std::bind(distribution, generator); 

/* make_random_vector(len)
   Returns a vector<int> of random values, where each entry is between 0 and
   INT_MAX. The optional second parameter lets you specify the seed to be used 
   for the RNG.
*/
std::vector<int> make_random_vector(
    std::size_t len,
    int = 1) 
{
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
    int = 1) 
{
    std::vector<unsigned> ret(len, 0);

    // Initialize vector to 0...len-1
    for(std::size_t i = 0; i < len; ++i) 
        ret.at(i) = i;

    std::shuffle(ret.begin(), ret.end(), generator);

    return ret;

}

/* These functions must be defined in the student's code */
void merge(int* input, int size, int* output, bool output_asc);
void mergesort(int *input, int size, int* output, bool output_asc);

// Helper function for allocating temporary space. 
// Note that the allocated space must be destroyed by the caller!
int* mergesort(int* input, int size) 
{
    int* output = new int[size];
    mergesort(input, size, output, true);
    return output;
}

// Convenience function for using mergesort on vectors
int* mergesort(const vector<int>& data) 
{
    return mergesort(const_cast<int*>(data.data()), data.size());
}

/* is_sorted(data, size)
   Returns true if the data is sorted ascending.
*/
bool is_sorted(int* data, int size) {
    for(int* p = data + 1; p < data + size; ++p) {
        if(*p < *(p-1))
            return false;
    }

    return true;
}

bool is_permutation(int* input, int size, int* sorted) {
    for(int i = 0; i < size; ++i) {
        // Check if input[i] is in sorted
        int elem = input[i];
        bool found = false;
        for(int j = 0; j < size; ++j)
            if(sorted[j] == elem) {
                found = true;
                break;
            }

        if(!found)
            return false;

        // Check if sorted[i] is in input
        elem = sorted[i];
        found = false;
        for(int j = 0; j < size; ++j)
            if(input[j] == elem) {
                found = true;
                break;
            }

        if(!found)
            return false;
    }

    return true;
}

/* out << vec
   Convenience overload for printing vector<int>
*/
ostream& operator<<(ostream& out, const vector<int>& data) 
{
    out << "{";
    for(size_t i = 0; i < data.size() - 1; ++i)
        out << data[i] << (i != data.size() - 1 ? ", " : "");
    out << data.back() << "}";

    return out;
}

/* random_growth(start,size,asc)
   Generates a vector whose values start at `start` and either increase (if asc is
   true) or decrease by random increments. The increment is in the range 
   0...8. 
*/
[[deprecated]]
void random_growth(int* data, int start, int size, bool asc) 
{    
    const int step = asc ? +1 : -1;

    if(size > 0) {
        data[0] = start;
        for(int i = 1; i < size; ++i)
            data[i] = data[i-1] + step * rnd();
    }
}

void random_growth(
    vector<int>& data, 
    size_t start_index, 
    size_t size,
    int start,
    bool asc) 
{    
    const int step = asc ? +1 : -1;

    if(size > 0) {
        data[start_index] = start;
        for(size_t i = start_index + 1; i < start_index + size; ++i)
            data[i] = data[i-1] + step * rnd();
    }
}

/* test_merge()
   Test the merge() function.
   This basically just checks merge() to make sure that the output is merged
   in the correct order. We also check things like merging small arrays
   (size 0, 1, 2, and 3) since those are easy to get wrong. 
*/
bool test_merge() {

    cout << "Testing simple two-element merge: ";
    vector<int> v1 = { 1, 2 };         // Input array
    vector<int> vout = { -1, -1 };     // Output array

    // Merge asc.
    merge(&v1[0], 2, &vout[0], true);
    if(vout[0] != 1 || vout[1] != 2) {
        cout << "FAILED: after merge, expected {1, 2} but got " << 
                vout << ".\n";
        return false;
    }

    // Merge desc.
    merge(&v1[0], 2, &vout[0], false);
    if(vout[0] != 2 || vout[1] != 1) {
        cout << "FAILED: after merge, expected {2, 1} but got " << vout << ".\n";
        return false;
    }
    cout << "OK\n";

    // Generate asc-desc datasets for testing
    for(int split : { 0, 5, 10, 15, 20 }) {
        cout << "Testing 20-element merge (" << split << " and " << 20 - split <<  "): ";

        vector<int> data(20);
        random_growth(data, 0,     split,      0, true);    // Ascending section
        random_growth(data, split, 20 - split, 2, false);   // Descending section

        vector<int> dataout(20, -1000000);
        merge(&data[0], data.size(), &dataout[0], true);

        if(!is_sorted(&dataout[0], data.size())) {
            cout << "FAILED: merge did not produce ascending sorted output : "
                 << dataout << endl;
            return false;
        }

        if(not is_permutation(dataout.begin(), dataout.end(), data.begin())) {
            cout << "FAILED: `merge` did not produce a permutation of input array\n"
                 << "  Input data: " << data << "\n"
                 << "  Output:     " << dataout << "\n";
            return false;
        }

        cout << "OK\n";
    }

    cout << "Testing 21-element merge: ";
    vector<int> data(21, -1000000);
    random_growth(data, 0,  10, 0, true);
    random_growth(data, 10, 11, 2, false);
    vector<int> dataout(21, -1000000);
    merge(&data[0], data.size(), &dataout[0], true);

    if(!is_sorted(&dataout[0], data.size())) {
        cout << "FAILED: merge did not produce sorted output: "
             << dataout << endl;
        return false;
    }

    if(not is_permutation(dataout.begin(), dataout.end(), data.begin())) {
        cout << "FAILED: `merge` did not produce a permutation of input array\n"
             << "  Input data: " << data << "\n"
             << "  Output:     " << dataout << "\n";
        return false;
    }

    cout << "OK\n";

    return true;
}



/* test_mergesort()
   Test mergesort on a variety of inputs.
*/
bool test_mergesort() 
{
    cout << "Sorting empty sequence: ";
    vector<int> no_data;

    int* no_data_sorted = mergesort(no_data);
    // No data means nothing to check!
    delete[] no_data_sorted;
    cout << "OK\n";

    vector<int> sizes = {2, 3, 4, 5, 7, 8, 15, 16, 19, 20, 50, 64, 100, 128};

    for(int s : sizes) {

        // Test sorting a vector of random data
        vector<int> data = make_random_vector(s);
        vector<int> expected = data;
        std::sort(expected.begin(), expected.end());

        cout << "Sorting random vector of size " << s << " ";

        int* data_sorted = mergesort(data);
        bool result = true;
        if(!is_sorted(data_sorted, data.size())) {
            cout << "FAILED: result is not sorted:\n";

            cout << "  Expected: " << expected << "\n"; 
            cout << "  Result:   {";
            for(int* i = data_sorted; i != data_sorted + data.size() - 1; ++i)
                cout << *i << ", ";
            cout << data_sorted[data.size() - 1] << "}\n";

            result = false;
        }
        else if(!is_permutation(&data[0], data.size(), data_sorted)) {
            cout << "FAILED: result is not a permutation of the input sequence:\n";
            cout << "  Expected: " << expected << "\n"; 
            cout << "  Result:    {";
            for(int* i = data_sorted; i != data_sorted + data.size() - 1; ++i)
                cout << *i << ", ";
            cout << data_sorted[data.size() - 1] << "}\n";

            result = false;            
        }
        else
            cout << "OK\n";

        delete[] data_sorted;

        if(not result)        
            return false;
    }

    return true;

}

int main() {

    cout << "**** Testing mergesort ****\n";
    if(test_merge() && 
       test_mergesort())
        cout << "**** All tests passed! ****\n";    

    return 0;
}