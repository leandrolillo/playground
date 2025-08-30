#ifndef MATERIALRESOURCE_H_
#define MATERIALRESOURCE_H_

#include <Math3d.h>
#include "Resource.h"
#include "StringUtils.h"

/**
 * Should this be a resource?
 */
class MaterialResource: public Resource {
private:
	String name;
	vector3 ambient;
	vector3 diffuse;
	vector3 specular;
	vector3 emissive;
	real alpha = 1.0;
	real shininess = 2;

	String ambientTexture = "";
	String diffuseTexture = "";
	String specularTexture = "";
	String alphaTexture = "";
	String bumpTexture = "";

public:
	MaterialResource(const vector &ambient, const vector &diffuse, const vector &specular, real shininess, real alpha = 1.0) : Resource(0, MimeTypes::WAVEFRONT_MATERIAL) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->alpha = alpha;
	}

	MaterialResource(const String &name, const vector &ambient, const vector &diffuse, const vector &specular, real shininess, real alpha = 1.0) : MaterialResource(ambient, diffuse, specular, shininess, alpha) {
		this->setName(name);
	}

	const vector& getAmbient() const {
		return this->ambient;
	}

	const vector& getDiffuse() const {
		return this->diffuse;
	}

	const vector& getSpecular() const {
		return this->specular;
	}

	const real& getShininess() const {
		return this->shininess;
	}

	const real &getAlpha() const {
	    return this->alpha;
	}

	void setAmbient(const vector &ambient) {
		this->ambient = ambient;
	}

	void setDiffuse(const vector &diffuse) {
		this->diffuse = diffuse;
	}

	void setSpecular(const vector &specular) {
		this->specular = specular;
	}

	void setShininess(real shininess) {
		this->shininess = shininess;
	}

	const String& getName() const {
		return name;
	}

	void setName(const String &name) {
		this->name = name;
		StringUtils::trim(this->name);
	}

	void setAlpha(real alpha = 1.0) {
		this->alpha = alpha;
	}

	const vector3& getEmissive() const {
		return emissive;
	}

	void setEmissive(const vector3 &emissive) {
		this->emissive = emissive;
	}

	const String& getAlphaTexture() const {
		return alphaTexture;
	}

	void setAlphaTexture(const String &alphaTexture) {
		this->alphaTexture = alphaTexture;
	}

	const String& getAmbientTexture() const {
		return ambientTexture;
	}

	void setAmbientTexture(const String &ambientTexture) {
		this->ambientTexture = ambientTexture;
	}

	const String& getBumpTexture() const {
		return bumpTexture;
	}

	void setBumpTexture(const String &bumptTexture) {
		this->bumpTexture = bumptTexture;
	}

	const String& getDiffuseTexture() const {
		return diffuseTexture;
	}

	void setDiffuseTexture(const String &diffuseTexture) {
		this->diffuseTexture = diffuseTexture;
	}

	const String& getSpecularTexture() const {
		return specularTexture;
	}

	void setSpecularTexture(const String &specularTexture) {
		this->specularTexture = specularTexture;
	}
};
#endif
