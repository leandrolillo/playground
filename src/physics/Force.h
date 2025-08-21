/*
 * Force.h
 *
 *  Created on: Mar 25, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_FORCE_H_
#define SRC_PHYSICS_FORCE_H_

#include<Particle.h>
#include<vector>


class Force {
public:
	virtual void apply(real dt, const std::vector<Particle *> &particles) const = 0;
	virtual ~Force() {}
 };


#endif /* SRC_PHYSICS_FORCE_H_ */
