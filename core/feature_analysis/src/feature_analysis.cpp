//
// Created by xuqw on 17-9-11.
//
#include <pluginlib/class_list_macros.h>
#include "feature_analysis.h"

PLUGINLIB_DECLARE_CLASS(nodelet_feature_analysis, FeatureAnalysis, nodelet_feature_analysis::FeatureAnalysis, nodelet::Nodelet);

namespace nodelet_feature_analysis
{
    void FeatureAnalysis::onInit() {
        ros::NodeHandle& private_nh = getPrivateNodeHandle();
        sub = private_nh.subscribe("features", 1, &FeatureAnalysis::featureCallback, this);
        path2voc = string("/home/xuqw/slam_ws/src/monocular/cfg/small_orb_omni_voc_9_6.yml");
        path2settings = string("/home/xuqw/slam_ws/src/monocular/cfg/Slam_Settings_indoor1.yaml");
        path2calibrations = string("/home/xuqw/slam_ws/src/monocular/cfg/");
        MultiSLAM = new MultiColSLAM::cSystem(path2voc, path2settings, path2calibrations, true);
    }

    void FeatureAnalysis::featureCallback(const fog_msgs::FeaturesConstPtr &msg) {
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
        MultiSLAM->TrackMultiColSLAM(mDescriptors, mDescriptorMasks, keypoints, ros::Time::now().sec);
    }
}