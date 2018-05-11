/*
 * Test suite for RbTst class
 *
 * Copyright (c) Peter Lotts 2018
 *
 */

#include <cxxtest/TestSuite.h>
#include <RbTst/RbTst.hpp>

#include <list>
#include <iterator>

class RbTstInternalTest : public CxxTest::TestSuite
{
private:
	int a;

public:
	void testIsBlackPredicate(void)
	{
		RbTst<char, int> t;
		RbTstNode<char, int> node('a', &a);

		node.setRed(true);
		TS_ASSERT(t.isBlack(&node) == false);
		node.setRed(false);
		TS_ASSERT(t.isBlack(&node) == true);
		TS_ASSERT(t.isBlack(NULL) == true);
	}

	void testRotation(void)
	{
		RbTstNode<char, int> *p = NULL;
		RbTstNode<char, int> 	alpha('x', p),
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

		RbTst<char, int> t;
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

	void testCollectionDump(void)
	{
		RbTst<char, int> trie;
		char k1[] = "ananas";
		char k2[] = "anas";
		char k3[] = "as";
		TS_ASSERT(trie.insert(k1, 6, &a));
		TS_ASSERT(trie.insert(k2, 4, &a));
		TS_ASSERT(trie.insert(k3, 2, &a));
		std::list<RbTst<char, int>::tKey> list;
		trie.getKeysWithPrefix(NULL, 0, std::back_inserter(list));

		std::list<RbTst<char, int>::tKey>::iterator it = list.begin();
		TS_ASSERT(it->second == 6);
		TS_ASSERT(memcmp(it->first, k1, 6) == 0);
		delete[] it->first;

		it++;
		TS_ASSERT(it->second == 4);
		TS_ASSERT(memcmp(it->first, k2, 4) == 0);
		delete[] it->first;

		it++;
		TS_ASSERT(it->second == 2);
		TS_ASSERT(memcmp(it->first, k3, 2) == 0);
		delete[] it->first;
	}

	void testGetKeysWithPrefix(void)
	{
		RbTst<char, int> trie;
		char k1[] = "ananas";
		char k2[] = "anas";
		char k3[] = "as";
		char prefix[] = "an";
		TS_ASSERT(trie.insert(k1, 6, &a));
		TS_ASSERT(trie.insert(k2, 4, &a));
		TS_ASSERT(trie.insert(k3, 2, &a));
		std::list<RbTst<char, int>::tKey> list;
		trie.getKeysWithPrefix(prefix, 2, std::back_inserter(list));

		std::list<RbTst<char, int>::tKey>::iterator it = list.begin();
		TS_ASSERT(it->second == 6);
		TS_ASSERT(memcmp(it->first, k1, 6) == 0);
		delete[] it->first;

		it++;
		TS_ASSERT(it->second == 4);
		TS_ASSERT(memcmp(it->first, k2, 4) == 0);
		delete[] it->first;
	}

#define testRbtFixupSetup(R, L1, L2) \
		RbTst<char, int> trie; \
		RbTstNode<char, int> *ptr = NULL; \
		RbTstNode<char, int> 	head		('x', ptr), \
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

	// Different cases for fixupRbTst to handle - there are 8,
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

	void testLongInsertion(void)
	{
		RbTst<char, int> trie;
		int x = 4;
		char k[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+=";
		TS_ASSERT(trie.insert(k, 66, &a));
		RbTstNode<char, int> *node = trie.head;
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

	void testSharedPrefixLeafInsertion(void)
	{
		RbTst<char, int> trie;
		char k1[] = "The quick brown fox";
		char k2[] = "The quick brown dog";
		TS_ASSERT(trie.insert(k1, 19, &a));
		TS_ASSERT(trie.insert(k2, 19, &a));

		RbTstNode<char, int> *node = trie.head;
		TS_ASSERT(node != NULL);
		for (unsigned int i = 0; node != NULL;
					 node = node->getChild(), i++)
		{
			if (!node->isLeaf())
			{
				TS_ASSERT(i < 17);
				TS_ASSERT(node->getKeyEntry() == k1[i]);
			}
			else
			{
				unsigned int len = 0;
				char *suffix = node->getSuffixCopy(&len);
				TS_ASSERT(suffix != NULL);
				TS_ASSERT(memcmp(suffix, &k1[i], len) == 0);
				delete[] suffix;

				TS_ASSERT(node->getParent() != NULL);
				node = node->getParent();
				TS_ASSERT(node->getLeft() != NULL);
				node = node->getLeft();
				TS_ASSERT(node->getChild() != NULL);
				node = node->getChild();

				len = 0;
				suffix = node->getSuffixCopy(&len);
				TS_ASSERT(suffix != NULL);
				TS_ASSERT(memcmp(suffix, &k2[i], len) == 0);
				delete[] suffix;
			}
		}
	}

	void testSharedPrefixInsertion(void)
	{
		RbTst<char, int> trie;
		char k1[] = "The quick brown fox";
		char k2[] = "The quick brown dog";
		char k3[] = "The cat";
		char k4[] = "The ant";
		TS_ASSERT(trie.insert(k1, 19, &a));
		// This insertion guarantees that the trie contains
		// "The quick brown " for us to play with
		TS_ASSERT(trie.insert(k2, 19, &a));

		// This causes a split after "The "
		TS_ASSERT(trie.insert(k3, 7, &a));

		// This causes a violation of the RB rules, and so a rotation
		TS_ASSERT(trie.insert(k4, 7, &a));

		// Move through to the node with the split
		RbTstNode<char, int> *node = trie.head;
		TS_ASSERT(node != NULL);
		for (unsigned int i = 0; node != NULL && i < 4;
					 node = node->getChild(), i++)
			;

		TS_ASSERT(node->getKeyEntry() == 'c');
		TS_ASSERT(node->getLeft() != NULL);
		TS_ASSERT(node->getLeft()->getKeyEntry() == 'a');
		TS_ASSERT(node->getRight() != NULL);
		TS_ASSERT(node->getRight()->getKeyEntry() == 'q');
	}
};
