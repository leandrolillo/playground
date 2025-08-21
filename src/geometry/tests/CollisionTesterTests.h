/*
 * GeometryTests.h
 *
 *  Created on: Jun 29, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_TESTS_COLLISIONTESTERTESTS_H_
#define SRC_GEOMETRY_TESTS_COLLISIONTESTERTESTS_H_

#include <Tests.h>
#include <Geometry.h>
#include <GeometryContact.h>
#include <vector>
#include "../intersection/CollisionTester.h"

class CollisionTesterTests : public UnitTest {
public:
    CollisionTesterTests() {
        logger = LoggerFactory::getLogger("CollisionTesterTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("CollisionTesterTests::testSphereIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&CollisionTesterTests::testSphereIntersections));
        this->addTest("CollisionTesterTests::testPlaneIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&CollisionTesterTests::testPlaneIntersections));
        this->addTest("CollisionTesterTests::testAabbIntersections", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&CollisionTesterTests::testAabbIntersections));
        this->addTest("CollisionTesterTests::testSphereContacts", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&CollisionTesterTests::testSphereContacts));
    }

    /**
     * Provide a sample usage of a polymorfic geometry intersection use case - using std::unique_ptr
     */
    void testGeometryIntersections(PlaygroundRunner *runner) {

    }

    void testSphereIntersections(PlaygroundRunner *runner)
    {
        CollisionTester intersectionTester;

        Sphere sphere(vector(0, 0, 0), 2);
        Sphere anotherSphere(vector(3, 1, 1), 2);
        Plane plane(vector(0, 0, 0), vector(0, 1, 0));

        AABB aabb(vector(-1, 0, 0), vector(1, 1, 1));

        assertTrue("Spheres are intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertTrue("Sphere is intersecting plane", intersectionTester.intersects((Geometry &)sphere, (Geometry &)plane));
        assertTrue("Sphere is intersecting aabb", intersectionTester.intersects((Geometry &)sphere, (Geometry &)aabb));

        anotherSphere.setOrigin(vector(0, 6, 0));
        assertFalse("Spheres are not intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertFalse("Sphere is not intersecting plane", intersectionTester.intersects((Geometry &)anotherSphere, (Geometry &)plane));
        assertFalse("Sphere is not intersecting aabb", intersectionTester.intersects((Geometry &)anotherSphere, (Geometry &)aabb));
    }

    void testPlaneIntersections(PlaygroundRunner *runner)
    {
        CollisionTester intersectionTester;

        Sphere sphere(vector(0, 0, 0), 2);
        Sphere anotherSphere(vector(0, 6, 0), 2);
        Plane plane(vector(0, 0, 0), vector(0, 1, 0));

        assertTrue("Plane is intersecting sphere", intersectionTester.intersects((Geometry &)plane, (Geometry &)sphere));
        assertFalse("Plane is not intersecting sphere", intersectionTester.intersects((Geometry &)plane, (Geometry &)anotherSphere));
    }

    void testAabbIntersections(PlaygroundRunner *runner)
    {
        CollisionTester intersectionTester;

        Sphere sphere(vector(0, 0, 0), 2);
        AABB aabb(vector(1, 0, 0), vector(1, 1, 1));

        assertTrue("Aabb is intersecting sphere", intersectionTester.intersects((Geometry &)aabb, (Geometry &)sphere));
        aabb.setOrigin(vector(4, 0, 0));
        assertFalse("Aabb is not intersecting sphere", intersectionTester.intersects((Geometry &)aabb, (Geometry &)sphere));
    }

    void testSphereContacts(PlaygroundRunner *runner)
    {
        CollisionTester intersectionTester;

        real radius = 2.0f;
        Sphere sphere(vector(-1, 0, 0), radius);
        Sphere anotherSphere(vector(2, 1, 1), radius);

//        Plane plane(vector(0, 0, 0), vector(0, 1, 0));
//        AABB aabb(vector(-1, 0, 0), vector(1, 1, 1));

        std::vector<GeometryContact> contacts = intersectionTester.detectCollision((Geometry &)sphere, (Geometry &)anotherSphere);
        assertTrue("Spheres reported as not intersecting", !contacts.empty());
        GeometryContact contact = *contacts.begin();

        assertEquals("Unexpected geometryA", (void *)&sphere, (void *)contact.getGeometryA());
        assertEquals("Unexpected geometryB", (void *)&anotherSphere, (void *)contact.getGeometryB());

        vector expectedNormal = (sphere.getOrigin() - anotherSphere.getOrigin()).normalizado();
        real expectedPenetration = (radius + radius) - (sphere.getOrigin() - anotherSphere.getOrigin()).modulo();

        assertEquals("Unexpected contact normal", expectedNormal, contact.getNormal());
        assertEquals("Unexpected contact penetration", expectedPenetration, contact.getPenetration());


        anotherSphere.setOrigin(vector(0, 6, 0));
        contacts = intersectionTester.detectCollision((Geometry &)sphere, (Geometry &)anotherSphere);
        assertTrue("Spheres reported as intersecting", contacts.empty());
    }
};


#endif /* SRC_GEOMETRY_TESTS_COLLISIONTESTERTESTS_H_ */
