#ifndef __APP_H__
#define __APP_H__

#include <memory>
#include "GLWin.h"
#include "ClothSim.h"

class App {
public:
    std::unique_ptr<GLWin> glWin;
    std::shared_ptr<ArcballCamera> camera;
    std::shared_ptr<ClothSim> sim;
};

#endif