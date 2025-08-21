/*
 * Source.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include <Resource.h>
#include <Math3d.h>

class Source: public Resource {
	private:
		vector position;
		vector velocity;
		bool loop;
		float gain;
		float pitch;
		float rolloff;

	public:
		Source(unsigned int id) :
				Resource(id, MimeTypes::AUDIOSOURCE) {
			velocity = position = vector(0, 0, 0);
			loop = false;
			gain = pitch = rolloff = 1.0f;
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

#endif /* SOURCE_H_ */
