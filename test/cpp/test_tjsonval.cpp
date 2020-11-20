#include <base.h>
#include <mine.h>
#include <qminer.h>

#include "microtest.h"


TEST(TJsonValParsing) {
    // test objects and arrays
    ASSERT_TRUE(TJsonVal::GetValFromStr("{ }")->IsObj());
    ASSERT_TRUE(TJsonVal::GetValFromStr("{\"a\":1}")->IsObj());
    ASSERT_TRUE(TJsonVal::GetValFromStr("[ ]")->IsArr());
    ASSERT_FALSE(TJsonVal::GetValFromStr("{ ]")->IsDef());
    // test basic types
    ASSERT_TRUE(TJsonVal::GetValFromStr("123")->IsNum());
    ASSERT_TRUE(TJsonVal::GetValFromStr("true")->IsBool());
    ASSERT_TRUE(TJsonVal::GetValFromStr("\"abc\"")->IsStr());
    ASSERT_TRUE(TJsonVal::GetValFromStr("null")->IsNull());

    // // ignore escaped null character
     PJsonVal StrNullVal = TJsonVal::GetValFromStr("[\"xxx\\u0000yyy\"]");
     ASSERT_TRUE(StrNullVal->IsArr());
     ASSERT_EQ(StrNullVal->GetArrVals(), 1);
     ASSERT_TRUE(StrNullVal->GetArrVal(0)->IsStr());
     ASSERT_EQ_TSTR(StrNullVal->GetArrVal(0)->GetStr(), TStr("xxx yyy"));

    // handling of escapes
     ASSERT_EQ_TSTR(TJsonVal::GetValFromStr("\"\\t\"")->GetStr(), TStr("\t"));
     ASSERT_EQ_TSTR(TJsonVal::GetValFromStr("\"\\R\"")->GetStr(), TStr("R"));
}