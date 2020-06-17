/*
 * BalancedTreeHolder.h
 *
 *  Created on: 13 Jun 2017
 *      Author: sartofi
 */

#ifndef BALANCEDTREEHOLDER_H_
#define BALANCEDTREEHOLDER_H_

#include "GenericIterator.h"
#include "BalancedTreeNodeT.h"


namespace MARTe{


/**
 * @brief to handle balanced binary trees
 * @tparam loadClass the payload of the list.
 * @tparam keyClass an index type for the list
 * @tparam loadkey namespace that provides ToNodeKey,Compare and CompareToKey functions
 *
 */
template <class loadClass,class keyClass, typename loadKey>
class BalancedTreeHolder{
public:

	/**
	 *
	 */
	inline BalancedTreeHolder();

	/**
	 *
	 */
	inline virtual ~BalancedTreeHolder();

	/**
	 * node is supposed to be without childrens - treeSize=1 treeImbalance = 0
	 * insertion position is determined by the natural ordering of the actual object
	 */
	inline ErrorManagement::ErrorType Insert( loadClass &data);

	/**
	 */
	inline ErrorManagement::ErrorType Delete(const keyClass &key);

	/**
	 *  supports only deleteNode and errors
	 *
	 */
	inline ErrorManagement::ErrorType Iterate(GenericIterator<loadClass> &iterator);

	/**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	inline loadClass *operator[](const uint32 index) const;

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	inline BalancedTreeNodeT<loadClass,keyClass, loadKey> *Seek(const uint32 index) const;

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	inline loadClass *operator[](const keyClass &key) const;

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    inline uint32 Size() const;

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    inline uint32 Depth() const;



private:
	/**
	 * the root of the tree
	 */
	BalancedTreeNodeT<loadClass,keyClass, loadKey> *root;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class loadClass,class keyClass, typename loadKey>
inline BalancedTreeHolder<loadClass,keyClass,loadKey>::BalancedTreeHolder(){
	root = NULL;
}

/**
 *
 */
template<class loadClass,class keyClass, typename loadKey>
inline  BalancedTreeHolder<loadClass,keyClass,loadKey>::~BalancedTreeHolder(){
	root = NULL;
}

template<class loadClass,class keyClass, typename loadKey>
inline ErrorManagement::ErrorType BalancedTreeHolder<loadClass,keyClass,loadKey>::Iterate(GenericIterator<loadClass> &iterator){
	ErrorManagement::ErrorType ret;
	IteratorAction ia;
	ret.illegalOperation = (root == NULL);
	if (ret){
		ia = root->Iterate(iterator,0);
		if (ia){
			switch (ia.ActionCode()){
			case noAction:{

			}break;
			case deleteNode:{
				BalancedTreeNode *extracted;
				BalancedTreeNodeKey dummy;
				BalancedTreeNode *rootG = root;
				if (BalancedTreeNode::ExtractAVL(rootG, extracted,dummy,0)){
					root = static_cast<BalancedTreeNodeT<loadClass,keyClass, loadKey>*>(rootG);
//					root = reinterpret_cast<BalancedTreeNodeT<loadClass,keyClass, loadKey>*>(rootG);
					delete extracted;
				}
			} break;
			default:{
				ret.invalidOperation=true;
			}
			}
		} else {
			ret.fatalError = true;
		}
	}
	return ret;
}

template<class loadClass,class keyClass, typename loadKey>
inline ErrorManagement::ErrorType BalancedTreeHolder<loadClass,keyClass,loadKey>::Insert( loadClass &data){
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

template<class loadClass,class keyClass, typename loadKey>
inline ErrorManagement::ErrorType BalancedTreeHolder<loadClass,keyClass,loadKey>::Delete(const keyClass &key){
	ErrorManagement::ErrorType ret;
	ret.illegalOperation = (root == NULL);
	if (ret){
		BalancedTreeNode *rootG = static_cast<BalancedTreeNode *>(root);
		BalancedTreeNode *extracted;
		BalancedTreeNodeKey K = loadKey::ToNodeKey(key);
		ret = ExtractAVL(root, extracted,K);
		root = static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(rootG);
		if (extracted != NULL){
			delete extracted;
		}
	}
	return ret;
}

template<class loadClass,class keyClass, typename loadKey>
inline loadClass *BalancedTreeHolder<loadClass,keyClass,loadKey>::operator[](const uint32 index) const{
	loadClass *p = NULL;
	if (root != NULL){
		BalancedTreeNodeT<loadClass,keyClass, loadKey> *pn = root->Seek(index);
		if (pn != NULL){
			p = &pn->GetData();
		}
	}
	return p;
}

template<class loadClass,class keyClass, typename loadKey>
inline BalancedTreeNodeT<loadClass,keyClass, loadKey> *BalancedTreeHolder<loadClass,keyClass,loadKey>::Seek(const uint32 index) const{
	BalancedTreeNodeT<loadClass,keyClass, loadKey>  *p = NULL;
	if (root != NULL){
		p = root->Seek(index);
	}
	return p;
}

template<class loadClass,class keyClass, typename loadKey>
inline loadClass *BalancedTreeHolder<loadClass,keyClass,loadKey>::operator[](const keyClass &key) const{
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

template<class loadClass,class keyClass, typename loadKey>
inline uint32 BalancedTreeHolder<loadClass,keyClass,loadKey>::Size() const{
	uint32 size = 0;
	if (root != NULL){
		size = root->Size();
	}
	return size;
};

template<class loadClass,class keyClass, typename loadKey>
inline uint32 BalancedTreeHolder<loadClass,keyClass,loadKey>::Depth() const{
	uint32 size = 0;
	if (root != NULL){
		size = root->Depth();
	}
	return size;
};



}


#endif /* BALANCEDTREEHOLDER_H_ */
