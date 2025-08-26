#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"
#include "ImageResource.h"
#include "PngResourceAdapter.h"
#include "Camera.h"


TEST_CASE("Video Runner Test case")
{
	Camera camera;
	REQUIRE(1 == 1);

}

TEST_CASE("Image Resource test case")
{
  ImageResource resource(MimeTypes::IMAGE);
  resource.resize(100, 200, 16);

  REQUIRE(resource.getAlto() == 100);
  REQUIRE(resource.getAncho() == 200);
  REQUIRE(resource.getBpp() == 16);
  //REQUIRE(resource.getBytespp() == 2);
  REQUIRE(resource.getBufferSize() == 100 * 200 * 2);


  resource.resize(200, 300, 8);

  REQUIRE(resource.getAlto() == 200);
  REQUIRE(resource.getAncho() == 300);
  REQUIRE(resource.getBpp() == 8);
  //REQUIRE(resource.getBytespp() == 1);
  REQUIRE(resource.getBufferSize() == 200 * 300 * 1);
}

TEST_CASE("PngResourceAdapter test case")
{
  PngResourceAdapter adapter;

  ResourceManagerMock resourceManager("");
  ResourceLoadRequest request("resources/cuadrille.png");
  ResourceLoadResponse response(request, resourceManager);

  adapter.load(request, response);

  REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::IMAGE));


}
