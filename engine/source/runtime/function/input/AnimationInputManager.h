#pragma once
#include "glm/fwd.hpp"
#include <map>
#include "glm/geometric.hpp"
#include "runtime/include/JsonManipulation.h"
#include "runtime/platform/path_utility/PathUtility.h"
#include "runtime/function/global/global_context.h"
#include "runtime/core/math/CoordinateMapper.h"
#include "runtime/core/math/Math.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vcruntime.h>
#include <vector>
namespace EasyEngine {
using PU = EasyEngine::PathUtility;

class AnimationInputManager{
public:
    AnimationInputManager(Camera &camera, const glm::vec2 &resolution, const float planeDistance):
    m_coordinateMapper(std::make_unique<CoordinateMapper>(camera,resolution,planeDistance) ) {
    };

    void readBoneLengthFromJson(const std::string& relativePath){
        JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),relativePath));
        Json::Value poseData = jsonManipulation.getJsonData();
        auto bonesInfo = poseData["part_len"];
        const auto keys = bonesInfo.getMemberNames();
        for (const auto& key : keys) {
            float value = bonesInfo[key].asFloat();
            m_boneLengthMap[key] = value;
        }
    }

    void readBoneDirectionFromJson(const std::string& relativePath){
        JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),relativePath));
        Json::Value boneDirectionData = jsonManipulation.getJsonData();
        const auto keys = boneDirectionData.getMemberNames();
        for (const auto& key : keys) {
            auto valueJson = boneDirectionData[key];
            float x = valueJson[0].asFloat();
            float y = valueJson[1].asFloat();
            float z = valueJson[2].asFloat();
            glm::vec3 dir = glm::normalize(glm::vec3(x,y,z));
            m_boneDirection[key] = dir;
        }
    }

    void readPoseFromJson(const std::string& relativePath){
        JsonManipulation jsonManipulation(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),relativePath));
        Json::Value poseData = jsonManipulation.getJsonData();
        auto keyPointsGroup = poseData["instance_info"];
        for (const auto & instanceInfo:keyPointsGroup) {
            const auto& keyPoints = instanceInfo["instances"][0]["keypoints"];
            std::cout<<keyPoints.isArray()<<std::endl;
            std::unordered_map<std::string, glm::vec2> mp;
            int keypointSize = keyPoints.size();
            for(int i = 0;i<keypointSize;i++){
                const auto ssx = keyPoints[i][0].asFloat();
                const auto ssy = keyPoints[i][1].asFloat();
                auto pos2D = glm::vec2(ssx,ssy);
                mp[jointID2Name[i]] = pos2D;
                // auto worldPosition = m_coordinateMapper->screen2world(ssx, ssy);
                // mp[id2Name[i]] = worldPosition;
            }
            // mp["lower_spine"] = Math::bilinearInterpolation(0.5,-100.0f,mp["left_hip"],mp["right_hip"],mp["left_shoulder"],mp["right_shoulder"]);
            // mp["middle_spine"] = Math::bilinearInterpolation(0.5,0.2f,mp["left_hip"],mp["right_hip"],mp["left_shoulder"],mp["right_shoulder"]);
            // mp["neck"] = Math::bilinearInterpolation(0.5,0.15f,mp["left_shoulder"],mp["right_shoulder"],mp["left_ear"],mp["right_ear"]);
            m_keypointScreenPositions.emplace_back(mp);
        }
    }
    
    void screenPosition2WorldPosition(){
        for (auto mpScreenPos:m_keypointScreenPositions) {
            std::unordered_map<std::string, glm::vec3> mpWorldPos;
            // for(auto&[boneName,imagePos]:mpScreenPos){
            //     auto worldPos = m_coordinateMapper->screen2world(imagePos.x, imagePos.y);
            //     mpWorldPos[boneName] = worldPos;
            // }
            auto middleShoulderPos2D = (mpScreenPos["left_shoulder"]+mpScreenPos["right_shoulder"])*0.5f;
            auto middleHipPose2D = (mpScreenPos["left_hip"]+mpScreenPos["right_hip"])*0.5f;
            //calculate fix "joint" world pos
            mpWorldPos["middle_shoulder"] = m_coordinateMapper->screen2world(middleShoulderPos2D.x, middleShoulderPos2D.y);
            mpWorldPos["middle_hip"] = m_coordinateMapper->screen2world(middleHipPose2D.x, middleHipPose2D.y);
            
            //calculate joint world pos recursively
            const float halfShoulderDistance = m_boneLengthMap["left_shoulder-right_shoulder"]/2;
            const float halfHipDistance = m_boneLengthMap["left_hip-right_hip"]/2;            
            
            mpWorldPos["left_shoulder"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["middle_shoulder"],mpScreenPos["left_shoulder"],halfShoulderDistance);
            mpWorldPos["right_shoulder"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["middle_shoulder"],mpScreenPos["right_shoulder"],halfShoulderDistance);
            mpWorldPos["left_hip"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["middle_hip"],mpScreenPos["left_hip"],halfHipDistance);
            mpWorldPos["right_hip"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["middle_hip"],mpScreenPos["right_hip"],halfHipDistance);
            
            mpWorldPos["left_elbow"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["left_shoulder"],mpScreenPos["left_elbow"],m_boneLengthMap["left_elbow-left_shoulder"]);
            mpWorldPos["right_elbow"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["right_shoulder"],mpScreenPos["right_elbow"],m_boneLengthMap["left_elbow-left_shoulder"]);
            mpWorldPos["left_knee"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["left_hip"],mpScreenPos["left_knee"],m_boneLengthMap["left_hip-left_knee"]);
            mpWorldPos["right_knee"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["right_hip"],mpScreenPos["right_knee"],m_boneLengthMap["left_hip-left_knee"]);

            mpWorldPos["left_wrist"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["left_elbow"],mpScreenPos["left_wrist"],m_boneLengthMap["left_wrist-left_elbow"]);
            mpWorldPos["right_wrist"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["right_elbow"],mpScreenPos["right_wrist"],m_boneLengthMap["left_wrist-left_elbow"]);
            mpWorldPos["left_ankle"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["left_knee"],mpScreenPos["left_ankle"],m_boneLengthMap["left_knee-left_ankle"]);
            mpWorldPos["right_ankle"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["right_knee"],mpScreenPos["right_ankle"],m_boneLengthMap["left_knee-left_ankle"]);

            mpWorldPos["nose"] = m_coordinateMapper->screen2WorldFromRefPos(mpWorldPos["middle_shoulder"],mpScreenPos["nose"],m_boneLengthMap["nose-neck"]);
            //interpote missing joint
            mpWorldPos["lower_spine"] = Math::bilinearInterpolation(0.5,0.0f,mpWorldPos["left_hip"],mpWorldPos["right_hip"],mpWorldPos["left_shoulder"],mpWorldPos["right_shoulder"]);
            mpWorldPos["middle_spine"] = Math::bilinearInterpolation(0.5,0.2f,mpWorldPos["left_hip"],mpWorldPos["right_hip"],mpWorldPos["left_shoulder"],mpWorldPos["right_shoulder"]);
            mpWorldPos["neck"] = glm::mix(mpWorldPos["middle_shoulder"],mpWorldPos["nose"],0.2f);
            // mpWorldPos["middle_hip"] = (mpWorldPos["left_hip"] + mpWorldPos["right_hip"])*0.5f+glm::vec3(0,-1020.0f,0);
            mpWorldPos["hip_R_up"] = mpWorldPos["left_hip"];
            mpWorldPos["hip_L_up"] = mpWorldPos["right_hip"];
            // mpWorldPos["neck"] = Math::bilinearInterpolation(0.5,0.15f,mpWorldPos["left_shoulder"],mpWorldPos["right_shoulder"],mpWorldPos["left_ear"],mpWorldPos["right_ear"]);
            m_keypointWorldPositions.emplace_back(mpWorldPos);
        }
    }

    glm::vec3 getKeypoint(int frameIndex,std::string boneName){
        return m_keypointWorldPositions[frameIndex][boneName];
    }

    const size_t getFrameCount(){
        return m_keypointWorldPositions.size();
    }

    glm::vec3 getRefDirectionByBoneName(const std::string& boneName){
        glm::vec3 dir = m_boneDirection[boneName];
        return dir;
    }

private:
    std::vector<std::unordered_map<std::string, glm::vec3>> m_keypointWorldPositions;
    std::vector<std::unordered_map<std::string, glm::vec2>> m_keypointScreenPositions;
    std::unordered_map<std::string, glm::vec3> m_boneDirection;
    std::unordered_map<std::string, float> m_boneLengthMap;

    std::unique_ptr<CoordinateMapper> m_coordinateMapper;
    const std::vector<std::string> jointID2Name = {"nose","left_eye","right_eye","left_ear",
    "right_ear","left_shoulder","right_shoulder","left_elbow","right_elbow","left_wrist",
    "right_wrist","left_hip","right_hip","left_knee","right_knee","left_ankle","right_ankle","neck"};
    const std::vector<std::string> boneLengthID2Name = {"nose-neck","left_wrist-left_elbow","left_elbow-left_shoulder","left_shoulder-right_shoulder",
    "left_shoulder-left_hip","left_hip-left_knee","left_knee-left_ankle","left_hip-right_hip"};
    
};
}