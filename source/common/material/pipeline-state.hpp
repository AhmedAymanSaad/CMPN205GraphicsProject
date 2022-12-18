#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            /*Try mentally visualizing a 3D cube and count the maximum number of faces you'll be able to see from any direction. If your imagination is not too creative you probably ended up with a maximum number of 3. You can view a cube from any position and/or direction, but you would never be able to see more than 3 faces. So why would we waste the effort of drawing those other 3 faces that we can't even see. If we could discard those in some way we would save more than 50% of this cube's total fragment shader runs
            * save more than 50%.
            * face culling does. OpenGL checks all the faces that are front facing towards the viewer and renders those while discarding all the faces that are back facing
            */
            
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            /*In most systems you'll see a depth buffer with a precision of 24 bits.
            *Depth testing is done in screen space after the fragment shader has run
            *GL_EQUAL->Passes if the fragment's depth value is equal to the stored depth value.
            */
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            /*Blending in OpenGL is commonly known as the technique to implement transparency within objects
            * The amount of transparency of an object is defined by its color's alpha value
            * alpha value of 0.0 would result in the object having complete transparency
            * Cresult=Csource∗Fsource+Cdestination∗Fdestination
            * Csource= the source color vector. This is the color output of the fragment shader.
            * Fsource=the source factor value. Sets the impact of the alpha value on the source color.
            */
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        //                      red   green        alpha  false->no change is made to the red component of any pixel in any of the color buffers, regardless of the drawing operation attempted.
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function
            if (faceCulling.enabled) {
                glEnable(GL_CULL_FACE); // Enable face culling to be used
                glCullFace(faceCulling.culledFace); // Specify which face to be culled
                glFrontFace(faceCulling.frontFace); // Specify which face to be rendered (counter clock wise or clockwise)
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
            if (depthTesting.enabled) {
                glEnable(GL_DEPTH_TEST);    // Enable depth testing to be used
                glDepthFunc(depthTesting.function); // Specify when to replace the written depth value with the new value
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }

            if (blending.enabled) {
                glEnable(GL_BLEND);// Specify the the source and destination blending factors according to "constant color" and it is defaulted to be 0.
                glBlendColor(blending.constantColor[0], blending.constantColor[1], blending.constantColor[2], blending.constantColor[3]);
                glBlendEquation(blending.equation); // Add the two colors together including the factor (GL_FUNC_ADD)
                glBlendFunc(blending.sourceFactor, blending.destinationFactor); // Specify the factor for source color and destination color
            }
            else
            {
                glDisable(GL_BLEND);
            }
            // Enable the changes in all color channels (if set to be true)
            glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}