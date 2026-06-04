/*
 * OpenALRunner.h
 */

#pragma once

#include "AudioRunner.h"
#include "BufferResourceAdapter.h"
#include "SourceResourceAdapter.h"

#include "al.h"
#include "alc.h"

class OpenALRunner: public AudioRunner {
private:
  Logger *logger = LoggerFactory::getLogger("audio/OpenALRunner.h");
  ALCdevice *device = null;
  ALCcontext *context = null;

public:
  using AudioRunner::AudioRunner;

  virtual bool initialize() override {
    AudioRunner::initialize();
    this->getResourceManager().addAdapter<SourceResourceAdapter>();
    this->getResourceManager().addAdapter<BufferResourceAdapter>();

    device = alcOpenDevice(null);
    if (device == null) {
      logger->error("Error opening alcDevice");
      return false;
    }

    logger->debug("OpenAL device opened");

    context = alcCreateContext(device, null);
    if (context == null) {
      logger->error("Error creating context");
      return false;
    }

    logger->debug("OpenAL context created");

    alcMakeContextCurrent(context);

    logger->debug("OpenAL context set");
    alGetError(); // clear error code

    updateListener(vector(0, 0, 0));

    return true;
  }

  virtual Source *updateSource(Source *source) override {
    if (source != null) {
      alSourcefv(source->getId(), AL_POSITION, (real *) source->getPosition());
      alSourcefv(source->getId(), AL_VELOCITY, (real *) source->getVelocity());
      alSourcef(source->getId(), AL_PITCH, source->getPitch());
      alSourcef(source->getId(), AL_GAIN, source->getGain());
      alSourcef(source->getId(), AL_ROLLOFF_FACTOR, source->getRolloff());
      alSourcei(source->getId(), AL_LOOPING, source->getLoop());
    }
    return source;
  }

  virtual Source *playSource(Source *source) override {
    if (source != null) {
      alSourcePlay(source->getId());
    }
    return source;
  }

  virtual Source *stopSource(Source *source) override {
    if (source != null) {
      alSourceStop(source->getId());
    }
    return source;
  }

  virtual Source *pauseSource(Source *source) override {
    if (source != null) {
      alSourcePause(source->getId());
    }
    return source;
  }

  virtual bool updateListener(vector position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) override {
    float orientation[] = {to.x, to.y, to.z, up.x, up.y, up.z};
    alListenerfv(AL_POSITION, position);
    alListenerfv(AL_VELOCITY, velocity);
    alListenerfv(AL_ORIENTATION, orientation);

    if (alGetError() != AL_NO_ERROR) {
      logger->error("Error updating Listener");
      return false;
    }
    return true;
  }

  virtual String toString() const override {
    return "OpenALRunner(id:" + std::to_string(this->getId()) + ")";
  }

  virtual ~OpenALRunner() {
    alcMakeContextCurrent(NULL);
    if (context != null) {
      alcDestroyContext(context);
      logger->debug("OpenAL context destroyed");
    }

    if (device != null) {
      alcCloseDevice(device);
      logger->debug("OpenAL device destroyed");
    }
  }
};
