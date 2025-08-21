/*
 * Chronometer.h
 *
 *  Created on: Apr 17, 2023
 *      Author: leandro
 */

#ifndef SRC_CORE_CHRONOMETER_H_
#define SRC_CORE_CHRONOMETER_H_

#include<math3d.h>

/**
 * Performance counter and performance frequency are platform specific - thus have to be provided by some runner
 */
class Chronometer {
	real invPerformanceFreq = -1.0f;
	real dt = 0;
	unsigned long to = 0;
	unsigned long initialTime = 0;
public:
	virtual unsigned long getPerformanceCounter() const = 0;
	virtual unsigned long getPerformanceFreq() const = 0;


	void start() {
		this->to = getPerformanceCounter();

		if(invPerformanceFreq < 0.0) {
			invPerformanceFreq = (real)1 / (real)getPerformanceFreq();
		}

		if(initialTime == 0) {
			initialTime = this->to;
		}
	}

	real getElapsedTime() {
		return dt;
	}

	real update() {
		unsigned long tf = getPerformanceCounter();
		this->dt = (real)(tf - to) * invPerformanceFreq;
		to = tf;

		return dt;

	}

	real getTotalTime() {
		unsigned long tf = getPerformanceCounter();
		return (real)(tf - initialTime) * invPerformanceFreq;
	}

	virtual ~Chronometer() {
	}
};



#endif /* SRC_CORE_CHRONOMETER_H_ */
