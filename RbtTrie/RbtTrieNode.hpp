/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Red-Black Ternary Trie,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * This class represents each node of the Ternary Trie,
 * holding pointers to neighbour nodes and to payload data
 * at the leaves of the tree.
 */

#ifndef RBT_TRIE_NODE_HPP
#define RBT_TRIE_NODE_HPP

template <typename KE, typename V>
class RbtTrieNode
{
private:
	struct
	{
		bool isRed : 1;
		bool isLeaf : 1;
		/* NB: This places a 64-element limit on suffixes */
		unsigned int suffixLen : 6;
	} flags;

	KE keyEntry;
	RbtTrieNode *middle;

	// To save space, the left and right pointers get re-purposed when
	// we are a leaf node. The left pointer points to the payload of a
	// leaf node, and the right points to a KE array containing the
	// key suffix, if we have one.

	union {
	       RbtTrieNode *node;
	       V *payload;
	} left;

	union {
		RbtTrieNode *node;
		KE *suffix;
	} right;

public:
	/* Leaf non-suffix constructor */
	RbtTrieNode(KE keyEntry, V *payload)
	{
		constructWithSingletonSuffix(keyEntry, payload);
	}
	/* Leaf suffix constructor */
	RbtTrieNode(KE *keySuffix, unsigned int suffixLen, V *payload)
	{
		if (suffixLen == 1)
		{
			constructWithSingletonSuffix(keySuffix[0], payload);
			return;
		}

		left.payload = payload;
		middle = NULL;
		right.suffix = new KE[suffixLen];
		memcpy(right.suffix, keySuffix, suffixLen);
		flags.isRed = false;
		flags.isLeaf = true;
		flags.suffixLen = suffixLen;
		if ((unsigned int)flags.suffixLen != suffixLen)
			throw std::out_of_range("Key suffix too long");
	}
	/* Internal node constructor */
	RbtTrieNode(KE keyEntry, RbtTrieNode *child)
	{
		this->keyEntry = keyEntry;
		left.node = NULL;
		middle = child;
		right.node = NULL;
		flags.isRed = false;
		flags.isLeaf = false;
		flags.suffixLen = 1;
	}

	bool isRed()  { return flags.isRed;  }
	bool isLeaf() { return flags.isLeaf; }

	RbtTrieNode *getLeft()
	{
		return flags.isLeaf ? NULL : left.node;
	}
	void setLeft(RbtTrieNode *node)
	{
		if (!flags.isLeaf)
			left.node = node;
	}

	RbtTrieNode *getRight()
	{
		return flags.isLeaf ? NULL : right.node;
	}
	void setRight(RbtTrieNode *node)
	{
		if (!flags.isLeaf)
			right.node = node;
	}

	V *getPayload()
	{
		return flags.isLeaf ? left.payload : NULL;
	}
	RbtTrieNode *getChild()
	{
		return flags.isLeaf ? NULL : middle;
	}

	KE getKeyEntry() { return keyEntry; }
	bool matchesKeySuffix(KE key[], unsigned int length)
	{
		if (!flags.isLeaf || length != flags.suffixLen)
			return false;

		if (length == 1)
			return !(keyEntry < key[0] || key[0] < keyEntry);

		KE *suffix = right.suffix;
		for (int i=0; i<length; i++)
			if (key[i] < suffix[i] || suffix[i] < key[i])
				return false;
		return true;
	}
	KE *getSuffixCopy(unsigned int *len)
	{
		if (!flags.isLeaf)
			return NULL;

		*len += flags.suffixLen;
		KE *copy = new KE[*len];
		if (flags.suffixLen == 1)
			copy[0] = keyEntry;
		else
			memcpy(copy, right.suffix, *len);
		return copy;
	}

	~RbtTrieNode()
	{
		if (isLeaf() && right.suffix != NULL)
			delete[] right.suffix;
	}

private:
	void constructWithSingletonSuffix(KE keyEntry, V *payload)
	{
		this->keyEntry = keyEntry;
		left.payload = payload;
		middle = NULL;
		right.suffix = NULL;
		flags.isRed = false;
		flags.isLeaf = true;
		flags.suffixLen = 1;
	}
};
#endif /* RBT_TRIE_NODE_HPP */
