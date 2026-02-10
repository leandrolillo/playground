#pragma once

#include "Renderer.h"
#include "FontResource.h"
class Text {
  String text;
  vector2 position;
  vector3 color;
public:
  Text(const String &text, const vector2 &position, const vector3 &color) :
    text(text),
    position(position),
    color(color)
  {
  }
  const vector3 &getColor() const {
    return this->color;
  }
  const vector2 &getPosition() const {
    return this->position;
  }
  const String &getValue() const {
    return this->text;
  }
};

class TextRenderer: public Renderer {
private:
  Logger *logger = LoggerFactory::getLogger("SpriteRenderer");
  std::unordered_map<const FontResource *, std::vector<Text>>textsByFont;
  VertexArrayResource *quad = null;
  FontResource *defaultFont = null;

  unsigned long maxTextures = 32;
public:
  using Renderer::Renderer;

  virtual RendererStatus initialize() override {
    if(this->shader == null) {
      if(this->shader = (ShaderProgramResource*) this->resourceManager.load("core/font.program.json", MimeTypes::SHADERPROGRAM); shader == null) {
        logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
        return RendererStatus::FAILED;
      }
    }

    this->maxTextures = video.getIntegerOption(VideoAttribute::MAX_TEXTURES);

    if(this->quad == null) {
      if(this->quad = (VertexArrayResource *)this->resourceManager.load("core/glyph.json", MimeTypes::VERTEXARRAY); this->quad == null) {
        logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
        return RendererStatus::FAILED;
      }
    }

    if(defaultFont == null) {
      if(this->defaultFont = (FontResource *)this->resourceManager.load("core/Geneva.ttf"); this->defaultFont == null) {
        logger->error("Failed to initialize Renderer [%s]", this->toString().c_str());
        return RendererStatus::FAILED;
      }
    }

    return RendererStatus::INITIALIZED;
  }

  void print(const vector2 &position, const String &text, const vector3 &color = vector3(1, 1, 1)) {
    print(*defaultFont, position, text, color);
  }

  void print(const FontResource &font, const vector2 &position, const String &text, const vector3 &color = vector3(1, 1, 1)) {
    textsByFont[&font].emplace_back(Text(text, position, color));
  }

  void clear() {
    textsByFont.clear();
  }


protected:
  void doRender(const Camera &camera) override {
    real scale = 1.0;

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
        vector2 cursor = text.getPosition();

        for(char character : text.getValue()) {
          auto glyph = font.getGlyph(character);

          vector2 position = cursor +  vector2(glyph.getOffset().x, glyph.getSize().y - glyph.getOffset().y) * scale;
          vector2 size = glyph.getSize() * scale;

          video.drawVertexArray(this->quad,
              std::vector { position.x,           position.y + size.y,    0.0f, 0.0f,
                            position.x + size.x,  position.y,             1.0f, 1.0f,
                            position.x,           position.y + size.y,    0.0f, 0.0f,
                            position.x + size.x,  position.y,             1.0f, 1.0f,
                            position.x + size.x,  position.y + size.y,    1.0f, 0.0f }  );

          cursor.y += glyph.getAdvance();
        }
      }
    }
  }

};
