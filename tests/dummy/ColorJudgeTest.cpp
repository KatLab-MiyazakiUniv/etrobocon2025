/**
 * @file   ColorJudgeTest.cpp
 * @brief  ColorJudgeクラスをテストする
 * @author HaruArima08
 */

#include "ColorJudge.h"
#include <gtest/gtest.h>

namespace etrobocon2024_test {
  // 黒色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorBlack)
  {
    const char* str = "BLACK";
    COLOR expected = COLOR::BLACK;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 白色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorWhite)
  {
    const char* str = "WHITE";
    COLOR expected = COLOR::WHITE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 青色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorBlue)
  {
    const char* str = "BLUE";
    COLOR expected = COLOR::BLUE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 緑色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorGreen)
  {
    const char* str = "GREEN";
    COLOR expected = COLOR::GREEN;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 黄色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorYellow)
  {
    const char* str = "YELLOW";
    COLOR expected = COLOR::YELLOW;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 赤色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorRed)
  {
    const char* str = "RED";
    COLOR expected = COLOR::RED;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 無色の文字列から列挙型の色に変換する
  TEST(ColorJudgeTest, stringToColorNone)
  {
    const char* str = "NONE";
    COLOR expected = COLOR::NONE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 列挙型の黒色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringBlack)
  {
    COLOR color = COLOR::BLACK;
    const char* expected = "BLACK";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の白色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringWhite)
  {
    COLOR color = COLOR::WHITE;
    const char* expected = "WHITE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の青色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringBlue)
  {
    COLOR color = COLOR::BLUE;
    const char* expected = "BLUE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の緑色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringGreen)
  {
    COLOR color = COLOR::GREEN;
    const char* expected = "GREEN";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の黄色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringYellow)
  {
    COLOR color = COLOR::YELLOW;
    const char* expected = "YELLOW";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の赤色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringRed)
  {
    COLOR color = COLOR::RED;
    const char* expected = "RED";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の無色から文字列に変換する
  TEST(ColorJudgeTest, colorToStringNone)
  {
    COLOR color = COLOR::NONE;
    const char* expected = "NONE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

}  // namespace etrobocon2024_test