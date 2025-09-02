#include "ResourceAdapter.h"

//TODO: Maybe this should be expanded for handling logic here instead of in resource manager - for example checking request has mimetype and matches the output of the adapter, there's a uri, etc.
std::vector<Resource*> ResourceAdapter::load(ResourceLoadRequest &request) {
  std::map<String, int> objectCountByMimeType;

  if (this != null && !this->isValid()) {
    logger->error("Resource Adapter [%s] is invalid: [%s]", this->toString().c_str(), this->errors().c_str());
    return std::vector<Resource*>();
  }

  auto resources = doLoad(request);

  auto iterator = resources.begin();
  while (iterator != resources.end()) {
    Resource *resource = *iterator;
    if (resource != null) {
      resource->setUri(request.getFilePath());

      if (resource->getMimeType().empty()) {
        resource->setMimeType(request.getOutputMimeType());
      }

      if (resource->getName().empty()) {
        unsigned int mimeTypeCount = std::count_if(resources.begin(), resources.end(),
            [resource](auto it) {
              return it != null && it->getMimeType() == resource->getMimeType();
            });

        if (mimeTypeCount == 1) { //If the file contains only one resource of this mimetype set name to "", this allows loading by filename without object name
          resource->setName("");
        } else { // generate a name based on mimetype + cardinality
          int count = 0;
          auto it = objectCountByMimeType.find(resource->getMimeType());
          if (it != objectCountByMimeType.end()) {
            count = it->second;
          }

          resource->setName(/*resource->getMimeType() +*/"[" + std::to_string(count) + "]"); //mimetype is not required as part of the name since that is already used as key in resource manager cache
          objectCountByMimeType[resource->getMimeType()] = count++;
        }
      }

      resource->setLabels(request.getLabels());
      logger->debug("Loaded [%s]", resource->toString().c_str());
      iterator++; //only increment iterator if not deleting anything
    } else { //remove null pointers from result
      resources.erase(iterator);
      logger->warn("Request [%s] returned null", request.toString().c_str());
    }
  }

  return resources;
}
