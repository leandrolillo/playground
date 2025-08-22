/*
 * TerrainRenderer.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_RUNNERS_VIDEO_RENDERERS_TERRAINRENDERER_H_
#define SRC_RUNNERS_VIDEO_RENDERERS_TERRAINRENDERER_H_

#include "../../runners/video/resources/LightResource.h"
#include "../../runners/video/resources/ShaderProgramResource.h"
#include "../../runners/video/resources/TerrainResource.h"
#include "../../runners/video/VideoRunner.h"

class TerrainTile {
protected:
    matriz_4x4 position;
    const TerrainResource *terrain;
public:
    TerrainTile(const vector &posicion, const TerrainResource *terrain) {
        this->terrain = terrain;
        this->position = matriz_4x4::traslacion(posicion);
    }

    const matriz_4x4 &getModelMatrix() const {
        return this->position;
    }

    const TerrainResource* getTerrain() const {
        return this->terrain;
    }

};
class TerrainRenderer : public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("TerrainRenderer");

	const LightResource *light = null;

	std::vector<TerrainTile>terrainTiles;

public:
	void addTerrain(const vector &position, const TerrainResource *terrain) {
		this->terrainTiles.push_back(TerrainTile(position, terrain));
	}

	void setLight(const LightResource *light) {
	    this->light = light;
	}

    bool init() override {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource *)resourceManager->load("shaders/terrain/terrain.program.json", MimeTypes::SHADERPROGRAM);
        }

        return true;
    }

    bool isEnabled() const override {
    	return Renderer::isEnabled() && !terrainTiles.empty();
    }

	void render(const Camera &camera) override {
	    if(isEnabled()) {
            videoRunner->useProgramResource(shader);

            this->sendLight(light);
            for(const auto &terrainTile : terrainTiles) {
                videoRunner->setTexture(0, "background", terrainTile.getTerrain()->getA());
                videoRunner->setTexture(1, "textureR", terrainTile.getTerrain()->getR());
                videoRunner->setTexture(2, "textureG", terrainTile.getTerrain()->getG());
                videoRunner->setTexture(3, "textureB",terrainTile.getTerrain()->getB());
                videoRunner->setTexture(4, "blendMap", terrainTile.getTerrain()->getMap());

                videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * terrainTile.getModelMatrix());
//                videoRunner->sendMatrix("matrices.model", terrainTile.getModelMatrix());
                videoRunner->sendMatrix("matrices.normal", matriz_3x3::identidad);

                logger->debug("Drawing terrain at\n%s", terrainTile.getModelMatrix().toString().c_str());
                videoRunner->drawVertexArray(terrainTile.getTerrain()->getModel());
            }


            videoRunner->setTexture(0, "background", null);
            videoRunner->setTexture(1, "textureR", null);
            videoRunner->setTexture(2, "textureG", null);
            videoRunner->setTexture(3, "textureB", null);
            videoRunner->setTexture(4, "blendMap", null);

            videoRunner->useProgramResource(null);
	    } else {
	        logger->error("Not rendering! shader or terrain not set.");

	    }
	}
};



#endif /* SRC_RUNNERS_VIDEO_RENDERERS_TERRAINRENDERER_H_ */
