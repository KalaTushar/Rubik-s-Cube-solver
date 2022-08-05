#ifndef _BUSYBIN_CUBE_SOLVER_H_
#define _BUSYBIN_CUBE_SOLVER_H_

#include "../CubeMover.h"
#include "../../../Util/ThreadPool.h"
#include "../../../Util/Timer.h"
#include "../../../Model/RubiksCube.h"
#include "../../../Model/WorldObject/RubiksCubeWorldObject.h"
#include "../../../Model/MoveStore/MoveStore.h"
#include "../../../Model/Goal/Goal.h"
#include "../../../OpenGLSeed/Controller/Command/Command.h"
#include "../../../OpenGLSeed/Model/World.h"
#include "../../../OpenGLSeed/View/WorldWindow.h"
#include <iostream>
using std::cout;
using std::endl;
#include <functional>
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
#include <atomic>
using std::atomic_bool;
#include <mutex>
using std::mutex;
using std::lock_guard;
#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <iterator>
using std::istream_iterator;

namespace busybin
{
  /**
   * Solver controller for the cube.
   */
  class CubeSolver : public Command
  {
  protected:
    typedef RubiksCube::MOVE MOVE;

    // Order is import.  The cube pointer has to be initialized before the
    // MoveStores.
    RubiksCubeWorldObject* pCube;
    ThreadPool*            pThreadPool;

  private:
    CubeMover*  pMover;
    atomic_bool solving;
    atomic_bool movesInQueue;
    queue<MOVE> moveQueue;
    mutex       moveMutex;
    Timer       moveTimer;
    int         solveKey;

    void onKeypress(int key, int scancode, int action, int mods);
    void onPulse(double elapsed);
    void replace(const string& needle, string& haystack, const string& with) const;

  protected:
    virtual void solveCube() = 0;
    void setSolving(bool solving);
    void processGoalMoves(const Goal& goal, RubiksCube& cube,
      unsigned goalNum, vector<MOVE>& allMoves, vector<MOVE>& goalMoves);

  public:
    CubeSolver(World* pWorld, WorldWindow* pWorldWnd,
      CubeMover* pMover, ThreadPool* pThreadPool, int solveKey);
    virtual void initialize();
    vector<string> simplifyMoves(const vector<string>& moves) const;
  };
}

#endif

