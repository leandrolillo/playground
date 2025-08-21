//============================================================================
// Name        : playground.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../core/Playground.h"



ResourceManager *PlaygroundRunner::getResourceManager() const {
	return this->container->getResourceManager();
}

Chronometer &PlaygroundRunner::getStopWatch() const {
	return this->container->getStopWatch();
}
