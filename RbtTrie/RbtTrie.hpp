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
			head = buildLinearTrie(key, length, val);
			return true;
		}

		unsigned int len = length;
		pNode node = findClosestMatch(key, &len);

		// Key already exists (or is a prefix of one which exists)
		if (len == 0)
			return false;

		return true;
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

	pNode buildLinearTrie(KE key[], unsigned int length, V *val)
	{
		unsigned int i = length > 63 ? length-63 : 0;
		pNode tempNode = new RbtTrieNode<KE, V>
						(&key[i], length-i, val);
		for (;i > 0; i--)
			tempNode = new RbtTrieNode<KE, V> (key[i-1], tempNode);
		return tempNode;
	}
};

#endif /* RBT_TRIE_HPP */
