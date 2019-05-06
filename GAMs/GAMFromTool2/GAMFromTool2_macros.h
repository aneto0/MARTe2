#define TOOL_METHODS_DECLARATION() \
    virtual bool ConfigureToolSignals();\
    virtual void ToolMembersConstructor();\
    virtual void ToolMembersDestructor(); \
    virtual bool ConfigureToolMembers(StructuredDataI &data);
#define TOOL_MEMBERS_DECLARATION()\
    StructSignal1 *fixedOut;\
    StructSignal2 **output2;\
    uint32 numberOfoutput2;\
    uint32 *fixedIn;\
    float32 **input2;\
    uint32 numberOfinput2;