#include <catch2/catch_test_macros.hpp>

#include "OpenGLRunner.h"

TEST_CASE("OpenGLRunner Test case")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  OpenGLRunner *runner = (OpenGLRunner *)playground.addRunner(new OpenGLRunner());
  runner->initialize();

  SECTION("TextureResourceAdapter") {
    ResourceAdapter *resourceAdapter = runner->getResourceManager().addAdapter(std::make_unique<TextureResourceAdapter>());

    //From PNG
    ResourceLoadRequest pngRequest = runner->getResourceManager().newRequest("images/image.png").acceptMimeType(MimeTypes::TEXTURE);
    auto response = resourceAdapter->load(pngRequest);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From JPG
    ResourceLoadRequest jpgRequest = runner->getResourceManager().newRequest("images/image.jpg").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter->load(jpgRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From TGA
    ResourceLoadRequest tgaRequest = runner->getResourceManager().newRequest("images/image.tga").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter->load(tgaRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());
  }

  SECTION("LoadVertexBuffer") {
    ResourceAdapter *resourceAdapter = runner->getResourceManager().addAdapter(std::make_unique<VertexArrayResourceAdapter>());

    ResourceLoadRequest request = runner->getResourceManager().newRequest("geometry/geometry.json");
    auto response = resourceAdapter->load(request.acceptMimeType(MimeTypes::VERTEXARRAY));
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }

  SECTION("LoadShaders")  {
    ResourceAdapter *resourceAdapter = runner->getResourceManager().addAdapter(std::make_unique<ShaderResourceAdapter>());

    ResourceLoadRequest request = runner->getResourceManager().newRequest("shader/vertexShader.glsl");
    auto response = resourceAdapter->load(request.acceptMimeType(MimeTypes::VERTEXSHADER));
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("LoadShaderProgram") {
    ResourceAdapter *resourceAdapter = runner->getResourceManager().addAdapter(std::make_unique<ShaderProgramResourceAdapter>());

    ResourceLoadRequest request = runner->getResourceManager().newRequest("shader/shaderProgram.json");
    auto response = resourceAdapter->load(request.acceptMimeType(MimeTypes::SHADERPROGRAM));
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }
}
