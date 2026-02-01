/*
 * VideoRunner.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#pragma once

#include "Math3d.h"
#include "Playground.h"
#include "GeometryResourceAdapter.h"
#include "JpegResourceAdapter.h"
#include "MtlResourceAdapter.h"
#include "ObjResourceAdapter.h"
#include "PngResourceAdapter.h"
#include "TgaResourceAdapter.h"
#include "LightResource.h"
#include "ShaderProgramResource.h"
#include "VertexArrayResource.h"
#include "HeightMapResourceAdapter.h"
#include "TextureResource.h"

enum class VideoAttribute { //TODO: how to map these to opengl and d3d? Maybe a map in the implementations.
  DEPTH_TEST,
  /**
   * Face culling options (for now they have to be defined in the same order as in opengl header)
   */
  CULL_FACE,
    NONE,
    FRONT_LEFT,
    FRONT_RIGHT,
    BACK_LEFT,
    BACK_RIGHT,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
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

  RELATIVE_MOUSE_MODE,
  LINE_WIDTH,
  MAX_TEXTURES
};


class VideoRunner : public PlaygroundRunner {
public:
	static const unsigned char ID;
protected:
	bool fullScreen = false;
	unsigned int height = 0;
	unsigned int width = 0;
public:

	using PlaygroundRunner::PlaygroundRunner; //inherit constructors

	virtual unsigned char getId() const override {
		return ID;
	}

	unsigned int getScreenHeight() const {
		return height;
	}

	unsigned int getScreenWidth() const {
		return width;
	}

	bool getFullscreen() const {
		return this->fullScreen;
	}

	virtual void onResize(unsigned int height, unsigned int width) override {
		this->height = height;
		this->width = width;
	}

	virtual bool move(unsigned short x, unsigned short y) {
		return true;
	}

	virtual bool setFullscreen(bool fullScreen) {
		this->fullScreen = fullScreen;

		return this->fullScreen;
	}

	virtual void resize(unsigned int height, unsigned int width) {

	}


	virtual void setMousePosition(unsigned int x, unsigned int y) = 0;

	virtual bool initialize() override {
		this->getResourceManager().addAdapter<PngResourceAdapter>();
		this->getResourceManager().addAdapter<JpegResourceAdapter>();
		this->getResourceManager().addAdapter<TgaResourceAdapter>();
		this->getResourceManager().addAdapter<GeometryResourceAdapter>();
		this->getResourceManager().addAdapter<ObjResourceAdapter>();
		this->getResourceManager().addAdapter<MtlResourceAdapter>();
    this->getResourceManager().addAdapter<HeightMapResourceAdapter>();

		return true;
	}

	/**
	 * Shader methods - should this go to a shader class?
	 */

	virtual void useProgramResource(const ShaderProgramResource *program) = 0;
	virtual bool sendUnsignedInt(const String &name, unsigned int value) const = 0;
	virtual bool sendReal(const String &name, const real &value) const = 0;
	virtual bool sendVector(const String &name, const vector &value) const = 0;
	virtual bool sendVector4(const String &name, const vector4 &value) const = 0;
	virtual bool sendMatrix(const String &name, const matriz_4x4 &value) const = 0;
	virtual bool sendMatrix(const String &name, const matriz_3x3 &value) const = 0;

	/**
	 * Drawing methods
	 */
	virtual void setClearColor(real r, real g, real b, real a) const = 0;
	virtual void enable(VideoAttribute attribute, VideoAttribute param1, VideoAttribute param2 = VideoAttribute::NONE) {};
	virtual void disable(VideoAttribute attribute) {};
	virtual void setOption(VideoAttribute attribute, real value) { };
	virtual real getRealOption(VideoAttribute attribute) const { return (real)0; };
	virtual int getIntegerOption(VideoAttribute attribute) const { return 0; };

	virtual void setTexture(unsigned int location, const TextureResource *texture, unsigned int type = 0x0DE1) = 0;
	virtual void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, unsigned int type = 0x0DE1) = 0;
	virtual void drawVertexArray(const VertexArrayResource *vertexArrayResource) const = 0;

	//TODO: get rid of this method
	virtual TextureResource *getDefaultTexture() const = 0;

	virtual String toString() const override {
		return "VideoRunner(id:" + std::to_string(this->getId()) + ")";
	}
};
