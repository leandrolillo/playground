#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "mathMatchers.h"

#include "ImageResource.h"
#include "HeightMapResource.h"
#include "TextureResource.h"
#include "CubeMapResource.h"
#include "ShaderResource.h"
#include "ShaderProgramResource.h"
#include "MaterialResource.h"
#include "LightResource.h"
#include "TerrainResource.h"
#include "GeometryResource.h"
#include "MeshResource.h"
#include "VertexArrayResource.h"
#include "FontResource.h"

TEST_CASE("Image Resource test case")
{
  LoggerFactory::doNotLogToFile();
  LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

  ImageResource resource;
  CHECK(resource.getMimeType() == MimeTypes::IMAGE);
  resource.resize(200, 100, 16);

  CHECK(resource.getHeight() == 100);
  CHECK(resource.getWidth() == 200);
  CHECK(resource.getBpp() == 16);
  //CHECK(resource.getBytespp() == 2);
  CHECK(resource.getBufferSize() == 100 * 200 * 2);

  resource.resize(300, 200, 8);

  CHECK(resource.getHeight() == 200);
  CHECK(resource.getWidth() == 300);
  CHECK(resource.getBpp() == 8);
  //CHECK(resource.getBytespp() == 1);
  CHECK(resource.getBufferSize() == 200 * 300 * 1);

  /**
   * Test copy
   */
  //make sure image is clear
  std::memset(resource.getData(), 0, resource.getBufferSize());
  CHECK(resource.getData()[resource.getOffset(0, 0)] == 0);
  CHECK(resource.getData()[resource.getOffset(99, 99)] == 0);
  CHECK(resource.getData()[resource.getOffset(250, 150)] == 0);
  CHECK(resource.getData()[resource.getOffset(349, 249)] == 0);

  //copy image that fits inside and check contents
  ImageResource anotherResource(100, 100, 8);
  std::memset(anotherResource.getData(), 3, anotherResource.getBufferSize());
  resource.copy(0, 0, anotherResource.getData(), anotherResource.getWidth(), anotherResource.getHeight(), anotherResource.getBpp());
  CHECK(resource.getData()[resource.getOffset(0, 0)] == 3);
  CHECK(resource.getData()[resource.getOffset(99, 99)] == 3);
  CHECK(resource.getData()[resource.getOffset(100, 100)] == 0);
  CHECK(resource.getWidth() == 300);
  CHECK(resource.getHeight() == 200);


  //copy over the boundaries
  resource.copy(250, 150, anotherResource.getData(), anotherResource.getWidth(), anotherResource.getHeight(), anotherResource.getBpp());
  CHECK(resource.getData()[resource.getOffset(250, 150)] == 3);
  CHECK(resource.getData()[resource.getOffset(299, 199)] == 3);
  CHECK(resource.getWidth() == 300);
  CHECK(resource.getHeight() == 200);
}

TEST_CASE("HeightMapResource::normalAt returns a flat normal at right edge for a flat map")
{
  ImageResource image(3, 3, 24);
  std::memset(image.getData(), 255, image.getBufferSize());

  HeightMapResource heightMap(&image, vector(1, 1, 1));
  vector actual = heightMap.normalAt(heightMap.getWidth(), 1.0);

  CHECK_THAT(actual, EqualsVector(vector3(0, -1, 0)));
}

TEST_CASE("Video resource simple classes expose expected defaults and properties")
{
  TextureResource texture(10);
  CubeMapResource cubeMap(11);
  ShaderResource shader(12);
  ShaderProgramResource shaderProgram(13);

  CHECK(texture.getId() == 10);
  CHECK(texture.getMimeType() == MimeTypes::TEXTURE);
  CHECK(cubeMap.getMimeType() == MimeTypes::CUBEMAP);
  CHECK(shader.getMimeType() == "");
  CHECK(shaderProgram.getMimeType() == MimeTypes::SHADERPROGRAM);

  shaderProgram.setShaders({ &shader });
  REQUIRE(shaderProgram.getShaders().size() == 1);
  CHECK(shaderProgram.getShaders()[0] == &shader);
}

TEST_CASE("Material and light resources keep assigned values")
{
  MaterialResource material("mat", vector3(0.1, 0.2, 0.3), vector3(0.4, 0.5, 0.6), vector3(0.7, 0.8, 0.9), 16.0, 0.5);

  material.setEmissive(vector3(0.9, 0.8, 0.7));
  material.setAmbientTexture("ambient.png");
  material.setDiffuseTexture("diffuse.png");
  material.setSpecularTexture("specular.png");
  material.setAlphaTexture("alpha.png");
  material.setBumpTexture("normal.png");

  CHECK(material.getName() == "mat");
  CHECK_THAT(material.getAmbient(), EqualsVector(vector3(0.1, 0.2, 0.3)));
  CHECK_THAT(material.getDiffuse(), EqualsVector(vector3(0.4, 0.5, 0.6)));
  CHECK_THAT(material.getSpecular(), EqualsVector(vector3(0.7, 0.8, 0.9)));
  CHECK(material.getShininess() == Catch::Approx(16.0));
  CHECK(material.getAlpha() == Catch::Approx(0.5));
  CHECK_THAT(material.getEmissive(), EqualsVector(vector3(0.9, 0.8, 0.7)));
  CHECK(material.getAmbientTexture() == "ambient.png");
  CHECK(material.getDiffuseTexture() == "diffuse.png");
  CHECK(material.getSpecularTexture() == "specular.png");
  CHECK(material.getAlphaTexture() == "alpha.png");
  CHECK(material.getBumpTexture() == "normal.png");

  LightResource light(vector3(1, 2, 3), vector3(0.1, 0.1, 0.1), vector3(0.2, 0.2, 0.2), vector3(0.3, 0.3, 0.3), 8);
  CHECK(light.getMimeType() == "video/light");
  CHECK_THAT(light.getPosition(), EqualsVector(vector3(1, 2, 3)));
  CHECK_THAT(light.getAmbient(), EqualsVector(vector3(0.1, 0.1, 0.1)));
}

TEST_CASE("Geometry and vertex array resources support data assignment")
{
  GeometryResource geometry;
  geometry.addVertex(vector3(0, 0, 0));
  geometry.addVertex(vector3(2, 3, 4));
  geometry.addNormal(vector3(0, 1, 0));
  geometry.addTextureCoordinate(vector2(1, 0));
  geometry.addColor(vector3(1, 0, 0));
  geometry.addIndex(0);
  geometry.addIndex(1);
  geometry.setType(PrimitiveType::LINES);
  geometry.setData({0, 1, 2, 3});
  geometry.setDataComponentsPerVertex(2);

  CHECK_THAT(geometry.getSize(), EqualsVector(vector3(2, 3, 4)));
  CHECK(geometry.getNormals().size() == 1);
  CHECK(geometry.getTextureCoordinates().size() == 1);
  CHECK(geometry.getColors().size() == 1);
  CHECK(geometry.getIndices().size() == 2);
  CHECK(geometry.getType() == PrimitiveType::LINES);
  CHECK(geometry.getData().size() == 4);
  CHECK(geometry.getDataComponentsPerVertex() == 2);

  VertexArrayResource vertexArray(20);
  vertexArray.setPrimitiveType(PrimitiveType::TRIANGLE_STRIP);
  vertexArray.setSize(vector3(4, 5, 6));
  vertexArray.addAttribute(0, 30, 0, 3, VideoAttribute::ARRAY_BUFFER, VideoAttribute::STATIC_DRAW);

  const VertexAttribPointer* attribute = vertexArray.getAttribute(0);
  REQUIRE(attribute != null);
  CHECK(attribute->getBuffer() == 30);
  CHECK(attribute->getCount() == 3);
  CHECK(attribute->getStart() == 0);
  CHECK(attribute->getBufferDestination() == VideoAttribute::ARRAY_BUFFER);
  CHECK(attribute->getBufferUsage() == VideoAttribute::STATIC_DRAW);
  CHECK(vertexArray.getPrimitiveType() == PrimitiveType::TRIANGLE_STRIP);
  CHECK_THAT(vertexArray.getSize(), EqualsVector(vector3(4, 5, 6)));

  vertexArray.clearVertexAttribPointers();
  CHECK(vertexArray.getAttributes().empty());
}

TEST_CASE("Mesh, terrain and font resources wire dependencies")
{
  TextureResource texture(1);
  TextureResource map(2);
  TextureResource red(3);
  TextureResource green(4);
  TextureResource blue(5);
  TextureResource alpha(6);
  VertexArrayResource model(7);

  ImageResource image(2, 2, 24);
  std::memset(image.getData(), 255, image.getBufferSize());
  HeightMapResource heightMap(&image, vector3(1, 2, 3));

  TerrainResource terrain;
  terrain.setMap(&map);
  terrain.setR(&red);
  terrain.setG(&green);
  terrain.setB(&blue);
  terrain.setA(&alpha);
  terrain.setModel(&model);
  terrain.setHeightmap(&heightMap);

  CHECK(terrain.getMap() == &map);
  CHECK(terrain.getR() == &red);
  CHECK(terrain.getG() == &green);
  CHECK(terrain.getB() == &blue);
  CHECK(terrain.getA() == &alpha);
  CHECK(terrain.getModel() == &model);
  CHECK(terrain.getHeightMap() == &heightMap);

  MaterialResource material(vector3(0.2, 0.2, 0.2), vector3(0.3, 0.3, 0.3), vector3(0.4, 0.4, 0.4), 4.0);
  MeshResource mesh;
  mesh.setVertexArray(&model);
  mesh.setMaterial(&material);
  mesh.setTexture(&texture);
  mesh.setSpecularMap(&map);
  mesh.setNormalMap(&alpha);

  CHECK(mesh.getVertexArray() == &model);
  CHECK(mesh.getMaterial() == &material);
  CHECK(mesh.getTexture() == &texture);
  CHECK(mesh.getSpecularMap() == &map);
  CHECK(mesh.getNormalMap() == &alpha);

  FontResource font;
  font.add('A', vector2(0.0, 0.0), vector2(0.2, 0.2), vector2(12, 16), vector2(1, 2), 9)
      ->setTextureAtlas(&texture);

  const Glyph& glyph = font.getGlyph('A');
  CHECK_THAT(glyph.getTextureTopLeft(), EqualsVector(vector2(0.0, 0.0)));
  CHECK_THAT(glyph.getTextureBottomRight(), EqualsVector(vector2(0.2, 0.2)));
  CHECK_THAT(glyph.getSize(), EqualsVector(vector2(12, 16)));
  CHECK_THAT(glyph.getOffset(), EqualsVector(vector2(1, 2)));
  CHECK(glyph.getAdvance() == Catch::Approx(9));
  CHECK(font.getTextureAtlas() == &texture);
}
