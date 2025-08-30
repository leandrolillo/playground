/*
 * PngResourceAdapter.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#ifndef PNGRESOURCEADAPTER_H_
#define PNGRESOURCEADAPTER_H_

#include <png.h>
#include <stdio.h>

#include "ResourceAdapter.h"
#include "ImageResource.h"

class PngResourceAdapter : public ResourceAdapter {
		public:
		PngResourceAdapter() {
			logger = LoggerFactory::getLogger("video/PngResourceAdapter");
			this->accepts(MimeTypes::PNG);
			this->produces(MimeTypes::IMAGE);
		}

		virtual void load(ResourceLoadRequest &request, ResourceLoadResponse &response) const override {
			unsigned char *pBitmap;
			unsigned int width, height;
			int  bit_depth;

			png_structp png_ptr;
			png_infop info_ptr;
			int channels, color_type;

			unsigned char sig[8];
			request.getFileParser().read(sig, 1, 8);

			if (!png_check_sig(sig, 8)) {
				logger->error( "Not a valid png file");
				return;   /* bad signature */
			}

			/* could pass pointers to user-defined error handlers instead of NULLs: */
			if(!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, null, null, null))) {
				logger->error( "could not create read struct: out of memory");
				return;
			}

			if(!(info_ptr = png_create_info_struct(png_ptr))) {
				png_destroy_read_struct(&png_ptr, null, null);
				logger->error( "Could not create info struct: out of memory");
				return;
			}

			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_read_struct(&png_ptr, &info_ptr, null);
				return;
			}

			png_init_io(png_ptr, request.getFileParser().getStream());
			png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */

			png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
					(int *)null, (int *)null, (int *)null);

			logger->verbose("Png properties :\n	Width [%d]\n	Height [%d]\n	Bpp [%d]\n", width, height, bit_depth);

			//double  gamma;
			png_uint_32  i, image_rowbytes;
			png_bytepp  row_pointers = null;
			unsigned char  *image_data = null;

			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_read_struct(&png_ptr, &info_ptr, null);
				return;
			}

			if (color_type == PNG_COLOR_TYPE_PALETTE || (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) || png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
				png_set_expand(png_ptr);
			}

			if (bit_depth == 16) {
				png_set_strip_16(png_ptr);
			}

			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
				png_set_gray_to_rgb(png_ptr);
			}

			png_read_update_info(png_ptr, info_ptr);

			image_rowbytes = png_get_rowbytes(png_ptr, info_ptr);
			channels = (int)png_get_channels(png_ptr, info_ptr);

			if ((image_data =  new unsigned char [image_rowbytes * height]) == null) {
				png_destroy_read_struct(&png_ptr, &info_ptr, null);
				logger->error("Could not create image data: out of memory");
				return;
			}
			if ((row_pointers = new png_bytep[height]) == null) {
				png_destroy_read_struct(&png_ptr, &info_ptr, null);
				logger->error("Could not create row pointers: out of memory");
				delete [] image_data;
				return;
			}

			for (i = 0;  i < height; i++)
				row_pointers[i] = image_data + i * image_rowbytes;

			png_read_image(png_ptr, row_pointers);

			delete row_pointers;
			row_pointers = null;

			png_read_end(png_ptr, null);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

			unsigned long wimage_rowbytes = ((4 * width + 3L) >> 2) << 2;

			ImageResource *resource = new ImageResource();
      pBitmap = resource->resize(height, width, 32, wimage_rowbytes * height);

			if (!pBitmap) {
				logger->error("could not create bitmap pointer: out of memory");
				return;
			}

			if(channels == 3) {
				for(unsigned long y = 0; y < height; y++) {
					for(unsigned long x = 0; x < width; x++) {
						pBitmap[y * wimage_rowbytes + x * 4] = image_data[y * image_rowbytes + x * 3+ 2];
						pBitmap[y * wimage_rowbytes + x * 4 + 1] = image_data[y * image_rowbytes + x * 3 + 1];
						pBitmap[y * wimage_rowbytes + x * 4 + 2] = image_data[y * image_rowbytes + x * 3];
						pBitmap[y * wimage_rowbytes + x * 4 + 3] = 255;
					}
				}
			}
			else {
				for(unsigned long y = 0; y < height; y++) {
					for(unsigned long x = 0; x < width; x++) {
						pBitmap[y * wimage_rowbytes + x * 4] = image_data[y * image_rowbytes + x * 4 + 2];
						pBitmap[y * wimage_rowbytes + x * 4 + 1] = image_data[y * image_rowbytes + x * 4 + 1];
						pBitmap[y * wimage_rowbytes + x * 4 + 2] = image_data[y * image_rowbytes + x * 4];
						pBitmap[y * wimage_rowbytes + x * 4 + 3] = image_data[y * image_rowbytes + x * 4 + 3];
					}
				}
			}

			bit_depth = 32;
			delete image_data;

			response.addResource(resource);
		}
	};

#endif /* PNGRESOURCEADAPTER_H_ */
