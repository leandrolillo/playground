#include <catch2/catch_test_macros.hpp>
#include "Resource.h"
#include "ResourceAdapter.h"
#include "ResourceManagerMock.h"
#include "ResourceAdapterMock.h"

TEST_CASE("Resource") {
  SECTION("Constructor with mimeType only defaults id to zero") {
    Resource resource("test/mimetype");
    CHECK(0 == resource.getId());
    CHECK("test/mimetype" == resource.getMimeType());
  }

  SECTION("Constructor with id and mimeType") {
    Resource resource(42, "test/mimetype");
    CHECK(42 == resource.getId());
    CHECK("test/mimetype" == resource.getMimeType());
  }

  SECTION("SetId and getId") {
    Resource resource("test/mimetype");
    resource.setId(99);
    CHECK(99 == resource.getId());
  }

  SECTION("SetUri and getUri") {
    Resource resource("test/mimetype");
    resource.setUri("/some/path");
    CHECK("/some/path" == resource.getUri());
  }

  SECTION("SetUri trims whitespace") {
    Resource resource("test/mimetype");
    resource.setUri("  /some/path  ");
    CHECK("/some/path" == resource.getUri());
  }

  SECTION("SetName and getName") {
    Resource resource("test/mimetype");
    resource.setName("objectName");
    CHECK("objectName" == resource.getName());
  }

  SECTION("SetName trims whitespace") {
    Resource resource("test/mimetype");
    resource.setName("  objectName  ");
    CHECK("objectName" == resource.getName());
  }

  SECTION("SetMimeType and getMimeType") {
    Resource resource("original/mimetype");
    resource.setMimeType("updated/mimetype");
    CHECK("updated/mimetype" == resource.getMimeType());
  }

  SECTION("SetMimeType trims whitespace") {
    Resource resource("original/mimetype");
    resource.setMimeType("  updated/mimetype  ");
    CHECK("updated/mimetype" == resource.getMimeType());
  }

  SECTION("GetFqdn without name returns uri") {
    Resource resource("test/mimetype");
    resource.setUri("/some/path/file.json");
    CHECK("/some/path/file.json" == resource.getFqdn());
  }

  SECTION("GetFqdn with name appends name to uri") {
    Resource resource("test/mimetype");
    resource.setUri("/some/path/file.json");
    resource.setName("myObject");
    CHECK("/some/path/file.json/myObject" == resource.getFqdn());
  }

  SECTION("GetFqdn with empty uri and name returns name") {
    Resource resource("test/mimetype");
    resource.setName("myObject");
    CHECK("myObject" == resource.getFqdn());
  }

  SECTION("AddLabel adds to labels set") {
    Resource resource("test/mimetype");
    resource.addLabel("ephemeral");
    CHECK(1 == resource.getLabels().size());
    CHECK(resource.getLabels().count("ephemeral") == 1);
  }

  SECTION("AddLabel trims whitespace") {
    Resource resource("test/mimetype");
    resource.addLabel("  ephemeral  ");
    CHECK(1 == resource.getLabels().size());
    CHECK(resource.getLabels().count("ephemeral") == 1);
  }

  SECTION("AddLabel ignores duplicate labels") {
    Resource resource("test/mimetype");
    resource.addLabel("ephemeral");
    resource.addLabel("ephemeral");
    CHECK(1 == resource.getLabels().size());
  }

  SECTION("AddLabel accumulates multiple labels") {
    Resource resource("test/mimetype");
    resource.addLabel("label1");
    resource.addLabel("label2");
    resource.addLabel("label3");
    CHECK(3 == resource.getLabels().size());
  }

  SECTION("SetLabels replaces existing labels") {
    Resource resource("test/mimetype");
    resource.addLabel("old-label");
    resource.setLabels({"new-label1", "new-label2"});
    CHECK(2 == resource.getLabels().size());
    CHECK(resource.getLabels().count("new-label1") == 1);
    CHECK(resource.getLabels().count("new-label2") == 1);
    CHECK(resource.getLabels().count("old-label") == 0);
  }

  SECTION("SupportsCaching returns true by default") {
    Resource resource("test/mimetype");
    CHECK(resource.supportsCaching());
  }

  SECTION("ToString contains id, mimeType and fqdn") {
    Resource resource(7, "test/mimetype");
    resource.setUri("/some/path");
    resource.setName("obj");
    String str = resource.toString();
    CHECK(str.find("7") != String::npos);
    CHECK(str.find("test/mimetype") != String::npos);
    CHECK(str.find("/some/path/obj") != String::npos);
  }
}

TEST_CASE("ResourceAdapter") {
  ResourceManagerMock resourceManager("resources");

  SECTION("IsValid is false when no output mimetypes are set") {
    ResourceAdapterMock adapter(resourceManager, {}, "test/inputMimeType");
    CHECK(!adapter.isValid());
    CHECK(!adapter.errors().empty());
  }

  SECTION("IsValid is true when at least one output mimetype is set") {
    ResourceAdapterMock adapter(resourceManager, {"test/outputMimeType"}, "test/inputMimeType");
    CHECK(adapter.isValid());
    CHECK(adapter.errors().empty());
  }

  SECTION("Accepts sets input mimetype") {
    ResourceAdapterMock adapter(resourceManager, {"test/outputMimeType"}, "");
    adapter.accepts("text/plain");
    CHECK("text/plain" == adapter.getInputMimeType());
  }

  SECTION("Produces adds output mimetypes") {
    ResourceAdapterMock adapter(resourceManager, {}, "test/inputMimeType");
    adapter.produces("test/outputMimeType");
    adapter.produces("test/anotherOutputMimeType");
    CHECK(2 == adapter.getOutputMimeTypes().size());
    CHECK(adapter.getOutputMimeTypes().count("test/outputMimeType") == 1);
    CHECK(adapter.getOutputMimeTypes().count("test/anotherOutputMimeType") == 1);
  }

  SECTION("ToString contains output mimetypes and input mimetype") {
    ResourceAdapterMock adapter(resourceManager, {"test/outputMimeType"}, "test/inputMimeType");
    String str = adapter.toString();
    CHECK(str.find("test/outputMimeType") != String::npos);
    CHECK(str.find("test/inputMimeType") != String::npos);
  }

  SECTION("ToString marks invalid adapter") {
    ResourceAdapterMock adapter(resourceManager, {}, "test/inputMimeType");
    String str = adapter.toString();
    CHECK(str.find("x ") != String::npos);
  }

  SECTION("ToString marks valid adapter") {
    ResourceAdapterMock adapter(resourceManager, {"test/outputMimeType"}, "test/inputMimeType");
    String str = adapter.toString();
    CHECK(str.find("✓") != String::npos);
  }

  SECTION("ToString uses wildcard when no input mimetype") {
    ResourceAdapterMock adapter(resourceManager, {"test/outputMimeType"}, "");
    String str = adapter.toString();
    CHECK(str.find("*") != String::npos);
  }
}

TEST_CASE("ResourceLoadRequest options") {
  ResourceManagerMock resourceManager("resources");

  SECTION("WithOption and getOption") {
    auto request = resourceManager.newRequest("fileToParse.txt")
        .acceptMimeType("text/plain")
        .withOption("key1", "value1");
    CHECK("value1" == request.getOption("key1"));
  }

  SECTION("GetOption returns empty string for missing key") {
    auto request = resourceManager.newRequest("fileToParse.txt")
        .acceptMimeType("text/plain");
    CHECK("" == request.getOption("missingKey"));
  }

  SECTION("WithOptions sets multiple options") {
    auto request = resourceManager.newRequest("fileToParse.txt")
        .acceptMimeType("text/plain")
        .withOptions({{"key1", "value1"}, {"key2", "value2"}});
    CHECK("value1" == request.getOption("key1"));
    CHECK("value2" == request.getOption("key2"));
  }

  SECTION("WithOption overwrites previous value for same key") {
    auto request = resourceManager.newRequest("fileToParse.txt")
        .acceptMimeType("text/plain")
        .withOption("key1", "original")
        .withOption("key1", "updated");
    CHECK("updated" == request.getOption("key1"));
  }
}

TEST_CASE("ResourceLoadRequest errors") {
  ResourceManagerMock resourceManager("resources");

  SECTION("Errors contains uri required when uri is empty") {
    auto request = resourceManager.newRequest("").acceptMimeType("test/mimetype").withInputMimeType("text/plain");
    CHECK(!request.isValid());
    CHECK(request.errors().find("uri is required") != String::npos);
  }

  SECTION("Errors contains output mimetype required when not set") {
    auto request = resourceManager.newRequest("fileToParse.txt").withInputMimeType("text/plain");
    CHECK(!request.isValid());
    CHECK(request.errors().find("Output mimetype is required") != String::npos);
  }

  SECTION("Errors contains input mimetype required when not guessable") {
    auto request = resourceManager.newRequest("fileWithNoExtension").acceptMimeType("test/mimetype");
    CHECK(!request.isValid());
    CHECK(request.errors().find("Could not determine input mimeType") != String::npos);
  }

  SECTION("Valid request has no errors") {
    auto request = resourceManager.newRequest("fileToParse.txt").acceptMimeType("text/plain");
    CHECK(request.isValid());
    CHECK(request.errors().empty());
  }
}
