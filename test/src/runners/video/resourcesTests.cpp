#include <catch2/catch_test_macros.hpp>
#include "mathMatchers.h"

#include "ImageResource.h"

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

  //Test copy
  std::memset(resource.getData(), 0, resource.getBufferSize());
  CHECK(resource.getData()[resource.getOffset(0, 0)] == 0);
  CHECK(resource.getData()[resource.getOffset(99, 99)] == 0);
  CHECK(resource.getData()[resource.getOffset(250, 150)] == 0);
  CHECK(resource.getData()[resource.getOffset(349, 249)] == 0);

  //copy image that fits inside
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
  CHECK(resource.getData()[resource.getOffset(349, 249)] == 3);
  CHECK(resource.getWidth() == 300);
  CHECK(resource.getHeight() == 200);
}
