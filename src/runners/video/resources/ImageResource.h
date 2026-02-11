/*
 * ImageResource.h
 *
 *  Created on: 25/01/2013
 *      Author: Lean
 */

#pragma once

#include <Math3d.h>
#include "Resource.h"
#include "MimeTypes.h"

class ImageResource: public Resource {
private:
  unsigned int height = 0;
  unsigned int width = 0;
  unsigned char bpp = 0; //bits per pixel
  unsigned char format = 0; //future storage of rgba, rgb, bgr, bgra, etc.
  unsigned char *data = null;

public:
  //Inherit constructors
  using Resource::Resource;
  ImageResource() : Resource(0, MimeTypes::IMAGE) {
    logger = LoggerFactory::getLogger("video/image");
  }

  ImageResource(unsigned int width, unsigned int height, unsigned char bpp) : ImageResource() {
    this->resize(width, height, bpp);
  }

  /*
   * Rule of five and copy-and-swap
   */
  friend void swap(ImageResource &left, ImageResource &right) {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;

    Resource::swap(left, right);

    // by swapping the members of two objects, the two objects are effectively swapped
    swap(left.data, right.data);
    swap(left.height, right.height);
    swap(left.width, right.width);
    swap(left.bpp, right.bpp);
    swap(left.format, right.format);
  }

  ImageResource(const ImageResource &right) :
      Resource(right) {

    this->resize(right.width, right.height, right.bpp);
    this->format = right.format;
    memcpy(data, right.data, this->getBufferSize());
  }

  ImageResource(ImageResource &&rvalue) :
      Resource(rvalue) {
    swap(*this, rvalue);
  }

  ImageResource& operator =(ImageResource right) {
    swap(*this, right);
    return *this;
  }
  /*
   * End of Rule of five and copy-and-swap
   */


  ~ImageResource() {
    this->resize(0, 0, 0);
  }

  unsigned int getHeight() const {
    return height;
  }

  unsigned int getWidth() const {
    return width;
  }

  unsigned char getBpp() const {
    return bpp;
  }

  unsigned char *resize(unsigned int width, unsigned int height, unsigned char bpp, unsigned int bufferSize = 0)
  {
    if(bufferSize == 0) {
      bufferSize = getBufferSize(width, height, bpp);
    }

    if(bufferSize < getBufferSize(width, height, bpp)) {
      throw std::invalid_argument(StringFormatter::format("Image: Buffer size [%d] is smaller than dimensions [%d]x[%d]x[%d]bpp", bufferSize, height, width, bpp));
    }

    if(this->data != null) { //TODO: maybe we should delete only if the requested buffer is bigger
      delete [] this->data;
    }

    this->height = height;
    this->width = width;
    this->bpp = bpp;
    this->data = new unsigned char[bufferSize];

    return this->data;
  }

  /*
   * Copies an image fragment (at sourceTopLeft and size sourceSize) into this image resource at the specified targetTopLeft location
   */
  void copy(unsigned int x, unsigned int y, const unsigned char *sourceData, unsigned int sourceWidth, unsigned int sourceHeight, unsigned int sourceBpp) {
    if(this->getBpp() == sourceBpp) {
      if(x < this->width && y < this->height) {
        auto targetWidth = std::min(sourceWidth, this->width - x);
        auto targetHeight = std::min(sourceHeight, this->height - y);

        for(unsigned int j = 0; j < targetHeight; j++) {
          auto destinationPosition = getOffset(x, y + j);
          auto sourcePosition = j * sourceWidth * asBytespp(sourceBpp);
          std::memcpy(this->data + destinationPosition, sourceData + sourcePosition, targetWidth * asBytespp(sourceBpp));
        }
      }
    }
  }


  unsigned char* getData() const {
    return data;
  }

  void setData(unsigned char *data) {
    this->data = data;
  }

  unsigned int getBufferSize() const {
    return getBufferSize(width, height, bpp);
  }

  unsigned int getOffset(unsigned int x, unsigned int y) const {
    return (y * this->getWidth() + x) * asBytespp(bpp);
  }

  //TODO: Should be a vector3 of integers instead of reals.
  vector getPixel(unsigned int x, unsigned int y) const {
    if (x < this->getWidth() && y < this->getHeight()) {
      unsigned int position = getOffset(x, y);

      return vector(((unsigned char*) this->data)[position],
          ((unsigned char*) this->data)[position + 1],
          ((unsigned char*) this->data)[position + 2]);
    }

    return vector(0, 0, 0);
  }

  virtual String toString() const {
    return "Image(id:" + std::to_string(this->getId()) + ") ["
        + this->getMimeType() + "] [" + this->getUri() + "]: ["
        + std::to_string(this->getWidth()) + "x"
        + std::to_string(this->getHeight()) + "x" + std::to_string(this->getBpp())
        + "bpp]";
  }

private:
  unsigned char asBytespp(unsigned char bpp) const {
    /**
     * bits divided by 8
     */
    return bpp >> 3;
  }

  unsigned int getBufferSize(unsigned int width, unsigned int height, unsigned char bpp) const {
    return height * width * asBytespp(bpp);
  }

};
