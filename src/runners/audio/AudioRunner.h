/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "Playground.h"
#include "BufferResourceAdapter.h"
#include "OggResourceAdapter.h"
#include "SourceResourceAdapter.h"
#include "WavResourceAdapter.h"

#include "al.h"
#include "alc.h"

class AudioRunner: public PlaygroundRunner {
	private:
		Logger *logger = LoggerFactory::getLogger("audio/AudioRunner.h");
		ALCdevice *device = null;
		ALCcontext *context = null;
	public:
		static const unsigned char ID;
	public:
		using PlaygroundRunner::PlaygroundRunner; //inherit constructors

		virtual unsigned char getId() const override {
			return ID;
		}

		virtual bool initialize() override {
			this->getResourceManager().addAdapter(std::make_unique<SourceResourceAdapter>());
			this->getResourceManager().addAdapter(std::make_unique<BufferResourceAdapter>());
			this->getResourceManager().addAdapter(std::make_unique<OggResourceAdapter>());
			this->getResourceManager().addAdapter(std::make_unique<WavResourceAdapter>());

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

		Source *createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), bool loop = true)
		{
			Source *source = (Source *)getResourceManager().load(fileName, MimeTypes::AUDIOSOURCE);
			if(source != null)
			{
				source->setPosition(position);
				source->setVelocity(velocity);
				source->setLoop(loop);
			}

			return this->updateSource(source);
		}
		Source * updateSource(Source *source)
		{
			if(source != null) {
				alSourcefv(source->getId(), AL_POSITION, (real *)source->getPosition());
				alSourcefv(source->getId(), AL_VELOCITY, (real *)source->getVelocity());
				alSourcef (source->getId(), AL_PITCH, source->getPitch());
				alSourcef (source->getId(), AL_GAIN, source->getGain());
				alSourcef (source->getId(), AL_ROLLOFF_FACTOR, source->getRolloff());
				alSourcei (source->getId(), AL_LOOPING, source->getLoop());
			}
			return source;
		}

		Source * playSource(Source *source)
		{
			if(source != null) {
				alSourcePlay(source->getId());
			}
			return source;
		}

		Source *stopSource(Source *source)
		{
			if(source != null) {
				alSourceStop(source->getId());
			}
			return source;
		}

		Source *pauseSource(Source *source)
		{
			if(source != null) {
				alSourcePause(source->getId());
			}
			return source;
		}

		bool updateListener(vector position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0))
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


		virtual ~AudioRunner()
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

	};


const unsigned char AudioRunner::ID = 3;
