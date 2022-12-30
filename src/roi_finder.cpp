#ifndef _ROI_FINDER_CPP_
#define _ROI_FINDER_CPP_

#include "../include/roi_finder.h"

void RoiFinder:: init(Mat originFrame) {
    m_originFrame = originFrame;
}


void RoiFinder:: getOrigin(){
    originRect = Rect(0, 0, m_originFrame.cols, m_originFrame.rows);
}

void RoiFinder::findRoi(armors armor,bool isDetected)
{
    Rect ROIRect;
    cout<< "isdtected = " << isDetected <<endl;
    if(isDetected)
    {
        Size2i roi_size = Size2i(m_roi_factor_w * (int)armor.boardw, m_roi_factor_h * (int)armor.boardh);
        // cout << size <<endl; 
        Point2i roi_point = Point2i((m_roi_factor_w/2) * armor.boardw, (m_roi_factor_h/2) * armor.boardh);
        // cout << point <<endl;
        ROIRect = Rect(armor.corner[1], armor.corner[3]) + roi_size - roi_point ;
        circle(m_originFrame, armor.center, 3, Scalar(255,0,0));
        cout << "roirect = " << ROIRect <<endl;
    }
    else
    {
        getOrigin();
        ROIRect = originRect;
    }
#ifdef IMSHOW
    rectangle(m_originFrame, ROIRect, Scalar(255, 0, 0));
#endif

    // Mat roiframe = originFrame(ROIRect);
    // imshow("roi", roiframe);
    m_roi_que.push(ROIRect);
}

Rect RoiFinder::getRoi() {
    if(m_roi_que.empty()) {
        getOrigin();
        cout << "origin" << endl;
        return originRect;
    }
    else {
        Rect tempRect;
        tempRect = m_roi_que.front();
        //cout << m_roi_que.size() << endl;
        m_roi_que.pop();
        // cnt++;
        // cout << "cnt = " << cnt << endl;
        // if(cnt == 15){
        //     cnt = 0;
        //     getOrigin();
        //     return originRect;
        // }
        return tempRect;
    }
}

#endif