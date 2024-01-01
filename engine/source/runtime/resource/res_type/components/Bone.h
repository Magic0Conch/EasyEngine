#include "glm/fwd.hpp"
#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

namespace EasyEngine {
struct KeyPosition{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale{
    glm::vec3 scale;
    float timeStamp;
};


class Bone{
private:
    std::vector<KeyPosition> m_positions;
    std::vector<KeyRotation> m_rotations;
    std::vector<KeyScale> m_scales;

    int m_numPositions;
    int m_numRotations;
    int m_numScalings;

    glm::mat4 m_localTransform;
    std::string m_name;
    int m_id;
public:
    Bone(const std::string& name,int ID,const aiNodeAnim* channel);
    Bone(const std::string& name,int ID);
    void update(float animationTime,glm::mat4* translation = nullptr,glm::mat4* rotation = nullptr,glm::mat4* scale = nullptr);
    
    glm::mat4 getLocalTransform();
    std::string getBoneName() const;
    int getBoneID();
    int seekPositionIndex(float animationTime);
    int seekRotationIndex(float animationTime);
    int seekScaleIndex(float animationTime);
    void addKeyPosition(const glm::vec3& position,float timeStamp);
    void addKeyRotation(const glm::quat& rotation,float timeStamp);
    void addKeyScaling(const glm::vec3& scale,float timeStamp);
private:
    float getScaleFactor(float lastTimeStamp,float nextTimeStamp,float animationTime);
    glm::mat4 interpolatePosition(float animationTime);
    glm::mat4 interpolateRotation(float animationTime);
    glm::mat4 interpolateScaling(float animationTime);
};
}