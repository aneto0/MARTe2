expression:
        cdbFile __End


cdbFile:
        expressions { expressions }

expressions:
        < __GetNodeName STRING > variables < / STRING >	                            __AddLeaf
        < __GetNodeName STRING > ( STRING __GetTypeCast ) variables < / STRING >    __AddLeaf
        < __CreateNode  STRING >  block  < / STRING >          	                    __BlockEnd

variables:
		scalar
		vector
		matrix

scalar:
		__AddScalar token 

vector:
        < > { scalar }+ < / >

matrix:
        < > { vector __EndRow }+ < / >

block:
        expressions { expressions } 

token:
	STRING
	NUMBER
