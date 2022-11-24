#ifndef _ROI_FINDER_CPP_
#define _ROI_FINDER_CPP_

#include "../include/roi_finder.h"

RoiFinder::RoiFinder(Mat frame) {
    originRect = Rect(0, 0, frame.cols, frame.rows);
}

void RoiFinder::findRoi(armors armor, Mat originFrame)
{
    Size2i roi_size = Size2i(m_roi_factor_w * (int)armor.boardw, m_roi_factor_h * (int)armor.boardh);
    // cout << size <<endl;
    Point2i roi_point = Point2i((m_roi_factor_w/2) * armor.boardw, (m_roi_factor_h/2) * armor.boardh);
    // cout << point <<endl;
    Rect ROIRect = Rect(armor.corner[1], armor.corner[3]) + roi_size - roi_point ;
#ifdef IMSHOW
    rectangle(originFrame, ROIRect, Scalar(255, 0, 0));
#endif
    m_roi_que.push(ROIRect);
}

Rect RoiFinder::getRoi() {
    if(m_roi_que.empty()) {
        return originRect;
    }
    else{
        Rect tempRect;
        tempRect = m_roi_que.front();
        m_roi_que.pop();
        return tempRect;
    }
}

#endif