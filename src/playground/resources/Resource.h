/*
 * Resource.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */
#pragma once

#include<set>
#include "JavaLike.h"
#include "Logger.h"
#include "MimeTypes.h"
#include "Paths.h"

class Resource {
protected:
	Logger *logger = LoggerFactory::getLogger("resources/Resource");

private:
	unsigned long id; // id makes sense to business only - thus can be repeated. It is not a sequential primary key for all resources
	String uri;
	String name;
	String mimeType;
	std::set<String> labels;

public:
	Resource(unsigned long id, const String &mimeType) : mimeType(mimeType) {
		setId(id);
	}

  Resource(const String &mimeType) : Resource(0, mimeType)  {
  }

  /* Not required by this class, but saves work to children classes if they need to implement rule of five/copy and swap*/
  /*friend*/
  static void swap(Resource &first, Resource &second) noexcept
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

  const String& getUri() const {
    return uri;
  }

  const String &getName() const {
    return name;
  }

  const String getFqdn() const {
    String fqdn = Paths::add(this->uri, this->name);
    return Paths::add(this->uri, this->name);
  }

  const String& getMimeType() const {
    return mimeType;
  }


	Resource &setId(unsigned long id) {
		this->id = id;
		return *this;
	}

	Resource &setName(const String &name) {
	  this->name = name;
	  return *this;
	}

	void setMimeType(const String & mimeType) {
		this->mimeType = mimeType;
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

	virtual bool supportsCaching() {
	  return true;
	}

	virtual String toString() const {
		return String("Resource(id:" + std::to_string(this->getId()) + ") [" + this->getMimeType() + "] [" + this->uri + "]");
	}
};
