expression:
        cdbFile __End


cdbFile:
        expressions { expressions }

expressions:
        < __GetNodeName STRING > variables < / STRING >	                            __AddLeaf
        < __GetNodeName STRING > ( __GetTypeCast STRING ) variables < / STRING >    __AddLeaf
        < __CreateNode  STRING >  block  < / STRING >          	                    

variables:
		scalar
		vector
		matrix

scalar:
		__AddScalar token 

vector:
        \{ { scalar }+ __EndVector \} 

matrix:
        \{ { vector }+ __EndMatrix \}

block:
        expressions { expressions } __BlockEnd

token:
	STRING
	NUMBER
