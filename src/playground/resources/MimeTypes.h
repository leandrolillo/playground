/*
 * MimeTypes.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#pragma once
#include "JavaLike.h"
#include "StringUtils.h"

class MimeTypes {
public:
  Logger *logger = LoggerFactory::getLogger("ResourceLoadRequest");

  inline static const String TXT = "text/plain";
  inline static const String JSON = "application/json";

  inline static const String PNG = "image/png";
  inline static const String TGA = "image/tga";
  inline static const String JPEG = "image/jpeg";
  inline static const String IMAGE = "video/image";
  inline static const String TEXTURE = "video/texture";
  inline static const String CUBEMAP = "video/cubemap";

  inline static const String WAVE = "audio/wav";
  inline static const String OGG = "audio/ogg";
  inline static const String AUDIO = "audio/audio";
  inline static const String AUDIOBUFFER = "audio/buffer";
  inline static const String AUDIOSOURCE = "audio/source";

  inline static const String WAVEFRONT_OBJ = "model/obj";
  inline static const String WAVEFRONT_MATERIAL = "model/mtl";
  inline static const String GEOMETRY = "model/geometry";
  inline static const String MATERIAL = "model/material";
  inline static const String VERTEXARRAY = "video/vertex-array";
  inline static const String MESH = "video/mesh";

  inline static const String HEIGHTMAP = "video/heightmap";
  inline static const String TERRAIN = "video/terrain";

  inline static const String GLSL = "text/x-glsl";
  inline static const String SHADERPROGRAM = "video/shader-program";
  inline static const String VERTEXSHADER = "video/vertex-shader";
  inline static const String FRAGMENTSHADER = "video/fragment-shader";
  inline static const String GEOMETRYSHADER = "video/geometry-shader";
  inline static const String TESELLATIONSHADER = "video/tesellation-shader";

  static String guessMimeType(const String &fileName) {
    return defaultInputMimeType(fileName);
  }

  static String defaultInputMimeType(const String &fileName) {
    String extension = guessExtension(fileName);
    if (extension == "ogg") {
      return OGG;
    } else if (extension == "wav") {
      return WAVE;
    } else if (extension == "jpeg" || extension == "jpg") {
      return JPEG;
    } else if (extension == "png") {
      return PNG;
    } else if (extension == "tga") {
      return TGA;
    } else if (extension == "txt") {
      return TXT;
    } else if (extension == "json") {
      return JSON;
    } else if (extension == "glsl") {
      return GLSL;
    } else if (extension == "obj") {
      return WAVEFRONT_OBJ;
    } else if (extension == "mtl") {
      return WAVEFRONT_MATERIAL;
    }

    return "";
  }
  static String defaultOutputMimeType(const String &fileName) {
    String extension = guessExtension(fileName);
    if (extension == "ogg") {
      return AUDIO;
    } else if (extension == "wav") {
      return AUDIO;
    } else if (extension == "jpeg" || extension == "jpg") {
      return IMAGE;
    } else if (extension == "png") {
      return IMAGE;
    } else if (extension == "tga") {
      return IMAGE;
    } else if (extension == "obj") {
      return GEOMETRY;
    } else if (extension == "mtl") {
      return MATERIAL;
    }

    return "";
  }
private:
  static String guessExtension(const String &fileName) {
    auto start = fileName.find_last_of(".");
    if (start != std::string::npos) {
      auto end = fileName.find("/", start + 1); //if we need more delimiter characters we should switch to regexs.
      if(end == std::string::npos) {
        end = fileName.length();
      }

      return StringUtils::trim(StringUtils::toLowercase(fileName.substr(start + 1, end - start - 1)));
    }

    return "";
  }

};
