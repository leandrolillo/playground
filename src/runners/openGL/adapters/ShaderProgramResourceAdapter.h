/*
 * ShaderProgramResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERPROGRAMRESOURCEADAPTER_H_
#define SHADERPROGRAMRESOURCEADAPTER_H_


/*
 * ShaderResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#include <map>
#include <string>

#include "../../../playground/parser/JsonParser.h"
#include "../../runners/openGL/adapters/OpenGLResourceAdapter.h"
#include "../../runners/video/resources/ShaderProgramResource.h"


class ShaderProgramResourceAdapter: public OpenGLResourceAdapter {
	private:
		std::map<std::string, std::string> shadersMimeTypes;
	public:

		ShaderProgramResourceAdapter() {
			logger = LoggerFactory::getLogger("video/ShaderProgramResourceAdapter");
			this->produces(MimeTypes::SHADERPROGRAM);
			this->accepts(MimeTypes::JSON);

			shadersMimeTypes["vertexShaders"] = MimeTypes::VERTEXSHADER;
			shadersMimeTypes["fragmentShaders"] = MimeTypes::FRAGMENTSHADER;
			shadersMimeTypes["geometryShaders"] = MimeTypes::GEOMETRYSHADER;
			shadersMimeTypes["tesellationShaders"] = MimeTypes::TESELLATIONSHADER;
		}

		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			GLenum glError = glGetError();

			JsonParser parser(request.getFileParser());

			ShaderProgramResource *resource = null;

			unsigned int programId = glCreateProgram();
			resource = new ShaderProgramResource(programId);

			String token;
			parser.readStartObject();
			while((token = parser.readToken()) != END_OBJECT && token != FileParser::eof)
			{
				if(token == "vertexShaders" || token == "fragmentShaders" || token == "geometryShaders" || token == "tesellationShaders") {
					parser.readValueSeparator();
					std::vector<String> vertexShadersFiles = parser.readStringArray();

					for(std::vector<String>::iterator stringIterator = vertexShadersFiles.begin(); stringIterator != vertexShadersFiles.end(); stringIterator++) {
						ShaderResource *shader = (ShaderResource *)getResourceManager()->load(request.getFilePath(), *stringIterator, shadersMimeTypes.at(token));
						if(shader != null)
							resource->getShaders().push_back(shader);
						else
							logger->error("Could not load shader [%s]", (*stringIterator).c_str());
					}

		            if (parser.peekToken() == ",") {
		                parser.readToken();
		            }

				} else {
					logger->warn("Unexpected token: %s", token.c_str());
				}
			}

			for(std::vector<ShaderResource *>::iterator shaderIterator = resource->getShaders().begin(); shaderIterator != resource->getShaders().end(); shaderIterator++)
				glAttachShader(resource->getId(), (*shaderIterator)->getId());

			glBindAttribLocation(resource->getId(), VERTEX_LOCATION, "position");
			glBindAttribLocation(resource->getId(), INDEX_LOCATION, "index");
			glBindAttribLocation(resource->getId(), NORMAL_LOCATION, "normal");
			glBindAttribLocation(resource->getId(), TEXTURE_COORDINATES_LOCATION, "textureCoordinate");
			glBindAttribLocation(resource->getId(), COLOR_LOCATION, "color");

			glLinkProgram(resource->getId());

			int operationSuccessful = 0;
			glGetProgramiv(resource->getId(), GL_LINK_STATUS, &operationSuccessful);
			if (!operationSuccessful) {
				logger->error("Failed to link program - [%d]: [%s]\n", operationSuccessful, getInfoLog(resource->getId()).c_str());
				dispose(resource);
				return;
			}

			Resource *vertexArray = this->getResourceManager()->load("core/sphere.json", MimeTypes::VERTEXARRAY);
			if(vertexArray) {
				glBindVertexArray(vertexArray->getId());
				glValidateProgram(resource->getId());
				glBindVertexArray(0);
				glGetProgramiv(resource->getId(), GL_VALIDATE_STATUS, &operationSuccessful);
				if (!operationSuccessful) {
					logger->error("Failed to validate program - [%d]: [%s]\n", operationSuccessful, getInfoLog(resource->getId()).c_str());
					dispose(resource);
					return;
				}
			}

			logger->debug("Shader executable [%s] successfully linked and validated", request.getFilePath().c_str());

			for(std::vector<ShaderResource *>::iterator shaderIterator = resource->getShaders().begin(); shaderIterator != resource->getShaders().end(); shaderIterator++)
				glDetachShader(resource->getId(), (*shaderIterator)->getId());

			glError = glGetError();
			if(glError != GL_NO_ERROR) {
				logger->error("Error loading texture [%s]: 0x[%x]", request.getFilePath().c_str(), glError);
				dispose(resource);
				return;
			}

			glUseProgram(resource->getId());
			logger->verbose("vertex attrib location = [%d]", glGetAttribLocation(resource->getId(), "vertex"));
			logger->verbose("index location = [%d]", glGetAttribLocation(resource->getId(), "index"));
			logger->verbose("normal attrib location = [%d]", glGetAttribLocation(resource->getId(), "normal"));
			logger->verbose("textureCoordinate attrib location = [%d]", glGetAttribLocation(resource->getId(), "textureCoordinate"));
			logger->verbose("color attrib location = [%d]", glGetAttribLocation(resource->getId(), "color"));


			response.addResource(resource);
		}

		virtual void dispose(Resource *resource) const override {
			ShaderProgramResource *shaderProgramResource = (ShaderProgramResource *)resource;
			if(shaderProgramResource->getId() != 0) {
				glDeleteProgram(shaderProgramResource->getId());
				shaderProgramResource->setId(0);
			}
		}
};

#endif /* SHADERPROGRAMRESOURCEADAPTER_H_ */
