/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "OggResourceAdapter.h"
#include "WavResourceAdapter.h"
#include "AudioSource.h"
#include "Playground.h"

class AudioRunner: public PlaygroundRunner {
  friend class AudioSource;
	public:
		static const unsigned char ID;
	protected:
		Logger *logger = LoggerFactory::getLogger("audio/AudioRunner");
	public:
		using PlaygroundRunner::PlaygroundRunner; //inherit constructors

		virtual unsigned char getId() const override {
			return ID;
		}

		virtual bool initialize() override {
			this->getResourceManager().addAdapter<OggResourceAdapter>();
			this->getResourceManager().addAdapter<WavResourceAdapter>();
			return true;
		}

		/**
		 * Factory methods
		 */
		virtual std::unique_ptr<AudioSource> createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), bool looping = true) {
      AudioBufferResource *buffer = (AudioBufferResource*) getResourceManager().load(fileName, MimeTypes::AUDIOBUFFER);
      if (buffer == null) {
        logger->error("Error creating source: could not load buffer for [%s]", fileName.c_str());
        return null;
      }

      return std::make_unique<AudioSource>(*this, *buffer, position, velocity, looping);
		}

		/**
		 * listener methods
		 */
		virtual bool updateListener(const vector &position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) = 0;

		virtual String toString() const override {
			return "AudioRunner(id:" + std::to_string(this->getId()) + ")";
		}


		virtual ~AudioRunner()
		{
		}
	protected:
    /**
     * source wrapper methods
     */
    virtual unsigned int createSource() = 0;
    virtual void deleteSource(unsigned int sourceId) = 0;
    virtual void setSourceBuffer(unsigned int sourceId, unsigned int bufferId) = 0;
    virtual int getSourceBuffer(unsigned int sourceId) = 0;
    virtual void setSourcePosition(unsigned int sourceId, const vector &position) = 0;
    virtual vector getSourcePosition(unsigned int sourceId) = 0;
    virtual void setSourceVelocity(unsigned int sourceId, const vector &velocity) = 0;
    virtual vector getSourceVelocity(unsigned int sourceId) = 0;
    virtual void setSourcePitch(unsigned int sourceId, real pitch) = 0;
    virtual real getSourcePitch(unsigned int sourceId) = 0;
    virtual void setSourceGain(unsigned int sourceId, real gain) = 0;
    virtual real getSourceGain(unsigned int sourceId) = 0;
    virtual void setSourceRolloffFactor(unsigned int sourceId, real roloffFactor) = 0;
    virtual real getSourceRolloffFactor(unsigned int sourceId) = 0;
    virtual void setSourceLooping(unsigned int sourceId, bool looping) = 0;
    virtual bool getSourceLooping(unsigned int sourceId) = 0;

    /**
     * playback methods
     */
    virtual void playSource(AudioSource &source) = 0;
    virtual void stopSource(AudioSource &source) = 0;
    virtual void pauseSource(AudioSource &source) = 0;

	};
