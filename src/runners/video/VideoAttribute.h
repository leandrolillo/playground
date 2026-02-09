#pragma once

enum class VideoAttribute {
  /**
   * types
   */
  BOOLEAN,
  INT,
  UNSIGNED_INT,
  FLOAT,


  /**
   * DEPTH TEST
   */
  DEPTH_TEST,

  /**
   * Face culling options (for now they have to be defined in the same order as in opengl header)
   */
  CULL_FACE,
    NONE,
    FRONT,
    BACK,
    FRONT_AND_BACK,

/**
 * Blending options
 */
  BLEND,
    //destination factor
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,

    //source factor
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA_SATURATE,

  /*
   * Textures
   * */
  TEXTURE_2D,
  TEXTURE_CUBE_MAP,
  MAX_TEXTURES,

  /*
   * Vertex Arrays & buffers
   */
  ARRAY_BUFFER,
  ELEMENT_ARRAY_BUFFER,
  DYNAMIC_DRAW,
  STATIC_DRAW,

  LINE_WIDTH,
  RELATIVE_MOUSE_MODE,
};
