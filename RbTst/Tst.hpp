/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a read only Ternary Tree,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * The data structure may be thought of as a trie when in use,
 * but internally it uses the space-compressed representation
 * of a ternary tree.
 */

#include <RbTst/RbTstNode.hpp>
#include <RbTst/TraversableTst.hpp>

#ifndef TST_HPP
#define TST_HPP

template <typename KE, typename V>
class Tst : public TraversableTst<KE, V> {
	friend class RbTstInternalTest;
protected:
	typedef typename TraversableTst<KE, V>::pNode pNode;
public:
	bool insert(const KE key[], unsigned int length, V *val) {
		return (insertImpl(key, length, val) != NULL);
	}

protected:
	pNode insertImpl(const KE key[], unsigned int length, V *val) {
		// The trivial case, we can fit the entire key in the head
		if (this->head == NULL)
		{
			this->head = buildLinearTrie(key, length, length, val);
			return this->head;
		}

		unsigned int len = length;
		pNode node = this->findClosestMatch(key, &len);

		// Key already exists (or is a prefix of one which exists)
		if (len == 0)
			return NULL;
		const KE *keySuffix = &key[length - len];

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
				return NULL;

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

			if (node == this->head)
				this->head = newHead;
			else
				node->getParent()->setChild(newHead);

			delete node;
			delete[] nodeSuffix;
			return newHead;
		}
		else
		{
			pNode branch = buildLinearTrie(keySuffix, len,
								  len-1, val);
			if (branch->getKeyEntry() < node->getKeyEntry())
				node->setLeft(branch);
			else
				node->setRight(branch);

			return branch;
		}
		return NULL;
	}

	pNode buildLinearTrie(const KE key[], unsigned int length,
					      unsigned int suffixLength, V *val)
	{
		unsigned int i;
		if (suffixLength <= length && suffixLength <= 63)
			i = length - suffixLength;
		else
			i = length > 63 ? length-63 : 0;

		KE *newSuffix = new KE[length-i];
		memcpy(newSuffix, &key[i], length-i);
		pNode tempNode = new RbTstNode<KE, V>
						(newSuffix, length-i, val);
		for (;i > 0; i--)
			tempNode = new RbTstNode<KE, V>(key[i-1], tempNode);
		return tempNode;
	}
};

#endif /* TST_HPP */
