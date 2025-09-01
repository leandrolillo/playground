/*
 * MtlResourceAdapter.h
 *
 *  Created on: Oct 5, 2022
 *      Author: leandro
 */

#pragma once

#include "TextParser.h"
#include "ResourceAdapter.h"
#include "MaterialResource.h"
#include "MaterialCollection.h"

class MtlResourceAdapter: public ResourceAdapter {
public:
  MtlResourceAdapter() {
    logger = LoggerFactory::getLogger(typeid(*this).name());
    this->produces(MimeTypes::MATERIALCOLLECTION);
    this->accepts(MimeTypes::WAVEFRONT_MATERIAL);
  }
protected:
  virtual std::vector<Resource*> doLoad(ResourceLoadRequest &request) const override {
    std::vector<Resource*> response;
    TextParser textParser(request.getFileParser());

    MaterialCollection *materials = new MaterialCollection();

    String token;
    while ((token = textParser.takeToken()) != FileParser::eof) {
      if (token == "newmtl") {
        MaterialResource *material = parseMaterial(textParser, textParser.takeLine(), request);
        material->setUri(Paths::add(request.getFilePath(), material->getName()));
        material->setMimeType(MimeTypes::WAVEFRONT_MATERIAL);

        response.push_back(material);
        materials->addMaterial(material);
      } else {
        String line = textParser.takeLine().c_str();
        logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
      }
    }

    response.push_back(materials);

    return response;
  }

private:
  MaterialResource* parseMaterial(TextParser &textParser, const String &name, ResourceLoadRequest &request) const {
    MaterialResource *material = new MaterialResource(name, vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), 1.0);

    String token;
    while ((token = textParser.takeToken()) != FileParser::eof) {
      if (token == "Ka") {
        material->setAmbient(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
      } else if (token == "Ks") {
        material->setSpecular(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
      } else if (token == "Kd") {
        material->setDiffuse(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
      } else if (token == "Ke") {
        material->setEmissive(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
      } else if (token == "Ns") {
        material->setShininess(textParser.readReal());
      } else if (token == "Tr") {
        material->setAlpha(textParser.readReal());
      } else if (token == "d") {
        material->setAlpha(1.0 - textParser.readReal());
      } else if (token == "map_Ka") {
        material->setAmbientTexture(request.relativeUri(textParser.takeLine()));
      } else if (token == "map_Kd") {
        material->setDiffuseTexture(request.relativeUri(textParser.takeLine()));
      } else if (token == "map_Ks") {
        material->setSpecularTexture(request.relativeUri(textParser.takeLine()));
      } else if (token == "map_d") {
        material->setAlphaTexture(textParser.takeLine());
      } else if (token == "map_bump" || token == "bump") {
        material->setBumpTexture(request.relativeUri(textParser.takeLine()));
      } else {
        String line = textParser.takeLine().c_str();
        logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
      }

      if (textParser.peekToken() == "newmtl") {
        break;
      }
    }

    return material;
  }
};
