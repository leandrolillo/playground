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

class GridRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("TerrainRenderer");
  const VertexArrayResource *plane = null;

public:
  using Renderer::Renderer;

  virtual RendererStatus initialize() override {
    if (this->shader == null) {
      this->shader = (ShaderProgramResource*) resourceManager.load("shaders/grid/grid.program.json", MimeTypes::SHADERPROGRAM);
    }

    if (this->plane == null) {
      this->plane = (VertexArrayResource*) resourceManager.load("geometry/grid/plane.json", MimeTypes::VERTEXARRAY);
    }

    if (shader == null || this->plane == null) {
      logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
      return RendererStatus::FAILED;
    }

    return RendererStatus::INITIALIZED;
  }
protected:
  void doRender(const Camera &camera) override {
    videoRunner.enable(VideoAttribute::BLEND, VideoAttribute::SRC_ALPHA, VideoAttribute::ONE_MINUS_SRC_ALPHA);
    videoRunner.sendMatrix("matrices.proj", camera.getProjectionMatrix());
    videoRunner.sendMatrix("matrices.view", camera.getViewMatrix());
    videoRunner.sendReal("near", camera.getZNear());
    videoRunner.sendReal("far", camera.getZFar());

    videoRunner.drawVertexArray(plane);
  }
};
