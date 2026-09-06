#include "AudioRunner.h"

AudioSource *AudioRunner::playBackgroundMusic(AudioBufferResource &buffer) {
  if(!background) {
    background = createSource(buffer);
  }

  background->setBuffer(buffer);
  return background.get();
}

void AudioRunner::stopBackgroundMusic() {
  if(background) {
    background->stop();
  }
}

