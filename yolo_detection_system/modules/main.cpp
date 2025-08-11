/**
 * @file   main.cpp
 * @brief  エントリポイント
 * @author takahashitom
 */

#include "BackgroundDirectionDetector.h"
#include "MiniFigDirectionDetector.h"

int main(int argc, char* argv[])
{
  if(argc < 2) {
    std::cerr << "引数が足りません" << std::endl;
    return -1;
  }

  // 受け取った引数によって判定対象を選択する
  std::string target = argv[1];
  if(target == "fig") {
    MiniFigDirectionDetector detector(
        "/home/takahashi/etrobocon/etrobo/raspike-athrill-v850e2m/sdk/workspace/etrobocon2025/"
        "datafiles/models/11n_100epoch_&_650imgsz_fig.onnx");
    detector.detect();
  } else if(target == "background") {
    BackgroundDirectionDetector detector(
        "/home/takahashi/etrobocon/etrobo/raspike-athrill-v850e2m/sdk/workspace/etrobocon2025/"
        "datafiles/models/11n_100epoch_&_640imgsz_background.onnx");
    detector.detect();
  } else {
    std::cerr << "引数が間違っています" << std::endl;
    return -1;
  }

  return 0;
}