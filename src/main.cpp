// todo: 加注释

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#include <iostream>
#include <vector>
#include "../define/define.h"
#include "../include/video_cap.h"
#include "../include/armor_detector.h"
#include "../include/SolvePnP.h"
#include "../include/find_armor_factory.h"
#include "../include/armor_tracker.h"

int main() {
#ifdef NX
    Serial uart(BR115200, WORDLENGTH_8B, STOPBITS_1, PARITY_NONE, HWCONTROL_NONE);
    uart.sOpen("/dev/ttyTHS2");
#endif
    int times = 0;
    double sum = 0;
    clock_t start, finish;
    VideoCap cap;
    cv::Mat frame, originFrame, frame1;
    ArmorTracker trackArmor;
    while (1) {
        bool isDetected = false;
#ifdef CLOCK
        start = clock();
#endif
        cap.algorithmInterface();
        frame = cap.getCurrentImage();
        // frame = imread("D:\\QQ\\1023342887\\FileRecv\\car.jpg");
        //resize(frame1, frame, frame.size(), 0.5, 0.5);
        frame.copyTo(originFrame);       //展示效果
        frame.copyTo(frame1);

        FindArmorFactory findArmor(frame, originFrame);
        armors finalarmor = findArmor.getFinalArmor();
        std::vector<armors> Armors = findArmor.getArmors();
        if (!Armors.empty()) {
            isDetected = true;
        }
#ifdef PREDICT
        trackArmor.track(finalarmor, isDetected, frame, originFrame);//追踪器
		// if(armors.size()!=0)
        // m_k.predict(finalarmor,originFrame);
#endif

        if(!Armors.empty()){
            SOLVEPNP pnp;
            pnp.caculate(finalarmor);
            // if(waitKey(1) >= 0) break;
        }
#ifdef IMSHOW
        imshow("okey", originFrame);
#endif
#ifdef CLOCK
        finish = clock();
            double totalTime = (double)(finish - start) / CLOCKS_PER_SEC;
            sum += totalTime;
            times += 1;
            if (sum > 1)
            {
                std::cout <<"times"<< times << std::endl;
                times = 0;
                sum = 0;
            }
            std::cout << "Time" << totalTime << std::endl;
#endif

        cv::waitKey(30);
    }
    return 0;
}

#pragma clang diagnostic pop