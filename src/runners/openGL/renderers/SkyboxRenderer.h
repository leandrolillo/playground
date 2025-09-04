#pragma once

#include <OpenGL/gl3.h>
#include "CubeMapResource.h"
#include "ShaderProgramResource.h"
#include "VideoRunner.h"
#include "Renderer.h"


class SkyboxRenderer: public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("SkyboxRenderer");

    const CubeMapResource *cubeMap = null;
    const VertexArrayResource *box = null;
    real size = 300;
public:
    void setCubeMap(const CubeMapResource *cubeMap) {
        this->cubeMap = cubeMap;
    }

    void setBox(const VertexArrayResource *box) {
        this->box = box;
    }

    void setSize(real size) {
        this->size = size;
    }

    bool init() override {
        if (this->shader == null) {
            this->shader = (ShaderProgramResource*) this->resourceManager->load("shaders/skybox/skybox.program.json", MimeTypes::SHADERPROGRAM);
        }

        if (this->cubeMap == null) {
            this->cubeMap = (CubeMapResource*) this->resourceManager->load("geometry/skybox/skybox.json", MimeTypes::CUBEMAP);
        }

        if (this->box == null) {
            this->box = (VertexArrayResource*) this->resourceManager->load("geometry/skybox/skybox_geometry.json", MimeTypes::VERTEXARRAY);
        }

        return true;
    }

    void render(const Camera &camera) override {
        if (isEnabled()) {
            videoRunner->useProgramResource(shader);
            videoRunner->setTexture(0, "textureUnit", cubeMap, GL_TEXTURE_CUBE_MAP);
            videoRunner->sendMatrix("matrices.p", camera.getProjectionMatrix());
            videoRunner->sendMatrix("matrices.v", camera.getViewMatrix());
            videoRunner->sendReal("boxSize", this->size);

            videoRunner->drawVertexArray(box);

            videoRunner->useProgramResource(null);
            videoRunner->setTexture(0, null, GL_TEXTURE_CUBE_MAP);
        }
    }

    bool isEnabled() const override {
    	return Renderer::isEnabled() && this->box != null && this->cubeMap != null;
    }
};
