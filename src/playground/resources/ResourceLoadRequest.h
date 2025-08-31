/*
 * ResourceLoadRequest.h
 *
 *  Created on: Oct 21, 2022
 *      Author: leandro
 */
#pragma once

#include <set>
#include <map>
#include "FileParser.h"
#include "Paths.h"
#include "Resource.h"

class ResourceLoadRequest {
  friend class ResourceManager;
private:
  Logger *logger = LoggerFactory::getLogger("ResourceLoadRequest");

  /*resource uri or fileParser*/
  String uri;
  std::shared_ptr<FileParser> fileParser;

  /*Mimetype of the file to read - if not provided it will be guessed from uri*/
  String inputMimeType;

  /*MimeType of the expected resulting resource*/
  String outputMimeType;

  std::set<String> labels;
  std::map<String, String> options;
  const Resource *parent = null;
  std::vector<String> parents;

  ResourceLoadRequest(const String &uri) {
    withUri(uri);
  }

  ResourceLoadRequest(std::shared_ptr<FileParser> &fileParser) : ResourceLoadRequest(fileParser.get() != null ? fileParser->getFilename() : null) {
    this->fileParser = fileParser;
  }

public:
  /*
   * Rule of five and copy-and-swap
   */
  friend void swap(ResourceLoadRequest &left, ResourceLoadRequest &right) {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;

    // by swapping the members of two objects, the two objects are effectively swapped
    swap(left.uri, right.uri);
    swap(left.fileParser, right.fileParser);
    swap(left.inputMimeType, right.inputMimeType);
    swap(left.outputMimeType, right.outputMimeType);
    swap(left.labels, right.labels);
    swap(left.options, right.options);
    swap(left.parents, right.parents);
    swap(left.parent, right.parent);
  }

  ResourceLoadRequest(const ResourceLoadRequest &right) {
    fileParser = right.fileParser;
    uri = right.uri;
    inputMimeType = right.inputMimeType;
    outputMimeType = right.outputMimeType;
    labels = right.labels;
    options = right.options;
    parents = right.parents;
    parent = right.parent;
  }

  ResourceLoadRequest(ResourceLoadRequest &&right) {
    swap(*this, right);
  }

  ResourceLoadRequest& operator =(ResourceLoadRequest rightCopy) {
    swap(*this, rightCopy);
    return *this;
  }
  /*
   * End of Rule of five and copy-and-swap
   */

  ResourceLoadRequest& withUri(const String &uri) {
    this->uri = uri;
    return *this;
  }

  //TODO: review if it would be better to return new objects - would be safer and shorter syntax while giving up performance
  ResourceLoadRequest& acceptMimeType(const String &mimeType) {
    this->outputMimeType = mimeType;
    return *this;
  }

  ResourceLoadRequest& produceMimeType(const String &mimeType) {
    this->outputMimeType = mimeType;
    return *this;
  }

  ResourceLoadRequest& withInputMimeType(const String &mimeType) {
    this->inputMimeType = mimeType;
    return *this;
  }

  ResourceLoadRequest& withLabels(const std::set<String> &labels) {
    this->labels = labels;
    return *this;
  }

  ResourceLoadRequest& withAdditionalLabels(const std::set<String> &labels) {
    this->labels.insert(labels.begin(), labels.end());

    return *this;
  }

  ResourceLoadRequest& withOptions(const std::map<String, String> &options) {
    this->options = options;
    return *this;
  }

  ResourceLoadRequest& withOption(const String &key, const String &value) {
    this->options[key] = value;
    return *this;
  }

  ResourceLoadRequest& withParent(const Resource *resource) {
    if (resource != null) {
      this->parents.push_back(Paths::getActualPath(resource->getUri()));
    }
    return *this;
  }

  ResourceLoadRequest& withParent(const String &parent) {
    this->parents.push_back(parent);
    return *this;
  }

  const String& getUri() const {
    return this->uri;
  }

  FileParser& getFileParser() {
    if (!fileParser) {
      fileParser = std::shared_ptr <FileParser> (new FileParser(this->getFilePath()));
    }
    return *fileParser.get();
  }

  const String getFilePath() const {
    String path = "";
    for (auto &parent : parents) {
      path = Paths::relative(path, parent);
    }

    return Paths::getActualPath(Paths::relative(path, this->uri));
  }

  const String& getInputMimeType() {
    if (this->inputMimeType.empty()) {
      this->inputMimeType = MimeTypes::guessMimeType(getFilePath());
    }

    return inputMimeType;
  }

  const std::set<String>& getLabels() const {
    return labels;
  }

  const std::map<String, String>& getOptions() const {
    return this->options;
  }

  String getOption(const String &key) const {
    auto iterator = this->options.find(key);
    if (this->options.find(key) != this->options.end()) {
      return iterator->second;
    }

    return "";
  }

  bool hasOption(const String &key) const {
    return getOption(key).empty();
  }

  const String& getOutputMimeType() const {
    return outputMimeType;
  }

  String errors() {
    String errors;

    if (uri.empty()) {
      errors.append("uri is required");
    }

    if (outputMimeType.empty()) {
      errors.append("Output mimetype is required");
    }

    if (getInputMimeType().empty()) {
      errors.append("Could not determine input mimeType and it was not provided");
    }

    return errors;
  }

  bool isValid() {
    return errors().empty();
  }

  String toString() {
    return StringFormatter::format("[%s]<-[%s] [%s]", getOutputMimeType().c_str(), getInputMimeType().c_str(), getUri().c_str());
  }
};
