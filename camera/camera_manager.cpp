#include <limbo/setup.hpp>
#include <common/controls.hpp>

class CameraManager {
    glm::vec3 mSubjectPosition; // TODO: allow things not the origin
    float mRho, mPhi, mTheta; // For the vector from the subject to the camera
public:
    CameraManager() {
        mSubjectPosition = glm::vec3( 0, 0, 0 );
        mRho = 5;
        mPhi = 3.14f/2;
        mTheta = 0;
    }
    
    void change_camera_perspective(const float x, const float y) {
        const float veritcal_limit = 0.1;
        mTheta += x;
        mPhi = std::min(3.1416f-veritcal_limit, std::max(veritcal_limit, mPhi+y));
    }

    glm::vec3 getCameraPosition() {
        const glm::vec3 cameraDisplacementVector(mRho * cos(mTheta) * sin(mPhi),
                                                 mRho * sin(mTheta) * sin(mPhi),
                                                 mRho * cos(mPhi));
        return mSubjectPosition + cameraDisplacementVector;
    }
    
    glm::mat4 getViewMatrix() {
        // Setup a camera, look at the origin
        const glm::vec3 cameraPosition = getCameraPosition();
        const glm::vec3 upDirection(0,1,0);
        return glm::lookAt(cameraPosition, mSubjectPosition, upDirection);
    }
    
    glm::mat4 getProjectionMatrix() {
        // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        return glm::perspective(glm::radians(45.0f), 4.f/3, 0.1f, 100.0f);
    }

};
