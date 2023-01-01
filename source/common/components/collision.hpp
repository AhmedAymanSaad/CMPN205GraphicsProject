#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {


    class CollisionComponent : public Component {
    public:
        
        glm::vec3 boundingBox; // The bounding box of the object
        glm::vec3 lastPosition; // The last position of the object
        bool collider; // If the object is a collider
        glm::vec3 collided = glm::vec3(0,0,0) ;// If the object has collided
        bool ground = false; // If the object the ground
        bool coin = false; // If the object is a coin (used for the coin counter and the coin removal)

        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads the bounding box from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}