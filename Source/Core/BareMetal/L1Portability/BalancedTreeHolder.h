/*
 * BalancedTreeHolder.h
 *
 *  Created on: 13 Jun 2017
 *      Author: sartofi
 */

#ifndef BALANCEDTREEHOLDER_H_
#define BALANCEDTREEHOLDER_H_


#include "BalancedTreeNode.h"

namespace MARTe{


/**
 *
 */
template <class T,typename F>
class BalancedTreeNodeT : public BalancedTreeNode{
public:
	/**
	 *
	 */
	BalancedTreeNodeT(const T &payLoadIn){
		payLoad = payLoadIn;
	}

	/**
	 *
	 */
	virtual ~BalancedTreeNodeT(){
	}

	/**
	 * navigates tree smaller to greater
	 * returns ptr to node
	 */
	inline BalancedTreeNodeT<T,F> *Seek(const uint32 index){
		return  static_cast<BalancedTreeNodeT<T,F> *>(BalancedTreeNode::Seek(index));
	}

    /**
     * @brief Searches a specified element in this list.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	inline BalancedTreeNodeT<T,F> *Search(const BalancedTreeNodeKey &key,uint32 &index){
		return static_cast<BalancedTreeNodeT<T,F> *>(BalancedTreeNode::Search(key,index) );
	}

    /**
     * @brief Searches the specified key and moves left-right on the basis of the specified offset.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	inline BalancedTreeNodeT<T,F> *RelativeSeek(const BalancedTreeNodeKey &key,int32 offset){
		return static_cast<BalancedTreeNodeT<T,F> *>(BalancedTreeNode::RelativeSeek(key,offset) );
	}

	T & GetData(){
		return payLoad;
	}
private:

	/**
	 *
	 */
	virtual const BalancedTreeNodeKey GetKey(){
		F f;
		return f(payLoad);
	}

	/**
	 *
	 */
	T payLoad;


};

/**
 * to handle balanced binary trees
 */
template <class T,typename F>
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
	ErrorManagement::ErrorType Insert( T &data){
		ErrorManagement::ErrorType ret;
		F f;
		CCString key = f(data);
		BalancedTreeNodeT<T,F> *newNode = new BalancedTreeNodeT<T,F>(data);

		ret.fatalError = (newNode == NULL);

		if (ret){
			if (root == NULL){
				root = newNode;
			} else {
				BalancedTreeNode *rootL = static_cast<BalancedTreeNode *>(root);
				ret = BalancedTreeNode::InsertAVL(rootL,static_cast<BalancedTreeNode *>(newNode));
				root = static_cast<BalancedTreeNodeT<T,F> *>(rootL);
			}
		}
		return ret;
	}

	/**
	 */
	ErrorManagement::ErrorType Delete(const BalancedTreeNodeKey &key){
		ErrorManagement::ErrorType ret;
		ret.illegalOperation = (root == NULL);
		if (ret){
			BalancedTreeNode *rootG = static_cast<BalancedTreeNode *>(root);
			BalancedTreeNode *extracted;
			ret = ExtractAVL(root, extracted,key);
			root = static_cast<BalancedTreeNodeT<T,f> *>(rootG);
			if (extracted != NULL){
				delete extracted;
			}
		}
		return ret;
	}

    /**
     * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
	T *operator[](const uint32 index) const{
		T *p = NULL;
		if (root != NULL){
			BalancedTreeNodeT<T,F> *pn = root->Seek(index);
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
	BalancedTreeNodeT<T,F> *Seek(const uint32 index) const{
		BalancedTreeNodeT<T,F>  *p = NULL;
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
	T *operator[](const BalancedTreeNodeKey &key) const{
		uint32 index;
		T *p = NULL;
		if (root != NULL){
			BalancedTreeNodeT<T,F> *pn = root->Search(key,index);
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
	BalancedTreeNodeT<T,F> *root;

};






}


#endif /* BALANCEDTREEHOLDER_H_ */
