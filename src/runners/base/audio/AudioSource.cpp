#include "AudioSource.h"
#include "AudioRunner.h"
#include <stdexcept>

AudioSource::AudioSource(AudioRunner &owner) :
  owner(&owner) {
  this->id = owner.createSource();
  if (id == 0) {
    throw std::runtime_error("Could not create audio source");
  }
  try {
    owner.sources.insert(this);
  } catch (...) {
    owner.deleteSource(id);
    throw;
  }
}


AudioSource::~AudioSource() {
  release();
}

AudioRunner &AudioSource::getOwner() const {
  if (owner == nullptr) {
    throw std::logic_error("Audio source has been released");
  }
  return *owner;
}

void AudioSource::release() {
  if (owner != nullptr) {
    owner->deleteSource(id);
    owner->sources.erase(this);
    owner = nullptr;
    id = 0;
  }
}

void AudioSource::setBuffer(AudioBufferResource &buffer) {
  getOwner().setSourceBuffer(this->getId(), buffer.getId());
}

vector AudioSource::getPosition() const {
  return getOwner().getSourcePosition(this->id);
}

void AudioSource::setPosition(const vector& position) {
  getOwner().setSourcePosition(this->id, position);
}

vector AudioSource::getVelocity() const {
  return getOwner().getSourceVelocity(this->id);
}

void AudioSource::setVelocity(const vector& velocity) {
  getOwner().setSourceVelocity(this->id, velocity);
}

float AudioSource::getGain() const {
  return getOwner().getSourceGain(this->id);
}

void AudioSource::setGain(float gain) {
  getOwner().setSourceGain(this->id, gain);
}

unsigned char AudioSource::getLooping() const {
  return getOwner().getSourceLooping(this->id);
}

void AudioSource::setLooping(bool looping) {
  getOwner().setSourceLooping(this->id, looping);
}

float AudioSource::getPitch() const {
  return getOwner().getSourcePitch(this->id);
}

void AudioSource::setPitch(float pitch) {
  getOwner().setSourcePitch(this->id, pitch);
}

float AudioSource::getRolloffFactor() const {
  return getOwner().getSourceRolloffFactor(this->id);
}

void AudioSource::setRolloffFactor(float rolloffFactor) {
  getOwner().setSourceRolloffFactor(this->id, rolloffFactor);
}


/*
 * Playback methods
 */
void AudioSource::play() {
  getOwner().playSource(this->id);

}
void AudioSource::stop() {
  getOwner().stopSource(this->id);
}
void AudioSource::pause() {
  getOwner().pauseSource(this->id);
}
