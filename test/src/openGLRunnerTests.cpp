#include <catch2/catch_test_macros.hpp>

#include "OpenGLRunner.h"
#include "ResourceLoadResponseMock.h"

TEST_CASE("OpenGLRunner Test case")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  OpenGLRunner *runner = (OpenGLRunner *)playground.addRunner(new OpenGLRunner());
  runner->initialize()
      ;

  SECTION("TextureResourceAdapter") {
    TextureResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(runner->getResourceManager()); //TODO: Move resourceManager to load method?

    ResourceLoadRequest pngRequest = runner->getResourceManager()->newRequest("image.png").acceptMimeType(MimeTypes::TEXTURE);
    ResourceLoadResponseMock pngResponse(pngRequest, *runner->getResourceManager());
    resourceAdapter.load(pngRequest, pngResponse);
    Resource *resource = pngResponse.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    ResourceLoadRequest jpgRequest = runner->getResourceManager()->newRequest("image.jpg").acceptMimeType(MimeTypes::TEXTURE);
    ResourceLoadResponseMock jpgResponse(pngRequest, *runner->getResourceManager());
    resourceAdapter.load(jpgRequest, jpgResponse);
    resource = jpgResponse.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    ResourceLoadRequest tgaRequest = runner->getResourceManager()->newRequest("image.tga").acceptMimeType(MimeTypes::TEXTURE);
    ResourceLoadResponseMock tgaResponse(pngRequest, *runner->getResourceManager());
    resourceAdapter.load(tgaRequest, tgaResponse);
    resource = tgaResponse.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());
  }

  SECTION("LoadVertexBuffer") {
    VertexArrayResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(runner->getResourceManager()); //TODO: Move resourceManager to load method?

    ResourceLoadRequest request = runner->getResourceManager()->newRequest("geometry.json");
    ResourceLoadResponseMock response(request, *runner->getResourceManager());

    resourceAdapter.load(request.acceptMimeType(MimeTypes::VERTEXARRAY), response);
    Resource *resource = response.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }

  SECTION("LoadShaders")  {
    ShaderResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(runner->getResourceManager()); //TODO: Move resourceManager to load method?

    ResourceLoadRequest request = runner->getResourceManager()->newRequest("vertexShader.glsl");

    ResourceLoadResponseMock response(request, *runner->getResourceManager());

    resourceAdapter.load(request.acceptMimeType(MimeTypes::VERTEXSHADER), response);
    Resource *resource = response.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("LoadShaderProgram") {
    ShaderProgramResourceAdapter resourceAdapter;
    resourceAdapter.setResourceManager(runner->getResourceManager()); //TODO: Move resourceManager to load method?

    ResourceLoadRequest request = runner->getResourceManager()->newRequest("shaderProgram.json");
    ResourceLoadResponseMock response(request, *runner->getResourceManager());

    resourceAdapter.load(request.acceptMimeType(MimeTypes::SHADERPROGRAM), response);
    Resource *resource = response.getLastAdded();

    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }
}
