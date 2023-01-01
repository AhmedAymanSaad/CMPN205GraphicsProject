#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the PlayerControllerComponent will move the owning entity using user inputs.
    // the child of the player would the camera to make a 3rd person view
    // we did that so that we can have the option to change camera view without changing the player position
    class PlayerControllerComponent : public Component {
    public:
        // The senstivity paramter defined sensitive the camera rotation & fov is to the mouse moves and wheel scrolling
        float rotationSensitivity = 0.01f; // The angle change per pixel of mouse movement
        float fovSensitivity = 0.3f; // The fov angle change per unit of mouse wheel scrolling
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; // The unity per second of camera movement if WASD is pressed
        float speedupFactor = 5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.

        int coinsCollected = 0; // The number of coins collected - for game logic

        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Player Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}