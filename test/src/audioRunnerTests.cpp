#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"
#include "ResourceLoadResponseMock.h"

#include "AudioRunner.h"

#include "OggResourceAdapter.h"
#include "WavResourceAdapter.h"
#include "BufferResourceAdapter.h"
#include "Logger.h"

TEST_CASE("AudioRunner ResourceAdapters Tests (No resourceManager.load())")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);
  ResourceManagerMock resourceManager("resources"); //This will be a new one per SECTION

  SECTION("OggResourceAdapter")
  {
    OggResourceAdapter resourceAdapter;

    ResourceLoadRequest request = resourceManager.newRequest("audio.ogg");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    CHECK(1 == resourceManager.getResourcesCount(MimeTypes::AUDIO));

    AudioResource *resource = (AudioResource *)response.getLastAdded();
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }

  SECTION("AudioResourceAdapter")
  {
    WavResourceAdapter resourceAdapter;

    ResourceLoadRequest request = resourceManager.newRequest("audio.wav");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    CHECK(1 == resourceManager.getResourcesCount(MimeTypes::AUDIO));

    AudioResource *resource = (AudioResource *)response.getLastAdded();
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }
}

TEST_CASE("OpenAL ResourceAdapter tests") { //TODO: Move to openALRunner
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  AudioRunner *audioRunner = (AudioRunner *)playground.addRunner(new AudioRunner());
  audioRunner->initialize();

  SECTION("AudioBufferResourceAdapter") { //requires initialized audio runner
    BufferResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(audioRunner->getResourceManager()); //TODO: Move resourceManager to load method?

    ResourceLoadRequest request = audioRunner->getResourceManager()->newRequest("audio.ogg");
    ResourceLoadResponseMock response(request, *audioRunner->getResourceManager());

    resourceAdapter.load(request.acceptMimeType(MimeTypes::AUDIOBUFFER), response);
    BufferResource *resource = (BufferResource *)response.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
  }

//  SECTION("AudioSourceResourceAdapter")   { //TODO: AudioSource should not be a resource or at least should not be cached.
//    Source *resource = (Source *)this->getContainer()->getResourceManager()->load("tests/audio.ogg", MimeTypes::AUDIOSOURCE);
//    CHECK(resource != null);
//    CHECK(resource->getId() != 0);
//    CHECK(MimeTypes::AUDIOSOURCE == resource->getMimeType());
//  }
}
