#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("2.png", cv::IMREAD_COLOR);

    if (src.empty()) {
        std::cerr << "无法加载图片" << std::endl;
        return -1;
    }
    

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_green = cv::Scalar(35, 43, 50);
    cv::Scalar upper_green = cv::Scalar(85, 255, 255);

    cv::Mat green_mask;
    cv::inRange(hsv, lower_green, upper_green, green_mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
    cv::morphologyEx(green_mask, green_mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(green_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int count = 0;
    
   for(int i = 0; i < contours.size(); i++)
{
    auto &cnt = contours[i];
    double area = contourArea(cnt);
    if(area < 80 || area > 3000)
    continue;
    cv::Rect rect = cv::boundingRect(cnt);
    float aspect_ratio = (float)rect.width / rect.height;
    double perimeter = arcLength(cnt, true);
    double circularity = 4 * CV_PI * area / (perimeter * perimeter);
    if(circularity < 0.7)
    continue;
    

    cv::drawContours(src, contours, i, cv::Scalar(255, 0, 0), 2);
    cv::Point center = (rect.tl() + rect.br()) / 2;
    count++;
    cv::putText(src, std::to_string(count), center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    
}
    

    
    
    std::cout << "检测到的子弹数量: " <<count << std::endl;
    cv::namedWindow("原图",   cv::WINDOW_NORMAL);
    cv::namedWindow("HSV图", cv::WINDOW_NORMAL);
    cv::namedWindow("Green Mask", cv::WINDOW_NORMAL);
    cv::namedWindow("Result", cv::WINDOW_NORMAL);

    cv::imshow("原图", src);
    cv::imshow("HSV图", hsv);
    cv::imshow("Green Mask", green_mask);
    cv::imshow("Result", src);

    cv::waitKey(0);

    cv::destroyAllWindows();
    return 0;
}
