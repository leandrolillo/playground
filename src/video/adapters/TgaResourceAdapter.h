/*
 * TgaResourceAdapter.h
 *
 *  Created on: 19/02/2013
 *      Author: Lean
 */

#ifndef TGARESOURCEADAPTER_H_
#define TGARESOURCEADAPTER_H_

#include <stdio.h>
#include <ResourceAdapter.h>
#include <resources/ImageResource.h>

class TgaResourceAdapter: public ResourceAdapter {
private:
	short le_short(unsigned char *bytes) const {
		return bytes[0] | ((char) bytes[1] << 8);
	}
public:
	TgaResourceAdapter() {
		logger = LoggerFactory::getLogger("video/TgaResourceAdapter");
		this->accepts(MimeTypes::TGA);
		this->produces(MimeTypes::IMAGE);
	}

	virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
		ImageResource *resource = new ImageResource(0, "image/tga");

		struct tgaHeader {
			char id_length;
			char color_map_type;
			char data_type_code;
			unsigned char color_map_origin[2];
			unsigned char color_map_length[2];
			char color_map_depth;
			unsigned char x_origin[2];
			unsigned char y_origin[2];
			unsigned char width[2];
			unsigned char height[2];
			char bits_per_pixel;
			char image_descriptor;
		} header;

		if (request.getFileParser().read(&header, sizeof(header), 1) != 1) {
			logger->error("%s has incomplete tga header\n",
					request.getFilePath().c_str());
			return;
		}
		if (header.data_type_code != 2) {
			logger->error("[%s] compressed tga not supported\n",
					request.getFilePath().c_str());
			return;
		}
		if (header.bits_per_pixel != 24) {
			logger->error("%s is not a 24-bit uncompressed RGB tga file\n",
					request.getFilePath().c_str());
			return;
		}

		for (int i = 0; i < header.id_length; ++i)
			if (request.getFileParser().takeByte() == EOF) {
				logger->error("%s has incomplete id string\n",
						request.getFilePath().c_str());
				return;
			}

		unsigned int color_map_size = le_short(header.color_map_length)
				* (header.color_map_depth / 8);
		for (unsigned int i = 0; i < color_map_size; ++i)
			if (request.getFileParser().takeByte() == EOF) {
				logger->error("%s has incomplete color map\n",
						request.getFilePath().c_str());
				return;
			}

		resource->setAncho(le_short(header.width));
		resource->setAlto(le_short(header.height));
		resource->setBpp(header.bits_per_pixel >> 3);

		unsigned int size = resource->getAncho() * resource->getAlto() * resource->getBpp();
		resource->setData(new char[size]);

		if (request.getFileParser().read(resource->getData(), 1, size) != size) {
			logger->error("%s has incomplete image\n",
					request.getFilePath().c_str());
			dispose(resource);
			return;
		}

		response.addResource(resource);
	}
};
#endif /* TGARESOURCEADAPTER_H_ */
