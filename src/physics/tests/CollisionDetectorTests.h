/*
 * CollisionDetectorTests.h
 *
 *  Created on: Sep 20, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_TESTS_COLLISIONDETECTORTESTS_H_
#define SRC_PHYSICS_TESTS_COLLISIONDETECTORTESTS_H_

#include <Tests.h>
#include <Geometry.h>
#include <GeometryContact.h>
#include <vector>
#include "../intersection/CollisionTester.h"

class CollisionDetectorTests : public UnitTest {
public:
    CollisionDetectorTests() {
        logger = LoggerFactory::getLogger("CollisionDetectorTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("CollisionDetectorTests::testSphereIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&CollisionDetectorTests::testSphereSphere));
    }

    void testSphereSphere(PlaygroundRunner *runner) {
        CollisionDetector collisionDetector;

        real radius = 2.0f;
        std::vector<Particle*> particles;

        Particle sphereParticle(new Sphere(vector(-1, 0, 0), radius));
        sphereParticle.setMass(1.0f);

        Particle anotherSphereParticle(new Sphere(vector(2, 1, 1), radius));
        anotherSphereParticle.setMass(1.0f);

        particles.push_back(&sphereParticle);
        particles.push_back(&anotherSphereParticle);

        std::vector<ParticleContact> contacts = collisionDetector.detectCollisions(particles);
        ParticleContact contact = *contacts.begin();

        assertEquals("Unexpected particleA", (void*) &sphereParticle, (void*) contact.getParticleA());
        assertEquals("Unexpected particleB", (void*) &anotherSphereParticle, (void*) contact.getParticleB());

        vector expectedNormal = (sphereParticle.getPosition() - anotherSphereParticle.getPosition()).normalizado();
        real expectedPenetration = (radius + radius) - (sphereParticle.getPosition() - anotherSphereParticle.getPosition()).modulo();

        assertEquals("Unexpected contact normal", expectedNormal, contact.getNormal());
        assertRealEquals("Unexpected contact penetration", expectedPenetration, contact.getPenetration());
    }
};




#endif /* SRC_PHYSICS_TESTS_COLLISIONDETECTORTESTS_H_ */
