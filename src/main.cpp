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
    int times = 0;
    double sum = 0;
    clock_t start, finish;
    VideoCap cap;
    cv::Mat frame, binary, frame1;
    ArmorTracker trackArmor;
    while (1) {
        bool isDetected = false;
#ifdef CLOCK
        start = clock();
#endif
        cap.algorithmInterface();
        frame = cap.getCurrentImage();
        //resize(frame1, frame, frame.size(), 0.5, 0.5);
        frame.copyTo(binary);       //展示效果
        frame.copyTo(frame1);

        FindArmorFactory findArmor(frame, binary);
        armors finalarmor = findArmor.getFinalArmor();
        std::vector<armors> Armors = findArmor.getArmors();
        if (!Armors.empty()) {
            isDetected = true;
        }
#ifdef PREDICT
        trackArmor.track(finalarmor, isDetected, binary);//追踪器
		// if(armors.size()!=0)
        // m_k.predict(finalarmor,binary);
#endif

        if(!Armors.empty()){
            SOLVEPNP pnp;
            pnp.caculate(finalarmor);
            // if(waitKey(1) >= 0) break;
        }
#ifdef IMSHOW
        imshow("okey", binary);
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