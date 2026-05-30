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

//Use double for storing time, floats are file for deltas (elapsed time, total time):
//https://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
typedef std::chrono::duration<double> doubleSeconds;

class Chronometer {
  doubleSeconds dt{0s};
  steady_clock::time_point to;
  steady_clock::time_point initialTime { steady_clock::time_point::min()};
public:


  void start() {
    this->to = steady_clock::now();

    if(initialTime == steady_clock::time_point::min()) {
      initialTime = this->to;
    }
  }

  real getElapsedTime() const {
    return dt.count();
  }

  real update() {
    auto tf = steady_clock::now();
    this->dt = duration_cast<doubleSeconds>(tf - to);
    to = tf;

    return dt.count();
  }

  real getTotalTime() const {
    auto tf = steady_clock::now();
    return duration_cast<doubleSeconds>(tf - initialTime).count();
  }

  virtual ~Chronometer() {
  }
};
