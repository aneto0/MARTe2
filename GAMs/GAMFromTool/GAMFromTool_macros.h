#define TOOL_METHODS_DECLARATION() \
    virtual bool ConfigureToolSignals();\
    virtual void ToolMembersConstructor();\
    virtual void ToolMembersDestructor(); \
    virtual bool ConfigureToolMembers(StructuredDataI &data);
#define TOOL_MEMBERS_DECLARATION()\
    /** +Validator = {Class=BasicTypeStandardValidator min=0 max=9} */\
    /** This is a basic type with a range */\
    uint32 par1;\
    /** +Alias = {Class=StringAliasAll A={0.5 1.5} B={2.5 3.5}} */\
    /** This is a vector with alias */\
    float32 par2[2];\
    /** fixed structure */\
    Struct1 par4;\
    /** This is a vector with undefined size */\
    uint32 *par5;\
    uint32 numberOfpar5;\
    /** array of structures */\
    Struct2 *par6;\
    uint32 numberOfpar6;\
    uint32 *fixedIn;\
    float32 **input2;\
    uint32 numberOfinput2;\
    StructSignal1 *fixedOut;\
    StructSignal2 **output2;\
    uint32 numberOfoutput2;