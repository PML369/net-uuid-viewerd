/*
 * Copyright (C) Peter Lotts 2018
 *
 * A template implementation of a Red-Black Ternary Trie,
 * using templated arrays as the keys
 * (the template type must support < comparison)
 *
 * The data structure may be thought of as a trie when in use,
 * but internally it uses the space-compressed representation
 * of a ternary tree, with each level of the ternary tree kept
 * balanced by making that tree a red-black tree.
 */

#include <RbtTrie/RbtTrieNode.hpp>
#include <RbtTrie/TreeTrie.hpp>

#ifndef RBT_TRIE_HPP
#define RBT_TRIE_HPP

template <typename KE, typename V>
class RbtTrie : public TreeTrie<KE, V> {
	friend class RbtTrieInternalTest;
protected:
	typedef typename TreeTrie<KE, V>::pNode pNode;

public:
	bool insert(const KE key[], unsigned int length, V *val) {
		pNode branch = this->insertImpl(key, length, val);
		if (branch == NULL)
			return false;

		fixupRedBlackInvariants(branch);
		return true;
	}

protected:
	void fixupRedBlackInvariants(pNode newNode)
	{
		// On insert, assume node is red for now
		newNode->setRed(true);
		pNode p, n = newNode;

		while (n->isRed())
		{
			if ((p = n->getParent()) == NULL || p->getChild() == n)
			{
				// This node is the head of its RB tree,
				// so it is black
				n->setRed(false);
				return;
			}

			// Rule out simple case where no further work is needed
			if (!p->isRed())
				return;
			pNode pp = p->getParent();

			// Bad case 1: Red promotion
			// 	The parent and uncle are both red
			// 	(with the grandparent necessarily black).
			// Either     B	    or    B     or    B     or    B
			//           / \         / \         / \         / \
			//          R   R       R   R       R   R       R   R
			//         / \	           / \         / \     / \
			//        R    	              R       R           R
			if (!isBlack(pp->getLeft()) && !isBlack(pp->getRight()))
			{
				pp->getLeft()->setRed(false);
				pp->getRight()->setRed(false);
				pp->setRed(true);
				newNode = pp;
				continue;
			}

			// Bad case 2: Rotations into case 3
			// Either     B	    or    B
			//           / \         / \
			//     (a)  R   B   (b) B   R
			//         / \	           / \
			//            R	          R
			if (isOnLeft(p) && isOnRight(n)) // Case 2a
			{
				n = p;
				rotateLeft(n);
				p = n->getParent();
			}
			else if (isOnRight(p) && isOnLeft(n)) // Case 2b
			{
				n = p;
				rotateRight(n);
				p = n->getParent();
			}

			// Bad case 3: Rotations
			// Either     B	    or    B
			//           / \         / \
			//     (a)  R   B   (b) B   R
			//         / \	           / \
			//        R    	              R
			if (isOnLeft(p) && isOnLeft(n)) // Case 3a
			{
				p->setRed(false);
				pp->setRed(true);
				rotateRight(pp);
				n = p;
			}
			else if (isOnRight(p) && isOnRight(n)) // Case 3b
			{
				p->setRed(false);
				pp->setRed(true);
				rotateLeft(pp);
				n = p;
			}
		}
	}
	

	bool hasOneLRChild(pNode n)
	{
		return ((n->getLeft() == NULL && n->getRight() != NULL)
		     || (n->getLeft() != NULL && n->getRight() == NULL));
	}
	bool isOnLeft(pNode child)
	{
		if (child == NULL || child->getParent() == NULL)
			return false;
		return child->getParent()->getLeft() == child;
	}
	bool isOnRight(pNode child)
	{
		if (child == NULL || child->getParent() == NULL)
			return false;
		return child->getParent()->getRight() == child;
	}
	bool isBlack(pNode n)
	{
		return n == NULL || !n->isRed();
	}
	void replaceNode(pNode nOld, pNode nNew)
	{
		pNode p = nOld->getParent();
		if (p == NULL)
		{
			if (nOld == this->head)
			{
				this->head = nNew;
				nNew->clearParent();
			}
			return;
		}

		if (nOld == p->getLeft())
			p->setLeft(nNew);
		if (nOld == p->getChild())
			p->setChild(nNew);
		if (nOld == p->getRight())
			p->setRight(nNew);
	}
	void rotateLeft(pNode x) // Idential formulation to CLRS
	{
		if (x == NULL) return;
		pNode y = x->getRight();
		if (y == NULL) return;

		pNode beta = y->getLeft();
		if (beta != NULL)
			x->setRight(beta);
		else
			x->clearRight();

		replaceNode(x, y);
		y->setLeft(x);
	}
	void rotateRight(pNode y) // Idential formulation to CLRS
	{
		if (y == NULL) return;
		pNode x = y->getLeft();
		if (x == NULL) return;

		pNode beta = x->getRight();
		if (beta != NULL)
			y->setLeft(beta);
		else
			y->clearLeft();

		replaceNode(y, x);
		x->setRight(y);
	}
};

#endif /* RBT_TRIE_HPP */
