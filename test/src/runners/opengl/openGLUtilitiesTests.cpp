#include <catch2/catch_test_macros.hpp>

#include "OpenGLRunner.h"
#include "OpenGLUtilities.h"

TEST_CASE("OpenGLUtilities tests", "[opengl]") {
  CHECK(GL_BYTE == OpenGLUtilities::asGlAttribute(VideoAttribute::BOOLEAN));
  CHECK(GL_INT == OpenGLUtilities::asGlAttribute(VideoAttribute::INT));
  CHECK(GL_UNSIGNED_INT == OpenGLUtilities::asGlAttribute(VideoAttribute::UNSIGNED_INT));
  CHECK(GL_FLOAT == OpenGLUtilities::asGlAttribute(VideoAttribute::FLOAT));

  CHECK(GL_POINTS == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::POINTS));
  CHECK(GL_LINE_LOOP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINE_LOOP));
  CHECK(GL_LINE_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINE_STRIP));
  CHECK(GL_LINES == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINES));
  CHECK(GL_TRIANGLES == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLES));
  CHECK(GL_TRIANGLE_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLE_STRIP));
  CHECK(GL_TRIANGLE_FAN == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLE_FAN));
  CHECK(GL_QUADS == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::QUADS));
  CHECK(GL_QUAD_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::QUAD_STRIP));
  CHECK(GL_POLYGON == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::POLYGON));

  //GL_POINTS,GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON

  CHECK(GL_DEPTH_TEST == OpenGLUtilities::asGlAttribute(VideoAttribute::DEPTH_TEST));
  CHECK(GL_CULL_FACE == OpenGLUtilities::asGlAttribute(VideoAttribute::CULL_FACE));
  CHECK(GL_NONE == OpenGLUtilities::asGlAttribute(VideoAttribute::NONE));
  CHECK(GL_FRONT == OpenGLUtilities::asGlAttribute(VideoAttribute::FRONT));
  CHECK(GL_BACK == OpenGLUtilities::asGlAttribute(VideoAttribute::BACK));
  CHECK(GL_FRONT_AND_BACK == OpenGLUtilities::asGlAttribute(VideoAttribute::FRONT_AND_BACK));

  CHECK(GL_BLEND == OpenGLUtilities::asGlAttribute(VideoAttribute::BLEND));
  CHECK(GL_SRC_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_COLOR));
  CHECK(GL_ONE_MINUS_SRC_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_SRC_COLOR));
  CHECK(GL_SRC_ALPHA == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_ALPHA));
  CHECK(GL_ONE_MINUS_SRC_ALPHA == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_SRC_ALPHA));
  CHECK(GL_DST_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::DST_COLOR));
  CHECK(GL_ONE_MINUS_DST_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_DST_COLOR));
  CHECK(GL_SRC_ALPHA_SATURATE == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_ALPHA_SATURATE));


  CHECK(GL_TEXTURE_2D == OpenGLUtilities::asGlAttribute(VideoAttribute::TEXTURE_2D));
  CHECK(GL_TEXTURE_CUBE_MAP == OpenGLUtilities::asGlAttribute(VideoAttribute::TEXTURE_CUBE_MAP));
  CHECK(GL_MAX_TEXTURE_IMAGE_UNITS == OpenGLUtilities::asGlAttribute(VideoAttribute::MAX_TEXTURES));

  CHECK(GL_ARRAY_BUFFER == OpenGLUtilities::asGlAttribute(VideoAttribute::ARRAY_BUFFER));
  CHECK(GL_ELEMENT_ARRAY_BUFFER == OpenGLUtilities::asGlAttribute(VideoAttribute::ELEMENT_ARRAY_BUFFER));
  CHECK(GL_DYNAMIC_DRAW == OpenGLUtilities::asGlAttribute(VideoAttribute::DYNAMIC_DRAW));
  CHECK(GL_STATIC_DRAW == OpenGLUtilities::asGlAttribute(VideoAttribute::STATIC_DRAW));

  CHECK(GL_LINE_WIDTH == OpenGLUtilities::asGlAttribute(VideoAttribute::LINE_WIDTH));
}
