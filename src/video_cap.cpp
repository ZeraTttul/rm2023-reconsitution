//
// Created by JYSimilar on 2022/11/3.
//
//

#include "../include/video_cap.h"


VideoCap :: VideoCap() {
    if (!m_filePath.empty()) {
        m_cap.open(m_filePath);
    } else {
        m_cap.open(m_capIndex);
    }
}


// VideoCap :: VideoCap(int mode) 
// {
//     switch (mode)
//     {
//         case 0:                             //Video
//             if(!m_filePath.empty()) {
//                 m_cap.open(m_filePath);
//             } else {
//                 std::cout<< "can not open the Video" << std::endl;
//             }
//             break;
        
//         case 1:                             //Cap
//             m_cap.open(m_capIndex);
//             break;
        
//         case 2:                             //Hikvision
//             MVS_Cap.CamInfoShow();
//             break;
//         default:
//             break;
//     }
// }

void VideoCap :: runCap() {
    m_cap.read(m_frame);
}

// void VideoCap :: runHikCap() {
//     MVS_Cap.ReadImg(m_frame);
// }

cv::Mat VideoCap ::getCurrentImage() {
    return m_frame;
}
