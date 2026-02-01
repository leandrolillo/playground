#include "OpenGLUtilities.h"

const std::unordered_map<PrimitiveType, GLenum> OpenGLUtilities::glPrimitiveTypes = {
  { PrimitiveType::POINTS, GL_POINTS },
  { PrimitiveType::LINE_LOOP, GL_LINE_LOOP },
  { PrimitiveType::LINE_STRIP, GL_LINE_STRIP },
  { PrimitiveType::LINES, GL_LINES },
  { PrimitiveType::TRIANGLES, GL_TRIANGLES },
  { PrimitiveType::TRIANGLE_STRIP, GL_TRIANGLE_STRIP },
  { PrimitiveType::TRIANGLE_FAN, GL_TRIANGLE_FAN },
  { PrimitiveType::QUADS, GL_QUADS },
  { PrimitiveType::QUAD_STRIP, GL_QUAD_STRIP },
  { PrimitiveType::POLYGON, GL_POLYGON }
};

