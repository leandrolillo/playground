/*
 * OpenGLUtilities.h
 *
 *  Created on: Oct 13, 2022
 *      Author: leandro
 */

#pragma once

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include "GeometryResource.h"
#include "VertexArrayResource.h"

class OpenGLUtilities {
protected:
  inline static Logger *logger = null;

  /**
   * use a get logger method to avoid static initialization hell colliding with LoggerFactory::getLogger
   */
  static Logger* getLogger() {
    if (logger == null) {
      logger = LoggerFactory::getLogger("openGL/OpenGLUtilities");
      //logger->setLogLevel(LogLevel::DEBUG);
    }

    return logger;
  }

  //need to be in the same order as PrimitiveTypes
  inline static auto glPrimitiveTypes = std::array{GL_POINTS,GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON };

  //need to be in the same order as VideoAttributes
  inline static auto glAttributes = std::array{
    GL_BYTE, GL_INT, GL_UNSIGNED_INT, GL_FLOAT,
    GL_DEPTH_TEST,
    GL_CULL_FACE, GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK,
    GL_BLEND, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE,
    GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW, GL_STATIC_DRAW,
    GL_LINE_WIDTH,
};

public:
  static VertexArrayResource* generateVertexArray(const GeometryResource *geometry) {
    VertexArrayResource *resource = null;

    glGetError();

    //Create vertex Array
    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error("Error creating vertex array  0x[%x]: [%s]", glError, gluErrorString(glError));
      return null;
    }

    resource = new VertexArrayResource(vertexArray);
    resource->setPrimitiveType(geometry->getType());
    resource->setSize(geometry->getSize());
    resource->setUri(geometry->getUri());
    resource->setName(geometry->getName());

    glBindVertexArray(resource->getId());
    glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error("Error creating vertex array  0x[%x]: [%s]", glError, gluErrorString(glError));
      disposeVertexArray(resource);
      return null;
    }

    if (!geometry->getVertices().empty()) {
      if(!addBuffer<vector3>(*resource, geometry->getVertices(), VERTEX_LOCATION, 1)) {
        disposeVertexArray(resource);
        return null;
      }
    } else if(!geometry->getData().empty()) {
      if(!addBuffer<real>(*resource, geometry->getData(), VERTEX_LOCATION, 1, VideoAttribute::ARRAY_BUFFER, VideoAttribute::FLOAT, VideoAttribute::DYNAMIC_DRAW)) { //TODO: make bufferUsage configurable somehow - either via labels or json in geometry file.
        disposeVertexArray(resource);
        return null;
      }
    }

    if (!geometry->getNormals().empty() && !addBuffer<vector3>(*resource, geometry->getNormals(), NORMAL_LOCATION, 3)) {
      disposeVertexArray(resource);
      return null;
    }

    if (!geometry->getTextureCoordinates().empty() && !addBuffer<vector2>(*resource, geometry->getTextureCoordinates(), TEXTURE_COORDINATES_LOCATION, 2)) {
      disposeVertexArray(resource);
      return null;
    }

    if (!geometry->getColors().empty() && !addBuffer<vector3>(*resource, geometry->getColors(), COLOR_LOCATION, 3)) {
      disposeVertexArray(resource);
      return null;
    }

    /**
     * Indices need to be loaded after vertices
     */
    if (!geometry->getIndices().empty() && !addBuffer<unsigned int>(*resource, geometry->getIndices(), INDEX_LOCATION, 1, VideoAttribute::ELEMENT_ARRAY_BUFFER, VideoAttribute::UNSIGNED_INT)) {
      disposeVertexArray(resource);
      return null;
    }

    glBindVertexArray(0);

    // remove objects from context.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return resource;
  }

  template<typename Primitive>
  static bool addBuffer(VertexArrayResource &resource, const std::vector<Primitive> &data, ShaderAttributeLocation attributeLocation, GLint bufferElementComponents, VideoAttribute bufferDestination = VideoAttribute::ARRAY_BUFFER, VideoAttribute bufferElementType = VideoAttribute::FLOAT, VideoAttribute bufferUsage = VideoAttribute::STATIC_DRAW) {
    if (data.size() > 0) {
      getLogger()->verbose("Creating [%d] vector2 buffer for attribute [%d]", attributeLocation);

      unsigned int sizeofPrimitive = sizeof(Primitive);
      // create vertex buffer
      unsigned int buffer;
      glGenBuffers(1, &buffer); // Generate Vertex Buffer Object
      glBindBuffer(asGlAttribute(bufferDestination), buffer); // Assign it to destination (GL_ARRAY_BUFFER | GL_ELEMENT_ARRAY_BUFFER)
      glBufferData(asGlAttribute(bufferDestination), data.size() * sizeof(Primitive), data.data(), asGlAttribute(bufferUsage)); //send data

      GLenum glError = glGetError();
      if (glError != GL_NO_ERROR) {
        getLogger()->error("Error loading vertex buffer: 0x[%x]: [%s]", glError, gluErrorString(glError));
        return false;
      }

      if (attributeLocation >= 0) { //Add attribute location
        resource.addAttribute(attributeLocation, buffer, 0, data.size(), bufferDestination, bufferUsage);
        glVertexAttribPointer((GLuint) attributeLocation, bufferElementComponents, asGlAttribute(bufferElementType), GL_FALSE, 0, 0);
        glEnableVertexAttribArray(attributeLocation);
        getLogger()->verbose("Enabled attribute [%d]", attributeLocation);
      }

      glError = glGetError();
      if (glError != GL_NO_ERROR) {
        getLogger()->error("Error binding buffer [%d] to attribute Location [%d]: 0x[%x]: [%s]", attributeLocation, buffer, glError, gluErrorString(glError));
        return false;
      }
    }

    return true;
  }

  static void disposeVertexArray(VertexArrayResource *vertexArrayResource) {
    if (vertexArrayResource != null && vertexArrayResource->getId() != 0) {
      getLogger()->debug("Deleting [%s]", vertexArrayResource->toString().c_str());

      if (vertexArrayResource->getId() != 0) {
        getLogger()->debug("VAB [%lu] bound", vertexArrayResource->getId());
        glBindVertexArray(vertexArrayResource->getId());

        for (const auto& [key, value] : vertexArrayResource->getAttributes()) {
          if (value.get() != null) {
            glDisableVertexAttribArray(key);
            unsigned int buffer = value->getBuffer();
            glDeleteBuffers(1, &buffer);
          }
        }
        getLogger()->debug("VAB [%lu] buffers deleted", vertexArrayResource->getId());

        unsigned int vertexArray = vertexArrayResource->getId();
        glDeleteVertexArrays(1, &vertexArray);

        getLogger()->debug("VAB [%lu] deleted", vertexArrayResource->getId());

        glBindVertexArray(0);
        vertexArrayResource->setId(0);
      }
    }
  }

  static GLenum asGlPrimitiveType(PrimitiveType type) {
      return glPrimitiveTypes[(int)type];
  }

  static GLenum asGlAttribute(VideoAttribute attribute) {
    try {
      return glAttributes[(int)attribute];
    } catch(const std::out_of_range& outOfRangeException) {
      logger->error("Invalid attribute [%s]", std::to_string((int)attribute).c_str());
      throw std::invalid_argument("Invalid attribute: [" + std::to_string((int)attribute) + "]");
    }
  }

  static String getGlError() { //Unify with OpenGLResourceAdapter getGLError
    String errorMessage;
    GLenum glError;
    while ((glError = glGetError()) != GL_NO_ERROR) {
      if (errorMessage.size() != 0) {
        errorMessage += ", ";
      }

      errorMessage += std::to_string(glError) + ": " + (const char*) gluErrorString(glGetError());
    }

    return errorMessage;
  }

};
