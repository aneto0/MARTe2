expression:
        cdbFile __End


cdbFile:
        expressions { expressions }

expressions:
        __GetNodeName  STRING   \: variables	                            __AddLeaf
        __CreateNode   STRING   \: block            	                    
        bblock

variables:
		scalar
		vector
		matrix

scalar:
		__AddScalar token 

vector:
        \[ { scalar }+ __EndVector \] 

matrix:
        \[ { vector }+ __EndMatrix \]

block:
        \{ expressions { expressions } __BlockEnd \}

bblock:
        \{ expressions { expressions } \}

token:
	STRING
	NUMBER
