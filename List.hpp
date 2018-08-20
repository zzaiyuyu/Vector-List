#pragma once
#include <iostream>
//带头双向循环链表
template<typename T>
struct ListNode {
	ListNode<T>* _pNext;
	ListNode<T>* _pPre;
	T _data;
	ListNode(T data) {
		_data = data;
		_pNext = NULL;
		_pPre = NULL;
	}
};

//封装链表结点指针——迭代器
template<class T, class Ref, class Ptr>
class ListIterator {
	typedef ListNode<T> Node;
	typedef Node* PNode;
	typedef ListIterator<T, Ref, Ptr> Self;
public:
	ListIterator()
		:_pNode(0)
	{
	}
	ListIterator(PNode pNode)
		:_pNode(pNode)
	{
	}
	ListIterator(const Self& s)
		:_pNode(s._pNode)
	{
	}
	Self& operator++() {
		_pNode = _pNode->_pNext;
		return *this;
	}
	Self& operator=(const PNode& pNode) {
		_pNode = pNode;
		return *this;
	}
	Ref operator*()
	{
		return _pNode->_data;
	}
	Ptr operator->()
	{
		return &(operator*());
	}
	bool operator!=(const Self& s) {
		return _pNode != s._pNode;
	}
private:
	PNode _pNode;
};

template<typename T>
class List {
public:
	List() {
		_CreateHead();
	}

	List(size_t n, const T& data = T()) {
		_CreateHead();
		for (size_t i = 0; i < n; i++) {
			PushBack(data);
		}
	}
	List(const T*first, const T* last) {
		_CreateHead();
		while (first != last) {
			PushBack(*first);
			++first;
		}
	}

	List(const List<T>& list) {
		_CreateHead();
		ListNode<T>* pCur = list._pHead->_pNext;
		while (pCur != list._pHead) {
			PushBack(pCur->_data);
			pCur = pCur->_pNext;
		}
	}

	List<T>& operator=(const List<T>& list)
	{
		if (&list != this) {
			//清理掉原链表
			while (_pHead->_pNext != _pHead) {
				PopBack();
			}
			//逐个插入list表结点
			ListNode<T>* pCur = list._pHead->_pNext;
			while (pCur != list._pHead) {
				PushBack(pCur->_data);
				pCur = pCur->_pNext;
			}
		}
		return *this;
	}

	~List() {
		while (_pHead->_pNext != _pHead) {
			PopBack();
		}
		delete _pHead;
		_pHead = NULL;
	}

	bool Empty()const {
		return _pHead->_pNext == _pHead;
	}

	size_t Size()const {
		ListNode<T>* pCur = _pHead->_pNext;
		size_t count = 0;
		while (pCur != _pHead){
			count++;
			pCur = pCur->_pNext;
		}
		return count;
	}
	void Resize(size_t newSize, const T& data = T()) {
		size_t old = Size();
		if (newSize < old) {
			while (newSize != old) {
				PopBack();
				old--;
			}
		}
		else {
			while (newSize != old) {
				PushBack(data);
				old++;
			}
		}
	}

	T& Front() {
		return _pHead->_pNext->_data;
	}
	T& Back() {
		return _pHead->_pPre->_data;
	}
	void PushBack(const T& data) {
		ListNode<T>* pNewNode = new ListNode<T>(data);
		pNewNode->_pNext = _pHead;
		pNewNode->_pPre = _pHead->_pPre;
		_pHead->_pPre = pNewNode;
		pNewNode->_pPre->_pNext = pNewNode;
	}
	void PopBack() {
		ListNode<T>* pDelNode = _pHead->_pPre;
		if (pDelNode!=_pHead) {
			pDelNode->_pPre->_pNext = _pHead;
			_pHead->_pPre = pDelNode->_pPre;
			delete pDelNode;
		}
	}
	void PushFront(const T& data)
	{
		ListNode<T>* pNewNode = new ListNode<T>(data);
		pNewNode->_pNext = _pHead->_pNext;
		pNewNode->_pPre = _pHead;
		_pHead->_pNext = pNewNode;
		pNewNode->_pNext->_pPre = pNewNode;
	}
	void PopFront()
	{
		if (_pHead->_pNext != _pHead) {
			ListNode<T>* pDel = _pHead->_pNext;
			pDel->_pNext->_pPre = _pHead;
			_pHead->_pNext = pDel->_pNext;
			delete pDel;
		}
	}
	ListNode<T>* Insert(ListNode<T>* pos, const T& data) {
		ListNode<T>* pNewNode = new ListNode<T>(data);
		ListNode<T>* pCur = _pHead;
		while (pCur->_pNext != pos) {
			pCur = pCur->_pNext;
		}
		pNewNode->_pPre = pCur;
		pNewNode->_pNext = pCur->_pNext;
		pCur->_pNext = pNewNode;
		pNewNode->_pNext->_pPre = pNewNode;
		return pNewNode;
	}
	void Erase(ListNode<T>* pos)
	{
		ListNode<T>* pCur = _pHead;
		while (pCur->_pNext != pos) {
			pCur = pCur->_pNext;
		}
		ListNode<T>* pDel = pCur -> _pNext;
		pCur->_pNext = pCur->_pNext->_pNext;
		pCur->_pNext->_pPre = pCur;
		delete pDel;
	}
	ListNode<T>* Find(T data) {
		ListNode<T>* pCur = _pHead->_pNext;
		while (pCur->_data != data && pCur != _pHead) {
			pCur = pCur->_pNext;
		}
		return pCur == _pHead ? NULL : pCur;
	}
	ListIterator<T, T&, T*> Begin() {
		return _pHead->_pNext;
	}
	ListIterator<T, T&, T*> End() {
		return _pHead;
	}
private:
	void _CreateHead() {
		_pHead = new ListNode<T>(0);
		_pHead->_pNext = _pHead;
		_pHead->_pPre = _pHead;
	}
private:
	ListNode<T>* _pHead;
};

static void TestLList()
{
	List<int> list;
	int arr[] = { 1,2,3,4 };
	for (int i = 0; i < 4; i++) {
		list.PushBack(arr[i]);
	}
	List<int> listSec(list);
	List<int> listThi;
	listThi = listSec;
	listThi.PushFront(-1);
	listThi.PushFront(-2);
	listThi.PopFront();
	listThi.Insert(listThi.Find(3), 6);
	listThi.Erase(listThi.Find(2));
	ListIterator<int, int&, int*> it;
	for (it = listThi.Begin(); it != listThi.End(); ++it) {
		std::cout << *it << std::endl;
	}
}
