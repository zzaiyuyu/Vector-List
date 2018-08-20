#pragma once
#include <iostream>
#include <assert.h>
#include "String.h"

/*类型萃取，判断当前对象是否是自定义类型*/
/*原理：利用编译器，在编译期间判断T的类型，若在特化列表里优先使用特化的模板类*/
struct TrueType {
	static bool Get()
	{
		return true;
	}
};

struct FalseType {
	static bool Get()
	{
		return false;
	}
};
//如果没有特化，就是自定义类型
template<class T>
struct TypeTraits {
	typedef FalseType PODTYPE;
};
template <>
struct TypeTraits<int> {
	typedef TrueType PODTYPE;
};
template <>
struct TypeTraits<double> {
	typedef TrueType PODTYPE;
};

/**********************vector*****************************/


template<class T>
class Vector {
public:
	static void TestVector();
	//连续空间，指针完全胜任迭代器
	typedef T* Iterator;
	Vector() 
		:_start(0),_finish(0),_end(0)
	{
	}
	//用size个对象填充
	Vector(int size, T data = T()) {
		T* pTmp = new T[size * sizeof(T)];
		for (int i = 0; i < size; i++) {
			pTmp[i] = data;
		}
		_start = pTmp;
		_finish = pTmp+size;
		_end = pTmp + size;
	}
	//[first, last)拷贝进来
	Vector(const T* first, const T* last) {
		int size = last - first;
		T* pTmp = new T[size * sizeof(T)];
		for (; first != last; first++) {
			pTmp[i] = *first;
		}
		_start = pTmp;
		_finish = pTmp + size;
		_end = pTmp + size;
	}

	Vector(const Vector<T>& v) {
		T* pTmp = new T[sizeof(T) * v.Capacity()];
		for (int i = 0; i < v.Size(); i++) {
			pTmp[i] = v._start[i];
		}
		_start = pTmp;
		_finish = pTmp + v.Size();
		_end = pTmp + v.Capacity();
	}

	Vector<T>& operator=(const Vector<T>& v)
	{
		if (*this != v) {
			T* pNew = new T[sizeof(T)*Size()];
			memcpy(pNew, v._start, Size());
			delete[] _start;
			_start = pNew;
			_finish = pNew + v.Size();
			_end = pNew + v.Capacity();
		}
		return *this;
	}
	~Vector() {
		if (_start) {
			delete[] _start;
		}
	}

	size_t Size()const {
		return _finish - _start;
	}
	size_t Capacity()const {
		return _end - _start;
	}
	bool Empty()const {
		return _start == _finish;
	}

	void Resize(size_t newSize, const T& data = T()) {
		size_t old = Size();
		if (newSize <= old) {
			_finish = _start + newSize;
		}
		else {
			size_t capacity = Capacity();
			if (newSize < capacity) {
				//往里补
				while (newSize != old) {
					*_finish = data;
					++_finish;
					--newSize;
				}
			}
			else {
				//1.开辟新空间
				T* pTmp = new T[newSize];
				if (_start) {
					//2搬移元素
					for (size_t i = 0; i < old; i++) {
						pTmp[i] = _start[i];
					}
					for (size_t i = old; i < newSize; i++) {
						pTmp[i] = data;
					}
					delete[] _start;
				}
				//3释放旧空间
			
				_start = pTmp;
				_finish = _start + newSize;
				_end = _finish;
			}

		}
	}
	//预留空间
	void Reserve(size_t n) {
		if (n > Capacity()) {
			//1.开辟新空间
			size_t size = Size();
			T* pTmp = new T[n];
			if (_start) {
				//2搬移元素
				for (size_t i = 0; i < old; i++) {
					pTmp[i] = _start[i];
				}
				for (size_t i = oldSize; i < newSize; i++) {
					pTmp[i] = data;
				}
				//3释放旧空间
				delete[] _start;
			}
			_start = pTmp;
			_finish = _start + size;
			_end = _start + n;

		}
	}

	void PushBack(const T& data);
	void PopBack();
	void Erase(size_t pos);
	void Insert(size_t pos, const T& data);

	T& operator[](size_t index) {
		assert(index < Size());
		return _start[index];
	}
	const T& operator[](size_t index) const {
		assert(index < Size());
		return _start[index];
	}
	T& Front() {
		return *_start;
	}
	T& Back() {
		return *(_finish - 1);
	}

	void Clear() {
		_finish = _start;
	}
	void Swap(Vector<T>& v) {
		swap(_start, v._start);
		swap(_finish, v._finish);
		swap(_end, v._end);
	}

	Iterator Begin() {
		return _start;
	}
	Iterator End() {
		return _finish;
	}

private:
	void _CheckCapacity() {
		if (_finish == _end) {
			size_t newSize = Size() * 2 + 3;
			T* pTmp = new T[newSize];
			size_t size = Size();
			if (_start) {
				//内置类型用memcpy更快，类型萃取
				if (TypeTraits<T>::PODTYPE::Get()) {
					memcpy(pTmp, _start, sizeof(T)*size);
				}
				else {
					//自定义类型依赖于其赋值运算符
					for (size_t i = 0; i < size; i++) {
						pTmp[i] = _start[i];
					}
				}
				delete[] _start;
			}
			_start = pTmp;
			_finish = _start + size;
			_end = _start + newSize;
		}
	}

private:
	Iterator _start;	//动态开辟空间起始
	Iterator _finish;	//当前未放元素的起始空间
	Iterator _end;	//动态开辟空间末尾static void TestVector()
};

//凡是需要用T的地方都需要加上template说明T
template<class T>
void Vector<T>::PushBack(const T& data) {
	_CheckCapacity();
	*_finish = data;
	++_finish;
}

template<class T>
void Vector<T>::PopBack() {
	if (_start != _finish) {
		--_finish;
	}
}
template<class T>
void Vector<T>::Insert(size_t pos, const T& data)
{
	//pos从1开始
	_CheckCapacity();
	//有个坑，size_t是无符号数
	int index = (int)Size()-1;
	while (index>=0 && index >= pos-1) {
		_start[index+1] = _start[index];
		index--;
	}
	_start[++index] = data;
	_finish++;
}

template<class T>
void Vector<T>::Erase(size_t pos)
{
	size_t end = Size();
	--pos;//位次转为下标
	while (pos < end) {
		_start[pos] = _start[pos + 1];
		pos++;
	}
	--_finish;
}


template<typename T>
void Vector<T>::TestVector()
{
	Vector<double> vi(5, 3.14);
	vi.PushBack(1.5);
	vi.PushBack(2.5);
	vi.PushBack(3.33);
	std::cout << vi.Size() << std::endl;
	std::cout << vi.Capacity() << std::endl;
	std::cout << vi.Front() << std::endl;
	std::cout << vi.Back() << std::endl;
	vi.PopBack();
	vi.PopBack();
	std::cout << vi.Size() << std::endl;
	std::cout << vi.Capacity() << std::endl;
	std::cout << vi.Front() << std::endl;
	std::cout << vi.Back() << std::endl;
	vi.Resize(20, 9);
	std::cout << std::endl;
	for (size_t i = 0; i < vi.Size(); i++) {
		std::cout << vi[i] << " ";
	}
	std::cout << std::endl;
	Vector<double> vb(vi);
	for (size_t i = 0; i < vb.Size(); i++) {
		std::cout << vb[i] << " ";
	}
	Vector<String> vs(10, "Nice");
	vs.PushBack("l1");
	vs.PushBack("12");
	vs.Erase(10);
	vs.Insert(1, "insert");
	vs.PopBack();
	Vector<String>::Iterator it;
	for (it = vs.Begin(); it != vs.End(); it++) {
		std::cout << *it << std::endl;
	}
}
