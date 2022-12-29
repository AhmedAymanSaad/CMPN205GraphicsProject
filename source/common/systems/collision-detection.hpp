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
        void initialize(World *world)
        {
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                CollisionComponent *collision = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if (collision == nullptr)
                    continue;
                if (collision->collider)
                {
                    Entity *parent = entity;
                    while (parent->parent != nullptr)
                    {
                        parent = parent->parent;
                    }
                    collision->lastPosition = parent->localTransform.position;
                }
            }
        }
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
                if (collision->collider)
                {
                    Entity *parent = entity;
                    collision->collided = glm::vec3(0, 0, 0);
                    while (parent->parent != nullptr)
                    {
                        parent = parent->parent;
                    }
                    detectCollision(collision, world);
                    glm::vec3 collisionDirection = collision->collided;
                    if (collisionDirection.x != 0 || collisionDirection.y != 0 || collisionDirection.z != 0)
                    {
                        // std::cout << "Collision" << std::endl;
                        // std::cout << "Current position: " << entity->localTransform.position.x << ", " << entity->localTransform.position.y << ", " << entity->localTransform.position.z << std::endl;
                        // std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
                        std::cout << "Collision Direction " << collisionDirection.x << ", " << collisionDirection.y << ", " << collisionDirection.z << std::endl;
                        std::cout << "Current position: " << parent->localTransform.position.x << ", " << parent->localTransform.position.y << ", " << parent->localTransform.position.z << std::endl;
                        std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
                        // parent->localTransform.position = collision->lastPosition;

                        if (collisionDirection.x == 1)
                        {
                            parent->localTransform.position.x = collision->lastPosition.x;
                            parent->localTransform.position.z = collision->lastPosition.z;
                        }
                        if (collisionDirection.y == 1)
                        {
                            parent->localTransform.position.y = collision->lastPosition.y;
                        }
                        if (collisionDirection.z == 1)
                        {
                            parent->localTransform.position.y = collision->lastPosition.y;
                        }
                    }
                    else
                    {
                        collision->lastPosition = parent->localTransform.position;
                    }
                }
            }
        }

        void detectCollision(CollisionComponent *collision, World *world)
        {
            Entity *colliderEntity = collision->getOwner();
            glm::mat4 colliderEntityPosition = colliderEntity->getLocalToWorldMatrix();

            for (auto entity : world->getEntities())

            {
                CollisionComponent *collidedWith = entity->getComponent<CollisionComponent>();
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

                            if (collidedWith->ground ){
                                collision->collided.y = 1;
                                continue;
                            }

                          
                            // get the center of each face
                            glm::vec3 collidedWithCenter = glm::vec3(collidedWithPosition[3].x, collidedWithPosition[3].y, collidedWithPosition[3].z);
                            
                            // get the 6 faces of the collider
                            // 1 = right, 2 = left, 3 = top, 4 = bottom, 5 = front, 6 = back
                            glm::vec3 colliderFace1 = glm::vec3(colliderEntityPosition[3].x + collision->boundingBox.x, colliderEntityPosition[3].y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace2 = glm::vec3(colliderEntityPosition[3].x - collision->boundingBox.x, colliderEntityPosition[3].y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace3 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y + collision->boundingBox.y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace4 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y - collision->boundingBox.y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace5 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y, colliderEntityPosition[3].z + collision->boundingBox.z);
                            glm::vec3 colliderFace6 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y, colliderEntityPosition[3].z - collision->boundingBox.z);

                            // get the distance between the center of the collidedWith and each face of the collider
                            float distance1 = glm::distance(collidedWithCenter, colliderFace1);
                            float distance2 = glm::distance(collidedWithCenter, colliderFace2);
                            float distance3 = glm::distance(collidedWithCenter, colliderFace3);
                            float distance4 = glm::distance(collidedWithCenter, colliderFace4);
                            float distance5 = glm::distance(collidedWithCenter, colliderFace5);
                            float distance6 = glm::distance(collidedWithCenter, colliderFace6);

                            // get the minimum distance
                            float minDistance = std::min({distance1, distance2, distance3, distance4, distance5, distance6});

                            // get the direction of the collision
                            if (minDistance == distance1)
                            {
                                collision->collided.x = 1;
                            }
                            if (minDistance == distance2)
                            {
                                collision->collided.x = 1;
                            }
                            if (minDistance == distance3)
                            {
                                collision->collided.z = 1;
                            }
                            if (minDistance == distance4)
                            {
                                collision->collided.y = 1;
                            }
                            if (minDistance == distance5)
                            {
                                collision->collided.x = 1;
                            }
                            if (minDistance == distance6)
                            {
                                collision->collided.x = 1;
                            }


                        }
                        
                    }
                }
            }
            return;
        }
    };

}
