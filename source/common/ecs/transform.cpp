#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        //Creating scale, rotation, translation 4x4 matrices, though we only need 3x3
        //for scaling and rotating, but it's more convenient to use 4x4 since we will multiply it by translation matrix
        //which has to be a 4x4 matrix
        glm::mat4 scale_matrix = glm::mat4(1.0f);
        glm::mat4 rotation_matrix = glm::mat4(1.0f);
        glm::mat4 translation_matrix = glm::mat4(1.0f);

        //function scales the matrix by the given vector
        scale_matrix = glm::scale(scale_matrix, scale);
        rotation_matrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        translation_matrix = glm::translate(translation_matrix, position);

        // order of transformations is: Scaling, Rotation then Translation
        return translation_matrix * rotation_matrix * scale_matrix;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}