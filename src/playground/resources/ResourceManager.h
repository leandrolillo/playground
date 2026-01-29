#pragma once
#include <set>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <filesystem>
#include "Logger.h"
#include "FileParser.h"
#include "Paths.h"
#include "Resource.h"
#include "ResourceAdapter.h"

/**
 * Comparator to enable sets of unique_ptr<ResourceAdapter> by comparing the raw pointers
 */
struct resourceAdapterComparator
{
  using is_transparent = void;
  bool operator()(std::unique_ptr<ResourceAdapter> const &left, std::unique_ptr<ResourceAdapter> const &right) const
      {
    return left.get() < right.get();
  }
  bool operator()(ResourceAdapter *left, std::unique_ptr<ResourceAdapter> const &right) const
      {
    return left < right.get();
  }
  bool operator()(std::unique_ptr<ResourceAdapter>const &left, ResourceAdapter *right) const
      {
    return left.get() < right;
  }
};

/**
 * Comparator to enable sets of unique_ptr<Resource> by comparing the raw pointers
 */
struct resourceComparator
{
  using is_transparent = void;
  bool operator()(std::unique_ptr<Resource> const &left, std::unique_ptr<Resource> const &right) const
      {
    return left.get() < right.get();
  }
  bool operator()(Resource *left, std::unique_ptr<Resource> const &right) const
      {
    return left < right.get();
  }
  bool operator()(std::unique_ptr<Resource>const &left, Resource *right) const
      {
    return left.get() < right;
  }
};

class ResourceManager {
public:
  static const String EphemeralLabel;

protected:
  Logger *logger = LoggerFactory::getLogger("resources/ResourceManager");

  /**
   * Define adapters before resources so that they are initialized before them, and deleted after them.
   */
  std::set<std::unique_ptr<ResourceAdapter>, resourceAdapterComparator> resourceAdapters;
  std::set<std::unique_ptr<Resource>, resourceComparator> resources;

  std::map<String, Resource*> resourcesCache;
  std::map<String, ResourceAdapter*> adaptersCache;

  String rootFolder;

public:
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  ResourceManager(const String &rootFolder) {
    //logger->setLogLevel(LogLevel::DEBUG);
    this->rootFolder = std::filesystem::absolute(rootFolder); //for now it has to be an absolute path or there will be issues with resource loading
  }

  /*
   * Rule of five and copy-and-swap
   */
  friend void swap(ResourceManager &left, ResourceManager &right) {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;

    // by swapping the members of two objects, the two objects are effectively swapped
    swap(left.resourceAdapters, right.resourceAdapters);
    swap(left.resources, right.resources);
    swap(left.resourcesCache, right.resourcesCache);
    swap(left.rootFolder, right.rootFolder);
  }

  ResourceManager(ResourceManager &&rvalue) {
    swap(*this, rvalue);
  }

  /*
   * End of Rule of five and copy-and-swap
   */

  /*****************
   * Adapter methods
   *****************/
  template<typename Adapter, typename... Args>
  Adapter &addAdapter(Args&&... args) {
    logger->debug("Adding adapter");

    static_assert(std::is_base_of<ResourceAdapter, Adapter>::value, "Adapter type parameter in addAdapter must be derived from ResourceAdapter");
    auto adapterUniquePtr { std::make_unique<Adapter>(*this, std::forward<Args>(args)...)};

    if (adapterUniquePtr) {
      if (adapterUniquePtr->isValid()) {
        logger->debug("Adapter [%s] is valid - proceeding to add it", adapterUniquePtr->toString().c_str());

        //TODO: review this logic since it should compare classes or mimetypes instead of pointers
        Adapter *adapter = adapterUniquePtr.get();
        if (resourceAdapters.find(adapter) == resourceAdapters.end()) {
          /* std::move makes this an l-value so it behaves like it is a temporary variable - it makes adapterUniquePtr variable point to null so it should not be referenced after this. */
          resourceAdapters.insert(std::move(adapterUniquePtr));

          logger->debug("Adapter [%s] added to set", adapter->toString().c_str());

          for (auto &outputMimeType : adapter->getOutputMimeTypes()) {
            String key = adapter->getInputMimeType().empty() ? outputMimeType + "|" : outputMimeType + "|" + adapter->getInputMimeType();
            adaptersCache[key] = adapter;

            logger->debug("Adapter [%s] added to manage [%s] with key [%s]", adapter->toString().c_str(), outputMimeType.c_str(),
                key.c_str());
          }
        } else {
          logger->warn("Skipping adapter [%s] - already managed", adapter->toString().c_str());

          //TODO: should return previous adapter?
        }

        return *adapter;
      } else {
        String errorMessage = StringFormatter::format("NOT adding invalid adapter [%s]: [%s] ",
            adapterUniquePtr->toString().c_str(),
            adapterUniquePtr->errors().c_str());
        logger->error(errorMessage);
        throw std::invalid_argument(errorMessage);
      }
    } else {
      logger->error("Could not instantiate adapter");
      throw std::invalid_argument("Could not instantiate adapter");
    }
  }

  /*************************************
   * ResourceLoadRequest factory methods
   *************************************/

  ResourceLoadRequest newRequest(std::shared_ptr<FileParser> &fileParser) {
    return ResourceLoadRequest(fileParser);
  }

  ResourceLoadRequest newRequest(const String &fileName) {
    return ResourceLoadRequest(fileName).withRootFolder(rootFolder);
  }

  /*************************************
   * Load methods
   *************************************/

  Resource* load(const String &fileName) {
    logger->debug("Load [%s]", fileName.c_str());
    return this->load(newRequest(fileName));
  }

  Resource* load(std::shared_ptr<FileParser> &fileParser, const String &outputMimeType, std::set<String> labels = { }) {
    logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileParser.get()->getFilename().c_str());

    return load(newRequest(fileParser).acceptMimeType(outputMimeType).withLabels(labels));
  }

  Resource* load(const String &fileName, const String &outputMimeType, std::set<String> labels = { },
      std::map<String, String> options = { }) {
    logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileName.c_str(), fileName.c_str());

    return load(newRequest(fileName).acceptMimeType(outputMimeType).withLabels(labels).withOptions(options));
  }

  Resource* load(ResourceLoadRequest &resourceLoadRequest);
  Resource* load(ResourceLoadRequest &&rvalue) {
    ResourceLoadRequest resourceLoadRequest(rvalue);
    return load(resourceLoadRequest);
  }

  /*****************
   * Resource methods
   *****************/
  Resource* addResource(Resource *resource) {
    if (resources.find(resource) == resources.end()) {
      resources.insert(std::unique_ptr<Resource>(resource));
    }

    if (resource == null || resource->getFqdn().empty() || resource->getMimeType().empty()) { //Todo: review this validation and getKey exceptions
      logger->error("ResourceManager: Could not add [%s] - uri and mimeType are required", resource->toString().c_str());
    } else {
      String key = getCacheKey(*resource);
      if (!key.empty()) {
        resourcesCache[key] = resource;
      }
    }

    return resource;
  }

  std::set<std::unique_ptr<Resource>>::iterator unload(Resource *resource) {
    if (resource != null) {
      auto iterator = std::find_if(
          resources.begin(), resources.end(),
          [&resource](const std::unique_ptr<Resource> &resourceUniquePtr) {
            return resourceUniquePtr.get() == resource;
          });

      if (iterator != resources.end()) {
        this->dispose(resource);
        this->resourcesCache.erase(getCacheKey(*resource));
        return resources.erase(iterator);
      }
    }

    return resources.end();
  }

  void unload(String label) {
    auto iterator = std::find_if(
        resources.begin(), resources.end(),
        [&label](const std::unique_ptr<Resource> &resourceUniquePtr) {
          return resourceUniquePtr.get() != null && resourceUniquePtr->getLabels().find(label) != resourceUniquePtr->getLabels().end();
        });

    if (iterator != resources.end()) {
      this->dispose(iterator->get());
      this->resourcesCache.erase(getCacheKey(*iterator->get()));
      resources.erase(iterator);
    }
  }

  void unload(std::set<String> labels) {
    std::set<std::unique_ptr<Resource>>::iterator iterator = resources.begin();

    while (iterator != resources.end()) {
      bool hasAllLabels = true;
      if (*iterator) {
        for (auto &label : labels) {
          const std::set<String> labels = iterator->get()->getLabels();
          if (labels.find(label) == labels.end()) {
            hasAllLabels = false;
            break;
          }
        }
      }

      if (hasAllLabels) {
        iterator = this->unload(iterator->get());
      } else {
        ++iterator;
      }
    }
  }

  void dispose(Resource *resource) const;
  ~ResourceManager();

public:
  const String& getRootFolder() {
    return this->rootFolder;
  }

  void logStatus() {
    logger->debug("Resource Manager status:");
    logger->debug("Resources %d", resourcesCache.size());
    logger->debug("Resource adapters: %d", resourceAdapters.size());
  }

  String toString() const {
    return "ResourceManager[" + this->rootFolder + "] / [ " + std::to_string(resourceAdapters.size()) + "] adapters";
  }


private:
  /**
   * Find Adapter suitable for handling the request
   */
  ResourceAdapter* getAdapter(ResourceLoadRequest &request) const {
    /**
     * Look for adapter matching output mimetype and empty input mimetype first - kind of a input mimetype wildcard
     */
    auto adaptersPair = adaptersCache.find(request.getOutputMimeType() + "|");
    if (adaptersPair != adaptersCache.end()) {
      return adaptersPair->second;
    }

    /**
     * Then look for a precise match of output and input mimetype
     */
    adaptersPair = adaptersCache.find(request.getOutputMimeType() + "|" + request.getInputMimeType());
    return adaptersPair == adaptersCache.end() ? null : adaptersPair->second;
  }

  /**
   * Find Adapter suitable for handling the resource - currently used for dispose logic
   */

  ResourceAdapter* getAdapter(const Resource *resource) const {
    auto adaptersPair = adaptersCache.end();

    if (resource != null && !resource->getMimeType().empty()) {
      auto adaptersPair = adaptersCache.lower_bound(resource->getMimeType() + "|");
      if (adaptersPair != adaptersCache.end()) {
        return adaptersPair->second;
      }

      /* this was the original code - leaving it here just in case but I think it has no effect */
      adaptersPair = adaptersCache.find(resource->getMimeType() + "|");
      if (adaptersPair != adaptersCache.end()) {
        return adaptersPair->second;
      }
    }

    return null;
  }

  const String getCacheKey(const String &filename, const String &mimeType) const {
    if (filename.empty() || mimeType.empty()) {
      throw std::invalid_argument("Can not get cache key from empty values - filename: [" + filename + "] mimeType[ " + mimeType + "]");
    }

    return filename + "|" + mimeType;
  }

  const String getCacheKey(const Resource &resource) {
    return getCacheKey(resource.getFqdn(), resource.getMimeType());
  }

  const String getCacheKey(const ResourceLoadRequest &resourceLoadRequest) {
    return getCacheKey(resourceLoadRequest.getFqdn(), resourceLoadRequest.getOutputMimeType());
  }


  Resource* getCacheResource(const String &cacheKey) {
    auto pair = resourcesCache.find(cacheKey);

    if (pair != resourcesCache.end()) {
      return pair->second;
    }

    return null;
  }
};
