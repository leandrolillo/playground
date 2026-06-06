#include "AudioSource.h"
#include "AudioRunner.h"

AudioSource::AudioSource(AudioRunner &owner) :
  owner(owner) {
  this->id = owner.createSource();
}


AudioSource::~AudioSource() {
  this->owner.deleteSource(this->getId());
}

void AudioSource::setBuffer(AudioBufferResource &buffer) {
  this->owner.setSourceBuffer(this->getId(), buffer.getId());
}

const vector& AudioSource::getPosition() const {
  return this->owner.getSourcePosition(this->id);
}

void AudioSource::setPosition(const vector& position) {
  this->owner.setSourcePosition(this->id, position);
}

const vector& AudioSource::getVelocity() const {
  return this->owner.getSourceVelocity(this->id);
}

void AudioSource::setVelocity(const vector& velocity) {
  this->owner.setSourceVelocity(this->id, velocity);
}

float AudioSource::getGain() const {
  return this->owner.getSourceGain(this->id);
}

void AudioSource::setGain(float gain) {
  this->owner.setSourceGain(this->id, gain);
}

unsigned char AudioSource::getLooping() const {
  return this->owner.getSourceLooping(this->id);
}

void AudioSource::setLooping(bool looping) {
  this->owner.setSourceLooping(this->id, looping);
}

float AudioSource::getPitch() const {
  return this->owner.getSourcePitch(this->id);
}

void AudioSource::setPitch(float pitch) {
  this->owner.setSourcePitch(this->id, pitch);
}

float AudioSource::getRolloffFactor() const {
  return this->owner.getSourceRolloffFactor(this->id);
}

void AudioSource::setRolloffFactor(float rolloffFactor) {
  owner.setSourceRolloffFactor(this->id, rolloffFactor);
}


/*
 * Playback methods
 */
void AudioSource::play() {
  this->owner.playSource(this->id);

}
void AudioSource::stop() {
  this->owner.stopSource(this->id);
}
void AudioSource::pause() {
  this->owner.pauseSource(this->id);
}
