/*
 * Test suite for RbtTrie class
 *
 * Copyright (c) Peter Lotts 2018
 *
 */

#include <cxxtest/TestSuite.h>
#include <RbtTrie/RbtTrie.hpp>

class RbtTrieInternalTest : public CxxTest::TestSuite
{
public:
	void testLongInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 4;
		char k[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+=";
		TS_ASSERT(trie.insert(k, 66, &x));
		RbtTrieNode<char, int> *node = trie.head;
		TS_ASSERT(node != NULL);
		for (unsigned int i = 0; node != NULL;
					 node = node->getChild(), i++)
		{
			TS_ASSERT(i < 66);
			if (!node->isLeaf())
			{
				TS_ASSERT(node->getKeyEntry() == k[i]);
			}
			else
			{
				unsigned int len = 0;
				char *suffix = node->getSuffixCopy(&len);
				TS_ASSERT(suffix != NULL);
				TS_ASSERT(memcmp(suffix, &k[i], 66-len) == 0);
				delete[] suffix;
			}
		}
	}
	
	void testRotation(void)
	{
		RbtTrieNode<char, int> *p = NULL;
		RbtTrieNode<char, int> 	alpha('x', p),
					beta ('x', p),
					gamma('x', p),
					x    ('x', p),
					y    ('x', p),
					head ('x', p);

		head.setLeft(&x);
		x.setRight(&y);
		x.setLeft(&alpha);
		y.setLeft(&beta);
		y.setRight(&gamma);

		TS_ASSERT(head.getLeft() == &x);
		TS_ASSERT(head.getLeft()->getRight() == &y);
		TS_ASSERT(head.getLeft()->getLeft() == &alpha);
		TS_ASSERT(head.getLeft()->getRight()->getLeft() == &beta);
		TS_ASSERT(head.getLeft()->getRight()->getRight() == &gamma);

		RbtTrie<char, int> t;
		t.rotateLeft (head.getLeft());

		TS_ASSERT(head.getLeft() == &y);
		TS_ASSERT(head.getLeft()->getRight() == &gamma);
		TS_ASSERT(head.getLeft()->getLeft() == &x);
		TS_ASSERT(head.getLeft()->getLeft()->getLeft() == &alpha);
		TS_ASSERT(head.getLeft()->getLeft()->getRight() == &beta);

		t.rotateRight(head.getLeft());

		TS_ASSERT(head.getLeft() == &x);
		TS_ASSERT(head.getLeft()->getRight() == &y);
		TS_ASSERT(head.getLeft()->getLeft() == &alpha);
		TS_ASSERT(head.getLeft()->getRight()->getLeft() == &beta);
		TS_ASSERT(head.getLeft()->getRight()->getRight() == &gamma);
	}

	void testIsBlackPredicate(void)
	{
		RbtTrie<char, int> t;
		RbtTrieNode<char, int> node('a', &a);

		node.setRed(true);
		TS_ASSERT(t.isBlack(&node) == false);
		node.setRed(false);
		TS_ASSERT(t.isBlack(&node) == true);
		TS_ASSERT(t.isBlack(NULL) == true);
	}
};
