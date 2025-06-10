/**
 * @file   StringOperatorTest.cpp
 * @brief  StringOperatorクラスのテスト
 * @author Hara1274
 */

#include "StringOperator.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

using namespace std;

namespace etrobocon2025_test {

  // 改行コードを削除するテスト（LF）
  TEST(StringOperatorTest, removeEOL_LF)
  {
    string actual = "StringOperator test\n";
    string expected = "StringOperator test";

    actual = StringOperator::removeEOL(actual);

    EXPECT_EQ(expected, actual);
  }

  // 改行コードを削除するテスト（CR）
  TEST(StringOperatorTest, removeEOL_CR)
  {
    string actual = "StringOperator test\r";
    string expected = "StringOperator test";

    actual = StringOperator::removeEOL(actual);

    EXPECT_EQ(expected, actual);
  }

  // 改行コードを削除するテスト（CRLF）
  TEST(StringOperatorTest, removeEOL_CRLF)
  {
    string actual = "StringOperator test\r\n";
    string expected = "StringOperator test";

    actual = StringOperator::removeEOL(actual);

    EXPECT_EQ(expected, actual);
  }

  // 改行のない文字列が渡された場合
  TEST(StringOperatorTest, notRemove)
  {
    string actual = "No newline here";
    string expected = "No newline here";

    actual = StringOperator::removeEOL(actual);

    EXPECT_EQ(expected, actual);
  }

  // 空の文字列が渡された場合
  TEST(StringOperatorTest, emptyString)
  {
    string actual = "";
    string expected = "";

    actual = StringOperator::removeEOL(actual);

    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test
