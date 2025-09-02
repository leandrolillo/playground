/*
 * GeometryRenderer.h
 *
 *  Created on: Sep 11, 2022
 *      Author: leandro
 */

#pragma once

#include <Geometry.h>
#include "DefaultRenderer.h"

class GeometryRenderer {
protected:
    DefaultRenderer &renderer;
    Logger *logger = LoggerFactory::getLogger("GeometryRenderer");

public:
	GeometryRenderer(DefaultRenderer &renderer) : renderer(renderer) {		}

	void render(const Geometry *geometry, bool isColliding = false) const {
		if(geometry != null) {
			switch(geometry->getType()) {
				case GeometryType::SPHERE:
					render((const Sphere *)geometry);
				break;
				case GeometryType::AABB:
					render((const AABB *)geometry);
				break;
				case GeometryType::PLANE:
					render((const Plane *)geometry);
				break;
				case GeometryType::HIERARCHY:
					render((const HierarchicalGeometry *)geometry, isColliding);
				break;
				case GeometryType::HEIGHTMAP:
					render((const HeightMapGeometry *)geometry);
				break;
				case GeometryType::LINE:
					render((const Line *)geometry);
				break;
				case GeometryType::FRUSTUM:
					render((const Frustum *)geometry);
				break;
			}
		}
	}

	void render(const AABB *aabb) const {
        renderer.drawBox(matriz::traslacion(aabb->getOrigin()),
                (real)2 * aabb->getHalfSizes().x,
                (real)2 * aabb->getHalfSizes().y,
                (real)2 * aabb->getHalfSizes().z);
	}

	void render(const Sphere *sphere) const {
		renderer.drawSphere(matriz::traslacion(sphere->getOrigin()), sphere->getRadius());
	}


	/**
	 * I'm sure there's a better way of doing this
	 */
	void render(const Plane *plane, real width = 10.0, real height = 0.01, real depth = 10.0) const {
		vector j = plane->getNormal();
		vector i(1, 0, 0);
		i = (i - (j * (j * i))).normalizado();
		vector k = i ^ j;

		matriz_3x3 orientacion(i, j, k);

		renderer.drawBox(matriz_4x4::base(orientacion, plane->getOrigin()), width, height, depth);
	}

	void render(const Line *line) const {
	}

	void render(const HeightMapGeometry *heightmap) const {

	}

	void render(const HierarchicalGeometry *hierarchy, bool isColliding) const {
		if(isColliding) {
			for(auto &child : hierarchy->getChildren()) {
				this->render(child.get());
			}
		} else {
			this->render(&hierarchy->getBoundingVolume());
		}
	}

	void render(const Frustum *frustum) const {
		logger->info("");
		logger->info("Rendering frustum");
		for( auto &plane: frustum->getHalfSpaces()) {

			logger->info("	Rendering plane %s", plane.toString().c_str());

			renderer.drawLine(matriz_4x4::identidad, plane.getOrigin(), plane.getOrigin() + plane.getNormal() * 10);
			render(&plane, 1, 0.01, 1);
		}
	}
};
