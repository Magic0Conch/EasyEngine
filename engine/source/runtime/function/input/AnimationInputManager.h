#pragma once
#include "glm/fwd.hpp"
#include <map>
#include "runtime/include/JsonManipulation.h"
#include "runtime/platform/path_utility/PathUtility.h"
#include "runtime/function/global/global_context.h"
#include "runtime/core/math/CoordinateMapper.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace EasyEngine {
using PU = EasyEngine::PathUtility;

class AnimationInputManager{
public:
    AnimationInputManager(Camera &camera, const glm::vec2 &resolution, const float planeDistance):
    m_coordinateMapper(std::make_unique<CoordinateMapper>(camera,resolution,planeDistance) ) {
    };

    void readPoseFromJson(const std::string& relativePath){
        JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),relativePath));
        Json::Value poseData = jsonManipulation.getJsonData();
        auto keyPointsGroup = poseData["instance_info"];
        int poseLen = keyPointsGroup.size();
        for (const auto & instanceInfo:keyPointsGroup) {
            const auto& keyPoints = instanceInfo["instances"][0]["keypoints"];
            std::cout<<keyPoints.isArray()<<std::endl;
            std::unordered_map<std::string, glm::vec3> mp;
            int keypointSize = keyPoints.size();
            for(int i = 0;i<keypointSize;i++){
                const auto ssx = keyPoints[i][0].asFloat();
                const auto ssy = keyPoints[i][1].asFloat();
                auto worldPosition = m_coordinateMapper->screen2world(ssx, ssy);
                mp[id2Name[i]] = worldPosition;
            }
            mp["neck"] = (mp["left_shoulder"] + mp["right_shoulder"])*0.5f;
            m_keypointWorldPositions.emplace_back(mp);
        }
    }

    glm::vec3 getKeypoint(int frameIndex,std::string boneName){
        return m_keypointWorldPositions[frameIndex][boneName];
    }

    const int getFrameCount(){
        return m_keypointWorldPositions.size();
    }
private:
    std::vector<std::unordered_map<std::string, glm::vec3>> m_keypointWorldPositions;
    std::vector<glm::vec3> m_worldPositon;
    std::unique_ptr<CoordinateMapper> m_coordinateMapper;
    const std::vector<std::string> id2Name = {"nose","left_eye","right_eye","left_ear",
    "right_ear","left_shoulder","right_shoulder","left_elbow","right_elbow","left_wrist",
    "right_wrist","left_hip","right_hip","left_knee","right_knee","left_ankle","right_ankle","neck"};
};
}