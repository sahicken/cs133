#include "dlist.hpp"
#include <iostream>

	void
dlist::remove(node* which)
{
	if (which!=nullptr) {
		if (which==head()) {
			_head=which->next;
			if (which->next!=nullptr)
				which->next->prev=nullptr;
		} else if (which==tail()) {
			_tail=which->prev;
			which->prev->next=nullptr;
		} else {
			which->prev->next=which->next;
			which->next->prev=which->prev;
		}
		if(head()==nullptr)
			_tail=nullptr;
		if(tail()==nullptr)
			_head=nullptr;
		delete which;
	}
}

	void
dlist::pop_front()
{
	remove(head());
}

	void
dlist::pop_back()
{
	remove(tail());
}

	void
dlist::insert(node* previous, int value) 
{
	node* n=new node{value,nullptr,nullptr};
	if (previous==nullptr) _head=_tail=n;
	else {
		n->prev=previous;
		n->next=previous->next;
		previous->next=n;
		if (tail()==n->prev) _tail=n;
		else n->next->prev=n;
	}
}

	void 
dlist::push_back(int value)
{
	insert(tail(), value);
}

	void 
dlist::push_front(int value)
{
	node* n=new node{value,nullptr,nullptr};
	n->next=head();
	if (n->next!=nullptr)
		n->next->prev=n;
	_head=n;
	if(tail()==nullptr)
		_tail=head();
}

int
dlist::size() const
{
	node* n=head();
	int i=0;
	if(tail()!=nullptr)
		++i;
	while(n!=tail()){
		n=n->next;
		++i;
	}
	return i;
}

bool
dlist::empty() const
{
	return head()==nullptr&&tail()==nullptr;
}

dlist::node*
dlist::at(int n) const
{
	node* x=head();
	for (int i=0; i<n; ++i) {
		x=x->next;
	}
	return x;
}

bool operator== (const dlist& a, const dlist& b)
{
	bool equals = true;
	dlist::node* i = a.head();
	dlist::node* j = b.head();
	while (i!=a.tail()||j!=b.tail()) {
		if(i==nullptr||j==nullptr) {
			if (i!=nullptr||j!=nullptr)
				equals=false;
			break;
		}
		if(i->value!=j->value) {
			equals=false;
			break;
		} else {
			i=i->next;
			j=j->next;
		}
	}
	return equals;
}

dlist operator+ (const dlist& a, const dlist& b)
{
	dlist dl = dlist();
	if (a.head()!=nullptr) {
		for(auto i=a.head();i!=a.tail();i=i->next)
			dl.push_back(i->value);
		dl.push_back(a.tail()->value);
	}
	if (b.head()!=nullptr) {
		for(auto j=b.head();j!=b.tail();j=j->next)
			dl.push_back(j->value);
		dl.push_back(b.tail()->value);
	}
	return dl;
}

dlist reverse(const dlist& l)
{
	dlist dl = dlist();
	if (l.head()!=nullptr) {
		for(auto i=l.head();i!=l.tail();i=i->next)
			dl.push_front(i->value);
		dl.push_front(l.tail()->value);
	}
	return dl;
}
