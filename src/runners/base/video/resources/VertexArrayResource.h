#pragma once

#include "Resource.h"
#include "VideoAttribute.h"

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
		VertexAttribPointer(unsigned int buffer, unsigned int start, unsigned int count, VideoAttribute bufferDestination, VideoAttribute bufferUsage)
		{
			this->buffer = buffer;
			this->start = start;
			this->count = count;
			this->bufferDestination = bufferDestination;
			this->bufferUsage = bufferUsage;
		}

		unsigned int getCount() const {
			return count;
		}

		unsigned int getBuffer() const {
			return buffer;
		}

		unsigned int getStart() const {
			return start;
		}

		VideoAttribute getBufferDestination() const {
		  return this->bufferDestination;
		}

    VideoAttribute getBufferUsage() const {
      return this->bufferUsage;
    }


		String toString() const {
			return "[buffer: " + std::to_string(this->buffer) + " start: " + std::to_string(this->start) + " count: " + std::to_string(this->count);
		}

	private:
		unsigned int buffer;
		unsigned int start;
		unsigned int count;
    VideoAttribute bufferDestination;
    VideoAttribute bufferUsage;
};

class VertexArrayResource : public Resource
{
	private:
		std::unordered_map<unsigned int, std::unique_ptr<VertexAttribPointer>> attributes;
		PrimitiveType primitiveType;
		vector size;
	public:
		VertexArrayResource(unsigned int id) : Resource(id, MimeTypes::VERTEXARRAY)
		{
			primitiveType = PrimitiveType::TRIANGLES;
		}

		/**
		 * stores vertex buffer in the next free vertexAttribPointer, and returns the pointer.
		 */
		void addAttribute(unsigned int location, unsigned int vertexBuffer, unsigned int start, unsigned int count, VideoAttribute bufferDestination, VideoAttribute bufferUsage)
		{
			attributes[location] = std::unique_ptr<VertexAttribPointer>(new VertexAttribPointer(vertexBuffer, start, count, bufferDestination, bufferUsage));
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

		PrimitiveType getPrimitiveType() const {
			return primitiveType;
		}

		void setPrimitiveType(PrimitiveType primitiveType) {
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
