#pragma once

#include "Resource.h"
#include "TextureResource.h"

class Glyph {
  /**
   * normalized texture coordinates [0..1] in textureAtlas
   */
  vector2 textureTopLeft;
  vector2 textureBottomRight;

  /**
   * glyph data in pixels
   */
  vector2 size;
  vector2 offset;
  real advance;
public:
  Glyph() : textureTopLeft(0, 0), textureBottomRight(1, 1), size (10, 10), offset (0,0), advance(12) { //default values for character not found in map
  }

  Glyph(const vector2 &textureTopLeft, const vector2 &textureBottomRight, const vector2 &size, const vector2 &offset, real advance) :
    textureTopLeft(textureTopLeft),
    textureBottomRight(textureBottomRight),
    size(size),
    offset(offset) {
      this->advance = advance;
  }

  const vector2 &getTextureTopLeft() const {
    return this->textureTopLeft;
  }
  const vector2 &getTextureBottomRight() const {
    return this->textureBottomRight;
  }

  const vector2 &getSize() const {
    return this->size;
  }
  const vector2 &getOffset() const {
    return this->offset;
  }

  real getAdvance() const {
    return this->advance;
  }

};

class FontResource: public Resource {
private:
  Logger *logger = LoggerFactory::getLogger("video/font");
  const TextureResource *textureAtlas = null;
  std::unordered_map<char, Glyph> charMap;

public:
  //Inherit constructors
  using Resource::Resource;
  FontResource() : Resource(0, MimeTypes::FONT) {
  }

  FontResource *add(char character, const vector2 &textureTopLeft, const vector2 &textureBottomRight, const vector2 &size, const vector2 &offset, real advance) {
    charMap.emplace(character, Glyph(textureTopLeft, textureBottomRight, size, offset, advance));
    return this;
  }

  const Glyph &getGlyph(char character) const {
    return charMap.at(character); //return default value if not present
  }

  FontResource *setTextureAtlas(const TextureResource *textureAtlas) {
    this->textureAtlas = textureAtlas;
    return this;
  }

  const TextureResource *getTextureAtlas() const {
    return textureAtlas;
  }
};
