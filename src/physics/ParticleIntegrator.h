/*
 * ParticleIntegrator.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLEINTEGRATOR_H_
#define SRC_PHYSICS_PARTICLEINTEGRATOR_H_

#include<Math3d.h>
#include <Particle.h>

#include<vector>

class ParticleIntegrator {
public:
	void integrate(real dt, Particle &particle) const {
		particle.acceleration = particle.forceAccumulator * particle.inverseMass;
		particle.setPosition(particle.getPosition() + particle.velocity * dt + 0.5 * particle.acceleration * dt * dt);
		particle.velocity = particle.velocity * powr(particle.damping, dt) + particle.acceleration * dt;
	}
};



#endif /* SRC_PHYSICS_PARTICLEINTEGRATOR_H_ */
