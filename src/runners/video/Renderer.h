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

class Renderer {
protected:
  const ShaderProgramResource *shader = null;
  VideoRunner &videoRunner;
  ResourceManager &resourceManager;
  bool enabled = true;
public:

  Renderer(VideoRunner &videoRunner) : videoRunner(videoRunner), resourceManager(videoRunner.getResourceManager()) {
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

  virtual bool init() {
    return true;
  }

  virtual void render(const Camera &camera) = 0;

  virtual bool isEnabled() const {
    return this->shader != null && this->enabled;
  }

  void setEnabled(bool enabled) {
    this->enabled = enabled;
  }

};
