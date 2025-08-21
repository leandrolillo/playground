#pragma once

#include "resources/MeshResource.h"

class SceneNode {
private:
	matriz_4x4 base;
	const MeshResource &mesh;
	std::vector<SceneNode> children;
	String name;
public:
	const matriz_4x4 &getBase() const {
		return base;
	}

	const MeshResource &getMesh() const {
		return this->mesh;
	}

};
