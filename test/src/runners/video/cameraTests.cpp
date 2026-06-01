#include <catch2/catch_test_macros.hpp>
#include "mathMatchers.h"

#include "Camera.h"
#include "ArcBall.h"

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

TEST_CASE("Camera orientation and position updates affect view") {
  Camera camera;

  camera.setOrientation(matriz_3x3::identidad);
  camera.setPosition(vector3(2, 3, 4));

  CHECK_THAT(camera.getPosition(), EqualsVector(vector3(2, 3, 4)));
  CHECK_THAT(camera.getForward(), EqualsVector(vector3(0, 0, 1)));
  CHECK(camera.getZNear() > 0);
  CHECK(camera.getZFar() > camera.getZNear());
}

TEST_CASE("ArcBall tracks dragging state and rotation") {
  ArcBall arcBall;

  CHECK_FALSE(arcBall.isDragging());

  arcBall.startDrag(vector2(50, 50), matriz_3x3::identidad, 200, 200);
  CHECK(arcBall.isDragging());

  matriz_3x3 rotation = arcBall.drag(vector2(30, 0));
  vector rotatedForward = rotation * vector3(0, 0, 1);
  CHECK_FALSE(rotatedForward == vector3(0, 0, 1));

  arcBall.endDrag(vector2(120, 50));
  CHECK_FALSE(arcBall.isDragging());
}
