#ifndef _HEAP_H_
#define _HEAP_H_

#pragma once

#include <vector>
#include <algorithm>

using namespace std;

template<class T>
class Heap
{
public:
	Heap();
	Heap(bool minHeap);
	~Heap();

	int getSize() const;
	bool isEmpty() const;

	bool Add(const T * const toAdd);
	bool Retrieve(T &retrieved);

private:
	int count = 0;
	bool minHeap = true;
	vector<T*> items;

	inline bool heapCompare(const T &left, const T &right) const;
	void heapRebuild(int subrootIndex);
};




template<class T>
Heap<T>::Heap()
{
}

template<class T>
Heap<T>::Heap(bool minHeap)
{
}

template<class T>
Heap<T>::~Heap()
{
}



template<class T>
inline int Heap<T>::getSize() const
{
	return this->count;
}

template<class T>
bool Heap<T>::isEmpty() const
{
	return (this->count == 0);
}

template<class T>
bool Heap<T>::Add(const T * const toAdd)
{
	T *item = new T(*toAdd);

	if (this->isEmpty())
	{
		this->items.insert(this->items.begin(), item);
	}
	else
	{
		this->items.push_back(item);
	}

	int newDataIndex = this->count;

	bool inPlace = false;
	while ((newDataIndex > 0) && !inPlace)
	{
		int parentIndex = (newDataIndex - 1) / 2;
		if (heapCompare(*this->items.at(parentIndex), *this->items.at(newDataIndex)))
		{
			inPlace = true;
		}
		else
		{
			swap(this->items.at(parentIndex), this->items.at(newDataIndex));
			newDataIndex = parentIndex;
		}
	}

	this->count += 1;

	return true;
}

template<class T>
bool Heap<T>::Retrieve(T &retrieved)
{
	if (this->count > 1)
	{
		T * temp = this->items.at(0);
		this->items.at(0) = this->items.at(this->count - 1);
		this->items.erase(this->items.begin() + (this->count - 1));
		this->count -= 1;
		heapRebuild(0);

		retrieved = *temp;
		delete temp;
		return true;
	}
	else if (this->count == 1)
	{
		this->count -= 1;
		T *temp = items.at(0);
		items.erase(this->items.begin());
		
		retrieved = *temp;
		delete temp;
		return true;
	}
	else
	{
		return false;
	}
}





template<class T>
inline bool Heap<T>::heapCompare(const T &left, const T &right) const
{
	if (minHeap)
		return left < right;
	else
		return left > right;
}

template<class T>
void Heap<T>::heapRebuild(int subrootIndex)
{
	int leftChildIndex = 2 * subrootIndex + 1;
	int rightChildIndex = 2 * subrootIndex + 2;
	T *leftChild = nullptr;
	T *rightChild = nullptr;

	bool subrootHasLeftChild = false;
	bool subrootHasRightChild = false;
	try
	{
		leftChild = this->items.at(leftChildIndex);
		subrootHasLeftChild = true;

		rightChild = this->items.at(rightChildIndex);
		subrootHasRightChild = true;
	}
	catch (out_of_range ex)
	{
	}

	if (subrootHasLeftChild)
	{//if the subroot has a right child, it is guaranteed to have a left child as well
		int topChildIndex = leftChildIndex;

		if (subrootHasRightChild && (heapCompare(*rightChild, *leftChild)))
			topChildIndex = rightChildIndex;

		if (heapCompare(*this->items.at(topChildIndex), *this->items.at(subrootIndex)))
		{
			swap(this->items.at(topChildIndex), this->items.at(subrootIndex));
			heapRebuild(topChildIndex);
		}
	}
}
#endif //_HEAP_H_