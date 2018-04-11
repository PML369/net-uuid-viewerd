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
private:
	int a;

public:
	void testLongInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 4;
		char k[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+=";
		TS_ASSERT(trie.insert(k, 66, &a));
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
				TS_ASSERT(memcmp(suffix, &k[i], len) == 0);
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

#define testRbtFixupSetup(R, L1, L2) \
		RbtTrie<char, int> trie; \
		RbtTrieNode<char, int> *ptr = NULL; \
		RbtTrieNode<char, int> 	head		('x', ptr), \
					grandparent	('x', ptr), \
					parent		('x', ptr), \
					child		('x', ptr), \
					uncle		('x', ptr); \
		trie.head = &head; \
		head.setLeft(&grandparent); \
		grandparent.setLeft(L1 ? &parent : &uncle); \
		grandparent.setRight(L1 ? &uncle : &parent); \
		if (L2) \
			parent.setLeft(&child); \
		else \
			parent.setRight(&child); \
		\
		head.setRed(false); \
		grandparent.setRed(false); \
		parent.setRed(true); \
		uncle.setRed(R); \
		child.setRed(true); \
		\
		trie.fixupRedBlackInvariants(&child);

	// Different cases for fixupRbtTrie to handle - there are 8,
	// coded as follows:
	// testRbtFixup [uncle colour]
	// 		[grandparent-parent direction]
	// 		[parent-child direction]
	void testRbtFixupRLL(void)
	{
		testRbtFixupSetup(true, true, true)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &parent);
		TS_ASSERT(grandparent.getRight() == &uncle);
		TS_ASSERT(parent.getLeft() == &child);
	}
	void testRbtFixupRLR(void)
	{
		testRbtFixupSetup(true, true, false)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &parent);
		TS_ASSERT(grandparent.getRight() == &uncle);
		TS_ASSERT(parent.getRight() == &child);
	}
	void testRbtFixupRRL(void)
	{
		testRbtFixupSetup(true, false, true)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &uncle);
		TS_ASSERT(grandparent.getRight() == &parent);
		TS_ASSERT(parent.getLeft() == &child);
	}
	void testRbtFixupRRR(void)
	{
		testRbtFixupSetup(true, false, false)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &uncle);
		TS_ASSERT(grandparent.getRight() == &parent);
		TS_ASSERT(parent.getRight() == &child);
	}



	void testRbtFixupBLL(void)
	{
		testRbtFixupSetup(false, true, true)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &parent);
		TS_ASSERT(parent.getLeft() == &child);
		TS_ASSERT(parent.getRight() == &grandparent);
		TS_ASSERT(grandparent.getRight() == &uncle);
	}
	void testRbtFixupBLR(void)
	{
		testRbtFixupSetup(false, true, false)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == true);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == false);

		TS_ASSERT(head.getLeft() == &child);
		TS_ASSERT(child.getLeft() == &parent);
		TS_ASSERT(child.getRight() == &grandparent);
		TS_ASSERT(grandparent.getRight() == &uncle);
	}
	void testRbtFixupBRL(void)
	{
		testRbtFixupSetup(false, false, true)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == true);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == false);

		TS_ASSERT(head.getLeft() == &child);
		TS_ASSERT(child.getRight() == &parent);
		TS_ASSERT(child.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &uncle);
	}
	void testRbtFixupBRR(void)
	{
		testRbtFixupSetup(false, false, false)

		TS_ASSERT(head.isRed() == false);
		TS_ASSERT(grandparent.isRed() == true);
		TS_ASSERT(parent.isRed() == false);
		TS_ASSERT(uncle.isRed() == false);
		TS_ASSERT(child.isRed() == true);

		TS_ASSERT(head.getLeft() == &parent);
		TS_ASSERT(parent.getRight() == &child);
		TS_ASSERT(parent.getLeft() == &grandparent);
		TS_ASSERT(grandparent.getLeft() == &uncle);
	}
};
