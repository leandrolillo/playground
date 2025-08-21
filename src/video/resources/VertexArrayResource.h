/*
 * BufferResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERRESOURCE_H_
#define VERTEXBUFFERRESOURCE_H_

#include <Resource.h>

enum ShaderAttributeLocation
{
	VERTEX_LOCATION = 0,
	INDEX_LOCATION,
	NORMAL_LOCATION,
	TEXTURE_COORDINATES_LOCATION,
	COLOR_LOCATION
};


class VertexAttribPointer {
//	Logger *logger = LoggerFactory::getLogger("video/VertexAttribPointer");

	public:
		VertexAttribPointer(unsigned int buffer, unsigned int start, unsigned int count)
		{
			this->buffer = buffer;
			this->start = start;
			this->count = count;
		}
		VertexAttribPointer()
		{
			this->buffer = -1;
			this->start = 0;
			this->count = 0;
		}

//		~VertexAttribPointer() {
//			logger->info("Deleting VertexAttribPointer");
//		}

		unsigned int getCount() const {
			return count;
		}

		void setCount(unsigned int count) {
			this->count = count;
		}

		unsigned int getBuffer() const {
			return buffer;
		}

		void setBuffer(unsigned int pointer) {
			this->buffer = pointer;
		}

		unsigned int getStart() const {
			return start;
		}

		void setStart(unsigned int start) {
			this->start = start;
		}

		String toString() const {
			return "[buffer: " + std::to_string(this->buffer) + " start: " + std::to_string(this->start) + " count: " + std::to_string(this->count);
		}

	private:
		unsigned int buffer;
		unsigned int start;
		unsigned int count;
};

class VertexArrayResource : public Resource
{
	private:
		std::unordered_map<unsigned int, std::unique_ptr<VertexAttribPointer>> attributes;
		unsigned int primitiveType;
		vector size;
	public:
		VertexArrayResource(unsigned int id) : Resource(id, MimeTypes::VERTEXARRAY)
		{
			primitiveType = GL_TRIANGLES;
		}

		/**
		 * stores vertex buffer in the next free vertexAttribPointer, and returns the pointer.
		 */
		void addAttribute(unsigned int location, unsigned int vertexBuffer, unsigned int start, unsigned int count)
		{
			attributes[location] = std::unique_ptr<VertexAttribPointer>(new VertexAttribPointer(vertexBuffer, start, count));
		}

		const VertexAttribPointer *getAttribute(unsigned int location) const {
			auto iterator = attributes.find(location);
			if(iterator != attributes.end())
				return iterator->second.get();

			return null;
		}

		std::unordered_map<unsigned int, std::unique_ptr<VertexAttribPointer>> &getAttributes() {
			return attributes;
		}

		void clearVertexAttribPointers()
		{
			attributes.clear();
		}

		unsigned int getPrimitiveType() const {
			return primitiveType;
		}

		void setPrimitiveType(unsigned int primitiveType) {
			this->primitiveType = primitiveType;
		}

		void setSize(const vector &size) {
			this->size = size;
		}

		vector getSize() const {
			return this->size;
		}

		String toString() const {
			String result = Resource::toString();

//			for(auto attribute : this->attributes) {
//				result += "location = " + std::to_string(attribute.first) + ": " + attribute.second.toString().c_str() + "\n";
//			}

			return result;
		}
};



#endif /* BUFFERRESOURCE_H_ */
