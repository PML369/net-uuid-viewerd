/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Ternary Trie,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * The data structure may be thought of as a trie when in use,
 * but internally it uses the space-compressed representation
 * of a ternary tree.
 */

#include <RbtTrie/RbtTrieNode.hpp>

#ifndef SEARCHABLE_TREE_TRIE_HPP
#define SEARCHABLE_TREE_TRIE_HPP

template <typename KE, typename V>
class SearchableTreeTrie {
	friend class RbtTrieInternalTest;
protected:
	typedef RbtTrieNode<KE, V> *pNode;
	pNode head;
public:
	typedef std::pair<KE *, unsigned int> tKey;
	SearchableTreeTrie() {
		head = NULL;
	}

	V *get(const KE key[], unsigned int length) {
		unsigned int matchLength = length;
		pNode match = findClosestMatch(key, &matchLength);
		if (matchLength != 0)
			return NULL;
		return match->getPayload();
	}

protected:
	pNode findClosestMatch(const KE key[], unsigned int *plength)
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
};

#endif /* RBT_TRIE_HPP */