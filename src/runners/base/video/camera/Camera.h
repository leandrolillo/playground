/*
 * Camera.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#pragma once

#include<Math3d.h>
#include "Geometry.h"
#include "Logger.h"

class Camera {
protected:
  Logger *logger = LoggerFactory::getLogger("Camera");

  matriz_4x4 projectionMatrix;
  matriz_4x4 viewMatrix;
  matriz_4x4 projectionViewMatrix;

  vector position;
  matriz_3x3 orientation;

  real near = 0.1;
  real far = 300;
  vector topNormal;
  vector bottomNormal;
  vector leftNormal;
  vector rightNormal;

  Frustum frustum;
  bool frustumUpdated = false;

protected:
  void setProjectionMatrix(const matriz_4x4 &projectionMatrix) {
    this->projectionMatrix = projectionMatrix;
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
  }

  void updateFrustrumNormals(real width, real height, real near, real far) {
    vector front = orientation.columna(2);
    vector right = orientation.columna(0);
    vector up = orientation.columna(1);

    vector frontTimesNear = front * near;

    /**
     * Update frustrum normals according to the new camera projection - might need to refactor for orthographic projection
     */
    topNormal = ((frontTimesNear + up * height * 0.5) ^ right).normalizado();
    bottomNormal = (right ^ (frontTimesNear - up * height * 0.5)).normalizado();

    rightNormal = (up ^ (frontTimesNear + right * width * 0.5)).normalizado();
    leftNormal = ((frontTimesNear - right * width * 0.5) ^ up).normalizado();

    /**
     * Keep track of Z near and far
     */
    this->near = near;
    this->far = far;

    frustumUpdated = false;
  }

  void updateFrustum() {
    frustum.getHalfSpace(0).setOrigin(-this->position);
    frustum.getHalfSpace(0).setNormal(this->orientation.traspuesta() * leftNormal);

    frustum.getHalfSpace(1).setOrigin(-this->position);
    frustum.getHalfSpace(1).setNormal(this->orientation.traspuesta() * rightNormal);

    frustum.getHalfSpace(2).setOrigin(-this->position);
    frustum.getHalfSpace(2).setNormal(this->orientation.traspuesta() * topNormal);

    frustum.getHalfSpace(3).setOrigin(-this->position);
    frustum.getHalfSpace(3).setNormal(this->orientation.traspuesta() * bottomNormal);

    vector front = orientation.columna(2);
    frustum.getHalfSpace(4).setOrigin(-this->position + front * this->near);
    frustum.getHalfSpace(4).setNormal(this->orientation.traspuesta() * -front);

    frustum.getHalfSpace(5).setOrigin(-this->position + front * this->far * 0.1);
    frustum.getHalfSpace(5).setNormal(this->orientation.traspuesta() * front);
  }

public:
  Camera() : frustum(
      { Plane(vector(), vector(1, 0, 0)), Plane(vector(), vector(1, 0, 0)), Plane(vector(), vector(1, 0, 0)), Plane(vector(),
          vector(1, 0, 0)), Plane(vector(), vector(1, 0, 0)), Plane(vector(), vector(1, 0, 0)) }) {
    /**
     * Default to perspective projection.
     */
    setPerspectiveProjectionFov(45.0, 800.0 / 600.0, 2, 30);
  }
  /**
   * Mode, view and projection matrixes
   */
  const matriz_4x4& getProjectionMatrix() const {
    return this->projectionMatrix;
  }

  void setPerspectiveProjectionFov(real fovy, real aspect, real near, real far) {
    real height = 2.0 * tan(radian(fovy) * 0.5) * near; // half height = tan of half the fov times near
    setPerspectiveProjection(height * aspect, height, near, far);
  }

  /**
   * This is opengl style perspective projection, where height and width are the dimensions on the near plane
   */
  void setPerspectiveProjection(real width, real height, real near, real far) {
    setProjectionMatrix(matriz_4x4::perspectiveProjection(width, height, near, far));
    updateFrustrumNormals(width, height, near, far);
  }

  void setOrthographicProjection(real width, real height, real near, real far) { //This sets orthograpic projection with (0,0) in the center of the screen (instead of top left)
    setProjectionMatrix(matriz_4x4::orthographicProjection(width, height, near, far));
    updateFrustrumNormals(width, height, near, far);
  }

  void setOrthographicProjection(real left, real top, real right, real bottom, real near, real far) { //This sets orthograpic projection with (0,0) in the center of the screen (instead of top left)
    setProjectionMatrix(matriz_4x4::orthographicProjection(left, top, right, bottom, near, far));
    updateFrustrumNormals(right - left, bottom - top, near, far);
  }

  const matriz_4x4& getProjectionViewMatrix() const {
    return this->projectionViewMatrix;
  }

  void setViewMatrix(const matriz_4x4 &viewMatrix) {
    this->viewMatrix = viewMatrix;
    this->orientation = (matriz_3x3) viewMatrix;
    this->position = -(vector3) ((matriz::rotacion(orientation.traspuesta()) * this->viewMatrix).columna(3));
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;

    this->frustumUpdated = false;

  }

  const matriz_4x4& getViewMatrix() const {
    return this->viewMatrix;
  }

  matriz_3x3 getOrientation() const {
    return this->orientation.traspuesta();
  }

  void setOrientation(const matriz_3x3 &orientation) {
    this->orientation = orientation;

    this->viewMatrix = matriz::rotacion(orientation) * matriz::traslacion(-position);
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;

    this->frustumUpdated = false;
  }

  const vector getPosition() const {
    return this->position;
  }

  void setPosition(const vector &position) {
    this->position = position;

    this->viewMatrix = matriz::rotacion(orientation) * matriz::traslacion(-position);
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;

    this->frustumUpdated = false;
  }

  vector getForward() const {
    return this->getOrientation().columna(2);
  }

  real getZNear() const {
    return this->near;
  }

  real getZFar() const {
    return this->far;
  }

  Frustum& getFrustum() {
    if (!frustumUpdated) {
      this->updateFrustum();
      frustumUpdated = true;
    }

    return frustum;
  }

  /**
   * From https://antongerdelan.net/opengl/raycasting.html
   * Basically pipeline is modelMatrix * viewMatrix * projectionMatrix -> normalised Device Space -> viewport, then we do the opposite process:
   *
   *   viewMatrix-1 * projectionMatrix-1 * (normalized device coordinates <- viewport(x, y))
   */

  vector unproject(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const {
    //Viewport to normalized device coordinates - result should be in range [-1:1, -1:1, -1:1, -1:1]
    vector3 normalizedDeviceCoordinates = vector3(
        (real) 2 * (real) x / (real) width - (real) 1,
        (real) 1 - (real) 2 * (real) y / (real) height,
        -1);

    vector4 homogeneousClipCoordinates(
        normalizedDeviceCoordinates.x,
        normalizedDeviceCoordinates.y,
        normalizedDeviceCoordinates.z,
        1);

    vector4 cameraCoordinates = projectionMatrix.inversa() * homogeneousClipCoordinates;
    cameraCoordinates.z = (real) -1;
    cameraCoordinates.w = (real) 0;

    return (vector3) (viewMatrix.inversa() * cameraCoordinates);
  }

  String toString() {
    return "Camera([" + this->getPosition().toString() + "], front[" + this->getOrientation().columna(2).toString() + "])";
  }
};
