/*
 * Gravity.h
 *
 *  Created on: Mar 25, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_FORCES_GRAVITY_H_
#define SRC_PHYSICS_FORCES_GRAVITY_H_

#include<Force.h>

class Gravity : public Force {
	vector acceleration;

public:
	Gravity(vector acceleration) {
		this->acceleration = acceleration;
	}

	void apply(real dt, const std::vector<Particle *> &particles) const override {
		for(std::vector<Particle *>::const_iterator iterator = particles.begin(); iterator != particles.end(); iterator++) {
			Particle *particle = *iterator;
			if(particle->getStatus()) {
				vector force = this->acceleration * particle->getMass();
				particle->applyForce(force);
			}
		}
	}
};



#endif /* SRC_PHYSICS_FORCES_GRAVITY_H_ */
