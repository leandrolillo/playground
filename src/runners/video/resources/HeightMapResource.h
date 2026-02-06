/*
 * HeightmapResource.h
 *
 *  Created on: Jun 15, 2021
 *      Author: leandro
 */

#pragma once

#include <Math3d.h>
#include "Logger.h"
#include "Resource.h"
#include "ImageResource.h"
#include "Geometry.h"

constexpr real one_over_max_color = 0.000000059604645; // 1.0 / (256.0 * 256.0 * 256.0)

class HeightMapResource: public Resource, public HeightMap {
private:
    vector voxelSize;
    ImageResource *heightMap;
    real height;

public:
    /**
     * VoxelSize can be interpreted as the dimensions of each and all individual voxels, or the factor/zoom applied to enlarge the original image dimensions
     */
    HeightMapResource(ImageResource *heightMap, vector voxelSize) : Resource(0, MimeTypes::HEIGHTMAP) {
      logger = LoggerFactory::getLogger("video/HeightMapResource");

    	if(heightMap == null) {
    		throw std::invalid_argument("Heightmap can not be null");
    	}
        this->heightMap = heightMap;
        this->voxelSize = voxelSize;
        this->height = calculateHeight();
    }

    // returns 3D generated width
    real getWidth() const {
        return (real) (this->heightMap->getWidth() - 1) * voxelSize.x;
    }

    // returns 3D generated height
    real getHeight() const {
        //return 2 * voxelSize.y;
    	return height;
    }

    // returns 3D generated depth
    real getDepth() const {
        return (real) (this->heightMap->getHeight() - 1) * voxelSize.z;
    }

    const vector &getVoxelSize() const {
    	return this->voxelSize;
    }

    // returns 2D grid width
    unsigned int getGridWidth() const {
        return this->heightMap->getWidth();
    }

    // returns 2D grid height
    unsigned int getGridHeight() const {
        return this->heightMap->getHeight();
    }

    unsigned int getI(real x) const {
    	return std::max(0, std::min((int)floor(x / voxelSize.x), (int)this->getGridWidth()));
    }

    unsigned int getJ(real z) const {
        return std::max(0, std::min((int)floor(z / voxelSize.z), (int)this->getGridHeight()));
    }

    vector positionAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        return vector((real) i * voxelSize.x, heightAtGrid(i, j) * voxelSize.y, (real) j * voxelSize.z);
    }

    vector normalAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        real hL = heightAtGrid(i - 1, j) * voxelSize.y;
        real hR = heightAtGrid(i + 1, j) * voxelSize.y;
        real hD = heightAtGrid(i, j + 1) * voxelSize.y;
        real hU = heightAtGrid(i, j - 1) * voxelSize.y;

        return vector(hL - hR, 2.0, hD - hU).normalizado();
    }

    /**
     * Returns texture coordinates in the range [0, 1] for the given (i, j) coordinates
     */
    vector2 textCoordAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        return vector2((real) i / (real) (this->heightMap->getWidth() - 1), (real) j / (real) (this->heightMap->getHeight() - 1));
    }

    /**
     * Returns (2D) height for the given 2D (i, j) coordinates. i and j are unsigned thus in the range [0, 2D grid width] and [0, 2D grid height] respectivelly
     */
    real heightAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        vector pixel = this->heightMap->getPixel(i, j);

        //logger->info("<%u, %u) = <%.0f, %.0f, %.0f>", i, j, pixel.x, pixel.y, pixel.z);

        return (pixel.x * pixel.y * pixel.z * one_over_max_color);
    }

    /*
     * Returns (3d) height for the given 3D (x, z), thus forming an implicit 3D point at <x, height, z>. Coordinates x and z are in the range [0, 3D width] and [0, 3D depth] respectively
     * */
    real heightAt(real x, real z) const {
        unsigned int i = getI(x);
        unsigned int j = getJ(z);

        real di = (x - (real) i * voxelSize.x) / voxelSize.x;
        real dj = (z - (real) j * voxelSize.z) / voxelSize.z;

        if (di <= ((real) 1 - dj)) {
            return barycentric(
                    vector(0, heightAtGrid(i, j) * voxelSize.y, 0),
                    vector(1, heightAtGrid(i + 1, j) * voxelSize.y, 0),
                    vector(0, heightAtGrid(i, j + 1) * voxelSize.y, 1),
                    vector2(di, dj));

        } else {
            return barycentric(
                    vector(1, heightAtGrid(i + 1, j) * voxelSize.y, 0),
                    vector(1, heightAtGrid(i + 1, j + 1) * voxelSize.y, 1),
                    vector(0, heightAtGrid(i, j + 1) * voxelSize.y, 1),
                    vector2(di, dj));
        }
    }

    /**
     * Returns the normal at a  given 3D (x, z)
     */
    vector normalAt(real x, real z) const {
        unsigned int i = getI(x);
        unsigned int j = getJ(z);

        real di = (x - (real) i * voxelSize.x) / voxelSize.x;
        real dj = (z - (real) j * voxelSize.z) / voxelSize.z;

        if (di <= ((real) 1 - dj)) {
			vector A = positionAtGrid(i, j);
			vector B = positionAtGrid(i + 1, j);
			vector C = positionAtGrid(i, j+1);

			return ((B-A) ^ (C-A)).normalizado();
        } else {
            vector A = positionAtGrid(i + 1, j);
			vector B = positionAtGrid(i + 1, j + 1);
            vector C = positionAtGrid(i, j + 1);
			return ((B-A) ^ (C-A)).normalizado();
        }
    }

    vector positionAt(real x, real z) const {
    	return vector(x, heightAt(x, z), z);
    }

	virtual String toString() const {
		return String("HeightMap patch ["  + std::to_string(this->getWidth()) + "x" + std::to_string(this->getHeight()) + "x" + std::to_string(this->getDepth()) + "] based on [ " + std::to_string(this->getGridWidth()) + "x" + std::to_string(this->getGridHeight()) + " - voxel size: " + this->voxelSize.toString());
	}


private:
    real barycentric(const vector &p1, const vector &p2, const vector &p3, const vector2 &pos) const {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }

    real calculateHeight() const {
    	real maxHeight = -1;
    	for(unsigned int i = 0; i < this->getGridWidth(); i++) {
    		for(unsigned j = 0; j < this->getGridHeight(); j++) {
    			maxHeight = std::max(abs(maxHeight), abs(heightAtGrid(i, j) * voxelSize.y));
    		}
    	}

    	return maxHeight;
    }
};
