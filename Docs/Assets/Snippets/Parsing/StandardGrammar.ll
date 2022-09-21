expression:
        cdbFile __End


cdbFile:
        expressions { expressions }

expressions:
        __GetNodeName  STRING = variables	                       __AddLeaf
        __GetNodeName  STRING = ( __GetTypeCast STRING ) variables __AddLeaf
        __GetNodeName  STRING = ( __GetExprCast STRING | scalar )  __AddLeaf
        __CreateNode   STRING = block            	                    
        

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
        \{ expressions { expressions } __BlockEnd \}

token:
	STRING
	NUMBER
