#pragma once

#include<ParticleManager.h>
#include "Playground.h"

class PhysicsRunner: public PlaygroundRunner {
	Logger *logger = LoggerFactory::getLogger("physics/PhysicsRunner");

	ParticleManager particleManager;

	real playbackSpeed = 1.0f;

	/**
	 * stats
	 */
	unsigned long frames = 0;
public:
	static const unsigned char ID;
public:
	PhysicsRunner(Playground &container) : PlaygroundRunner(container) {
	}

	unsigned char getId() const override {
		return ID;
	}

	ParticleManager &getParticleManager() {
		return this->particleManager;
	}

	void setPlaybackSpeed(real playbackspeed) {
	    this->playbackSpeed = playbackspeed;
	}

	virtual bool afterInitialize() override {
		this->start();

		return true;
	}


	void beforeLoop() override {
		this->particleManager.clearAccumulators();
	}

	void start() {
		logger->info("Starting physics runner");
		this->setEnabled(true);

	}

	void stop() {
		logger->info("Stopping physics runner");
		this->setEnabled(false);
	}

	void step(real dt) {
		if(!this->getEnabled()) {
			this->beforeLoop();
			this->particleManager.step(dt);
		}
	}

	LoopResult doLoop() override {
		real dt = getStopWatch().getElapsedTime();

		this->particleManager.step(dt * playbackSpeed);

		/**
		 * stats
		 */
//		frames++;
//      real fps = (real)frames / this->stopWatch->getTotalTime();
//		printf("Elapsed time: %.2f, frames: %u, fps:%.2f. View position: %s - count = %d\r",
//				elapsedTime,
//				frames,
//				fps,
//				viewPosition.toString().c_str(),
//				count);

		return LoopResult::CONTINUE;
	}

	virtual String toString() const override {
		return "PhysicsRunner(id:" + std::to_string(this->getId()) + ")";
	}

};

const unsigned char PhysicsRunner::ID = 4;
