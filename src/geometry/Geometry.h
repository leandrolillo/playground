/*
 * Geometry.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRY_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRY_H_

#include<vector>
#include<Math3d.h>
#include<GeometryContact.h>


enum class GeometryType {
    SPHERE,
    PLANE,
    LINE,
    AABB,
		OOBB,
    HIERARCHY,
    HEIGHTMAP,
		FRUSTUM
};


class Geometry {
    vector origin; //keep this property private and use getOrigin instead.
public:
    Geometry(const vector &origin) {
        this->origin = origin;
    }

    virtual ~Geometry() {}

    virtual const vector& getOrigin() const {
        return this->origin;
    }

    virtual void setOrigin(const vector &origin) {
        this->origin = origin;
    }

    virtual String toString() const {
        return "Geometry(origin: " + origin.toString() + ")";
    }

    virtual GeometryType getType() const = 0;
};

class Sphere: public Geometry {
    real radius;
public:
    Sphere(const vector &origin, real radius) : Geometry(origin) {
        this->radius = radius;
    }

    real getRadius() const {
        return this->radius;
    }

    void setRadius(real radius) {
        this->radius = radius;
    }

    String toString() const override {
        return "Sphere(origin: " + this->getOrigin().toString() + ", radius: " + std::to_string(this->radius) + ")";
    }

    GeometryType getType() const override {
        return GeometryType::SPHERE;
    }
};

class Plane: public Geometry {
    vector normal;
public:
    Plane(const vector &origin, const vector &normal) : Geometry(origin) {
        this->normal = normal.normalizado();
    }

    const vector &getNormal() const {
        return this->normal;
    }

    virtual void setNormal(const vector &normal) {
    	this->normal = normal;
    }

    String toString() const override {
        return "Plane(origin: " + this->getOrigin().toString("%.6f") + ", normal: " + this->normal.toString("%.6f") + ")";
    }

    GeometryType getType() const override {
        return GeometryType::PLANE;
    }
};

class Line: public Geometry {
    vector direction;
public:
    Line(const vector &origin, const vector &direction) : Geometry(origin){
        this->direction = direction.normalizado();
    }

    const vector& getDirection() const {
        return this->direction;
    }

    String toString() const override {
        return "Line(origin: " + this->getOrigin().toString() + ", dir: " + this->direction.toString() + ")";
    }

    GeometryType getType() const override {
        return GeometryType::LINE;
    }
};

class AABB : public Geometry {
	Logger *logger = LoggerFactory::getLogger("geometry/AABB");
    vector halfSizes;
public:
    AABB(const vector &origin, const vector &halfSizes) : Geometry(origin) {
        this->halfSizes = halfSizes;
    }

    const vector &getHalfSizes() const {
        return this->halfSizes;
    }

    void setHalfSizes(const vector &halfSizes) {
        this->halfSizes = halfSizes;
    }

    /**
     * Returns the bottom left position of the aabb. On the other hand, Origin is the center.
     */
    vector getPosition() const {
    	return this->getOrigin()- halfSizes;
    }

    String toString() const override {
        return "AABB(origin: " + this->getOrigin().toString() + ", halfSizes: " + this->halfSizes.toString() + ")";
    }

    GeometryType getType() const override {
        return GeometryType::AABB;
    }

    vector closestPoint(const vector &target) const {
        vector mins = this->getOrigin() - this->getHalfSizes();
        vector maxs = this->getOrigin() + this->getHalfSizes();


        return vector(std::max(mins.x, std::min(target.x, maxs.x)),
                std::max(mins.y, std::min(target.y, maxs.y)),
                std::max(mins.z, std::min(target.z, maxs.z))
                );
    }

    vector closestSurfacePoint(const vector &target) const {
    	vector surfacePoint;
    	real minDistance = REAL_MAX;

    	real faceCoord = this->getOrigin().y + this->halfSizes.y;
    	real faceDistanceSq = (faceCoord - target.y) * (faceCoord - target.y);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(target.x, faceCoord, target.z);
    	}

    	faceCoord = this->getOrigin().y - this->halfSizes.y;
    	faceDistanceSq = (faceCoord - target.y) * (faceCoord - target.y);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(target.x, faceCoord, target.z);
    	}

    	faceCoord = this->getOrigin().x - this->halfSizes.x;
    	faceDistanceSq = (faceCoord - target.x) * (faceCoord - target.x);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(faceCoord, target.y, target.z);
    	}

    	faceCoord = this->getOrigin().x + this->halfSizes.x;
    	faceDistanceSq = (faceCoord - target.x) * (faceCoord - target.x);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(faceCoord, target.y, target.z);
    	}

    	faceCoord = this->getOrigin().z - this->halfSizes.z;
    	faceDistanceSq = (faceCoord - target.z) * (faceCoord - target.z);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(target.x, target.y, faceCoord);
    	}

    	faceCoord = this->getOrigin().z + this->halfSizes.z;
    	faceDistanceSq = (faceCoord - target.z) * (faceCoord - target.z);
    	if(faceDistanceSq < minDistance) {
    		minDistance = faceDistanceSq;
    		surfacePoint = vector(target.x, target.y, faceCoord);
    	}


		return surfacePoint;
	}
};

/**
 * Loose ends: the bounding volume should be refreshed upon children transformations
 */
class HierarchicalGeometry : public Geometry {
    std::unique_ptr<Geometry> boundingVolume;
    std::vector<std::unique_ptr<Geometry>> children;
public:
    HierarchicalGeometry(Geometry *boundingVolume) : Geometry(boundingVolume->getOrigin()) {
        this->boundingVolume = std::unique_ptr<Geometry>(boundingVolume);
    }
    HierarchicalGeometry(Geometry *boundingVolume, Geometry *child) :  HierarchicalGeometry(boundingVolume) {
		this->addChildren(child);
	}
    const vector& getOrigin() const override {
        return this->boundingVolume->getOrigin();
    }

    void setOrigin(const vector &origin) override {
        vector delta = origin - this->getOrigin();

        this->boundingVolume->setOrigin(origin);

        for(auto &child : children) {
            child->setOrigin(child->getOrigin() + delta);
        }
    }

    void addChildren(Geometry *children) {
        this->children.push_back(std::unique_ptr<Geometry>(children));

    }

    String toString() const override {
        return "HierarchicalGeometry(origin: " + this->getOrigin().toString() + ", children: " + std::to_string(this->children.size()) + ")";
    }

    GeometryType getType() const override {
        return GeometryType::HIERARCHY;
    }

    const Geometry &getBoundingVolume() const {
        return *this->boundingVolume.get();
    }

    const std::vector<std::unique_ptr<Geometry>> &getChildren() const {
        return this->children;
    }
};

class HeightMapGeometry : public AABB {
    const HeightMapResource *heightMap;
    Logger *logger = LoggerFactory::getLogger("geometry/HeightMapGeometry");
public:
    HeightMapGeometry(const vector &position, const HeightMapResource *heightMap) :
    	AABB(position + vector(heightMap->getWidth() * 0.5, heightMap->getHeight() * 0.5, heightMap->getDepth() * 0.5),
    		vector(heightMap->getWidth() * 0.5, heightMap->getHeight() * 0.5, heightMap->getDepth() * 0.5)){
        this->heightMap = heightMap;
    }

    const HeightMapResource *getHeightMap() const {
    	return this->heightMap;
    }

    GeometryType getType() const override {
        return GeometryType::HEIGHTMAP;
    }


    real heightAt(real x, real z) const {
    	return this->getHeightMap()->heightAt(x - this->getPosition().x, z - this->getPosition().z);
    }

    vector normalAt(real x, real z) const {
    	return this->heightMap->normalAt(x - this->getPosition().x, z - this->getPosition().z);
    }
};

/**
 * Loose ends
 *  - contact and collision tests are gona be generic same as hierarchy - maybe could use a single method and add pairs automatically
 * 	- we should check the planes create a closed solid
 */
class Frustum : public Geometry {
	std::vector<Plane> halfSpaces;
public:
	Frustum(const std::vector<Plane> &halfSpaces) : Geometry(vector(0, 0, 0)) {
		this->halfSpaces = halfSpaces;
	}

	GeometryType getType() const override {
		return GeometryType::FRUSTUM;
	}

	const std::vector<Plane>& getHalfSpaces() const {
		return halfSpaces;
	}

	Plane &getHalfSpace(unsigned int index) {
		return halfSpaces[index];
	}
};

#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
