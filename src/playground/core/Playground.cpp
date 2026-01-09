//============================================================================
// Name        : playground.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Playground.h"


PlaygroundRunner::PlaygroundRunner(Playground &container) : container(container), resourceManager(container.getResourceManager()) {
}


Chronometer &PlaygroundRunner::getStopWatch() const {
	return this->container.getStopWatch();
}
