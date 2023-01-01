#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for making the collider collide with every entity which contains a CollisionComponent.
    class CollisionDetectionSystem
    {
    public:
        void initialize(World *world)
        {
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the collision component if it exists
                CollisionComponent *collision = entity->getComponent<CollisionComponent>();
                // If the collision component exists
                if (collision == nullptr)
                    continue;
                // if the entity is the collider, this will be the n-child of the player, n = [0, inf]
                if (collision->collider)
                {
                    // loop intil the parent (player) is found
                    // to reduce computation we only check collisions for collider objects which are limited to moving objects
                    if (collision->collider)
                    {
                        // get the parent of the collider component to reference its location in the world
                        Entity *parent = entity;
                        while (parent->parent != nullptr)
                        {
                            parent = parent->parent;
                        }
                        // set the last location for the collider to use in collision resolution
                        collision->lastPosition = parent->localTransform.position;
                    }
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
                // case entity does not collision component
                if (collision == nullptr)
                    continue;
                // if the entity is the collider, this will be the n-child of the player, n = [0, inf]
                if (collision->collider)
                {
                    // loop intil the parent (player) is found
                    Entity *parent = entity;
                    collision->collided = glm::vec3(0, 0, 0);
                    while (parent->parent != nullptr)
                    {
                        parent = parent->parent;
                    }
                    // detects if the player is colliding with another entity
                    detectCollision(collision, world);
                    // the next part if for knowing from which side the player is colliding with
                    glm::vec3 collisionDirection = collision->collided;
                    // if the player is colliding with something, any direction
                    if (collisionDirection.x != 0 || collisionDirection.y != 0 || collisionDirection.z != 0)
                    {
                        // std::cout << "Collision Direction " << collisionDirection.x << ", " << collisionDirection.y << ", " << collisionDirection.z << std::endl;
                        // std::cout << "Current position: " << parent->localTransform.position.x << ", " << parent->localTransform.position.y << ", " << parent->localTransform.position.z << std::endl;
                        // std::cout << "Last position: " << collision->lastPosition.x << ", " << collision->lastPosition.y << ", " << collision->lastPosition.z << std::endl;
                        //  if the player is colliding with something in the x axis
                        if (collisionDirection.x == 1)
                        {
                            // then we reset the x position to the last position before the collision
                            parent->localTransform.position.x = collision->lastPosition.x;
                            parent->localTransform.position.z = collision->lastPosition.z;
                        }
                        // if the player is colliding with something in the y axis
                        if (collisionDirection.y == 1)
                        {
                            // then we reset the y position to the last y position before the collision
                            parent->localTransform.position.y = collision->lastPosition.y;
                        }
                        // if the player is colliding with something in the z axis
                        if (collisionDirection.z == 1)
                        {

                            parent->localTransform.position.y = collision->lastPosition.y;
                        }
                    }
                    // save the position of the collider
                    collision->lastPosition = parent->localTransform.position;
                }
            }
        }

        // this function detect collision based on knowing which vertices of the bounding box are colliding
        void detectCollision(CollisionComponent *collision, World *world)
        {
            // get the player entity
            Entity *colliderEntity = collision->getOwner();
            // get the player position in the world
            glm::mat4 colliderEntityPosition = colliderEntity->getLocalToWorldMatrix();

            // for each entity in the world, to check if the player is colliding with it
            for (auto entity : world->getEntities())

            {
                // get the collided with component of the entity
                CollisionComponent *collidedWith = entity->getComponent<CollisionComponent>();
                glm::mat4 collidedWithPosition = entity->getLocalToWorldMatrix();
                // first we check if the entity has a collision component
                if (collidedWith)
                {
                    // check if the entity is not the player
                    if (collidedWith != collision)
                    {

                        if (colliderEntityPosition[3].x + collision->boundingBox.x > collidedWithPosition[3].x - collidedWith->boundingBox.x &&
                            colliderEntityPosition[3].x - collision->boundingBox.x < collidedWithPosition[3].x + collidedWith->boundingBox.x &&
                            colliderEntityPosition[3].y + collision->boundingBox.y > collidedWithPosition[3].y - collidedWith->boundingBox.y &&
                            colliderEntityPosition[3].y - collision->boundingBox.y < collidedWithPosition[3].y + collidedWith->boundingBox.y &&
                            colliderEntityPosition[3].z + collision->boundingBox.z > collidedWithPosition[3].z - collidedWith->boundingBox.z &&
                            colliderEntityPosition[3].z - collision->boundingBox.z < collidedWithPosition[3].z + collidedWith->boundingBox.z)
                        {

                            // check if the player is colliding with a ground, then we set the y axis to 1
                            // meaning that it is colliding in the y axis
                            if (collidedWith->ground)
                            {
                                collision->collided.y = 1;
                                continue;
                            }
                            // check if the player is colliding with a coin, then we mark the coin for removal
                            // to give the collecting effect
                            if (collidedWith->coin)
                            {
                                collidedWith->getOwner()->getWorld()->markForRemoval(collidedWith->getOwner());
                                // collidedWith->getOwner()->getWorld()->deleteMarkedEntities();
                                // add 1 to the coins collected
                                colliderEntity->parent->getComponent<PlayerControllerComponent>()->coinsCollected += 1;
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

                            // check if collider is colliding with a wall
                            if (colliderFace4.y < collidedWithFace3.y && colliderFace4.y > collidedWithFace4.y && colliderFace3.y < collidedWithFace3.y && colliderFace3.y > collidedWithFace4.y)
                            {
                                collision->collided.x = 1;
                                continue;
                            }

                            // check if the difference betweeen y of the top face of the collider and the bottom face of the collidedWith is smaller than that of the x and y
                            if (colliderFace4.y < collidedWithFace3.y && colliderFace4.y > collidedWithFace4.y)
                            {

                                // check if collider is partially colliding (almost making a jump)
                                double diffY = colliderFace4.y - collidedWithFace3.y;
                                double diffZ = colliderFace4.z - collidedWithFace3.z;
                                if (abs(diffY) > abs(0.01))
                                {
                                    // set collision with sides and ground to 1 (true)
                                    collision->collided.x = 1;
                                    collision->collided.y = 1;
                                    continue;
                                }
                                // check if they are colliding on the x and z axis
                                bool collisionX = (colliderFace4.x + collision->boundingBox.x > collidedWithFace3.x - collidedWith->boundingBox.x && colliderFace4.x + collision->boundingBox.x < collidedWithFace3.x + collidedWith->boundingBox.x) || (colliderFace4.x - collision->boundingBox.x < collidedWithFace3.x + collidedWith->boundingBox.x && colliderFace4.x - collision->boundingBox.x > collidedWithFace3.x - collidedWith->boundingBox.x);
                                bool collisionZ = (colliderFace4.z + collision->boundingBox.z > collidedWithFace3.z - collidedWith->boundingBox.z && colliderFace4.z + collision->boundingBox.z < collidedWithFace3.z + collidedWith->boundingBox.z) || (colliderFace4.z - collision->boundingBox.z < collidedWithFace3.z + collidedWith->boundingBox.z && colliderFace4.z - collision->boundingBox.z > collidedWithFace3.z - collidedWith->boundingBox.z);
                                if (collisionX && collisionZ)
                                {
                                    // colliding with ground
                                    collision->collided.y = 1;
                                    continue;
                                }
                                int passed = 0;
                            }
                            // check if colliding with ceiling
                            if (colliderFace3.y > collidedWithFace4.y && colliderFace3.y < collidedWithFace3.y)
                            {
                                double diffY = colliderFace3.y - collidedWithFace4.y;
                                double diffZ = colliderFace3.z - collidedWithFace4.z;
                                if (abs(diffY) > abs(0.01))
                                {
                                    // check if partially colliding with ceiling
                                    collision->collided.x = 1;
                                    collision->collided.z = 1;
                                    continue;
                                }
                                // check if they are colliding on the x and z axis
                                bool collisionX = (colliderFace3.x + collision->boundingBox.x > collidedWithFace4.x - collidedWith->boundingBox.x && colliderFace3.x + collision->boundingBox.x < collidedWithFace4.x + collidedWith->boundingBox.x) || (colliderFace3.x - collision->boundingBox.x < collidedWithFace4.x + collidedWith->boundingBox.x && colliderFace3.x - collision->boundingBox.x > collidedWithFace4.x - collidedWith->boundingBox.x);
                                bool collisionZ = (colliderFace3.z + collision->boundingBox.z > collidedWithFace4.z - collidedWith->boundingBox.z && colliderFace3.z + collision->boundingBox.z < collidedWithFace4.z + collidedWith->boundingBox.z) || (colliderFace3.z - collision->boundingBox.z < collidedWithFace4.z + collidedWith->boundingBox.z && colliderFace3.z - collision->boundingBox.z > collidedWithFace4.z - collidedWith->boundingBox.z);
                                if (collisionX && collisionZ)
                                {
                                    // colliding with ceiling
                                    collision->collided.z = 1;
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
