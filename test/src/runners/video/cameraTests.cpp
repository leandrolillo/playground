#include <catch2/catch_test_macros.hpp>
#include "mathMatchers.h"

#include "Camera.h"

TEST_CASE("Camera tests") {
  Camera camera;

  camera.setPerspectiveProjection(800, 600, 0.1, 300.0);
  camera.setPerspectiveProjectionFov(45.0, (real)800/ (real)600, 0.1, 300.0);
  camera.setOrthographicProjection(0, 0, 800, 600, -1, 1);
  camera.setOrthographicProjection(800, 600, -1, 1);
}

TEST_CASE("MousePicking tests") {
  unsigned int width = 640;
  unsigned int height = 480;

  Camera camera;
  camera.setPerspectiveProjectionFov(45.0, (real) width / (real) height, 0.1, 300.0);
  camera.setViewMatrix(matriz_4x4::identidad);

  vector rayDirection = camera.unproject(320, 240, width, height).normalizado();
  CHECK(vector(0, 0, -1) == rayDirection);

  camera.setOrthographicProjection(640, 480, -100, 100);
  camera.setViewMatrix(matriz_4x4::identidad);

  vector unprojected = camera.unproject(320, 240, width, height);
  CHECK_THAT(rayDirection, EqualsVector(vector(0, 0, -1)));

  unprojected = camera.unproject(0, 0, width, height);
  CHECK_THAT(rayDirection, EqualsVector(vector(-320, 240, -1)));
}
