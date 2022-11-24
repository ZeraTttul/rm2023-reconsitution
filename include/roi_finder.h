#ifndef _ROI_FINDER_H_
#define _ROI_FINDER_H_

#include "../define/define.h"
#include "armor_detector.h"

using namespace cv;
using namespace std;

class RoiFinder
{
    private:
        cv::Rect originRect;
        float m_roi_factor_w = 6; //roi系数
        float m_roi_factor_h = 8;
        std::queue<cv::Rect> m_roi_que;
    
    public:
        RoiFinder(Mat frame);
        void findRoi(armors armor, Mat originFrame);
        Rect getRoi();
};



#endif