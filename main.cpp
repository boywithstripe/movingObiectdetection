//ʹ�ñ����������˶�Ŀ����
#include<iostream>
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
//#include <opencv2\imgcodecs\imgcodecs_c.h>
//#include "opencv2/imgcodecs/legacy/constants_c.h"
using namespace std;

//�˶������⡪��֡�  

cv::Mat MoveDetect(cv::Mat temp, cv::Mat frame)
{
    cv::Mat result = frame.clone();
    //1.��background��frameתΪ�Ҷ�ͼ  
    cv::Mat gray1, gray2;
    cv::cvtColor(temp, gray1, CV_BGR2GRAY);
    cv::cvtColor(frame, gray2, CV_BGR2GRAY);
    //2.��background��frame����
    cv::Mat diff;
    cv::absdiff(gray1, gray2, diff);
    cv::imshow("diff", diff);
    //3.�Բ�ֵͼdiff_thresh������ֵ������  
    cv::Mat diff_thresh;
    cv::threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
    cv::imshow("diff_thresh", diff_thresh);
    //4.��ʴ
    cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(18, 18));
    cv::erode(diff_thresh, diff_thresh, kernel_erode);
    cv::imshow("erode", diff_thresh);
    //5.����  
    cv::dilate(diff_thresh, diff_thresh, kernel_dilate);
    cv::imshow("dilate", diff_thresh);
    //6.������������������  
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cv::drawContours(result, contours, -1, cv::Scalar(0, 0, 255), 2);//��result�ϻ�������  
                                                             //7.��������Ӿ���  
    std::vector<cv::Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        boundRect[i] = boundingRect(contours[i]);
        rectangle(result, boundRect[i], cv::Scalar(0, 255, 0), 2);//��result�ϻ�������Ӿ���  
    }
    return result;//����result  
}

int main() {
    cv::VideoCapture video("Rec 0001.mp4");
    if (!video.isOpened())
        return -1;
    int frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);//��ȡ֡��
    double FPS = video.get(cv::CAP_PROP_FPS);//��ȡFPS  
    cv::Mat frame;//�洢֡
    cv::Mat temp;//�洢ǰһ֡ͼ��  
    cv::Mat result;//�洢���ͼ��  
    for (int i = 0; i < frameCount; i++)
    {
        if (!video.read(frame))
            break;
        cv::imshow("frame", frame);
        if (frame.empty())//��֡�����쳣���  
        {
            cout << "frame is empty!" << endl;
            break;
        }
        if (i == 0)//���Ϊ��һ֡��temp��Ϊ�գ�  
        {
            result = MoveDetect(frame, frame);//����MoveDetect()�����˶������⣬����ֵ����result  
        }
        else//�����ǵ�һ֡��temp��ֵ�ˣ�  
        {
            result = MoveDetect(temp, frame);//����MoveDetect()�����˶������⣬����ֵ����result  

        }
        cv::imshow("result", result);
        if (cv::waitKey(1000.0 / FPS) == 27)//��ԭFPS��ʾ  
        {
            cout << "ESC�˳�!" << endl;
            break;
        }
        temp = frame.clone();
    }
    return 0;
}