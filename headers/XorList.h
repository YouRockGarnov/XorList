#pragma once
#include "../StackAllocator/headers/StackAlloc.h"
#include "XorNode.h"
#include "XorListIterator.h"
#include "ConstXorListIterator.h"
#include <list>
#include <utility>
#include <memory>
#include <ctime>

template<typename T, class Alloc>
class XorList {
private:

public:
	using sizetype = std::size_t;
	
	using c_node_ptr = XorNode<T>*;
	using node_ptr = c_node_ptr;
	using node = XorNode<T>;
	using XorNodeAlloc = typename Alloc::template rebind<node>::other;

	using iterator = XorListIterator<T, Alloc>;
	using alloc_traits = std::allocator_traits<XorNodeAlloc>;

	explicit XorList(const Alloc& alloc = Alloc()) : _alloc(alloc) {}

	XorList(sizetype count, const T& value = T(), const Alloc& alloc = Alloc()) : _alloc(XorNodeAlloc(alloc)) {
		for (size_t i = 0; i < count; ++i) {
			push_back(value);
		}
	}

	XorList(const XorList& xlist) : _alloc(xlist._alloc), head(nullptr),
		tail(nullptr) {
		auto iter = xlist.begin();
		while (sz != xlist.sz) {
			push_back(*iter);
		}
	}

	XorList(XorList&& xlist) : _alloc(std::move(xlist._alloc)), head(nullptr),
		tail(nullptr) {
		auto iter = xlist.begin();
		while (sz != xlist.size()) {
			push_back(std::move(*iter));
		}
	}

	XorList& operator=(const XorList& xlist);
	XorList& operator=(XorList&& xlist) noexcept;

	sizetype size();
	void _delete(node_ptr);
	void clear();

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
		return XorListIterator<T, Alloc>(nullptr, head);
	}

	ConstXorListIterator<T, Alloc> begin() const {
		return ConstXorListIterator<T, Alloc>(nullptr, head);
	}

	~XorList() {
		auto prev = head;
		auto node = head->get_address_xor();

		for (size_t i = 0; i < sz-1; i++) {
			auto next = node->get_other_ptr(prev);
			_delete(prev);
			prev = node;
			node = next;
		}

		_delete(tail);
	}

private:
	node_ptr head = nullptr;
	node_ptr tail = nullptr;
	sizetype sz = 0;
	XorNodeAlloc _alloc = XorNodeAlloc();

	node_ptr make_shared(XorNode<T>* ptr);

	node_ptr create_node(T&& value);

	node_ptr create_node(const T& value);


	void single_insert_before(iterator&, c_node_ptr&);
	void single_insert_after(iterator&, c_node_ptr&);

	void create_head_tail(const T& val);

	void create_head_tail(T&& val);

	void push_after(node_ptr& pushing, node_ptr& ending);

	void push_between(c_node_ptr& left, c_node_ptr& pushing, c_node_ptr& right);
};

#include "xorlist.ipp"