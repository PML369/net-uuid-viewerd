/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Ternary Tree,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * The data structure may be thought of as a trie when in use,
 * but internally it uses the space-compressed representation
 * of a ternary tree.
 */

#include <RbTst/RbTstNode.hpp>

#ifndef SEARCHABLE_TST_HPP
#define SEARCHABLE_TST_HPP

template <typename KE, typename V>
class SearchableTst {
	friend class RbTstInternalTest;
protected:
	typedef RbTstNode<KE, V> *pNode;
	pNode head;
public:
	typedef std::pair<KE *, unsigned int> tKey;
	SearchableTst() {
		head = NULL;
	}

	V *get(const KE key[], unsigned int length) {
		unsigned int matchLength;
		pNode match;
		if (findClosestMatch(key, length, &match, &matchLength)
				&& matchLength == 0 && match->isLeaf())
			return match->getPayload();
		return NULL;
	}

protected:
	bool findClosestMatch(const KE key[], unsigned int length,
				pNode *match, unsigned int *nodeMatchLength)
	{
		if (head == NULL)
			return false;
		*match = head;
		unsigned int index = 0;
		while (length > index)
		{
			if ((*match)->isLeaf())
			{
				if ((*match)->matchesPrefixOfKeySuffix(
						&key[index], length-index))
				{
					*nodeMatchLength = (*match)
						->getSuffixLength() - 
						(length - index);
					return true;
				}
				*nodeMatchLength = index;
				return false;
			}
			else
			{
				KE testKeyEntry = (*match)->getKeyEntry();
				pNode newNode;
				if (key[index] < testKeyEntry)
					newNode = (*match)->getLeft();
				else if (testKeyEntry < key[index])
					newNode = (*match)->getRight();
				else
				{
					newNode = (*match)->getChild();
					index++;
				}

				if (newNode == NULL)
				{
					*nodeMatchLength = index;
					return false;
				}
				(*match) = newNode;
			}
		}
		// We've consumed all input (perhaps we are prefix matching)
		(*nodeMatchLength) = ((*match)->isLeaf() ?
					(*match)->getSuffixLength() : 0);
		return true;
	}
};

#endif /* SEARCHABLE_TST_HPP */
