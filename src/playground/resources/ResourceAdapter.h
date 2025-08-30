/*
 * ResourceAdapter.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#pragma once

#include<vector>
#include "Resource.h"
#include "ResourceLoadRequest.h"
#include "ResourceLoadResponse.h"

class ResourceManager;
class ResourceLoadResponse;

/**
 * Resource adapters have
 * - ONE input mimetype - it knows how to parse/handle (accepts)
 * - SEVERAL output mimetypes - they can produce many resources of different mimetypes when loading a file (produces)
 *    - If the produced resources require special logic for deleting them, they should provide dispose(Resource *resource) or the resource be raii
 */
class ResourceAdapter {
private:
  ResourceManager *resourceManager = null;
  String inputMimeType = "";
  std::set<String> outputMimeTypes;
  protected:
  Logger *logger = null;
  public:
  virtual ~ResourceAdapter() {
    if (logger != null) {
      logger->debug("Destroying resource adapter %s", this->toString().c_str());
    }
  }

  ResourceManager* getResourceManager() const {
    return resourceManager;
  }

  void setResourceManager(ResourceManager *resourceManager) {
    this->resourceManager = resourceManager;
  }

  //TODO: Maybe this should be expanded for handling logic here instead of in resource manager - for example checking request has mimetype and matches the output of the adapter, there's a uri, etc.
  virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const = 0;
  virtual void dispose(Resource *resource) const {
    if (resource != null && logger != null) {
      logger->warn("NOT disposing [%s] since dispose method is not overridden", resource->toString().c_str());
    }
  }
  ;

  const String& getInputMimeType() const {
    return inputMimeType;
  }

  ResourceAdapter* accepts(const String &inputMimeType) {
    this->inputMimeType = inputMimeType;
    return this;
  }

  const std::set<String>& getOutputMimeTypes() const {
    return outputMimeTypes;
  }

  void setOutputMimeTypes(const std::set<String> &outputMimeTypes) {
    this->outputMimeTypes = outputMimeTypes;
  }

  ResourceAdapter* produces(const String &outputMimeType) {
    this->outputMimeTypes.insert(outputMimeType);
    return this;
  }

  String errors() const {
    String errors;

    if (outputMimeTypes.empty()) {
      errors.append("Output mimetypes are required");
    }

    return errors;
  }

  bool isValid() const {
    return errors().empty();
  }

  virtual String toString() const {
    String result = this->isValid() ? "✓ " : "x ";

    if (logger != null) {
      logger->getBasename();
    }

    result += "[";
    for (auto &mimeType : this->outputMimeTypes) {
      result += (mimeType + " ");
    }
    result += "] <-";
    result += inputMimeType.empty() ? "*" : inputMimeType;

    return result;
  }

};
