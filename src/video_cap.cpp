//
// Created by JYSimilar on 2022/11/3.
//
// todo: 加注释

#include "../include/video_cap.h"


VideoCap :: VideoCap() {
    if (!m_filePath.empty()) {
        m_cap.open(m_filePath);
    } else {
        m_cap.open(m_capIndex);
    }
}

void VideoCap :: algorithmInterface() {
    m_cap.read(m_frame);
}

cv::Mat VideoCap ::getCurrentImage() {
    return m_frame;
}
