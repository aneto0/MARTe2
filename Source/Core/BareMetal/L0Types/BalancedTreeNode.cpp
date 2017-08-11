/*
 * BalancedTreeNode.cpp
 *
 *  Created on: 28 May 2017
 *      Author: sartofi
 */


#include "BalancedTreeNode.h"

namespace MARTe{


/*---------------------------------------------------------------------------*/
/*                        Method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * destructor virtual for proper derived class destruction
 */
BalancedTreeNode::~BalancedTreeNode(){
	Clean();
}

/**
 * inserts node into natural ordered position
 * rebalancing as AVL
 */
//#include <stdio.h>
ErrorManagement::ErrorType BalancedTreeNode::InsertAVL(BalancedTreeNode *&root, BalancedTreeNode * const node){
	ErrorManagement::ErrorType ret;

	ret.parametersError = ((node == NULL) || (root == NULL));

	int8 sign = 0;
	if (ret){
		node->Clean();

		sign = root->CompareToNode(node);
//		sign = root->Compare(node->GetKey());

		// TODO choose better error code
		ret.fatalError =  (sign == 0) || (sign > 2);
	}

	if (ret){
		// greater
		if (sign == 1){
			if (root->greater == NULL){
				root->greater = node;
			} else {
				ret = InsertAVL(root->greater,node);
			}
		} else {
			if (root->smaller == NULL){
				root->smaller = node;
			} else {
				ret = InsertAVL(root->smaller,node);
			}
		}
	}

	// AVL rebalance
	if (ret){
		root->UpdateStatistics();

		int32 di = root->TreeDepthImbalance();
		if (di > 1){
			ret = RotateG(root);
		} else
		if (di < -1){
			ret = RotateS(root);
		}
	}

	return ret;
}


/**

 * bias = 2 -> extract matching
 * bias = 1 -> extract the smallest greater
 * bias = -1 -> extract the greatest smaller
 * bias = 0 -> extract root!
 */
ErrorManagement::ErrorType BalancedTreeNode::ExtractAVL(BalancedTreeNode *&root, BalancedTreeNode *&extracted,const BalancedTreeNodeKey &key,int8 bias){
	ErrorManagement::ErrorType ret;

	ret.parametersError = (root == NULL);

	int8 sign = 0;
	if (ret){

		if (bias == 2){
			sign = root->CompareToKey(key);
//			sign = root->Compare(key);
		} else {
			sign = bias;

		}

		// found
		if (sign == 0){
			extracted = root;
			if (root->greater == NULL){
				root = root->smaller;
			} else
			if (root->smaller == NULL){
				root = root->greater;
			} else {
				ret = ExtractAVL(extracted->greater,root,key,1);
				if (ret && (root != NULL)){
					root->greater = extracted->greater;
					root->smaller = extracted->smaller;
				}
			}
		} else
		if (sign > 0){
			if (root->smaller == NULL){
				if (bias == 2){
					extracted = NULL;
					ret.notCompleted = true;
				} else {
					extracted = root;
					root = root->greater;
				}
			} else {
				ret = ExtractAVL(root->smaller,extracted,key,bias);
			}
		} else {
			if (root->greater == NULL){
				if (bias == 2){
					extracted = NULL;
					ret.notCompleted = true;
				} else {
					extracted = root;
					root = root->smaller;
				}
			} else {
				ret = ExtractAVL(root->greater,extracted,key,bias);
			}
		}
	}

	// AVL rebalance
	if (ret && (root != NULL)){
		root->UpdateStatistics();
		int32 di = root->TreeDepthImbalance();
		if (di > 1){
			ret = RotateG(root);
		} else
		if (di < -1){
			ret = RotateS(root);
		}
	}

	return ret;
}


/**
 * navigates tree smaller to greater
 * returns ptr to node
 */
BalancedTreeNode *BalancedTreeNode::Seek(const uint32 index) {
	BalancedTreeNode *ret = NULL;
	if (index < treeSize){
		uint32 smallerSize = SmallerSize();
		if (index < smallerSize){
			// smallerSize > 0 implies smaller !NULL
			ret = smaller->Seek(index);
		}
		if (index == smallerSize){
			ret = this;
		} else {
			if ((index > smallerSize) && (greater != NULL)){
				ret = greater->Seek(index-smallerSize-1);
			}
		}
	}
	return ret;
}

/**
 * @brief Searches a specified element in this list.
 * @param[in] key a reference to the key to search.
 * @return pointer to the element.
 */
BalancedTreeNode *BalancedTreeNode::Search(const BalancedTreeNodeKey &key,uint32 &index){
	BalancedTreeNode *ptr = NULL;
	int8 compare = CompareToKey(key);
	if (compare == 0) {
		ptr = this;
		index = this->SmallerSize();
	} else
	if ((compare == 1) && (greater != NULL)){
		ptr = greater->Search(key,index);
		index += (1 + this->SmallerSize());
	} else
	if ((compare == -1) && (smaller != NULL)){
		ptr = smaller->Search(key,index);
	}
	return ptr;
}

/**
 * @brief Searches the specified key and moves left-right on the basis of the specified offset.
 * @param[in] key a reference to the key to search.
 * @return pointer to the element.
 */
BalancedTreeNode *BalancedTreeNode::RelativeSeek(const BalancedTreeNodeKey &key,int32 offset){
	uint32 index;
	BalancedTreeNode *node = Search(key,index);

	if (node != NULL){
		int32 localIndex = node->SmallerSize() + offset;
		if (localIndex >= 0){
			node = node->Seek(localIndex);
		} else {
			node = NULL;
		}

		// out of local subtree range
		if (node == NULL){
			int32 sIndex = index;
			sIndex += offset;
			if (sIndex >= 0){
				node = Seek(sIndex);
			} else {
				node = NULL;
			}
		}
	}

	return node;
}



/**
 * switch the root node with the greater subtree
 * deals with the G-S case
 */
ErrorManagement::ErrorType BalancedTreeNode::RotateG(BalancedTreeNode *&father){
	ErrorManagement::ErrorType ret;
	BalancedTreeNode *greater = NULL;
	ret.parametersError = (father == NULL);
	if (ret){
		greater = father->greater;
		ret.fatalError = (greater == NULL);
	}
	if (ret){
		// detect case where the greater subtree has larger smaller subtree!
		if (greater->TreeDepthImbalance()< 0){
			BalancedTreeNode *greaterSmaller = greater->smaller;
			ret.fatalError = (greaterSmaller == NULL);
			if (ret){
				BalancedTreeNode *exFather = father;
				father = greaterSmaller;
				exFather->greater = father->smaller;
				greater->smaller = father->greater;
				father->smaller = exFather;
				father->greater = greater;

				exFather->UpdateStatistics();
				greater->UpdateStatistics();
				father->UpdateStatistics();
			}

		} else {
			BalancedTreeNode *exFather = father;
			father = greater;
			exFather->greater = father->smaller;
			father->smaller = exFather;
			exFather->UpdateStatistics();
			father->UpdateStatistics();
		}


	}
	return ret;
}

/**
 * switch the root node with the greater subtree
 */
ErrorManagement::ErrorType BalancedTreeNode::RotateS(BalancedTreeNode *&father){
	ErrorManagement::ErrorType ret;
	BalancedTreeNode *smaller = NULL;
	ret.parametersError = (father == NULL);
	if (ret){
		smaller = father->smaller;
		ret.fatalError = (smaller == NULL);
	}
	if (ret){
		// detect case where the greater subtree has larger smaller subtree!
		if (smaller->TreeDepthImbalance() > 0){
			BalancedTreeNode *smallerGreater = smaller->greater;
			ret.fatalError = (smallerGreater == NULL);
			if (ret){
				BalancedTreeNode *exFather = father;
				father = smallerGreater;
				exFather->smaller = father->greater;
				smaller->greater = father->smaller;
				father->greater = exFather;
				father->smaller = smaller;

				exFather->UpdateStatistics();
				smaller->UpdateStatistics();
				father->UpdateStatistics();
			}
		} else {
			BalancedTreeNode *exFather = father;
			father = smaller;
			exFather->smaller = father->greater;
			father->greater = exFather;
			exFather->UpdateStatistics();
			father->UpdateStatistics();
		}
	}
	return ret;
}

#if 0
/**
 * subtree to list
 */
static ErrorManagement::ErrorType ToListS(BalancedTreeNode *&root,BalancedTreeNode *appendix = NULL){
	ErrorManagement::ErrorType ret;
	BalancedTreeNode *saveRoot = root;
	ret.parametersError = (saveRoot == NULL);
	if (ret){
		if (saveRoot->smaller != NULL){
			root = saveRoot->smaller;
			saveRoot->smaller = NULL;
			ret = ToListG(root,appendix);
			appendix = root;
		}
	}
	if (ret){
		saveRoot->smaller= appendix;
		saveRoot->UpdateStatistics();
		root = saveRoot;

		if (saveRoot->greater != NULL){
			root = saveRoot->greater;
			saveRoot->greater = NULL;
			ret = ToListG(root,saveRoot);
		}
	}
	return ret;
}

/**
 * subtree to list
 * From list made of smaller pointers to balanced tree
 */
static ErrorManagement::ErrorType ToTreeS(BalancedTreeNode *&root){
	ErrorManagement::ErrorType ret;
	ret.parametersError = (root == NULL);

	if (ret){

		uint32 size = 1;

		BalancedTreeNode *&rootT = root;
		while ((rootT != NULL)&&(size <= rootT->SmallerSize())){
			RotateS(rootT);
			rootT = rootT->smaller;
			size *=2;
		}


		uint32 size = root->treeSize;

		//perform rotations!


	}

}
#endif





}
