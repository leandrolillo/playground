/*
 * videoRunner.h
 *
 *  Created on: 22/11/2012
 *      Author: Lean
 */

#pragma once

#include <unistd.h>

#define GL_SILENCE_DEPRECATION //hide opengl deprecated on osx warnings
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include "SDL3/SDL.h"

#include "Playground.h"
#include "VideoRunner.h"
#include "CubeMapResourceAdapter.h"
#include "FragmentShaderResourceAdapter.h"
#include "MeshResourceAdapter.h"
#include "ShaderProgramResourceAdapter.h"
#include "TextureResourceAdapter.h"
#include "VertexArrayAdapter.h"
#include "VertexShaderResourceAdapter.h"
#include "TerrainResourceAdapter.h"

#ifndef GL_MAJOR_VERSION
	#define GL_MAJOR_VERSION 0x821B
#endif

#ifndef GL_MINOR_VERSION
	#define GL_MINOR_VERSION 0x821C
#endif

bool playgroundEventFilter(void *context, SDL_Event *event);

class OpenGLRunner: public VideoRunner {
private:
  Logger *logger = LoggerFactory::getLogger("video/openGlRunner");

  SDL_Window *window = null;
  SDL_GLContext glcontext = null;
  unsigned int majorVersion = 0;
  unsigned int minorVersion = 0;

  TextureResource *defaultTexture = null;
  const ShaderProgramResource *currentShaderProgram = null;
  std::unordered_map<unsigned int, unsigned int> boundTextures;

public:

  using VideoRunner::VideoRunner;

  virtual unsigned char getInterests() const override {
    return RESIZE | KEY_DOWN;
  }

  virtual ~OpenGLRunner() {
    if (window != null) {
      SDL_HideWindow(window);
      logger->debug("HIding sdl window");
      if (glcontext != null) {
        SDL_GL_DestroyContext(glcontext);
        logger->debug("Deleted opengl context");
      }
      this->useProgramResource(null);
      logger->debug("Remove active shaders");
      SDL_DestroyWindow(window);
      logger->info("OpenGL/SDL Window destroyed");
    }
    SDL_Quit();
    logger->info("SDL shutdown");
  }

  virtual bool initialize() override {
    //logger->setLogLevel(LogLevel::DEBUG);
    VideoRunner::initialize();
    this->getResourceManager().addAdapter<TextureResourceAdapter>();
    this->getResourceManager().addAdapter<CubeMapResourceAdapter>();
    this->getResourceManager().addAdapter<VertexArrayResourceAdapter>();
    this->getResourceManager().addAdapter<MeshResourceAdapter>();
    this->getResourceManager().addAdapter<VertexShaderResourceAdapter>();
    this->getResourceManager().addAdapter<FragmentShaderResourceAdapter>();
    this->getResourceManager().addAdapter<ShaderProgramResourceAdapter>();
    this->getResourceManager().addAdapter<TerrainResourceAdapter>();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      logger->error("SDL_Init Error: %s", SDL_GetError() == null ? "" : SDL_GetError());
      return false;
    }

    logger->debug("SDL initialized");

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);

    this->window = SDL_CreateWindow("SDL2/OpenGL Demo", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    logger->debug("SDL Window created");

//        SDL_PumpEvents();
//        SDL_SetRelativeMouseMode(true);
    SDL_AddEventWatch(playgroundEventFilter, this);

    logger->debug("SDL event watch registered");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    if ((glcontext = SDL_GL_CreateContext(window)) == null) {
      logger->error("SDL_GL_CreateContext Error: %s", SDL_GetError());
      return false;
    }

    int majorVersion;
    int minorVersion;

    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    this->majorVersion = (unsigned int) majorVersion;
    this->minorVersion = (unsigned int) minorVersion;

    logger->info("OpenGL [%d].[%d] initialized\nVersion: [%s]\nGLSL Version: [%s]\nGLEW Version [%s]\nVendor: [%s]\nRenderer: [%s]",
        this->majorVersion, this->minorVersion, glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION), "0", //glewGetString(GLEW_VERSION),
        glGetString(GL_VENDOR), glGetString(GL_RENDERER));

    /**
     * OpenGL defaults so that something is rendered with minimum configuration.
     */
    defaultTexture = new TextureResource(this->generateDefaultTexture());
    defaultTexture->setUri("OpenGLRunner::defaultTextureResource");
    this->getContainer().getResourceManager().addResource(defaultTexture);

    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
      logger->error("SDL_Init Error: %s", SDL_GetError());
      return false;
    }

    return true;
  }

  virtual bool afterInitialize() override {
    int height = 0;
    int width = 0;
    SDL_GetWindowSize(window, &width, &height);

    this->getContainer().onResize(width, height);

    SDL_WarpMouseInWindow(window, width >> 1, height >> 1);

    return true;
  }

  virtual void beforeLoop() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  LoopResult doLoop() override {
    SDL_Event windowEvent;

    while (SDL_PollEvent(&windowEvent)) {
      switch (windowEvent.type) {
      case SDL_EVENT_QUIT:
        return LoopResult::STOP;
      }
    }

    return LoopResult::CONTINUE;
  }

  virtual void afterLoop() override {
    SDL_GL_SwapWindow(window);
  }

  virtual bool processEvent(SDL_Event *event) {
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      SDL_SetWindowMouseGrab(this->window, false);
      logger->debug("WINDOW RESIZED to [%d, %d]", event->window.data2, event->window.data1);
      this->getContainer().onResize(event->window.data1, event->window.data2);
      SDL_SetWindowMouseGrab(this->window, true);
      return true;
    case SDL_EVENT_KEY_DOWN:
      //SDL_Log("SDL_KEYDOWN %d", event->key.keysym.sym);
      logger->verbose("KEYDOWN: %d, %d", event->key.key, event->key.mod);
      this->getContainer().onKeyDown(event->key.key, event->key.mod);
      return true;
    case SDL_EVENT_KEY_UP:
      //SDL_Log("SDL_KEYUP %d", event->key.keysym.sym);
      logger->verbose("KEYUP: %d, %d", event->key.key, event->key.mod);
      this->getContainer().onKeyUp(event->key.key, event->key.mod);
      return true;
    case SDL_EVENT_MOUSE_MOTION:
      //SDL_Log("SDL_MOUSEMOTION (%d,%d) delta=(%d,%d)", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
      this->getContainer().onMouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel, event->motion.state);
      logger->verbose("MOUSEMOVE: (%d, %d)", event->motion.xrel, event->motion.yrel);
      return true;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      //SDL_Log("SDL_MOUSEBUTTONDOWN %d", event->button.button);
      logger->verbose("MOUSEBUTTONDOWN: %d at <%d, %d>", event->button.button, event->button.x, event->button.y);
      this->getContainer().onMouseButtonDown(event->button.button, event->button.x, event->button.y);
      return true;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      //SDL_Log("SDL_MOUSEBUTTONUP %d", event->button.button);
      logger->verbose("MOUSEBUTTONUP: %d at <%d, %d>", event->button.button, event->button.x, event->button.y);
      this->getContainer().onMouseButtonUp(event->button.button, event->button.x, event->button.y);
      return true;
    case SDL_EVENT_MOUSE_WHEEL:
      //SDL_Log("SDL_MOUSEWHEEL %d %d", event->wheel.direction, event->wheel.y);
      logger->verbose("MOUSEWHEEL: %d", event->wheel.y);
      this->getContainer().onMouseWheel(event->wheel.y);
      return true;
    }

    return false;
  }

  virtual void onResize(unsigned int width, unsigned int height) override {
    VideoRunner::onResize(width, height);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) override {
    logger->info("Key down: %d", key);
    logger->info("KMOD_ALT: %d", keyModifier & SDL_KMOD_ALT);
    logger->info("KMOD_ALT: %d", keyModifier & SDL_KMOD_ALT);
    logger->info("KMOD_CTRL: %d", keyModifier & SDL_KMOD_CTRL);
    logger->info("KMOD_SHIFT: %d", keyModifier & SDL_KMOD_SHIFT);
    logger->info("KMOD_GUI: %d", keyModifier & SDL_KMOD_GUI);
    logger->info("KMOD_MODE: %d", keyModifier & SDL_KMOD_MODE);

    switch (key) {
    case SDLK_ESCAPE:
      this->getContainer().stop();
      break;
    case SDLK_F4:
      if (keyModifier & SDL_KMOD_GUI) {
        this->getContainer().stop();
      }
      break;
    case SDLK_Q:
      if (keyModifier & SDL_KMOD_ALT) {
        this->getContainer().stop();
      }
      break;

    case SDLK_RETURN:
      if (keyModifier & SDL_KMOD_GUI) {
        this->setFullscreen(!this->getFullscreen());
      }
      break;
    }
  }

  bool setFullscreen(bool fullScreen) override {
    SDL_SetWindowFullscreen(this->window, VideoRunner::setFullscreen(fullScreen));

    int height = 0;
    int width = 0;
    SDL_GetWindowSize(window, &width, &height);

    this->getContainer().onResize(width, height);

    return this->getFullscreen();
  }

  virtual void resize(unsigned int width, unsigned int height) override {
    SDL_SetWindowSize(window, width, height);
  }

  void setMousePosition(unsigned int x, unsigned int y) override {
    SDL_WarpMouseInWindow(this->window, x, y);
  }

  unsigned int getMajorVersion() const {
    return majorVersion;
  }

  unsigned int getMinorVersion() const {
    return minorVersion;
  }

  void useProgramResource(const ShaderProgramResource *program) override {
    if (program != null) {
      if (currentShaderProgram == null || currentShaderProgram->getId() != program->getId()) {
        glUseProgram(program->getId());
        currentShaderProgram = program;

#ifdef VERBOSE
					int maxLength = 0;
					int numberOfParams = 0;
					glGetProgramiv(program->getId(), GL_ACTIVE_ATTRIBUTES, &numberOfParams);
					glGetProgramiv(program->getId(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
					char *nameBuffer = new char[maxLength + 1];

					logger->debug("Program [%s] has [%d] attributes:", program->toString().c_str(), numberOfParams);
					for(int index = 0; index < numberOfParams; index++) {
						GLsizei actualLength = 0;
						GLenum type;
						GLint size;
						glGetActiveAttrib(program->getId(), index, maxLength, &actualLength, &size, &type, nameBuffer);


						logger->debug("		Attribute: [%s] at location [%d] of type [%d] and size [%d]", nameBuffer, glGetAttribLocation(program->getId(), nameBuffer), type, size);
					}
					delete [] nameBuffer;

					glGetProgramiv(program->getId(), GL_ACTIVE_UNIFORMS, &numberOfParams);
					glGetProgramiv(program->getId(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
					nameBuffer = new char[maxLength + 1];

					logger->debug("Program [%s] has [%d] uniforms:", program->toString().c_str(), numberOfParams);
					for(int index = 0; index < numberOfParams; index++) {
						GLsizei actualLength = 0;
						GLenum type;
						GLint size;
						glGetActiveUniform(program->getId(), index, maxLength, &actualLength, &size, &type, nameBuffer);

						logger->debug("		Uniform: [%s] of type [%d] and size [%d]", nameBuffer, type, size);
					}
					delete [] nameBuffer;
#endif
      }
    } else {
      logger->debug("Using program 0");
      glUseProgram(0);
      currentShaderProgram = null;
    }
  }

  bool sendUnsignedInt(const String &name, unsigned int value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glProgramUniform1i(currentShaderProgram->getId(), glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending unsigned int %s=%u: %s", name.c_str(), value, errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  bool sendMatrix(const String &name, const matriz_4x4 &value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glUniformMatrix4fv(glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), 1, GL_TRUE, (real*) value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending matrix %s: %s", name.c_str(), errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  bool sendMatrix(const String &name, const matriz_3x3 &value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glUniformMatrix3fv(glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), 1, GL_TRUE, (real*) value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending matrix %s: %s", name.c_str(), errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  bool sendVector(const String &name, const vector &value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glUniform3fv(glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), 1, (real*) value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending vector %s: %s", name.c_str(), errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  bool sendVector4(const String &name, const vector4 &value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glUniform4fv(glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), 1, (real*) value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending vector %s: %s", name.c_str(), errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  bool sendReal(const String &name, const real &value) const override {
    if (currentShaderProgram != null) {
      String errorMessage;
      glUniform1fv(glGetUniformLocation(currentShaderProgram->getId(), name.c_str()), 1, &value);
      if (!(errorMessage = getGlError()).empty()) {
        logger->error("Error sending real %s: %s", name.c_str(), errorMessage.c_str());
        return false;
      }
    }
    return true;
  }

  void setTexture(unsigned int location, const TextureResource *texture, VideoAttribute type = VideoAttribute::TEXTURE_2D) override {
    glActiveTexture(GL_TEXTURE0 + location);
    if (texture != null) {
      if (boundTextures.count(GL_TEXTURE0 + location) == 0 || boundTextures.at(GL_TEXTURE0 + location) != texture->getId()) {
        boundTextures[GL_TEXTURE0 + location] = texture->getId();
        glBindTexture(OpenGLUtilities::asGlAttribute(type), texture->getId());

        String errorMessage;
        if (!(errorMessage = getGlError()).empty()) {
          logger->error("Error binding texture [%s] of type [%u] at location [%u]", texture->toString().c_str(), type, location,
              errorMessage.c_str());
        }
      }
    } else {
      boundTextures[GL_TEXTURE0 + location] = 0;
      glBindTexture(OpenGLUtilities::asGlAttribute(type), 0);
    }
  }
  void setTexture(unsigned int location, const String &samplerName, const TextureResource *texture, VideoAttribute type = VideoAttribute::TEXTURE_2D)
      override {
    setTexture(location, texture, type);
    sendUnsignedInt(samplerName, location);
  }

  void setClearColor(real r, real g, real b, real a) const override {
    glClearColor(r, g, b, a);
  }

  void enable(VideoAttribute attribute, VideoAttribute param1 = VideoAttribute::NONE, VideoAttribute param2 = VideoAttribute::NONE) override {
    switch (attribute) {
    case (VideoAttribute::DEPTH_TEST):
        glEnable(GL_DEPTH_TEST);
      break;
    case (VideoAttribute::CULL_FACE):
      glCullFace(OpenGLUtilities::asGlAttribute(param1));
      glEnable(GL_CULL_FACE);
      break;
    case (VideoAttribute::BLEND):
      glBlendFunc(OpenGLUtilities::asGlAttribute(param1), OpenGLUtilities::asGlAttribute(param2));
      glEnable(GL_BLEND);
      break;
    case (VideoAttribute::RELATIVE_MOUSE_MODE):
      SDL_SetWindowRelativeMouseMode(this->window, true);
      break;
    }
  }

  void disable(VideoAttribute attribute) override {
    switch (attribute) {
    case (VideoAttribute::DEPTH_TEST):
      glDisable(GL_DEPTH_TEST);
      break;
    case (VideoAttribute::CULL_FACE):
      glDisable(GL_CULL_FACE);
      break;
    case (VideoAttribute::BLEND):
      glDisable(GL_BLEND);
      break;
    case (VideoAttribute::RELATIVE_MOUSE_MODE):
      SDL_SetWindowRelativeMouseMode(this->window, true);
      break;
    }
  }

  void setOption(VideoAttribute attribute, real value) override {
    switch (attribute) {
    case VideoAttribute::LINE_WIDTH:
      logger->info("Setting line width to %.2f", value);
      glLineWidth(value);
      break;
    }
  }


  int getIntegerOption(VideoAttribute attribute) const override {
    switch(attribute) {
    case VideoAttribute::MAX_TEXTURES:
      GLint max_fragment_texture_units;
      glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_fragment_texture_units);
      return max_fragment_texture_units;
    }

    return 0;
  }


  /**
   * Expects [number of index] elements in every vertex array attribute. E.g. You can't have three vertices, six indices and six texture coordinates. See http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_Stream
   */
  void drawVertexArray(const VertexArrayResource *vertexArrayResource, const std::vector<real> &bufferSubData = {}) const override {
    String errorMessage;

    if (vertexArrayResource != null && vertexArrayResource->getId() > 0) {
      unsigned int primitiveType = OpenGLUtilities::asGlPrimitiveType(vertexArrayResource->getPrimitiveType());
      if (primitiveType < 7) {
        getGlError();
        //logger->info("Drawing vertexArray %s", vertexArrayResource->toString().c_str());

        glEnableVertexAttribArray(0);
        glBindVertexArray(vertexArrayResource->getId());
        if (!(errorMessage = getGlError()).empty()) {
          logger->error("Error binding vertex array [%s]: %s", vertexArrayResource->toString().c_str(), errorMessage.c_str());
        }

        const VertexAttribPointer *indices = vertexArrayResource->getAttribute(INDEX_LOCATION);
        if (indices != null) {
          glDrawElements(primitiveType, indices->getCount(), GL_UNSIGNED_INT, (GLvoid*) 0);
          if (!(errorMessage = getGlError()).empty()) {
            logger->error("Error drawing elements [%s]: %s", vertexArrayResource->toString().c_str(), errorMessage.c_str());
          }
        } else {
          auto vertexAttribute = vertexArrayResource->getAttribute(VERTEX_LOCATION);
          if(vertexAttribute != null) {
            if(vertexAttribute->getBufferUsage() == VideoAttribute::DYNAMIC_DRAW && !bufferSubData.empty()) {
              GLenum bufferDestination = OpenGLUtilities::asGlAttribute(vertexAttribute->getBufferDestination());
              glBindBuffer(bufferDestination, vertexAttribute->getBuffer());
              glBufferSubData(bufferDestination, 0, bufferSubData.size() * sizeof(real), bufferSubData.data());
              glBindBuffer(bufferDestination, 0);
            }

            glDrawArrays(primitiveType, 0, vertexAttribute->getCount());
            if (!(errorMessage = getGlError()).empty()) {
              logger->error("Error drawing arrays [%s]: %s", vertexArrayResource->toString().c_str(), errorMessage.c_str());
            }
          }
        }

        //glBindVertexArray(0);
        glDisableVertexAttribArray(0);
      }
    }
  }

  TextureResource* getDefaultTexture() const override {
    return this->defaultTexture;
  }

protected:
  unsigned int generateDefaultTexture() {
    String errorMessage;
    glGetError();

    unsigned int textureHandler = 0;
    glGenTextures(1, &textureHandler);

    GLubyte data[] = { 255, 255, 255, 255 };

    glBindTexture(GL_TEXTURE_2D, textureHandler);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (!(errorMessage = getGlError()).empty()) {
      logger->error("Error generating default texture", errorMessage.c_str());
      return false;
    }

    return textureHandler;
  }

  String getGlError() const { //Unify with OpenGLResourceAdapter getGLError
    return OpenGLUtilities::getGlError();
  }

  virtual String toString() const override {
    return "OpenGLRunner(id:" + std::to_string(this->getId()) + ")";
  }

};

inline bool playgroundEventFilter(void *context, SDL_Event *event) {
  OpenGLRunner *runner = (OpenGLRunner*) context;
  return runner->processEvent(event);
}
