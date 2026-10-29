#pragma once

#include <Math3d.h>
#include "AudioBufferResource.h"

class AudioRunner;

class AudioSource {
  friend class AudioRunner;
	private:
    AudioRunner *owner;
    unsigned int id = 0;

    AudioRunner &getOwner() const;
    void release();

	public:
		AudioSource(AudioRunner &owner);

    AudioSource(const AudioSource &) = delete;
    AudioSource &operator=(const AudioSource &) = delete;
    AudioSource(AudioSource &&) = delete;
    AudioSource &operator=(AudioSource &&) = delete;

    AudioSource(AudioRunner &owner, AudioBufferResource &buffer, const vector &position, const vector &velocity, const bool &looping) : AudioSource(owner) {
      setBuffer(buffer);
      setPosition(position);
      setVelocity(velocity);
      setLooping(looping);
      setGain(1.0f);
      setPitch(1.0f);
      setRolloffFactor(1.0f);
    }

    ~AudioSource();

	  unsigned long getId() const {
	    return id;
	  }

	  void setBuffer(AudioBufferResource &buffer);

	  vector getPosition() const;
    void setPosition(const vector& position);

    vector getVelocity() const;
    void setVelocity(const vector& velocity);

	  float getGain() const;
		void setGain(float gain);

		unsigned char getLooping() const;
		void setLooping(bool looping);

		float getPitch() const;
		void setPitch(float pitch);

		float getRolloffFactor() const;
		void setRolloffFactor(float rolloffFactor);

		/*
		 * Playback methods
		 */
    void play();
    void stop();
    void pause();
};
