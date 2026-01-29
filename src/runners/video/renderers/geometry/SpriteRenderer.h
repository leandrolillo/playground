#pragma once

#include "Renderer.h"

class SpriteRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("SpriteRenderer");

  const TextureResource *currentTexture = null;
  const MaterialResource *currentMaterial = null;
  const LightResource *light = null;

  const VertexArrayResource *rectangle = null;
public:
  SpriteRenderer(VideoRunner &videoRunner) : Renderer(videoRunner) {
      this->shader = (ShaderProgramResource*) this->resourceManager.load("core/sprite.program.json", MimeTypes::SHADERPROGRAM);
      this->rectangle = (VertexArrayResource *)this->resourceManager.load("core/rectangle.json", MimeTypes::VERTEXARRAY);
  }

  void render(const Camera &camera) override {

  }

  void draw(const TextureResource &texture, vector2 position, vector2 size, real rotation, vector3 color) {
    videoRunner.useProgramResource(this->shader);
    videoRunner.setTexture(0, "image", texture);
    videoRunner.sendMatrix("projection", null);
    videoRunner.sendMatrix("model", matriz_4x4::rotacion(vector3(0.0, 0.0, rotation)) * matriz_4x4::traslacion(vector3(position + size * 0.5, 0.0)) * matriz_4x4::zoom(vector3(size, 1.0)));

    videoRunner.drawVertexArray(rectangle);
  }
};
