//
// Created by xuqw on 17-9-11.
//

#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H

#include <nodelet/nodelet.h>
#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "fog_msgs/Features.h"
#include "fog_msgs/Keypoint.h"
#include "cam_model_omni.h"
#include "cSystem.h"
using namespace MultiColSLAM;
using namespace std;


namespace nodelet_feature_extraction
{
    class FeatureExtraction : public nodelet::Nodelet
    {
    public:
        FeatureExtraction(){}

    private:
        virtual void onInit();
        void Load_Calib(string path2calibrations, cCamModelGeneral_ &cam_model);
        void imageCallback(const sensor_msgs::ImageConstPtr& image);

        ros::Publisher pub;
        image_transport::Subscriber sub;
        cCamModelGeneral_ cam_model;
        mdBRIEFextractorOct* brief_extractor;
        string path2calibrations;
    };
}


#endif //FEATURE_EXTRACTION_H
