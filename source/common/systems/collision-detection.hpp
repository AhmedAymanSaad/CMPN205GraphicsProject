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
                        std::cout << "Collision Direction " << collisionDirection.x << ", " << collisionDirection.y << ", " << collisionDirection.z << std::endl;
                        std::cout << "Current position: " << parent->localTransform.position.x << ", " << parent->localTransform.position.y << ", " << parent->localTransform.position.z << std::endl;
                        std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
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
                    collision->lastPosition = parent->localTransform.position;
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

                          
                            // 1 = right, 2 = left, 3 = top, 4 = bottom, 5 = front, 6 = back
                            glm::vec3 collidedWithFace1 = glm::vec3(collidedWithPosition[3].x + collidedWith->boundingBox.x, collidedWithPosition[3].y, collidedWithPosition[3].z);
                            glm::vec3 collidedWithFace2 = glm::vec3(collidedWithPosition[3].x - collidedWith->boundingBox.x, collidedWithPosition[3].y, collidedWithPosition[3].z);
                            glm::vec3 collidedWithFace3 = glm::vec3(collidedWithPosition[3].x, collidedWithPosition[3].y + collidedWith->boundingBox.y, collidedWithPosition[3].z);
                            glm::vec3 collidedWithFace4 = glm::vec3(collidedWithPosition[3].x, collidedWithPosition[3].y - collidedWith->boundingBox.y, collidedWithPosition[3].z);
                            glm::vec3 collidedWithFace5 = glm::vec3(collidedWithPosition[3].x, collidedWithPosition[3].y, collidedWithPosition[3].z + collidedWith->boundingBox.z);
                            glm::vec3 collidedWithFace6 = glm::vec3(collidedWithPosition[3].x, collidedWithPosition[3].y, collidedWithPosition[3].z - collidedWith->boundingBox.z);

                            
                            // get the 6 faces of the collider
                            // 1 = right, 2 = left, 3 = top, 4 = bottom, 5 = front, 6 = back
                            glm::vec3 colliderFace1 = glm::vec3(colliderEntityPosition[3].x + collision->boundingBox.x, colliderEntityPosition[3].y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace2 = glm::vec3(colliderEntityPosition[3].x - collision->boundingBox.x, colliderEntityPosition[3].y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace3 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y + collision->boundingBox.y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace4 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y - collision->boundingBox.y, colliderEntityPosition[3].z);
                            glm::vec3 colliderFace5 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y, colliderEntityPosition[3].z + collision->boundingBox.z);
                            glm::vec3 colliderFace6 = glm::vec3(colliderEntityPosition[3].x, colliderEntityPosition[3].y, colliderEntityPosition[3].z - collision->boundingBox.z);

                            if (colliderFace4.y<collidedWithFace3.y && colliderFace4.y>collidedWithFace4.y && colliderFace3.y<collidedWithFace3.y && colliderFace3.y>collidedWithFace4.y){
                                collision->collided.x = 1; 
                                continue;
                            }

                            // check if the difference betweeen y of the top face of the collider and the bottom face of the collidedWith is smaller than that of the x and y
                            if (colliderFace4.y<collidedWithFace3.y && colliderFace4.y>collidedWithFace4.y)
                            {
                                double diffY = colliderFace4.y - collidedWithFace3.y;
                                double diffZ = colliderFace4.z - collidedWithFace3.z;
                                if (abs(diffY) > abs(0.01))
                                {
                                    collision->collided.x = 1;
                                    collision->collided.y = 1;
                                    continue;
                                }
                                //check if they are colliding on the x and z axis
                                bool collisionX = (colliderFace4.x + collision->boundingBox.x > collidedWithFace3.x - collidedWith->boundingBox.x && colliderFace4.x + collision->boundingBox.x < collidedWithFace3.x + collidedWith->boundingBox.x) || (colliderFace4.x - collision->boundingBox.x < collidedWithFace3.x + collidedWith->boundingBox.x && colliderFace4.x - collision->boundingBox.x > collidedWithFace3.x - collidedWith->boundingBox.x);
                                bool collisionZ = (colliderFace4.z  + collision->boundingBox.z > collidedWithFace3.z - collidedWith->boundingBox.z && colliderFace4.z + collision->boundingBox.z < collidedWithFace3.z + collidedWith->boundingBox.z) || (colliderFace4.z - collision->boundingBox.z < collidedWithFace3.z + collidedWith->boundingBox.z && colliderFace4.z - collision->boundingBox.z > collidedWithFace3.z - collidedWith->boundingBox.z);
                                if (collisionX && collisionZ)
                                {
                                    collision->collided.y = 1;
                                    continue; 
                                }
                                int passed = 0;
                            }
                            if (colliderFace3.y<collidedWithFace4.y && colliderFace3.y>collidedWithFace3.y)
                            {
                                //check if they are colliding on the x and z axis
                                bool collisionX = (colliderFace3.x + collision->boundingBox.x > collidedWithFace4.x - collidedWith->boundingBox.x && colliderFace3.x + collision->boundingBox.x < collidedWithFace4.x + collidedWith->boundingBox.x) || (colliderFace3.x - collision->boundingBox.x < collidedWithFace4.x + collidedWith->boundingBox.x && colliderFace3.x - collision->boundingBox.x > collidedWithFace4.x - collidedWith->boundingBox.x);
                                bool collisionZ = (colliderFace3.z  + collision->boundingBox.z > collidedWithFace4.z - collidedWith->boundingBox.z && colliderFace3.z + collision->boundingBox.z < collidedWithFace4.z + collidedWith->boundingBox.z) || (colliderFace3.z - collision->boundingBox.z < collidedWithFace4.z + collidedWith->boundingBox.z && colliderFace3.z - collision->boundingBox.z > collidedWithFace4.z - collidedWith->boundingBox.z);
                                if (collisionX && collisionZ)
                                {
                                    collision->collided.y = 1;
                                    continue; 
                                }
                                int passed = 0;
                            }
                            collision->collided.x = 1;     
                        }
                        
                    }
                }
            }
            return;
        }
    };

   

}
