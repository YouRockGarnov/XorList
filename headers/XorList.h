#pragma once
#include "../StackAllocator/headers/StackAlloc.h"
#include "XorNode.h"
#include "XorListIterator.h"
#include <list>
#include <utility>
#include <memory>
#include <ctime>

template<typename T, class Alloc>
class XorList {
private:

public:
	using sizetype = size_t;
	using node_ptr = std::shared_ptr<XorNode<T, Alloc> >;
	using node = XorNode<T, Alloc>;

	explicit XorList(const Alloc& alloc = Alloc()) : _alloc(alloc) { init(); }

	XorList(sizetype count, const T& value = T(), const Alloc& alloc = Alloc());
	XorList(XorList&&) = default;

	sizetype size();

	void push_back(const T&);
	void push_back(T&&);
	void push_front(const T&);
	void push_front(T&&);

	void pop_back();
	void pop_front();

	XorListIterator<T, Alloc> begin() {
		//return XorListIterator<T, Alloc>(make_shared(head->get_address_xor()), head);
		return XorListIterator<T, Alloc>(nullptr, head);
	}

private:
	node_ptr head = nullptr;
	node_ptr tail = nullptr;
	sizetype sz = 0;
	Alloc _alloc = std::make_shared<Alloc>(new Alloc());

	node_ptr make_shared(XorNode<T, Alloc>* ptr) {
		return std::shared_ptr<XorNode<T, Alloc> >(ptr);
	}

	node_ptr create_node(T&& value) {
		return std::make_shared<XorNode<T, Alloc> >(XorNode<T, Alloc>(std::move(value)));
	}

	node_ptr create_node(const T& value) {
		sz++;
		return std::make_shared<XorNode<T, Alloc> >(XorNode<T, Alloc>(value));
	}

	void init() {

	}

	void create_head_tail(const T& val) {
		node_ptr t = create_node(val);
		tail = t;
		head = t;
	}

	void push_after(node_ptr& pushing, node_ptr& ending) {
		sz++;

		ending->add_ptr_to_xor(pushing); //ptr on XorNode, not on value
		pushing->add_ptr_to_xor(ending);
		ending = pushing;
	}
};

template<typename T, class Alloc>
typename XorList<T, Alloc>::sizetype XorList<T, Alloc>::size() {
	return sz;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::push_back(const T& val) {
	if (sz == 0)
		create_head_tail(val);
	else {
		node_ptr t = create_node(val);
		push_after(t, tail);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::push_back(T&& val) { 
	if (sz == 0)
		create_head_tail(val);
	else {
		node_ptr t = create_node(std::move(val));
		push_after(t, tail);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::push_front(const T& val) {
	if (sz == 0)
		create_head_tail(val);
	else {
		node_ptr t = create_node(val);
		push_after(t, head);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::push_front(T&& val) {
	if (sz == 0)
		create_head_tail(val);
	else {
		node_ptr t = create_node(std::move(val));
		push_after(t, head);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::pop_front() {
	sz--;

	XorNode<T, Alloc>* prev = head->get_address_xor();

	if (prev != nullptr) {
		prev->delete_ptr_from_xor_addr(head);
		head = std::shared_ptr<node>(prev); //need check!!!
	} else
		if (tail == head)
			tail = head = nullptr;
		else
			tail = nullptr;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::pop_back() { //TODO CHECK IF HEAD == TAIL
	sz--;

	XorNode<T, Alloc>* next = head->get_address_xor();

	if (next != nullptr) {
		next->delete_ptr_from_xor_addr(tail);
		tail = std::shared_ptr<node>(next);
	} else
		if (tail == head)
			tail = head = nullptr;
		else
			tail = nullptr;
}

void random_check(int count_of_oper) {
	srand(time(NULL));

	XorList<int, StackAlloc<int> > xl;

	int choice = std::rand() % 4;

	switch (choice) {
		case 0 :{
			xl.push_back(int(std::rand()));
			break;
		}

		case 1:{
			xl.push_front(int(std::rand()));
			break;
		}

		case 2:
		{
			if (xl.size() != 0)
				xl.pop_back();
			break;
		}

		case 3:
		{
			if (xl.size() != 0)
				xl.pop_front();
			break;
		}

		default:
			break;
	}
}