#pragma once

#include "Renderer.h"

class Sprite {
  friend class SpriteRenderer;

protected:
  const TextureResource &texture;
  vector2 position;
  real rotation;
  vector3 color;
public:
  Sprite(const TextureResource &texture, const vector2 &position, real rotation, const vector3 &color) :
    texture(texture),
    position(position),
    rotation(rotation),
    color(color)
  {

  }
};

class SpriteRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("SpriteRenderer");

  const TextureResource *currentTexture = null;
  const MaterialResource *currentMaterial = null;
  const LightResource *light = null;

  const VertexArrayResource *rectangle = null;

  std::map<const TextureResource *, std::vector<Sprite>>spritesByTexture;
public:
  SpriteRenderer(VideoRunner &videoRunner) : Renderer(videoRunner) {
      this->shader = (ShaderProgramResource*) this->resourceManager.load("core/sprite.program.json", MimeTypes::SHADERPROGRAM);
      this->rectangle = (VertexArrayResource *)this->resourceManager.load("core/rectangle.json", MimeTypes::VERTEXARRAY);
  }

  void render(const Camera &camera) override {
    videoRunner.useProgramResource(this->shader);
//    videoRunner.setTexture(0, "image", &texture);
//    videoRunner.sendMatrix("projection", null);
//    videoRunner.sendMatrix("model", matriz_4x4::rotacion(vector3(0.0, 0.0, rotation)) * matriz_4x4::traslacion(vector3(position + size * 0.5, 0.0)) * matriz_4x4::zoom(vector3(size, 1.0)));
//
//    videoRunner.drawVertexArray(rectangle);

  }

  void draw(const TextureResource &texture, const vector2 &position, const vector2 &size, real rotation, const vector3 &color) {
    this->spritesByTexture[this->currentTexture].emplace_back(texture, position, rotation, color);
  }
};
