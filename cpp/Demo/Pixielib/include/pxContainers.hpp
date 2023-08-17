#pragma once






using resize_expr = size_t(*)(size_t);

static constexpr size_t RESIZE_DISABLED(size_t val)  { return val; };
static constexpr size_t INCREASE_ONE(size_t val)     { return val == 0 ? 1 : 0 + val + 1; };
static constexpr size_t INCREASE_QUARTER(size_t val) { return val == 0 ? 1 : 0 + val + val / 4; };
static constexpr size_t INCREASE_HALF(size_t val)    { return val == 0 ? 1 : 0 + val + val / 2; };
static constexpr size_t INCREASE_DOUBLE(size_t val)  { return val==0?1:0 + val + val; };


static constexpr size_t DEFAULT_ARR_INITIAL_SIZE = 3;

template<typename T>
class pxContainer {
public:
	virtual size_t size() const = 0;
	virtual void add(const T&)=0;
	virtual void remove(size_t)=0;
	virtual T& get(size_t) = 0;
	virtual ~pxContainer() = default;

	void operator +=(const T& content) {
		add(content);
	}

	void operator -=(size_t index) {
		remove(index);
	}

	T& operator[](size_t index) {
		return get(index);
	}

};



#include <iostream>
template<typename T, resize_expr expr = INCREASE_DOUBLE, size_t initial_size= DEFAULT_ARR_INITIAL_SIZE>
class pxArray : public pxContainer<T>{
public:

	pxArray() : _ptr((T*)calloc(initial_size, sizeof(T))) {

	}

	pxArray(const pxArray& arr) :_capacity(arr._size), _ptr((T*)calloc(arr._size, sizeof(T))) {
		for (size_t i = 0; i < arr.size(); i++) {
			pxArray::add(arr._ptr[i]);
		}
	}
	

	T& get(size_t index) override {
		return _ptr[index];
	}

	void add(const T& _tx) override {
		if(_size >= _capacity) {
			// realloc
			if(!_realloc(expr(_capacity))) {
				return;
			}
		}
		_ptr[_size++] = _tx;
	}

	void remove(size_t index) override {
		for(size_t i = index; i < size(); i++) {
			if(i < size()-1) {
				_ptr[i] = _ptr[i + 1];
			}
		}
		_size--;

	}

	size_t size() const override {
		return _size;
	}

	size_t MaxSize() const {
		return _capacity;
	}

	~pxArray() override {
		Dispose();
	}


	void PrintInfo(bool show_null_members = false) const {
		std::cout << "Class pxArray<" <<typeid(T).name() << "> { \n";
		std::cout << "Capacity : " << _capacity << "\n";
		std::cout << "Size : " << _size << "\n";
		const int offset = printf("Contents");
		printf(" : {\n");
		size_t max_count = show_null_members ? MaxSize() : size();
		for (size_t i = 0; i < max_count; i++) {
			constexpr int n = 3;
			printf("%*s%.*llu",offset-n+1,".",n,  i);
			std::cout << ": " << _ptr[i] << "\n";
		}
		std::cout << "    }\n";
		std::cout << "}\n";

	}

	void PrintContent() const {
		std::cout << "pxArray<"<< typeid(T).name()<<">{ ";
		for (size_t i = 0; i < size(); i++) {
			std::cout << _ptr[i];
			if (i < (size() - 1))
				std::cout << ", ";
		}
		std::cout << " }\n";
	}
	
private:
	void Dispose() {
		_capacity = 0;
		_size = 0;
		free(_ptr);
	}


	bool _alloc(size_t cb) {
		_ptr = (T*)calloc(cb, sizeof(T));


		return true;
	}

	bool _realloc(size_t newcb) {
		if (newcb == _capacity)
			return false;
		T* tmp = (T*)calloc(newcb, sizeof(T));
		memcpy(tmp, _ptr, _capacity * sizeof(T));
		free(_ptr);
		_ptr = (T*)calloc(newcb, sizeof(T));
		memcpy(_ptr, tmp, _capacity * sizeof(T));

		_capacity = newcb;
		return true;
	}

	size_t _capacity = 0;
	size_t _size = 0;
	T* _ptr = 0;
};

