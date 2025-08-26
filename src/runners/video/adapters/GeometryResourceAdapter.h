/*
 * GeometryResourceAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef GEOMETRYRESOURCEADAPTER_H_
#define GEOMETRYRESOURCEADAPTER_H_
#include "JsonParser.h"
#include "ResourceAdapter.h"
#include "NormalGenerator.h"
#include "GeometryCollection.h"
#include "GeometryResource.h"


class GeometryResourceAdapter: public ResourceAdapter {
public:
	GeometryResourceAdapter() {
		logger = LoggerFactory::getLogger("video/GeometryResourceAdapter");
		this->produces(MimeTypes::GEOMETRYCOLLECTION);
		this->accepts(MimeTypes::JSON);
	}

	virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
		JsonParser parser(request.getFileParser());
		GeometryCollection *geometryCollection = new GeometryCollection;
		GeometryResource *resource = new GeometryResource(0);
		resource->setName(Paths::getBasename(request.getFilePath()));
		resource->setUri(request.getFilePath());

		bool generateNormals = false;
		bool generateIndexes = false;

		String token;
		parser.readStartObject();
		while ((token = parser.readToken()) != END_OBJECT && token != FileParser::eof) {
            parser.readValueSeparator();

			if(token == "name") {
				resource->setName(parser.readString());
			} else if (token == "vertices") {
				resource->setVertices(parser.readVector3Array());
			} else if (token == "material") {
				resource->setMaterial(parseMaterial(parser, response));
			} else if (token == "textureCoordinates") {
				resource->setTextureCoordinates(parser.readVector2Array());
			} else if (token == "normals") {
				resource->setNormals(parser.readVector3Array());
			} else if (token == "colors") {
				resource->setColors(parser.readVector3Array());
			} else if (token == "type") {
				String typeString = parser.readString();
				resource->setType(typeString);
			} else if (token == "indices") {
				resource->setIndices(parser.readUnsignedIntegerArray());
			} else if (token == "generateNormals") {
				generateNormals = parser.readBoolean();
			} else if (token == "generateIndexes") {
				generateIndexes = parser.readBoolean();
			} else {
				logger->error("Unexpected token: [%s] at (%d, %d)",
						token.c_str(), parser.getLine(), parser.getColumn());
				parser.takeLine();
			}

			if (parser.peekToken() == ",") {
				parser.readToken();
			}
		}

		if (generateNormals && resource->getNormals().size() == 0) {
			ensureNormals(resource);
		}

		if (generateIndexes && resource->getIndices().size() == 0) {
			buildIndicesArray(resource);
		}

		logger->debug(
				"Primitive %s: [%d] vertices, [%d] indices, [%d] colors, [%d] normals, [%d] textureCoordinates",
				resource->getType().c_str(), resource->getVertices().size(),
				resource->getIndices().size(), resource->getColors().size(),
				resource->getNormals().size(),
				resource->getTextureCoordinates().size());

		log("vertices ", resource->getVertices());
		log("indices ", resource->getIndices());
		log("colors ", resource->getColors());
		log("normals ", resource->getNormals());
		log("textureCoordinates ", resource->getTextureCoordinates());

		response.addResource(resource);
		geometryCollection->addObject(resource);

		response.addResource(geometryCollection);
	}

	/**
	 * This is not really necessary since vector takes care of its own data.
	 */
//	virtual void dispose(Resource *resource) const override {
//	    logger->info("Disposing of [%s]", resource->toString().c_str());
//
//		GeometryResource *geometryResource = (GeometryResource*) resource;
//		geometryResource->getVertices().clear();
//		geometryResource->getColors().clear();
//		geometryResource->getNormals().clear();
//		geometryResource->getTextureCoordinates().clear();
//
//		logger->info("Disposed of %s", resource->toString().c_str());
//	}

private:
	//TODO: Review vertex normals generation. Review amount of needed vertex normals (according to number of indexes or unique vertices?). May need to build adapters and factories based on primitive type.
	void ensureNormals(GeometryResource *resource) const {
		if (resource->getNormals().size() == 0) {
			NormalGenerator *generator = NormalGeneratorFactory::getGenerator(
					resource->getType());

			if (generator != null)
				generator->generateNormals(resource);
		}
	}

	void buildIndicesArray(GeometryResource *resource) const {
		//create one to one index array.
		unsigned int currentIndex = 0;

		for(auto &source : resource->getVertices()) {
			resource->addIndex(currentIndex++);
		}
	}

	void ensureColors(GeometryResource *resource) const {
		unsigned int numberOfColors = (resource->getVertices().size() - resource->getColors().size());
		for (unsigned int currentIndex = 0; currentIndex < numberOfColors; currentIndex++) {
			resource->addColor(vector3(1.0f, 1.0f, 1.0f));
		}
	}

	void buildVerticesArray(GeometryResource *resource, std::vector<vector3> &vertices) const {
		if (resource != null) {
			for (auto &source : vertices) {
				unsigned int index = 0;
				bool preExisting = false;
				for (auto &destination : resource->getVertices()) {
					if (source == destination) {
						resource->addIndex(index);
						preExisting = true;
						break;
					}
					index++;
				}
				if (!preExisting) {
					resource->addVertex(source);
					resource->addIndex(resource->getVertices().size() - 1);
				}
			}
		}
	}

	MaterialResource *parseMaterial(JsonParser &parser, ResourceLoadResponse &response) const {
		String token;
		parser.readStartObject();
		MaterialResource *material = new MaterialResource(vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), 1.0);

		while ((token = parser.readToken()) != END_OBJECT && token != FileParser::eof) {
            parser.readValueSeparator();

            if (token == "name") {
            	material->setName(parser.readString());
            } else if (token == "ambient") {
        		material->setAmbient(parser.readVector3());
        	} else if (token == "specular") {
        		material->setSpecular(parser.readVector3());
        	} else if (token == "diffuse") {
        		material->setDiffuse(parser.readVector3());
        	} else if (token == "emissive") {
        		material->setEmissive(parser.readVector3());
        	} else if (token == "shininess") {
        		material->setShininess(parser.readReal());
        	} else if (token == "alpha") {
        		material->setAlpha(parser.readReal());
        	} else if (token == "d") {
        		material->setAlpha(1.0 - parser.readReal());
        	} else if (token == "ambientTexture") {
        		material->setAmbientTexture(response.getFullPath(parser.readString()));
        	} else if (token == "diffuseTexture") {
        		material->setDiffuseTexture(response.getFullPath(parser.readString()));
        	} else if (token == "specularTexture") {
        		material->setSpecularTexture(response.getFullPath(parser.readString()));
        	} else if (token == "alphaTexture") {
        		material->setAlphaTexture(parser.readString());
        	} else if (token == "bumpTexture" ) {
        		material->setBumptTexture(response.getFullPath(parser.readString()));
			} else {
				logger->error("Unexpected token: [%s] at (%d, %d)",
						token.c_str(), parser.getLine(), parser.getColumn());
				parser.takeLine();
			}

			if (parser.peekToken() == ",") {
				parser.readToken();
			}
		}

		return (MaterialResource *)response.addResource(material);

	}

	void log(String prefix, std::vector<vector2> array) const {
    prefix.insert(0, "[" + std::to_string(array.size()) + "]");
    prefix.append(" [");
    for(auto &current : array) {
      prefix.append(current.toString() + " ");
    }
    prefix.append(" ]");

		logger->verbose(prefix.c_str());
	}

	void log(String prefix, std::vector<unsigned int> array) const {
	  prefix.insert(0, "[" + std::to_string(array.size()) + "]");
	  prefix.append(" [");
	  for(auto &current : array) {
	    prefix.append(std::to_string(current) + " ");
	  }
		prefix.append(" ]");

		logger->verbose(prefix.c_str());
	}

	void log(String prefix, std::vector<vector3> array) const {
		prefix.insert(0, "[" + std::to_string(array.size()) + "]");
		prefix.append(" [");
		for(auto &current : array) {
		  prefix.append(current.toString() + " ");
		}
		prefix.append(" ]");

		logger->verbose(prefix.c_str());
	}
};
#endif /* GEOMETRYRESOURCEADAPTER_H_ */
