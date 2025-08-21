/*
 * CollisionResolverTests.h
 *
 *  Created on: Sep 15, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_TESTS_COLLISIONRESOLVERTESTS_H_
#define SRC_PHYSICS_TESTS_COLLISIONRESOLVERTESTS_H_

#include <Tests.h>
#include <Geometry.h>
#include <GeometryContact.h>
#include <vector>
#include "../intersection/CollisionTester.h"
#include<StringFormatter.h>

class ContactResolverTests : public UnitTest {
public:
    ContactResolverTests() {
        logger = LoggerFactory::getLogger("ContactResolverTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ContactResolverTests::testSphereIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ContactResolverTests::testSphereSphere));
        this->addTest("ContactResolverTests::testSpherePlaneIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ContactResolverTests::testSpherePlane));
        this->addTest("ContactResolverTests::testPlaneSphereIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ContactResolverTests::testPlaneSphere));
        this->addTest("ContactResolverTests::testSphereAabbIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ContactResolverTests::testSphereAabb));
        this->addTest("ContactResolverTests::testAabbSphereIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ContactResolverTests::testAabbSphere));
    }

    void testSphereSphere(PlaygroundRunner *runner) {
        CollisionDetector collisionDetector;
        ContactResolver contactResolver;

        real radius = 2.0f;
        std::vector<Particle*> particles;

        Particle sphereParticle(new Sphere(vector(-1, 0, 0), radius));
        sphereParticle.setVelocity(vector(1, 1, 1));
        sphereParticle.setMass(1.0f);

        Particle anotherSphereParticle(new Sphere(vector(2, 1, 1), radius));
        anotherSphereParticle.setVelocity(vector(-1, -1, -1));
        anotherSphereParticle.setMass(1.0f);

        particles.push_back(&sphereParticle);
        particles.push_back(&anotherSphereParticle);

        std::vector<ParticleContact> contacts = collisionDetector.detectCollisions(particles);
        contactResolver.resolve(contacts, 0.1f);

        vector delta = anotherSphereParticle.getPosition() - sphereParticle.getPosition();
        vector normal = delta.normalizado();
        real actualPenetration = (radius + radius) - delta.modulo();
        real deltaVelocity = (sphereParticle.getVelocity() - anotherSphereParticle.getVelocity()) * normal;

        assertTrue("Unexpected penetration", 0.0 >= actualPenetration);
        assertTrue("Unexpected velocity", 0.0 >= deltaVelocity);
    }

    void testSpherePlane(PlaygroundRunner *runner) {
        CollisionDetector collisionDetector;
        ContactResolver contactResolver;

        real radius = 2.0f;
        std::vector<Particle*> particles;

        Particle sphereParticle(new Sphere(vector(0, 0, 0), radius));
        sphereParticle.setVelocity(vector(0, -1, 0));
        sphereParticle.setMass(1.0f);

        Particle planeParticle(new Plane(vector(0, 0, 0), vector(0, 1, 0)));

        particles.push_back(&sphereParticle);
        particles.push_back(&planeParticle);

        std::vector<ParticleContact> contacts = collisionDetector.detectCollisions(particles);
        contactResolver.resolve(contacts, 0.1f);

        vector delta =  sphereParticle.getPosition() - planeParticle.getPosition();
        vector normal = -((Plane *)planeParticle.getBoundingVolume())->getNormal();
        real actualPenetration = delta * normal;
        real deltaVelocity = (sphereParticle.getVelocity() - planeParticle.getVelocity()) * normal;

        assertTrue(StringFormatter::format("Unexpected penetration %.2f", actualPenetration), 0.0 >= actualPenetration);
        assertTrue("Unexpected velocity", 0.0 >= deltaVelocity);
    }

    void testPlaneSphere(PlaygroundRunner *runner) {

    }

    void testSphereAabb(PlaygroundRunner *runner) {

    }

    void testAabbSphere(PlaygroundRunner *runner) {

    }
};



#endif /* SRC_PHYSICS_TESTS_COLLISIONRESOLVERTESTS_H_ */
