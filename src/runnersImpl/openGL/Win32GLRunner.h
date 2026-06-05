/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#ifdef _WIN32

#ifndef WGLRUNNER_H_
#define WGLRUNNER_H_

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "win32apiRunner.h"

class OpenGLRunner: public Win32apiRunner {
	protected:
		HDC hDeviceContext;
		unsigned int bitsPerPixel;
		HGLRC hRenderingContext;
		Logger *logger;
		unsigned int majorVersion;
		unsigned int minorVersion;

	public:
		OpenGLRunner() :
				Win32apiRunner() {
			hDeviceContext = null;
			bitsPerPixel = 16;
			hRenderingContext = null;

			logger = LoggerFactory::getLogger("win32/wglRunner.h");
		}

		virtual ~OpenGLRunner() {
			wglMakeCurrent(null, null);

			logger->debug("Rendering context deselected");

			if (this->hRenderingContext != null) {
				wglDeleteContext(this->hRenderingContext);
				logger->debug("Rendering context deleted");
			}

			if (this->hDeviceContext != null) {
				ReleaseDC(this->getWnd(), this->hDeviceContext);
				logger->debug("Device context released");
			}

		}
		virtual void afterLoop() {
			SwapBuffers(this->hDeviceContext);
		}

		virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT messageCode,
				WPARAM wParam, LPARAM lParam) {
			switch(messageCode)
			{
				//suggestion from http://www.opengl.org/wiki/Platform_specifics:_Windows#It.27s_flickering.21_Why.3F
				case WM_ERASEBKGND:
				return true;
			}

			return Win32apiRunner::WndProc(hwnd, messageCode, wParam, lParam);
		}
		virtual bool createGlContext()
		{
			if (!(this->hDeviceContext = GetDC(this->getWnd()))) {
				printLastError("Could not retrieve device context");
				return false;
			}

			logger->debug("Device context initialized");

			static PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
					1, // Version Number
					PFD_DRAW_TO_WINDOW | // Format Must Support Window
							PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
							PFD_DOUBLEBUFFER, // Must Support Double Buffering
					PFD_TYPE_RGBA, // Request An RGBA Format
					bitsPerPixel, // Select Our Color Depth
					0, 0, 0, 0, 0, 0, // Color Bits Ignored
					0, // No Alpha Buffer
					0, // Shift Bit Ignored
					0, // No Accumulation Buffer
					0, 0, 0, 0, // Accumulation Bits Ignored
					bitsPerPixel, // 16Bit Z-Buffer (Depth Buffer)
					0, // No Stencil Buffer
					0, // No Auxiliary Buffer
					PFD_MAIN_PLANE, // Main Drawing Layer
					0, // Reserved
					0, 0, 0 // Layer Masks Ignored
					};

			GLuint PixelFormat;

			if (!(PixelFormat = ChoosePixelFormat(this->hDeviceContext,
					&pfd))) {
				printLastError(
						"Error al intentar seleccionar el formato de p�xel");
				return (false);
			}

			logger->debug("Pixel format selected");

			if (!SetPixelFormat(this->hDeviceContext, PixelFormat, &pfd)) {
				printLastError(
						"Error al intentar elegir el pixel format en el DC");
				return (false);
			}

			logger->debug("Pixel format set");

			if (!(this->hRenderingContext = wglCreateContext(
					this->hDeviceContext))) {
				printLastError("Error al intentar crear Rendering Context");
				return (false);
			}

			logger->debug("Rendering context created");

			if (!wglMakeCurrent(this->hDeviceContext,
					this->hRenderingContext)) {
				printLastError("Error al intentar elegir el RC en el DC");
				return (false);
			}

			logger->debug("Rendering context set");

			GLenum error = glewInit(); // Enable GLEW
			if (error != GLEW_OK) {
				printLastError("Error initializing glew");
				return false;
			}
			logger->debug("glew initialized");

			if(!GLEW_VERSION_2_0)
			{
				logger->error("OpenGL version 2.0 required");
				return false;
			} else {
				int attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
									 WGL_CONTEXT_MINOR_VERSION_ARB, 1,
									 WGL_CONTEXT_FLAGS_ARB,  WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
									 0  };

				HGLRC oldRenderingContext = this->hRenderingContext;

				if((this->hRenderingContext = wglCreateContextAttribsARB(this->hDeviceContext, null, attributes)) == null){
					logger->error("Error creating glew rendering context");
					return false;
				}
				logger->debug("glew rendering context created [%d] - disabling previous rendering context [%d]", hRenderingContext, oldRenderingContext);

				wglMakeCurrent(null, null); // Remove the temporary context from being active
				wglDeleteContext(oldRenderingContext); // Delete the temporary OpenGL 2.1 context

				if(!wglMakeCurrent(this->hDeviceContext, this->hRenderingContext)) {
					printLastError("Error setting glew rendering context");
					return false;
				}
				logger->debug("glew rendering context set");
			}

			int majorVersion;
			int minorVersion;
			glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
			glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

			this->majorVersion = (unsigned int)majorVersion;
			this->minorVersion = (unsigned int)minorVersion;

			logger->info("OpenGL [%d].[%d] initialized\nVersion: [%s]\nGLSL Version: [%s]\nGLEW Version [%s]\nVendor: [%s]\nRenderer: [%s]",
					this->majorVersion,
					this->minorVersion,
					glGetString(GL_VERSION),
					glGetString(GL_SHADING_LANGUAGE_VERSION),
					glewGetString(GLEW_VERSION),
					glGetString(GL_VENDOR),
					glGetString(GL_RENDERER));

			return true;
		}
		virtual bool init() {
			if (Win32apiRunner::init()) {
				return createGlContext();
			}

			return false;
		}

	unsigned int getMajorVersion() const {
		return majorVersion;
	}

	unsigned int getMinorVersion() const {
		return minorVersion;
	}
};

#endif /* WGLRUNNER_H_ */
#endif /*_WIN32*/
