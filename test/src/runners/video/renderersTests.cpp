#include <catch2/catch_test_macros.hpp>

#include "mathMatchers.h"

#include "VideoRunnerMock.h"

#include "DefaultRenderer.h"
#include "GeometryRenderer.h"
#include "TerrainRenderer.h"
#include "GridRenderer.h"
#include "SkyboxRenderer.h"
#include "SpriteRenderer.h"
#include "FontRenderer.h"

#include "Camera.h"


TEST_CASE("Video Renderers") {
  Playground playground("resources");
  VideoRunnerMock videoRunnerMock(playground);

  //TODO: implement test cases
  DefaultRenderer renderer(videoRunnerMock);
  CHECK(renderer.isEnabled() == false);
  renderer.toString();

  GridRenderer gridRenderer(videoRunnerMock);
  CHECK(gridRenderer.isEnabled() == false);
  gridRenderer.toString();

  SkyboxRenderer skyboxRenderer(videoRunnerMock);
  CHECK(skyboxRenderer.isEnabled() == false);
  skyboxRenderer.toString();

  TerrainRenderer terrainRenderer(videoRunnerMock);
  CHECK(terrainRenderer.isEnabled() == false);
  terrainRenderer.toString();

  SpriteRenderer spriteRenderer(videoRunnerMock);
  CHECK(spriteRenderer.isEnabled() == false);
  spriteRenderer.toString();

  Sprite sprite(null, vector2(0, 0), vector2(100, 100));

  FontRenderer fontRenderer(videoRunnerMock);
  CHECK(spriteRenderer.isEnabled() == false);
  fontRenderer.toString();
}

TEST_CASE("Geometry (defaultRenderer) Render") {
  Playground playground("resources");
  VideoRunnerMock videoRunnerMock(playground);

  //TODO: implement test cases
  DefaultRenderer renderer(videoRunnerMock);
  CHECK(renderer.isEnabled() == false);
  renderer.toString();

  GeometryRenderer geometryRenderer(renderer);
}


