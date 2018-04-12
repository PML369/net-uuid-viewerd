/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Red-Black Ternary Trie,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * The data structure may be thought of as a trie when in use,
 * but internally it uses the space-compressed representation
 * of a ternary tree, with each level of the ternary tree kept
 * balanced by making that tree a red-black tree.
 */

#include <RbtTrie/RbtTrieNode.hpp>

#ifndef RBT_TRIE_HPP
#define RBT_TRIE_HPP

template <typename KE, typename V>
class RbtTrie {
	friend class RbtTrieInternalTest;
private:
	typedef RbtTrieNode<KE, V> *pNode;
	pNode head;
public:
	typedef std::pair<KE *, unsigned int> tKey;
	RbtTrie() {
		head = NULL;
	}

	bool insert(KE key[], unsigned int length, V *val) {
		// The trivial case, we can fit the entire key in the head
		if (head == NULL)
		{
			head = buildLinearTrie(key, length, length, val);
			return true;
		}

		unsigned int len = length;
		pNode node = findClosestMatch(key, &len);

		// Key already exists (or is a prefix of one which exists)
		if (len == 0)
			return false;
		KE *keySuffix = &key[length - len];

		if (node->isLeaf())
		{
			unsigned int nodeSuffixLen = 0;
			KE *nodeSuffix = node->getSuffixCopy(&nodeSuffixLen);

			unsigned int suffixLen = (len < nodeSuffixLen ? len :
								nodeSuffixLen);
			unsigned int i = 0;
			for (; i < suffixLen; i++)
				if (keySuffix[i] < nodeSuffix[i] ||
						nodeSuffix[i] < keySuffix[i])
					break;
			if (i == suffixLen) // One key is a prefix of another
				return false;

			pNode newHead = buildLinearTrie(nodeSuffix,
							nodeSuffixLen,
							(nodeSuffixLen -i-1),
							node->getPayload());

			pNode split = newHead;
			for (; !split->isLeaf(); split = split->getChild()) ;
			split = split->getParent();

			pNode branch = buildLinearTrie(&keySuffix[i],
							len-i, len -i-1, val);
			branch->setRed(true);
			if (branch->getKeyEntry() < split->getKeyEntry())
				split->setLeft(branch);
			else
				split->setRight(branch);

			if (node == head)
				head = newHead;
			else
				node->getParent()->setChild(newHead);

			delete node;
			delete[] nodeSuffix;
			return true;
		}
		else
		{
			pNode branch = buildLinearTrie(keySuffix, len,
								  len-1, val);
			if (branch->getKeyEntry() < node->getKeyEntry())
				node->setLeft(branch);
			else
				node->setRight(branch);

			fixupRedBlackInvariants(branch);
			return true;
		}
		return false;
	}

	V *get(KE key[], unsigned int length) {
		unsigned int matchLength = length;
		pNode match = findClosestMatch(key, &matchLength);
		if (matchLength != 0)
			return NULL;
		return match->getPayload();
	}

	template <typename OutputIterator>
	void getKeysWithPrefix(KE prefix[], unsigned int length,
			OutputIterator out)
	{
		unsigned int matchLength = length;
		pNode match = findClosestMatch(prefix, &matchLength);
		if (matchLength != 0)
			return;

		inOrderTraverse(match, prefix, length, length,
			&RbtTrie<KE, V>::getKeysWithPrefixTraverseAction, &out);
	}

private:
	template <typename T>
	void inOrderTraverse(pNode start, KE prefix[],
			unsigned int prefixLength, unsigned int prefixAlloc,
			void (*action)(T *, tKey), T *obj)
	{
		if (start == NULL)
			return;

		if (start->isLeaf())
		{
			unsigned int suffixLength = 0;
			KE *suffix = start->getSuffixCopy(&suffixLength);
			KE *key = new KE[prefixLength + suffixLength];
			memcpy(key, prefix, prefixLength);
			memcpy(&key[prefixLength], suffix, suffixLength);
			action(obj, tKey(key, prefixLength + suffixLength));
			delete[] suffix;
			return;
		}

		pNode n = start;
		pNode prev = NULL;
		while (true)
		{
			bool pathFailedTryNext = false;

			if (prev == NULL || prev == n->getParent())
			{
				if (n->getLeft() != NULL)
				{
					// go left
					prev = n;
					n = n->getLeft();
					continue;
				}
				pathFailedTryNext = true;
			}

			if (pathFailedTryNext || prev == n->getLeft())
			{
				if (n->getChild() != NULL)
				{
					// recursive call on child
					if (prefixLength < prefixAlloc)
					{
						prefix[prefixLength] =
							n->getKeyEntry();
						inOrderTraverse(
							n->getChild(),
							prefix,
							prefixLength + 1,
							prefixAlloc,
							action, obj);
					}
					else
					{
						KE *newPrefix = new
							KE[prefixAlloc * 2];
						memcpy(newPrefix, prefix,
								prefixLength);
						newPrefix[prefixLength] =
							n->getKeyEntry();
						inOrderTraverse(
							n->getChild(),
							newPrefix,
							prefixLength + 1,
							prefixAlloc * 2,
							action, obj);
					}
				}

				if (n->getRight() != NULL)
				{
					// go right
					prev = n;
					n = n->getRight();
					continue;
				}
				pathFailedTryNext = true;
			}

			if (pathFailedTryNext || prev == n->getRight())
			{
				if (n == start)
					break;
				prev = n;
				n = n->getParent();
			}
		}
	}

	template <typename OutputIterator>
	static void getKeysWithPrefixTraverseAction(OutputIterator *out,
			tKey key)
	{
		*((*out)++) = key;
	}

	pNode findClosestMatch(KE key[], unsigned int *plength)
	{
		if (head == NULL)
			return NULL;
		pNode node = head;
		unsigned int index = 0;
		while (*plength > index)
		{
			if (node->isLeaf())
			{
				if (node->matchesKeySuffix(&key[index],
							   *plength-index))
					*plength = 0;
				else
					*plength -= index;
				return node;
			}
			else
			{
				KE testKeyEntry = node->getKeyEntry();
				pNode newNode;
				if (key[index] < testKeyEntry)
					newNode = node->getLeft();
				else if (testKeyEntry < key[index])
					newNode = node->getRight();
				else
				{
					newNode = node->getChild();
					index++;
				}

				if (newNode == NULL)
				{
					*plength -= index;
					return node;
				}
				node = newNode;
			}
		}
		// We've consumed all input (perhaps we are prefix matching)
		*plength = 0;
		return node;
	}

	pNode buildLinearTrie(KE key[], unsigned int length,
					unsigned int suffixLength, V *val)
	{
		unsigned int i;
		if (suffixLength <= length && suffixLength <= 63)
			i = length - suffixLength;
		else
			i = length > 63 ? length-63 : 0;

		KE *newSuffix = new KE[length-i];
		memcpy(newSuffix, &key[i], length-i);
		pNode tempNode = new RbtTrieNode<KE, V>
						(newSuffix, length-i, val);
		for (;i > 0; i--)
			tempNode = new RbtTrieNode<KE, V>(key[i-1], tempNode);
		return tempNode;
	}

	void fixupRedBlackInvariants(pNode newNode)
	{
		// On insert, assume node is red for now
		newNode->setRed(true);
		pNode p, n = newNode;

		while (n->isRed())
		{
			if ((p = n->getParent()) == NULL || p->getChild() == n)
			{
				// This node is the head of its RB tree,
				// so it is black
				n->setRed(false);
				return;
			}

			// Rule out simple case where no further work is needed
			if (!p->isRed())
				return;
			pNode pp = p->getParent();

			// Bad case 1: Red promotion
			// 	The parent and uncle are both red
			// 	(with the grandparent necessarily black).
			// Either     B	    or    B     or    B     or    B
			//           / \         / \         / \         / \
			//          R   R       R   R       R   R       R   R
			//         / \	           / \         / \     / \
			//        R    	              R       R           R
			if (!isBlack(pp->getLeft()) && !isBlack(pp->getRight()))
			{
				pp->getLeft()->setRed(false);
				pp->getRight()->setRed(false);
				pp->setRed(true);
				newNode = pp;
				continue;
			}

			// Bad case 2: Rotations into case 3
			// Either     B	    or    B
			//           / \         / \
			//     (a)  R   B   (b) B   R
			//         / \	           / \
			//            R	          R
			if (isOnLeft(p) && isOnRight(n)) // Case 2a
			{
				n = p;
				rotateLeft(n);
				p = n->getParent();
			}
			else if (isOnRight(p) && isOnLeft(n)) // Case 2b
			{
				n = p;
				rotateRight(n);
				p = n->getParent();
			}

			// Bad case 3: Rotations
			// Either     B	    or    B
			//           / \         / \
			//     (a)  R   B   (b) B   R
			//         / \	           / \
			//        R    	              R
			if (isOnLeft(p) && isOnLeft(n)) // Case 3a
			{
				p->setRed(false);
				pp->setRed(true);
				rotateRight(pp);
				n = p;
			}
			else if (isOnRight(p) && isOnRight(n)) // Case 3b
			{
				p->setRed(false);
				pp->setRed(true);
				rotateLeft(pp);
				n = p;
			}
		}
	}
	

	bool hasOneLRChild(pNode n)
	{
		return ((n->getLeft() == NULL && n->getRight() != NULL)
		     || (n->getLeft() != NULL && n->getRight() == NULL));
	}
	bool isOnLeft(pNode child)
	{
		if (child == NULL || child->getParent() == NULL)
			return false;
		return child->getParent()->getLeft() == child;
	}
	bool isOnRight(pNode child)
	{
		if (child == NULL || child->getParent() == NULL)
			return false;
		return child->getParent()->getRight() == child;
	}
	bool isBlack(pNode n)
	{
		return n == NULL || !n->isRed();
	}
	void replaceNode(pNode nOld, pNode nNew)
	{
		pNode p = nOld->getParent();
		if (p == NULL)
		{
			if (nOld == head)
			{
				head = nNew;
				nNew->clearParent();
			}
			return;
		}

		if (nOld == p->getLeft())
			p->setLeft(nNew);
		if (nOld == p->getChild())
			p->setChild(nNew);
		if (nOld == p->getRight())
			p->setRight(nNew);
	}
	void rotateLeft(pNode x) // Idential formulation to CLRS
	{
		if (x == NULL) return;
		pNode y = x->getRight();
		if (y == NULL) return;

		pNode beta = y->getLeft();
		if (beta != NULL)
			x->setRight(beta);
		else
			x->clearRight();

		replaceNode(x, y);
		y->setLeft(x);
	}
	void rotateRight(pNode y) // Idential formulation to CLRS
	{
		if (y == NULL) return;
		pNode x = y->getLeft();
		if (x == NULL) return;

		pNode beta = x->getRight();
		if (beta != NULL)
			y->setLeft(beta);
		else
			y->clearLeft();

		replaceNode(y, x);
		x->setRight(y);
	}
};

#endif /* RBT_TRIE_HPP */
