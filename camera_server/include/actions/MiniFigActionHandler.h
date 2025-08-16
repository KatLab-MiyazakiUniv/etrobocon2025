#ifndef MINIFIG_ACTION_HANDLER_H
#define MINIFIG_ACTION_HANDLER_H

#include "camera/CameraCapture.h"
#include "image_processors/MiniFigDirectionDetector.h"
#include "common/SocketProtocol.h"

class MiniFigActionHandler {
public:
    MiniFigActionHandler(CameraCapture& camera);

    /**
     * @brief Executes the minifig detection sequence.
     * @param position The current position/attempt number (0-3).
     * @param response The response to be sent back to the client.
     */
    void execute(int position, CameraServer::MiniFigActionResponse& response);

private:
    CameraCapture& camera;
    MiniFigDirectionDetector detector;
    MiniFigDirectionResult firstAttemptResult; // Stores the result of the first detection (at position 0)
    const char* filePath = "etrobocon2025/datafiles/snapshots/";
    const char* uploadFileName = "Fig_FRONT.jpg";

    void detectDirection(cv::Mat& frame, MiniFigDirectionResult& result);
};

#endif // MINIFIG_ACTION_HANDLER_H
