/*
 * MaterialCollection.h
 *
 *  Created on: Oct 12, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_MATERIALCOLLECTION_H_
#define SRC_VIDEO_RESOURCES_MATERIALCOLLECTION_H_

#include <Resource.h>
#include <MaterialResource.h>
#include <unordered_map>

class MaterialCollection : public Resource {
private:
	std::unordered_map<String, MaterialResource *>materials;
public:
	MaterialCollection() : Resource(0, MimeTypes::MATERIALCOLLECTION) {

	}
	void addMaterial(MaterialResource *material) {
		if(material != null && !material->getName().empty()) {
			materials[material->getName()] = material;
		}
	}

	MaterialResource *getMaterial(const String &name) const {
		auto iterator = materials.find(name);
		if(iterator == materials.end()) {
			return null;
		}

		return iterator->second;
	}

};



#endif /* SRC_VIDEO_RESOURCES_MATERIALCOLLECTION_H_ */
