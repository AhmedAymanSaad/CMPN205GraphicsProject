#include "collision.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    // Reads the bounding box from the given json object
    void CollisionComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;
        boundingBox = data.value ("boundingBox", glm::vec3(0.0f, 0.0f, 0.0f));
        collider = data.value ("collider", false);
        ground = data.value ("ground", false);
        coin = data.value ("coin", false);
    }

}