/*
 * Test suite for RbtTrie class
 *
 * Copyright (c) Peter Lotts 2018
 *
 */

#include <cxxtest/TestSuite.h>
#include <RbtTrie/RbtTrieNode.hpp>

class RbtTrieNodeTest : public CxxTest::TestSuite
{
public:
	void x(void)
	{
		char k[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+=";
		RbtTrieNode<char, int> tn(k, 65, NULL);
	}
	void testNodeSuffixLengthLimit(void)
	{
		TS_ASSERT_THROWS(x(), std::out_of_range);
	}

	void testKeySuffixMatchTrue(void)
	{
		char k = 'A';
		RbtTrieNode<char, int> tn(&k, 1, (int *)NULL);
		TS_ASSERT(tn.matchesKeySuffix(&k, 1));
	}

	void testKeySuffixMatchMismatch(void)
	{
		char k = 'A';
		RbtTrieNode<char, int> tn(&k, 1, (int *)NULL);
		k = 'Q';
		TS_ASSERT(!tn.matchesKeySuffix(&k, 1));
	}

	void testKeySuffixMatchDifferentLength(void)
	{
		char k = 'A';
		RbtTrieNode<char, int> tn(&k, 1, (int *)NULL);
		char j[] = "ABC";
		TS_ASSERT(!tn.matchesKeySuffix(j, 3));
	}
};
