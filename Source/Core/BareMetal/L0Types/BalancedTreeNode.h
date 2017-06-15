/*
 * BalancedTreeNode.h
 *
 *  Created on: 13 Jun 2017
 *      Author: sartofi
 */

#ifndef BALANCEDTREENODE_H_
#define BALANCEDTREENODE_H_


#include "ErrorType.h"



namespace MARTe {



typedef const void * BalancedTreeNodeKey;

/**
 * BalancedTreeNode-able
 *
 */
class BalancedTreeNode{
public:

	/**
	 * default constructor
	 */
	inline BalancedTreeNode();

	/**
	 * destructor virtual for proper derived class destruction
	 */
	virtual ~BalancedTreeNode();


	/**
	 * inserts node into natural ordered position
	 * rebalancing as AVL
	 */
	static ErrorManagement::ErrorType InsertAVL(BalancedTreeNode *&root, BalancedTreeNode * const node);
	/**
	 * inserts node into natural ordered position
	 * rebalancing as AVL
	 * bias = 0 -> extract matching
	 * bias = 1 -> extract the smallest greater
	 * bias = -1 -> extract the greatest smaller
	 */
	static ErrorManagement::ErrorType ExtractAVL(BalancedTreeNode *&root, BalancedTreeNode *&extracted,const BalancedTreeNodeKey &key,int8 bias = 0);

	/**
	 * navigates tree smaller to greater
	 * returns ptr to node
	 */
	BalancedTreeNode *Seek(const uint32 index) ;

    /**
     * @brief Searches a specified element in this list.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	BalancedTreeNode *Search(const BalancedTreeNodeKey &key,uint32 &index);
    /**
     * @brief Searches the specified key and moves left-right on the basis of the specified offset.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	BalancedTreeNode *RelativeSeek(const BalancedTreeNodeKey &key,int32 offset);

	/*
	 * left to right tree imbalance
	 * positive if greater tree is bigger
	 */
	inline int32 TreeSizeImbalance()const;

	/*
	 * left to right tree depth imbalance
	 * positive if greater tree is taller
	 */
	inline int32 TreeDepthImbalance()const;

	/**
	 * number of elements in this subtree
	 */
	inline uint32 Size() const;

	/**
	 * number of elements in this subtree
	 */
	inline uint32 Depth() const;

private:
	/**
	 * to be subclassed and specialised
	 */
	virtual int8 CompareToKey(BalancedTreeNodeKey K)=0;

	/**
	 * to be subclassed and specialised
	 */
	virtual int8 CompareToNode(BalancedTreeNode *N)=0;

	/**
	 * switch the root node with the greater subtree
	 * deals with the G-S case
	 */
	static ErrorManagement::ErrorType RotateG(BalancedTreeNode *&father);
	/**
	 * switch the root node with the greater subtree
	 */
	static ErrorManagement::ErrorType RotateS(BalancedTreeNode *&father);

	/**
	 * sets as leaf node.
	 */
	inline void  Clean();
	/**
	 * size of the "greater" subtree
	 */
	inline uint32 GreaterSize() const;

	/**
	 * size of the "smaller" subtree
	 */
	inline uint32 SmallerSize() const;

	/**
	 * size of the "greater" subtree
	 */
	inline uint32 GreaterDepth() const;

	/**
	 * size of the "smaller" subtree
	 */
	inline uint32 SmallerDepth() const;

	/**
	 *
	 */
	inline void UpdateStatistics();



    // max of (greater->treeDepth, smaller->treeDepth)
	uint32 treeDepth;
	// size of subtree  (sum of greter->treeDepth, smaller->treeDepth)
	uint32 treeSize;
	// greater subtree
	BalancedTreeNode *greater;
	// samller subtree
	BalancedTreeNode *smaller;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


/**
 * default constructor
 */
BalancedTreeNode::BalancedTreeNode(){
	Clean();
}


/*
 * left to right tree imbalance
 * positive if greater tree is bigger
 */
int32 BalancedTreeNode::TreeSizeImbalance()const{
	return GreaterSize() - SmallerSize();
}

/*
 * left to right tree depth imbalance
 * positive if greater tree is taller
 */
int32 BalancedTreeNode::TreeDepthImbalance()const{
	return GreaterDepth() - SmallerDepth();
}

/**
 * number of elements in this subtree
 */
uint32 BalancedTreeNode::Size() const{
	return treeSize;
}

/**
 * number of elements in this subtree
 */
uint32 BalancedTreeNode::Depth() const{
	return treeDepth;
}

/**
 * sets as leaf node.
 */
void  BalancedTreeNode::Clean(){
	treeSize = 1;
	treeDepth = 1;
	greater = NULL;
	smaller = NULL;
}

/**
 * size of the "greater" subtree
 */
uint32 BalancedTreeNode::GreaterSize() const{
	uint32 size = 0;
	if (greater != NULL){
		size = greater->treeSize;
	}
	return size;
}

/**
 * size of the "smaller" subtree
 */
uint32 BalancedTreeNode::SmallerSize() const{
	uint32 size = 0;
	if (smaller != NULL){
		size = smaller->treeSize;
	}
	return size;
}

/**
 * size of the "greater" subtree
 */
uint32 BalancedTreeNode::GreaterDepth() const{
	uint32 depth = 0;
	if (greater != NULL){
		depth = greater->treeDepth;
	}
	return depth;
}

/**
 * size of the "smaller" subtree
 */
uint32 BalancedTreeNode::SmallerDepth() const{
	uint32 depth = 0;
	if (smaller != NULL){
		depth = smaller->treeDepth;
	}
	return depth;
}

/**
 *
 */
void BalancedTreeNode::UpdateStatistics(){
	treeSize  = 1 + SmallerSize()+GreaterSize();
	treeDepth = 1 + max(SmallerDepth(),GreaterDepth());
}






}



#endif /* BALANCEDTREENODE_H_ */
