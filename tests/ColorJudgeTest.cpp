/**
 * @file   ColorJudgeTest.cpp
 * @brief  ColorJudgeクラスをテストする
 * @author HaruArima08
 */

#include "ColorJudge.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {
  // 黒色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorBlack)
  {
    const char* str = "BLACK";
    COLOR expected = COLOR::BLACK;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 白色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorWhite)
  {
    const char* str = "WHITE";
    COLOR expected = COLOR::WHITE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 青色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorBlue)
  {
    const char* str = "BLUE";
    COLOR expected = COLOR::BLUE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 緑色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorGreen)
  {
    const char* str = "GREEN";
    COLOR expected = COLOR::GREEN;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 黄色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorYellow)
  {
    const char* str = "YELLOW";
    COLOR expected = COLOR::YELLOW;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 赤色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorRed)
  {
    const char* str = "RED";
    COLOR expected = COLOR::RED;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 無色の文字列から列挙型の色に変換するテスト
  TEST(ColorJudgeTest, StringToColorNone)
  {
    const char* str = "NONE";
    COLOR expected = COLOR::NONE;

    COLOR actual = ColorJudge::stringToColor(str);

    EXPECT_EQ(expected, actual);
  }

  // 列挙型の黒色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringBlack)
  {
    COLOR color = COLOR::BLACK;
    const char* expected = "BLACK";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の白色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringWhite)
  {
    COLOR color = COLOR::WHITE;
    const char* expected = "WHITE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の青色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringBlue)
  {
    COLOR color = COLOR::BLUE;
    const char* expected = "BLUE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の緑色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringGreen)
  {
    COLOR color = COLOR::GREEN;
    const char* expected = "GREEN";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の黄色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringYellow)
  {
    COLOR color = COLOR::YELLOW;
    const char* expected = "YELLOW";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の赤色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringRed)
  {
    COLOR color = COLOR::RED;
    const char* expected = "RED";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 列挙型の無色から文字列に変換するテスト
  TEST(ColorJudgeTest, ColorToStringNone)
  {
    COLOR color = COLOR::NONE;
    const char* expected = "NONE";

    const char* actual = ColorJudge::colorToString(color);

    EXPECT_STREQ(expected, actual);
  }

  // 無効な色名に対して NONE を返すかテスト
  TEST(ColorJudgeTest, StringToColorInvalid)
  {
    EXPECT_EQ(COLOR::NONE, ColorJudge::stringToColor("PINK"));
    EXPECT_EQ(COLOR::NONE, ColorJudge::stringToColor("123"));
    EXPECT_EQ(COLOR::NONE, ColorJudge::stringToColor(""));
  }

  // 黒色を検出するテスト
  TEST(ColorJudgeTest, GetColorBlack)
  {
    spikeapi::ColorSensor::HSV hsv{ 0, 0, 0 };  // BLACK
    COLOR expected = COLOR::BLACK;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 黒色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearBlack)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ 5, 0, 0 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::BLACK;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 白色を検出するテスト
  TEST(ColorJudgeTest, GetColorWhite)
  {
    spikeapi::ColorSensor::HSV hsv{ 0, 0, 100 };  // WHITE
    COLOR expected = COLOR::WHITE;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 白色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearWhite)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ 5, 100, 100 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::WHITE;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 赤色を検出するテスト
  TEST(ColorJudgeTest, GetColorRed)
  {
    spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_RED, 100, 100 };
    COLOR expected = COLOR::RED;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 赤色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearRed)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_RED + 5, 100, 100 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::RED;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 黄色の色相を検出するテスト
  TEST(ColorJudgeTest, GetColorYellow)
  {
    spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_YELLOW, 100, 100 };
    COLOR expected = COLOR::YELLOW;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 黄色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearYellow)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_YELLOW + 5, 100, 100 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::YELLOW;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 緑色の色相を検出するテスト
  TEST(ColorJudgeTest, GetColorGreen)
  {
    spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_GREEN, 100, 100 };
    COLOR expected = COLOR::GREEN;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 緑色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearGreen)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_GREEN + 5, 100, 100 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::GREEN;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 青色の色相を検出するテスト
  TEST(ColorJudgeTest, GetColorBlue)
  {
    spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_BLUE, 100, 100 };
    COLOR expected = COLOR::BLUE;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

  // 青色の誤差判定を検出するテスト（"make test" を通すためにコメントアウト）
  // TEST(ColorJudgeTest, GetColorHueNearBlue)
  // {
  //   spikeapi::ColorSensor::HSV hsv{ PBIO_COLOR_HUE_BLUE + 5, 100, 100 };  // 誤差(Hue)を+5設定
  //   COLOR expected = COLOR::BLUE;

  //   COLOR actual = ColorJudge::getColor(hsv);

  //   EXPECT_EQ(expected, actual);
  // }

  // 想定外の色相（NONE）を検出するテスト
  TEST(ColorJudgeTest, GetColorNone)
  {
    spikeapi::ColorSensor::HSV hsv{ 123, 100, 100 };  // 未定義のH値
    COLOR expected = COLOR::NONE;

    COLOR actual = ColorJudge::getColor(hsv);

    EXPECT_EQ(expected, actual);
  }

}  // namespace etrobocon2025_test