#include <catch2/catch_test_macros.hpp>

#include "OpenGLRunner.h"
#include "SkyboxRenderer.h"

TEST_CASE("OpenGLRunner Test case", "[opengl]")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  PlaygroundRunner *runner = playground.addRunner(std::make_unique<OpenGLRunner>());
  runner->initialize();
  ResourceManager &resourceManager = runner->getResourceManager();

  SECTION("TextureResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<TextureResourceAdapter>());

    //From PNG
    ResourceLoadRequest pngRequest = resourceManager.newRequest("images/image.png").acceptMimeType(MimeTypes::TEXTURE);
    auto response = resourceAdapter->load(pngRequest);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From JPG
    ResourceLoadRequest jpgRequest = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter->load(jpgRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From TGA
    ResourceLoadRequest tgaRequest = resourceManager.newRequest("images/image.tga").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter->load(tgaRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());
  }

  SECTION("TextureResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<TextureResourceAdapter>());

    //From PNG
    ResourceLoadRequest pngRequest = resourceManager.newRequest("images/image.png").acceptMimeType(MimeTypes::TEXTURE);
    Resource *resource = resourceManager.load(pngRequest);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From JPG
    ResourceLoadRequest jpgRequest = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::TEXTURE);
    resource = resourceManager.load(jpgRequest);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From TGA
    ResourceLoadRequest tgaRequest = resourceManager.newRequest("images/image.tga").acceptMimeType(MimeTypes::TEXTURE);
    resource = resourceManager.load(tgaRequest);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());
  }

  SECTION("VertexArrayResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<VertexArrayResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json/geometry").acceptMimeType(MimeTypes::VERTEXARRAY);
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }

  SECTION("VertexArrayResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<VertexArrayResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json/geometry").acceptMimeType(MimeTypes::VERTEXARRAY);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }

  SECTION("MeshResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<MeshResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj/Axes").acceptMimeType(MimeTypes::MESH);
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);
    MeshResource *resource = (MeshResource *)response.back();
    REQUIRE(resource != null);
    CHECK(MimeTypes::MESH == resource->getMimeType());
    CHECK(resource->getVertexArray() != null);
    CHECK(resource->getMaterial() != null);
  }

  SECTION("MeshResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<MeshResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj/Axes").acceptMimeType(MimeTypes::MESH);
    MeshResource *resource = (MeshResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(MimeTypes::MESH == resource->getMimeType());
    CHECK(resource->getVertexArray() != null);
    CHECK(resource->getMaterial() != null);
  }

  SECTION("ShaderResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ShaderResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("shader/vertexShader.glsl").acceptMimeType(MimeTypes::VERTEXSHADER);
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("ShaderResourceAdapter with Resource Manager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ShaderResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("shader/vertexShader.glsl").acceptMimeType(MimeTypes::VERTEXSHADER);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("ShaderProgramResourceAdapter (No resource manager) test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ShaderProgramResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("shader/shaderProgram.json").acceptMimeType(MimeTypes::SHADERPROGRAM);
    auto response = resourceAdapter->load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }

  SECTION("ShaderProgramResourceAdapter with ResourceManager test") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::make_unique<ShaderProgramResourceAdapter>());

    ResourceLoadRequest request = resourceManager.newRequest("shader/shaderProgram.json").acceptMimeType(MimeTypes::SHADERPROGRAM);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }
}

TEST_CASE("OpenGL Renderers") {
  //TODO: implement test cases
  SkyboxRenderer skyboxRenderer();
}
