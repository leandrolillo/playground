#pragma once

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
  using Renderer::Renderer;

  virtual RendererStatus initialize() override {
    if (this->shader == null) {
      this->shader = (ShaderProgramResource*) this->resourceManager.load("shaders/skybox/skybox.program.json", MimeTypes::SHADERPROGRAM);
    }

    if (this->cubeMap == null) {
      this->cubeMap = (CubeMapResource*) this->resourceManager.load("geometry/skybox/skybox.json", MimeTypes::CUBEMAP);
    }

    if (this->box == null) {
      this->box = (VertexArrayResource*) this->resourceManager.load("geometry/skybox/skybox_geometry.json", MimeTypes::VERTEXARRAY);
    }

    if (this->shader == null || this->cubeMap == null || this->box == null) {
      logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
      return RendererStatus::FAILED;
    }

    return RendererStatus::INITIALIZED;
  }

  void setCubeMap(const CubeMapResource *cubeMap) {
    this->cubeMap = cubeMap;
  }

  void setBox(const VertexArrayResource *box) {
    this->box = box;
  }

  void setSize(real size) {
    this->size = size;
  }

protected:
  void doRender(const Camera &camera) override {
    videoRunner.setTexture(0, "textureUnit", cubeMap, VideoAttribute::TEXTURE_CUBE_MAP); //TODO: Refactor this to use generic properties and move to videoRunner
    videoRunner.sendMatrix("matrices.p", camera.getProjectionMatrix());
    videoRunner.sendMatrix("matrices.v", camera.getViewMatrix());
    videoRunner.sendReal("boxSize", this->size);

    videoRunner.drawVertexArray(box);

    videoRunner.setTexture(0, null, VideoAttribute::TEXTURE_CUBE_MAP);
  }
};
