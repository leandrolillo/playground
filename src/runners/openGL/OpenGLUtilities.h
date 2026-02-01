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
    GL_DEPTH_TEST,
    GL_CULL_FACE, GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK,
    GL_BLEND, GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE,
    GL_LINE_WIDTH,
    GL_MAX_TEXTURE_IMAGE_UNITS
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

    if (geometry->getVertices().size() != 0
        && !addBuffer(VERTEX_LOCATION, resource, GL_ARRAY_BUFFER,
            geometry->getVertices())) {
      disposeVertexArray(resource);
      return null;
    }

    if (geometry->getNormals().size() != 0
        && !addBuffer(NORMAL_LOCATION, resource, GL_ARRAY_BUFFER,
            geometry->getNormals())) {
      disposeVertexArray(resource);
      return null;
    }

    if (geometry->getTextureCoordinates().size() != 0
        && !addBuffer(TEXTURE_COORDINATES_LOCATION, resource,
        GL_ARRAY_BUFFER,
            geometry->getTextureCoordinates())) {
      disposeVertexArray(resource);
      return null;
    }

    if (geometry->getColors().size() != 0
        && !addBuffer(COLOR_LOCATION, resource, GL_ARRAY_BUFFER,
            geometry->getColors())) {
      disposeVertexArray(resource);
      return null;
    }

    /**
     * Indices need to be loaded after vertices
     */
    if (geometry->getIndices().size() != 0
        && !addBuffer(INDEX_LOCATION, resource,
        GL_ELEMENT_ARRAY_BUFFER, geometry->getIndices())) {
      disposeVertexArray(resource);
      return null;
    }

    glBindVertexArray(0);

    // remove objects from context.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return resource;
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

protected:
  static bool addBuffer(ShaderAttributeLocation attributeLocation,
      VertexArrayResource *resource, GLenum bufferDestination,
      const std::vector<vector2> &data) {
    if (data.size() > 0) {
      getLogger()->verbose("Creating [%d] vector2 buffer for attribute [%d]",
          data.size(), attributeLocation);

      // create vertex buffer
      unsigned int buffer;
      glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
      glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
      glBufferData(bufferDestination, data.size() * sizeof(vector2),
          (real*) data.data(), GL_STATIC_DRAW);

      GLenum glError = glGetError();
      if (glError != GL_NO_ERROR) {
        getLogger()->error("Error loading vertex buffer: 0x[%x]: [%s]", glError, gluErrorString(glError));
        return false;
      }

      if (attributeLocation >= 0) {
        resource->addAttribute(attributeLocation, buffer, 0,
            data.size());
        glVertexAttribPointer((GLuint) attributeLocation, 2, GL_FLOAT,
        GL_FALSE, 0, 0);
        glEnableVertexAttribArray(attributeLocation);
        getLogger()->verbose("Enabled attribute [%d]", attributeLocation);
      }

      glError = glGetError();
      if (glError != GL_NO_ERROR) {
        getLogger()->error(
            "Error binding buffer [%d] to attribute Location [%d]: 0x[%x]: [%s]",
            attributeLocation, buffer, glError, gluErrorString(glError));
        return false;
      }
    }

    return true;
  }

  static bool addBuffer(ShaderAttributeLocation attributeLocation,
      VertexArrayResource *resource, GLenum bufferDestination,
      const std::vector<vector3> &data) {
    getLogger()->verbose("Creating [%d] vector3 buffer for attribute [%d]",
        data.size(), attributeLocation);

    // create vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
    glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
    glBufferData(bufferDestination, data.size() * sizeof(vector3),
        (real*) data.data(), GL_STATIC_DRAW);

    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error("Error loading vertex buffer: 0x[%x]: [%s]", glError, gluErrorString(glError));
      return false;
    }

    if (attributeLocation >= 0) {
      resource->addAttribute(attributeLocation, buffer, 0, data.size());
      glVertexAttribPointer((GLuint) attributeLocation, 3, GL_FLOAT,
      GL_FALSE, 0, 0);
      glEnableVertexAttribArray(attributeLocation);
      getLogger()->verbose("Enabled attribute [%d]", attributeLocation);
    }

    glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error(
          "Error binding buffer [%d] to attribute location [%d]: 0x[%x]: [%s]",
          attributeLocation, buffer, glError, gluErrorString(glError));
      return false;
    }

    return true;
  }
  static bool addBuffer(ShaderAttributeLocation attributeLocation,
      VertexArrayResource *resource, GLenum bufferDestination,
      const std::vector<unsigned int> &data) {
    getLogger()->verbose("Creating [%d] unsigned int buffer for attribute [%d]",
        data.size(), attributeLocation);

    // create vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
    glBindBuffer(bufferDestination, buffer); // bind to GL_ELEMENT_ARRAY_BUFFER, as we're about to call glBufferData(GL_ELEMENT_ARRAY_BUFFER)
    glBufferData(bufferDestination, data.size() * sizeof(unsigned int),
        (unsigned int*) data.data(), GL_STATIC_DRAW);

    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error("Error loading vertex buffer: 0x[%x]: [%s]", glError, gluErrorString(glError));
      return false;
    }

    if (attributeLocation >= 0) {
      resource->addAttribute(attributeLocation, buffer, 0, data.size());
      glVertexAttribPointer((GLuint) attributeLocation, 1,
      GL_UNSIGNED_INT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(attributeLocation);
      getLogger()->verbose("Enabled attribute [%d]", attributeLocation);
    }

    glError = glGetError();
    if (glError != GL_NO_ERROR) {
      getLogger()->error("Error binding buffer [%d] to location [%d]: 0x[%x]: [%s]",
          buffer, attributeLocation, glError, gluErrorString(glError));
      return false;
    }

    return true;
  }
};
