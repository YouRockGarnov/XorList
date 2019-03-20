#pragma once
#include <iterator>
 #include "XorNode.h"

template<typename T, class Alloc>
class XorList;

 template<typename T, class Alloc>
 class ConstXorListIterator { //s: public std::iterator<std::random_access_iterator_tag, T> {
 public:
	 //friend const XorList<T, Alloc>;

 	using node_ptr = XorNode<T>*;
	using shared_ptr = std::shared_ptr<XorNode<T> >;
	using c_node_ptr = XorNode<T>*;

 	ConstXorListIterator(node_ptr prev, node_ptr rec_node) : node(rec_node), prev_node(prev) {}
 	ConstXorListIterator() = default;

 	ConstXorListIterator(const ConstXorListIterator &other) = default;

 	T& operator->() {
 		return node->get_value();
 	}

 	T& operator*() {
 		return node->get_value();
 	}

 	ConstXorListIterator& operator++() {
 		node_ptr next = node->get_other_ptr(prev_node);
 		prev_node = node;
 		node = next;

 		return (*this);
 	}

 	ConstXorListIterator& operator++(int) {
        //ConstXorListIterator bi(*this);
        ConstXorListIterator *bi = new ConstXorListIterator(*this);
        operator++();
        return *bi;
    }

 	ConstXorListIterator& operator--() {
		node_ptr more_prev = prev_node->get_other_ptr(node);

 		node = prev_node;
 		prev_node = more_prev;

 		return (*this);
 	}

 	ConstXorListIterator& operator--(int) {
 		ConstXorListIterator* bi = new ConstXorListIterator(*this);
 		operator--();
 		return *bi;
 	}

// 	bool operator==(const ConstXorListIterator &other) const {
// 		return (index == other.index && list_ptr == other.list_ptr);
// 	}
//
// 	bool operator<(const ConstXorListIterator &other) const {
// 		return (index < other.index);
// 	}
//
// 	bool operator!=(const ConstXorListIterator &other) const {
// 		return !((*this) == other);
// 	}
//
// 	bool operator<=(const ConstXorListIterator &other) const {
// 		return (index <= other.index);
// 	}

	node_ptr make_node_ptr(std::shared_ptr<XorNode<T> >& ptr) {
		return ptr.get();
	}

	node_ptr next() {
		return get_node()->get_other_ptr(get_prev_node());
	}

 private:
	 c_node_ptr& get_prev_node() {
		 return prev_node;
	 }

	 c_node_ptr& get_node() {
		 return node;
	 }

	 void replace_prev_node(c_node_ptr ptr) {
		 prev_node = ptr;
	}

 	node_ptr prev_node; //near to head
	node_ptr node; //near to tail
 };
