/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

const String ResourceManager::EphemeralLabel = {"ephemeral"};

ResourceAdapter * ResourceManager::addAdapter(std::unique_ptr<ResourceAdapter> adapterUniquePtr) { //should this accept a unique_ptr and move it?
	logger->debug("Adding adapter");
	if(adapterUniquePtr.get() != null && adapterUniquePtr->isValid()) {
		logger->debug("Adapter [%s] is valid - proceeding to add it", adapterUniquePtr->toString().c_str());

		ResourceAdapter * adapter = adapterUniquePtr.get();
		if(resourceAdapters.find(adapter) == resourceAdapters.end()) {
			/* std::move makes this an l-value so it behaves like it is a temporary variable - it makes adapterUniquePtr variable point to null so it should not be referenced after this. */
			resourceAdapters.insert(std::move(adapterUniquePtr));
			adapter->setResourceManager(this);

			logger->debug("Adapter [%s] added to set with resourceManager [%d]", adapter->toString().c_str(), adapter->getResourceManager());
		} else {
			logger->warn("Skipping adapter [%s] - already managed", adapter->toString().c_str());
		}

		for(auto &outputMimeType : adapter->getOutputMimeTypes()) {
			String key = adapter->getInputMimeType().empty() ? outputMimeType  + "|" : outputMimeType + "|" + adapter->getInputMimeType();
			adaptersCache[key] = adapter;

			logger->debug("Adapter [%s] added to manage [%s] with key [%s]", adapter->toString().c_str(), outputMimeType.c_str(), key.c_str());
		}

		return adapter;
	}

	logger->error("NOT adding invalid adapter [%s]: [%s] ",
			adapterUniquePtr.get() == null ? "null" : adapterUniquePtr->toString().c_str(),
			adapterUniquePtr.get() == null ? "N/A" : adapterUniquePtr->errors().c_str());

	return null;
}

Resource* ResourceManager::load(ResourceLoadRequest &resourceLoadRequest) {
	Resource *result = null;
	try {
		/**
		 * Find the resource in the cache. If not there, then trigger loading via adapter.
		 * Adapter loads as many objects as it wants and puts them in resource manager cache
		 * Finally return the single cached object matching the request.
		 */
		result = getCacheResource(getCacheKey(resourceLoadRequest));
		if (result == null) {
			if(resourceLoadRequest.isValid()) {
					logger->verbose("Resource was not cached previously");

					ResourceAdapter *adapter = getAdapter(resourceLoadRequest);
					if (adapter != null) {
						logger->debug("Loading %s with adapter [%s]",
								resourceLoadRequest.toString().c_str(),
								adapter->toString().c_str());

						try {
							ResourceLoadResponse response(resourceLoadRequest, *this);
							adapter->load(resourceLoadRequest, response);
						} catch(Exception &exception) {
							logger->error("Could not load %s with adapter [%s]: %s", resourceLoadRequest.toString().c_str(), adapter->toString().c_str(), exception.getMessage().c_str());
						} catch(...) {
							;
						}
					} else {
						logger->error("No adapter found for loading %s", resourceLoadRequest.toString().c_str());
					}
			} else {
				logger->error("Invalid Resource load request %s: %s ", resourceLoadRequest.toString().c_str(), resourceLoadRequest.errors().c_str());
			}

			/**
			 * The returned object is always read from the cache - even if the adapter logic was invoked.
			 */
			result = getCacheResource(getCacheKey(resourceLoadRequest));
			if(result == null) {
				logger->warn("Could not load [%s]", resourceLoadRequest.toString().c_str());
			} else if(result->getMimeType() != resourceLoadRequest.getOutputMimeType()) {
				logger->warn("Could not load [%s] with mimetype [%s] (found with mimetype[%s])", resourceLoadRequest.toString().c_str(), resourceLoadRequest.getOutputMimeType().c_str(), result->getMimeType().c_str());
				result = null;
			}

		} else {
			logger->debug("Getting %s from cache", resourceLoadRequest.toString().c_str());
		}

	} catch (Exception &e) {
		logger->error("Error loading [%s]: [%s]",
				resourceLoadRequest.toString().c_str(),
				e.getMessage().c_str());
	}

	return result;
}

/**
 * Disposes of the resource by calling the appropriate adapter dispose method. This frees allocated resources but does not remove the resource from internal caches - caller needs to do so as it would break iteration logic.
 */
void ResourceManager::dispose(Resource *resource) const {
	if(resource != null) {
		logger->debug("Disposing of resource [%s]", resource->toString().c_str());

		ResourceAdapter *adapter = this->getAdapter(resource);
		if(adapter != null) {
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
	for(const auto &resource : resources) {
		if(resource != null) {
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
