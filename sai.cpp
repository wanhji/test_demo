#include <opencv2/opencv.hpp>
#include <iostream>

int main() 
{
    cv::Mat src = cv::imread("2.png", cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        std::cerr << "无法加载图片" << std::endl;
        return -1;
    }
    

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_green = cv::Scalar(35, 43, 120);
    cv::Scalar upper_green = cv::Scalar(85, 255, 255);

    cv::Mat green_mask;
    cv::inRange(hsv, lower_green, upper_green, green_mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    for (int i = 0; i < 6; i++) 
    {
        cv::erode(green_mask, green_mask, kernel);
    }    
    for (int i = 0; i < 5; i++) 
    {    
        cv::dilate(green_mask, green_mask, kernel);   
        cv::medianBlur(green_mask, green_mask,5);
    }
    cv::morphologyEx(green_mask, green_mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(green_mask, green_mask, cv::MORPH_OPEN, kernel);
    

    
    cv::Mat result_img = src.clone();
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(green_mask, circles, cv::HOUGH_GRADIENT, 
                     1,      
                     15,    
                     100,    
                     13,    
                     16,     
                     42      
                     );
    int count = 0;
    for (size_t i = 0; i < circles.size(); i++) 
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
        
        cv::circle(result_img, center, radius, cv::Scalar(0, 0, 255), 2);
        count++;
    }
    std::string show_text = "number:" + std::to_string(count);
    cv::Point text_pos(result_img.cols - 150, 50);
    cv::putText(result_img, show_text, text_pos, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    std::cout << "霍夫圆检测数量: " << count << std::endl;
    cv::namedWindow("原图", cv::WINDOW_NORMAL);
    cv::namedWindow("绿色掩码", cv::WINDOW_NORMAL);
    cv::namedWindow("检测结果", cv::WINDOW_NORMAL);
    cv::imshow("原图", src);
    cv::imshow("绿色掩码", green_mask);
    cv::imshow("检测结果", result_img);

    cv::waitKey(0);
    return 0;
}
