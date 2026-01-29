/*
 * ResourceManagerMock.h
 *
 *  Created on: Jan 4, 2023
 *      Author: leandro
 */

#pragma once

#include "Playground.h"

class PlaygroundMock : public Playground {
public:
  PlaygroundMock(const String &rootFolder) : Playground(rootFolder){

  }
  unsigned int getRunnersCount() const {
    return this->runners.size();
  }
};
