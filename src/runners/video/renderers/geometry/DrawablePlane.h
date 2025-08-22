/*
 * RenderableAABB.h
 *
 *  Created on: Jul 8, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEPLANE_H_
#define SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEPLANE_H_

#include<Geometry.h>
#include "../../../runners/video/renderers/DefaultRenderer.h"

class DrawablePlane: public Plane, public Drawable {
public:
    DrawablePlane(const vector &origin, const vector &normal) : Plane(origin, normal) {
    }

    void draw(DefaultRenderer &renderer) const override {
//        renderer.drawBox(matriz::matrizTraslacion(this->getOrigin()),
//                (real)2 * this->getHalfSizes().x,
//                (real)2 * this->getHalfSizes().y,
//                (real)2 * this->getHalfSizes().z);
    }
};

#endif /* SRC_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_ */
