/**
 * @file   ColorJudgeTest.cpp
 * @brief  ColorJudgeクラスをテストする
 * @author HaruArima08
 */

#include "ColorJudge.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {
  // 黒色の文字列から列挙型の色に変換するテスト:
  // "BLACK"文字列がCOLOR::BLACKに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorBlack)
  {
    const char* str = "BLACK";
    COLOR expected = COLOR::BLACK;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 白色の文字列から列挙型の色に変換するテスト:
  // "WHITE"文字列がCOLOR::WHITEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorWhite)
  {
    const char* str = "WHITE";
    COLOR expected = COLOR::WHITE;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 青色の文字列から列挙型の色に変換するテスト:
  // "BLUE"文字列がCOLOR::BLUEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorBlue)
  {
    const char* str = "BLUE";
    COLOR expected = COLOR::BLUE;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 緑色の文字列から列挙型の色に変換するテスト:
  // "GREEN"文字列がCOLOR::GREENに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorGreen)
  {
    const char* str = "GREEN";
    COLOR expected = COLOR::GREEN;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 黄色の文字列から列挙型の色に変換するテスト:
  // "YELLOW"文字列がCOLOR::YELLOWに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorYellow)
  {
    const char* str = "YELLOW";
    COLOR expected = COLOR::YELLOW;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 赤色の文字列から列挙型の色に変換するテスト:
  // "RED"文字列がCOLOR::REDに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorRed)
  {
    const char* str = "RED";
    COLOR expected = COLOR::RED;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 無色の文字列から列挙型の色に変換するテスト:
  // "NONE"文字列がCOLOR::NONEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, StringToColorNone)
  {
    const char* str = "NONE";
    COLOR expected = COLOR::NONE;

    COLOR actual = ColorJudge::convertStringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 列挙型の黒色から文字列に変換するテスト:
  // COLOR::BLACKが"BLACK"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringBlack)
  {
    COLOR color = COLOR::BLACK;
    const char* expected = "BLACK";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の白色から文字列に変換するテスト:
  // COLOR::WHITEが"WHITE"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringWhite)
  {
    COLOR color = COLOR::WHITE;
    const char* expected = "WHITE";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の青色から文字列に変換するテスト:
  // COLOR::BLUEが"BLUE"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringBlue)
  {
    COLOR color = COLOR::BLUE;
    const char* expected = "BLUE";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の緑色から文字列に変換するテスト:
  // COLOR::GREENが"GREEN"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringGreen)
  {
    COLOR color = COLOR::GREEN;
    const char* expected = "GREEN";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の黄色から文字列に変換するテスト:
  // COLOR::YELLOWが"YELLOW"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringYellow)
  {
    COLOR color = COLOR::YELLOW;
    const char* expected = "YELLOW";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の赤色から文字列に変換するテスト:
  // COLOR::REDが"RED"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringRed)
  {
    COLOR color = COLOR::RED;
    const char* expected = "RED";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の無色から文字列に変換するテスト:
  // COLOR::NONEが"NONE"文字列に正しく変換されることを検証する。
  TEST(ColorJudgeTest, ColorToStringNone)
  {
    COLOR color = COLOR::NONE;
    const char* expected = "NONE";

    const char* actual = ColorJudge::convertColorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 無効な色名に対して NONE を返すかテスト:
  // 無効な色名が与えられた場合にCOLOR::NONEが返されることを検証する。
  TEST(ColorJudgeTest, StringToColorInvalid)
  {
    EXPECT_EQ(COLOR::NONE, ColorJudge::convertStringToColor("PINK"));
    EXPECT_EQ(COLOR::NONE, ColorJudge::convertStringToColor("123"));
    EXPECT_EQ(COLOR::NONE, ColorJudge::convertStringToColor(""));
  }

  // 黒色の色相を検出するテスト: 黒色のHSV値がCOLOR::BLACKに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorBlack)
  {
    spike::HsvResponse hsv{ 0, 0, 0 };  // BLACK
    COLOR expected = COLOR::BLACK;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 白色の色相を検出するテスト: 白色のHSV値がCOLOR::WHITEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorWhite)
  {
    spike::HsvResponse hsv{ 0, 0, 100 };  // WHITE
    COLOR expected = COLOR::WHITE;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 赤色の色相を検出するテスト: 赤色のHSV値がCOLOR::REDに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorRed)
  {
    spike::HsvResponse hsv{ static_cast<int>(spike::ColorHue::RED), 100, 100 };
    COLOR expected = COLOR::RED;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 黄色の色相を検出するテスト: 黄色のHSV値がCOLOR::YELLOWに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorYellow)
  {
    spike::HsvResponse hsv{ static_cast<int>(spike::ColorHue::YELLOW), 100, 100 };
    COLOR expected = COLOR::YELLOW;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 緑色の色相を検出するテスト: 緑色のHSV値がCOLOR::GREENに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorGreen)
  {
    spike::HsvResponse hsv{ static_cast<int>(spike::ColorHue::GREEN), 100, 100 };
    COLOR expected = COLOR::GREEN;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 青色の色相を検出するテスト: 青色のHSV値がCOLOR::BLUEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorBlue)
  {
    spike::HsvResponse hsv{ static_cast<int>(spike::ColorHue::BLUE), 100, 100 };
    COLOR expected = COLOR::BLUE;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 想定外の色相（NONE）を検出するテスト:
  // 未定義のHSV値がCOLOR::NONEに正しく変換されることを検証する。
  TEST(ColorJudgeTest, GetColorNone)
  {
    spike::HsvResponse hsv{ 123, 100, 100 };  // 未定義のH値
    COLOR expected = COLOR::NONE;

    COLOR actual = ColorJudge::convertHsvToColor(hsv);

    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test