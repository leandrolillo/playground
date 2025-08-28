#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"
#include "ResourceLoadResponseMock.h"
#include "ImageResource.h"
#include "PngResourceAdapter.h"
#include "JpegResourceAdapter.h"
#include "TgaResourceAdapter.h"
#include "GeometryResourceAdapter.h"
#include "ObjResourceAdapter.h"

#include "Camera.h"

TEST_CASE("Video Runner Test case")
{
  Camera camera;
  REQUIRE(1 == 1);

}

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

TEST_CASE("VideoRunner ResourceAdapters Tests (No resourceManager.load())")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  ResourceManagerMock resourceManager("resources"); //This will be a new one per SECTION

  SECTION("PngResourceAdapter tests")
  {
    PngResourceAdapter resourceAdapter;

    ResourceLoadRequest request = resourceManager.newRequest("image.png");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::IMAGE));
    ImageResource *resource = (ImageResource *)response.getLastAdded();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("JpegResourceAdapter tests") {
    JpegResourceAdapter resourceAdapter;

    ResourceLoadRequest request = resourceManager.newRequest("image.jpg");
    ResourceLoadResponseMock response(request, resourceManager);
    resourceAdapter.load(request, response);

    REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::IMAGE));

    ImageResource *resource = (ImageResource *)response.getLastAdded();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(756 == resource->getAncho());
    CHECK(512 == resource->getAlto());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("TgaResourceAdapter") {
    TgaResourceAdapter resourceAdapter;

    ResourceLoadRequest request = resourceManager.newRequest("image.tga");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::IMAGE));

    ImageResource *resource = (ImageResource *)response.getLastAdded();
    REQUIRE(resource != null);
    CHECK(resource->getData() != null);
    CHECK(400 == resource->getAncho());
    CHECK(300 == resource->getAlto());
    CHECK(MimeTypes::IMAGE == resource->getMimeType());
  }

  SECTION("GeometryResourceAdapter") {
    GeometryResourceAdapter resourceAdapter;
    ResourceLoadRequest request = resourceManager.newRequest("geometry.json");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    REQUIRE(1 == resourceManager.getResourcesCount(MimeTypes::GEOMETRYCOLLECTION));

    GeometryCollection *resources = (GeometryCollection *)response.getLastAdded();
    REQUIRE(resources != null);
    GeometryResource *resource = resources->getObject("geometry.json");
    REQUIRE(resource != null);
    CHECK(MimeTypes::GEOMETRY == resource->getMimeType());
    CHECK(11 == resource->getVertices().size());
    CHECK(3 == resource->getColors().size());
    CHECK(3 == resource->getTextureCoordinates().size());
    CHECK(3 == resource->getNormals().size());
  }
  SECTION("ObjResourceAdapter tests") {
    ObjResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(&resourceManager);

    ResourceLoadRequest request = resourceManager.newRequest("axes.obj");
    ResourceLoadResponseMock response(request, resourceManager);

    resourceAdapter.load(request, response);

    GeometryCollection *resources = (GeometryCollection *)response.getLastAdded();
    REQUIRE(resources != null);
    GeometryResource *resource = resources->getObject("Axes");
    REQUIRE(resource != null);
  }
}

  //  SECTION("ResourceManager Load") {
//    /*
//     * Note that resource Adapters are owned by resource manager, thus we should send a new() and not a heap variable.
//     * Otherwise we get crazy things like accessing deleted memory in logs appenders.
//     */
//    resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new GeometryResourceAdapter()));
//
//    Resource *resource = resourceManager.load("tests/geometry.json", MimeTypes::GEOMETRYCOLLECTION);
//    CHECK(resource != null);
//  }
//
//      SECTION("ObjResourceAdapter tests") {
//        ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());
//        ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new ObjResourceAdapter()));
//
//        ResourceLoadRequest request(Paths::normalize("tests/axes.obj", runner->getContainer()->getResourceManager()->getRootFolder()));
//        ResourceLoadResponseMock response(request, resourceManager);
//        resourceAdapter->load(request.acceptMimeType(MimeTypes::GEOMETRYCOLLECTION), response);
//        Resource *resource = response.getLastAdded();
//        CHECK(resource != null);
////  SECTION("ResourceMAnager Load") {
////    /*
////     * Note that resource Adapters are owned by resource manager, thus we should send a new() and not a heap variable.
////     * Otherwise we get crazy things like accessing deleted memory in logs appenders.
////     */
////    resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new ObjResourceAdapter()));
////
////    Resource *resource = resourceManager.load("tests/axes.obj", MimeTypes::GEOMETRYCOLLECTION);
////    CHECK(resource != null);
////    CHECK(4, = resourceManager.getResourcesCount(MimeTypes::GEOMETRY));
////    CHECK(1 = resourceManager.getResourcesCount(MimeTypes::GEOMETRYCOLLECTION));
////  }
//      }
//    }
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
