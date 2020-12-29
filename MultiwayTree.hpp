#ifndef MULTIWAY_TREE_HPP
#define MULTIWAY_TREE_HPP

#include <cstddef> // size_t
#include <string>
#include <functional>
#include <iostream>
#include <cassert>

namespace SDA
{
	/* MultiwayTree container */
	template <class T>
	class MultiwayTree
	{
	public:
		template <class T>
		struct Node
		{
			Node()
				: name(), data(), parent(nullptr)
				, next(nullptr), prev(nullptr)
				, children(nullptr), childrenCount(0)
				, height(0), depth(0)
			{}

			Node(const T& newData)
				: Node(), data(newData)
			{}

			virtual ~Node()
			{
				data.~T();
				parent = nullptr;
				next = nullptr;
				prev = nullptr;
				children = nullptr;
				childrenCount = 0;
				height = 0;
				depth = 0;
			}

			std::string name; //TODO replace with our SDA::String
			T data;
			Node* parent;
			Node* next;
			Node* prev;
			Node* children;
			size_t childrenCount;
			size_t height;
			size_t depth;
		};

		//using is the way to go when defining a templated typedef
		template <class T>
		using NodeT = Node<T>;

		template <class T>
		using NodePtr = NodeT<T>*;


		MultiwayTree();
		MultiwayTree(const T& rootData);
		MultiwayTree(MultiwayTree<T>::NodePtr<T> rootNode);
		MultiwayTree(const MultiwayTree<T>& tree);
		MultiwayTree(MultiwayTree<T>&& tree);
		virtual ~MultiwayTree();
		void destroy();

		MultiwayTree<T>& operator =(const MultiwayTree<T>& tree);
		MultiwayTree<T>& operator =(MultiwayTree<T>&& tree);

		bool isEmpty() const;
		size_t height() const;
		size_t height(const std::string& nodeName) const;

		size_t depth() const;
		size_t depth(const std::string& nodeName) const;

		MultiwayTree<T>::NodePtr<T> findNode(const std::string& nodeName);
		void addNode(const std::string& parentName, MultiwayTree<T>::NodePtr<T> nodeToAdd);
		void removeNode(const std::string& nodeNameToRemove);

		enum class TR_DIR : unsigned short
		{
			TR_DIR_FORWARD = 0,
			TR_DIR_BACKWARD,
			TR_DIR_COUNT
		};

		void default_func(MultiwayTree<T>::NodePtr<T> node) {};

		MultiwayTree<T>::NodePtr<T> traverse(MultiwayTree<T>::NodePtr<T> startNode, const std::string& finishNodeName,
			const std::function<void(MultiwayTree<T>::NodePtr<T> node)>& func = default_func, TR_DIR dir = TR_DIR::TR_DIR_FORWARD);

	private:
		NodePtr<T> mRoot;
		NodePtr<T> mBottom;
	};

	// cin & cout
	template <class T>
	std::ostream& operator << (std::ostream& out, const MultiwayTree<T>& tree)
	{
	/*	tree.traverse(nullptr, "", [&out](MultiwayTree<T>::NodePtr<T> node)
		{
			if (node)
			{
				out << node.name << " ";
				out << node.data << " ";
			}
		}
		, MultiwayTree<T>::TR_DIR::TR_DIR_FORWARD);

		std::cout << std::endl;
	*/
		return out;
	}

	template <class T>
	std::istream& operator >> (std::istream& in, MultiwayTree<T>& tree)
	{
		//TODO

		return in;
	}
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	MultiwayTree<T>::MultiwayTree()
		: mRoot(nullptr), mBottom(nullptr)
	{}

	template <class T>
	MultiwayTree<T>::MultiwayTree(const T& rootData)
		: MultiwayTree()
	{
		mRoot = new Node(rootData);
	}

	template <class T>
	MultiwayTree<T>::MultiwayTree(MultiwayTree<T>::NodePtr<T> rootNode)
		: MultiwayTree()
	{
		mRoot = rootNode;
	}

	template <class T>
	MultiwayTree<T>::MultiwayTree(const MultiwayTree<T>& tree)
		: MultiwayTree()
	{
		if (this != &tree)
		{
			mRoot = tree.mRoot;
		}
	}

	template <class T>
	MultiwayTree<T>::MultiwayTree(MultiwayTree<T>&& tree)
	{
		if (this != &tree)
		{
			// destroy old tree
			destroy();

			// copy pointer of the new root node
			mRoot = tree.mRoot;

			// invalidate new tree
			tree.mRoot = nullptr;
			tree.mBottom = nullptr;
		}
	}

	template <class T>
	MultiwayTree<T>::~MultiwayTree()
	{
		destroy();
	}

	template <class T>
	void MultiwayTree<T>::destroy()
	{
		traverse(mBottom, "", [](NodePtr<T> node)
			{
				if (node) { delete node; }
			}
		, TR_DIR::TR_DIR_BACKWARD);
	}

	template <class T>
	MultiwayTree<T>& MultiwayTree<T>::operator =(const MultiwayTree<T>& tree)
	{
		MultiwayTree(tree);

		return *this;
	}

	template <class T>
	MultiwayTree<T>& MultiwayTree<T>::operator =(MultiwayTree<T>&& tree)
	{
		MultiwayTree(tree);

		return *this;
	}


	template <class T>
	bool MultiwayTree<T>::isEmpty() const
	{
		return (mRoot && mRoot->children);
	}

	template <class T>
	size_t MultiwayTree<T>::height() const
	{
		return mRoot->height;
	}

	template <class T>
	size_t MultiwayTree<T>::height(const std::string& nodeName) const
	{
		NodePtr<T> node = findNode(nodeName);

		if (node)
		{
			return node->height;
		}
		else
		{
			// error, node not found!
		}

		return 0;
	}

	template <class T>
	size_t MultiwayTree<T>::depth() const
	{
		return mBottom->depth;
	}

	template <class T>
	size_t MultiwayTree<T>::depth(const std::string& nodeName) const
	{
		NodePtr<T> node = findNode(nodeName);

		if (node)
		{
			return node->depth;
		}
		else
		{
			// error, node not found!
		}

		return 0;
	}

	template <class T>
	MultiwayTree<T>::NodePtr<T> MultiwayTree<T>::findNode(const std::string& nodeName)
	{
		if (nodeName.empty())
		{
			//error
		}

		return traverse(mRoot, nodeName);
	}

	template <class T>
	MultiwayTree<T>::NodePtr<T> MultiwayTree<T>::traverse(MultiwayTree<T>::NodePtr<T> startNode, const std::string& finishNodeName,
		const std::function<void(MultiwayTree<T>::NodePtr<T> node)>& func, TR_DIR dir)
	{
		// if startNode is nullprr we use the mRoot
		if (startNode == nullptr)
		{
			startNode = mRoot;
		}

		//if finishNodeName.empty() we go to till the last node in the tree
		//(complete traversal)

		NodePtr<T> returnNode = nullptr;
		NodePtr<T> crrNode = nullptr;

		NodePtr<T> dirNode = nullptr;
		NodePtr<T> verticalNode = nullptr;
		if (dir == TR_DIR::TR_DIR_FORWARD) //forward
		{
			dirNode = crrNode->next;
			verticalNode = crrNode->children;
		}
		else //backward
		{
			dirNode = crrNode->prev;
			verticalNode = crrNode->parent;
		}

		for (crrNode = startNode; crrNode != nullptr; crrNode = dirNode)
		{
			if (false == finishNodeName.empty())
			{
				if (crrNode->name.compare(finishNodeName) == 0)
				{
					return crrNode;
				}
			}
			else
			{
				// no stoping node, go till the last node of the tree - complete traversal
			}

			// if we have to also do something per node
			if (func)
			{
				func(crrNode);
			}

			returnNode = traverse(verticalNode, finishNodeName, func, dir);
		}

		return returnNode;
	}

	template <class T>
	void MultiwayTree<T>::addNode(const std::string& parentName, MultiwayTree<T>::NodePtr<T> nodeToAdd)
	{
		// we consider that the new node is a leaf node!

		if (parentName.empty())
		{
			//error
		}

		if (nodeToAdd == nullptr)
		{
			// error, early exit
		}

		NodePtr<T> parent = nullptr;
		if (parentName.empty()) // root node case
		{
			parent = mRoot;
		}
		else // other
		{
			parent = findNode(parentName);
		}

		if (parent)
		{
			if (parent->childrenCount == 0)
			{
				parent->children = nodeToAdd;
			}
			else
			{
				if (parent->children)
				{
					// iterate to the last child that has no next
					NodePtr<T> crrNode = nullptr;
					for (crrNode = parent->children; crrNode != nullptr; crrNode = crrNode->next);
					// add the new child as next
					crrNode->next = nodeToAdd;
					nodeToAdd->prev = crrNode;
				}
			}

			nodeToAdd->parent = parent;
			parent->childrenCount++;

			// the depth of the parent is up to date and can be used
			nodeToAdd->depth = nodeToAdd->parent->depth + 1;

			// to update the height we must go back to the root and update the height of every parent
			traverse(nodeToAdd, "", [](NodePtr<T> node)
				{
					if (node) { node->parent->height++; }
				}
			, TR_DIR::TR_DIR_BACKWARD);
		}
		else
		{
			// parent not found!
		}
	}

	template <class T>
	void  MultiwayTree<T>::removeNode(const std::string& nodeNameToRemove)
	{
		if (nodeNameToRemove.empty())
		{
			//error
		}

		NodePtr<T> nodeToRemove = findNode(nodeNameToRemove);

		if (nodeToRemove == nullptr)
		{
			//error
		}

		if ((nodeToRemove == mRoot) || (nodeToRemove->parent == nullptr))
		{
			// error, can not delete the root node!
		}

		if (nodeToRemove->children) // is parent
		{
			if (nodeToRemove->parent) // has parent
			{
				addNode(nodeToRemove->parent, nodeToRemove->children->name);
			}
			else // is root
			{
				// we shouldn't get here!!! error!
			}
		}

		if (nodeToRemove)
		{
			if (nodeToRemove->prev)
			{
				nodeToRemove->prev->next = nodeToRemove->next;
			}
			else
			{
				//error , invalid prev node!
			}

			// next node is not mandatory to exist!
			if (nodeToRemove->next)
			{
				nodeToRemove->next->prev = nodeToRemove->prev;
			}

			nodeToRemove->parent->childrenCount--;

			delete nodeToRemove;
		}
		else
		{
			// node not found!
		}
	}
}

#endif /* MULTIWAY_TREE_HPP */