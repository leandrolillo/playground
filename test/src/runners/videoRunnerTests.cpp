#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"
#include "ImageResource.h"
#include "PngResourceAdapter.h"
#include "JpegResourceAdapter.h"
#include "TgaResourceAdapter.h"
#include "GeometryResourceAdapter.h"
#include "ObjResourceAdapter.h"

#include "DefaultRenderer.h"
#include "Camera.h"

TEST_CASE("Video Runner Test case")
{
  Camera camera;
  REQUIRE(1 == 1);

}

//---

TEST_CASE("Image Resource test case")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  ImageResource resource(MimeTypes::IMAGE);
  resource.resize(100, 200, 16);

  CHECK(resource.getAlto() == 100);
  CHECK(resource.getAncho() == 200);
  CHECK(resource.getBpp() == 16);
  //CHECK(resource.getBytespp() == 2);
  CHECK(resource.getBufferSize() == 100 * 200 * 2);

  resource.resize(200, 300, 8);

  CHECK(resource.getAlto() == 200);
  CHECK(resource.getAncho() == 300);
  CHECK(resource.getBpp() == 8);
  //CHECK(resource.getBytespp() == 1);
  CHECK(resource.getBufferSize() == 200 * 300 * 1);
}

//---

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
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<PngResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("images/image.png");
    auto response = resourceAdapter->load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("PngResourceAdapter with ResourceManager test")
    {
      ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<PngResourceAdapter>());

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
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<JpegResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::IMAGE);
    auto response = resourceAdapter->load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(756 == resource->getAncho());
    CHECK(512 == resource->getAlto());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("JpegResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<JpegResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::IMAGE);
    ImageResource *resource = (ImageResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(756 == resource->getAncho());
    CHECK(512 == resource->getAlto());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  /*****
   * TGA
   *****/
  SECTION("TgaResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<TgaResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("images/image.tga");
    auto response = resourceAdapter->load(request);
    REQUIRE(1 == response.size());

    ImageResource *resource = (ImageResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(400 == resource->getAncho());
    CHECK(300 == resource->getAlto());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("TgaResourceAdapter with ResourceManager test") {
      ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<TgaResourceAdapter>());

      ResourceLoadRequest request = resourceManager.newRequest("images/image.tga").acceptMimeType(MimeTypes::IMAGE);
      ImageResource *resource = (ImageResource *)resourceManager.load(request);
      REQUIRE(resource != null);
      CHECK(resource->getData() != null);
      CHECK(400 == resource->getAncho());
      CHECK(300 == resource->getAlto());
      CHECK(MimeTypes::IMAGE == resource->getMimeType());
    }

  /**********
   * GEOMETRY
   **********/
  SECTION("GeometryResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<GeometryResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json");
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);

    GeometryResource *resource = (GeometryResource *)response.back();
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
  }

  SECTION("GeometryResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<GeometryResourceAdapter>());

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
  }

  /*****
   * OBJ
   *****/
  SECTION("ObjResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ObjResourceAdapter>());
    resourceManager.addAdapter(std::make_unique<MtlResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj");
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);

    GeometryResource *resource = (GeometryResource *)response.back();
    REQUIRE(resource != null);
    CHECK(resource->getMimeType() == MimeTypes::GEOMETRY);
    CHECK(resource->getVertices().size() > 0);
    CHECK(resource->getMaterial() != null); //Should add Material resource adapter for this to be loaded.
  }

  SECTION("ObjResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ObjResourceAdapter>());
    resourceManager.addAdapter(std::make_unique<MtlResourceAdapter>());

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

//---

TEST_CASE("MousePicking tests") {
  vector4 point(0, 0, -1, 0);

  unsigned int width = 640;
  unsigned int height = 480;

  Camera camera;
  camera.setPerspectiveProjectionFov(45.0, (real) width / (real) height, 0.1, 300.0);
  camera.setViewMatrix(matriz_4x4::identidad);

  vector rayDirection = camera.getRayDirection(320, 240, width, height);

//        vector4 eyePoint = camera.getViewMatrix() * point;
//        vector4 projectedPoint = camera.getProjectionMatrix() * point;
//        vector2 ndcPoint = (vector2(projectedPoint.x, projectedPoint.y) + vector2(1, 1)) * (real)0.5;// ((vector)projectedPoint) * ((real)1 / projectedPoint.w);
//            vector2 renderedPoint = ndcPoint;
//
//        renderedPoint.x *= (real)width;
//        renderedPoint.y *= (real)height;
//
//        printf("Transformed point point: %s\n->Eye point: %s\n->Projected point: %s\n->NDC point: %s\n->ViewPort point: %s\n",
//                point.toString("%.2f").c_str(),
//                eyePoint.toString("%.2f").c_str(),
//                projectedPoint.toString("%.2f").c_str(),
//                ndcPoint.toString("%.2f").c_str(),
//                renderedPoint.toString("%.2f").c_str());

  CHECK(vector(0, 0, -1) == rayDirection);
}

TEST_CASE("DefaultRenderer") {
  DefaultRenderer renderer;
}


