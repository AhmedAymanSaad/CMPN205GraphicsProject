#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    class CollisionDetectionSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            CollisionComponent *collision = nullptr;
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                collision = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if (collision == nullptr)
                    continue;
                if ( collision->collider)
                {
                    Entity * parent = entity;
                    while (parent->parent != nullptr)
                    {
                        parent = parent->parent;
                    }
                    if (detectCollision(collision, world))
                    {
                        // std::cout << "Collision" << std::endl;
                        // std::cout << "Current position: " << entity->localTransform.position.x << ", " << entity->localTransform.position.y << ", " << entity->localTransform.position.z << std::endl;
                        // std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
                        std::cout << "Collision" << std::endl;
                        std::cout << "Current position: " << parent->localTransform.position.x << ", " << parent->localTransform.position.y << ", " << parent->localTransform.position.z << std::endl;
                        std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
                        parent->localTransform.position = collision->lastPosition;

                    }
                    else
                    {
                        collision->lastPosition = parent->localTransform.position;
                    }
                }
            }
        }

        bool detectCollision(CollisionComponent *collision, World *world)
        {
            Entity * colliderEntity = collision->getOwner();
            glm::mat4 colliderEntityPosition = colliderEntity->getLocalToWorldMatrix();
            for (auto entity : world->getEntities())
            {
                CollisionComponent* collidedWith = entity->getComponent<CollisionComponent>();
                glm::mat4 collidedWithPosition = entity->getLocalToWorldMatrix();
                if (collidedWith)
                {
                    if (collidedWith != collision)
                    {
                        if (colliderEntityPosition[3].x + collision->boundingBox.x > collidedWithPosition[3].x - collidedWith->boundingBox.x &&
                            colliderEntityPosition[3].x - collision->boundingBox.x < collidedWithPosition[3].x + collidedWith->boundingBox.x &&
                            colliderEntityPosition[3].y + collision->boundingBox.y > collidedWithPosition[3].y - collidedWith->boundingBox.y &&
                            colliderEntityPosition[3].y - collision->boundingBox.y < collidedWithPosition[3].y + collidedWith->boundingBox.y &&
                            colliderEntityPosition[3].z + collision->boundingBox.z > collidedWithPosition[3].z - collidedWith->boundingBox.z &&
                            colliderEntityPosition[3].z - collision->boundingBox.z < collidedWithPosition[3].z + collidedWith->boundingBox.z)
                        {
                            return true;
                        }
                        
                    
                    }
                }
            }
            return false;
        }
    };

}