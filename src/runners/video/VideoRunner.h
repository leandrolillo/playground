/*
 * VideoRunner.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_VIDEORUNNER_H_
#define SRC_RUNNERS_VIDEO_VIDEORUNNER_H_

#include <resources/LightResource.h>
#include <Math3d.h>
#include "../Playground.h"
#include "../openGL/adapters/TerrainResourceAdapter.h"
#include "../runners/video/adapters/GeometryResourceAdapter.h"
#include "../runners/video/adapters/JpegResourceAdapter.h"
#include "../runners/video/adapters/MtlResourceAdapter.h"
#include "../runners/video/adapters/ObjResourceAdapter.h"
#include "../runners/video/adapters/PngResourceAdapter.h"
#include "../runners/video/adapters/TgaResourceAdapter.h"
#include "../runners/video/resources/LightResource.h"
#include "../runners/video/resources/ShaderProgramResource.h"
#include "../runners/video/resources/VertexArrayResource.h"


class VideoRunner : public PlaygroundRunner, public Chronometer {
public:
	static const unsigned char ID;
protected:
	bool fullScreen;
	unsigned int height;
	unsigned int width;
public:
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

	virtual bool init() override {
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new PngResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new JpegResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new TgaResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new GeometryResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new TerrainResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new ObjResourceAdapter()));
		this->getResourceManager()->addAdapter(std::unique_ptr<ResourceAdapter>(new MtlResourceAdapter()));

		this->getContainer()->setStopWatch(this);

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

const unsigned char VideoRunner::ID = 0;



#endif /* SRC_RUNNERS_VIDEO_VIDEORUNNER_H_ */
