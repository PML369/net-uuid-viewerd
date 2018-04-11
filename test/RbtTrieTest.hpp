/*
 * Test suite for RbtTrie class
 *
 * Copyright (c) Peter Lotts 2018
 *
 */

#include <cxxtest/TestSuite.h>
#include <RbtTrie/RbtTrie.hpp>

#define LONGKEY "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+="

class RbtTrieTest : public CxxTest::TestSuite
{
public:
	void testInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 4;
		char k[] = "A";
		TS_ASSERT(trie.insert(k, 1, &x));
	}

	void testLongInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 4;
		char k[] = LONGKEY;
		TS_ASSERT(trie.insert(k, 65, &x));
	}

	void testDuplicateInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 4;
		char k[] = "A";
		TS_ASSERT( trie.insert(k, 1, &x));
		TS_ASSERT(!trie.insert(k, 1, &x));

		RbtTrie<char, int> trie2;
		char k2[] = LONGKEY;
		TS_ASSERT( trie2.insert(k2, 65, &x));
		TS_ASSERT(!trie2.insert(k2, 65, &x));
	}

	void testSharedPrefixLeafInsertion(void)
	{
		RbtTrie<char, int> trie;
		int x = 8;
		char k1[] = "The quick brown fox";
		char k2[] = "The quick brown dog";
		TS_ASSERT(trie.insert(k1, 19, &x));
		TS_ASSERT(trie.insert(k2, 19, &x));

		TS_ASSERT(trie.get(k1, 19) == &x);
		TS_ASSERT(trie.get(k2, 19) == &x);
	}

	void testPrefixGetAttempt(void)
	{
		RbtTrie<char, int> trie;
		int x = 7;
		char k[] = "ABCD";
		TS_ASSERT(trie.insert(k, 4, &x));
		TS_ASSERT(trie.get(k, 1) == NULL);
		TS_ASSERT(trie.get(k, 2) == NULL);
		TS_ASSERT(trie.get(k, 3) == NULL);
		TS_ASSERT(trie.get(k, 4) == &x);
	}
};
