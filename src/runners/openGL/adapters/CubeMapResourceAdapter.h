/*
 * CubeMapResourceAdapter.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#pragma once

#include "JsonParser.h"
#include "TextureResourceAdapter.h"
#include "CubeMapResource.h"

class CubeMapResourceAdapter: public TextureResourceAdapter {
public:
  CubeMapResourceAdapter(ResourceManager &resourceManager) : TextureResourceAdapter(resourceManager) {
    logger = LoggerFactory::getLogger("video/CubeMapResourceAdapter");

    this->setOutputMimeTypes(std::set<String> { MimeTypes::CUBEMAP }); // override texture resource adapter mimetypes.
  }

protected:
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;
    JsonParser jsonParser(request.getFileParser());

    unsigned int textureHandler = 0;
    getGlError();
    glGenTextures(1, &textureHandler);

    CubeMapResource *resource = new CubeMapResource(textureHandler);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandler);

    jsonParser.readStartObject();

    String token;
    while ((token = jsonParser.readToken()) != END_OBJECT && token != FileParser::eof) {
      String faceName = token;
      jsonParser.readValueSeparator();
      String faceFileName = jsonParser.readString();
      ImageResource *imageResource = (ImageResource*) this->getResourceManager().load(faceFileName,
          MimeTypes::IMAGE,
          std::set<String> { ResourceManager::EphemeralLabel });

      if (imageResource != null) {
        if (imageResource->getBpp() == 32) {
          glTexImage2D(getLocation(faceName), 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA, GL_UNSIGNED_BYTE,
              imageResource->getData());
        }
        else {
          glTexImage2D(getLocation(faceName), 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR, GL_UNSIGNED_BYTE,
              imageResource->getData());
        }
      } else {
        logger->error("Could not load image [%s]", faceFileName.c_str());
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      String errorMessage;
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error loading [%s] cubeMap: face [%s (%u)]: [%s]",
            request.getFilePath().c_str(),
            faceFileName.c_str(),
            getLocation(faceName),
            errorMessage.c_str());
        dispose(resource);
        return response;
      }

      if (jsonParser.peekToken() == ",") {
        jsonParser.readToken();
      }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    response.push_back(resource);

    return response;
  }

  //TODO: review if this needs to implement dispose
//	void dispose(Resource *resource) const override {
//
//	}
};
