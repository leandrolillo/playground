/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once
#include <Math3d.h>
#include "Playground.h"
#include "OggResourceAdapter.h"
#include "SourceResource.h"
#include "WavResourceAdapter.h"

class AudioRunner: public PlaygroundRunner {
private:
Logger *logger = LoggerFactory::getLogger("audio/AudioRunner.h");
public:
static const unsigned char ID;
public:
using PlaygroundRunner::PlaygroundRunner; //inherit constructors

virtual unsigned char getId() const override {
return ID;
}

virtual bool initialize() override {
this->getResourceManager().addAdapter<OggResourceAdapter>();
this->getResourceManager().addAdapter<WavResourceAdapter>();
return true;
}

Source *createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), bool loop = true)
{
Source *source = (Source *)getResourceManager().load(fileName, MimeTypes::AUDIOSOURCE);
if(source != null)
{
source->setPosition(position);
source->setVelocity(velocity);
source->setLoop(loop);
}

return this->updateSource(source);
}

virtual Source *updateSource(Source *source) = 0;
virtual Source *playSource(Source *source) = 0;
virtual Source *stopSource(Source *source) = 0;
virtual Source *pauseSource(Source *source) = 0;
virtual bool updateListener(vector position, vector velocity = vector(0, 0, 0), vector to = vector(0, 0, 1), vector up = vector(0, 1, 0)) = 0;

virtual String toString() const override {
return "AudioRunner(id:" + std::to_string(this->getId()) + ")";
}

virtual ~AudioRunner() = default;

};


const unsigned char AudioRunner::ID = 3;
