#include <catch2/catch_test_macros.hpp>

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

//  SECTION("AudioSourceResourceAdapter")   { //TODO: AudioSource should not be a resource or at least should not be cached.
//    Source *resource = (Source *)this->getContainer().getResourceManager()->load("tests/audio.ogg", MimeTypes::AUDIOSOURCE);
//    CHECK(resource != null);
//    CHECK(resource->getId() != 0);
//    CHECK(MimeTypes::AUDIOSOURCE == resource->getMimeType());
//  }
}
