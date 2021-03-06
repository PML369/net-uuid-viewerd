/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Red-Black Ternary Tree,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * This class represents each node of the Ternary Tree,
 * holding pointers to neighbour nodes and to payload data
 * at the leaves of the tree.
 */

#ifndef RB_TST_NODE_HPP
#define RB_TST_NODE_HPP

template <typename KE, typename V>
class RbTstNode
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
	RbTstNode *middle;
	RbTstNode *parent;

	// To save space, the left and right pointers get re-purposed when
	// we are a leaf node. The left pointer points to the payload of a
	// leaf node, and the right points to a KE array containing the
	// key suffix, if we have one.

	union {
	       RbTstNode *node;
	       V *payload;
	} left;

	union {
		RbTstNode *node;
		KE *suffix;
	} right;

public:
	/* Leaf non-suffix constructor */
	RbTstNode(KE keyEntry, V *payload)
	{
		constructWithSingletonSuffix(keyEntry, payload);
	}
	/* Leaf suffix constructor */
	RbTstNode(KE *keySuffix, unsigned int suffixLen, V *payload)
	{
		if (suffixLen == 1)
		{
			constructWithSingletonSuffix(keySuffix[0], payload);
			return;
		}

		left.payload = payload;
		middle = NULL;
		parent = NULL;
		right.suffix = new KE[suffixLen];
		memcpy(right.suffix, keySuffix, suffixLen);
		flags.isRed = false;
		flags.isLeaf = true;
		flags.suffixLen = suffixLen;
		if ((unsigned int)flags.suffixLen != suffixLen)
			throw std::out_of_range("Key suffix too long");
	}
	/* Internal node constructor */
	RbTstNode(KE keyEntry, RbTstNode *child)
	{
		this->keyEntry = keyEntry;
		left.node = NULL;
		middle = child;
		parent = NULL;
		if (child != NULL)
			child->setParent(this);
		right.node = NULL;
		flags.isRed = false;
		flags.isLeaf = false;
		flags.suffixLen = 1;
	}

	bool isLeaf() { return flags.isLeaf; }
	bool isRed()  { return flags.isRed;  }
	void setRed(bool red) { flags.isRed = red; }

	RbTstNode *getLeft()
	{
		return flags.isLeaf ? NULL : left.node;
	}
	void setLeft(RbTstNode *node)
	{
		if (flags.isLeaf)
			return;
		left.node = node;
		node->setParent(this);
	}
	void clearLeft(void)
	{
		if (flags.isLeaf) return;
		if (left.node)
			left.node->clearParent();
		left.node = NULL;
	}

	RbTstNode *getRight()
	{
		return flags.isLeaf ? NULL : right.node;
	}
	void setRight(RbTstNode *node)
	{
		if (flags.isLeaf)
			return;
		right.node = node;
		node->setParent(this);
	}
	void clearRight(void)
	{
		if (flags.isLeaf) return;
		if (right.node)
			right.node->clearParent();
		right.node = NULL;
	}

	V *getPayload()
	{
		return flags.isLeaf ? left.payload : NULL;
	}
	RbTstNode *getParent()
	{
		return parent;
	}
	void setParent(RbTstNode *newParent)
	{
		parent = newParent;
	}
	void clearParent(void) { parent = NULL; }
	RbTstNode *getChild()
	{
		return flags.isLeaf ? NULL : middle;
	}
	bool setChild(RbTstNode *newChild)
	{
		if (flags.isLeaf)
			return false;
		middle = newChild;
		middle->setParent(this);
		return true;
	}

	KE getKeyEntry() { return keyEntry; }
	bool matchesKeySuffix(const KE key[], unsigned int length)
	{
		if (length != flags.suffixLen)
			return false;

		return matchesPrefixOfKeySuffix(key, length);
	}
	bool matchesPrefixOfKeySuffix(const KE key[], unsigned int length)
	{
		if (!flags.isLeaf || length > flags.suffixLen)
			return false;

		if (flags.suffixLen == 1)
		{
			if (length == 1)
				return !(keyEntry < key[0] ||key[0] < keyEntry);
			else
				return false;
		}

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
	unsigned int getSuffixLength()
	{
		return flags.suffixLen;
	}

	~RbTstNode()
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
		parent = NULL;
		right.suffix = NULL;
		flags.isRed = false;
		flags.isLeaf = true;
		flags.suffixLen = 1;
	}
};
#endif /* RB_TST_NODE_HPP */
