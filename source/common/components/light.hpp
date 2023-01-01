#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

    // An enum that defines the type of the camera (ORTHOGRAPHIC or PERSPECTIVE) 



namespace our {

    // enum to identify the type of the light
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component {
    public:

    // Here we define our light. First member specifies its type.
    LightType type;
    // We also define the color & intensity of the light for each component of the Phong model (Ambient, Diffuse, Specular).
    glm::vec3 diffuse, specular, ambient;
    glm::vec3 direction; // Used for Directional and Spot Lights only
    glm::vec3 position; // Used for Point and Spot Lights only
    bool enabled;       // case enabled is false, the light is not rendered, (paused this feature for now)

    // struct to define attenuation in order to simulate the real world light attenuation.
    //Used for Point and Spot Lights only
    struct {
        float constant, linear, quadratic;
    } attenuation;
    
    // This specifies the inner and outer cone of the spot light.
    // The light power is 0 outside the outer cone, the light power is full inside the inner cone.
    // The light power is interpolated in between the inner and outer cone.
    struct {
        float inner, outer;
    } spot_angle; // Used for Spot Lights only

        // light.type = LightType::DIRECTIONAL;
        // light.diffuse = {1,1,1};
        // light.specular = {1,1,1};
        // light.ambient = {0.1f, 0.1f, 0.1f};
        // light.direction = {-1, -1, -1};
        // light.position = {0, 1, 2};
        // light.attenuation = {0, 0, 1};
        // light.spot_angle = {glm::pi<float>()/4, glm::pi<float>()/2};

        // The ID of this component type is "LightComponent"
        static std::string getID() { return "Light"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}