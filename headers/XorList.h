#pragma once
#include "../StackAllocator/headers/StackAlloc.h"
#include "XorNode.h"
#include <list>
#include <utility>
#include <memory>

template<typename T, class Alloc>
class XorList {
private:
	

public:
	using sizetype = size_t;
	using node_ptr = std::shared_ptr<XorNode<T, Alloc> >;

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

private:
	node_ptr head = nullptr;
	node_ptr tail = nullptr;
	sizetype sz = 0;
	Alloc _alloc = std::make_shared<Alloc>(new Alloc());

	node_ptr create_node(T&& value) {
		return std::make_shared<XorNode<T, Alloc> >(XorNode<T, Alloc>(std::move(value)));
	}

	node_ptr create_node(const T& value) {
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

	T* prev = head->get_address_xor();
	head = std::make_shared<T>(prev);
}

template<typename T, class Alloc>
void XorList<T, Alloc>::pop_back() { //TODO CHECK IF HEAD == TAIL
	sz--;

	T* next = tail->get_address_xor();
	tail = std::make_shared<T>(next);
}