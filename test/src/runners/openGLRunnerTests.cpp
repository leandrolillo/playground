#include <catch2/catch_test_macros.hpp>

#include "../../../src/runners/video/renderers/SkyboxRenderer.h"
#include "OpenGLRunner.h"
#include "VideoRunnerMock.h"

TEST_CASE("OpenGLRunner Test case", "[opengl]")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  Playground playground("resources");
  OpenGLRunner &runner = playground.addRunner<OpenGLRunner>();
  runner.initialize();
  ResourceManager &resourceManager = runner.getResourceManager();

  SECTION("TextureResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<TextureResourceAdapter>();

    //From PNG
    ResourceLoadRequest pngRequest = resourceManager.newRequest("images/image.png").acceptMimeType(MimeTypes::TEXTURE);
    auto response = resourceAdapter.load(pngRequest);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From JPG
    ResourceLoadRequest jpgRequest = resourceManager.newRequest("images/image.jpg").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter.load(jpgRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());

    //From TGA
    ResourceLoadRequest tgaRequest = resourceManager.newRequest("images/image.tga").acceptMimeType(MimeTypes::TEXTURE);
    response = resourceAdapter.load(tgaRequest);
    REQUIRE(response.size() > 0);
    resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::TEXTURE == resource->getMimeType());
  }

  SECTION("TextureResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<TextureResourceAdapter>();

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
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<VertexArrayResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json/geometry").acceptMimeType(MimeTypes::VERTEXARRAY);
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }
  SECTION("VertexArrayResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<VertexArrayResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/geometry.json/geometry").acceptMimeType(MimeTypes::VERTEXARRAY);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
  }

  SECTION("MeshResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<MeshResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj/Axes").acceptMimeType(MimeTypes::MESH);
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);
    MeshResource *resource = (MeshResource *)response.back();
    REQUIRE(resource != null);
    CHECK(MimeTypes::MESH == resource->getMimeType());
    CHECK(resource->getVertexArray() != null);
    CHECK(resource->getMaterial() != null);
  }

  SECTION("MeshResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<MeshResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("geometry/axes.obj/Axes").acceptMimeType(MimeTypes::MESH);
    MeshResource *resource = (MeshResource *)resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(MimeTypes::MESH == resource->getMimeType());
    CHECK(resource->getVertexArray() != null);
    CHECK(resource->getMaterial() != null);
  }

  SECTION("ShaderResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ShaderResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("shader/vertexShader.glsl").acceptMimeType(MimeTypes::VERTEXSHADER);
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("ShaderResourceAdapter with Resource Manager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ShaderResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("shader/vertexShader.glsl").acceptMimeType(MimeTypes::VERTEXSHADER);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
  }

  SECTION("ShaderProgramResourceAdapter (No resource manager) test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ShaderProgramResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("shader/shaderProgram.json").acceptMimeType(MimeTypes::SHADERPROGRAM);
    auto response = resourceAdapter.load(request);
    REQUIRE(response.size() > 0);
    Resource *resource = response.back();
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }

  SECTION("ShaderProgramResourceAdapter with ResourceManager test") {
    ResourceAdapter &resourceAdapter = resourceManager.addAdapter<ShaderProgramResourceAdapter>();

    ResourceLoadRequest request = resourceManager.newRequest("shader/shaderProgram.json").acceptMimeType(MimeTypes::SHADERPROGRAM);
    Resource *resource = resourceManager.load(request);
    REQUIRE(resource != null);
    CHECK(resource->getId() != 0);
    CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
  }
}

TEST_CASE("OpenGLUtilities test", "[opengl]") {
  CHECK(GL_POINTS == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::POINTS));
  CHECK(GL_LINE_LOOP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINE_LOOP));
  CHECK(GL_LINE_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINE_STRIP));
  CHECK(GL_LINES == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::LINES));
  CHECK(GL_TRIANGLES == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLES));
  CHECK(GL_TRIANGLE_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLE_STRIP));
  CHECK(GL_TRIANGLE_FAN == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::TRIANGLE_FAN));
  CHECK(GL_QUADS == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::QUADS));
  CHECK(GL_QUAD_STRIP == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::QUAD_STRIP));
  CHECK(GL_POLYGON == OpenGLUtilities::asGlPrimitiveType(PrimitiveType::POLYGON));

  //GL_POINTS,GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON

  CHECK(GL_DEPTH_TEST == OpenGLUtilities::asGlAttribute(VideoAttribute::DEPTH_TEST));
  CHECK(GL_CULL_FACE == OpenGLUtilities::asGlAttribute(VideoAttribute::CULL_FACE));
  CHECK(GL_NONE == OpenGLUtilities::asGlAttribute(VideoAttribute::NONE));
  CHECK(GL_FRONT == OpenGLUtilities::asGlAttribute(VideoAttribute::FRONT));
  CHECK(GL_BACK == OpenGLUtilities::asGlAttribute(VideoAttribute::BACK));
  CHECK(GL_FRONT_AND_BACK == OpenGLUtilities::asGlAttribute(VideoAttribute::FRONT_AND_BACK));

  CHECK(GL_BLEND == OpenGLUtilities::asGlAttribute(VideoAttribute::BLEND));
  CHECK(GL_SRC_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_COLOR));
  CHECK(GL_ONE_MINUS_SRC_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_SRC_COLOR));
  CHECK(GL_SRC_ALPHA == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_ALPHA));
  CHECK(GL_ONE_MINUS_SRC_ALPHA == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_SRC_ALPHA));
  CHECK(GL_DST_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::DST_COLOR));
  CHECK(GL_ONE_MINUS_DST_COLOR == OpenGLUtilities::asGlAttribute(VideoAttribute::ONE_MINUS_DST_COLOR));
  CHECK(GL_SRC_ALPHA_SATURATE == OpenGLUtilities::asGlAttribute(VideoAttribute::SRC_ALPHA_SATURATE));


  CHECK(GL_TEXTURE_2D == OpenGLUtilities::asGlAttribute(VideoAttribute::TEXTURE_2D));
  CHECK(GL_TEXTURE_CUBE_MAP == OpenGLUtilities::asGlAttribute(VideoAttribute::TEXTURE_CUBE_MAP));
  CHECK(GL_MAX_TEXTURE_IMAGE_UNITS == OpenGLUtilities::asGlAttribute(VideoAttribute::MAX_TEXTURES));
  CHECK(GL_LINE_WIDTH == OpenGLUtilities::asGlAttribute(VideoAttribute::LINE_WIDTH));
}
