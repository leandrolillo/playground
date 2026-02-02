#pragma once

#include "VideoRunner.h"

class VideoRunnerMock : public VideoRunner {
public:
  VideoRunnerMock(Playground &container) : VideoRunner(container) {

  }
  void setMousePosition(unsigned int x, unsigned int y) override {

  }

  /**
   * Shader methods - should this go to a shader class?
   */

  virtual void useProgramResource(const ShaderProgramResource *program) override {

  }
  virtual bool sendUnsignedInt(const String &name, unsigned int value) const override {
    return true;
  }
  virtual bool sendReal(const String &name, const real &value) const override {
    return true;
  }
  virtual bool sendVector(const String &name, const vector &value) const override {
    return true;
  }
  virtual bool sendVector4(const String &name, const vector4 &value) const override {
    return true;
  }
  virtual bool sendMatrix(const String &name, const matriz_4x4 &value) const override {
    return true;
  }
  virtual bool sendMatrix(const String &name, const matriz_3x3 &value) const override {
    return true;
  }

  /**
   * Drawing methods
   */
  virtual void setClearColor(real r, real g, real b, real a) const override {
  }

  virtual void setTexture(unsigned int location, const TextureResource *texture, VideoAttribute type = VideoAttribute::TEXTURE_2D) override {

  }
  virtual void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, VideoAttribute type = VideoAttribute::TEXTURE_2D) override {

  }
  virtual void drawVertexArray(const VertexArrayResource *vertexArrayResource) const override {

  }

  //TODO: get rid of this method
  virtual TextureResource *getDefaultTexture() const override {
    return null;
  }
};
