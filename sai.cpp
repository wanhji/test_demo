#include <opencv2/opencv.hpp>
#include <iostream>

int main() 
{   //1 锁定图片路径
    std::vector<std::string> image_a ={"yuantu/1.png","yuantu/2.png","yuantu/3.png","yuantu/4.png","yuantu/5.png",};
    for(int x = 1 ; x <= 5; x++)
 {
    std::string abc=image_a[x-1];
    cv::Mat src = cv::imread(abc, cv::IMREAD_COLOR);

    if (src.empty()) 
    {
        std::cerr << "无法加载图片" << std::endl;
        continue;;
    }
    cv::Mat hsv ,green_mask,blur;
    //2 将图像从BGR颜色空间转换为HSV颜色空间
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    //3 定义绿色的HSV范围
    cv::Scalar sin_green = cv::Scalar(33, 40, 75);
    cv::Scalar max_green = cv::Scalar(70, 220, 210);
    //4 创建掩码，提取绿色区域
    cv::inRange(hsv, sin_green, max_green, green_mask);
    ////////////////////////////////////////
    ////////————————形态学优化————————////////
    ////////////////////////////////////////
    //1 创建1形态学操作核
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    //2 多次腐蚀
    for (int i = 0; i < 6; i++) 
    {   
        cv::erode(green_mask, green_mask, kernel);
    }
    //3 多次膨胀和中值滤波    
    for (int i = 0; i < 5; i++) 
    {     
        cv::dilate(green_mask, green_mask, kernel);
        cv::medianBlur(green_mask, green_mask,5);
    }
    //4 开运算（去除小噪点）
    cv::morphologyEx(green_mask, green_mask, cv::MORPH_OPEN, kernel);
    //5 闭运算（填充小球内部孔洞）
    cv::morphologyEx(green_mask, green_mask, cv::MORPH_CLOSE, kernel);
    //6 高斯滤波
    cv::GaussianBlur(green_mask, blur, cv::Size(9, 9), 0);
    ///////////////////////////////////////////
    ///////——————面积过滤，霍夫圆检测——————///////
    ///////////////////////////////////////////
    //1 寻找轮廓
    std::vector<std::vector<cv::Point>> contours; 
    std::vector<cv::Vec4i> hierarchy;
    findContours(green_mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //2 遍历所有轮廓并进行面积过滤
    cv::Mat clean_mask = cv::Mat::zeros(green_mask.size(), CV_8UC1);
    for (size_t i = 0; i < contours.size(); i++)
  { //3 算面积大小  
    double area = cv::contourArea(contours[i]);
    
    
    if (area >600 && area < 40000) 
    {
        //4 将符合条件的轮廓，填充在clean_mask上
        drawContours(clean_mask, contours, i, cv::Scalar(255), -1);
    }

  } 
    
    //5 霍夫圆检测
    cv::Mat result_img = src.clone(); 
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(clean_mask, circles, cv::HOUGH_GRADIENT, 
        1,      
        25,     
        100,    
        11,     
        15,   
        45  
        ); 
    /////////////////////////////////////////
    /////————在原图上绘制圆并显示圆数目————//////
    ////////////////////////////////////////
    //1 提取圆心坐标和半径
    int count = 0;
    for (size_t i = 0; i < circles.size(); i++) 
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        
        //2 用红色画圆
        cv::circle(result_img, center, radius, cv::Scalar(0, 0, 255), 2);
        count++;

    }   
    //3 输出检测到的圆的数量
    std::string show_text = "ball number:" + std::to_string(count);
    cv::Point text_pos(result_img.cols - 150, 50);
    cv::putText(result_img, show_text, text_pos, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    //4 创建窗口并显示
    std::cout << "霍夫圆检测数量: " << count << std::endl;
    cv::namedWindow("原图", cv::WINDOW_NORMAL);
    cv::namedWindow("检测结果", cv::WINDOW_NORMAL);
    cv::imshow("原图", src);
    cv::imshow("检测结果",result_img );
    //5 等待按键
    cv::waitKey(0);
    //6 关闭窗口
    cv::destroyAllWindows();
 }
    return 0;
}
