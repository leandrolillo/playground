#pragma once

#include "Renderer.h"

class Sprite {
  friend class SpriteRenderer;

protected:
  const TextureResource *texture = null; //would love to make this a non nullable reference
  vector2 position;
  vector2 size;
  real rotation;
  vector3 color;
public:
  Sprite(const TextureResource *texture, const vector2 &position, const vector2 &size, real rotation = 0.0, const vector3 &color = vector3(1, 1, 1)) :
    texture(texture),
    position(position),
    size(size),
    rotation(rotation),
    color(color)
  {

  }

  Sprite &setTexture(const TextureResource *texture) {
    this->texture = texture;
    return *this;
  }
  const TextureResource *getTexture() const {
    return this->texture;
  }

  Sprite &setSize(const vector2 &size) {
    this->size = size;
    return *this;
  }

  virtual vector2 getPosition() const {
    return this->position;
  }

  real getRotation() const {
    return this->rotation;
  }

  const vector2 &getSize() const {
    return this->size;
  }


  const vector3 &getColor() const {
    return this->color;
  }
};

class SpriteRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("SpriteRenderer");

  const VertexArrayResource *rectangle = null;

  std::map<const TextureResource *, std::vector<Sprite>>spritesByTexture;
  unsigned long maxTextures = 32;
public:
  using Renderer::Renderer;

  virtual RendererStatus initialize() override {
    if(this->shader == null) {
      this->shader = (ShaderProgramResource*) this->resourceManager.load("core/sprite.program.json", MimeTypes::SHADERPROGRAM);
    }

    if(this->rectangle == null) {
      this->rectangle = (VertexArrayResource *)this->resourceManager.load("core/rectangle.json", MimeTypes::VERTEXARRAY);
    }

    if(shader == null || rectangle == null) {
      logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
      return RendererStatus::FAILED;
    }

    this->maxTextures = videoRunner.getIntegerOption(VideoAttribute::MAX_TEXTURES);

    return RendererStatus::INITIALIZED;
  }

  void clear() {
    this->spritesByTexture.clear();
  }

  void draw(const TextureResource &texture, const vector2 &position, const vector2 &size, real rotation, const vector3 &color = vector3(1.0, 1.0, 1.0)) {
    this->spritesByTexture[texture].emplace_back(&texture, position, size, rotation, color);
  }

  void draw(const Sprite &sprite) {
    if(sprite.texture != null) {
      this->spritesByTexture[sprite.texture].emplace_back(sprite);
    }
  }

protected:
  void doRender(const Camera &camera) override {
    videoRunner.sendMatrix("matrices.projectionView", camera.getProjectionMatrix() * matriz_4x4::traslacion(camera.getPosition()));

    unsigned long currentTextureIndex = 0;
    for(auto &entry : spritesByTexture) {
      videoRunner.setTexture(currentTextureIndex++, "image", entry.first);
      //currentTextureIndex++;
      if(maxTextures <= currentTextureIndex) {
        currentTextureIndex = 0;
      }

      for(auto &sprite : entry.second) {
        videoRunner.sendMatrix("matrices.model", matriz_4x4::rotacion(vector3(0.0, 0.0, sprite.getRotation())) * matriz_4x4::traslacion(vector3(sprite.getPosition() + sprite.getSize() * 0.5, 0.0)) * matriz_4x4::zoom(vector3(sprite.getSize(), 1.0)));
        videoRunner.sendVector("color", sprite.getColor());
        videoRunner.drawVertexArray(rectangle);
      }
    }
  }

};
