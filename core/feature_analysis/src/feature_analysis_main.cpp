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

string path2voc = string("/home/xuqw/slam_ws/src/monocular/cfg/small_orb_omni_voc_9_6.yml");
string path2settings = string("/home/xuqw/slam_ws/src/monocular/cfg/Slam_Settings_indoor1.yaml");
string path2calibrations = string("/home/xuqw/slam_ws/src/monocular/cfg/");

MultiColSLAM::cSystem MultiSLAM(path2voc, path2settings, path2calibrations, true); // It will be initialized when the program begins
cv::Mat mDescriptors;
cv::Mat mDescriptorMasks;
vector<cv::KeyPoint> keypoints;
void featureCallback(const fog_msgs::FeaturesConstPtr & msg)
{
    keypoints.resize(msg->keypoints.size());
    for(size_t i = 0; i < msg->keypoints.size(); i++){
        keypoints.at(i).angle = msg->keypoints.at(i).angle;
        keypoints.at(i).class_id = msg->keypoints.at(i).class_id;
        keypoints.at(i).octave = msg->keypoints.at(i).octave;
        keypoints.at(i).pt.x = msg->keypoints.at(i).ptx;
        keypoints.at(i).pt.y = msg->keypoints.at(i).pty;
        keypoints.at(i).response = msg->keypoints.at(i).response;
        keypoints.at(i).size = msg->keypoints.at(i).size;
    }
    mDescriptors = cv_bridge::toCvCopy(msg->descriptor.descriptors, "mono8")->image;
    mDescriptorMasks = cv_bridge::toCvCopy(msg->descriptor.masks, "mono8")->image;
    MultiSLAM.TrackMultiColSLAM(mDescriptors, mDescriptorMasks, keypoints, ros::Time::now().sec);
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "feature_analysis");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("features", 1, featureCallback);
    ros::Rate rate(10);

    ros::spin();
}

