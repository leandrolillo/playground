/*
 * Contact.h
 *
 *  Created on: May 18, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTION_CONTACT_H_
#define SRC_PHYSICS_COLLISIONDETECTION_CONTACT_H_

#include<Math3d.h>

class BaseContact {
protected:
    vector normal;
    vector intersection;
    real penetration;
    real restitution;

public:
    BaseContact(const vector &intersection, const vector &normal, real restitution, real penetration = 0.0) {
        this->intersection = intersection;
        this->normal = normal;
        this->penetration = penetration;
        this->restitution = restitution;
    }

    const vector &getNormal() const {
        return this->normal;
    }

    const vector &getIntersection() const {
        return this->intersection;
    }

    real getPenetration() const {
        return this->penetration;
    }

    real getRestitution() const {
        return this->restitution;
    }

    bool isIntersecting() const {
        return this->penetration >= 0.0;
    }
};



#endif /* SRC_PHYSICS_COLLISIONDETECTION_CONTACT_H_ */
