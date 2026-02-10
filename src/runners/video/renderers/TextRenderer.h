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

  std::vector<real> quadVertices = {    0, 1,   0, 0,
                                        0, 0,   0, 1,
                                        1, 0,   1, 1,

                                        0, 1,   0, 0,
                                        1, 0,   1, 1,
                                        1, 1,   1, 0 };

public:
  using Renderer::Renderer;

  const FontResource *getDefaultFont() const {
    return this->defaultFont;
  }

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

  void print(const String &text, const vector2 &position, const vector3 &color = vector3(1, 1, 1)) {
    if(defaultFont != null) {
      print(*defaultFont, text, position, color);
    }
  }

  void print(const FontResource &font, const String &text, const vector2 &position, const vector3 &color = vector3(1, 1, 1)) {
    textsByFont[&font].emplace_back(Text(text, position, color));
  }

  void clear() {
    for(auto &entry : textsByFont) {
      entry.second.clear();
    }
  }


protected:
  void doRender(const Camera &camera) override {
    real scale = 1.0;

    video.sendMatrix("projectionView", camera.getProjectionMatrix() * matriz_4x4::traslacion(camera.getPosition()));

    unsigned long currentTextureIndex = 0;
    for(auto &entry : textsByFont) {
      auto &font = *(entry.first);
      video.setTexture(currentTextureIndex++, "image", font.getTextureAtlas());
      //currentTextureIndex++;
      if(maxTextures <= currentTextureIndex) {
        currentTextureIndex = 0;
      }

      for(auto &text : entry.second) {
        video.sendVector("color", text.getColor());
        vector2 cursor = text.getPosition();

        for(char character : text.getValue()) {
          auto &glyph = font.getGlyph(character);

          vector2 position = cursor +  vector2(glyph.getOffset().x, glyph.getSize().y - glyph.getOffset().y) * scale;
          vector2 size = glyph.getSize() * scale;

          this->quadVertices[0] = position.x;             this->quadVertices[1] = position.y + size.y;
          this->quadVertices[1*4] = position.x;           this->quadVertices[1*4+1] = position.y;
          this->quadVertices[2*4] = position.x + size.x;  this->quadVertices[2*4+1] = position.y;
          this->quadVertices[3*4] = position.x;           this->quadVertices[3*4+1] = position.y + size.y;
          this->quadVertices[4*4] = position.x + size.x;  this->quadVertices[4*4+1] = position.y;
          this->quadVertices[5*4] = position.x + size.x;  this->quadVertices[5*4+1] = position.y + size.y;

          video.drawVertexArray(this->quad, this->quadVertices);

          cursor.y += glyph.getAdvance();
        }
      }
    }
  }

};
