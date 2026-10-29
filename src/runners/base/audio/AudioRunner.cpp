#include "AudioRunner.h"

AudioSource *AudioRunner::playBackgroundMusic(AudioBufferResource &buffer) {
  if(!background) {
    background = createSource(buffer);
    if (!background) {
      logger->error("Could not create background audio source");
      return nullptr;
    }
  } else {
    background->stop();
    background->setBuffer(buffer);
  }
  background->play();
  return background.get();
}

void AudioRunner::stopBackgroundMusic() {
  if(background) {
    background->stop();
  }
}

void AudioRunner::shutdown() {
  while (!sources.empty()) {
    (*sources.begin())->release();
  }
  background.reset();
}

AudioRunner::~AudioRunner() {
  // Backends must call shutdown() before destroying their native context.
  if (!sources.empty()) {
    logger->warn("Audio backend destroyed without releasing its sources");
    for (auto source : sources) {
      source->owner = nullptr;
      source->id = 0;
    }
  }
}
