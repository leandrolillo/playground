/*
 * TerrainResourceAdapter.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#pragma once

#include "JsonParser.h"
#include "ResourceAdapter.h"
#include "OpenGLUtilities.h"
#include "TerrainResource.h"

class TerrainResourceAdapter: public ResourceAdapter {
public:
  TerrainResourceAdapter(ResourceManager &resourceManager) : ResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/TerrainResourceAdapter");
    this->produces(MimeTypes::TERRAIN);
  }

  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*>response;

    JsonParser jsonParser(request.getFileParser());
    TerrainResource *resource = new TerrainResource();

    jsonParser.readStartObject();

    String token;
    while ((token = jsonParser.readToken()) != END_OBJECT && token != FileParser::eof) {
      jsonParser.readValueSeparator();

      if (token == "r") {
        resource->setR(
            (TextureResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::TEXTURE)));
      } else if (token == "g") {
        resource->setG(
            (TextureResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::TEXTURE)));
      } else if (token == "b") {
        resource->setB(
            (TextureResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::TEXTURE)));
      } else if (token == "a") {
        resource->setA(
            (TextureResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::TEXTURE)));
      } else if (token == "map") {
        resource->setMap(
            (TextureResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::TEXTURE)));
      } else if (token == "model") {
        resource->setModel(
            (VertexArrayResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::VERTEXARRAY)));
      } else if (token == "heightmap") {
        resource->setHeightmap(
            (HeightMapResource*) this->getResourceManager().load(request.newRequest(jsonParser.readString()).acceptMimeType(MimeTypes::HEIGHTMAP)));
        resource->setModel(buildModel(resource->getHeightMap()));
        response.push_back(resource->getModel());
      } else {
        logger->error("Unexpected token: [%s] at (%d, %d)",
            token.c_str(), jsonParser.getLine(), jsonParser.getColumn());
      }

      if (jsonParser.peekToken() == ",") {
        jsonParser.readToken();
      }
    }

    response.push_back(resource);

    return response;
  }

protected:
  VertexArrayResource* buildModel(const HeightMapResource *heightMap) const {
    VertexArrayResource *model = null;
    if (heightMap != null) {
      GeometryResource heightMapGeometry(0);
      heightMapGeometry.setName(Paths::getBasename(heightMap->getUri()));
      heightMapGeometry.setType("triangles");

      for (unsigned int i = 0; i < heightMap->getGridWidth(); i++) {
        for (unsigned int j = 0; j < heightMap->getGridHeight(); j++) {
          heightMapGeometry.addVertex(heightMap->positionAtGrid(i, j));
          heightMapGeometry.addNormal(heightMap->normalAtGrid(i, j));
          heightMapGeometry.addTextureCoordinate(heightMap->textCoordAtGrid(i, j));
        }
      }

      for (unsigned int i = 1; i < heightMap->getGridWidth(); i++) {
        for (unsigned int j = 1; j < heightMap->getGridHeight(); j++) {
          heightMapGeometry.addIndex(i * heightMap->getGridWidth() + j);
          heightMapGeometry.addIndex(i * heightMap->getGridWidth() + (j - 1));
          heightMapGeometry.addIndex((i - 1) * heightMap->getGridWidth() + (j - 1));

          heightMapGeometry.addIndex((i - 1) * heightMap->getGridWidth() + j);
          heightMapGeometry.addIndex(i * heightMap->getGridWidth() + j);
          heightMapGeometry.addIndex((i - 1) * heightMap->getGridWidth() + (j - 1));
        }
      }

      model = OpenGLUtilites::generateVertexBuffer(&heightMapGeometry);
      model->setUri(heightMap->getUri());
    }

    return model;
  }

};
