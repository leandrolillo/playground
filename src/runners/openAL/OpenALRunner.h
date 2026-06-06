/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "AudioRunner.h"
#include "al.h"
#include "alc.h"

#include "Playground.h"
#include "AudioBufferResourceAdapter.h"

class OpenALRunner: public AudioRunner {
  friend class AudioSource;
  private:
    Logger *logger = LoggerFactory::getLogger("audio/OpenALRunner");
    ALCdevice *device = null;
    ALCcontext *context = null;
  public:
    using AudioRunner::AudioRunner; //inherit constructors

    virtual bool initialize() override {
      AudioRunner::initialize();
      //this->getResourceManager().addAdapter<SourceResourceAdapter>();
      this->getResourceManager().addAdapter<AudioBufferResourceAdapter>();

      device = alcOpenDevice(null);
      if(device == null) {
        logger->error("Error opening alcDevice");
        return false;
      }

      logger->debug("OpenAL device opened");

      context = alcCreateContext(device, null);
      if(context == null)
      {
        logger->error("Error creating context");
        return false;
      }

      logger->debug("OpenAL context created");

      alcMakeContextCurrent(context);

      logger->debug("OpenAL context set");
      alGetError(); // clear error code

      logger->info("\nOpenAL [%s] initialized.\n\tVendor: [%s]\n\tRenderer: [%s]\n\tExtensions: [%s]",
          alGetString(AL_VERSION),
          alGetString(AL_VENDOR),
          alGetString(AL_RENDERER),
          alGetString(AL_EXTENSIONS));

      updateListener(vector(0, 0, 0));

      return true;
    }

    virtual String toString() const override {
      return "AudioRunner(id:" + std::to_string(this->getId()) + ")";
    }


    virtual ~OpenALRunner()
    {
      alcMakeContextCurrent(NULL);
      if(context != null) {
        alcDestroyContext(context);
        logger->debug("OpenAL context destroyed");
      }

      if(device != null) {
        alcCloseDevice(device);
        logger->debug("OpenAL device destroyed");
      }
    }

  protected:

    /**
     * source wrapper methods
     */
    unsigned int createSource() override {
      unsigned int sourceId = 0;
      alGenSources(1, &sourceId);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
        return 0;
      }

      return sourceId;
    }

    void deleteSource(unsigned int sourceId) override {
      alDeleteSources(1, &sourceId);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }

    void setSourceBuffer(unsigned int sourceId, unsigned int audioBufferId) override {
      alSourcei(sourceId, AL_BUFFER, audioBufferId);
    }
    int getSourceBuffer(unsigned int sourceId) override {
      int result = 0;
      alGetSourcei(sourceId, AL_BUFFER, &result);
      return result;

    }

    void setSourcePosition(unsigned int sourceId, const vector &position) override {
      alSourcefv(sourceId, AL_POSITION, (real *)position);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    vector getSourcePosition(unsigned int sourceId) override {
      vector result;
      alGetSourcefv(sourceId, AL_POSITION, (real *)result);
      return result;

    }

    void setSourceVelocity(unsigned int sourceId, const vector &velocity) override {
      alSourcefv(sourceId, AL_VELOCITY, (real *)velocity);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    vector getSourceVelocity(unsigned int sourceId) override {
      vector result;
      alGetSourcefv(sourceId, AL_VELOCITY, (real *)result);
      return result;

    }

    void setSourcePitch(unsigned int sourceId, real pitch) override {
      alSourcef(sourceId, AL_PITCH, pitch);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    real getSourcePitch(unsigned int sourceId) override {
      real result = 0;
      alGetSourcef(sourceId, AL_PITCH, &result);
      return result;
    }

    void setSourceGain(unsigned int sourceId, real gain) override {
      alSourcef(sourceId, AL_GAIN, gain);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    real getSourceGain(unsigned int sourceId) override {
      real result = 0;
      alGetSourcef(sourceId, AL_GAIN, &result);
      return result;

    }

    void setSourceRolloffFactor(unsigned int sourceId, real rolloffFactor) override {
      alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloffFactor);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    real getSourceRolloffFactor(unsigned int sourceId) override {
      real result = 0;
      alGetSourcef(sourceId, AL_ROLLOFF_FACTOR, &result);
      return result;

    }

    void setSourceLooping(unsigned int sourceId, bool looping) override {
      alSourcei(sourceId, AL_LOOPING, looping);
      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
      }
    }
    bool getSourceLooping(unsigned int sourceId) override {
      int result = 0;
      alGetSourcei(sourceId, AL_LOOPING, &result);
      return (bool)result;
    }


    /**
     * playback methods
     */
    void playSource(AudioSource &source) override
    {
        alSourcePlay(source.getId());
        if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
          logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
        }

    }

    void stopSource(AudioSource &source) override
    {
        alSourceStop(source.getId());
        if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
          logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
        }

    }

    void pauseSource(AudioSource &source) override
    {
        alSourcePause(source.getId());
        if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
          logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
        }
    }


    /**
     * listener methods
     */
    bool updateListener(const vector &position, const vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) override
    {
      float orientation[] = {to.x, to.y, to.z, up.x, up.y, up.z};
      alListenerfv(AL_POSITION, position);
      alListenerfv(AL_VELOCITY, velocity);
      alListenerfv(AL_ORIENTATION, orientation);

      if (ALenum error = alGetError() ; error != AL_NO_ERROR) {
        logger->error("Error at [%s]: [%d]", __PRETTY_FUNCTION__, error);
        return false;
      }

      return(true);

    }
  };
