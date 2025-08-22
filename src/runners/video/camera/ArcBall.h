/*
 * ArcBall.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ARCBALL_H_
#define SRC_VIDEO_ARCBALL_H_

#include<Math3d.h>
#include<math.h>

class ArcBall {
private:
	Logger *logger = LoggerFactory::getLogger("ArcBall");

	bool dragging = false;

	vector2 startPosition;
	vector2 endPosition;

	matriz_3x3 viewModel;
	unsigned short height;
	unsigned short width;
public:

	bool isDragging() {
		return this->dragging;
	}

	void startDrag(const vector2 &position, const matriz_3x3 &viewModel, unsigned short width, unsigned short height) {
		this->dragging = true;
		this->startPosition = position;
		this->endPosition = position;
		this->viewModel = viewModel;
		this->height = height;
		this->width = width;

//		logger->info("arcball startDrag at %s with viewModel:\n%s\n",
//				startPosition.toString().c_str(),
//				viewModel.toString().c_str());

	}

	matriz_3x3 drag(const vector2 &delta) {
		if(this->dragging) {
			this->endPosition = this->endPosition + delta;

//			logger->info("Arcball Dragging - %s ---> %s: (%s)\n%s\n",
//					startPosition.toString().c_str(),
//					endPosition.toString().c_str(),
//					delta.toString().c_str(),
//					getRotation().toString().c_str());

			return getRotation();
		}

		return viewModel;
	}

	matriz_3x3 endDrag(const vector2 &position) {
		cuaternion cuaternion;

		if(this->dragging) {
			this->dragging = false;

			this->endPosition = position;
//			logger->info("%s ---> %s\n%s\n",
//					startPosition.toString().c_str(),
//					endPosition.toString().c_str(),
//					getRotation().toString().c_str());

			return getRotation();

		}

		return viewModel;
	}

	/**
	 * Devuelve el eje y el angulo codificado en un cuaternion. NO es un cuaternion rotacion.
	 */
	matriz_3x3 getRotation() const {
		/* onIdle() */
		  if (startPosition != endPosition) {
		    vector va = calculateVector(this->endPosition, this->width, this->height);
		    vector vb = calculateVector( this->startPosition, this->width, this->height);
		    float angle = acos(std::min(1.0f, va * vb));

		    vector axis_in_camera_coord = va ^ vb;
//		    matriz_3x3 camera2object = this->viewModel.inversa();
//		    vector  axis_in_object_coord = camera2object * axis_in_camera_coord;
//
//            logger->info("Arcball Rotation- axis %s angle %.2f world axis %s",
//                    axis_in_camera_coord.toString().c_str(),
//                    angle,
//                    axis_in_object_coord.toString().c_str()
//                    );

		    return matriz_3x3::matrizRotacion(angle, axis_in_camera_coord) * this->viewModel;
		  }

		return this->viewModel;
	}
private:
	/**
	 * Get a normalized vector from the center of the virtual ball O to a
	 * point P on the virtual ball surface, such that P is aligned on
	 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
	 * sphere, return the nearest point on the virtual ball surface.
	 */
	vector calculateVector(vector2 screenPosition, unsigned int width, unsigned int height) const {
	  vector arcballVector = vector(	1.0 * screenPosition.x / width * 2 - 1.0,
				  	  	  				1.0 * screenPosition.y / height * 2 - 1.0,
										0);
	  arcballVector.y = -arcballVector.y;

	  float OP_squared = arcballVector.x * arcballVector.x + arcballVector.y * arcballVector.y;
	  if (OP_squared <= 1.0)
	    arcballVector.z = sqrt(1.0 - OP_squared);  // Pythagoras
	  else
	    arcballVector = arcballVector.normalizado();  // nearest point
	  return arcballVector;
	}
};


#endif /* SRC_VIDEO_ARCBALL_H_ */
