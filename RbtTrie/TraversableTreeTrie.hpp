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

#ifndef TRAVERSABLE_TREE_TRIE_HPP
#define TRAVERSABLE_TREE_TRIE_HPP

template <typename KE, typename V>
class TraversableTreeTrie {
	friend class RbtTrieInternalTest;
protected:
	typedef RbtTrieNode<KE, V> *pNode;
	pNode head;
public:
	typedef std::pair<KE *, unsigned int> tKey;
	TraversableTreeTrie() {
		head = NULL;
	}

	V *get(const KE key[], unsigned int length) {
		unsigned int matchLength = length;
		pNode match = findClosestMatch(key, &matchLength);
		if (matchLength != 0)
			return NULL;
		return match->getPayload();
	}

	template <typename OutputIterator>
	void getKeysWithPrefix(const KE prefix[], unsigned int length,
			OutputIterator out)
	{
		getDataWithKeyPrefix<tKey>(prefix, length, out,
					[](tKey key, V *val) { return key; });
	}

	template <typename OutputIterator>
	void getValuesWithKeyPrefix(const KE prefix[], unsigned int length,
			OutputIterator out)
	{
		getDataWithKeyPrefix<V *>(prefix, length, out,
					[](tKey key, V *val) { return val; });
	}

	template <typename value_type, typename OutputIterator>
	void getDataWithKeyPrefix(const KE prefix[], unsigned int length,
			OutputIterator out, value_type(*callback)(tKey, V *))
	{
		unsigned int matchLength = length;
		pNode match = findClosestMatch(prefix, &matchLength);
		if (matchLength != 0)
			return;

		auto params = std::make_pair(callback, out);

		inOrderTraverse(match, prefix, length,
			&TraversableTreeTrie<KE, V>::
				getDataWithKeyPrefixTraverseAction, &params);
	}

protected:
	template <typename T>
	void inOrderTraverse(pNode start, const KE prefix[],
			unsigned int prefixLength,
			void (*action)(T *, tKey, V *), T *obj)
	{
		unsigned int prefixAlloc = 2 * prefixLength;
		if (prefixAlloc == 0)
			prefixAlloc = 2;
		KE *newPrefix = new KE[prefixAlloc];
		memcpy(newPrefix, prefix, prefixLength * sizeof(KE));
		inOrderTraverseInternal(start, newPrefix, prefixLength,
						prefixAlloc, action, obj);
	}

	template <typename T>
	void inOrderTraverseInternal(pNode start, KE prefix[],
			unsigned int prefixLength, unsigned int prefixAlloc,
			void (*action)(T *, tKey, V *), T *obj)
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
			action(obj,
					tKey(key, prefixLength + suffixLength),
					start->getPayload());
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
						inOrderTraverseInternal(
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
						inOrderTraverseInternal(
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

	template <typename OutputPair>
	static void getDataWithKeyPrefixTraverseAction(OutputPair *out,
			tKey key, V *value)
	{
		*(out->second) = (out->first)(key, value);
		(out->second)++;
	}

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
