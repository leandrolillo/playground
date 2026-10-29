#include <catch2/catch_test_macros.hpp>
#include <type_traits>

#include "OggResourceAdapter.h"
#include "WavResourceAdapter.h"
#include "AudioBufferResourceAdapter.h"
#include "ResourceManagerMock.h"

#include "OpenALRunner.h"

#include "Logger.h"

TEST_CASE("OpenAL ResourceAdapter tests") { //TODO: Move to openALRunner
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  ResourceManager &resourceManager = playground.getResourceManager();
  OpenALRunner &runner = playground.addRunner<OpenALRunner>();
  runner.initialize();

  SECTION("AudioBufferResourceAdapter (No resource manager) test") { //requires initialized audio runner
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<AudioBufferResourceAdapter>();

    auto response = resourceAdapter.load(resourceManager.newRequest("audio/audio.ogg").acceptMimeType(MimeTypes::AUDIOBUFFER));
    REQUIRE(response.size() == 1);
    AudioBufferResource *resource = (AudioBufferResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
  }

  SECTION("AudioBufferResourceAdapter with ResourceManager test") { //requires initialized audio runner
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<AudioBufferResourceAdapter>();
    ResourceLoadRequest request = resourceManager.newRequest("audio/audio.ogg").acceptMimeType(MimeTypes::AUDIOBUFFER);
    AudioBufferResource *resource = (AudioBufferResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
  }

}

static_assert(!std::is_copy_constructible_v<AudioSource>);
static_assert(!std::is_copy_assignable_v<AudioSource>);
static_assert(!std::is_move_constructible_v<AudioSource>);
static_assert(!std::is_move_assignable_v<AudioSource>);
static_assert(std::is_same_v<decltype(std::declval<AudioSource>().getPosition()), vector>);
static_assert(std::is_same_v<decltype(std::declval<AudioSource>().getVelocity()), vector>);

TEST_CASE("OpenAL sources are independent playback instances", "[audio]") {
  LoggerFactory::doNotLogToFile();
  Playground playground("resources");
  auto &runner = playground.addRunner<OpenALRunner>();

  auto first = runner.createSource("audio/audio.wav");
  auto second = runner.createSource("audio/audio.wav");
  REQUIRE(first);
  REQUIRE(second);
  CHECK(first->getId() != second->getId());

  ALint firstBuffer = 0;
  ALint secondBuffer = 0;
  alGetSourcei(first->getId(), AL_BUFFER, &firstBuffer);
  alGetSourcei(second->getId(), AL_BUFFER, &secondBuffer);
  CHECK(firstBuffer != 0);
  CHECK(firstBuffer == secondBuffer);

  first->setPosition(vector(1, 2, 3));
  first->setVelocity(vector(4, 5, 6));
  first->setGain(0.5f);
  first->setLooping(false);
  CHECK(first->getPosition().x == 1);
  CHECK(first->getPosition().y == 2);
  CHECK(first->getPosition().z == 3);
  CHECK(first->getVelocity().x == 4);
  CHECK(first->getVelocity().y == 5);
  CHECK(first->getVelocity().z == 6);
  CHECK(second->getPosition().x == 0);
  CHECK(second->getGain() == 1);
  CHECK(second->getLooping());

  first->play();
  second->play();
  first->pause();
  ALint state = 0;
  alGetSourcei(first->getId(), AL_SOURCE_STATE, &state);
  CHECK(state == AL_PAUSED);
  alGetSourcei(second->getId(), AL_SOURCE_STATE, &state);
  CHECK(state == AL_PLAYING);

  auto firstId = first->getId();
  first.reset();
  CHECK_FALSE(alIsSource(firstId));
  CHECK(alIsSource(second->getId()));
  CHECK(alIsBuffer(firstBuffer));
  CHECK(alGetError() == AL_NO_ERROR);
}

TEST_CASE("OpenAL background music reuses an instance and shuts down safely", "[audio]") {
  LoggerFactory::doNotLogToFile();
  Playground playground("resources");
  auto &runner = playground.addRunner<OpenALRunner>();
  auto background = runner.playBackgroundMusic("audio/audio.wav");
  REQUIRE(background);
  auto sourceId = background->getId();
  CHECK(runner.playBackgroundMusic("audio/awesomeness.wav") == background);
  CHECK(background->getId() == sourceId);
  runner.stopBackgroundMusic();
  ALint state = 0;
  alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
  CHECK(state == AL_STOPPED);

  CHECK(runner.playBackgroundMusic("audio/missing.wav") == nullptr);
  CHECK(alIsSource(sourceId));
  runner.shutdown();
  CHECK_FALSE(alIsSource(sourceId));
  runner.shutdown();
  CHECK(alGetError() == AL_NO_ERROR);
}

class ObservedAudioBufferResourceAdapter : public AudioBufferResourceAdapter {
  bool &disposed;
  unsigned int &sourceId;
public:
  ObservedAudioBufferResourceAdapter(ResourceManager &manager, bool &disposed, unsigned int &sourceId) :
      AudioBufferResourceAdapter(manager), disposed(disposed), sourceId(sourceId) {
  }

  void dispose(Resource *resource) const override {
    CHECK(alcGetCurrentContext() != nullptr);
    CHECK_FALSE(alIsSource(sourceId));
    AudioBufferResourceAdapter::dispose(resource);
    CHECK_FALSE(alIsBuffer(resource->getId()));
    CHECK(alGetError() == AL_NO_ERROR);
    disposed = true;
  }
};

TEST_CASE("OpenAL shutdown releases sources before buffers and context", "[audio]") {
  LoggerFactory::doNotLogToFile();
  std::unique_ptr<AudioSource> source;
  bool bufferDisposed = false;
  unsigned int sourceId = 0;
  {
    Playground playground("resources");
    auto &runner = playground.addRunner<OpenALRunner>();
    playground.getResourceManager().addAdapter<ObservedAudioBufferResourceAdapter>(bufferDisposed, sourceId);
    source = runner.createSource("audio/audio.wav");
    REQUIRE(source);
    sourceId = source->getId();
    source->play();
    REQUIRE(runner.playBackgroundMusic("audio/audio.wav"));
  }
  CHECK(bufferDisposed);
  CHECK(alcGetCurrentContext() == nullptr);
  CHECK(source->getId() == 0);
  CHECK_THROWS_AS(source->play(), std::logic_error);
  CHECK_THROWS_AS(source->setGain(1), std::logic_error);
  CHECK_THROWS_AS(source->getPosition(), std::logic_error);
  source.reset();
}

class FailingSourceOpenALRunner : public OpenALRunner {
public:
  using OpenALRunner::OpenALRunner;
protected:
  unsigned int createSource() override {
    return 0;
  }
};

TEST_CASE("OpenAL source creation failure does not produce an invalid instance", "[audio]") {
  LoggerFactory::doNotLogToFile();
  Playground playground("resources");
  AudioRunner &runner = playground.addRunner<FailingSourceOpenALRunner>();
  CHECK_THROWS_AS(runner.createSource("audio/audio.wav"), std::runtime_error);
  CHECK_THROWS_AS(runner.playBackgroundMusic("audio/audio.wav"), std::runtime_error);
  runner.shutdown();
}

TEST_CASE("OpenAL runner destruction invalidates surviving source wrappers", "[audio]") {
  LoggerFactory::doNotLogToFile();
  Playground playground("resources");
  std::unique_ptr<AudioSource> source;
  {
    OpenALRunner runner(playground);
    source = std::make_unique<AudioSource>(runner);
    REQUIRE(alIsSource(source->getId()));
  }
  CHECK(source->getId() == 0);
  CHECK_THROWS_AS(source->stop(), std::logic_error);
  source.reset();
}
