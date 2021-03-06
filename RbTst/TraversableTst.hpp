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
#include <RbTst/SearchableTst.hpp>

#ifndef TRAVERSABLE_TST_HPP
#define TRAVERSABLE_TST_HPP

template <typename KE, typename V>
class TraversableTst : public SearchableTst<KE, V> {
	friend class RbTstInternalTest;
protected:
	typedef typename SearchableTst<KE, V>::pNode pNode;
public:
	typedef typename SearchableTst<KE, V>::tKey tKey;

	template <typename OutputIterator>
	void getKeysWithPrefix(const KE prefix[], unsigned int length,
			OutputIterator out)
	{
		getDataWithKeyPrefix<tKey>(prefix, length, out,
					[](tKey key, V *val) { return key; });
	}

	template <typename OutputIterator>
	void getValuesWithKeyPrefix(const KE prefix[], unsigned int length,
			OutputIterator out)
	{
		getDataWithKeyPrefix<V *>(prefix, length, out,
					[](tKey key, V *val) { return val; });
	}

	template <typename value_type, typename OutputIterator>
	void getDataWithKeyPrefix(const KE prefix[], unsigned int length,
			OutputIterator out, value_type(*convert)(tKey, V *))
	{
		getDataWithKeyPrefix(prefix, length, out, convert,
				[](tKey l, V *v) { return true; });
	}

	template <typename value_type, typename OutputIterator>
	void getDataWithKeyPrefix(const KE prefix[], unsigned int length,
			OutputIterator out, value_type(*convert)(tKey, V *),
			bool(*predicate)(tKey, V *))
	{
		unsigned int matchLength = length;
		pNode match;
		if (!this->findClosestMatch(prefix,length, &match,&matchLength))
			return;

		auto params = std::make_pair(
					std::make_pair(predicate, convert),
					out);

		if (match->isLeaf())
		{
			unsigned int suffixLength = 0;
			KE *suffix = match->getSuffixCopy(&suffixLength);
			unsigned int keyLen = length + matchLength;
			KE *key = new KE[keyLen];
			memcpy(key, prefix, length);
			memcpy(&key[length], &suffix[suffixLength -matchLength],
						matchLength);

			getDataWithKeyPrefixTraverseAction(&params,
					tKey(key, keyLen),
					match->getPayload());
			delete[] suffix;
			return;
		}

		inOrderTraverse(match, prefix, length,
			&TraversableTst<KE, V>::
				getDataWithKeyPrefixTraverseAction, &params);
	}

protected:
	template <typename T>
	void inOrderTraverse(pNode start, const KE prefix[],
			unsigned int prefixLength,
			void (*action)(T *, tKey, V *), T *obj)
	{
		unsigned int prefixAlloc = 2 * prefixLength;
		if (prefixAlloc == 0)
			prefixAlloc = 2;
		KE *newPrefix = new KE[prefixAlloc];
		memcpy(newPrefix, prefix, prefixLength * sizeof(KE));
		inOrderTraverseInternal(start, newPrefix, prefixLength,
						prefixAlloc, action, obj);
	}

	template <typename T>
	void inOrderTraverseInternal(pNode start, KE prefix[],
			unsigned int prefixLength, unsigned int prefixAlloc,
			void (*action)(T *, tKey, V *), T *obj)
	{
		if (start == NULL)
			return;

		if (start->isLeaf())
		{
			unsigned int suffixLength = 0;
			KE *suffix = start->getSuffixCopy(&suffixLength);
			KE *key = new KE[prefixLength + suffixLength];
			memcpy(key, prefix, prefixLength);
			memcpy(&key[prefixLength], suffix, suffixLength);
			action(obj,
					tKey(key, prefixLength + suffixLength),
					start->getPayload());
			delete[] suffix;
			return;
		}

		pNode n = start;
		pNode prev = NULL;
		while (true)
		{
			bool pathFailedTryNext = false;

			if (prev == NULL || prev == n->getParent())
			{
				if (n->getLeft() != NULL)
				{
					// go left
					prev = n;
					n = n->getLeft();
					continue;
				}
				pathFailedTryNext = true;
			}

			if (pathFailedTryNext || prev == n->getLeft())
			{
				if (n->getChild() != NULL)
				{
					// recursive call on child
					if (prefixLength < prefixAlloc)
					{
						prefix[prefixLength] =
							n->getKeyEntry();
						inOrderTraverseInternal(
							n->getChild(),
							prefix,
							prefixLength + 1,
							prefixAlloc,
							action, obj);
					}
					else
					{
						KE *newPrefix = new
							KE[prefixAlloc * 2];
						memcpy(newPrefix, prefix,
								prefixLength);
						newPrefix[prefixLength] =
							n->getKeyEntry();
						inOrderTraverseInternal(
							n->getChild(),
							newPrefix,
							prefixLength + 1,
							prefixAlloc * 2,
							action, obj);
					}
				}

				if (n->getRight() != NULL)
				{
					// go right
					prev = n;
					n = n->getRight();
					continue;
				}
				pathFailedTryNext = true;
			}

			if (pathFailedTryNext || prev == n->getRight())
			{
				if (n == start)
					break;
				prev = n;
				n = n->getParent();
			}
		}
	}

	template <typename OutputPair>
	static void getDataWithKeyPrefixTraverseAction(OutputPair *out,
			tKey key, V *value)
	{
		if (out->first.first(key, value))
		{
			*(out->second) = (out->first.second)(key, value);
			(out->second)++;
		}
	}
};

#endif /* TRAVERSABLE_TST_HPP */
