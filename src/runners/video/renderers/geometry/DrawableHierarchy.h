/*
 * RenderableAABB.h
 *
 *  Created on: Jul 8, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEHIERARCHY_H_
#define SRC_RUNNERS_VIDEO_RENDERERS_GEOMETRY_DRAWABLEHIERARCHY_H_

#include<Geometry.h>
#include "../../../runners/video/renderers/DefaultRenderer.h"

class DrawableHierarchy: public HierarchicalGeometry, public Drawable {
    MaterialResource defaultMaterial = {vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), 1.0, 0.3};
public:
    DrawableHierarchy(Geometry *boundingVolume) : HierarchicalGeometry(boundingVolume) {
    }

    void draw(DefaultRenderer &renderer) const override {
        vector halfSizes = ((AABB &)this->getBoundingVolume()).getHalfSizes();

        const MaterialResource *material = renderer.getCurrentMaterial();

        for(auto &child : this->getChildren()) {
            renderer.drawSphere(matriz::traslacion(child->getOrigin()), ((Sphere &)*child.get()).getRadius());
        }

        renderer.setMaterial(&defaultMaterial);
        renderer.drawBox(matriz::traslacion(this->getOrigin()),
                (real)2 * halfSizes.x,
                (real)2 * halfSizes.y,
                (real)2 * halfSizes.z);
        renderer.setMaterial(material);
    }
};

#endif /* SRC_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_ */
