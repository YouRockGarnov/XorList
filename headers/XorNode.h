#pragma once
#include <memory>
#include <stdint.h>

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