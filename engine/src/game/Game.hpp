#ifndef DUCTTAPE_ENGINE_GAME_GAME
#define DUCTTAPE_ENGINE_GAME_GAME

#include "Root.hpp"

#include "event/BeginFrameEvent.hpp"
#include "event/GoodbyeEvent.hpp"

namespace dt {

/**
  * The main instance of a game, running the main loop.
  */
class Game {
public:
    /**
      * Default constructor.
      */
    Game();

    /**
      * Called when initializing the game. Create custom stuff here.
      */
    virtual void OnInitialize() = 0;

    /**
      * The main loop of the Game. Calls OnInitialize().
      * @param argc Command line param count.
      * @param argv Command line params.
      */
    void Run(int argc, char** argv);

    /**
      * Requests a shutdown. The current frame is being processed, but then the main loop will be cancelled.
      */
    void RequestShutdown();

    /**
      * Returns whether the main loop is running.
      * @returns Whether the main loop is running.
      */
    bool IsRunning();

protected:
    sf::Clock mClock;           //!< A clock for timing the frames.
    bool mIsShutdownRequested;  //!< Whether a shutdown has been requested.
    bool mIsRunning;            //!< Whether the game loop is running.
};

}

#endif
