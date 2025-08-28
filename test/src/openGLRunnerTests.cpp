#include <catch2/catch_test_macros.hpp>

#include "OpenGLRunner.h"

TEST_CASE("OpenGLRunner Test case")
{
	OpenGLRunner runner;

	REQUIRE(1 == 1);

}

TEST_CASE("TextureResourceAdapter") {
  TextureResource *resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.png", MimeTypes::TEXTURE);

  CHECK(resource != null);
  CHECK(resource->getId() != 0);
  CHECK(MimeTypes::TEXTURE == resource->getMimeType());

  resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.jpg", MimeTypes::TEXTURE);

  CHECK(resource != null);
  CHECK(resource->getId() != 0);
  CHECK(MimeTypes::TEXTURE == resource->getMimeType());

  resource = (TextureResource *)this->getContainer()->getResourceManager()->load("tests/image.tga", MimeTypes::TEXTURE);

  CHECK(resource != null);
  CHECK(resource->getId() != 0);
  CHECK(MimeTypes::TEXTURE == resource->getMimeType());
}

TEST_CASE("LoadVertexBuffer") {
  VertexArrayResource *resource = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("tests/geometry.json", MimeTypes::VERTEXARRAY);

  CHECK(resource != null);
  CHECK(MimeTypes::VERTEXARRAY == resource->getMimeType());
}

TEST_CASE("LoadShaders")  {
  ShaderResource *resource = (ShaderResource *)this->getContainer()->getResourceManager()->load("tests/vertexShader.glsl", MimeTypes::VERTEXSHADER);

  CHECK(resource != null);
  CHECK(MimeTypes::VERTEXSHADER == resource->getMimeType());
}

TEST_CASE("LoadShaderProgram") {
  ShaderProgramResource *resource = (ShaderProgramResource *)this->getContainer()->getResourceManager()->load("tests/shaderProgram.json", MimeTypes::SHADERPROGRAM);

  CHECK(resource != null);
  CHECK(MimeTypes::SHADERPROGRAM == resource->getMimeType());
}
