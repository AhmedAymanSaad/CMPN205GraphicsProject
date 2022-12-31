#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "../components/collision.hpp"


namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class PlayerControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

        float velocity = 0;
        float gravity = -9.8;
        float lastYPos = 0;

        bool gameOver = false;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent* camera = nullptr;

            PlayerControllerComponent *controller = nullptr;
            CollisionComponent *collider = nullptr;
            for(auto entity : world->getEntities()){
                controller = entity->getComponent<PlayerControllerComponent>();
                if( controller) break;
            }
            for(auto entity : world->getEntities()){
                camera = entity->getComponent<CameraComponent>();
                if(camera) break;
            }
            for(auto entity : world->getEntities()){
                if (entity->parent==controller->getOwner()){
                    collider = entity->getComponent<CollisionComponent>();
                    if(collider) break;
                }
            }

            //check if game is over
            if ( controller->coinsCollected == 3 && !gameOver){
                std::cout << "You win!" << std::endl;
                gameOver = true;
            }
            

            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(controller)) return;
            if (!(camera)) return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = controller->getOwner();
            Entity* childEntity = camera->getOwner();

/////////////////////////////////////////////
            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked){
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
            // If the left mouse button is released, we unlock and unhide the mouse.
            } else if(!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;
            glm::vec3& parentRotation = entity->localTransform.rotation;

            glm::vec3& rotation = childEntity->localTransform.rotation;
            glm::vec3& childPosition = childEntity->localTransform.position;


            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
//////////////////////////////////////////////
            if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) ){
                glm::vec2 delta = app->getMouse().getMouseDelta();
                if (app->getKeyboard().isPressed(GLFW_KEY_Z)) {
                    rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                    rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw


                    float radius = 1;

                    childPosition.x = radius * glm::sin(rotation.y) * glm::cos(rotation.x);
                    childPosition.z = radius * glm::cos(rotation.y) * glm::cos(rotation.x);
                    childPosition.y = -radius * glm::sin(rotation.x);

                    float normal = glm::sqrt(childPosition.x * childPosition.x + childPosition.y * childPosition.y + childPosition.z * childPosition.z);
                    std:: cout << "camera position: " << childPosition.x << ", " << childPosition.y << ", " << childPosition.z << ", " << normal << std::endl;
                }
                else {
                    // parentRotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                    parentRotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
                }

            }




            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if(rotation.x < -glm::half_pi<float>() * 0.99f) rotation.x = -glm::half_pi<float>() * 0.99f;
            if(rotation.x >  glm::half_pi<float>() * 0.99f) rotation.x  = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time. 
            rotation.y = glm::wrapAngle(rotation.y);
            // moving the camera in a spherical coordinate system around the player where the radius is 1
            // the camera is always looking at the player


            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();
            // glm::mat4 childMatrix = childEntity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) position += front * (deltaTime * current_sensitivity.z);
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) position -= front * (deltaTime * current_sensitivity.z);
            // Q & E moves the player up and down
            if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * current_sensitivity.y);
            if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * (deltaTime * current_sensitivity.x);
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * (deltaTime * current_sensitivity.x);


            // get collider componenet
            
            if (collider->collided.y == 1) {
                velocity = 0;
            }

            if(app->getKeyboard().isPressed(GLFW_KEY_SPACE) && collider->collided.y==1) {
                velocity = 5;
            }
            
            if (deltaTime > 0.5) {
                deltaTime = 0.1;
            }
            if (collider->collided.y == 0)
                velocity += gravity * deltaTime;
            if (velocity < -10) {
                velocity = -8;
            }
            if (velocity > 10) {
                velocity = 10;
            }

            position.y += (deltaTime * velocity);

            std::cout << "velocity: " << velocity << std::endl;
            std::cout << "position: " << position.y << std::endl;
            
            
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        bool getGameOver() {
            return gameOver;
        }

    };

}
