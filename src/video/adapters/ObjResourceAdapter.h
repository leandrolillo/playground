/*
 * ObjResourceAdapter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include<GeometryCollection.h>
#include<MaterialCollection.h>

class ObjResourceAdapter: public ResourceAdapter {
public:
    ObjResourceAdapter() {
        logger = LoggerFactory::getLogger("video/ObjResourceAdapter");
        this->produces(MimeTypes::GEOMETRYCOLLECTION);
        this->accepts(MimeTypes::WAVEFRONT_OBJ);
    }

    virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
        TextParser textParser(request.getFileParser());

    	/**
    	 * Auxiliar variables for loading. Obj file format define one global list of vertices, one of normals and one of textCoords. All objects within the file reference this uber-list when using indices.
    	 */
    	std::vector<vector> vertices;
        std::vector<vector> normals;
        std::vector<vector2> textCoords;

        GeometryCollection *objects = new GeometryCollection();
        MaterialCollection *materials = null;

        String token;
        while ((token = textParser.peekToken()) != FileParser::eof) {
            if (token == "o" || token == "v" || token == "vn" || token == "vt" || token == "f") {
            	GeometryResource *object = parseObject(textParser, vertices, normals, textCoords, materials);
            	object->setUri(Paths::add(request.getFilePath(), object->getName()));

            	response.addResource(object);

            	objects->addObject(object);
            } else if (token == "mtllib") {
            	textParser.takeToken();
            	String materialLibraryName = textParser.takeLine();

            	materials = (MaterialCollection *)this->getResourceManager()->load(textParser.getFilename(), materialLibraryName, MimeTypes::MATERIALCOLLECTION);
            } else {
                String line = textParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }
        }

//        logger->info("Parsed [%s] file, converting to geometry...", request.getFilePath().c_str());


        response.addResource(objects);
    }

    GeometryResource *parseObject(TextParser &textParser,
		std::vector<vector> &vertices,
		std::vector<vector> &normals,
		std::vector<vector2> &textCoords,
		MaterialCollection *materials) const {

        GeometryResource *geometry = new GeometryResource(0);
        geometry->setUri(textParser.getFilename());
        geometry->setName(Paths::getBasename(textParser.getFilename()));
        geometry->setType("triangles");


        logger->debug("Parsing object");
        std::vector<vector3>indices;

        String token;
        while ((token = textParser.takeToken()) != FileParser::eof) {
        	if (token == "o") {
        		String name = StringUtils::trim(textParser.takeLine());
        		logger->debug("Name: %s", name.c_str());
        		geometry->setName(name);
        	} else if (token == "v") {
				vertices.push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
			} else if (token == "vn") {
				normals.push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
			} else if (token == "vt") {
				textCoords.push_back(vector2(textParser.readReal(), 1.0 - textParser.readReal()));
			} else if (token == "f") {
				vector newIndices = readIndicesRow(textParser);
				addIndex(geometry, newIndices, vertices, normals, textCoords, indices);

				newIndices = readIndicesRow(textParser);
				addIndex(geometry, newIndices, vertices, normals, textCoords, indices);

				newIndices = readIndicesRow(textParser);
				addIndex(geometry, newIndices, vertices, normals, textCoords, indices);


				String remaining = StringUtils::trim(textParser.takeLine());
				if(!remaining.empty()) {
					logger->error("Expected triangulated obj faces - got extra [%s] at line %d, column %d", remaining.c_str(), textParser.getLine(), textParser.getColumn());
					delete geometry;
					return null;
				}
			} else if (token == "usemtl" && materials != null) {
				String materialName = textParser.takeLine();
				StringUtils::trim(materialName);
				geometry->setMaterial(materials->getMaterial(materialName));
			} else {
                String line = textParser.takeLine();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }

        	if(textParser.peekToken() == "o") {
        		break;
        	}
        }

        logger->debug("Read %d vertices, %d textCoords, %d normals", geometry->getVertices().size(), textCoords.size(), normals.size());

        printLogInfo(geometry);

        return geometry;
    }
private:
    void printLogInfo(GeometryResource *geometry) const {
    	logger->debug("Object [%s]", geometry->getName().c_str());
        logger->debug("%d vertices", geometry->getVertices().size());
        logger->debug("%d textCoords", geometry->getTextureCoordinates().size());
        logger->debug("%d normals", geometry->getNormals().size());
        logger->debug("%d indices", geometry->getIndices().size());

        for (auto &index : geometry->getIndices()) {
            logger->verbose("%d: %s - %s - %s", index, geometry->getVertices().at(index).toString("%.6f").c_str(),
                    geometry->getTextureCoordinates().at(index).toString("%.6f").c_str(),
                    geometry->getNormals().at(index).toString("%.6f").c_str());
        }
    }

    vector readIndicesRow(TextParser &textParser) const {

        int vertexIndex = textParser.readInteger();
        int normalIndex = 0;
        int textCoordIndex = 0;

        if (textParser.takeToken() == "/") {
            if (textParser.peekToken() != "/") {
                textCoordIndex = textParser.readInteger();
            }
        }

        if (textParser.takeToken() == "/") {
            normalIndex = textParser.readInteger();
        }

        return vector(vertexIndex, textCoordIndex, normalIndex);
    }

    void addIndex(GeometryResource *geometry, const vector &newIndices, std::vector<vector> &vertices,
    		std::vector<vector> &normals,
    		std::vector<vector2> &textCoords,
			std::vector<vector3> &indices) const {

    	//TODO: Review this - should reuse data but it does not happen.
    	for(unsigned int index = 0; index < indices.size(); index++) {
    		vector currentIndices = indices[index];
    		if((int)currentIndices.x == (int)newIndices.x  && (int)currentIndices.y == (int)newIndices.y && (int)currentIndices.z == (int)newIndices.z) {
    			geometry->addIndex(index);
    			return;
    		}
    	}

        geometry->addIndex(geometry->getIndices().size());
		geometry->addVertex(vertices.at((int) newIndices.x - 1));
		geometry->addTextureCoordinate(newIndices.y > 0 ? textCoords.at((int) newIndices.y - 1) : vector2(0, 0));
        geometry->addNormal(newIndices.z > 0 ? normals.at((int) newIndices.z - 1) : vector3(0, 0, 0));

    }

};

#endif /* SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_ */
