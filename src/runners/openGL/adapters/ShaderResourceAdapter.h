/*
 * ShaderResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERRESOURCEADAPTER_H_
#define SHADERRESOURCEADAPTER_H_
#include "OpenGLResourceAdapter.h"
#include "ShaderResource.h"

class ShaderResourceAdapter: public OpenGLResourceAdapter {
	public:
		ShaderResourceAdapter() {
			this->accepts(MimeTypes::GLSL);
		}

		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			ShaderResource *resource = null;

			int shaderSize = request.getFileParser().size();
			char *shaderCode = new char[shaderSize + 1];

			if(request.getFileParser().read(shaderCode, sizeof(char), shaderSize) != (unsigned int)shaderSize)
			{
				logger->error("Error reading shader source [%s]", request.getFilePath().c_str());
				return;
			}
			shaderCode[request.getFileParser().size()] = '\0';

			unsigned int shaderId = glCreateShader(asShaderType(request.getOutputMimeType()));
			resource = new ShaderResource(shaderId);

			glShaderSource(resource->getId(), 1, (const char**)&shaderCode, &shaderSize);

			glCompileShader(resource->getId());

			int compilationSuccessfull = 0;
			glGetShaderiv(resource->getId(), GL_COMPILE_STATUS, &compilationSuccessfull);
			if (!compilationSuccessfull) {
				logger->error("Failed to compile [%s] [%s]: [%s]\n", request.getOutputMimeType().c_str(), request.getFilePath().c_str(), getInfoLog(shaderId).c_str());
				logger->debug("Shader code: \n%s", shaderCode);
				dispose(resource);
				return;
			} else {
				logger->debug("[%s] [%s] compilation successful.", request.getFilePath().c_str(), request.getOutputMimeType().c_str());
			}

			response.addResource(resource);
		}

		virtual void dispose(Resource *resource) const override {
			ShaderResource *shaderResource = (ShaderResource *)resource;
			if(shaderResource != null && shaderResource->getId() != 0) {
				glDeleteShader(shaderResource->getId());
				shaderResource->setId(0);
			}
		}
	private:
		String getInfoLog(GLuint object) const
		{
			GLint log_length;
			char *log;

			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
			log = new char[log_length];
			glGetShaderInfoLog(object, log_length, null, log);
			String response(log);
			delete log;

			return response;
		}

};
#endif /* SHADERRESOURCEADAPTER_H_ */
