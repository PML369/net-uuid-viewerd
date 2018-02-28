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
	void x()
	{
		char k[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./+=";
		RbtTrieNode<char, int> tn(k, 65, NULL);
	}
	void testNodeSuffixLengthLimit(void)
	{
		TS_ASSERT_THROWS(x(), std::out_of_range);
	}
};
