#include "AudioRunner.h"

AudioSource *AudioRunner::playBackgroundMusic(AudioBufferResource &buffer) {
  if(!background) {
    background = createSource(buffer);
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

