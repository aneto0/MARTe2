expression:
        cdbFile __End


cdbFile:
        expressions { expressions }

expressions:
        __GetNodeName  STRING   \: variables	                       __AddLeaf
        __GetNodeName  STRING   \: ( __GetTypeCast STRING ) variables  __AddLeaf
        __CreateNode   STRING   \: block            	                    
        __CreateNodeV  STRING   \: vectorb 
        bblock

variables:
		scalar
		vector
		matrix

scalar:
		__AddScalar token 

vectorb:
        \[ { blockv }+ __EndVectorB \] 

vector:
        \[ { scalar }+ __EndVector \] 

matrix:
        \[ { vector }+ __EndMatrix \]

block:
        \{ expressions { expressions } __BlockEnd \}

blockv:
        __CreateNodeVE block

bblock:
        \{ expressions { expressions } \}

token:
	STRING
	NUMBER
