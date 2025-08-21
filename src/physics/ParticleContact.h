/*
 * Contact.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLE_CONTACT_H_
#define SRC_PHYSICS_PARTICLE_CONTACT_H_

#include<BaseContact.h>
#include<Particle.h>

class ParticleContact : public BaseContact {
public:
    static ParticleContact noContact;
private:
    Particle *particleA;
    Particle *particleB;
    real relativeSpeed;

public:
    ParticleContact(Particle *particleA, Particle *particleB, const vector &intersection, const vector &normal, real restitution, real penetration = 0.0) : BaseContact(intersection, normal, restitution, penetration){
        this->particleA = particleA;
        this->particleB = particleB;

        vector relativeVelocity = particleA->getVelocity();
        if(particleB != null) {
            relativeVelocity -= particleB->getVelocity();
        }
        relativeSpeed = relativeVelocity * normal;

    }

    Particle *getParticleA() const {
        return this->particleA;
    }

    Particle *getParticleB() const {
        return this->particleB;
    }

    real getRelativeSpeed() const {
    	return this->relativeSpeed;
    }
};

#endif /* SRC_PHYSICS_CONTACT_H_ */
