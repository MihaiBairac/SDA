#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <functional>
#include <iostream>

/*
BinaryTree - a type of collection/container to store elements of the same type hierachicaly
 dynamically, in a non-contiguos way.

TIME COMPLEXITY:
- Traversal/Search = O(log n) best case, O(N) worst case
- Add/Delete an element = O(log n)
- Access an element = O(log n)
- Get size = O(log n)

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- easy traversal to retrieve elements

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to Vectors, Arrays
- extra memory needed for node connections (pointers)

USAGES:
1. Manipulate hierarchical data.
2. Make information easy to search (see tree traversal).
3. Manipulate sorted lists of data.
4. As a workflow for compositing digital images for visual effects.
5. Router algorithms/tables
6. Form of a multi-stage decision-making (see business chess).
7. Game states

Used to implement other data types as:
a) Binary Search Tree
b) Heap
c) Priority Queue
d) Red-Black Tree
e) Binary Space Partition - Used in almost every 3D video game to determine what objects need to be rendered.
e) etc.

MODEL:

	   j    <-- root
	 /   \
	f      k
  /   \      \
 a     h      z    <-- leaves

*A FULL binary tree (sometimes proper binary tree or 2-tree) is a tree in which every node other than the leaves has two children

*A COMPLETE binary tree is a binary tree in which every level, except possibly the last, is completely filled, and all nodes are as far left as possible.

*/

#ifndef BT_ITERATIVE
//#define BT_RECURSIVE
#endif

#ifndef BT_RECURSIVE
#define BT_ITERATIVE
#endif

#ifdef BT_RECURSIVE
#include <stack>
#endif
#ifdef BT_ITERATIVE
#include <queue>
#endif


namespace SDA
{
	template <class T>
	struct BinaryTreeNode
	{
		BinaryTreeNode()
			: data(), parentPtr(nullptr), leftPtr(nullptr), rightPtr(nullptr)
		{}

		BinaryTreeNode(const T& data_)
			: data(data_), parentPtr(nullptr), leftPtr(nullptr), rightPtr(nullptr)
		{}

		BinaryTreeNode(const BinaryTreeNode<T>& node)
			: data(node.data), parentPtr(node.parentPtr), leftPtr(node.nextPtr), rightPtr(node.rightPtr)
		{}

		~BinaryTreeNode()
		{
			parentPtr = nullptr;
			leftPtr = nullptr;
			rightPtr = nullptr;
		}

		Swap(const BinaryTreeNode<T>& node)
		{
			SDA::Swap(data, node.data);
			SDA::Swap(parentPtr, node.parentPtr);
			SDA::Swap(leftPtr, node.leftPtr);
			SDA::Swap(rightPtr, node.rightPtr);
		}

		T data;
		BinaryTreeNode<T> *parentPtr, *leftPtr, *rightPtr;
	};

	template <class T>
	class BinaryTree
	{
	public:
		bool default_func(BinaryTreeNode<T>* nodePtr) { return true; };

		enum class BT_DIR: unsigned short
		{
			BT_LEFT = 0,
			BT_RIGHT,
			BT_COUNT
		};

		BinaryTree();
		BinaryTree(const BinaryTree<T>& tree);
		BinaryTree(BinaryTree<T>&& tree);
		virtual ~BinaryTree();

		BinaryTree<T>& operator =(const BinaryTree<T>& tree);
		BinaryTree<T>& operator =(BinaryTree<T>&& tree);

		void AddNode(BinaryTreeNode<T>* parentPtr, BT_DIR dir, const T& val);
		void RemoveNode(BinaryTreeNode<T>* nodePtr);
		void DeleteNode(const T& val);

		BinaryTreeNode<T>* FindNode(const T& val);

		// Breadth First Traversal (variant of BFS)
		void LevelOrderTrversal(const std::function<bool(BinaryTreeNode<T>* nodePtr, bool isLastNode)>& func); // visiting each level of the tree

		// Depth First Traversals
		void InorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func); // Left, Root, Right
		void PreorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func); // Root, Left, Right
		void PostorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func); // Left, Right, Root

		size_t Height(BinaryTreeNode<T>* nodePtr);
		size_t Depth(BinaryTreeNode<T>* nodePtr);
		size_t Size(BinaryTreeNode<T>* nodePtr);

		bool IsLeaf(BinaryTreeNode<T>* nodePtr);

	private:
		void Copy(const BinaryTree<T>& tree);
		void Move(BinaryTree<T>&& tree);
		void Destroy();

		BinaryTreeNode<T>* mRootPtr;
	};


}


/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	BinaryTree<T>::BinaryTree()
		: mRootPtr(nullptr)
	{}

	template <class T>
	BinaryTree<T>::BinaryTree(const BinaryTree<T>& tree)
	{
		Copy(tree);
	}

	template <class T>
	BinaryTree<T>::BinaryTree(BinaryTree<T>&& tree)
	{
		Move(tree);
	}

	template <class T>
	BinaryTree<T>::~BinaryTree()
	{
		Destroy();
	}

	template <class T>
	void BinaryTree<T>::Copy(const BinaryTree<T>& tree)
	{
		if (this != &tree)
		{
			Destroy();

			auto copyR = [](BinaryTreeNode<T>* nodePtr) -> BinaryTreeNode<T>*
			{
				if (nodePtr == nullptr)
					return nullptr;

				BinaryTreeNode<T>* newNodePtr = new BinaryTreeNode<T>(nodePtr->data);
				BinaryTreeNode<T>* leftPtr = copyR(nodePtr->leftPtr);
				BinaryTreeNode<T>* leftPtr = copyR(nodePtr->rightPtr);

				newNodePtr->leftPtr = leftPtr;
				newNodePtr->rightPtr = rightPtr;

				return newNodePtr;
			};

			copyR(tree.mRootPtr);
		}
	}

	template <class T>
	void BinaryTree<T>::Move(BinaryTree<T>&& tree)
	{
		if (this != &tree)
		{
			Destroy();

			mRootPtr = tree.mRootPtr;
			tree.mRootPtr = nullptr;
		}
	}

	template <class T>
	void BinaryTree<T>::Destroy()
	{
		PostorderTraversal([this](BinaryTreeNode<T>* nodePtr) -> bool
		{
			RemoveNode(nodePtr);
		});

		mRootPtr = nullptr;
	}

	template <class T>
	BinaryTree<T>& BinaryTree<T>::operator =(const BinaryTree<T>& tree)
	{
		Copy(tree);

		return *this;
	}

	template <class T>
	BinaryTree<T>& BinaryTree<T>::operator =(BinaryTree<T>&& tree)
	{
		Move(tree);

		return *this;
	}

	template <class T>
	void BinaryTree<T>::AddNode(BinaryTreeNode<T>* parentPtr, BT_DIR dir, const T& val)
	{
		assert(dir < BT_DIR::BT_COUNT);

		BinaryTreeNode<T>* newNodePtr = new BinaryTreeNode<T>(val);

		if (parentPtr == nullptr) // add the root node
		{
			mRootPtr = newNodePtr;
		}
		else
		{
			if (dir == BT_DIR::BT_LEFT)
			{
				parentPtr->leftPtr = newNodePtr;
			}
			else if (dir == BT_DIR::BT_RIGHT)
			{
				parentPtr->rightPtr = newNodePtr;
			}

			newNodePtr->parentPtr = parentPtr;
		}
	}

	template <class T>
	void BinaryTree<T>::RemoveNode(BinaryTreeNode<T>* nodePtr)
	{
		assert(nodePtr != nullptr);

		/* Idea: 
			If the node to delete is a leaf, we just delete it, updating the parent links
			If the node to delete is a parent, replace the node to delete with the deepest node in the tree (for sure is a leaf)
		*/

		if (IsLeaf(nodePtr)) // leaf
		{
			// remove the current node from its parent
			if (nodePtr->parentPtr)
			{
				if (nodePtr == nodePtr->parentPtr->leftPtr)
					nodePtr->parentPtr->leftPtr = nullptr;

				if (nodePtr == nodePtr->parentPtr->rightPtr)
					nodePtr->parentPtr->rightPtr = nullptr;
			}
		}
		else // is parent
		{
			// find the deepest node (leaf)
			BinaryTreeNode<T>* deepestNodePtr = nullptr;
			LevelOrderTrversal([&deepestNodePtr](BinaryTreeNode<T>* crrNodePtr, bool isLast)
				{
					if (isLast)
					{
						deepestNodePtr = crrNodePtr;
					}
				});

			assert(deepestNodePtr != nullptr);

			// swap it with the node to delete
			deepestNodePtr->Swap(*nodePtr);
		}

		// delete the node
		nodePtr->parentPtr = nullptr;
		nodePtr->leftPtr = nullptr;
		nodePtr->rightPtr = nullptr;

		delete nodePtr;
	}

	template <class T>
	void BinaryTree<T>::DeleteNode(const T& val)
	{
		BinaryTreeNode<T>* foundNodePtr = FindNode(val);
		assert(foundNodePtr != nullptr);

		RemoveNode(foundNodePtr);
	}

	template <class T>
	BinaryTreeNode<T>* BinaryTree<T>::FindNode(const T& val)
	{
		assert(mRootPtr != nullptr);

		BinaryTreeNode<T>* foundNode = nullptr;
		PreorderTraversal([&foundNode](BinaryTreeNode<T>* nodePtr)
		{
			if (nodePtr)
			{
				if (nodePtr->data == val)
				{
					foundNode = nodePtr;
					return true;
				}
			}
		});

		return foundNode;
	}

	// Breadth First Traversal (variant of BFS)
	template <class T>
	void BinaryTree<T>::LevelOrderTrversal(const std::function<bool(BinaryTreeNode<T>*, bool)>& func) // visiting each level of the tree
	{
		// ONLY ITERATIVE SOLUTION

		/* We use a queue to process nodes by level */
		std::queue<BinaryTreeNode<T>*> queue;
		BinaryTreeNode<T>* nodePtr = nullptr;

		queue.push(mRootPtr);
		while (false == queue.empty())
		{
			nodePtr = queue.front();
			queue.pop();

			bool ret;
			if (func)
				ret = func(nodePtr, queue.size() == 1);

			if (ret) return;

			if (nodePtr->leftPtr)
				queue.push(nodePtr->leftPtr);

			if (nodePtr->rightPtr)
				queue.push(nodePtr->rightPtr);
		}
	}

	// Depth First Traversals
	template <class T>
	void BinaryTree<T>::InorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func) // Left, Root, Right
	{
		// ONLY RECURSIVE SOLUTION

		auto InOrderR = [](BinaryTreeNode<T>* nodePtr)
		{
			if (nodePtr)
			{
				InOrderR(nodePtr->leftPtr);

				bool ret;
				if (func)
					ret = func(nodePtr);

				if (ret) return;

				InOrderR(nodePtr->rightPtr);
			}
		};

		InOrderR(mRootPtr);
	}

	template <class T>
	void BinaryTree<T>::PreorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func) // Root, Left, Right
	{
#ifdef BT_RECURSIVE
		auto PreOrderR = [](BinaryTreeNode<T>* nodePtr)
		{
			if (nodePtr)
			{
				bool ret;
				if (func)
					ret = func(nodePtr);

				if (ret) return;

				PreOrderR(nodePtr->leftPtr);
				PreOrderR(nodePtr->rightPtr);
			}
		};

		PreOrderR(mRootPtr);
#elif BT_ITERATIVE
		/* We use a stack to process nodes */

		std::stack<BinaryTreeNode<T>*> stack;
		BinaryTreeNode<T>* nodePtr = nullptr;

		stack.push(mRootPtr);
		while (false == stack.empty())
		{
			nodePtr = stack.top();
			stack.pop();

			bool ret;
			if (func)
				ret = func(nodePtr);

			if (ret) return;

			if (nodePtr->leftPtr)
				stack.push(nodePtr->leftPtr);

			if (nodePtr->rightPtr)
				stack.push(nodePtr->rightPtr);
		}
#endif // 
	}

	template <class T>
	void BinaryTree<T>::PostorderTraversal(const std::function<bool(BinaryTreeNode<T>*)>& func = default_func) // Left, Right, Root
	{
		// ONLY RECURSIVE SOLUTION

		auto PostOrderR = [](BinaryTreeNode<T>* nodePtr)
		{
			if (nodePtr)
			{
				PostOrderR(nodePtr->leftPtr);
				PostOrderR(nodePtr->rightPtr);
			}

			bool ret;
			if (func)
				ret = func(nodePtr);

			if (ret) return;
		};

		PostOrderR(mRootPtr);
	}

	template <class T>
	size_t BinaryTree<T>::Height(BinaryTreeNode<T>* nodePtr)
	{
		assert(nodePtr == nullptr);

		// ONLY RECURSIVE SOLUTION

		auto HeightR = [](BinaryTreeNode<T>* nodePtr) -> int
		{
			if (nodePtr == nullptr) return -1;

			int leftH = HeightR(nodePtr->leftPtr);
			int rightH = HeightR(nodePtr->rightPtr);

			return 1 + (leftH > rightH ? leftH : rightH);
		};

		HeightR(nodePtr);
	}

	template <class T>
	size_t BinaryTree<T>::Depth(BinaryTreeNode<T>* nodePtr)
	{
		assert(nodePtr == nullptr);

		// ONLY RECURSVE SOLUTION

		if (nodePtr == mRootPtr)
			return 0;

		auto DepthR = [](BinaryTreeNode<T>* nodePtr) -> int
		{
			if (nodePtr == nullptr) return -1;

			int depthP = DepthR(nodePtr->parentPtr);

			return (1 + depthP);
		};

		DepthR(nodePtr);
	}

	template <class T>
	size_t BinaryTree<T>::Size(BinaryTreeNode<T>* nodePtr)
	{
		// ONLY RECURSIVE SOLUTION

		if (nodePtr == nullptr)
		{
			return 0;
		}
		else
		{
			size_t size = Size(nodePtr->leftPtr) + 1 + Size(nodePtr->rightPtr);
			return size;
		}
	}

	template <class T>
	bool BinaryTree<T>::IsLeaf(BinaryTreeNode<T>* nodePtr)
	{
		assert(nodePtr != nullptr);

		return ((nodePtr->leftPtr == nullptr) && (nodePtr->rightPtr == nullptr));
	}
}

#endif /* BINARY_TREE_HPP */