#pragma once
#include <iostream>
/*
通过仿函数灵活控制，通过模板和类型无关，通过迭代器实现和容器无关，
核心是泛化算法
*/
/*************仿函数*****************/
template <typename T>
struct Display {
	void operator() (const T& x){
		std::cout << x << ' ';
	}
};

/***************类型萃取****************/
/*内嵌类型：在容器中通过参数推演，typedef value_type ，使用时也进行参数推演，但不支持原生指针 */
/*利用类模板偏特化，对部分参数进行进一步限制*/
template<class I>
struct IteratorTraits {
	//嵌套从属类型必须要指明“我是一个类型”
	typedef typename I::value_type value_type;
};

template<class T>
struct IteratorTraits<T*> {
	typedef T value_type;
};

template<class T>
struct IteratorTraits<const T*> {
	typedef T value_type;
};

/******************算法********************/
template <class Iterator, class T>
Iterator Find(Iterator begin, Iterator end, const T& value) {
	while (begin != end && value != *begin) {
		++begin;
	}
	return begin;
}

template <class InputIterator, class Function>
Function ForEach(InputIterator begin, InputIterator end, Function f) {
	while (begin != end) {
		f(*begin);
		++begin;
	}
	return f;
}

//插入排序，只支持随机访问的迭代器
template <class Iterator>
void InsertionSort(Iterator first, Iterator last) {
	for (Iterator tmp= first+1; tmp != last; tmp++) {
		//需要知道迭代器所指类型，利用偏特化萃取
		_InsertionSort(first, tmp);//[first, tmp)
	}
}
template<class Iterator>
void _InsertionSort(Iterator first, Iterator last) {
	//中间层优化
	typedef typename IteratorTraits<Iterator>::value_type value_type;
	value_type value = *last;
	if (*last < *first) {
		while (last != first) {
			*last = *(last - 1);
			last--;
		}
		*first = value;
	}
	else {
		_UnguardedIns(last, value);
	}
}
template<class Iterator, class T>
void _UnguardedIns(Iterator last, T value) {
	last--;
	while (*last > value) {
		*(last + 1) = *(last);
		last--;
	}
	*(last + 1) = value;
}


/****************快排*********************/
/*三次取中把最差优化到nlogn，阈值限制递归层数，unguarded优化*/
template<class Iterator>
inline Iterator Median(Iterator a, Iterator b, Iterator c){
	if (*a > *b) {
		if (*b > *c) {
			return b;
		}
		if (*a > *c) {
			return c;
		}
		return a;
	}
	else if (*a > *c) {
		return a;
	}
	else if (*b > *c) {
		return c;
	}
	else {
		return b;
	}
}

template<class Iterator>
Iterator Partition(Iterator first, Iterator last) {
	Iterator piv = Median(first, first + (last - first) / 2, last - 1);
	swap(*piv, *(last - 1));
	Iterator pp = first - 1;
	for(; first < last-1; ++first){
		if (*first > *(last-1)) {
			continue;
		}
		else {
			++pp;
			if (pp != first) {
				swap(*pp, *first);
			}
		}
	}
	++pp;
	swap(*pp, *(last - 1));
	return pp;
}

const int THRESHOLD = 16;
template<class Iterator>
inline void Sort(Iterator first, Iterator last) {
	if (first != last) {
		_QuickSortLoop(first, last);
		InsertionSort(first, last);
	}
}

template<class Iterator>
inline void _QuickSortLoop(Iterator first, Iterator last) {
	if (last - first > THRESHOLD) {
		Iterator mid = Partition(first, last);
		_QuickSortLoop(first, mid);
		_QuickSortLoop(mid+1, last);
	}
}

