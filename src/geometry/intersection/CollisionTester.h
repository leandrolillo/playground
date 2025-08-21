/*
 * IntersectionTester.h
 *
 *  Created on: Jun 29, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_INTERSECTIONTESTER_H_
#define SRC_GEOMETRY_INTERSECTIONTESTER_H_

#include <vector>
#include <map>
#include <Geometry.h>
#include <GeometryContact.h>

class CollisionTester {
protected:
    std::map<std::pair<GeometryType, GeometryType>, std::vector<GeometryContact> (CollisionTester::*)(const Geometry &, const Geometry &) const> contactTestsTable;
    std::map<std::pair<GeometryType, GeometryType>, bool (CollisionTester::*)(const Geometry &, const Geometry &) const> intersectionTestsTable;

public:

    CollisionTester() {
        this->addIntersectionTests();
        this->addContactTests();
    }

    virtual ~CollisionTester() {
    }
    
    /**
     * TODO: should only add supported tests and generate a report with these (maybe toString?)
     */
    void addIntersectionTests() {
        this->addIntersectionTest(GeometryType::LINE, GeometryType::SPHERE, &CollisionTester::lineSphere);
//        this->addIntersectionTest(GeometryType::LINE, GeometryType::PLANE, &CollisionTester::linePlane);
//        this->addIntersectionTest(GeometryType::LINE, GeometryType::LINE, &CollisionTester::lineLine);
//        this->addIntersectionTest(GeometryType::LINE, GeometryType::AABB, &CollisionTester::lineAabb);
//        this->addIntersectionTest(GeometryType::LINE, GeometryType::OOBB, &CollisionTester::lineOobb);

        this->addIntersectionTest(GeometryType::PLANE, GeometryType::SPHERE, &CollisionTester::planeSphere);
//        this->addIntersectionTest(GeometryType::PLANE, GeometryType::PLANE, &CollisionTester::planePlane);
//        this->addIntersectionTest(GeometryType::PLANE, GeometryType::AABB, &CollisionTester::planeAabb);
//        this->addIntersectionTest(GeometryType::PLANE, GeometryType::OOBB, &CollisionTester::planeOobb);

        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::SPHERE, &CollisionTester::sphereSphere);
        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::AABB, &CollisionTester::sphereAabb);
//        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::OOBB, &CollisionTester::sphereOobb);
        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::HEIGHTMAP, &CollisionTester::sphereHeightmap);

//        this->addIntersectionTest(GeometryType::AABB, GeometryType::AABB, &CollisionTester::aabbAabb);
//        this->addIntersectionTest(GeometryType::AABB, GeometryType::OOBB, &CollisionTester::aabbOobb);
//
//        this->addIntersectionTest(GeometryType::OOBB, GeometryType::OOBB, &CollisionTester::oobbOobb);
    }

    /**
     * TODO: should only add supported tests and generate a report with these (maybe toString?)
     */

    void addContactTests() {
//        this->addContactTest(GeometryType::LINE, GeometryType::SPHERE, &CollisionTester::lineSphereContact);
//        this->addContactTest(GeometryType::LINE, GeometryType::PLANE, &CollisionTester::linePlaneContact);
//        this->addContactTest(GeometryType::LINE, GeometryType::LINE, &CollisionTester::lineLineContact);
//        this->addContactTest(GeometryType::LINE, GeometryType::AABB, &CollisionTester::lineAabbContact);
//        this->addContactTest(GeometryType::LINE, GeometryType::OOBB, &CollisionTester::lineOobbContact);

        this->addContactTest(GeometryType::PLANE, GeometryType::SPHERE, &CollisionTester::planeSphereContact);
//        this->addContactTest(GeometryType::PLANE, GeometryType::PLANE, &CollisionTester::planePlaneContact);
//        this->addContactTest(GeometryType::PLANE, GeometryType::AABB, &CollisionTester::planeAabbContact);
//        this->addContactTest(GeometryType::PLANE, GeometryType::OOBB, &CollisionTester::planeOobbContact);

        this->addContactTest(GeometryType::SPHERE, GeometryType::SPHERE, &CollisionTester::sphereSphereContact);
        this->addContactTest(GeometryType::SPHERE, GeometryType::AABB, &CollisionTester::sphereAabbContact);
//        this->addContactTest(GeometryType::SPHERE, GeometryType::OOBB, &CollisionTester::sphereOobbContact);
        this->addContactTest(GeometryType::SPHERE, GeometryType::HEIGHTMAP, &CollisionTester::sphereHeightmapContact);

//        this->addContactTest(GeometryType::AABB, GeometryType::AABB, &CollisionTester::aabbAabbContact);
//        this->addContactTest(GeometryType::AABB, GeometryType::OOBB, &CollisionTester::aabbOobbContact);
//
//        this->addContactTest(GeometryType::OOBB, GeometryType::OOBB, &CollisionTester::oobbOobbContact);
    }

    virtual void addIntersectionTest(const GeometryType &typeOp1, const GeometryType &typeOp2, bool (CollisionTester::*intersectionTest)(const Geometry &, const Geometry &) const) {
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, typeOp2)] = intersectionTest;

        //TODO: check we're not adding more than desired
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, GeometryType::FRUSTUM)] = &CollisionTester::geometryFrustum;
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp2, GeometryType::FRUSTUM)] = &CollisionTester::geometryFrustum;

        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchy;
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp2, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchy;
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(GeometryType::HIERARCHY, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchy; // TODO: this might be a special case

        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(GeometryType::HIERARCHY, GeometryType::FRUSTUM)] = &CollisionTester::geometryFrustum; // TODO: this might be a special case
    }

    virtual void addContactTest(const GeometryType &typeOp1, const GeometryType &typeOp2, std::vector<GeometryContact> (CollisionTester::*contactTest)(const Geometry &, const Geometry &) const) {
        contactTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, typeOp2)] = contactTest;

        //TODO: check we're not adding more than desired
        contactTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchyContact;
        contactTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp2, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchyContact;
        contactTestsTable[std::pair<const GeometryType &, const GeometryType &>(GeometryType::HIERARCHY, GeometryType::HIERARCHY)] = &CollisionTester::geometryHierarchyContact; // TODO: this might be a special case
    }


    virtual bool intersects(const Geometry &op1, const Geometry & op2) const {
        std::pair<GeometryType, GeometryType > key(op1.getType(), op2.getType());

        if(intersectionTestsTable.count(key) > 0) {
            bool (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
            testFunction = intersectionTestsTable.at(key);
            return (this->*testFunction)(op1, op2);
        } else {
            std::pair<GeometryType, GeometryType > inverseKey(op2.getType(), op1.getType());

            if(intersectionTestsTable.count(inverseKey) > 0) {
                bool (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
                testFunction = intersectionTestsTable.at(inverseKey);
                return (this->*testFunction)(op2, op1);
            }
        }

        return false;
    }

    virtual std::vector<GeometryContact>  detectCollision(const Geometry &op1, const Geometry &op2) const {
        std::pair<GeometryType, GeometryType > key(op1.getType(), op2.getType());

        if(contactTestsTable.count(key) > 0) {
            std::vector<GeometryContact> (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
            testFunction = contactTestsTable.at(key);
            return (this->*testFunction)(op1, op2);
        } else {
            std::pair<GeometryType, GeometryType > inverseKey(op2.getType(), op1.getType());

            if(contactTestsTable.count(inverseKey) > 0) {
                std::vector<GeometryContact> (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
                testFunction = contactTestsTable.at(inverseKey);
                return (this->*testFunction)(op2, op1);
            }
        }

        return std::vector<GeometryContact>();
    }

    virtual String toString() const {
    	String contactMappings;
    	for(auto &mapping : this->contactTestsTable) {
    		contactMappings += (contactMappings.empty() ? "" : ", ") + toString(mapping.first.first) + "<->" + toString(mapping.first.second);
    	}

    	String intersectionMappings;
    	for(auto &mapping : this->intersectionTestsTable) {
    		intersectionMappings += (intersectionMappings.empty() ? "" : ", ") + toString(mapping.first.first) + "<->" + toString(mapping.first.second);
    	}

    	return "CollisionTester(intersectionChecks: [" + intersectionMappings + "], contactChecks: [" + contactMappings + "]";
    }


protected:
    String toString(GeometryType geometryType) const {
    	switch(geometryType) {
    		case GeometryType::SPHERE:
    		return "SPHERE";
    		case GeometryType::LINE:
    			return "LINE";
    		case GeometryType::PLANE:
    		  return "PLANE";
    		case GeometryType::AABB:
    		  return "AABB";
    		case GeometryType::OOBB:
    		  return "OOBB";
    		case GeometryType::HIERARCHY:
    		  return "HIERARCHY";
    		case GeometryType::FRUSTUM:
    		  return "FRUSTUM";
    		case GeometryType::HEIGHTMAP:
    		  return "HEIGHTMAP";
    	}

    	return "UNKNOWN";

    }
    /*****
     *
     * Intersection Tests
     *
     *****/

    bool geometryHierarchy(const Geometry &geometry, const Geometry &hierarchicalGeometry) const {
    	const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchicalGeometry;

    	if(this->intersects(geometry, hierarchy.getBoundingVolume())) {
        for(auto & currentChildren: hierarchy.getChildren()) {
            if(this->intersects(geometry, *currentChildren.get())) {
            	return true;
            }
        }

    	}
      return false;
    }


    std::vector<GeometryContact> geometryHierarchyContact(const Geometry &geometry, const Geometry &hierarchyGeometry) const {
        const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;


        if(this->intersects(geometry, hierarchy.getBoundingVolume())) {
            std::vector<GeometryContact> response;

            for(auto & currentChildren: hierarchy.getChildren()) {
                std::vector<GeometryContact> currentChildrenContacts = this->detectCollision(geometry, *currentChildren.get());
                response.insert( response.end(), currentChildrenContacts.begin(), currentChildrenContacts.end());
            }

            return response;
        }

        return std::vector<GeometryContact>();
    }

    bool geometryFrustum(const Geometry &geometry, const Geometry &frustumGeometry) const {
      const Frustum &frustum = (const Frustum &)frustumGeometry;

    	for(auto plane : frustum.getHalfSpaces()) {
    		if(!this->intersects(geometry, plane)) {
    			return false;
    		}
    	}

    	return true;
    }



    /**
     * point sphere test
     */

    bool pointInSphere(const vector &point, const Sphere &sphere) const {
        vector delta = sphere.getOrigin() - point;
        return delta * delta <= sphere.getRadius() * sphere.getRadius();
    }


    /**
     * Line intersection test
     */
    bool lineSphere(const Geometry &lineGeometry, const Geometry &sphereGeometry) const {
        const Line &line = (const Line &) lineGeometry;
        const Sphere &sphere = (const Sphere &) sphereGeometry;

        real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
        vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
        vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

        return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    bool linePlane(const Geometry &line, const Geometry &plane) const {
       return false;
    }

    bool lineLine(const Geometry &line, const Geometry &anotherLine) const {
       return false;
    }

    bool lineAabb(const Geometry &line, const Geometry &aabb) const {
           return false;
    }

    bool lineOobb(const Geometry &line, const Geometry &oobb) const {
           return false;
    }

    /**
     * Plane intersection test - This is actually a half space / sphere test
     */
    bool planeSphere(const Geometry &planeGeometry, const Geometry &sphereGeometry) const {
        Plane &plane = (Plane &)planeGeometry;
        Sphere &sphere = (Sphere &)sphereGeometry;
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    bool planePlane(const Geometry &planeGeometry, const Geometry &anotherPlaneGeometry) const {
        return false;
    }

    bool planeAabb(const Geometry &planeGeometry, const Geometry &aabbGeometry) const {
            return false;
    }

    bool planeOobb(const Geometry &plane, const Geometry &oobb) const {
            return false;
    }

    /**
     * Sphere intersection test
     */
    bool sphereSphere(const Geometry &sphereGeometry, const Geometry &anotherSphereGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const Sphere &anotherSphere = (const Sphere &)anotherSphereGeometry;
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    bool sphereAabb(const Geometry &sphereGeometry, const Geometry &aabbGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;

        return pointInSphere(aabb.closestPoint(sphere.getOrigin()), sphere);
    }

    bool sphereOobb(const Geometry &sphere, const Geometry &oobb) const {
        return false;
    }

    bool sphereHeightmap(const Geometry &sphereGeometry, const Geometry &heightMapGeometry) const {
   		const Sphere &sphere = (const Sphere &)sphereGeometry;
   		const HeightMapGeometry &heightmap = (const HeightMapGeometry &)heightMapGeometry;

		vector aabbClosestPoint = heightmap.closestPoint(sphere.getOrigin());
		aabbClosestPoint.y = heightmap.getHeightMap()->heightAt(aabbClosestPoint.x - heightmap.getPosition().x, aabbClosestPoint.z - heightmap.getPosition().z);

		return pointInSphere(aabbClosestPoint, sphere);
   	}


    /**
     * AABB intersection tests
     */
    bool aabbAabb(const Geometry &aabb, const Geometry &anotherAabb) const {
        return false;
    }

    bool aabbOobb(const Geometry &aabb, const Geometry &anotherObb) const {
        return false;
    }


    /**
     * OOBB intersection tests
     */
    bool oobbOobb(const Geometry &oobb, const Geometry &anotherOobb) const {
        return false;
    }


    /*****
     *
     * Contact determination Tests
     *
     *****/

    /**
     * Line contact Determination
     */
    std::vector<GeometryContact> lineSphereContact(const Geometry &lineGeometry, const Geometry &sphereGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Sphere &sphere = (const Sphere &)sphereGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> linePlaneContact(const Geometry &lineGeometry, const Geometry &planeGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Plane &plane= (const Plane &)planeGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> lineLineContact(const Geometry &lineGeometry, const Geometry &anotherLineGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Line &anotherLine = (const Line &)anotherLineGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> lineAabbContact(const Geometry &lineGeometry, const Geometry &aabbGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> lineOobbContact(const Geometry &lineGeometry, const Geometry &oobbGeometry) const {
        return std::vector<GeometryContact>();
    }


    /**
     * Plane contact determination - This is actually a half space / sphere test
     */
    std::vector<GeometryContact> planeSphereContact(const Geometry &planeGeometry, const Geometry &sphereGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const Sphere & sphere= (const Sphere &)sphereGeometry;

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

    std::vector<GeometryContact> planePlaneContact(const Geometry &planeGeometry, const Geometry &anotherPlaneGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const Plane & anotherPlane = (const Plane &)anotherPlaneGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> planeAabbContact(const Geometry &planeGeometry, const Geometry &aabbGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const AABB & aabb = (const AABB &)aabbGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> planeOobbContact(const Geometry &planeGeometry, const Geometry &oobbGeometry) const {
        return std::vector<GeometryContact>();
    }


    /**
     * Sphere contact determination
     */
    std::vector<GeometryContact> sphereSphereContact(const Geometry &sphereGeometry, const Geometry &anotherSphereGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const Sphere &anotherSphere = (const Sphere &)anotherSphereGeometry;

        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        if(delta * delta <= radiuses * radiuses) {
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = radiuses - distance;
            vector intersection = sphere.getOrigin() + (normal * sphere.getRadius());
            return std::vector<GeometryContact> {GeometryContact(&sphere, &anotherSphere, intersection, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> sphereAabbContact(const Geometry &sphereGeometry, const Geometry &aabbGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;

        vector aabbClosestPoint = aabb.closestPoint(sphere.getOrigin());

        if(pointInSphere(aabbClosestPoint, sphere)) {
            vector delta = sphere.getOrigin() - aabbClosestPoint;
            if(equalsZero(delta * delta)) {
            	aabbClosestPoint = aabb.closestSurfacePoint(sphere.getOrigin());
            	delta = aabbClosestPoint - sphere.getOrigin();
            }
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = sphere.getRadius() - distance;

            return std::vector<GeometryContact> {GeometryContact(&sphere, &aabb, aabbClosestPoint, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> sphereOobbContact(const Geometry &sphereGeometry, const Geometry &oobbGeometry) const {
    	return std::vector<GeometryContact>();
    }


    /**
     * Non-accurate heightmap test. Returns data of the point directly below the sphere
     */
    std::vector<GeometryContact> sphereHeightmapContact(const Geometry &sphereGeometry, const Geometry &heightMapGeometry) const {
		const Sphere &sphere = (const Sphere &)sphereGeometry;
		const HeightMapGeometry &heightmap = (const HeightMapGeometry &)heightMapGeometry;

        vector aabbClosestPoint = heightmap.closestPoint(sphere.getOrigin());
        aabbClosestPoint.y = heightmap.heightAt(aabbClosestPoint.x, aabbClosestPoint.z);

        if(pointInSphere(aabbClosestPoint, sphere)) {
            vector delta = sphere.getOrigin() - aabbClosestPoint;
			real distance = delta.modulo();
			//vector normal = delta * (1.0 / distance); // method 1 - upwards pointing normal
			vector normal = heightmap.normalAt(aabbClosestPoint.x, aabbClosestPoint.z); // method 2 - triangle normal
			real penetration = sphere.getRadius() - distance;

			return std::vector<GeometryContact> {GeometryContact(&sphere, &heightMapGeometry, aabbClosestPoint, normal, 0.8f,  penetration) };
        }


		return std::vector<GeometryContact>();
	}



    /**
     * AABB contact determination
     */
    std::vector<GeometryContact> aabbAabbContact(const Geometry &aabbGeometry, const Geometry &anotherAabbGeometry) const {
        const AABB &aabb = (const AABB &)aabbGeometry;
        const AABB &anotherAabb = (const AABB &)anotherAabbGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> aabbOobbContact(const Geometry &aabbGeometry, const Geometry &anotherOobbGeometry) const {
        return std::vector<GeometryContact>();
    }


    /**
     * OOBB contact determination
     */
    std::vector<GeometryContact> oobbOobbContact(const Geometry &oobbGeometry, const Geometry &anotherOobbGeometry) const {
        return std::vector<GeometryContact>();
    }
};



#endif /* SRC_GEOMETRY_INTERSECTIONTESTER_H_ */
