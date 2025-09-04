/*
 * GridRenderer.h
 *
 *  Created on: Apr 11, 2021
 *      Author: leandro
 */

#pragma once

#include "ShaderProgramResource.h"
#include "Renderer.h"
#include "VideoRunner.h"

class GridRenderer : public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("TerrainRenderer");

    const VertexArrayResource *plane = null;

public:
    bool init() override {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource *)resourceManager->load("shaders/grid/grid.program.json", MimeTypes::SHADERPROGRAM);
        }

        if(this->plane == null) {
            this->plane = (VertexArrayResource *)resourceManager->load("geometry/grid/plane.json", MimeTypes::VERTEXARRAY);
        }

        return true;
    }

    void render(const Camera &camera) override {
        if(isEnabled()) {
            videoRunner->useProgramResource(shader);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable( GL_BLEND );

            videoRunner->sendMatrix("matrices.proj", camera.getProjectionMatrix());
            videoRunner->sendMatrix("matrices.view", camera.getViewMatrix());
            videoRunner->sendReal("near", camera.getZNear());
            videoRunner->sendReal("far", camera.getZFar());

            videoRunner->drawVertexArray(plane);

            videoRunner->useProgramResource(null);
        }
    }

    bool isEnabled() const override {
    	return Renderer::isEnabled() && this->plane != null;
    }

};
