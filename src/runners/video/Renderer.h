/*
 * Renderer.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#pragma once

#include "Camera.h"
#include "ShaderProgramResource.h"
#include "VideoRunner.h"

enum class RendererStatus {
  CREATED,
  INITIALIZED,
  FAILED
};


class Renderer {
protected:
  Logger *logger = LoggerFactory::getLogger("Renderer");
  RendererStatus status;
  const ShaderProgramResource *shader = null;
  VideoRunner &videoRunner;
  ResourceManager &resourceManager;
  bool enabled = true;
public:

  Renderer(VideoRunner &videoRunner) : videoRunner(videoRunner), resourceManager(videoRunner.getResourceManager()) {
    this->status = RendererStatus::CREATED;
  }

  virtual ~Renderer() {
  }

  void setShaderProgram(const ShaderProgramResource *shaderProgramResource) {
    this->shader = shaderProgramResource;
  }

  void sendMaterial(const MaterialResource *material) const {
    if (material != null) {
      videoRunner.sendVector("material.ambient", material->getAmbient());
      videoRunner.sendVector("material.diffuse", material->getDiffuse());
      videoRunner.sendVector("material.specular", material->getSpecular());
      videoRunner.sendReal("material.alpha", material->getAlpha());
      videoRunner.sendReal("material.shininess", material->getShininess());
    }
  }

  void sendLight(const LightResource *light) const {
    if (light) {
      videoRunner.sendVector("light.ambient", light->getAmbient() * light->getShininess());
      videoRunner.sendVector("light.diffuse", light->getDiffuse() * light->getShininess());
      videoRunner.sendVector("light.specular", light->getSpecular() * light->getShininess());
      videoRunner.sendVector("light.position", light->getPosition());
    }
  }

  virtual RendererStatus initialize() {
    if(this->status == RendererStatus::CREATED) {
      return RendererStatus::INITIALIZED;
    }

    return this->status;
  }

  virtual bool isEnabled() const {
    return this->status == RendererStatus::INITIALIZED && this->shader != null && this->enabled;
  }

  void setEnabled(bool enabled) {
    this->enabled = enabled;
  }

  virtual String toString() const {
    return logger->getBasename();
  }

  void render(const Camera &camera) {
    if(this->isEnabled()) {
      videoRunner.useProgramResource(shader);
      doRender(camera);
      //videoRunner.useProgramResource(null);
    } else {
      if(this->status == RendererStatus::CREATED) {
        if((this->status = initialize()) != RendererStatus::INITIALIZED) {
          logger->error("Renderer [%s] not properly initialized", this->toString().c_str());
        }
      } //else { logger->error("Renderer [%s] is not enabled", this->toString().c_str()); }
    }
  }

protected:
  virtual void doRender(const Camera &camera) = 0; //do the actual render
};
