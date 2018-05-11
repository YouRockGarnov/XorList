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
	//using node_ptr = std::shared_ptr<XorNode<T, Alloc> >;
	
	using c_node_ptr = XorNode<T, Alloc>*;
	using node_ptr = c_node_ptr;
	using node = XorNode<T, Alloc>;
	using iterator = XorListIterator<T, Alloc>;

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

	void insert_before(iterator& iter, const T&);
	void insert_before(iterator& iter, T&& val);
	void insert_after(iterator& iter, const T&);
	void insert_after(iterator& iter, T&& val);
	void erase(iterator&);

	XorListIterator<T, Alloc> begin() {
		//return XorListIterator<T, Alloc>(make_shared(head->get_address_xor()), head);
		return XorListIterator<T, Alloc>(nullptr, head);
	}

private:
	node_ptr head = nullptr;
	node_ptr tail = nullptr;
	sizetype sz = 0;
	Alloc _alloc = Alloc();

	node_ptr make_shared(XorNode<T, Alloc>* ptr) {
		return std::shared_ptr<XorNode<T, Alloc> >(ptr);
	}

	node_ptr create_node(T&& value) {
		//return std::make_shared<XorNode<T, Alloc> >(XorNode<T, Alloc>(std::move(value)));
		return new XorNode<T, Alloc>(std::move(value));
	}

	void init() {

	}

	node_ptr create_node(const T& value) {
		//return std::make_shared<XorNode<T, Alloc> >(XorNode<T, Alloc>(value));
		return new XorNode<T, Alloc>(value);
	}

	void create_head_tail(const T& val) {
		sz++;
		node_ptr t = create_node(val);
		tail = t;
		head = t;
	}

	void push_after(node_ptr& pushing, node_ptr& ending) {
		if (sz == 0)
			create_head_tail(pushing->get_value());
		else if (ending == nullptr) {
			sz++;

			ending = pushing;
			std::cout << "SHHHHIIIIIITTTT!!!!!!";
		}

		else {
			sz++;

			ending->add_ptr_to_xor(pushing); //ptr on XorNode, not on value
			pushing->add_ptr_to_xor(ending);
			ending = pushing;
		}
	}

	void push_between(c_node_ptr& left, c_node_ptr& pushing, c_node_ptr& right) {
		if (sz == 0) {
			create_head_tail(pushing->get_value());
		} else if (sz == 1) {
			if(left == nullptr)
				push_after(pushing, right);
			else
				push_after(pushing, left);

			return;
		}

		sz++;

        if(left == nullptr){
            pushing->add_ptr_to_xor(right);
            right->add_ptr_to_xor(pushing);

			right = pushing;
			head = pushing;
        } else if (right == nullptr){
            pushing->add_ptr_to_xor(left);
            left->add_ptr_to_xor(pushing);

			tail = pushing;
			left = pushing;
        } else {
            pushing->add_ptr_to_xor(left);
            pushing->add_ptr_to_xor(right);

            left->replace_ptr_from_xor_addr(right, pushing);
            right->replace_ptr_from_xor_addr(left, pushing);
        }
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
		head = prev; //need check!!! //DELETE
	} else
		if (tail == head)
			tail = head = nullptr;
		else
			head = nullptr; //STRANGE
}

template<typename T, class Alloc>
void XorList<T, Alloc>::pop_back() { //TODO CHECK IF HEAD == TAIL
	sz--;

	XorNode<T, Alloc>* next = tail->get_address_xor();

	if (next != nullptr) {
		next->delete_ptr_from_xor_addr(tail);
		tail = next;
	} else
		if (tail == head) {
			tail = nullptr;
			head = nullptr;
		}
		else
			tail = nullptr;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_before(iterator& iter, const T& val) {
	c_node_ptr inserting = new node(val);

	if (sz == 1) {
		sz++;

		head = inserting;
		iter.prev_node = inserting;
		iter.node = tail;

		head->add_ptr_to_xor(tail);
		tail->add_ptr_to_xor(head);
	} else {
		push_between(iter.prev_node, inserting, iter.node);
		iter.replace_prev_node(inserting);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_before(iterator& iter, T&& val) {
	c_node_ptr inserting = new node(std::move(val));

	if (sz == 1) {
		sz++;

		head = inserting;
		iter.prev_node = inserting;
		iter.node = tail;

		head->add_ptr_to_xor(tail);
		tail->add_ptr_to_xor(head);
	} else {
		push_between(iter.prev_node, inserting, iter.node);
		iter.replace_prev_node(inserting);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_after(iterator& iter, const T& val) {
	c_node_ptr inserting = new node(val);

	if (sz == 1) {
		sz++;

		tail = inserting;
		iter.prev_node = head;
		iter.node = tail;

		head->add_ptr_to_xor(tail);
		tail->add_ptr_to_xor(head);
	} else {
		c_node_ptr next = iter.next();
		push_between(iter.node, inserting, next);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_after(iterator& iter, T&& val) {
	c_node_ptr inserting = new node(std::move(val));

	if (sz == 1) {
		sz++;

		tail = inserting;
		iter.prev_node = head;
		iter.node = tail;

		head->add_ptr_to_xor(tail);
		tail->add_ptr_to_xor(head);
	} else {
		c_node_ptr next = iter.next();
		push_between(iter.node, inserting, next);
	}
	//iter.replace_prev_node(inserting);
}

template<typename T, class Alloc>
void XorList<T, Alloc>::erase(iterator& iter) {
	sz--;
	c_node_ptr left = iter.prev_node;
	c_node_ptr right = iter.next();
	c_node_ptr toDelete = iter.node;

	if (left != nullptr)
		left->replace_ptr_from_xor_addr(toDelete, right);
	else
		head = right;
	if (right != nullptr)
		right->replace_ptr_from_xor_addr(toDelete, left);
	else
		tail = left;
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