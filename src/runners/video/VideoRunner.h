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
		this->getResourceManager().addAdapter(std::make_unique<PngResourceAdapter>());
		this->getResourceManager().addAdapter(std::make_unique<JpegResourceAdapter>());
		this->getResourceManager().addAdapter(std::make_unique<TgaResourceAdapter>());
		this->getResourceManager().addAdapter(std::make_unique<GeometryResourceAdapter>());
		this->getResourceManager().addAdapter(std::make_unique<ObjResourceAdapter>());
		this->getResourceManager().addAdapter(std::make_unique<MtlResourceAdapter>());
    this->getResourceManager().addAdapter(std::make_unique<HeightMapResourceAdapter>());

		return true;
	}

//	/**
//	 * Performance counters
//	 */
//	virtual unsigned long getPerformanceCounter() const = 0;
//	virtual unsigned long getPerformanceFreq() const = 0;

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
	virtual void enable(unsigned int attributeCode, unsigned int param1, unsigned int param2 = 0) {};
	virtual void disable(unsigned int attributeCode) {};
	virtual void setOption(unsigned int attributeCode, real value) { };
	virtual real getOption(unsigned int attributeCode) { return (real)0; };

	virtual void setTexture(unsigned int location, const TextureResource *texture, unsigned int type = 0x0DE1) = 0;
	virtual void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, unsigned int type = 0x0DE1) = 0;
	virtual void drawVertexArray(const VertexArrayResource *vertexArrayResource) const = 0;

	//TODO: get rid of this method
	virtual TextureResource *getDefaultTexture() const = 0;

	virtual String toString() const override {
		return "VideoRunner(id:" + std::to_string(this->getId()) + ")";
	}
};
