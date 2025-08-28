/*
 * ResourceLoadRequest.h
 *
 *  Created on: Oct 21, 2022
 *      Author: leandro
 */

#ifndef SRC_PLAYGROUND_RESOURCES_RESOURCELOADREQUEST_H_
#define SRC_PLAYGROUND_RESOURCES_RESOURCELOADREQUEST_H_

#include <set>
#include <map>
#include "FileParser.h"
#include "Paths.h"
#include "Resource.h"

class ResourceLoadRequest {
private:
	Logger *logger = LoggerFactory::getLogger("ResourceLoadRequest");

	/*resource uri*/
	String uri;

	/*Mimetype of the file to read - if not provided it will be guessed from uri*/
	String inputMimeType;

	/*MimeType of the expected resulting resource*/
	String outputMimeType;

	std::set<String> labels;
	std::shared_ptr<FileParser> fileParser;
	const Resource *parent = null;
	std::map<String, String>options;
	std::vector<String>parents;

public:
	ResourceLoadRequest(const String &uri) {
		withUri(uri);
	}

	ResourceLoadRequest(std::shared_ptr<FileParser> &fileParser) : fileParser(fileParser) {
		this->uri = fileParser.get()->getFilename();
	}

  ResourceLoadRequest &withUri(const String &uri) {
    this->uri= uri;
    return *this;
  }

	//TODO: review if it would be better to return new objects - would be safer and shorter syntax while giving up performance
	ResourceLoadRequest &acceptMimeType(const String &mimeType) {
		this->outputMimeType = mimeType;
		return *this;
	}

	ResourceLoadRequest &produceMimeType(const String &mimeType) {
		this->outputMimeType = mimeType;
		return *this;
	}

	ResourceLoadRequest &withInputMimeType(const String &mimeType) {
		this->inputMimeType = mimeType;
		return *this;
	}

	ResourceLoadRequest &withLabels(const std::set<String> &labels) {
		this->labels = labels;
		return *this;
	}

	ResourceLoadRequest &withAdditionalLabels(const std::set<String> &labels) {
		this->labels.insert(labels.begin(), labels.end());

		return *this;
	}

	ResourceLoadRequest &withOptions(const std::map<String, String> &options) {
		this->options = options;
		return *this;
	}

	ResourceLoadRequest &withOption(const String &key, const String &value) {
		this->options[key] = value;
		return *this;
	}

	ResourceLoadRequest &withParent(const Resource *resource) {
		if(resource != null) {
			this->parents.push_back(Paths::getActualPath(resource->getUri()));
		}
		return *this;
	}

	ResourceLoadRequest &withParent(const String &parent) {
			this->parents.push_back(parent);
			return *this;
		}

	const String &getUri() const {
		return this->uri;
	}

	FileParser& getFileParser() {
		if(!fileParser) {
			fileParser = std::shared_ptr<FileParser>(new FileParser(this->getFilePath()));
		}
		return *fileParser.get();
	}

	const String getFilePath() const {
		String path = "";
		for(auto &parent : parents) {
			path = Paths::relative(path, parent);
		}

		return Paths::getActualPath(Paths::relative(path, this->uri));
	}

	const String& getInputMimeType() {
		if(this->inputMimeType.empty()) {
			this->inputMimeType = MimeTypes::guessMimeType(getFilePath());
		}

		return inputMimeType;
	}

	const std::set<String>& getLabels() const {
		return labels;
	}

	const std::map<String, String> &getOptions() const {
		return this->options;
	}

	String getOption(const String &key) const {
		auto iterator = this->options.find(key);
		if(this->options.find(key) != this->options.end()) {
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

		if(uri.empty()) {
			errors.append("uri is required");
		}

		if(outputMimeType.empty()) {
			errors.append("Output mimetype is required");
		}

		if(getInputMimeType().empty()) {
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




#endif /* SRC_PLAYGROUND_RESOURCES_RESOURCELOADREQUEST_H_ */
