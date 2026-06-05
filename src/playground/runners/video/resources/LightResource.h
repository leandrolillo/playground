#pragma once

#include <Math3d.h>
#include "MaterialResource.h"

class LightResource: public MaterialResource {
private:
	vector position;
public:
 	LightResource(const vector &position,
 			const vector &ambient,
			const vector &diffuse,
			const vector &specular, real shininess) : MaterialResource(ambient, diffuse, specular, shininess) {
 		this->setMimeType("video/light");
 		this->position = position;
 	}

 	const vector& getPosition() const {
		return this->position;
	}

	void setPosition(const vector &position) {
		this->position = position;
	}
};
