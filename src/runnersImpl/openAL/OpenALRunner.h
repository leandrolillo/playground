/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "al.h"
#include "alc.h"

#include "Playground.h"
#include "AudioRunner.h"
#include "AudioBufferResourceAdapter.h"

class OpenALRunner: public AudioRunner {
	private:
		Logger *logger = LoggerFactory::getLogger("audio/AudioRunner");
		ALCdevice *device = null;
		ALCcontext *context = null;

		std::vector<std::unique_ptr<AudioSource>>sources;
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

			updateListener(vector(0, 0, 0));

			return true;
		}

		AudioSource *createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), bool loop = true) override
		{
	    ALenum error = 0;

	    logger->debug("loading audio/source from [%s]", fileName.c_str());

	    AudioBufferResource *buffer = (AudioBufferResource*) getResourceManager().load(fileName, MimeTypes::AUDIOBUFFER);
	    if (buffer == null) {
	      logger->error("Error creating source: could not load buffer for [%s]", fileName.c_str());
	      return null;
	    }

	    ALuint sourceId;
	    alGenSources(1, &sourceId);
	    if ((error = alGetError()) != AL_NO_ERROR) {
	      logger->error("Error creating source for [%s]: %d", fileName.c_str(), error);
	      return null;
	    }

			this->sources.push_back(std::make_unique<AudioSource>(sourceId, position, velocity, loop));
			this->updateSource(*this->sources.back().get());

			return this->sources.back().get();
		}

		void updateSource(AudioSource &source, AudioBufferResource &audio) {
		  ALenum error = 0;

      alSourcei(source.getId(), AL_BUFFER, (ALuint) audio.getId());
      if ((error = alGetError()) != AL_NO_ERROR) {
        logger->error("Error setting properties for source [%s]: %d", audio.getUri().c_str(), error);
      }
      updateSource(source);
		}

		void updateSource(AudioSource &source) override
		{
				alSourcefv(source.getId(), AL_POSITION, (real *)source.getPosition());
				alSourcefv(source.getId(), AL_VELOCITY, (real *)source.getVelocity());
				alSourcef (source.getId(), AL_PITCH, source.getPitch());
				alSourcef (source.getId(), AL_GAIN, source.getGain());
				alSourcef (source.getId(), AL_ROLLOFF_FACTOR, source.getRolloff());
				alSourcei (source.getId(), AL_LOOPING, source.getLoop());
		}

		void playSource(AudioSource &source) override
		{
				alSourcePlay(source.getId());
		}

		void stopSource(AudioSource &source) override
		{
				alSourceStop(source.getId());
		}

		void pauseSource(AudioSource &source) override
		{
				alSourcePause(source.getId());
		}


		bool updateListener(vector position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) override
		{
			float orientation[] = {to.x, to.y, to.z, up.x, up.y, up.z};
			alListenerfv(AL_POSITION, position);
			alListenerfv(AL_VELOCITY, velocity);
			alListenerfv(AL_ORIENTATION, orientation);

			if(alGetError() != AL_NO_ERROR) {
				logger->error("Error updating Listener");
				return(false);
			}
			return(true);

		}

		virtual String toString() const override {
			return "AudioRunner(id:" + std::to_string(this->getId()) + ")";
		}


		virtual ~OpenALRunner()
		{
		  for(auto &source : sources) {
		    unsigned int sourceId = source->getId();
		    alDeleteSources(1, &sourceId);

		  }

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

	};
