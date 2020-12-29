#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <functional>
#include <iostream>

/*
BinarySearchTree - a type of collection/container to store elements of the same type hierachicaly
 dynamically, in a non-contiguos way. Based on BinaryTree.

TIME COMPLEXITY:
- Traversal/Search = O(log n) best case, O(N) worst case
- Add/Delete an element = O(log n)
- Access an element = O(log n)
- Get size = O(log n)

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- easy traversal to retrieve elements in sorted order

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to Vectors, Arrays
- extra memory needed for node connections (pointers)

USAGES:
1. Manipulate hierarchical data.
2. Make information easy to search (see tree traversal).
3. Sort data of different kinds

Used to implement other data types as:
a) Priority Queue
b) 

MODEL:

	   j    <-- root
	 /   \
	f      k
  /   \      \
 a     h      z    <-- leaves

PRIPRIETIES:
- The left subtree of a node contains only nodes with keys lesser than the node’s key.
- The right subtree of a node contains only nodes with keys greater than the node’s key.
- The left and right subtree each must also be a binary search tree.
- There must be no duplicate nodes.

- Inorder traversal of BST always produces sorted output.
- We can construct a BST with only Preorder or Postorder or Level Order traversal.
Note that we can always get inorder traversal by sorting the only given traversal.

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
	struct BinarySearchTreeNode
	{
		BinarySearchTreeNode()
			: key(), parentPtr(nullptr), leftPtr(nullptr), rightPtr(nullptr)
		{}

		BinarySearchTreeNode(const T& key_)
			: key(key_), parentPtr(nullptr), leftPtr(nullptr), rightPtr(nullptr)
		{}

		BinarySearchTreeNode(const BinarySearchTreeNode<T>& node)
			: key(node.key), parentPtr(node.parentPtr), leftPtr(node.nextPtr), rightPtr(node.rightPtr)
		{}

		~BinarySearchTreeNode()
		{
			parentPtr = nullptr;
			leftPtr = nullptr;
			rightPtr = nullptr;
		}

		Swap(const BinarySearchTreeNode<T>& node)
		{
			SDA::Swap(key, node.key);
			SDA::Swap(parentPtr, node.parentPtr);
			SDA::Swap(leftPtr, node.leftPtr);
			SDA::Swap(rightPtr, node.rightPtr);
		}

		T key;
		BinarySearchTreeNode<T>* parentPtr, * leftPtr, * rightPtr;
	};

	template <class T>
	class BinarySearchTree
	{
	public:
		bool default_func(BinarySearchTreeNode<T>* nodePtr) { return true; };

		BinarySearchTree();
		BinarySearchTree(const BinarySearchTree<T>& tree);
		BinarySearchTree(BinarySearchTree<T>&& tree);
		virtual ~BinarySearchTree();

		BinarySearchTree<T>& operator =(const BinarySearchTree<T>& tree);
		BinarySearchTree<T>& operator =(BinarySearchTree<T>&& tree);

		// adds a new node with key and returns the new node
		BinarySearchTreeNode<T>* AddNode(BinarySearchTreeNode<T>* nodePtr, const T& key);
		// deletes a node with key and returns the new root node 
		BinarySearchTreeNode<T>* DeleteNode(BinarySearchTreeNode<T>* nodePtr, const T& key);

		void RemoveNode(BinarySearchTreeNode<T>* nodePtr);

		// searches for a node with key and returns the found node
		BinarySearchTreeNode<T>* FindNode(BinarySearchTreeNode<T>* nodePtr, const T& key);
		// saerches for the node with smallest key in a given tree
		BinarySearchTreeNode<T>* FindMinKeyNode(BinarySearchTreeNode<T>* nodePtr);


		// Breadth First Traversal (variant of BFS)
		void LevelOrderTrversal(const std::function<bool(BinarySearchTreeNode<T>* nodePtr, bool isLastNode)>& func); // visiting each level of the tree

		// Depth First Traversals
		void InorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func); // Left, Root, Right
		void PreorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func); // Root, Left, Right
		void PostorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func); // Left, Right, Root

		size_t Height(BinarySearchTreeNode<T>* nodePtr);
		size_t Depth(BinarySearchTreeNode<T>* nodePtr);
		size_t Size(BinarySearchTreeNode<T>* nodePtr);

		bool IsLeaf(BinarySearchTreeNode<T>* nodePtr);

	private:
		void Copy(const BinarySearchTree<T>& tree);
		void Move(BinarySearchTree<T>&& tree);
		void Destroy();

		BinarySearchTreeNode<T>* mRootPtr;
	};


}


/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	BinarySearchTree<T>::BinarySearchTree()
		: mRootPtr(nullptr)
	{}

	template <class T>
	BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& tree)
	{
		Copy(tree);
	}

	template <class T>
	BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& tree)
	{
		Move(tree);
	}

	template <class T>
	BinarySearchTree<T>::~BinarySearchTree()
	{
		Destroy();
	}

	template <class T>
	void BinarySearchTree<T>::Copy(const BinarySearchTree<T>& tree)
	{
		if (this != &tree)
		{
			Destroy();

			auto copyR = [](BinarySearchTreeNode<T>* nodePtr) -> BinarySearchTreeNode<T> *
			{
				if (nodePtr == nullptr)
					return nullptr;

				BinarySearchTreeNode<T>* newNodePtr = new BinaryTreeNode<T>(nodePtr->data);
				BinarySearchTreeNode<T>* leftPtr = copyR(nodePtr->leftPtr);
				BinarySearchTreeNode<T>* leftPtr = copyR(nodePtr->rightPtr);

				BinarySearchTreeNode->leftPtr = leftPtr;
				BinarySearchTreeNode->rightPtr = rightPtr;

				return newNodePtr;
			};

			copyR(tree.mRootPtr);
		}
	}

	template <class T>
	void BinarySearchTree<T>::Move(BinarySearchTree<T>&& tree)
	{
		if (this != &tree)
		{
			Destroy();

			mRootPtr = tree.mRootPtr;
			tree.mRootPtr = nullptr;
		}
	}

	template <class T>
	void BinarySearchTree<T>::Destroy()
	{
		PostorderTraversal([this](BinaryTreeNode<T>* nodePtr) -> bool
			{
				RemoveNode(nodePtr);
			});

		mRootPtr = nullptr;
	}

	template <class T>
	BinarySearchTree<T>& BinarySearchTree<T>::operator =(const BinarySearchTree<T>& tree)
	{
		Copy(tree);

		return *this;
	}

	template <class T>
	BinarySearchTree<T>& BinarySearchTree<T>::operator =(BinarySearchTree<T>&& tree)
	{
		Move(tree);

		return *this;
	}

	template <class T>
	BinarySearchTreeNode<T>* BinarySearchTree<T>::AddNode(BinarySearchTreeNode<T>* nodePtr, const T& key)
	{
		if (nodePtr == nullptr) // add the root node
		{
			mRootPtr = new BinarySearchTreeNode<T>(key);
			return mRootPtr;
		}

		if (key < nodePtr->key)
			nodePtr->leftPtr = AddNode(nodePtr->leftPtr, key);
		
		if (key > nodePtr->key)
			nodePtr->rightPtr = AddNode(nodePtr->rightPtr, key);

		// unchanged (same key)
		return nodePtr;
	}

	template <class T>
	void BinarySearchTree<T>::RemoveNode(BinarySearchTreeNode<T>* nodePtr)
	{
		assert(nodePtr != nullptr);

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

		// delete the node
		nodePtr->parentPtr = nullptr;
		nodePtr->leftPtr = nullptr;
		nodePtr->rightPtr = nullptr;

		delete nodePtr;
	}

	template <class T>
	BinarySearchTreeNode<T>* BinarySearchTree<T>::DeleteNode(BinarySearchTreeNode<T>* nodePtr, const T& key)
	{
		if (nodePtr == nullptr)
			return nodePtr;

		BinarySearchTreeNode<T>* nodeToDeletePtr = FindNode(nodePtr, key);
		// the keys are the same

		// node has only one child

		if (nodeToDeletePtr->leftPtr == nullptr)
		{
			BinarySearchTreeNode<T>* newRootPtr = nodeToDeletePtr->rightPtr;

			RemoveNode(nodeToDeletePtr);

			return newRootPtr;
		}
		else if (nodeToDeletePtr->rightPtr == nullptr)
		{
			BinarySearchTreeNode<T>*  newRootPtr = nodeToDeletePtr->rightPtr;

			RemoveNode(nodeToDeletePtr);

			return newRootPtr;
		}

		// node has 2 children
		// 1) we find the inorder succesor (smallest in the right subtree) 
		BinarySearchTreeNode<T>* succNodePtr = FindMinKeyNode(nodeToDeletePtr->rightPtr);

		// 2) Swap node an the found succesor
		nodeToDeletePtr->Swap(*succNodePtr);

		// 3) Delete the succesor
		RemoveNode(succNodePtr);

		return nodePtr;
	}

	template <class T>
	BinarySearchTreeNode<T>* BinarySearchTree<T>::FindNode(BinarySearchTreeNode<T>* nodePtr, const T& key)
	{
		if ((nodePtr == nullptr) || (nodePtr->key == key))
			return nodePtr;

		if (key > nodePtr->key)
			return SearchNode(nodePtr->rightPtr, key);

		return SearchNode(nodePtr->leftPtr, key);
	}

	template <class T>
	BinarySearchTreeNode<T>* BinarySearchTree<T>::FindMinKeyNode(BinarySearchTreeNode<T>* nodePtr)
	{
		// No need to search the whole given tree, we just go down the left branch

		BinarySearchTreeNode<T>* crrNodePtr = nodePtr;

		while (crrNodePtr && crrNodePtr->leftPtr)
		{
			crrNodePtr = crrNodePtr->leftPtr;
		}

		return crrNodePtr;
	}

	// Breadth First Traversal (variant of BFS)
	template <class T>
	void BinarySearchTree<T>::LevelOrderTrversal(const std::function<bool(BinarySearchTreeNode<T>*, bool)>& func) // visiting each level of the tree
	{
		// ONLY ITERATIVE SOLUTION

		/* We use a queue to process nodes by level */
		std::queue<BinarySearchTreeNode<T>*> queue;
		BinarySearchTreeNode<T>* nodePtr = nullptr;

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
	void BinarySearchTree<T>::InorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func) // Left, Root, Right
	{
		// ONLY RECURSIVE SOLUTION

		auto InOrderR = [](BinarySearchTreeNode<T>* nodePtr)
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
	void BinarySearchTree<T>::PreorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func) // Root, Left, Right
	{
#ifdef BT_RECURSIVE
		auto PreOrderR = [](BinarySearchTreeNode<T>* nodePtr)
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

		std::stack<BinarySearchTreeNode<T>*> stack;
		BinarySearchTreeNode<T>* nodePtr = nullptr;

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
	void BinarySearchTree<T>::PostorderTraversal(const std::function<bool(BinarySearchTreeNode<T>*)>& func = default_func) // Left, Right, Root
	{
		// ONLY RECURSIVE SOLUTION

		auto PostOrderR = [](BinarySearchTreeNode<T>* nodePtr)
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
	size_t BinarySearchTree<T>::Height(BinarySearchTreeNode<T>* nodePtr)
	{
		assert(nodePtr == nullptr);

		// ONLY RECURSIVE SOLUTION

		auto HeightR = [](BinarySearchTreeNode<T>* nodePtr) -> int
		{
			if (nodePtr == nullptr) return -1;

			int leftH = HeightR(nodePtr->leftPtr);
			int rightH = HeightR(nodePtr->rightPtr);

			return 1 + (leftH > rightH ? leftH : rightH);
		};

		HeightR(nodePtr);
	}

	template <class T>
	size_t BinarySearchTree<T>::Depth(BinarySearchTreeNode<T>* nodePtr)
	{
		assert(nodePtr == nullptr);

		// ONLY RECURSVE SOLUTION

		if (nodePtr == mRootPtr)
			return 0;

		auto DepthR = [](BinarySearchTreeNode<T>* nodePtr) -> int
		{
			if (nodePtr == nullptr) return -1;

			int depthP = DepthR(nodePtr->parentPtr);

			return (1 + depthP);
		};

		DepthR(nodePtr);
	}

	template <class T>
	size_t BinarySearchTree<T>::Size(BinarySearchTreeNode<T>* nodePtr)
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
	bool BinarySearchTree<T>::IsLeaf(BinarySearchTreeNode<T>* nodePtr)
	{
		assert(nodePtr != nullptr);

		return ((nodePtr->leftPtr == nullptr) && (nodePtr->rightPtr == nullptr));
	}
}

#endif /* BINARY_TREE_HPP */