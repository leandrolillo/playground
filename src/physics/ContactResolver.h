/*
 * ContactResolver.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_CONTACTRESOLVER_H_
#define SRC_PHYSICS_CONTACTRESOLVER_H_

#include "ParticleContact.h"
#include <vector>

class ContactResolver {
    Logger *logger = LoggerFactory::getLogger("ContactResolver");

protected:
    real calculateSeparatingVelocity(const ParticleContact &contact) const {
        vector relativeVelocity = contact.getParticleA()->getVelocity();

        if(contact.getParticleB() != null) {
            relativeVelocity -= contact.getParticleB()->getVelocity();
        }

        return relativeVelocity * contact.getNormal();
    }

    real calculateAccCausedSeparatingVelocity(const ParticleContact &contact, real dt) const {
            vector velocity = contact.getParticleA()->getAcceleration();

            if(contact.getParticleB() != null) {
                velocity += contact.getParticleB()->getAcceleration();
            }

            return velocity * contact.getNormal() * dt;
        }

public:

    void resolveVelocity(const ParticleContact &contact, real dt) const {
        real separatingVelocity = contact.getRelativeSpeed();
        if(separatingVelocity <= 0.0) {
            real newSepVelocity = -separatingVelocity * contact.getRestitution();

            real accCausedSeparatingVelocity = calculateAccCausedSeparatingVelocity(contact, dt);
            if(accCausedSeparatingVelocity < 0) {
                newSepVelocity += contact.getRestitution() * accCausedSeparatingVelocity;
                newSepVelocity = std::max((real)0, newSepVelocity);
            }

            real deltaVelocity = newSepVelocity - separatingVelocity;

            Particle *particleA = contact.getParticleA();
            Particle *particleB = contact.getParticleB();

            real totalInverseMass = (real)0;

            if(particleA) {
                totalInverseMass = particleA->getInverseMass();
            }

            if (particleB) {
                totalInverseMass += particleB->getInverseMass();
            }

            if(totalInverseMass > (real)0) {
                real impulseAmountPerIMass = deltaVelocity / totalInverseMass;

//                vector impulsePerIMass = contact.getNormal() * (deltaVelocity / totalInverseMass);
                if (particleA != null && particleA->getInverseMass() > (real)0) {
                    particleA->setVelocity(particleA->getVelocity() + impulseAmountPerIMass * particleA->getInverseMass() * contact.getNormal());
                }

                if (particleB != null && particleB->getInverseMass() > (real)0) {
                    particleB->setVelocity(particleB->getVelocity() - impulseAmountPerIMass * particleB->getInverseMass() * contact.getNormal());
                }
            }
        }
    }

    void resolveInterpenetration(const ParticleContact &contact) const {
        if(contact.getPenetration() > (real)0) {
            Particle *particleA = contact.getParticleA();
            Particle *particleB = contact.getParticleB();

            real totalInverseMass = (real)0;

            if(particleA) {
                totalInverseMass += particleA->getInverseMass();
            }

            if(particleB != null) {
                totalInverseMass += particleB->getInverseMass();
            }

            if(totalInverseMass > (real)0) {
                vector movePerIMass = contact.getNormal() * (contact.getPenetration() / totalInverseMass);

                if(particleA != null && particleA->getInverseMass() > (real)0) {
                    vector deltaA = movePerIMass * particleA->getInverseMass();
                    particleA->setPosition(particleA->getPosition() + deltaA);
                }

                if(particleB != null && particleB->getInverseMass() > (real)0) {
                    vector deltaB = movePerIMass * particleB->getInverseMass();
                    particleB->setPosition(particleB->getPosition() - deltaB);
                }
            }
        }
    }

	void resolve(const std::vector<ParticleContact> &contacts, real dt) const {
	    for(std::vector<ParticleContact>::const_iterator iterator = contacts.begin(); iterator != contacts.end(); iterator++) {
	        resolveVelocity(*iterator, dt);
	        resolveInterpenetration(*iterator);

	        Particle *particleA = (*iterator).getParticleA();
	        Particle *particleB = (*iterator).getParticleB();

            particleA->onCollisionResolved(*iterator);

            if(particleB) {
                particleB->onCollisionResolved(*iterator);
            }
        }
    }

};




#endif /* SRC_PHYSICS_CONTACTRESOLVER_H_ */
