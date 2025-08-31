/*
 * playground.h
 *
 *  Created on: 03/11/2012
 *      Author: Lean
 */

#pragma once

#include <vector>
#include <unordered_map>
#include "Chronometer.h"
#include "JavaLike.h"
#include "Logger.h"
#include "ResourceManager.h"

enum class PlaygroundStatus {
  CREATED,
  INITIALIZED,
  RUNNERS_BROKEN,
  RUNNERS_INITIALIZED,
  RUNNING,
  STOPPED,
  ERROR
};

enum class LoopResult {
  CONTINUE, SKIP, STOP, FINISHED
};

enum Interests {
  RESIZE = 1,
  MOVE = 2,
  KEY_DOWN = 4,
  KEY_UP = 8,
  MOUSE_MOVE = 16,
  MOUSE_BUTTON_DOWN = 32,
  MOUSE_BUTTON_UP = 64,
  MOUSE_WHEEL = 128
};

class Playground;

class PlaygroundRunner {
private:
  Playground *container = null;
  bool _enabled = true;

public:
  PlaygroundRunner() {
  }

  virtual ~PlaygroundRunner() {
  }

  virtual bool initialize() {
    return true;
  }
  virtual bool afterInitialize() {
    return true;
  }
  virtual void beforeLoop() {
  }
  virtual LoopResult doLoop() {
    return LoopResult::CONTINUE;
  }
  virtual void afterLoop() {
  }

  void setContainer(Playground &container) {
    this->container = &container;
  }
  Playground* getContainer() const {
    return this->container;
  }

  Chronometer& getStopWatch() const;

  ResourceManager &getResourceManager() const;

  virtual unsigned char getId() const = 0;

  virtual unsigned char getInterests() const {
    return 0;
  }

  /**
   * TODO: Should move these methods to a separate interface (abstract class)?
   */
  virtual void onResize(unsigned int height, unsigned width) {
  }

  virtual void onKeyDown(unsigned int key, unsigned int keyModifier) {
  }

  virtual void onKeyUp(unsigned int key, unsigned int keyModifier) {
  }

  virtual void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) {
  }

  virtual void onMouseButtonDown(unsigned char button, int x, int y) {
  }

  virtual void onMouseButtonUp(unsigned char button, int x, int y) {
  }

  virtual void onMouseWheel(int wheel) {
  }

  unsigned char getEnabled() const {
    return _enabled;
  }

  void setEnabled(unsigned char enabled) {
    this->_enabled = enabled;
  }

  virtual String toString() const {
    return "PlaygroundRunner(id:" + std::to_string(this->getId()) + ")";
  }
};

class Playground {
private:
  Logger *logger = LoggerFactory::getLogger("core/Playground.h");
  String name;
  String resourcesRootFolder;
  std::vector<std::unique_ptr<PlaygroundRunner>> runners;
  std::unordered_map<unsigned char, PlaygroundRunner*> runners_by_id;
  PlaygroundStatus status = PlaygroundStatus::CREATED;
  std::vector<PlaygroundRunner*> resizeObservers;
  std::vector<PlaygroundRunner*> moveObservers;
  std::vector<PlaygroundRunner*> keyDownObservers;
  std::vector<PlaygroundRunner*> keyUpObservers;
  std::vector<PlaygroundRunner*> mouseMoveObservers;
  std::vector<PlaygroundRunner*> mouseButtonDownObservers;
  std::vector<PlaygroundRunner*> mouseButtonUpObservers;
  std::vector<PlaygroundRunner*> mouseWheelObservers;
  Chronometer *stopWatch = null;

  ResourceManager resourceManager; //MUST be defined after runners so that it is initialized after them and deleted before them.

public:
  Playground(const String &rootFolder) :
      resourceManager(rootFolder) {
  }

  Playground* withName(const String name) {
    this->name = name;
    return this;
  }

  virtual ResourceManager &getResourceManager() {
    return resourceManager;
  }

  PlaygroundStatus getStatus() const {
    return status;
  }

  void setStatus(const PlaygroundStatus &status) {
    this->status = status;
  }

  void setStopWatch(Chronometer *stopWatch) {
    this->stopWatch = stopWatch;
  }

  Chronometer& getStopWatch() {
    if (this->stopWatch == null) {
      throw std::domain_error(
          "StopWatch not registered - you need to include at least one playground runner that provides a stopwatch");
    }
    return *this->stopWatch;
  }

  PlaygroundRunner* getRequiredRunner(const unsigned char id) const {
    auto pair = runners_by_id.find(id);
    if (pair == runners_by_id.end()) {
      throw std::invalid_argument("Could not find required playground runner");
    }

    return pair->second;
  }

  PlaygroundRunner* getRunner(const unsigned char id) const {
    auto pair = runners_by_id.find(id);
    if (pair == runners_by_id.end()) {
      return null;
    }

    return pair->second;
  }

  PlaygroundRunner *addRunner(PlaygroundRunner *runner) {
    logger->debug("Adding runner with id [%d]", runner->getId());

    /**
     * Check for duplicates
     */
    for (auto &currentRunner : runners) {
      if (currentRunner->getId() == runner->getId()) {
        logger->error("Runner with id [%d] already added - skipping",
            runner->getId());
        return runner;
      } else {
        logger->debug("Added runner with id [%d]", runner->getId());
      }
    }

    /**
     * Insert ordered by id
     */
    std::vector<std::unique_ptr<PlaygroundRunner>>::iterator currentRunnerIterator =
        runners.begin();
    while (currentRunnerIterator != runners.end()
        && (*currentRunnerIterator)->getId() < runner->getId()) {
      currentRunnerIterator++;
    }

    runner->setContainer(*this);
    runners.insert(currentRunnerIterator, std::unique_ptr < PlaygroundRunner > (runner));
    runners_by_id[runner->getId()] = runner;

    return runner;
  }

  void initialize() {
    if (status == PlaygroundStatus::CREATED) {
      logger->debug("Initializing playground");
      initializePlayground();
      status = PlaygroundStatus::INITIALIZED;
    }

    if (status == PlaygroundStatus::INITIALIZED) {
      if (initializeRunners()) {
        status = PlaygroundStatus::RUNNERS_INITIALIZED;
        logger->info("Initialization completed");
      } else {
        status = PlaygroundStatus::RUNNERS_BROKEN;
        logger->error("Runners initialization failed");
      }
    }
  }

  virtual void run() {
    logger->debug("Playground - run");
    logger->info("\n -------------------\n| Running %s\n -------------------",
        this->toString().c_str());

    this->getResourceManager().logStatus();

    this->initialize();

    if (status == PlaygroundStatus::RUNNERS_INITIALIZED
        || status == PlaygroundStatus::STOPPED) {
      resourceManager.unload(ResourceManager::EphemeralLabel);

      logger->verbose("Entering loop");

      status = PlaygroundStatus::RUNNING;

      while (status == PlaygroundStatus::RUNNING) {
        this->loop();
      }
    }
    logger->debug("Finished looping... shutting down");
    //shutdown();
    logger->info("done.");
  }

  virtual void stop() {
    status = PlaygroundStatus::STOPPED;
  }

  virtual void loop() {
    try {
      if (this->stopWatch != null) {
        this->stopWatch->update();
      }

      logger->verbose("Calling enabled runners beforeLoop");
      for (auto &currentRunner : runners) {
        if (currentRunner->getEnabled()) {
          currentRunner->beforeLoop();
        }
      }

      logger->verbose("Calling enabled runners doLoop");
      unsigned int activeRunners = 0;
      for (std::vector<std::unique_ptr<PlaygroundRunner>>::iterator currentRunnerIterator =
          runners.begin(); currentRunnerIterator != runners.end();
          currentRunnerIterator++) {
        if ((*currentRunnerIterator)->getEnabled()) {
          logger->verbose("Running Loop of %d",
              (*currentRunnerIterator)->getId());
          LoopResult result = (*currentRunnerIterator)->doLoop();

          if (result != LoopResult::CONTINUE) {
            if (result == LoopResult::STOP) {
              this->status = PlaygroundStatus::STOPPED;
            } else if (result == LoopResult::FINISHED) {
              runners.erase(currentRunnerIterator);
            } else
              // result == SKIP
              activeRunners++;

            break;
          } else
            activeRunners++;
        }
      }

      logger->verbose("Calling enabled runners afterLoop");
      for (auto &currentRunner : runners) {
        if (currentRunner->getEnabled()) {
          currentRunner->afterLoop();
        }
      }

      if (activeRunners == 0) {
        this->status = PlaygroundStatus::STOPPED;
        logger->debug("There're no more runners active... stopping");
      }

    } catch (std::exception &exception) {
      logger->error("Loop broken: [%s]", exception.what());
      this->status = PlaygroundStatus::ERROR;
    }
  }

  void onResize(unsigned int height, unsigned width) {
    for (auto currentRunner : resizeObservers) {
      currentRunner->onResize(height, width);
    }
  }

  void onKeyDown(unsigned int key, unsigned int keyModifier) {
    for (auto currentRunner : keyDownObservers) {
      currentRunner->onKeyDown(key, keyModifier);
    }
  }

  void onKeyUp(unsigned int key, unsigned int keyModifier) {
    for (auto currentRunner : keyUpObservers) {
      currentRunner->onKeyUp(key, keyModifier);
    }
  }

  void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) const {
    for (auto currentRunner : mouseMoveObservers) {
      currentRunner->onMouseMove(x, y, dx, dy, buttons);
    }
  }

  void onMouseButtonDown(unsigned char button, int x, int y) {
    for (auto currentRunner : mouseButtonDownObservers) {
      currentRunner->onMouseButtonDown(button, x, y);
    }
  }

  void onMouseButtonUp(unsigned char button, int x, int y) {
    for (auto currentRunner : mouseButtonUpObservers) {
      currentRunner->onMouseButtonUp(button, x, y);
    }
  }

  void onMouseWheel(int wheel) {
    for (auto currentRunner : mouseWheelObservers) {
      currentRunner->onMouseWheel(wheel);
    }
  }

  virtual String toString() const {
    String runnersToString;
    for (auto const &runner : this->runners) {
      runnersToString += "  *" + runner->toString() + "\n";
    }

    return "Playground" + (this->name.empty() ? "" : " [" + this->name + "]") + "\n"
        + resourceManager.toString() + "\n"
        + (runnersToString.empty() ? "" : ":\n" + runnersToString);
  }

  virtual ~Playground() {
  }

protected:
  virtual void initializePlayground() {
    logger->debug("framework initialized");
  }

private:
  // "thread" creation
  bool initializeRunners() {
    if(status < PlaygroundStatus::RUNNERS_INITIALIZED) {
      logger->debug("initializing runners:");

      for (auto &currentRunner : runners) {
        logger->info("Initializing runner [%s]", currentRunner->toString().c_str());

        try {
          if (!currentRunner->initialize()) {
            logger->error("Failed to initialize runner [%d]",
                currentRunner->getId());
            return false;
          }
        } catch (const std::out_of_range &e) {
          logger->error("Failed to initialize runner [%d] - out of range: [%s]",
              currentRunner->getId(), e.what());
          return false;
        } catch (const std::exception &stdException) {
          logger->error("Failed to initialize runner [%d] - std::exception: [%s]",
              currentRunner->getId(), stdException.what());
          return false;
        } catch (...) {
          logger->error("Failed to initialize runner [%d]",
              currentRunner->getId());
          return false;
        }

        unsigned char interests = currentRunner->getInterests();

        if (interests & RESIZE)
          this->resizeObservers.push_back(currentRunner.get());
        if (interests & MOVE)
          this->moveObservers.push_back(currentRunner.get());
        if (interests & KEY_DOWN)
          this->keyDownObservers.push_back(currentRunner.get());
        if (interests & KEY_UP)
          this->keyUpObservers.push_back(currentRunner.get());
        if (interests & MOUSE_MOVE)
          this->mouseMoveObservers.push_back(currentRunner.get());
        if (interests & MOUSE_BUTTON_UP)
          this->mouseButtonUpObservers.push_back(currentRunner.get());
        if (interests & MOUSE_BUTTON_DOWN)
          this->mouseButtonDownObservers.push_back(currentRunner.get());
        if (interests & MOUSE_WHEEL)
          this->mouseWheelObservers.push_back(currentRunner.get());
      }

      for (auto &currentRunner : runners) {
        try {
          if (!currentRunner->afterInitialize()) {
            logger->error("Failed to after initialize runner [%d]",
                currentRunner->getId());
            return false;
          } else {
            logger->debug("Successful initialization of runner [%d]",
                currentRunner->getId());
          }
        } catch (std::exception &exception) {
          logger->error("Failed to initialize runner [%d]: [%s]",
              currentRunner->getId(), exception.what());
          return false;
        } catch (...) {
          logger->error("Failed to after initialize runner [%d]",
              currentRunner->getId());
          return false;
        }
      }
    }

    return true;
  }

};
