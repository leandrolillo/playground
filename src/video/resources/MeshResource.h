/*
 * MeshResource.h
 *
 *  Created on: Oct 3, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_MESHRESOURCE_H_
#define SRC_VIDEO_RESOURCES_MESHRESOURCE_H_

#include<Resource.h>
#include "VertexArrayResource.h"
#include "MaterialResource.h"
#include "TextureResource.h"

class MeshResource : public Resource
{
	VertexArrayResource *vertexArray = null;
	MaterialResource *material = null;
	TextureResource *texture = null;
	TextureResource *specularMap = null;
	TextureResource *normalMap = null;
public:
	MeshResource() : Resource(0, MimeTypes::MESH) {
	}

	const MaterialResource* getMaterial() const {
		return material;
	}

	void setMaterial(MaterialResource *material) {
		this->material = material;
	}

	TextureResource* getTexture() const {
		return texture;
	}

	void setTexture(TextureResource *texture) {
		this->texture = texture;
	}

	VertexArrayResource* getVertexArray() const {
		return vertexArray;
	}

	void setVertexArray(VertexArrayResource *vertexArray) {
		this->vertexArray = vertexArray;
	}

	TextureResource *getNormalMap() const {
		return normalMap;
	}

	void setNormalMap(TextureResource *normalMap = null) {
		this->normalMap = normalMap;
	}

	TextureResource * getSpecularMap() const {
		return specularMap;
	}

	void setSpecularMap(TextureResource *specularMap = null) {
		this->specularMap = specularMap;
	}
};


#endif /* SRC_VIDEO_RESOURCES_MESHRESOURCE_H_ */
