//
// Created by xuqw on 17-9-11.
//
#include <pluginlib/class_list_macros.h>
#include "feature_extraction.h"

PLUGINLIB_DECLARE_CLASS(nodelet_feature_extraction, FeatureExtraction, nodelet_feature_extraction::FeatureExtraction, nodelet::Nodelet);

namespace nodelet_feature_extraction
{
    void FeatureExtraction::onInit() {
        ros::NodeHandle& private_nh = getPrivateNodeHandle();
        pub = private_nh.advertise<fog_msgs::Features>("features", 1);
        image_transport::ImageTransport it(private_nh);
        sub = it.subscribe("/camera/image", 1, &FeatureExtraction::imageCallback, this);
        path2calibrations = string("/home/xuqw/slam_ws/src/monocular/cfg/");
        Load_Calib(path2calibrations, cam_model);
        brief_extractor = new mdBRIEFextractorOct(400, 1.2, 8, 25, 0, 0, 32, 20, false, 2, false, false, 32);
    }

    void FeatureExtraction::imageCallback(const sensor_msgs::ImageConstPtr &image) {
        ROS_INFO("I receive image");
        cv::Mat img= cv_bridge::toCvShare(image, "bgr8")->image;
        cv::Mat grey_img;
        cv::cvtColor(img, grey_img, cv::COLOR_BGR2GRAY);
        int mn_min_x = 0;
        int mn_max_x = cam_model.GetWidth();
        int mn_min_y = 0;
        int mn_max_y = cam_model.GetHeight();
        std::vector<cv::KeyPoint> key_points_temp;
        cv::Mat m_descriptors;
        cv::Mat m_descriptor_masks;
        (*brief_extractor)(grey_img,
                           cam_model.GetMirrorMask(0),
                           key_points_temp,
                           cam_model,
                           m_descriptors,
                           m_descriptor_masks);
        fog_msgs::Features features;
        features.keypoints.reserve(key_points_temp.size());
        for(size_t i = 0; i < key_points_temp.size(); ++i)
        {
            fog_msgs::Keypoint keypoint;
            keypoint.angle = key_points_temp.at(i).angle;
            keypoint.class_id = key_points_temp.at(i).class_id;
            keypoint.octave = key_points_temp.at(i).octave;
            keypoint.ptx = key_points_temp.at(i).pt.x;
            keypoint.pty = key_points_temp.at(i).pt.y;
            keypoint.response = key_points_temp.at(i).response;
            keypoint.size = key_points_temp.at(i).size;
            features.keypoints.push_back(keypoint);
        }
        features.descriptor.descriptors = *(cv_bridge::CvImage(std_msgs::Header(), "mono8", m_descriptors).toImageMsg());
        features.descriptor.masks = *(cv_bridge::CvImage(std_msgs::Header(), "mono8", m_descriptor_masks).toImageMsg());
        pub.publish(features);
    }

    void FeatureExtraction::Load_Calib(string path2calibrations, cCamModelGeneral_ &cam_model) {
        string mcs_settings = path2calibrations + "/MultiCamSys_Calibration.yaml";
        cv::FileStorage mcs_calib_data(mcs_settings, cv::FileStorage::READ);
        int nrCams = (int)mcs_calib_data["CameraSystem.nrCams"];
        cv::Matx44d M_c_s;
        cCamModelGeneral_ camModels;
        // all M_c
        cv::Matx61d tmp;
        for (int p = 1; p < 7; ++p)
        {
            string param = "CameraSystem.cam" + to_string(1) + "_" + to_string(p);
            tmp(p - 1) = mcs_calib_data[param];
        }
        M_c_s = cayley2hom<double>(tmp);

        // Interior orientation
        string calib_data = path2calibrations + "/InteriorOrientationFisheye" + to_string(0) + ".yaml";
        cv::FileStorage fSettings(calib_data, cv::FileStorage::READ);
        int nrpol = (int)fSettings["Camera.nrpol"];
        int nrinvpol = (int)fSettings["Camera.nrinvpol"];
        cv::Mat_<double> poly = cv::Mat::zeros(5, 1, CV_64F);
        for (int i = 0; i < nrpol; ++i)
            poly.at<double>(i, 0) = fSettings["Camera.a" + to_string(i)];
        cv::Mat_<double>  invpoly = cv::Mat::zeros(12, 1, CV_64F);
        for (int i = 0; i < nrinvpol; ++i)
            invpoly.at<double>(i, 0) = fSettings["Camera.pol" + to_string(i)];

        int Iw = (int)fSettings["Camera.Iw"];
        int Ih = (int)fSettings["Camera.Ih"];

        double cdeu0v0[5] = { fSettings["Camera.c"], fSettings["Camera.d"], fSettings["Camera.e"],
                              fSettings["Camera.u0"], fSettings["Camera.v0"] };

        cCamModelGeneral_ camModel = cCamModelGeneral_(cdeu0v0, poly, invpoly, Iw, Ih);
        vector<cv::Mat> mirrorMasks;

        int createMirrorMasks = (int)fSettings["Camera.mirrorMask"];
        if (createMirrorMasks == 1)
            CreateMirrorMask(camModel, 4, mirrorMasks);
        else
            mirrorMasks.push_back(cv::Mat::ones(cv::Size(Iw, Ih), CV_8UC1));

        camModel.SetMirrorMasks(mirrorMasks);

        camModel.SetMirrorMasks(mirrorMasks);
        cam_model = camModel;
    }
}