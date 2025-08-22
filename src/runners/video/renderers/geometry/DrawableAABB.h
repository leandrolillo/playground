/*
 * RenderableAABB.h
 *
 *  Created on: Jul 8, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_
#define SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_

#include<Geometry.h>
#include "../../../runners/video/renderers/DefaultRenderer.h"

class DrawableAABB: public AABB, public Drawable {
public:
    DrawableAABB(const vector &origin, const vector &halfSizes) : AABB(origin, halfSizes) {
    }

    void draw(DefaultRenderer &renderer) const override {
        renderer.drawBox(matriz::traslacion(this->getOrigin()),
                (real)2 * this->getHalfSizes().x,
                (real)2 * this->getHalfSizes().y,
                (real)2 * this->getHalfSizes().z);
    }
};

#endif /* SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_ */
