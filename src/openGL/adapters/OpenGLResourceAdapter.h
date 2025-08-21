/*
 * OpenGLResourceAdapter.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include <OpenGL/gl3.h>

class OpenGLResourceAdapter : public ResourceAdapter {
protected:
    String getGlError() const {
        String errorMessage;

        GLenum glError;
        while ((glError = glGetError()) != GL_NO_ERROR) {
            if (errorMessage.size() != 0) {
                errorMessage += ", ";
            }
            if (glError == GL_INVALID_ENUM) {
                errorMessage += "GL_INVALID_ENUM";
            } else if (glError == GL_INVALID_VALUE) {
                errorMessage += "GL_INVALID_VALUE";
            } else if (glError == GL_INVALID_OPERATION) {
                errorMessage += "GL_INVALID_OPERATION";
            } else if (glError == 0x0503) {
                errorMessage += "GL_STACK_OVERFLOW";
            } else if (glError == 0x0504) {
                errorMessage += "GL_STACK_UNDERFLOW";
            } else if (glError == GL_OUT_OF_MEMORY) {
                errorMessage += "GL_OUT_OF_MEMORY";
            } else if (glError == GL_INVALID_FRAMEBUFFER_OPERATION) {
                errorMessage += "GL_INVALID_FRAMEBUFFER_OPERATION";
            } else if (glError == 0x8031) {
                errorMessage += "GL_TABLE_TOO_LARGE";
            } else {
                errorMessage += std::to_string(glError);
            }
        }

        return errorMessage;
    }

    GLenum asGlPrimitiveType(const String &typeString) const {
        if (typeString == "points")
            return GL_POINTS;
        else if (typeString == "points")
            return GL_LINES;
        else if (typeString == "lineLoop")
            return GL_LINE_LOOP;
        else if (typeString == "lineStrip")
            return GL_LINE_STRIP;
        else if (typeString == "lines")
            return GL_LINES;
        else if (typeString == "triangles")
            return GL_TRIANGLES;
        else if (typeString == "triangleStrip")
            return GL_TRIANGLE_STRIP;
        else if (typeString == "triangleFan")
            return GL_TRIANGLE_FAN;
        else if (typeString == "quads")
            return GL_QUADS;
        else if (typeString == "triangleFan")
            return GL_TRIANGLE_FAN;
        else
            throw InvalidArgumentException("Invalid primitive type: [%s]",
                    typeString.c_str());
    }

    GLenum asShaderType(const String &mimeType) const {
    	if(mimeType == MimeTypes::VERTEXSHADER) {
    		return GL_VERTEX_SHADER;
    	} else if(mimeType == MimeTypes::FRAGMENTSHADER) {
    		return GL_FRAGMENT_SHADER;
    	} else if(mimeType == MimeTypes::FRAGMENTSHADER) {
    		return GL_GEOMETRY_SHADER;
    	} else if(mimeType == MimeTypes::GEOMETRYSHADER) {
    		return GL_FRAGMENT_SHADER;
    	} else {
            throw InvalidArgumentException("Invalid primitive type: [%s]", mimeType.c_str());
    	}
    }

    GLenum getLocation(const String &faceName) const {
        if(faceName == "top") {
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        } else if(faceName == "bottom") {
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        } else if(faceName == "right") {
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        } else if(faceName == "left") {
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        } else if(faceName == "front") {
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        } else if(faceName == "back") {
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        } else {
            throw InvalidArgumentException("Invalid face name[%s]", faceName.c_str());
        }
    }

	String getInfoLog(GLuint object) const
	{
		GLint log_length = 0;
		char *log;

		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
		log = new char[log_length];
		glGetProgramInfoLog(object, log_length, null, log);
		String response(log);
		delete log;

		return response;
	}
};



#endif /* SRC_OPENGL_ADAPTERS_OPENGLRESOURCEADAPTER_H_ */
