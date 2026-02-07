#include <catch2/catch_test_macros.hpp>
#include "mathMatchers.h"

#include "ResourceManagerMock.h"
#include "GeometryResourceAdapterMock.h"

#include "ImageResource.h"
#include "PngResourceAdapter.h"
#include "JpegResourceAdapter.h"
#include "TgaResourceAdapter.h"
#include "GeometryResourceAdapter.h"
#include "ObjResourceAdapter.h"

#include "DefaultRenderer.h"
#include "GeometryRenderer.h"
#include "TerrainRenderer.h"
#include "GridRenderer.h"
#include "SpriteRenderer.h"
#include "Camera.h"

TEST_CASE("VideoRunner ResourceAdapters Tests")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  ResourceManagerMock resourceManager("resources"); //This will be a new one per SECTION

  /*****
   * PNG
   *****/
  SECTION("PngResourceAdapter (No resource manager) test")
  {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<PngResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("images/image.png");
    auto response = resourceAdapter.load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("PngResourceAdapter with ResourceManager test")
    {
      ResourceAdapter &resourceAdapter = resourceManager.addAdapter<PngResourceAdapter>();

      ResourceLoadRequest request = resourceManager.newRequest("images/image.png").acceptMimeType(MimeTypes::IMAGE);
      ImageResource *resource = (ImageResource *)resourceManager.load(request);
      REQUIRE(resource != null);
      CHECK(resource->getData() != null);
      CHECK(MimeTypes::IMAGE == resource->getMimeType());
    }

  /*****
   * JPG
   *****/

  SECTION("JpegResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<JpegResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::IMAGE);
    auto response = resourceAdapter.load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(756 == resource->getWidth());
    CHECK(512 == resource->getHeight());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("JpegResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<JpegResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::IMAGE);
    ImageResource *resource = (ImageResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(756 == resource->getWidth());
    CHECK(512 == resource->getHeight());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  /*****
   * TGA
   *****/
  SECTION("TgaResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<TgaResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("images/image.tga");
    auto response = resourceAdapter.load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(400 == resource->getWidth());
    CHECK(300 == resource->getHeight());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("TgaResourceAdapter with ResourceManager test") {
      ResourceAdapter &resourceAdapter = resourceManager.addAdapter<TgaResourceAdapter>();

      ResourceLoadRequest request = resourceManager.newRequest("images/image.tga").acceptMimeType(MimeTypes::IMAGE);
      ImageResource *resource = (ImageResource *)resourceManager.load(request);
      REQUIRE(resource != null);
      CHECK(resource->getData() != null);
      CHECK(400 == resource->getWidth());
      CHECK(300 == resource->getHeight());
      CHECK(MimeTypes::IMAGE == resource->getMimeType());
    }



//  /**********
//   * GEOMETRY
//   **********/
  SECTION("GeometryResourceAdapter tests via Mock object") {
    GeometryResourceAdapterMock &resourceAdapter = resourceManager.addAdapter<GeometryResourceAdapterMock>();

    CHECK(PrimitiveType::POINTS == resourceAdapter.asPrimitiveTypePublic("points"));
    CHECK(PrimitiveType::POINTS == resourceAdapter.asPrimitiveTypePublic(" points "));
    CHECK(PrimitiveType::POINTS == resourceAdapter.asPrimitiveTypePublic("Points"));

    CHECK(PrimitiveType::LINE_LOOP == resourceAdapter.asPrimitiveTypePublic("LineLoop"));
    CHECK(PrimitiveType::LINE_STRIP == resourceAdapter.asPrimitiveTypePublic("lineStrip"));
    CHECK(PrimitiveType::LINES == resourceAdapter.asPrimitiveTypePublic("lines"));
    CHECK(PrimitiveType::TRIANGLES == resourceAdapter.asPrimitiveTypePublic("triangles"));
    CHECK(PrimitiveType::TRIANGLE_STRIP == resourceAdapter.asPrimitiveTypePublic("triangleStrip"));
    CHECK(PrimitiveType::TRIANGLE_FAN == resourceAdapter.asPrimitiveTypePublic("triangleFan"));
    CHECK(PrimitiveType::QUADS == resourceAdapter.asPrimitiveTypePublic("quads"));
    CHECK(PrimitiveType::QUAD_STRIP == resourceAdapter.asPrimitiveTypePublic("quadStrip"));
    CHECK(PrimitiveType::POLYGON == resourceAdapter.asPrimitiveTypePublic("polygon"));
  }

  SECTION("GeometryResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<GeometryResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json");
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);

    GeometryResource *resource = (GeometryResource *)response.back();
    REQUIRE(resource != null);
    CHECK(MimeTypes::GEOMETRY == resource->getMimeType());
    CHECK(11 == resource->getVertices().size());
    CHECK(3 == resource->getColors().size());
    CHECK(3 == resource->getTextureCoordinates().size());
    CHECK(3 == resource->getNormals().size());
    CHECK("geometry" == resource->getName());
    CHECK(PrimitiveType::TRIANGLES == resource->getType());

    REQUIRE(resource->getMaterial() != null);
    CHECK(resource->getMaterial()->getName() == "material");
    CHECK(resource->getMaterial()->getMimeType() == MimeTypes::MATERIAL);
    CHECK(resource->getMaterial()->getDiffuseTexture() == "/images/image.png");
    CHECK(resource->getMaterial()->getAmbientTexture() == "/geometry/fakeImage.png");
  }

  SECTION("GeometryResourceAdapter with ResourceManager test") {
    try {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<GeometryResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json/geometry").acceptMimeType(MimeTypes::GEOMETRY);
    GeometryResource *resource = (GeometryResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(MimeTypes::GEOMETRY == resource->getMimeType());
    CHECK(11 == resource->getVertices().size());
    CHECK(3 == resource->getColors().size());
    CHECK(3 == resource->getTextureCoordinates().size());
    CHECK(3 == resource->getNormals().size());
    CHECK("geometry" == resource->getName());

    REQUIRE(resource->getMaterial() != null);
    CHECK(resource->getMaterial()->getName() == "material");
    CHECK(resource->getMaterial()->getMimeType() == MimeTypes::MATERIAL);
    CHECK(resource->getMaterial()->getDiffuseTexture() == "/images/image.png");
    CHECK(resource->getMaterial()->getAmbientTexture() == "/geometry/fakeImage.png");
    } catch(const std::exception &exception) {
      INFO("Got exception: " << exception.what());
    }
  }

  /*****
   * OBJ
   *****/
  SECTION("ObjResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ObjResourceAdapter>();
    resourceManager.addAdapter<MtlResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj");
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);

    GeometryResource *resource = (GeometryResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getMimeType() == MimeTypes::GEOMETRY);
    CHECK(resource->getVertices().size() > 0);
    CHECK(resource->getMaterial() != null); //Should add Material resource adapter for this to be loaded.
  }

  SECTION("ObjResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ObjResourceAdapter>();
    resourceManager.addAdapter<MtlResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj/Axes").acceptMimeType(MimeTypes::GEOMETRY);
    GeometryResource *resource = (GeometryResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getMimeType() == MimeTypes::GEOMETRY);
    CHECK(resource->getVertices().size() > 0);
    REQUIRE(resource->getMaterial() != null);
    CHECK(resource->getMaterial()->getName() == "Material.002");
    CHECK(resource->getMaterial()->getDiffuseTexture() == "/geometry/image.png");
    CHECK(resource->getMaterial()->getShininess() == 250);



    CHECK(4 == resourceManager.getResourcesCount(MimeTypes::GEOMETRY));
  }
}
