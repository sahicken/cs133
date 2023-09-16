#include <stdexcept>
#include "ordered_array.hpp"

ordered_array::ordered_array(int c) :
	sz(0), cap(c) { data = new int[cap]; }
ordered_array::~ordered_array() { delete[] data; }

int ordered_array::size() 	const	{ return sz; }
int ordered_array::capacity() 	const 	{ return cap; }

int
ordered_array::at(int i) const
{
	if ((i < 0)||(i>=size()))
		throw std::out_of_range("bad index");
	else return data[i];
}

bool
ordered_array::exists(int e) const
{
	for (int i = 0; i < size(); ++i)
		if (e==at(i)) return true;
	return false;
}

void
ordered_array::insert(int e)
{
	if (e>-2147483648&&size()!=capacity()) {
		for (int i=0; i<size(); ++i) {
			if (e>at(i)) continue;
			else {
				int temp = at(i);
				data[i]=e;
				e = temp;
			}
		}
		data[sz]=e;
		++sz;
	}
}

void
ordered_array::remove(int e)
{
	bool flag=false;
	for (int i = 0; i < size(); ++i) {
		if (flag) data[i-1]=data[i];
		else if (e==at(i)) flag = true;
	}
	if (flag) --sz;
}

