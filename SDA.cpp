// SDA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "Timer.hpp"
#include "LiniarAllocator.hpp"
#include "MemoryBenchmark.hpp"
#include "RefCountedPtr.hpp"
#include "Singleton.hpp"
#include "Pair.hpp"
#include "Vector.hpp"
#include "FixedStack.hpp"
#include "FixedQueue.hpp"
//#include "SinglyLinkedList.hpp"
//#include "MultiwayTree.hpp"
//#include "LiniarSet.hpp"

#include "Sort.hpp"
#include "Search.hpp"

//#define TEST_TIMER
//#define TEST_CUSTOM_ALLOCATORS
//#define TEST_SMART_PTR
//#define TEST_SINGLETON
//#define TEST_PAIR
//#define TEST_VECTOR
//#define TEST_FIXED_STACK
//#define TEST_FIXED_QUEUE
//#define TEST_SINGLY_LINKED_LIST

//#define TEST_SORT
//#define TEST_SEARCH

	// C++ implementation below
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class A
{
	const int val = 0;
public:
	A() : val(2) { std::cout << val << "!"; }
	~A() { std::cout << val; }
};


class B: public A
{
	const int val = 1;
public:
	B() : val(4) { std::cout << val << "@"; }
	~B() { std::cout << val; }
};

class Test
{
public:
	Test()
		: mName()
	{
		cout << mName << " - Default constructor called " << endl;
	}
	Test(const std::string& name)
		: mName(name)
	{
		cout << mName << " - 1 Arg constructor called " << endl;
	}
	Test(const Test& t)
	{
		cout << mName << " - Copy constructor called " << endl;
	}
	~Test()
	{
		cout << mName << " - Destructor called " << endl;
	}

	Test& operator = (const Test& t)
	{
		cout << mName << " - Assignment operator called " << endl;
		return *this;
	}

private:
	std::string mName;
};

int main()
{

#ifdef TEST_TIMER
	SDA::Timer timer1;

	timer1.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	timer1.Stop();
	std::cout << "Elapsed time nano: " << timer1.ElapsedTimeInNanoseconds() << std::endl;
	std::cout << "Elapsed time micro: " << timer1.ElapsedTimeInMicroseconds() << std::endl;
	std::cout << "Elapsed time mili: " << timer1.ElapsedTimeInMiliseconds() << std::endl;
	std::cout << "Elapsed time sec: " << timer1.ElapsedTimeInSeconds() << std::endl;

	SDA::Timer timer2([]() { std::cout << "Timeout! " << std::endl; }, true, 2000);
	timer2.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(6000));
	timer2.Stop();
	timer2.Stop();

	timer2.Start();
	timer2.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	timer2.Stop();

	timer2.Restart();

#endif // TEST_TIMER

#ifdef TEST_CUSTOM_ALLOCATORS
	SDA::Allocator* allocator = new SDA::LiniarAllocator(1e9);

	SDA::MemoryBenchmark benchmark(1e1);

	std::cout << "LINEAR ALLOCATOR" << std::endl;
	benchmark.SingleAllocation(allocator, 4096, 8);
	//benchmark.SingleFree(allocator, 4096, 8);

#endif // TEST_CUSTOM_ALLOCATORS

#ifdef TEST_SMART_PTR
	struct Obj
	{
		Obj() { std::cout << "Obj::Ctor()" << std::endl; };
		~Obj() { std::cout << "Obj::Dtor()" << std::endl; };

		void Print() { std::cout << "Obj::Print()" << std::endl; };
	};

#if 1
	{
		SDA::RefCountedPtr<Obj> rp1(new Obj());
		rp1->Print();
		std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;

		{
			const SDA::RefCountedPtr<Obj> rp2(rp1);

			std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;
			std::cout << "rp2 ref count: " << rp2.GetRefCount() << std::endl;

			SDA::RefCountedPtr<Obj> rp3;
			rp3 = rp1;
			std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;
			std::cout << "rp3 ref count: " << rp3.GetRefCount() << std::endl;
			rp3 = rp2;
			std::cout << "rp3 ref count: " << rp3.GetRefCount() << std::endl;
			std::cout << "rp2 ref count: " << rp2.GetRefCount() << std::endl;

			SDA::RefCountedPtr<Obj> rp4(new Obj());
			rp4 = rp3;
			std::cout << "rp4 ref count: " << rp4.GetRefCount() << std::endl;
			std::cout << "rp3 ref count: " << rp3.GetRefCount() << std::endl;
			rp4 = rp1;
			std::cout << "rp4 ref count: " << rp4.GetRefCount() << std::endl;
			std::cout << "rp3 ref count: " << rp3.GetRefCount() << std::endl;

			SDA::RefCountedPtr<Obj> rp5(rp4);
			rp4.Reset();
			std::cout << "rp4 ref count: " << rp4.GetRefCount() << std::endl;
			std::cout << "rp5 ref count: " << rp5.GetRefCount() << std::endl;
			rp4->Print();

			SDA::RefCountedPtr<Obj> rp6 = rp1;
			rp6 = rp1;
			std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;
			std::cout << "rp6 ref count: " << rp6.GetRefCount() << std::endl;
			rp6.Reset(new Obj());
			std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;
			std::cout << "rp6 ref count: " << rp6.GetRefCount() << std::endl;
			rp4->Print();

			std::cout << "rp1 == rp3: " << (rp1 == rp3) << std::endl;
			std::cout << "rp4 <= rp6: " << (rp4 <= rp6) << std::endl;
			std::cout << "rp5 != rp5: " << (rp5 != rp5) << std::endl;
		}
		rp1->Print();
		std::cout << "rp1 ref count: " << rp1.GetRefCount() << std::endl;
	}
#endif

#if 0
	{
		std::shared_ptr<Obj> rp1(new Obj());
		rp1->Print();
		std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;

		{
			const std::shared_ptr<Obj> rp2(rp1);

			std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;
			std::cout << "rp2 ref count: " << rp2.use_count() << std::endl;

			std::shared_ptr<Obj> rp3;
			rp3 = rp1;
			std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;
			std::cout << "rp3 ref count: " << rp3.use_count() << std::endl;
			rp3 = rp2;
			std::cout << "rp3 ref count: " << rp3.use_count() << std::endl;
			std::cout << "rp2 ref count: " << rp2.use_count() << std::endl;

			std::shared_ptr<Obj> rp4(new Obj());
			rp4 = rp3;
			std::cout << "rp4 ref count: " << rp4.use_count() << std::endl;
			std::cout << "rp3 ref count: " << rp3.use_count() << std::endl;
			rp4 = rp1;
			std::cout << "rp4 ref count: " << rp4.use_count() << std::endl;
			std::cout << "rp3 ref count: " << rp3.use_count() << std::endl;

			std::shared_ptr<Obj> rp5(rp4);
			rp4.reset();
			std::cout << "rp4 ref count: " << rp4.use_count() << std::endl;
			std::cout << "rp5 ref count: " << rp5.use_count() << std::endl;

			std::shared_ptr<Obj> rp6 = rp1;
			rp6 = rp1;
			std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;
			std::cout << "rp6 ref count: " << rp6.use_count() << std::endl;
			rp6.reset(new Obj());
			std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;
			std::cout << "rp6 ref count: " << rp6.use_count() << std::endl;
			rp4->Print();
		}
		rp1->Print();
		std::cout << "rp1 ref count: " << rp1.use_count() << std::endl;
	}
#endif


#endif // TEST_SMART_PTR

#ifdef TEST_SINGLETON
	class TestSingleton : public SDA::Singleton<TestSingleton>
	{
		// to keep ctor() and dtor() private
		friend SDA::Singleton<TestSingleton>;

	public:

		void DoSomeStuff() { std::cout << "TestSingleton::DoSomeStuff()" << std::endl; }

	private:
		NON_COPY_AND_MOVE(TestSingleton)

			TestSingleton() { std::cout << "TestSingleton::Ctor()" << std::endl; }
		virtual ~TestSingleton() { std::cout << "TestSingleton::Dtor()" << std::endl; }
	};

	// TEST
	TestSingleton::GetInstance().DoSomeStuff();
#endif // TEST_SINGLETON

#ifdef TEST_PAIR
	SDA::Pair<int, std::string> pair1(12, "LAAB");
	std::cout << pair1 << std::endl;

	SDA::Pair<int, std::string> pair2(pair1);
	std::cout << pair2 << std::endl;

	SDA::Pair<int, std::string> pair3;
	pair3 = pair1;
	std::cout << pair3 << std::endl;

	SDA::Pair<int, std::string> pair4(34, "ABCDEF");
	pair4.Swap(pair1);
	std::cout << pair1 << std::endl;
	std::cout << pair4 << std::endl;

	pair4.SetFirst(99);
	pair4.SetSecond("HJKL");
	std::cout << pair4 << std::endl;

	std::cin >> pair1;
	std::cout << pair1 << std::endl;
#endif // TEST_PAIR

#ifdef TEST_VECTOR
	SDA::Vector<int> vec1(10, 3);
	std::cout << vec1;

	vec1.Erase(3);

	std::cout << vec1;

	vec1.Insert(0, 5);

	std::cout << vec1;

	vec1.PopBack();

	std::cout << vec1;

	std::cout << "is vec1 empty: " << vec1.IsEmpty() << std::endl;
	std::cout << "vec1 size: " << vec1.Size() << std::endl;
	std::cout << "vec1 capacity: " << vec1.Capacity() << std::endl;

	SDA::Vector<std::string> vec2;

	for (int i = 0; i < 3; ++ i)
	{
		vec2.PushBack(std::string("L"));
	}

	std::cout << vec2;
	std::cout << "vec2 data: " << vec2.GetData() << std::endl;

	std::cout << "vec2 size: " << vec2.Size() << std::endl;
	std::cout << "vec2 capacity: " << vec2.Capacity() << std::endl;

	vec2.Resize(8);

	std::cout << "vec2 size: " << vec2.Size() << std::endl;
	std::cout << "vec2 capacity: " << vec2.Capacity() << std::endl;

	std::cout << vec2;

	SDA::Vector<float> vf1;
	std::cout << vf1;

	SDA::Vector<float> vf2(3);

	vf1 = vf2;

	std::cout << vf1;
	std::cout << "vf1 capacity: " << vf1.Capacity() << std::endl;

	SDA::Vector<char> vec3(4);
	vec3[0] = 'a';
	vec3[1] = 'b';
	vec3[2] = 'c';
	vec3[3] = 'd';

	std::cout << "vec3 front: " << vec3.Front() << std::endl;
	std::cout << "vec3 back: " << vec3.Back() << std::endl;
	std::cout << "vec3 3rd element: " << vec3.At(2) << std::endl;

	SDA::Vector<char> vec4(4, 'i');
	vec4.Swap(vec3);
	std::cout << vec3;
	std::cout << vec4;

	SDA::Vector<char> vec5 = vec4;
	std::cout << vec5;

	//TODO - FIX
	SDA::Vector<char> vec6;
	std::cin >> vec6;
	std::cout << vec6;
#endif // TEST_VECTOR

#ifdef TEST_FIXED_STACK
	SDA::FixedStack<float> stack1;

	stack1.Push(11.2f);
	stack1.Push(34.5f);
	stack1.Pop();
	stack1.Push(123.04f);
	stack1.Push(-0.12f);

	std::cout << "stack1 capacity: " << stack1.Capacity() << std::endl;
	std::cout << "stack1 size: " << stack1.Size() << std::endl;
	std::cout << "stack1 isEmpty: " << stack1.IsEmpty() << std::endl;
	std::cout << "stack1 isFull: " << stack1.IsFull() << std::endl;

	std::cout << stack1;

	SDA::FixedStack<float> stack2 = stack1;
	std::cout << stack2;

	SDA::FixedStack<float> stack3(10);

	std::cout << "stack3 capacity: " << stack3.Capacity() << std::endl;
	std::cout << "stack3 size: " << stack3.Size() << std::endl;

	stack3.Swap(stack1);
	std::cout << "stack3 capacity: " << stack3.Capacity() << std::endl;
	std::cout << "stack3 size: " << stack3.Size() << std::endl;
	std::cout << stack3;
	std::cout << stack1;

#endif // TEST_FIXED_STACK

#ifdef TEST_FIXED_QUEUE
	SDA::FixedQueue<float> queue1;

	queue1.PushBack(11.2f);
	queue1.PushBack(34.5f);
	queue1.PopFront();
	queue1.PushBack(123.04f);
	queue1.PushBack(-0.12f);

	std::cout << "queue1 capacity: " << queue1.Capacity() << std::endl;
	std::cout << "queue1 size: " << queue1.Size() << std::endl;
	std::cout << "queue1 isEmpty: " << queue1.IsEmpty() << std::endl;
	std::cout << "queue1 isFull: " << queue1.IsFull() << std::endl;

	std::cout << queue1;

	SDA::FixedQueue<float> queue2 = queue1;
	std::cout << queue2;

	SDA::FixedQueue<float> queue3(10);

	std::cout << "queue3 capacity: " << queue3.Capacity() << std::endl;
	std::cout << "queue3 size: " << queue3.Size() << std::endl;

	queue3.Swap(queue1);
	std::cout << "queue3 capacity: " << queue3.Capacity() << std::endl;
	std::cout << "queue3 size: " << queue3.Size() << std::endl;
	std::cout << queue3;
	std::cout << queue1;
#endif // TEST_FIXED_QUEUE

#ifdef TEST_SINGLY_LINKED_LIST
	SDA::SinglyLinkedList<short> list;


#endif // TEST_SINGLY_LINKED_LIST

//	SDA::MultiwayTree<char*> tree;


#ifdef TEST_SORT

	SDA::Vector<int> vec1;
	vec1.PushBack(-12);
	vec1.PushBack(4);
	vec1.PushBack(35);
	vec1.PushBack(19);
	vec1.PushBack(-7);

	std::cout << vec1;

#if 0
	SDA::BubbleSort(vec1, SDA::Greater());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	SDA::BubbleSort(vec1, SDA::Less());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;
#endif

#if 0
	SDA::SelectionSort(vec1, SDA::Greater());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	SDA::SelectionSort(vec1, SDA::Less());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;
#endif

#if 0
	SDA::InsertionSort(vec1, SDA::Greater());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	SDA::InsertionSort(vec1, SDA::Less());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;
#endif

#if 0 //TODO
	SDA::CountSort(vec1, SDA::Greater());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	//	SDA::CountSort(vec1, SDA::Less());

	//	std::cout << "sorted:" << std::endl;
	//	std::cout << vec1;
#endif

#if 0
	SDA::QuickSort(vec1, 0, vec1.Size() - 1, SDA::Greater());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	SDA::QuickSort(vec1, 0, vec1.Size() - 1, SDA::Less());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;
#endif

#endif // TEST_SORT

#ifdef TEST_SEARCH
	SDA::Vector<int> vec1;
	vec1.PushBack(-2);
	vec1.PushBack(0);
	vec1.PushBack(14);
	vec1.PushBack(-1);
	vec1.PushBack(4);
	vec1.PushBack(3);

	SDA::BubbleSort<int>(vec1, SDA::Less<int>());

	std::cout << "sorted:" << std::endl;
	std::cout << vec1;

	size_t pos = SDA::BinarySearch<int>(vec1, -1, 0, vec1.Size() - 1);

	std::cout << "found1: " << pos << std::endl;
#endif // TEST_SEARCH

}

/*

DATA STRUCTURES

Smart Pointers
Custom Allocators
Pair
Array
Vector
String
Stack
Queue
LinkedList

MultiwayTree
BinaryTree
BinarySearchTree
HashMap
Heaps
PriorityQueue
Graphs

BiarySpacePartitioning Tree
kd-Tree
QuadTree
OctTree
other trees...

ALGORITHMS

Binary search
Divide Et Impera
Backtracking
Greedy 

Sorting
Tree traversals


*/