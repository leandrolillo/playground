/*
 * TgaResourceAdapter.h
 *
 *  Created on: 19/02/2013
 *      Author: Lean
 */

#pragma once

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "ResourceAdapter.h"
#include "ImageResource.h"
#include "FontResource.h"

class TrueTypeResourceAdapter: public ResourceAdapter {
public:

  TrueTypeResourceAdapter(ResourceManager &resourceManager) : ResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/FontResourceAdapter");
    this->accepts(MimeTypes::TRUE_TYPE_FONT);
    this->produces(MimeTypes::FONT);
  }

protected:
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;

    FontResource *font = new FontResource();

    ImageResource *textureAtlas = new ImageResource();
    textureAtlas->setUri(request.getFilePath()).addLabel(ResourceManager::EphemeralLabel);
    textureAtlas->resize(512, 512, 8);

    FT_Library ft;

    if (FT_Init_FreeType(&ft) != FT_Err_Ok)
    {
      logger->error("Could not initialize FreeType library");
      return response;
    }

    FT_Face face;
    if (FT_New_Face(ft, request.getFilePath().c_str(), 0, &face) != FT_Err_Ok)
    {
        logger->error("Could not load font [%s]", request.getFilePath().c_str());
        FT_Done_FreeType(ft);
        return response;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    FT_GlyphSlot glyph = face->glyph;

    //where to copy the character in the atlas
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int max_glyph_height = 0;

    for (unsigned char character = 32; character < 128; character++)
    {
      /* retrieve glyph index from character code */
      auto glyph_index = FT_Get_Char_Index( face, character );

      if (FT_Load_Char(face, glyph_index, FT_LOAD_RENDER) == FT_Err_Ok) {
        max_glyph_height = std::max(max_glyph_height, glyph->bitmap.rows);

        if(x + glyph->bitmap.width >= textureAtlas->getWidth()) {
          y += max_glyph_height;
          x = 0;
          max_glyph_height = 0;
        }
        font->add(character,
            vector2((real)x / textureAtlas->getWidth(), (real)y / textureAtlas->getHeight()),
            vector2((real)(x + glyph->bitmap.width) / textureAtlas->getWidth(), (real)(y + glyph->bitmap.rows) / textureAtlas->getHeight()),
            vector2(glyph->bitmap.width, glyph->bitmap.rows),
            vector2(glyph->bitmap_left, glyph->bitmap_top),
            glyph->advance.x >> 6);
        textureAtlas->copy(x, y, glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.pitch / glyph->bitmap.width * 8);

        x += glyph->bitmap.width;
      } else {
        logger->error("Could not load character [%c]", character);
      }
    }

    getResourceManager().addResource(textureAtlas);
    font->setTextureAtlas((TextureResource *)getResourceManager().load(request.getFilePath(), MimeTypes::TEXTURE)); //Should come from cache.
    response.push_back(font);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return response;
  }
};
