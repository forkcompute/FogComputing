//
// Created by xuqw on 17-9-11.
//

#ifndef FEATURE_ANALYSIS_H
#define FEATURE_ANALYSIS_H

#include <nodelet/nodelet.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <vector>

#include "fog_msgs/Features.h"
#include "cTracking.h"
#include "cConverter.h"
#include "cam_model_omni.h"
#include "cSystem.h"

namespace nodelet_feature_analysis
{
    class FeatureAnalysis : public nodelet::Nodelet
    {
    public:
        FeatureAnalysis(){
        }

    private:
        virtual void onInit();
        void featureCallback(const fog_msgs::FeaturesConstPtr & msg);

        ros::Subscriber sub;
        string path2voc;
        string path2settings;
        string path2calibrations;
        MultiColSLAM::cSystem* MultiSLAM;
        cv::Mat mDescriptors;
        cv::Mat mDescriptorMasks;
        vector<cv::KeyPoint> keypoints;
    };
}

#endif //FEATURE_ANALYSIS_H
