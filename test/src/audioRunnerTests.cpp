#include <catch2/catch_test_macros.hpp>
#include "AudioRunner.h"
#include "OggResourceAdapter.h"
#include "ResourceManagerMock.h"

TEST_CASE("Audio runner Test case")
{
	AudioRunner runner;
	REQUIRE(1 == 1);
}

TEST_CASE("Ogg Resource Adapter Test case")
{
  OggResourceAdapter resourceAdapter;

  ResourceManagerMock resourceManager("");
  ResourceLoadRequest request("resources/background.ogg");
  ResourceLoadResponse response(request, resourceManager);

  resourceAdapter.load(request, response);

  REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::AUDIO));
}
