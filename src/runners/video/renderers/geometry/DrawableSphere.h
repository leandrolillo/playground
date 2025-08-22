/*
 * Renderable.h
 *
 *  Created on: Jul 8, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_RENDERABLE_H_
#define SRC_VIDEO_RENDERERS_RENDERABLE_H_

#include<Geometry.h>
#include "../../../runners/video/renderers/DefaultRenderer.h"

class DrawableSphere : public Sphere, public Drawable {
public:
    DrawableSphere(const vector &origin, real radius) : Sphere(origin, radius) {
    }

    void draw(DefaultRenderer &renderer) const override {
        renderer.drawSphere(matriz::traslacion(this->getOrigin()), this->getRadius());
    }
};



#endif /* SRC_VIDEO_RENDERERS_RENDERABLE_H_ */
