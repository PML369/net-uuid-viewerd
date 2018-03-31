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
		return NULL;
	}

	KE **getKeysWithPrefix(KE prefix[]) {
		return NULL;
	}

private:
	pNode findClosestMatch(KE key[], unsigned int *plength)
	{
		pNode node = head;
		unsigned int index = 0;
		while (true)
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
	}

	pNode buildLinearTrie(KE key[], unsigned int length,
					unsigned int suffixLength, V *val)
	{
		unsigned int i;
		if (suffixLength <= length && suffixLength <= 63)
			i = length - suffixLength;
		else
			i = length > 63 ? length-63 : 0;
		pNode tempNode = new RbtTrieNode<KE, V>
						(&key[i], length-i, val);
		for (;i > 0; i--)
			tempNode = new RbtTrieNode<KE, V>(key[i-1], tempNode);
		return tempNode;
	}

	void fixupRedBlackInvariants(pNode newNode)
	{
		if (newNode->getParent() == NULL ||
				newNode->getParent()->getChild() == newNode)
		{
			// This node is the head of its RB tree, so it is black
			newNode->setRed(false);
			return;
		}

		newNode->setRed(true); // On insert, assume node is red for now
		// TODO: Implement rotations etc to maintain invariants
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
		x->setRight(y->getLeft()); // beta
		replaceNode(x, y);
		y->setLeft(x);
	}
	void rotateRight(pNode y) // Idential formulation to CLRS
	{
		if (y == NULL) return;
		pNode x = y->getLeft();
		if (x == NULL) return;
		y->setLeft(x->getRight()); // beta
		replaceNode(y, x);
		x->setRight(y);
	}
};

#endif /* RBT_TRIE_HPP */
