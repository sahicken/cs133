#include "dlist.hpp"

	void
dlist::remove(node* which)
{
	if (which!=nullptr) {
		if (which==head()) {
			_head=which->next;
			which->next->prev=nullptr;
		} else if (which==tail()) {
			_tail=which->prev;
			which->prev->next=nullptr;
		} else {
			which->prev->next=which->next;
			which->next->prev=which->prev;
		}
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
		n->next->prev=n;
		if (tail()==n->prev) _tail=n;
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
	n->next->prev=n;
	_head=n;
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
	for(auto i=a.head();i!=a.tail();i=i->next)
		dl.push_back(i->value);
	for(auto j=b.head();j!=b.tail();j=j->next)
		dl.push_back(j->value);
	return dl;
}

dlist reverse(const dlist& l)
{
	dlist dl = dlist();
	for(auto i=l.head();i!=l.tail();i=i->next)
		dl.push_front(i->value);
	return dl;
}
