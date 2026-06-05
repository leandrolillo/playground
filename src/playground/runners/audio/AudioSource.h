#pragma once

#include <Math3d.h>

class AudioSource {
	private:
    unsigned long id;
		vector position;
		vector velocity;
		bool loop;
		float gain;
		float pitch;
		float rolloff;

	public:
		AudioSource(unsigned int id) :
		  id(id),
		  velocity(0, 0, 0),
		  position(0, 0, 0),
		  loop(false),
		  gain(1.0f),
		  pitch(1.0f),
		  rolloff(1.0f) {
		}

    AudioSource(unsigned int id, const vector &position, const vector &velocity, const bool &loop) :
      id(id),
      velocity(velocity),
      position(position),
      loop(loop),
      gain(1.0f),
      pitch(1.0f),
      rolloff(1.0f) {
    }

	  unsigned long getId() const {
	    return id;
	  }

		float getGain() const {
			return gain;
		}

		void setGain(float gain) {
			this->gain = gain;
		}

		unsigned char getLoop() const {
			return loop;
		}

		void setLoop(bool loop) {
			this->loop = loop;
		}

		float getPitch() const {
			return pitch;
		}

		void setPitch(float pitch) {
			this->pitch = pitch;
		}

		const vector& getPosition() const {
			return position;
		}

		void setPosition(const vector& position) {
			this->position = position;
		}

		float getRolloff() const {
			return rolloff;
		}

		void setRolloff(float rolloff) {
			this->rolloff = rolloff;
		}

		const vector& getVelocity() const {
			return velocity;
		}

		void setVelocity(const vector& velocity) {
			this->velocity = velocity;
		}
};
