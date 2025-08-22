/*
 * Resource.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include<set>
#include "JavaLike.h"
#include "Logger.h"
#include "MimeTypes.h"

class Resource {
protected:
	Logger *logger = LoggerFactory::getLogger("resources/Resource");

private:
	unsigned long id; // id makes sense to business only - thus can be repeated. It is not a sequential primary key for all resources
	String uri;
	String mimeType;
	std::set<String> labels;

public:
	Resource(unsigned long id, const String &mimeType) {
		setId(id);
		setMimeType(mimeType);
	}

    /* Not required by this class, but saves work to children classes if they need to implement rule of five/copy and swap*/
    /*friend*/ static void swap(Resource &first, Resource &second)
	{
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two objects, the two objects are effectively swapped
		swap(first.labels, second.labels); /*TODO: check if we need to copy these fields or we can delegate to resource*/
		swap(first.mimeType, second.mimeType);
		swap(first.uri, second.uri);
	}


	virtual ~Resource() {
		logger->debug("Destroying [%s]", this->toString().c_str());
	}

	unsigned long getId() const {
		return id;
	}

	void setId(unsigned long id) {
		this->id = id;
	}

	const String& getMimeType() const {
		return mimeType;
	}

	void setMimeType(const String & mimeType) {
		this->mimeType = mimeType;
	}

	const String& getUri() const {
		return uri;
	}

	void setUri(const String & fileName) {
		this->uri = fileName;
	}

	const std::set<String>& getLabels() const {
		return labels;
	}

	void setLabels(const std::set<String> &labels) {
		this->labels = labels;
	}

	void addLabel(const String &label) {
		this->labels.insert(label);
	}

	virtual String toString() const {
		return String("Resource(id:" + std::to_string(this->getId()) + ") [" + this->getMimeType() + "] [" + this->uri + "]");
	}
};

#endif /* RESOURCE_H_ */
