
#include <RegularExpression.h>
#include <RuntimeEvaluator.h>
#include "Private/RuntimeEvaluatorVariableInformation.h"
#include "StartupManager.h"
#include "HighResolutionTimer.h"
#include "BasicConsole.h"
#include "MicroSeconds.h"
#include "MathExpressionParser.h"
#include "ConfigurationDatabase.h"
#include "StreamMemoryReference.h"
#include "AnyObjectT.h"



using namespace MARTe;



namespace ruleSet{

const uint32 commentElement    = 1;

const uint32 spaceElement      = 2;

const uint32 identifierElement = 3;

const uint32 stringElement     = 4;

const uint32 numberElement     = 5;

const uint32 operators         = 6;


/*
 */
const RegularExpression::PatternInformation rules[]={
        {"//!*[^\n]\n"                                                                       ,"line comment"     , commentElement       ,true},
        {"/\\*!*?\\a\\*/"                                                                    ,"multiline comment", commentElement       ,true},
        {"+[ \n\t]"                                                                          ,"separator"        , spaceElement         ,true},
        {"&&"                                                                                ,"AND operator"     , operators            ,false},
        {"\\|\\|"                                                                            ,"OR operator"      , operators            ,false},
        {"\\^"                                                                               ,"XOR operator"     , operators            ,false},
        {"<="                                                                                ,"LTE operator"     , operators            ,false},
        {">="                                                                                ,"GTE operator"     , operators            ,false},
        {"\\+"                                                                               ,"SUM operator"     , operators            ,false},
        {"\\-"                                                                               ,"SUB operator"     , operators            ,false},
        {"\\*"                                                                               ,"MUL operator"     , operators            ,false},
        {"/"                                                                                 ,"DIV operator"     , operators            ,false},
        {"="                                                                                 ,"EQ  operator"     , operators            ,false},
        {">"                                                                                 ,"GT  operator"     , operators            ,false},
        {"<"                                                                                 ,"LT  operator"     , operators            ,false},
        {";"                                                                                 ,";  operator"      , operators            ,false},
        {","                                                                                 ,",  operator"      , operators            ,false},
        {"\\!"                                                                               ,"!  operator"      , operators            ,false},
        {"\\("                                                                               ,"open("            , operators            ,false},
        {"\\)"                                                                               ,"close)"           , operators            ,false},
        {"[\\w_]*[\\d\\w_]"                                                                  ,"identifier"       , identifierElement    ,false},
        {"\"*[^\"]\""                                                                        ,"string"           , stringElement        ,false},
        {"($BODY(+\\d)$FRACTION(?(.*\\d))|$FRACTION(.*\\d))?([eE]!$EXP(?[+\\-]{1,5}\\d))"    ,"number"           , numberElement        ,false},
        {"\\"                                                                                ,"EOF"              , 0                    ,false},
        RegularExpression::emptyPattern
        //                {emptyString                                                                                                 ,emptyString             , 0                                        ,false}
};

}


#if 1
CCString Database=
        "expression = \"X=Z,Y=B , X=5+B*(float)C*!(X-Y)+PIPPO(X+Y)+Z; \" "
        "RPN = \"READ X \" "
        "X = (int32) 3";
#endif

CCString MATHExpr= "X=Z,Y=B , X=5+B*(float)C*!(X-Y)+PIPPO(X+Y)+Z; \n";
//CCString MATHExpr= "pipX=pipZ,A=B;";

CCString RPNCode=
        "READ M1\n"
        "READ A\n"
        "MUL\n"
        "READ M2\n"
        "MUL\n"
        "WRITE Mo\n"
        "READ A\n"
        "READ B\n"
        "SUB\n"
        "CONST float32 0.360000\n"
        "MUL\n"
        "WRITE C\n"
        "READ C\n"
        "CONST float32 0.500000\n"
        "POW\n"
        "TAN\n"
        "CAST int32\n"
        "WRITE D\n"
        "READ D\n"
        "READ C\n"
        "LOG\n"
        "CAST int32\n"
        "GT\n"
        "WRITE E\n"
        "READ N1\n"
        "READ N2\n"
        "SUB\n"
        "WRITE TEMP\n"
        "READ TEMP\n"
        "READ TEMP\n"
        "READ N3\n"
        "READ N4\n"
        "DIV\n"
        "ADD\n"
        "WRITE N5\n"
        "READ N5\n"
        "CAST int64\n"
        "GT\n"
        "WRITE F\n"
        ;


CCString line =
        " 121 ALPHA \"BIRRA\" // pip\tpo\n"
        " 122.5 _ALPha   \"BIRRA\"\"\" // pippo\n"
        "/* 123 ALPHA \"BIRRA\" // pippo\n"
        " 124 ALPHA \"BIRRA\" // */ ;pippo\n"
        " .124 126E4 .81 12.7E-5 231.32E97 .165E3 5\n"
        " (A + B    ) * Crap >= (D+X)/ 6\n"
        " (A > 2.0E7) &&  (D = X)\n";


//#########################################################################################################

namespace TestRegularExpressionCallBack{

class MyCallBackClass: public RegularExpression::PatternMatchCallBackClass{
public:

    virtual void Matched(CCString name, uint32 nameLength, CCString value, uint32 valueLength){
        if (name.GetSize() >= nameLength ){
            DynamicCString temp;
            temp().Append(name.GetList(),nameLength);
            printf("%s",temp.GetList());
        }
        printf("=");
        if (value.GetSize() >= valueLength ){
            DynamicCString temp;
            temp().Append(value.GetList(),valueLength);
            printf("%s",temp.GetList());
        }
        printf("\n");
    }

};

void Test()
{
    MyCallBackClass mcbc;
    ErrorManagement::ErrorType ret;
    CCString rule = "*(*[ ]\\-($SET($PARAM_(+\\w)*[ ]=*[ ]$VALUE(+\\d))|$COMMAND(bowl)))";
    CCString line = " -pippo = 3576 -pugnetta = 764 -bowl";
    ret = RegularExpression::Scan(line,rule,&mcbc);
}

}
//#########################################################################################################

//#########################################################################################################

namespace TestRegularExpressionCallBack2{

class xx{
public:
    int j;
};


class MyCallBackClass: public RegularExpression::PatternMatchCallBackClass{

    static CCString rule;

    Queue<Reference> parameters;

    DynamicCString command;

    ErrorManagement::ErrorType retCode;


public:
    MyCallBackClass(){

    }

    virtual void Matched(CCString name, uint32 nameLength, CCString value, uint32 valueLength){

if (name.GetSize() != 0)
printf ("%c %i %i\n",name[0],nameLength,retCode.format_as_integer);

        if ((nameLength >0) && retCode){
            if (name.CompareContent("COMMAND",nameLength)==0){
                if (command.GetSize() == 0){
                    command().Append(value.GetList(),valueLength);
                } else {
                    retCode.syntaxError = true;
                }
            } else

            if (name.CompareContent("VARNAME",nameLength)==0){
                ReferenceT<AnyObjectT<DynamicCString> > x(HeapManager::standardHeapId);

                retCode.fatalError = !x.IsValid();
                REPORT_ERROR(retCode,"failed creation of AnyObjectT<DynamicCString> >");

                if (retCode){
                    x->operator ()().Append(value,valueLength);
                    parameters.Insert(x);
                }
            } else

            if (name.CompareContent("NUMBER",nameLength)==0){
                StreamMemoryReference smr(value,valueLength);
                TypeDescriptor td = Float32Bit;
                Reference ref = AnyObject::Allocate(td.StorageSize(),td);
                AnyType dest ;
                ref.ToAnyType(dest);
                AnyType src(smr);
                smr.Seek(0);
                retCode = src.CopyTo(dest);
                REPORT_ERROR(retCode,"failed conversion");

                if (retCode){
                    parameters.Insert(ref);
                }

            } else
            if (name.CompareContent("STRING",nameLength)==0){

                ReferenceT<AnyObjectT<xx> > x(HeapManager::standardHeapId);

                retCode.fatalError = !x.IsValid();
                REPORT_ERROR(retCode,"failed creation of AnyObjectT<DynamicCString> >");

                if (retCode){
                    parameters.Insert(x);
                }

            } else
            if (name.CompareContent("LINE",nameLength)==0){
                StreamString output;
                output.Printf("COMMAND = %s ",command);
                command = "";

                while ((parameters.Size() > 0) && retCode){
                    Reference ref;
                    retCode = parameters.Remove(ref);
                    REPORT_ERROR(retCode,"failed parameters.Remove");

                    if (retCode){
                        output.Printf(" (%Q)%! ",ref,ref);
                    }
                }

                printf("%s\n",output.Buffer().GetList());
            }

        }

    }

    void Parse(CCString line ){
        MyCallBackClass mcbc;
        ErrorManagement::ErrorType ret;
        ret = RegularExpression::Scan(line,rule,this);
    }

};

//CCString MyCallBackClass::rule = "*($COMMAND([\\w_]*[\\d\\w_])*(*[\t ][$VARNAME([\\w_]*[\\d\\w_])|$VALUE(?($CAST(\\([\\w_]*[\\d\\w_]\\)))[$NUMBER(+\\d?(.*\\d)|.*\\d)?([eE]!(?[+\\-]{1,5}\\d))|\"$STRING(*[^\"])\"])])\n)";
//$NUMBER(+\\d?(.*\\d)|.*\\d)?([eE]!?[+\\-]{1,5}\\d)
//CCString MyCallBackClass::rule = "*($LINE($COMMAND([\\w_]*[\\d\\w_])*(*[\t ][$VARNAME([\\w_]*[\\d\\w_])|$NUMBER(+\\d?(.*\\d)|.*\\d)?([eE]!?[+\\-]{1,5}\\d)|\"$STRING(*[^\"])\"])\n))";

CCString MyCallBackClass::rule =
        "*($LINE("
               "$COMMAND([\\w_]*[\\d\\w_])"
               "*(*[\t ]("
                    "$VARNAME([\\w_]*[\\d\\w_])|"
                    "$NUMBER(+\\d?(.*\\d)|.*\\d)?([eE]!?[+\\-]{1,5}\\d)|"
                    "$VECTOR(\\{*[\\d\\w\t ,.\\-+]\\})|"
                    "$MATRIX(\\{+(*[\t ,]\\{*[\\d\\w\t ,.\\-+]\\})*[\t ,]\\})|"
                    "\"$STRING(*[^\"])\""
               "))*[\t ]\n"
        "))";


CCString RPNCode=
        "READ M1 MX\n"
        "READ A B \n"
        "MUL\n"
        "READ M2\n"
        "MUL\n"
        "WRITE Mo\n"
        "READ A\n"
        "READ B\n"
        "SUB\n"
        "CONST float32 0.360000\n"
        "MUL\n"
        "WRITE C\n"
        "READ C\n"
        "POW\n"
        "TAN\n"
        "CAST \"int32\"\n"
        "CONST float32 {0.50000 15,8 , a , b , 0.15E25, -56} \n"
        "CONST {{0.50000 15},{8 , a }, {b , 0.15E25, -56} }\n"
        "WRITE D\n"
        "READ D\n"
        "READ C\n"
        "LOG\n"
        "CAST int32\n"
        "GT\n"
        "WRITE E\n"
        "READ N1\n"
        "READ N2\n"
        "SUB\n"
        "WRITE TEMP\n"
        "READ TEMP\n"
        "READ TEMP\n"
        "READ N3\n"
        "READ N4\n"
        "DIV\n"
        "ADD\n"
        "WRITE N5\n"
        "READ N5\n"
        "CAST int64\n"
        "GT\n"
        "WRITE F\n"
        ;


void Test()
{
    MyCallBackClass mcbc;
    ErrorManagement::ErrorType ret;
    mcbc.Parse(RPNCode);
}

}
//#########################################################################################################



void ShowVariables(RuntimeEvaluator & context){
    uint32 index = 0;
    RuntimeEvaluator::VariableInformation *var;

    while(context.BrowseInputVariable(index,var)){
        StreamString ss;
        AnyType at = var->GetAnyType(context);
        ss.Printf("input  var % 3i @%04x (% 16t) % 16s = %10! ",index,var->GetLocation(),at,var->GetName(),at);
        if (var->GetExternalMemoryPtr() != NULL){
            ss.Printf ("@@%p",var->GetExternalMemoryPtr());
        }
        printf ("%s\n",ss.Buffer().GetList());
        index++;
    }

    index = 0;
    while(context.BrowseOutputVariable(index,var)){
        StreamString ss;
        AnyType at = var->GetAnyType(context);
        ss.Printf("input  var % 3i @%04x (% 16t) % 16s = %10! ",index,var->GetLocation(),at,var->GetName(),at);
        if (var->GetExternalMemoryPtr() != NULL){
            ss.Printf ("@@%p",var->GetExternalMemoryPtr());
        }
        printf ("%s\n",ss.Buffer().GetList());
        index++;
    }

    printf("size of data area = %i\n",context.GetSizeOfVariables());
    printf("size of code area = %i\n",context.GetSizeOfCode());
    printf("size of stack area = %i\n",context.GetSizeOfStack());

}

int main(){

    StartupManager::Initialise();

    DynamicCString dcs;

#if 0
    {
        StreamString ss;
        ss.Printf("[% 5x][%05x][% 5i][%05i]", 125,125,125,125);
        printf("%s\n",ss.Buffer().GetList());
        return 0;
    }
#endif

#if 0
    Matrix<Vector<float *> > const *  *(*pippo[2])[5][6];

    //        float * pippo;

    AnyType Apippo(pippo);

    Apippo.ToString(dcs);


    printf("[[%s][%s]]\n",dcs.GetList(),Apippo.GetFullVariableDescriptor().GetModifiers());


    VariableDescriptor vd;
    vd.FromString(dcs.GetList());
    DynamicCString dcs2;
    vd.ToString(dcs2);


    printf("[[%s][%s]]\n",dcs2.GetList(),vd.GetModifiers());


#endif
#if 0
    {
        CCString lineP = MATHExpr;
        ErrorManagement::ErrorType ret;
        while (ret) {
            DynamicCString content;
            //        const LexicalAnalyzerRule *q = Parse(lineP,content);
            const RegularExpression::PatternInformation *selectedRule = NULL;
            ret = RegularExpression::MatchRules(lineP,ZeroTerminatedArray<const RegularExpression::PatternInformation>(&ruleSet::rules[0]),selectedRule,content);

            if (selectedRule  && (content.GetSize() > 0) && ret){
                if (!selectedRule->skip){
                    printf("%s [%s]\n",selectedRule->ruleName.GetList(),content.GetList());fflush(stdout);
                } else {
                    printf("SKIP: %s [%s]\n",selectedRule->ruleName.GetList(),content.GetList());fflush(stdout);
                }
            } else {
                if (selectedRule){
                    printf("EMPTY RULE %s\n",selectedRule->ruleName.GetList());
                } else {
                    if (ret){
                        printf("UNMATCHED %s\n",lineP.GetList());
                    } else {
                        printf("ERROR @ %s\n",lineP.GetList());
                    }
                }
                break;
            }
        }
    }
#endif
#if 0
    {
        ErrorManagement::ErrorType ret;
        StreamString lineS = line;
        lineS.Seek(0);
        while (ret) {
            DynamicCString content;
            //        const LexicalAnalyzerRule *q = Parse(lineP,content);
            const RegularExpression::PatternInformation *selectedRule = NULL;
            ret = RegularExpression::MatchRules(lineS,ZeroTerminatedArray<const RegularExpression::PatternInformation>(&ruleSet::rules[0]),selectedRule,content);

            if (selectedRule  && (content.GetSize() > 0)  && ret){
                if (!selectedRule->skip){
                    printf("%s [%s]\n",selectedRule->ruleName.GetList(),content.GetList());
                    fflush(stdout);
                }
            } else {
                if (selectedRule){
                    printf("EMPTY RULE %s\n",selectedRule->ruleName.GetList());
                } else {
                    if (ret){
                        printf("UNMATCHED \n");
                    } else {
                        printf("ERROR \n");
                    }
                }
                break;
            }
        }
    }
#endif
#if 1
    //TestRegularExpressionCallBack::Test();

    TestRegularExpressionCallBack2::Test();
    return 0;
#endif

#if 0
    {
        ConfigurationDatabase cdb;

        AnyType at(Database);

        at.SetFormattedStreamType("CDB");
        at.CopyTo(cdb);

        cdb.MoveToRoot();
        cdb.GetNumberOfChildren();
        printf("cdb has %i nodes\n",cdb.GetNumberOfChildren());

        for (uint32 i =0; i <  cdb.GetNumberOfChildren();i++){
            DynamicCString test;
            cdb.Read(cdb.GetChildName(i).GetList(),test);
            printf("%s = %s\n",cdb.GetChildName(i).GetList(),test.GetList()); fflush(stdout);

        }
    }
#endif


#if 0
    {
        StreamMemoryReference smr(MATHExpr.GetList(),MATHExpr.GetSize());
        smr.Seek(0);
        MathExpressionParser mexp;
        mexp.Parse(smr,NULL/*&err*/,2);

        printf(">>\n%s\n<<\n",mexp.GetStackMachineExpression().GetList());

    }
#endif

#if 1
    {
        RuntimeEvaluator context;

        ErrorManagement::ErrorType ret;

        printf ("VAR SCAN \n");
        ret = context.ExtractVariables(RPNCode);
        printf ("VAR SCAN RESULT\n"); fflush(stdout);
        ShowVariables(context);

        //####################################################
        //    External variables
        //####################################################

        float externalA = 2.0;
        float externalB = 1.0;
        uint8 externalF = 0;

        float M1[3][4] = { { 1,2,3,4},{5,6,7,8},{9,10,11,12}};
        float M2[4][3] = { { 1,2,3},{4,5,6},{7,8,9},{10,11,12}};
        float Mo[3][3];

        if (ret){
            uint32 index = 0U;
            RuntimeEvaluator::VariableInformation *var;

            printf ("VAR BINDING: TYPE AND LOCATION ASSIGNMENT\n");
            while(context.BrowseInputVariable(index,var)){
                if (var->GetName() == "M1"){
                    var->SetType(M1);
                }
                if (var->GetName() == "M2"){
                    var->SetType(M2);
                }
                if (var->GetName() == "A"){
                    var->SetType(externalA);
                }
                if (var->GetName() == "B"){
                    var->SetType(externalB);
                }
                if (var->GetName() == "N1"){
                    var->SetType(AnyType(SignedInteger8Bit,NULL));
                }
                if (var->GetName() == "N2"){
                    var->SetType(AnyType(TypeDescriptor("int8"),NULL));
                }
                if (var->GetName() == "N3"){
                    var->SetType(AnyType(TypeDescriptor("int8"),NULL));
                }
                if (var->GetName() == "N4"){
                    var->SetType(AnyType(TypeDescriptor("int8"),NULL));
                }
                index++;
            }

            index = 0;
            while(context.BrowseOutputVariable(index,var)){
                if (var->GetName() == "Mo"){
                    var->SetType(Mo);
                }
                if (var->GetName() == "D"){
                    var->SetType(AnyType(SignedInteger32Bit,NULL));
                }
                if (var->GetName() == "E"){
                    var->SetType(AnyType(TypeDescriptor("int8"),NULL));
                }
                if (var->GetName() == "F"){
                    var->SetType(externalF);
                }
                if (var->GetName() == "N5"){
                    var->SetType(AnyType(TypeDescriptor("int8"),NULL));
                }
                index++;
            }

            printf ("VAR BINDING RESULT\n"); fflush(stdout);
            ShowVariables(context);
        }

        if (ret){
            printf ("COMPILE\n");fflush(stdout);
            ret = context.Compile(RPNCode);

            printf ("COMPILE RESULT\n");fflush(stdout);
            ShowVariables(context);
        }

        if (ret){
            printf ("ASSIGN INPUT VALUES\n");
            int32 index = 0;
            RuntimeEvaluator::VariableInformation *var;

            while(context.BrowseInputVariable(index,var)){
                index++;
                if (var->GetName() == "N1"){
                    context.Variable<int8>(var->GetLocation()) = 31;
                }
                if (var->GetName() == "N2"){
                    context.Variable<int8>(var->GetLocation()) = 22;
                }
                if (var->GetName() == "N3"){
                    context.Variable<int8>(var->GetLocation()) = 13;
                }
                if (var->GetName() == "N4"){
                    context.Variable<int8>(var->GetLocation()) = 127;
                }
            }

            printf ("VAR ASSIGNMENT RESULT\n");fflush(stdout);
            ShowVariables(context);
        }

        if (ret){
            printf ("DECOMPILE showing types pCode\n");
            DynamicCString RPNCodeRev;
            ret = context.DeCompile(RPNCodeRev,true);

            printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
        }

        if (ret){
            printf ("DECOMPILE not showing types pCode\n");
            DynamicCString RPNCodeRev;
            ret = context.DeCompile(RPNCodeRev,false);

            printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
            if (RPNCode == RPNCodeRev){
                printf("identical to the original source\n");
            } else {
                printf("not identical to the original source\n");
            }
        }


        if (ret){
            printf ("DEBUG MODE EXECUTION \n");
            // let stdout catch up..
            fflush(stdout);
            DynamicCString dcs;
            CStringTool cst = dcs();
            ret = context.Execute(RuntimeEvaluator::debugMode,&cst);
            if (dcs.GetSize() > 0){
                printf("%s\n",dcs.GetList());
            }
        }

        printf ("DEBUG RUN RESULTS\n");fflush(stdout);
        ShowVariables(context);

        if (ret){
            printf ("FAST MODE EXECUTION \n");
            printf ("Executes 1 Million times ");
            Ticks t1,t2;
            t1 = HighResolutionTimer::GetTicks();
            if (ret){
                for (int j = 0;(j<10) && ret;j++) {
                    printf (".");
                    fflush(stdout);
                    for (int i = 0;(i<100000) && ret;i++) {
                        ret = context.Execute(RuntimeEvaluator::fastMode,NULL);
                    }
                }
            }
            t2 = HighResolutionTimer::GetTicks();
            MilliSeconds dT = t2-t1;
            printf (" in %i milliseconds\n",dT.GetTimeRaw());
            fflush(stdout);
        }

        if (ret){
            printf ("SAFE MODE EXECUTION \n");
            printf ("Executes 1 Million times ");
            Ticks t1,t2;
            t1 = HighResolutionTimer::GetTicks();
            if (ret){
                for (int j = 0;(j<10) && ret;j++) {
                    printf (".");
                    fflush(stdout);
                    for (int i = 0;(i<100000) && ret;i++) {
                        ret = context.Execute(RuntimeEvaluator::safeMode,NULL);
                    }
                }
            }
            t2 = HighResolutionTimer::GetTicks();
            MilliSeconds dT = t2-t1;
            printf (" in %i milliseconds\n",dT.GetTimeRaw());
            fflush(stdout);

        }


        if (!ret){
            printf ("FAILED - see log\n");
        }
    }
#endif

    StartupManager::Terminate();

}

