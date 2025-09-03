#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"

#include "AudioRunner.h"

#include "OggResourceAdapter.h"
#include "WavResourceAdapter.h"
#include "BufferResourceAdapter.h"
#include "Logger.h"


//template<typename Adapter>
//class AdapterFixture : public Adapter {
//public:
//  // You can add public helper methods here if needed for testing
//  std::vector<Resource *> doLoad(ResourceLoadRequest &request) const {
//    return Adapter::doLoad(request);
//  }
//};

TEST_CASE("AudioRunner ResourceAdapters Tests (No resourceManager.load())")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);
  ResourceManagerMock resourceManager("resources"); //This will be a new one per SECTION

  SECTION("OggResourceAdapter (No resource manager) test")
  {
    //AdapterFixture<OggResourceAdapter> resourceAdapter;
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<OggResourceAdapter>());

    auto response = resourceAdapter->load(resourceManager.newRequest("audio/audio.ogg"));

    REQUIRE(response.size() == 1);

    AudioResource *resource = (AudioResource *)response.back();
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }

  SECTION("OggResourceAdapter with ResourceManager test")
  {
    //AdapterFixture<OggResourceAdapter> resourceAdapter;
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<OggResourceAdapter>());
    AudioResource *resource = (AudioResource *)resourceManager.load(resourceManager.newRequest("audio/audio.ogg"));
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }

  SECTION("AudioResourceAdapter (No resource manager) test")
  {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<WavResourceAdapter>());
    ResourceLoadRequest request = resourceManager.newRequest("audio/audio.wav");
    auto response = resourceAdapter->load(request);

    REQUIRE(response.size() == 1);

    AudioResource *resource = (AudioResource *)response.back();
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }

  SECTION("AudioResourceAdapter with ResourceManager test")
  {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<WavResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("audio/audio.wav");
    AudioResource *resource = (AudioResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(!resource->getData().empty());
    CHECK(MimeTypes::AUDIO == resource->getMimeType());
  }
}

TEST_CASE("OpenAL ResourceAdapter tests") { //TODO: Move to openALRunner
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  ResourceManager &resourceManager = playground.getResourceManager();
  PlaygroundRunner *runner = playground.addRunner(std::make_unique<AudioRunner>());
  runner->initialize();

  SECTION("AudioBufferResourceAdapter (No resource manager) test") { //requires initialized audio runner
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<BufferResourceAdapter>());

    auto response = resourceAdapter->load(resourceManager.newRequest("audio/audio.ogg").acceptMimeType(MimeTypes::AUDIOBUFFER));
    REQUIRE(response.size() == 1);
    BufferResource *resource = (BufferResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
  }

  SECTION("AudioBufferResourceAdapter with ResourceManager test") { //requires initialized audio runner
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<BufferResourceAdapter>());
    ResourceLoadRequest request = resourceManager.newRequest("audio/audio.ogg").acceptMimeType(MimeTypes::AUDIOBUFFER);
    BufferResource *resource = (BufferResource *)resourceManager.load(request);
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
