#include <iterator>
#include <memory>
#include <stdint.h>
#include <list>
#include <utility>
#include <memory>
#include <ctime>

template<typename T>
struct XorNode {
public:
	using node_ptr = std::shared_ptr<XorNode<T> >;
	using c_node_ptr = XorNode<T>*;

	XorNode(const T& val, node_ptr left = nullptr, node_ptr right = nullptr) : value(val) {
		if (right != nullptr && left != nullptr)
			address_xor = (intptr_t) xor_two_ptrs(left.get(), right.get());
		else if (right != nullptr)
			address_xor = (intptr_t) right.get();
		else if (left != nullptr)
			address_xor = (intptr_t) left.get();
		else
			address_xor = (intptr_t) nullptr;
	}
	XorNode(T&& val, node_ptr left = nullptr, node_ptr right = nullptr) : value(val) {
		if (right != nullptr && left != nullptr)
			address_xor = (intptr_t) xor_two_ptrs(left.get(), right.get());
		else if (right != nullptr)
			address_xor = (intptr_t) right.get();
		else if (left != nullptr)
			address_xor = (intptr_t) left.get();
		else
			address_xor = (intptr_t) nullptr;
	}

	XorNode<T>* xor_two_ptrs(c_node_ptr left, c_node_ptr right) {
		return (XorNode<T>*) ((intptr_t) left ^ (intptr_t) right);
	}

	void add_ptr_to_xor(c_node_ptr ptr) {
		if (address_xor == (intptr_t) nullptr)
			address_xor = (intptr_t) ptr;
		else
			address_xor = (intptr_t) xor_two_ptrs((c_node_ptr) address_xor, ptr);
	}

	c_node_ptr get_other_ptr(c_node_ptr ptr) {
		if (address_xor == (intptr_t) ptr)
			return nullptr;

		c_node_ptr other = xor_two_ptrs((c_node_ptr) address_xor, ptr);

		if (other == nullptr)
			return nullptr;
		else
			return other;
	}

	XorNode<T>* get_address_xor() {
		return (c_node_ptr) address_xor;
	}

	void delete_ptr_from_xor_addr(c_node_ptr ptr) {
		add_ptr_to_xor(ptr); //xor is a reverse operation for xor
	}

	void replace_ptr_from_xor_addr(c_node_ptr old_ptr, c_node_ptr new_ptr) {
		delete_ptr_from_xor_addr(old_ptr);
		add_ptr_to_xor(new_ptr);
	}

	T& get_value() {
		return value;
	}

	~XorNode() {}

private:
	intptr_t address_xor = (intptr_t) nullptr;
	T value;
};

template<typename T, class Alloc>
class XorList;

template<typename T, class Alloc>
class XorListIterator { //s: public std::iterator<std::random_access_iterator_tag, T> {
public:
	friend XorList<T, Alloc>;

	using node_ptr = XorNode<T>*;
	using shared_ptr = std::shared_ptr<XorNode<T> >;
	using c_node_ptr = XorNode<T>*;

	XorListIterator(node_ptr prev, node_ptr rec_node) : node(rec_node), prev_node(prev) {}
	XorListIterator() = default;

	XorListIterator(const XorListIterator &other) = default;

	T& operator->() {
		return node->get_value();
	}

	T& operator*() {
		return node->get_value();
	}

	XorListIterator& operator++() {
		node_ptr next = node->get_other_ptr(prev_node);
		prev_node = node;
		node = next;

		return (*this);
	}

	XorListIterator& operator++(int) {
		//XorListIterator bi(*this);
		XorListIterator *bi = new XorListIterator(*this);
		operator++();
		return *bi;
	}

	XorListIterator& operator--() {
		node_ptr more_prev = prev_node->get_other_ptr(node);

		node = prev_node;
		prev_node = more_prev;

		return (*this);
	}

	XorListIterator& operator--(int) {
		XorListIterator* bi = new XorListIterator(*this);
		operator--();
		return *bi;
	}

	node_ptr make_node_ptr(std::shared_ptr<XorNode<T> >& ptr) {
		return ptr.get();
	}

	node_ptr next() {
		return get_node()->get_other_ptr(get_prev_node());
	}

private:
	c_node_ptr & get_prev_node() {
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

template<typename T, class Alloc>
class XorList {
private:

public:
	using sizetype = size_t;

	using c_node_ptr = XorNode<T>*;
	using node_ptr = c_node_ptr;
	using node = XorNode<T>;
	using iterator = XorListIterator<T, Alloc>;
	using alloc_traits = std::allocator_traits<Alloc>;

	explicit XorList(const Alloc& alloc = Alloc()) : _alloc(alloc) { init(); }

	XorList(sizetype count, const T& value = T(), const Alloc& alloc = Alloc()) {
		for (size_t i = 0; i < count; i++) {
			push_back(value);
		}
	}

	XorList(XorList&&) = default;

	sizetype size();
	void _delete(node_ptr);

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

	~XorList() {
		auto prev = head;
		auto node = head->get_address_xor();

		for (size_t i = 0; i < sz - 1; i++) {
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
	Alloc _alloc = Alloc();

	node_ptr make_shared(XorNode<T>* ptr) {
		return std::shared_ptr<XorNode<T> >(ptr);
	}

	node_ptr create_node(T&& value) {
		//return std::make_shared<XorNode<T> >(XorNode<T>(std::move(value)));
		return new XorNode<T>(std::move(value));
	}

	void init() {

	}

	node_ptr create_node(const T& value) {
		//return std::make_shared<XorNode<T> >(XorNode<T>(value));
		return new XorNode<T>(value);
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
			//std::cout << "SHHHHIIIIIITTTT!!!!!!";
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
};

template<typename T, class Alloc>
typename XorList<T, Alloc>::sizetype XorList<T, Alloc>::size() {
	return sz;
}

template<typename T, class Alloc>
void XorList<T, Alloc>::_delete(XorList<T, Alloc>::node_ptr ptr) {
	alloc_traits::destroy(_alloc, ptr);
	alloc_traits::deallocate(_alloc, reinterpret_cast<int*>(ptr), 1);
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