/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

const String ResourceManager::EphemeralLabel = { "ephemeral" };

Resource* ResourceManager::load(ResourceLoadRequest &resourceLoadRequest) {
  Resource *result = null;
  try {
    /**
     * Find the resource in the cache. If not there, then trigger loading via adapter.
     * Adapter loads as many objects as it wants and puts them in resource manager cache
     * Finally return the single cached object matching the request.
     */
    if (resourceLoadRequest.isValid()) {
      result = getCacheResource(getCacheKey(resourceLoadRequest));
      if (result == null) {
          logger->verbose("Resource was not cached previously");

          ResourceAdapter *adapter = getAdapter(resourceLoadRequest);
          if (adapter != null) {
            logger->debug("Loading %s with adapter [%s]",
                resourceLoadRequest.toString().c_str(),
                adapter->toString().c_str());

            try {
              for (auto resource : adapter->load(resourceLoadRequest)) {
                if (resource != null) {
                  logger->debug("Adding Resource [%s]", resource->toString().c_str());
                  this->addResource(resource);
                }
              }
            } catch (std::exception &exception) {
              logger->error("Could not load %s with adapter [%s]: %s", resourceLoadRequest.toString().c_str(), adapter->toString().c_str(),
                  exception.what());
            } catch (...) {
              ;
            }

            /**
             * The returned object is read from the cache - even if the adapter logic was invoked.
             */
            result = getCacheResource(getCacheKey(resourceLoadRequest));
            if (result == null) { //Most likely cause of errors at this point are caused by resource key being different than request key.
              logger->warn("Could not load [%s]", resourceLoadRequest.toString().c_str());
            } else if (result->getMimeType() != resourceLoadRequest.getOutputMimeType()) {
              logger->warn("Could not load [%s] with mimetype [%s] (found with mimetype[%s])", resourceLoadRequest.toString().c_str(),
                  resourceLoadRequest.getOutputMimeType().c_str(), result->getMimeType().c_str());
              result = null;
            }
          } else {
            logger->error("No adapter found for loading %s", resourceLoadRequest.toString().c_str());
          }
      } else {
        logger->debug("Got [%s] from cache: [%s]", resourceLoadRequest.toString().c_str(), result->toString().c_str());
      }
    } else {
      logger->error("Invalid Resource load request %s: %s ", resourceLoadRequest.toString().c_str(),
          resourceLoadRequest.errors().c_str());
    }
  } catch (std::exception &e) {
    logger->error("Error loading [%s]: [%s]",
        resourceLoadRequest.toString().c_str(),
        e.what());
  }

  return result;
}

/**
 * Disposes of the resource by calling the appropriate adapter dispose method. This frees allocated resources but does not remove the resource from internal caches - caller needs to do so as it would break iteration logic.
 */
void ResourceManager::dispose(Resource *resource) const {
  if (resource != null) {
    logger->debug("Disposing of resource [%s]", resource->toString().c_str());

    ResourceAdapter *adapter = this->getAdapter(resource);
    if (adapter != null) {
      adapter->dispose(resource);
    } else {
      logger->warn("NO Adapter found for disposing [%s]: [%s]", resource->getMimeType().c_str(), resource->toString().c_str());
    }

  } else {
    logger->warn("Skipping resource disposal due to missing required information (resource not null and mimetype not empty)");
  }
}

ResourceManager::~ResourceManager() {
  logger->debug("Shutting down resource manager");
  logStatus();

  /**
   * We could not move dispose logic to Resource destructor since it conflicts with the destructor order.
   * As coded, adapter.dispose(resource) was being called in Resource::~Resource() - The issue was dispose usually needs to access member properties
   * as part of cleanup, but those had already been deleted by the subclass destructor.
   *
   */
  for (const auto &resource : resources) {
    if (resource != null) {
      logger->debug("Disposing of resource [%s]", resource.get()->toString().c_str());
      dispose(resource.get());
    }
  }

//	logger->debug("Disposing of %d resources", resourceCache.size());
//	resourceCache.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions
//
//	logger->debug("Disposing of %d resource adapters", resourceAdapters.size());
//	resourceAdapters.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions. This has to happen after resources are deleted, but that is managed by the member definition order.
//	adaptersCache.clear();

  logger->info("Resource manager shutdown complete");
}
