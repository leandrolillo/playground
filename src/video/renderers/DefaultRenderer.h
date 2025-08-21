/*
 * DefaultRenderer.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_
#define SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_

#include<Renderer.h>
#include<Camera.h>
#include<resources/MaterialResource.h>
#include<resources/LightResource.h>
#include<VideoRunner.h>

#include "../SceneNode.h"

#include<vector>

class WorldObject {
protected:
    matriz_4x4 modelMatrix;
    matriz_3x3 normalMatrix;
    const VertexArrayResource *object;
    const MaterialResource *material;
public:
    WorldObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object, const MaterialResource *material) {
        this->object = object;
        this->modelMatrix = modelMatrix;

        /**
         * Do not calculate inverse if determinant is zero - alternative to throwing the exception and stopping, specially for
         * drawLine functionality which generates singular matrixes.
         */
        matriz_3x3 reducedModelMatrix = (matriz_3x3) modelMatrix;
        real determinante = reducedModelMatrix.determinante();
        if(!equalsZero(determinante)) {
            this->normalMatrix = reducedModelMatrix.inversa(determinante).traspuesta();
        } else {
            this->normalMatrix = matriz_3x3::identidad;
        }
        this->material = material;
    }

    const matriz_4x4 &getModelMatrix() const {
        return this->modelMatrix;
    }

    const matriz_3x3 &getNormalMatrix() const {
        return this->normalMatrix;
    }

    const VertexArrayResource *getVertexArray() const {
        return this->object;
    }

    const MaterialResource *getMaterial() const {
        return this->material;
    }
};

class DefaultRenderer : public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("DefaultRenderer");

    const TextureResource *currentTexture = null;
    const MaterialResource *currentMaterial = null;
    const LightResource *light = null;

    std::map<const TextureResource *, std::vector<WorldObject>>objectsByTexture;
    std::map<const MeshResource *, std::vector<matriz_4x4>>objectsByMesh;

    const VertexArrayResource *axes = null;
    const VertexArrayResource *sphere = null;
    const VertexArrayResource *box = null;
    const VertexArrayResource *line = null;

    const MaterialResource defaultMaterial = MaterialResource(vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), 32);

public:
    void setMaterial(const MaterialResource *material) {
        this->currentMaterial = (material == null ? &defaultMaterial : material);
    }

    const MaterialResource *getCurrentMaterial() {
        return this->currentMaterial;
    }

    void setLight(const LightResource *light) {
        this->light = light;
    }

    void setTexture(const TextureResource *texture) {
        this->currentTexture = texture;
    }

    bool init() override {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource*) this->resourceManager->load("core/simple.program.json", MimeTypes::SHADERPROGRAM);
        }

        this->setMaterial(null);

        return true;
    }

    void render(const Camera &camera) override {
        if(isEnabled()) {
            videoRunner->useProgramResource(shader);
            videoRunner->sendVector("viewPosition", camera.getPosition());
            this->sendLight(light);

            const MaterialResource *lastMaterial = null;

            for(auto &iterator : this->objectsByMesh) {
            	const MeshResource *mesh = iterator.first;
            	auto &bases = iterator.second;
            	if(bases.size() > 0) {
            		videoRunner->setTexture(0, "textureUnit", mesh->getTexture() != null ? mesh->getTexture() : videoRunner->getDefaultTexture());
            		this->sendMaterial(mesh->getMaterial());

            		for (auto &base : bases) {
						videoRunner->sendMatrix("matrices.model", base);
						videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * base);

            			matriz_3x3 reducedModelMatrix = (matriz_3x3) base;
            			real determinante = reducedModelMatrix.determinante();

						videoRunner->sendMatrix("matrices.normal",
								!equalsZero(determinante) ?
										reducedModelMatrix.inversa(determinante).traspuesta() :
										matriz_3x3::identidad);
						videoRunner->drawVertexArray(mesh->getVertexArray());
            		}
            	}
            }

            for(auto &iterator : this->objectsByTexture)
            {
            	const TextureResource *texture = iterator.first;
            	auto &worldObjects = iterator.second;
            	if(worldObjects.size() > 0) {
					videoRunner->setTexture(0, "textureUnit", texture != null ? texture : videoRunner->getDefaultTexture());

					for (auto &object : worldObjects) {
						if(lastMaterial != object.getMaterial()) {
							lastMaterial = object.getMaterial();
							if(lastMaterial != null) {
								this->sendMaterial(lastMaterial);
							}
						}
						videoRunner->sendMatrix("matrices.model", object.getModelMatrix());
						videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * object.getModelMatrix());
						videoRunner->sendMatrix("matrices.normal", object.getNormalMatrix());
						videoRunner->drawVertexArray(object.getVertexArray());
					}
            	}
            }

            videoRunner->setTexture(0, null);
            videoRunner->useProgramResource(null);
        } else {
            logger->error("Not rendering! VideoRunner or Shader not set.");
        }
    }

    void drawObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object) {
    	if(object != null) {
    		this->objectsByTexture[this->currentTexture].push_back(WorldObject(modelMatrix, object, currentMaterial));
    	}
    }

    void drawObject(const matriz_4x4 &modelMatrix, const MeshResource *object) {
    	if(object != null) {
    		this->objectsByMesh[object].push_back(modelMatrix);
    	}
    }

    void drawObject(const SceneNode &object) {
    	this->objectsByMesh[&(object.getMesh())].push_back(object.getBase());
    }

    void drawAxes(const matriz_4x4 &modelMatrix, real length = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::zoom(length, length, length), getAxes());
    }

    void drawSphere(const matriz_4x4 &modelMatrix, real radius = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::zoom(radius, radius, radius), getSphere());
    }

    void drawBox(const matriz_4x4 &modelMatrix, real height = 1.0f, real width = 1.0f, real depth = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::zoom(height, width, depth), getBox());
    }

    void drawBox(const matriz_4x4 &modelMatrix, const vector &sizes) {
        this->drawBox(modelMatrix, sizes.x, sizes.y, sizes.z);
    }

    void drawLine(const matriz_4x4 &modelMatrix, const vector &start, const vector &end) {
        matriz_4x4 traslacion = matriz_4x4::traslacion(start);
        matriz_4x4 zoom = matriz_4x4::zoom(end - start);
        matriz_4x4 lineMatrix = modelMatrix * traslacion * zoom;
        this->drawObject(lineMatrix, getLine());
    }

    void clear() {
    	this->setTexture(videoRunner != null ? videoRunner->getDefaultTexture() : null);
        this->setMaterial(&defaultMaterial);
        this->objectsByTexture.clear();
        this->objectsByMesh.clear();
    }
private:
    const VertexArrayResource *getSphere() {
        if(this->sphere == null && resourceManager != null) {
            this->sphere = (VertexArrayResource*) this->resourceManager->load("core/sphere.json", MimeTypes::VERTEXARRAY);
        }

        return this->sphere;
    }

    const VertexArrayResource *getLine() {
        if(this->line == null && resourceManager != null) {
            this->line= (VertexArrayResource*) this->resourceManager->load("core/line.json", MimeTypes::VERTEXARRAY);
        }

        return this->line;
    }

    const VertexArrayResource *getAxes() {
        if(this->axes == null && resourceManager != null) {
            this->axes= (VertexArrayResource*) this->resourceManager->load("core/axis.json", MimeTypes::VERTEXARRAY);
        }

        return this->axes;
    }

    const VertexArrayResource *getBox() {
            if(this->box == null && resourceManager != null) {
                this->box= (VertexArrayResource*) this->resourceManager->load("core/box.json", MimeTypes::VERTEXARRAY);
            }

            return this->box;
        }
};


class Drawable {
public:
    virtual ~Drawable() {

    }
    virtual void draw(DefaultRenderer &renderer) const = 0;
};


#endif /* SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_ */
