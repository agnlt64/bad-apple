#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <opencv4/opencv2/opencv.hpp>

//const std::string density = "Ñ@#W$9876543210?!abc;:+=-,._ ";
const std::string density = "       .:-i|=+%O#@";

#define ESC 27
#define FPS 30
#define WIDTH 135
#define HEIGHT 49

std::string ascii(cv::Mat frame, int width, int height)
{
    std::string image;

    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(width, height));
    cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGR2GRAY);

    for (int y = 0; y < resizedFrame.rows; ++y)
    {
        for (int x = 0; x < resizedFrame.cols; ++x)
        {
            int luminance = resizedFrame.at<uchar>(y, x);
            int index = luminance * (density.size() - 1) / 255;
            image += density[index];
        }
        image += "\n";
    }
    
    return image;
}

void print_frame(cv::Mat frame)
{
    auto start = std::chrono::system_clock::now();
    std::string out = "";

    // for (int i = 0; i < 7; i++) out += ascii(frame, WIDTH, HEIGHT);

    // reset the terminal
    std::cout << "\033[2J\n";
    std::cout << ascii(frame, WIDTH, HEIGHT) << '\n';

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    int remainingDelay = std::max(FPS - static_cast<int>(elapsedMillis), 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(remainingDelay));

}

int main()
{
    cv::VideoCapture capture{ "./bad_apple.mp4" };

    if (!capture.isOpened())
    {
        std::cerr << "Could not read video\n";
        return 1;
    }

    while (true)
    {
        cv::Mat frame;
        capture >> frame;

        if (frame.empty()) break;
        
        print_frame(frame);
    }


    capture.release();
    return 0;
}
