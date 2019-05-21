#include "MultiLevelIndex.h"
#include "ErrorType.h"

namespace MARTe{

template <class payloadType, typename indexType>
struct Record{

	payloadType 	payload;

	indexType 		index[];

	static Record<payloadType, indexType> * Create(uint8 nOfIndexes);

};

template <class payloadType, typename indexType>
Record<payloadType, indexType> * Record<payloadType, indexType>::Create(uint8 nOfIndexes){
	return reinterpret_cast<Record<payloadType, indexType> * >(malloc(sizeof(Record)+sizeof(indexType)*nOfIndexes));
}

/**
 * allows handling variable size records.
 * The payLoadType and indexType must coincide
 * indexNo must be between 0 and the number of indexes in Record
 */
template <class payloadType, class indexerType,typename indexType>
class RecordReference {

public:
	/**
	 * recordIndex = 0
	 */
	RecordReference();

	/**
	 * recordIndex = convertToPtr(start.recordIndex)->index[indexNo]
	 */
	RecordReference(const RecordReference &start, const indexerType &indexer, uint8 indexNo);

	/**
	 * recordIndex = start.recordIndex
	 */
	RecordReference(const RecordReference &start);

	/**
	 * gets an index for this record
	 * sets recordIndex accordingly
	 */
	ErrorManagement::ErrorType Store(const indexerType &indexer,Record *record);

	/**
	 * retrieves payload of current node
	 */
	payloadType *Access(const indexerType &indexer);

	/**
	 * sets the value of an index
	 */
	ErrorManagement::ErrorType SetIndex(const indexerType &indexer,uint8 indexNo,indexType indexValue);

	/**
	 * gets the value of this record index
	 */
	indexType GetLocalIndex() const;
private:
	/**
	 * the index associated with current record
	 */
	indexType recordIndex;

	/**
	 * @brief points to a memory area containing a record
	 * the record is composed of a data pointer and a vector of indexes
	 */
	Record<payloadType,indexType> *GetRecord(const indexerType &indexer) const;

};

template <class payloadType, class indexerType, typename indexType>
class StackIndexer{
private:

	/**
	 * the index of the next operator
	 */
	const uint8 nextIndexNo;

	/**
	 * add to correct position
	 * in case of stack this is the top
	 */
	ErrorManagement::ErrorType Add(
			Record<payloadType,indexType> &dataRecord, indexType dataIndex,
			Record<payloadType,indexType> &rootRecord, indexType rootIndex
	){
		ErrorManagement::ErrorType ret;

		dataRecord.index[nextIndexNo] = rootIndex;

		return ret;
	}
};

template <class payloadType, class indexerType, typename indexType>
class BiDirList{
private:

	/**
	 * the index of the next operator
	 */
	const uint8 nextIndexNo;
	/**
	 * the index of the prev operator
	 */
	const uint8 prevIndexNo;

	/**
	 * add to correct position
	 * in case of stack this is the top
	 */
	ErrorManagement::ErrorType Add(
			Record<payloadType,indexType> &dataRecord, indexType dataIndex,
			Record<payloadType,indexType> &rootRecord, indexType rootIndex
			){
		ErrorManagement::ErrorType ret;

		indexType rootPrevIndex = rootRecord.index[prevIndexNo];
		rootRecord.index[prevIndexNo] = dataIndex;
		dataRecord.index[nextIndexNo] = rootIndex;
		dataRecord.index[prevIndexNo] = rootPrevIndex;

		return ret;
	}

	/**
	 * add to correct position
	 * in case of stack this is the top
	 */
	ErrorManagement::ErrorType Extract(
			Record<payloadType,indexType> &rootRecord, 	indexType rootIndex,
			Record<payloadType,indexType> &newRootRecord, indexType &newRootIndex
			){
		ErrorManagement::ErrorType ret;

		indexType rootPrevIndex = rootRecord.index[prevIndexNo];
		rootRecord.index[prevIndexNo] = dataIndex;
		dataRecord.index[nextIndexNo] = rootIndex;
		dataRecord.index[prevIndexNo] = rootPrevIndex;

		return ret;
	}
};


class DataBase{
public:

	ErrorManagement::ErrorType Add();

private:

	uint8 nOfIndexes;

	DataBaseIndexers indexers[];


};




//IMPLEMENTATION



template <class payloadType, class indexerType, typename indexType>
Record<payloadType,indexType> *RecordReference<payloadType, indexerType,indexType>::GetRecord(const indexerType &indexer) const{
	Record<payloadType,indexType> *p = NULL;
	if (recordIndex != 0){
		p = reinterpret_cast<Record<payloadType,indexType> *>(indexer[recordIndex]);
	}
	return p;
}

template <class payloadType, class indexerType, typename indexType>
ErrorManagement::ErrorType RecordReference<payloadType, indexerType,indexType>::Store(const indexerType &indexer,Record *record){
	ErrorManagement::ErrorType ret;
	indexType index;
	ret = indexer.Store(record,&index);
	return ret;
}

template <class payloadType, class indexerType, typename indexType>
indexType RecordReference<payloadType, indexerType,indexType>::GetLocalIndex() const{
	return recordIndex;
}


template <class payloadType, class indexerType, typename indexType>
RecordReference<payloadType, indexerType,indexType>::RecordReference(){
	recordIndex = 0;
}

template <class payloadType, class indexerType, typename indexType>
RecordReference<payloadType, indexerType,indexType>::RecordReference(const RecordReference<payloadType, indexerType,indexType> &start, const indexerType &indexer, uint8 indexNo){
	recordIndex = 0; // 0 is invalid!

	// recover the real pointer to the record which is encoded start
	Record<payloadType,indexType> *p = start.GetRecord(indexer);

	if (p != NULL){
		recordIndex = p->index[indexNo];
	}
}

template <class payloadType, class indexerType, typename indexType>
RecordReference<payloadType, indexerType,indexType>::RecordReference(const RecordReference &start){
	recordIndex = start.recordIndex;
}

template <class payloadType, class indexerType, typename indexType>
payloadType *RecordReference<payloadType, indexerType,indexType>::Access(const indexerType &indexer){
	payloadType *payload = NULL;
	Record<payloadType,indexType> *p = GetRecord(indexer);
	if (p != NULL){
		payload = p->payload;
	}
	return payload;
}

template <class payloadType, class indexerType, typename indexType>
ErrorManagement::ErrorType RecordReference<payloadType, indexerType,indexType>::SetIndex(const indexerType &indexer,uint8 indexNo,indexType indexValue){
	ErrorManagement::ErrorType ret;
	Record<payloadType,indexType> *p = GetRecord(indexer);
	ret.outOfRange = (p == NULL);
	if (ret){
		p->index[indexNo] = indexValue;

	}
	return ret;
}




int main_(){
	MultiLevelIndex *mli = MultiLevelIndex::CreateInstance(3,4);
	printf ("MLI(bits=%i levelSize=%i)\n",mli->parameters.levelSizeBits,mli->parameters.levelSize);

	MultiLevelIndexT<4> mli2(3);

	printf ("MLI(levelSize=%i)\n",mli->parameters.levelSize);
	printf ("MLI2(sizeof=%i levelSize=%i)\n",sizeof(mli2),mli2.parameters.levelSize);


	return 0;
}
}

int main(){
	return MARTe::main_();
}
