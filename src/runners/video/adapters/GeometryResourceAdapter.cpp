#include "GeometryResourceAdapter.h"

const std::unordered_map<String, PrimitiveType> GeometryResourceAdapter::primitiveTypes = {
  { "points", PrimitiveType::POINTS },
  { "lineloop", PrimitiveType::LINE_LOOP },
  { "linestrip", PrimitiveType::LINE_STRIP },
  { "lines", PrimitiveType::LINES },
  { "triangles", PrimitiveType::TRIANGLES },
  { "trianglestrip", PrimitiveType::TRIANGLE_STRIP },
  { "trianglefan", PrimitiveType::TRIANGLE_FAN },
  { "quads", PrimitiveType::QUADS },
  { "quadstrip", PrimitiveType::QUAD_STRIP },
  { "polygon", PrimitiveType::POLYGON }
};
