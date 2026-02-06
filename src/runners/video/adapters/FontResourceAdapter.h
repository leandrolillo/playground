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
#include "FontResource.h"

class FontResourceAdapter: public ResourceAdapter {
public:

  FontResourceAdapter(ResourceManager &resourceManager) : ResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/FontResourceAdapter");
    this->accepts(MimeTypes::TRUE_TYPE_FONT);
    this->produces(MimeTypes::FONT);
  }

protected:
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;

    FontResource *font = new FontResource();

    ImageResource textureAtlas;
    textureAtlas.setUri(request.getFilePath()).setName("textureAtlas");
    textureAtlas.resize(512, 512, 8);


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

    for (unsigned char character = 0; character < 128; character++)
    {
      /* retrieve glyph index from character code */
      auto glyph_index = FT_Get_Char_Index( face, character );

      if (FT_Load_Char(face, glyph_index, FT_LOAD_RENDER) == FT_Err_Ok) {
        font->add(character, vector2(0, 0), vector2(glyph->bitmap.width, glyph->bitmap.rows), vector2(glyph->bitmap_left, glyph->bitmap_top), glyph->advance.x);
        textureAtlas.copy(vector2(0, 0), glyph->bitmap.buffer, vector2(0, 0), vector2(glyph->bitmap.width, glyph->bitmap.rows));
      } else {
        logger->error("Could not load character [%c]", character);
      }
    }

    //getResourceManager().addResource(textureAtlas);
    response.push_back(font);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return response;
  }
};
