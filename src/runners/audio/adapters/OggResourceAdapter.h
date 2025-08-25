/*
 * OggResourceAdapter.h
 *
 *  Created on: 15/01/2013
 *      Author: Lean
 */

#ifndef OGGRESOURCEADAPTER_H_ //disabled momentarily. Please try again later.
#define OGGRESOURCEADAPTER_H_

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <stdlib.h>

#include "ResourceAdapter.h"
#include "AudioResource.h"

class OggResourceAdapter : public ResourceAdapter {
	private:
		Logger *logger;
	public:
		OggResourceAdapter() {
			logger = LoggerFactory::getLogger("audio/OggResourceAdapter.h");
			this->produces(MimeTypes::AUDIO);
			this->accepts(MimeTypes::OGG);
		}

		const unsigned int OGG_BUFFER_SIZE = 4096 * 4;
		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			OggVorbis_File oggStream;
			vorbis_info *vorbisInfo;
			vorbis_comment *vorbisComment;

			if(ov_open(request.getFileParser().getStream(), &oggStream, NULL, 0)  < 0) {
				logger->error("Error opening ogg stream [%s] - file not found or not a valid ogg", Paths::absolute(request.getFilePath()).c_str());
				return;
			}

			vorbisInfo = ov_info(&oggStream, -1);
			vorbisComment = ov_comment(&oggStream, -1);

			logger->debug("Vorbis file info:");
			for(int index = 0; index < vorbisComment->comments; index++)
				logger->debug("	%s", vorbisComment->user_comments[index]);

			AudioResource * resource = new AudioResource();
			resource->setFormat(vorbisInfo->channels == 1 ? AudioFormat::MONO16 : AudioFormat::STEREO16);
			resource->setFrequency(vorbisInfo->rate);

			char array[OGG_BUFFER_SIZE];
			int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
			int word_size = 2;
			int isSigned = 1;
			int bitStream = 1;
			std::vector <char>bufferData;
			long bytes;

			while ( (bytes = ov_read(&oggStream, array, OGG_BUFFER_SIZE, endian, word_size, isSigned, &bitStream)) > 0) {
				bufferData.insert(bufferData.end(), array, array + bytes);
			}

			logger->debug("Read %d bytes", bufferData.size());

			ov_clear(&oggStream);

			if(bytes < 0) {
				logger->error("Error reading ogg file [%s] data", request.getFilePath().c_str());
				return;
			}

			resource->setData(bufferData);
			response.addResource(resource);
		}
};



#endif /* OGGRESOURCEADAPTER_H_ */

