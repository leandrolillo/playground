/*
 * MeshCollection.h
 *
 *  Created on: Oct 13, 2022
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RESOURCES_MESHCOLLECTION_H_
#define SRC_RUNNERS_VIDEO_RESOURCES_MESHCOLLECTION_H_


#include <unordered_map>

#include "../../Resource.h"
#include "../../runners/video/resources/MeshResource.h"

class MeshCollection : public Resource {
private:
	std::unordered_map<String, MeshResource *>materials;
public:
	MeshCollection() : Resource(0, "video/meshes") {
	}

	void addMesh(MeshResource *mesh) {
		if(mesh != null && !mesh->getName().empty()) {
			materials[mesh->getName()] = mesh;
		}
	}

	MeshResource *getMesh(const String &name) const {
		auto iterator = materials.find(name);
		if(iterator == materials.end()) {
			return null;
		}

		return iterator->second;
	}
};


#endif /* SRC_RUNNERS_VIDEO_RESOURCES_MESHCOLLECTION_H_ */
