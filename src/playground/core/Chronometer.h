/*
 * Chronometer.h
 *
 *  Created on: Apr 17, 2023
 *      Author: leandro
 */

#pragma once
#include<Math3d.h>
#include<chrono>

using namespace std::chrono;

typedef std::chrono::duration<real> floatSeconds;

/**
 * Performance counter and performance frequency are platform specific - thus have to be provided by some runner
 */
class Chronometer {
  floatSeconds dt{0s};
	steady_clock::time_point to;
	steady_clock::time_point initialTime { steady_clock::time_point::min()};
public:


	void start() {
		this->to = steady_clock::now();

		if(initialTime == steady_clock::time_point::min()) {
			initialTime = this->to;
		}
	}

	real getElapsedTime() {
		return dt.count();
	}

	real update() {
		auto tf = steady_clock::now();
		this->dt = duration_cast<floatSeconds>(tf - to);
		to = tf;

		return dt.count();

	}

	real getTotalTime() {
		auto tf = steady_clock::now();
		return duration_cast<floatSeconds>(tf - initialTime).count();
	}

	virtual ~Chronometer() {
	}
};
