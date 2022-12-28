#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {


    class CollisionComponent : public Component {
    public:
        
        glm::vec3 boundingBox; // The bounding box of the object
        glm::vec3 lastPosition; // The last position of the object
        bool collider; // If the object is a collider

        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads the bounding box from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}