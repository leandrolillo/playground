/*
 * NormalGenerator.h
 *
 *  Created on: 18/03/2013
 *      Author: Lean
 */

#ifndef NORMALGENERATOR_H_
#define NORMALGENERATOR_H_

#include <MathUtilities.h>
#include "Logger.h"
#include "GeometryResource.h"


class NormalGenerator
{
	public:
		virtual void generateNormals(GeometryResource *resource) const = 0;

	protected:
	virtual void compressVertices(const std::vector<vector3> &unCompressed, std::vector<vector3> &compressed, std::vector<unsigned int> &indices) const
	{
		for(auto &source : unCompressed) {
			unsigned int index = 0;
			bool preExisting = false;
			for(auto &destination : compressed)
			{
				if(source == destination) {
					indices.push_back(index);
					preExisting = true;
					break;
				}
				index++;
			}
			if(!preExisting) {
				compressed.push_back(source);
				indices.push_back(compressed.size() - 1);
			}
		}
	}
};
class PerVertexGLTrianglesNormalGenerator : public NormalGenerator
{
	private:
		Logger *logger;
	public:
		PerVertexGLTrianglesNormalGenerator()
		{
			logger = LoggerFactory::getLogger("video/normalGenerator/PerVertexGLTrianglesNormalGenerator.h");
		}
		void generateNormals(GeometryResource *resource) const override
		{
			if(!resource->getVertices().empty())
			{
				std::vector<vector3> normals;
				std::vector<vector3> vertices;
				std::vector<unsigned int> indices;

				if(resource->getIndices().empty()) {
				    this->compressVertices(resource->getVertices(), vertices, indices);
				} else {
				    vertices = resource->getVertices();
				    indices = resource->getIndices();
				}

				logger->debug("Generating normals PER VERTEX / GL_TRIANGLES");

//				log("Vertices original = ", resource->getVertices());
//				log("Vertices compressed = ", vertices);
//				log("Indices = ", indices);

				std::vector<unsigned int> divisor;
				for(std::vector<vector3>::iterator iterator = vertices.begin(); iterator != vertices.end(); iterator++)
				{
					divisor.push_back(0);
					normals.push_back(vector(0, 0 ,0));
				}

				for(unsigned int currentVertexIndex = 0; currentVertexIndex < indices.size(); currentVertexIndex+=3)
				{
					vector tangente2 = vertices[indices[currentVertexIndex + 2]] - vertices[indices[currentVertexIndex]];
					vector tangente1 = vertices[indices[currentVertexIndex + 1]] - vertices[indices[currentVertexIndex]];
					vector normal = VectorUtilities::normalizar(tangente2 ^ tangente1);

//					logger->debug("Current Triangle: [%d], tangente1 = <%.2f, %.2f, %.2f>, tangente2=<%.2f, %.2f, %.2f>, normal=<%.2f, %.2f, %.2f>", currentVertexIndex,
//							tangente1.x, tangente1.y, tangente1.z,
//							tangente2.x, tangente2.y, tangente2.z,
//							normal.x, normal.y, normal.z);

					normals[indices[currentVertexIndex]] += normal;
					divisor[indices[currentVertexIndex]]++;
					normals[indices[currentVertexIndex + 1]] += normal;
					divisor[indices[currentVertexIndex + 1]]++;
					normals[indices[currentVertexIndex + 2]] += normal;
					divisor[indices[currentVertexIndex + 2]]++;
				}

//				log("Normals = ", normals);
//				log("divisors = ", divisor);

				for(unsigned int currentNormalIndex = 0; currentNormalIndex < normals.size(); currentNormalIndex++)
					normals[currentNormalIndex] =  VectorUtilities::normalizar(normals[currentNormalIndex] * (1.0f / divisor[currentNormalIndex]));

				resource->setNormals(std::vector<vector3> {}); //clear normals
				for(std::vector<unsigned int>::iterator iterator = indices.begin(); iterator != indices.end(); iterator++)
					resource->addNormal(normals[(*iterator)]);
			}
		}

	private:
		void log(String prefix, std::vector<vector3> array)
		{
			prefix.append("[");
			for(auto &current : array) {
				prefix.append(current.toString() + " ");
			}
			prefix.append(" ]");

			logger->debug(prefix.c_str());
		}

		void log(String prefix, std::vector<unsigned int> array)
		{
			prefix.append(" [");
			for(auto &current : array) {
				prefix.append(std::to_string(current) + " ");
			}
			prefix.append(" ]");

			logger->debug(prefix.c_str());

		}
};

class NormalGeneratorFactory
{
	public:
		static NormalGenerator *getGenerator(String primitiveType)
		{
			if(primitiveType == "triangles") {
					static PerVertexGLTrianglesNormalGenerator _perVertexGLTrianglesNormalGeneratorInstance;
					return &_perVertexGLTrianglesNormalGeneratorInstance;
			}

			return null;
		}
};



#endif /* NORMALGENERATOR_H_ */
