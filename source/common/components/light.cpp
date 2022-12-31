#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"


namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        type = data.value("type", LightType::DIRECTIONAL);
        diffuse = data.value("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        specular = data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f));
        ambient = data.value("ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        direction = data.value("direction", glm::vec3(0.0f, 0.0f, 0.0f));
        position = data.value("position", glm::vec3(0.0f, 0.0f, 0.0f));
        enabled = data.value("enabled", true);

        glm::vec3 attenuationTemp = data.value("attenuation", glm::vec3(0.0f, 0.0f, 0.0f));
        attenuation.constant = attenuationTemp.x;
        attenuation.linear = attenuationTemp.y;
        attenuation.quadratic = attenuationTemp.z;

        glm::vec2 spot_angleTemp = data.value("spot_angle", glm::vec2(0.0f, 0.0f));
        spot_angle.inner = spot_angleTemp.x;
        spot_angle.outer = spot_angleTemp.y;
    }
}