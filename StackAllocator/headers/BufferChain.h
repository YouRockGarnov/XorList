#pragma once
#include <memory>
#include <list>

template<typename T>
using ptr = T*;//std::shared_ptr<T>;
				 //using chain = std::list<std::shared_ptr<T> >;

struct block {
	constexpr static const int buffer_capacity = 1024;
	char buffer[buffer_capacity];
};

using chain = std::list<block>;

class BufferChain {
public:

	BufferChain() {
		_chain.push_back(block());
	}

	template<typename T>
	ptr<T> get_ptr_for_type(size_t cnt) {
		if (rec_buf_size + sizeof(T)*cnt > block::buffer_capacity)
			add_block();

		ptr<T> p =  ptr<T>(::operator new(cnt * sizeof(T), _chain.back().buffer + rec_buf_size));
		rec_buf_size += sizeof(T)*cnt;
		return p;
	}

	void add_block() {
		_chain.push_back(block());
	}

private:
	chain _chain;
	int rec_buf_size = 0;
};