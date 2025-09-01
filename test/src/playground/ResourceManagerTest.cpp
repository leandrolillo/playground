#include <catch2/catch_test_macros.hpp>
#include "ResourceManagerMock.h"
#include "ResourceAdapterMock.h"

TEST_CASE("ResourceLoadRequest tests") {
  ResourceManagerMock resourceManager("resources");

  SECTION("ToString") {
    String actual = resourceManager.newRequest("/test/filename.json").acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).toString();
    CHECK("[test/mimetype]<-[application/json] [/test/filename.json]" == actual);
  }

  SECTION("IsValid") {
    bool actual = resourceManager.newRequest("fileToParse.txt").acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).isValid();
    CHECK(actual);

//    /** Should be able to guess mimetype from filename even with object name*/
//    actual = resourceManager.newRequest("fileToParse.txt/objectName").withParent(resourceManager.getRootFolder()).acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).isValid();
//    CHECK(actual);

    actual = resourceManager.newRequest("").isValid();
    CHECK(!actual);

    actual = resourceManager.newRequest("filename").acceptMimeType("test/mimetype").isValid();
    CHECK(!actual); //expect could not guess input mimetype
  }

  SECTION("FilePath") {
    /* Just filename*/
    String actual = resourceManager.newRequest("/tests/filename.json").getFilePath();
    CHECK("tests/filename.json" == actual);

//    /* Uri with name - relative to root*/
//    actual = resourceManager.newRequest("fileToParse.txt/name").withParent(resourceManager.getRootFolder()).getFilePath();
//    CHECK(Paths::add(resourceManager.getRootFolder(), "fileToParse.txt") == actual);
//
//    /* Uri with name - and parent - returns the object as well since filename does not exist */
//    actual = resourceManager.newRequest("filename.json/name").withParent("/home").withParent("leandro").getFilePath();
//    CHECK( resourceManager.getRootFolder() + "/home/leandro/filename.json/name" == actual);
  }

  SECTION("SimpleUri") {
//    Resource parent(0, "test/mimetype");
//    parent.setFileName("/tests/filename.json");
//
//    ResourceLoadRequest request("child/filename.json");
//    //test fileParser
//
//
//
//    ResourceLoadRequest requestWithParent("/test/filename.json");
//    requestWithParent.withParent(&parent);
//    request.getFilePath() == "/tests/child/filename.json";
//    //test fileParser
//
//    ResourceLoadRequest requestFromFileParser(fileparser);
//    requestWithParent.withParent(&parent);
//    request.getFilePath() == "/tests/child/filename.json";
//    //test fileParser
  }
}

TEST_CASE("ResourceLoadResponse tests")
{
  SECTION("FilePath") {
//    ResourceManagerMock resourceManager("resources");
//    String actual = resourceManager.getFullPath(resourceManager.newRequest("basketball.json").getUri(), "~/images/basketball.png");
//    CHECK((String)std::__fs::filesystem::absolute("resources") + "/images/basketball.png" == actual);

  }

  SECTION("issueWithRootPathsBeingRelative") {
    String rootFolder("./target/../../media");
    ResourceManagerMock resourceManager(rootFolder);

//    String actual = resourceManager.getFullPath("geometry/basketball.json", "~/images/basketball.png");
//    actual = Paths::normalize(actual, rootFolder);
//    CHECK((String)std::__fs::filesystem::absolute(rootFolder) + "/images/basketball.png" == actual);
//    //CHECK(rootFolder + "/images/basketball.png" == actual); // this is the failure before the fix in resource manager constructor.
  }
}

TEST_CASE("ResourceManagerTests") {
  ResourceManagerMock resourceManager("resources");

  SECTION("AddResourceAdapter") {
    ResourceAdapter *resourceAdapter = resourceManager.addAdapter(
        std::make_unique<ResourceAdapterMock>(std::set<String> {"test/outputMimeType"}, "test/inputMimeType"));
    REQUIRE(resourceAdapter != null);
    CHECK(1 == resourceManager.getAdaptersCount());
  }

//  SECTION("GetResourceAdapter") {
//    CHECK(false); //"Implement getResourceAdapter tests for requests and resources"
//  }

  SECTION("Load") {
    ResourceAdapterMock *resourceAdapter = (ResourceAdapterMock *)resourceManager.addAdapter(
        std::make_unique<ResourceAdapterMock>(std::set<String> {"test/outputMimeType", "test/anotherOutputMimeType"}, "test/inputMimeType"));

//    /* Should populate default values */
//    resourceAdapter->withLoadResult(null);
//    resourceAdapter->withLoadResult(new Resource(1, ""));
//    resourceAdapter->withLoadResult(null);
//    Resource *actual = resourceManager.load(resourceManager.newRequest("/test/filename").withParent(resourceManager.getRootFolder()).acceptMimeType("test/outputMimeType").withInputMimeType("test/inputMimeType").withLabels(std::set<String> {"test"}));
//    REQUIRE(actual != null);
//    CHECK("test/outputMimeType" == actual->getMimeType());
//    CHECK("/test/filename" == actual->getUri());
//    CHECK(1 == actual->getLabels().size());
//    CHECK(1 == resourceManager.getResourcesCount());
//
//    /* unless already set */
//    Resource *mockResult = new Resource(1, "test/outputMimeType");
//    mockResult->setUri("/test/anotherFilename");
//    resourceAdapter->withLoadResult(mockResult);
//    actual = resourceManager.load(resourceManager.newRequest("/test/anotherFilename").withParent(resourceManager.getRootFolder()).acceptMimeType("test/outputMimeType").withInputMimeType("test/inputMimeType"));
//    REQUIRE(actual != null);
//    CHECK("test/outputMimeType" == actual->getMimeType());
//    CHECK("/test/anotherFilename" == actual->getUri());
//    CHECK(0 == actual->getLabels().size());
//    CHECK(2 == resourceManager.getResourcesCount());
//
//    /* Make sure we get the proper mimetype or null*/
//    actual = resourceManager.load(resourceManager.newRequest("test/filename").withParent(resourceManager.getRootFolder()).acceptMimeType("test/anotherOutputMimeType").withInputMimeType("test/inputMimeType"));
//    REQUIRE(actual == null);
//    CHECK(2 ==resourceManager.getResourcesCount());

  }

  SECTION ("AddResource") {
    Resource *resource = new Resource(1, "test/mimetype");
    resource->setUri("/test/filename");
    resourceManager.addResource(resource);
    resource->toString(); //any better way to try to trigger illegal access?
    CHECK(1 == resourceManager.getResourcesCount());

    resource = new Resource(2, "test/mimetype");
    resource->setUri("/test/filename2");
    resourceManager.addResource(resource);
    resource->toString();//any better way to try to trigger illegal access?
    CHECK(2 == resourceManager.getResourcesCount());

    resource = resourceManager.load("/test/filename", "test/mimetype");
    REQUIRE(resource != null);
    CHECK("/test/filename" == resource->getUri());
    CHECK("test/mimetype" == resource->getMimeType());

    /**
     * Add a duplicated resource to check nothing gets deleted
     */
    resourceManager.addResource(resource);
    resource->toString(); //any better way to try to trigger illegal access?
    CHECK(2 == resourceManager.getResourcesCount());
  }

  SECTION("Unload") {
    Resource *resource = new Resource(1, "test/mimetype");
    resource->setUri("test/filename1");
    resource->addLabel("ephemeral");
    resourceManager.addResource(resource);

    resource = new Resource(2, "test/mimetype");
    resource->setUri("test/filename2");
    resource->addLabel("not ephemeral");
    resource->addLabel("level 1");
    resourceManager.addResource(resource);

    resource = new Resource(3, "test/mimetype");
    resource->setUri("test/filename3");
    resource->addLabel("permanent");
    resourceManager.addResource(resource);

    resource = new Resource(4, "test/mimetype");
    resource->setUri("test/filename4");
    resourceManager.addResource(resource);

    //    /*
    //     * This one should evict the previous one from cache and keep resources count to 4
    //     */
    //    resource = new Resource(4, "test/mimetype");
    //    resource->setFileName("test/filename4");
    //    resourceManager.addResource(resource);

    CHECK(4 == resourceManager.getResourcesCount());

    /*
     * Test unload by resource
     */

    resourceManager.unload(resource);
    CHECK(3 == resourceManager.getResourcesCount());

    /*
     * Test unload by single label
     */
    resourceManager.unload("ephemeral");
    CHECK(2 == resourceManager.getResourcesCount());

    /*
     * Test unload by multiple labels
     */
    resourceManager.unload(std::set<String> {"not ephemeral", "level 1"});
    CHECK(1 == resourceManager.getResourcesCount());

  }

  SECTION("InvalidResource")
  {
    Resource *resource = resourceManager.load("tests/fake.wav", MimeTypes::AUDIO);
    CHECK(null == resource);

    resource = resourceManager.load("tests/fake.nomimetype", MimeTypes::AUDIO);
    CHECK(null == resource);

  }

  SECTION("FileParser")
  {
    String token;

    FileParser commentFileParser(Paths::add(resourceManager.getRootFolder(), "commentFileToParse.txt"));
    CHECK(FileParser::eof == commentFileParser.peekToken());
    CHECK(FileParser::eof == commentFileParser.takeToken());
    commentFileParser.close();

    FileParser emptyFileParser(Paths::add(resourceManager.getRootFolder(), "emptyFileToParse.txt"));
    CHECK(FileParser::eof == emptyFileParser.peekToken());
    CHECK(FileParser::eof == emptyFileParser.takeToken());
    commentFileParser.close();

    FileParser fileParser(Paths::add(resourceManager.getRootFolder(), "fileToParse.txt"));
    CHECK("{" == fileParser.peekToken());
    CHECK("{" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("property" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK(":" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("value" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("}" == fileParser.takeToken());
    CHECK(FileParser::eof == fileParser.peekToken());
    CHECK(FileParser::eof == fileParser.takeToken());
  }
}

TEST_CASE("ResourceAdapterTest") {
  ResourceManagerMock resourceManager("resources");

  ResourceAdapterMock *resourceAdapter = (ResourceAdapterMock *)resourceManager.addAdapter(
      std::make_unique<ResourceAdapterMock>(std::set<String> {"test/outputMimeType", "test/anotherOutputMimeType"}, "test/inputMimeType"));

  resourceAdapter->withLoadResult(new Resource(1, "test/outputMimeType"));
  resourceAdapter->withLoadResult(null);
  resourceAdapter->withLoadResult(new Resource(2, "test/outputMimeType"));


  auto response = resourceAdapter->load(resourceManager.newRequest("basketball.json"));

  CHECK(response.size() == 2);
  for(auto item : response) {
    CHECK(item != null);
  }
}
