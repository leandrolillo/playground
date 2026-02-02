#pragma once

#include "GeometryResourceAdapter.h"

class GeometryResourceAdapterMock : public GeometryResourceAdapter {
public:
  using GeometryResourceAdapter::GeometryResourceAdapter;

  PrimitiveType asPrimitiveTypePublic(const String &primitiveType) const {
    return asPrimitiveType(primitiveType);
  }
};
