/*
 * BalancedTreeHolder.h
 *
 *  Created on: 13 Jun 2017
 *      Author: sartofi
 */

#ifndef BALANCEDTREEHOLDER_H_
#define BALANCEDTREEHOLDER_H_


#include "BalancedTreeNodeT.h"
#include "GenericIterator.h"

namespace MARTe{



/**
 * to handle balanced binary trees
 */
template <class loadClass,class keyClass, typename loadKey>
class BalancedTreeHolder{
public:

	/**
	 *
	 */
	BalancedTreeHolder(){
		root = NULL;
	}

	/**
	 *
	 */
	virtual ~BalancedTreeHolder(){
		root = NULL;
	}

	/**
	 * node is supposed to be without childrens - treeSize=1 treeImbalance = 0
	 * insertion position is determined by the natural ordering of the actual object
	 */
	ErrorManagement::ErrorType Insert( loadClass &data){
		ErrorManagement::ErrorType ret;

		BalancedTreeNodeT<loadClass,keyClass, loadKey> *newNode = new BalancedTreeNodeT<loadClass,keyClass, loadKey>(data);

		ret.fatalError = (newNode == NULL);

		if (ret){
			if (root == NULL){
				root = newNode;
			} else {
				BalancedTreeNode *rootL = static_cast<BalancedTreeNode *>(root);
				ret = BalancedTreeNode::InsertAVL(rootL,static_cast<BalancedTreeNode *>(newNode));
				root = static_cast<BalancedTreeNodeT<loadClass,keyClass, loadKey> *>(rootL);
			}
		}
		return ret;
	}

	/**
	 */
	ErrorManagement::ErrorType Delete(const keyClass &key){
		ErrorManagement::ErrorType ret;
		ret.illegalOperation = (root == NULL);
		if (ret){
			BalancedTreeNode *rootG = static_cast<BalancedTreeNode *>(root);
			BalancedTreeNode *extracted;
			BalancedTreeNodeKey K = loadKey::ToNodeKey(key);
			ret = ExtractAVL(root, extracted,K);
			root = static_cast<BalancedTreeNodeT<loadClass,f> *>(rootG);
			if (extracted != NULL){
				delete extracted;
			}
		}
		return ret;
	}

	/**
	 *
	 */
	inline ErrorManagement::ErrorType Iterate(GenericIterator<loadClass> &iterator){
		ErrorManagement::ErrorType ret;
		IteratorAction ia;
		ret.illegalOperation = (root == NULL);
		if (ret){
			ia = root->Iterate(iterator,0);
			if (!ia.error){
				if (ia.deleteNode){
					BalancedTreeNode *extracted;
					BalancedTreeNodeKey dummy;
					BalancedTreeNode *rootG = root;
					if (BalancedTreeNode::ExtractAVL(rootG, extracted,dummy,0)){
						root = reinterpret_cast<BalancedTreeNodeT<loadClass,keyClass, loadKey>*>(rootG);
						delete extracted;
					}
				}
			} else {
				ret.fatalError = true;
			}
		}
		return ret;

	}

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	loadClass *operator[](const uint32 index) const{
		loadClass *p = NULL;
		if (root != NULL){
			BalancedTreeNodeT<loadClass,keyClass, loadKey> *pn = root->Seek(index);
			if (pn != NULL){
				p = &pn->GetData();
			}
		}
		return p;
    }

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	BalancedTreeNodeT<loadClass,keyClass, loadKey> *Seek(const uint32 index) const{
		BalancedTreeNodeT<loadClass,keyClass, loadKey>  *p = NULL;
		if (root != NULL){
			p = root->Seek(index);
		}
		return p;
	}

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	loadClass *operator[](const keyClass &key) const{
		uint32 index;
		loadClass *p = NULL;
		if (root != NULL){
			BalancedTreeNodeKey K = loadKey::ToNodeKey(key);
			BalancedTreeNodeT<loadClass,keyClass, loadKey> *pn = root->Search(key,index);
			if (pn != NULL){
				p = &pn->GetData();
			}
		}
		return p;
    }

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    uint32 Size() const{
    	uint32 size = 0;
		if (root != NULL){
			size = root->Size();
		}
		return size;
    };

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    uint32 Depth() const{
    	uint32 size = 0;
		if (root != NULL){
			size = root->Depth();
		}
		return size;
    };



private:
	/**
	 * the root of the tree
	 */
	BalancedTreeNodeT<loadClass,keyClass, loadKey> *root;

};


}


#endif /* BALANCEDTREEHOLDER_H_ */
