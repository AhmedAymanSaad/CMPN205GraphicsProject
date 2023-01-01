#include "player-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    // where data.value("x", x) means:
    // if json data file has a key called "x", then use its value, otherwise use the default value
    // saved as the second parameter
    void PlayerControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        rotationSensitivity = data.value("rotationSensitivity", rotationSensitivity);
        fovSensitivity = data.value("fovSensitivity", fovSensitivity);
        positionSensitivity = data.value("positionSensitivity", positionSensitivity);
        speedupFactor = data.value("speedupFactor", speedupFactor);
    }
}