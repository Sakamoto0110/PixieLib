#pragma once






using resize_expr = size_t(*)(size_t);

static constexpr size_t RESIZE_DISABLED(size_t val)  { return val; };
static constexpr size_t INCREASE_ONE(size_t val)     { return val + 1; };
static constexpr size_t INCREASE_QUARTER(size_t val) { return val + val / 4; };
static constexpr size_t INCREASE_HALF(size_t val)    { return val + val / 2; };
static constexpr size_t INCREASE_DOUBLE(size_t val)  { return val + val; };


static constexpr size_t DEFAULT_ARR_INITIAL_SIZE = 8;

template<typename T>
class pxContainer {
public:
	virtual size_t Size() const = 0;
	virtual void Add(T)=0;
	virtual void Remove(T)=0;
	virtual ~pxContainer() = default;


};



#include <iostream>
template<typename Tx, resize_expr expr = INCREASE_DOUBLE, size_t initial_size= DEFAULT_ARR_INITIAL_SIZE>
class pxArray : public pxContainer<Tx>{
public:
	pxArray() {
		_count = 0;
		_capacity = initial_size;
		_alloc(_capacity);
		
	}

	void Add(Tx _tx) override {
		if(_count+1 >= _capacity) {
			// realloc
		}
		_ptr[_count++] = _tx;
	}

	void Remove(Tx) override {
		
	}

	size_t Size() const override {
		return _count;
	}

	~pxArray() override {
		_count = 0;
		delete(_ptr);
		
	}


	void PrintArr() const {
		using std::cout;
		cout << "[";
		for (size_t i = 0; i < Size(); i++) {
			cout << _ptr[i];
			if (i < (Size() - 3))
				cout << ", ";
		}
		cout << "]\n";
	}

private:

	bool _alloc(size_t cb) {
		_ptr = new Tx[cb];
		return true;
	}
	bool _realloc(size_t newcb) {
		return true;
	}

	size_t _capacity;
	size_t _count;
	Tx* _ptr;
};

