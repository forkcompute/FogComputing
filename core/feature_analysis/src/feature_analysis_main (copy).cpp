#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <vector>

#include "messages/Descriptor.h"
#include "cTracking.h"
#include "cConverter.h"
#include "cam_model_omni.h"
#include "cSystem.h"

string path2voc = string("/home/xuqw/slam_ws/src/monocular/cfg/small_orb_omni_voc_9_6.yml");
string path2settings = string("/home/xuqw/slam_ws/src/monocular/cfg/Slam_Settings_indoor1.yaml");
string path2calibrations = string("/home/xuqw/slam_ws/src/monocular/cfg/");

MultiColSLAM::cSystem MultiSLAM(path2voc, path2settings, path2calibrations, true); // It will be initialized when the program begins
image_transport::Publisher pub;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try{
        cv::Mat img = cv_bridge::toCvShare(msg, "bgr8")->image;
        double tframe = (double) msg->header.stamp.toSec();
        
        cv::Mat flipImg;
        cv::flip(img, flipImg, 0);
        sensor_msgs::ImagePtr pubMsg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", flipImg).toImageMsg();
        pub.publish(pubMsg);
        
        cv::Mat flipGrey;
        cv::cvtColor(flipImg, flipGrey, cv::COLOR_BGR2GRAY);
        
        std::vector<cv::Mat> trackimgs;
        trackimgs.push_back(flipGrey);
        MultiSLAM.TrackMultiColSLAM(trackimgs, tframe);
        cout << endl;
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "monocular_slam");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
    pub = it.advertise("flip_image", 1);
    ros::spin();
}

