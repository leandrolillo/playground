#pragma once

#include "Resource.h"
#include "TextureResource.h"

class Glyph {
  vector2 topLeft;
  vector2 size;
  vector2 offset;
  real advance;
public:
  Glyph() : topLeft(0, 0), size (10, 10), offset (0,0), advance(12) { //default values for character not found in map

  }
  Glyph(const vector2 &topLeft, const vector2 &size, const vector2 &offset, real advance) :
    topLeft(topLeft),
    size(size),
    offset(offset) {
      this->advance = advance;
  }

  const vector2 &getTopLeft() const {
    return this->topLeft;
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
  TextureResource *textureAtlas = null;
  std::unordered_map<char, Glyph> charMap;

public:
  //Inherit constructors
  using Resource::Resource;
  FontResource() : Resource(0, MimeTypes::FONT) {
  }

  FontResource *add(char character, const vector2 &position, const vector2 &size, const vector2 &offset, real advance) {
    charMap.emplace(character, Glyph(position, size, offset, advance));
    return this;
  }

  const Glyph &getGlyph(char character) {
    return charMap[character]; //return default value if not present
  }

  FontResource *setTextureAtlas(TextureResource *textureAtlas) {
    this->textureAtlas = textureAtlas;
  }

  const TextureResource *getTextureAtlas() const {
    return textureAtlas;
  }
};
