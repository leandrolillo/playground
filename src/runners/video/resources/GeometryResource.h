/*
 * GeometryResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#pragma once

#include <Math3d.h>
#include <vector>

#include "Resource.h"
#include "MaterialResource.h"

/**
 * Holds "Geometric" (not true, it also has colors and texture coordinates) data. It does not send anything to the gcp.
 * TODO: Find better name specially since it causes confusion with geometry (coldet).
 */
class GeometryResource: public Resource {
private:
	Logger *logger = LoggerFactory::getLogger("video/GeometryResource");

	std::vector<vector3> vertices;
	std::vector<unsigned int> indices;
	std::vector<vector3> normals;
	std::vector<vector2> textureCoordinates;
	std::vector<vector3> colors;
	String type;
	MaterialResource *material = null;

	vector size {0.0, 0.0, 0.0 };
	vector max {REAL_MIN, REAL_MIN, REAL_MIN};
	vector min {REAL_MAX, REAL_MAX, REAL_MAX};
public:
	GeometryResource(unsigned int id) :
			Resource(id, MimeTypes::GEOMETRY) {
		type = "triangles";
	}

	const std::vector<vector3>& getNormals() const {
		return normals;
	}

	void setNormals(const std::vector<vector3> &normals) {
		this->normals = normals;
	}

	void addNormal(const vector3 &normal) {
		this->normals.push_back(normal);
	}


	const std::vector<vector2>& getTextureCoordinates() const {
		return textureCoordinates;
	}

	void setTextureCoordinates(const std::vector<vector2> &texCoordinates) {
		this->textureCoordinates = texCoordinates;
	}

	void addTextureCoordinate(const vector2 &texCoordinate) {
		this->textureCoordinates.push_back(texCoordinate);
	}

	const std::vector<vector3>& getVertices() const {
		return vertices;
	}

	void setVertices(const std::vector<vector3> &vertex) {
		this->vertices = vertex;

		//recalculate size
		max = vector(REAL_MIN, REAL_MIN, REAL_MIN);
		min = vector(REAL_MAX, REAL_MAX, REAL_MAX);
		for(auto &vertice : vertices) {
			max = vector(std::max(max.x, vertice.x), std::max(max.y, vertice.y), std::max(max.z, vertice.z));
			min = vector(std::min(min.x, vertice.x), std::min(min.y, vertice.y), std::min(min.z, vertice.z));
		}
		this->size = max - min;
		logger->info("[%s] size is: %s", getName().c_str(), size.toString().c_str());
	}

	void addVertex(const vector3 &vertex) {
		this->vertices.push_back(vertex);

		max = vector(std::max(max.x, vertex.x), std::max(max.y, vertex.y), std::max(max.z, vertex.z));
		min = vector(std::min(min.x, vertex.x), std::min(min.y, vertex.y), std::min(min.z, vertex.z));
		this->size = max - min;
		//logger->info("[%s] size is: %s", this->name.c_str(), size.toString().c_str());
	}

	const std::vector<vector3>& getColors() const {
		return colors;
	}

	void setColors(const std::vector<vector3> &colors) {
		this->colors = colors;
	}

	void addColor(const vector3 &color) {
		this->colors.push_back(color);
	}

	const std::vector<unsigned int>& getIndices() const {
		return indices;
	}

	void setIndices(const std::vector<unsigned int> &indexes) {
		this->indices = indexes;
	}

	void addIndex(unsigned int index) {
		this->indices.push_back(index);
	}

	const String &getType() const {
		return type;
	}

	void setType(String type) { //TODO: Move this to an enum
		this->type = type;
	}

	const vector3 &getSize() const {
		return size;
	}

	MaterialResource* getMaterial() const {
		return material;
	}

	void setMaterial(MaterialResource *material) {
		this->material = material;
	}
};
