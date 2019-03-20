#include "XorList.h"

template<typename T, class Alloc>
XorList<T, Alloc>& XorList<T, Alloc>::operator=(const XorList& xlist) {
	if (this != &xlist) {
		clear();

		if (alloc_traits::propagate_on_container_copy_assignment::value) {
			_alloc = xlist._alloc;
		}

		auto iter = xlist.begin();
		while (sz != xlist.size()) {
			push_back(*iter);
			iter++;
		}
	}

	return *this;
}

template<typename T, class Alloc>
XorList<T, Alloc>& XorList<T, Alloc>::operator=(XorList&& xlist) noexcept {
	if (this != &xlist) {
		clear();

		if (_alloc == xlist._alloc) {
			head = xlist.head;
			tail = xlist.tail;
			sz = xlist.sz;
		} else {
			auto iter = xlist.begin();
			while (sz != xlist.size())
				push_back(*iter);
		}

		return *this;
	}
}

template<typename T, class Alloc>
typename XorList<T, Alloc>::sizetype XorList<T, Alloc>::size() {
	return sz;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::_delete(XorList<T, Alloc>::node_ptr ptr) {
	alloc_traits::destroy(_alloc, ptr);
	alloc_traits::deallocate(_alloc, ptr, 1);
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
		create_head_tail(std::move(val));
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

	XorNode<T>* prev = head->get_address_xor();

	if (prev != nullptr) {
		prev->delete_ptr_from_xor_addr(head);
		_delete(head);
		head = prev; //need check!!! //DELETE
	} else
		if (tail == head) {
			_delete(tail);
			tail = head = nullptr;
		}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::pop_back() { //TODO CHECK IF HEAD == TAIL
	sz--;

	XorNode<T>* next = tail->get_address_xor();

	if (next != nullptr) {
		next->delete_ptr_from_xor_addr(tail);
		_delete(tail);
		tail = next;
	} else
		if (tail == head) {
			_delete(head);
			tail = nullptr;
			head = nullptr;
		}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_before(iterator& iter, const T& val) {
	c_node_ptr inserting = create_node(val); //CHANGE

	if (sz == 1) {
		single_insert_before(iter, inserting);
	} else {
		push_between(iter.prev_node, inserting, iter.node);
		iter.replace_prev_node(inserting);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_before(iterator& iter, T&& val) {
	c_node_ptr inserting = create_node(std::move(val));

	if (sz == 1) {
		single_insert_before(iter, inserting);
	} else {
		push_between(iter.prev_node, inserting, iter.node);
		iter.replace_prev_node(inserting);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_after(iterator& iter, const T& val) {
	c_node_ptr inserting = create_node(val);

	if (sz == 1) {
		single_insert_after(iter, inserting);
	} else {
		c_node_ptr next = iter.next();
		push_between(iter.node, inserting, next);
	}
}

template<typename T, class Alloc>
void XorList<T, Alloc>::insert_after(iterator& iter, T&& val) {
	c_node_ptr inserting = create_node(std::move(val));

	if (sz == 1) {
		single_insert_after(iter, inserting);
	} else {
		c_node_ptr next = iter.next();
		push_between(iter.node, inserting, next);
	}
	//iter.replace_prev_node(inserting);
}

template<typename T, class Alloc>
void XorList<T, Alloc>::erase(iterator& iter) {
	sz--;
	c_node_ptr prev = iter.prev_node;
	c_node_ptr next = iter.next();
	c_node_ptr deleting = iter.node;

	if (prev != nullptr)
		prev->replace_ptr_from_xor_addr(deleting, next);
	else
		head = next;
	if (next != nullptr)
		next->replace_ptr_from_xor_addr(deleting, prev);
	else
		tail = prev;

	_delete(deleting);
}

template<typename T, class Alloc>
void XorList<T, Alloc>::clear() {
	while (sz > 0)
		pop_back();
}




template<typename T, class Alloc>
void XorList<T, Alloc>::single_insert_before(iterator& iter, c_node_ptr& inserting){
	sz++;

	head = inserting;
	iter.prev_node = inserting;
	iter.node = tail;

	head->add_ptr_to_xor(tail);
	tail->add_ptr_to_xor(head);
}

template<typename T, class Alloc>
void XorList<T, Alloc>::single_insert_after(iterator& iter, c_node_ptr& inserting) {
	sz++;

	tail = inserting;
	iter.prev_node = head;
	iter.node = tail;

	head->add_ptr_to_xor(tail);
	tail->add_ptr_to_xor(head);
}

template<typename T, class Alloc>
typename XorList<T, Alloc>::node_ptr XorList<T, Alloc>::make_shared(XorNode<T>* ptr) {
	return std::shared_ptr<XorNode<T> >(ptr);
}

template<typename T, class Alloc>
typename XorList<T, Alloc>::node_ptr XorList<T, Alloc>::create_node(T&& value) {
	c_node_ptr new_node = reinterpret_cast<c_node_ptr>(alloc_traits::allocate(_alloc, 1));
	alloc_traits::construct(_alloc, new_node, std::move(value));
	return new_node;
}

template<typename T, class Alloc>
typename XorList<T, Alloc>::node_ptr XorList<T, Alloc>::create_node(const T& value) {
	c_node_ptr new_node = reinterpret_cast<c_node_ptr>(alloc_traits::allocate(_alloc, 1));
	alloc_traits::construct(_alloc, new_node, value);
	return new_node;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::create_head_tail(const T& val) {
	sz++;
	node_ptr t = create_node(val);
	tail = t;
	head = t;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::create_head_tail(T&& val) {
	sz++;
	node_ptr t = create_node(std::move(val));
	tail = t;
	head = t;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::push_after(node_ptr& pushing, node_ptr& ending) {
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

template<typename T, class Alloc>
void XorList<T, Alloc>::push_between(c_node_ptr& left, c_node_ptr& pushing, c_node_ptr& right) {
	if (sz == 0) {
		create_head_tail(pushing->get_value());
	} else if (sz == 1) {
		if (left == nullptr)
			push_after(pushing, right);
		else
			push_after(pushing, left);

		return;
	}

	sz++;

	if (left == nullptr) {
		pushing->add_ptr_to_xor(right);
		right->add_ptr_to_xor(pushing);

		right = pushing;
		head = pushing;
	} else if (right == nullptr) {
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




void random_check(int count_of_oper) {
	srand(time(NULL));

	XorList<int, StackAlloc<int> > xl;

	int choice = std::rand() % 4;

	switch (choice) {
		case 0:
		{
			xl.push_back(int(std::rand()));
			break;
		}

		case 1:
		{
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