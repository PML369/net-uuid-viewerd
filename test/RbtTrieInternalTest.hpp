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
};
