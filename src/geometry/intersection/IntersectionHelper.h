/*
 * IntersectionHelper.h
 *
 *  Created on: Jun 30, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_INTERSECTIONHELPER_H_
#define SRC_GEOMETRY_INTERSECTIONHELPER_H_

#include <vector>
#include <Geometry.h>
#include <GeometryContact.h>


class IntersectionHelper {
public:
    static bool pointSphere(const vector &point, const Sphere &sphere) {
        vector delta = sphere.getOrigin() - point;
        return delta * delta <= sphere.getRadius() * sphere.getRadius();
    }

    /**
     * Line intersection test
     */
    static bool lineSphere(const Line &line, const Sphere &sphere) {
       real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
       vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
       vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

       return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    static bool linePlane(const Line &line, const Plane &plane) {
       return false;
    }

    static bool lineLine(const Line &line, const Line &anotherLine) {
       return false;
    }

    static bool lineAabb(const Line &line, const AABB &aabb) {
           return false;
    }

    static bool lineHierarchy(const Line &line, const HierarchicalGeometry &hierarchy) {
           return false;
    }

    /**
     * Plane intersection test
     */
    static bool planeSphere(const Plane &plane, const Sphere &sphere) {
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    static bool planePlane(const Plane &plane, const Plane &anotherPlane) {
        return false;
    }

    static bool planeAabb(const Plane &plane, const AABB &aabb) {
            return false;
    }

    static bool planeHierarchy(const Plane &plane, const HierarchicalGeometry &hierarchy) {
           return false;
    }

    /**
     * Sphere intersection test
     */
    static bool sphereSphere(const Sphere &sphere, const Sphere &anotherSphere) {
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    static bool sphereAabb(const Sphere &sphere, const AABB &aabb) {
        //printf("Checking intersection for %s and %s \n", sphere.toString().c_str(), aabb.toString().c_str());
        vector mins = aabb.getOrigin() - aabb.getHalfSizes();
        vector maxs = aabb.getOrigin() + aabb.getHalfSizes();

        //printf("mins: %s - maxs: %s\n", mins.toString().c_str(), maxs.toString().c_str());


        vector aabbClosestPoint = vector(std::max(mins.x, std::min(sphere.getOrigin().x, maxs.x)),
                std::max(mins.y, std::min(sphere.getOrigin().y, maxs.y)),
                std::max(mins.z, std::min(sphere.getOrigin().z, maxs.z))
                );

        //printf("closest point: %s\n", aabbClosestPoint.toString().c_str());

        return pointSphere(aabbClosestPoint, sphere);
    }

    static bool sphereHierarchy(const Sphere &sphere, const HierarchicalGeometry &hierarchy) {
           return false;
    }


    /**
     * AABB intersection tests
     */
    static bool aabbAabb(const AABB &aabb, const AABB &anotherAabb) {
        return false;
    }

    static bool aabbHierarchy(const AABB &aabb, const HierarchicalGeometry &hierarchy) {
       return false;
    }

    /**
     * Hierarchy intersection tests
     */

    static bool hierarchyHierarchy(const HierarchicalGeometry &hierarchy, const HierarchicalGeometry &anotherHierarchy) {
       return false;
    }



    /**
     * Line contact Determination
     */
    static std::vector<GeometryContact> lineSphereContact(const Line &line, const Sphere &sphere) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> linePlaneContact(const Line &line, const Plane &plane) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> lineLineContact(const Line &line, const Line &anotherLine) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> lineAabbContact(const Line &line, const AABB &aabb) {
        return std::vector<GeometryContact>();
    }
    static std::vector<GeometryContact> lineHierarchyContact(const Line &line, const HierarchicalGeometry &hierarchy) {
        return std::vector<GeometryContact>();
    }



    /**
     * Plane contact determination
     */
    static std::vector<GeometryContact> planeSphereContact(const Plane &plane, const Sphere &sphere) {
        vector normal = plane.getNormal();

        real distance = (sphere.getOrigin() - plane.getOrigin()) * normal;

        if(distance <= sphere.getRadius()) {
            vector delta = distance * normal;
            real penetration = sphere.getRadius() - distance;
            vector intersection = sphere.getOrigin() - (normal * sphere.getRadius());

            return std::vector<GeometryContact> {GeometryContact(&plane, &sphere, intersection, normal, 0.8f, penetration) };
        }

        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> planePlaneContact(const Plane &plane, const Plane &anotherPlane) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> planeAabbContact(const Plane &plane, const AABB &aabb) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> planeHierarchyContact(const Plane &plane, const HierarchicalGeometry &hierarchy) {
        return std::vector<GeometryContact>();
    }


    /**
     * Sphere contact determination
     */
    static std::vector<GeometryContact> sphereSphereContact(const Sphere &sphereA, const Sphere &sphereB) {
        vector delta = sphereB.getOrigin() - sphereA.getOrigin();
        real radiuses = sphereA.getRadius() + sphereB.getRadius();

        if(delta * delta <= radiuses * radiuses) {
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = radiuses - distance;
            vector intersection = sphereA.getOrigin() + (normal * sphereA.getRadius());
            return std::vector<GeometryContact> {GeometryContact(&sphereA, &sphereB, intersection, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> sphereAabbContact(const Sphere &sphere, const AABB &aabb) {
        //printf("Checking intersection for %s and %s \n", sphere.toString().c_str(), aabb.toString().c_str());
        vector mins = aabb.getOrigin() - aabb.getHalfSizes();
        vector maxs = aabb.getOrigin() + aabb.getHalfSizes();

        //printf("mins: %s - maxs: %s\n", mins.toString().c_str(), maxs.toString().c_str());

        vector aabbClosestPoint = vector(std::max(mins.x, std::min(sphere.getOrigin().x, maxs.x)),
                std::max(mins.y, std::min(sphere.getOrigin().y, maxs.y)),
                std::max(mins.z, std::min(sphere.getOrigin().z, maxs.z))
                );

        //printf("closest point: %s\n", aabbClosestPoint.toString().c_str());

        if(pointSphere(aabbClosestPoint, sphere)) {
            vector delta = sphere.getOrigin() - aabbClosestPoint;
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = sphere.getRadius() - distance;

            return std::vector<GeometryContact> {GeometryContact(&sphere, &aabb, aabbClosestPoint, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> sphereHierarchyContact(const Sphere &sphere, const HierarchicalGeometry &hierarchy) {
        return std::vector<GeometryContact>();
    }



    /**
     * AABB contact determination
     */
    static std::vector<GeometryContact> aabbAabbContact(const AABB &aabb, const AABB &anotherAabb) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> aabbHierarchyContact(const AABB &aabb, const HierarchicalGeometry &hierarchy) {
        return std::vector<GeometryContact>();
    }

    /**
     * Hierarchy contact determination
     */
    static std::vector<GeometryContact> hierarchyHierarchyContact(const HierarchicalGeometry &hierarchy, const HierarchicalGeometry &anotherHierarchy) {
        return std::vector<GeometryContact>();
    }
};



#endif /* SRC_GEOMETRY_INTERSECTIONHELPER_H_ */
