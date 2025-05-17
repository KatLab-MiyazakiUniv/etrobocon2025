/**
 * @file   AreaACameraAction.cpp
 * @brief  カメラを制御するクラス
 * @author Haranaruki
 */

 #include "CameraCapure.h"

void CameraCapture::start() {
    std::cout << "Camera started." << std::endl;

    cv::VideoCapture cap(0); // デバイスID 0 を指定

    if (!cap.isOpened()) {
        std::cerr << "カメラを開けませんでした。" << std::endl;
        return ;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::imshow("USB Camera", frame);
        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return ;
}
