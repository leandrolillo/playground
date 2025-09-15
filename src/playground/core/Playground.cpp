//============================================================================
// Name        : playground.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Playground.h"



ResourceManager &PlaygroundRunner::getResourceManager() const {
  if(this->container == null) {
    throw std::domain_error("Playground Runner [" + this->toString() + "] container is null");
  }
	return this->container->getResourceManager();
}
