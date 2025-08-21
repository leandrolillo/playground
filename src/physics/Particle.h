/*
 * Particle.h
 *
 *  Created on: Mar 18, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLE_H_
#define SRC_PHYSICS_PARTICLE_H_

#include <Math3d.h>
#include <Geometry.h>

class ParticleContact;

class Particle {
	friend class ParticleIntegrator;

protected:
	std::unique_ptr<Geometry> boundingVolume;
	vector velocity;
	vector acceleration;

	real inverseMass = 0.0f;
	real mass = 0.0f;

	bool _status = true;

	/**
	 * rough approximation of drag to avoid numerical stability issues - without this objects are likely to accelerate magically.
	 *
	 */
	real damping = 0.0f;

	vector forceAccumulator;

public:
	Particle(std::unique_ptr<Geometry> geometry) {
	   this->boundingVolume = std::move(geometry);
	}

	Particle(Geometry *geometry) {
	   this->boundingVolume = std::unique_ptr<Geometry>(geometry);
	}

	Particle(Particle &&another) {
	    this->boundingVolume = std::move(boundingVolume);
	    another.boundingVolume = nullptr;
	}

	virtual ~Particle() {

	}

	virtual const Geometry *getBoundingVolume() const {
			return this->boundingVolume.get();
	}

	virtual void afterIntegrate(real dt) {
	}

	virtual void onCollision(const ParticleContact &contact) {

	}
	virtual void onCollisionResolved(const ParticleContact &contact) {

	}

	bool getStatus() const {
		return this->_status;
	}

	void setStatus(bool active) {
		this->_status = active;
	}

	void setPosition(const vector &position) {
		this->boundingVolume->setOrigin(position);
	}

	const vector &getPosition() const {
		return this->boundingVolume->getOrigin();
	}

	void setVelocity(const vector &velocity) {
		this->velocity = velocity;
	}

	const vector &getVelocity() const {
			return this->velocity;
		}

	void setAcceleration(const vector &acceleration) {
		this->acceleration = acceleration;
	}

	const vector &getAcceleration() const {
        return this->acceleration;
    }

	void setMass(real mass) {
		if(mass == 0.0f) {
			this->inverseMass = (real)0.0;
		} else {
			this->inverseMass = (real)1.0 / mass;
		}

		this->mass = mass;
	}

	void setInverseMass(real inverseMass) {
			if(inverseMass == 0.0f) {
				this->mass = (real)0.0;
			} else {
				this->mass = (real)1.0 / inverseMass;
			}

			this->inverseMass = inverseMass;
		}

	const real getMass() const {
		return this->mass;
	}

	const real getInverseMass() const {
		return this->inverseMass;
	}

	void setDamping(real damping) {
		this->damping = damping;
	}

	void clearForceAccumulator() {
		this->forceAccumulator = vector(0, 0, 0);
	}

	void applyForce(vector force) {
		this->forceAccumulator += force;
	}
};

#endif /* SRC_PHYSICS_PARTICLE_H_ */
