#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        Material::setup();
        // setting the uniform variable "tint" to the tint value
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        // Set to 0 because we need the same value saved in the Texture Unit
        GLuint textureUnit = 0;
        // save the texture to the textureUnit GL_TEXTURE0
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        if (sampler != nullptr){
            sampler->bind(textureUnit);
        // setting the uniform variable "tex" to the textureUnit
            shader->set("tex", (int)textureUnit);
        }
        //TODO: (Req 7) Write this function
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


    // This function should call the setup of its parent (TexturedMaterial) and
    // set the "ambient", "diffuse", "specular" and "shininess" uniforms to the values in the member variables
    void LitMaterial::setup() const {
        TexturedMaterial::setup();
        // shader uniform value setting
        shader->set("ambient", ambient);
        shader->set("diffuse", diffuse);
        shader->set("specular", specular);
        shader->set("shininess", shininess);
    }

    // This function read the lit material data from a json object, calls the parent's deserialize function first 
    // to get the texture and sampler
    void LitMaterial::deserialize(const nlohmann::json& data){
        TexturedMaterial::deserialize(data);
        if(!data.is_object()) return;
        ambient = data.value("ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        diffuse = data.value("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        specular = data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f));
        shininess = data.value("shininess", 0.0f);
    }

}