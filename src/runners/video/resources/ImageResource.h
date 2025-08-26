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
  Logger *logger = LoggerFactory::getLogger("video/image");
  unsigned int alto = 0;
  unsigned int ancho = 0;
  unsigned char bpp = 0;
  unsigned char format = 0; //future storage of rgba, rgb, bgr, bgra, etc.
  unsigned char *data = null;

public:
  //Inherit constructors
  using Resource::Resource;
  ImageResource() : Resource(0, MimeTypes::IMAGE) {
  }

  /*
   * Rule of five and copy-and-swap
   */
  friend void swap(ImageResource &first, ImageResource &second) {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;

    Resource::swap(first, second);

    // by swapping the members of two objects, the two objects are effectively swapped
    swap(first.data, second.data);
    swap(first.alto, second.alto);
    swap(first.ancho, second.ancho);
    swap(first.bpp, second.bpp);
    swap(first.format, second.format);
  }

  ImageResource(const ImageResource &other) :
      Resource(other) {

    this->resize(other.alto, other.ancho, other.bpp);
    this->format = other.format;
    memcpy(data, other.data, this->getBufferSize());
  }

  ImageResource(ImageResource &&other) :
      Resource(other) {
    swap(*this, other);
  }

  ImageResource& operator =(ImageResource other) {
    swap(*this, other);
    return *this;
  }
  /*
   * End of Rule of five and copy-and-swap
   */


  ~ImageResource() {
    this->resize(0, 0, 0);
  }

  unsigned int getAlto() const {
    return alto;
  }

  unsigned int getAncho() const {
    return ancho;
  }

  unsigned char getBpp() const {
    return bpp;
  }

  unsigned char *resize(unsigned int alto, unsigned int ancho, unsigned char bpp, unsigned int bufferSize = 0)
  {
    if(bufferSize == 0) {
      bufferSize = getBufferSize(alto, ancho, bpp);
    }

    if(bufferSize < getBufferSize(alto, ancho, bpp)) {
      throw std::invalid_argument(StringFormatter::format("Image: Buffer size [%d] is smaller than dimensions [%d]x[%d]x[%d]bpp", bufferSize, alto, ancho, bpp));
    }

    if(this->data != null) { //TODO: maybe we should delete only if the requested buffer is bigger
      delete [] this->data;
    }

    this->alto = alto;
    this->ancho = ancho;
    this->bpp = bpp;
    this->data = new unsigned char[bufferSize];

    return this->data;
  }


  unsigned char* getData() const {
    return data;
  }

  void setData(unsigned char *data) {
    this->data = data;
  }

  unsigned int getBufferSize() {
    return getBufferSize(alto, ancho, bpp);
  }

  //TODO: Should be a vector3 of integers instead of reals.
  vector getPixel(unsigned int x, unsigned int y) {
    if (x < this->getAlto() && y <= this->getAncho()) {
      unsigned int position = (y * this->getAncho() + x) * asBytespp(bpp);

      return vector(((unsigned char*) this->data)[position],
          ((unsigned char*) this->data)[position + 1],
          ((unsigned char*) this->data)[position + 2]);
    }

    return vector(0, 0, 0);
  }

  virtual String toString() const {
    return "Image(id:" + std::to_string(this->getId()) + ") ["
        + this->getMimeType() + "] [" + this->getUri() + "]: ["
        + std::to_string(this->getAlto()) + "x"
        + std::to_string(this->getAlto()) + "x" + std::to_string(this->getBpp())
        + "bpp]";
  }

private:
  unsigned char asBytespp(unsigned char bpp) const {
    /**
     * bits divided by 8
     */
    return bpp >> 3;
  }

  unsigned int getBufferSize(unsigned int alto, unsigned int ancho, unsigned char bpp) {
    return alto * ancho * asBytespp(bpp);
  }

};
