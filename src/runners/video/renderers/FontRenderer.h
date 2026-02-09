#pragma once

#include "Renderer.h"
#include "FontResource.h"
class Text {
  String text;
  vector2 position;
  vector3 color;

};

class FontRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("SpriteRenderer");

  std::unordered_map<const FontResource *, std::list<Text>>textsByFont;
  std::unique_ptr<VertexArrayResource> quad;
  unsigned long maxTextures = 32;
public:
  using Renderer::Renderer;

  virtual RendererStatus initialize() override {
    if(this->shader == null) {
      this->shader = (ShaderProgramResource*) this->resourceManager.load("core/font.program.json", MimeTypes::SHADERPROGRAM);
    }

    if(shader == null) {
      logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
      return RendererStatus::FAILED;
    }

    this->maxTextures = video.getIntegerOption(VideoAttribute::MAX_TEXTURES);

    if(this->quad == null) {
      this->quad = (VertexArrayResource *)this->resourceManager.load("core/glyph.json", MimeTypes::VERTEXARRAY);
    }


    return RendererStatus::INITIALIZED;
  }

  void clear() {
    textsByFont.clear();
  }


protected:
  void doRender(const Camera &camera) override {
    video.sendMatrix("projectionView", camera.getProjectionMatrix() * matriz_4x4::traslacion(camera.getPosition()));

    unsigned long currentTextureIndex = 0;
    for(auto &entry : textsByFont) {
      const FontResource &font = *entry.first;
      video.setTexture(currentTextureIndex++, "image", font.getTextureAtlas());
      //currentTextureIndex++;
      if(maxTextures <= currentTextureIndex) {
        currentTextureIndex = 0;
      }

      for(auto &text : entry.second) {
        video.sendVector("color", text.getColor());

        video.drawVertexArray(this->quad);
      }
    }
  }

};
