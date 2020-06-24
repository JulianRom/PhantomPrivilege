#ifndef BEEF_PLANET_MOUSE_CONTROLLER_H
#define BEEF_PLANET_MOUSE_CONTROLLER_H

#include "delta.h"

class MouseController {
public:
    MouseController();
    ~MouseController();

    void initialize(dbasic::DeltaEngine *engine);
    void process(ysVector &planet_position);
    void destroy();

protected:

    void getMouseChange(int *x, int *y);
    dbasic::DeltaEngine *m_engine;
    int m_mouse_x;
    int m_mouse_y;
    int m_pre_x;
    int m_pre_y;
    int m_zoom;
    ysMatrix m_camera_transform;

};

#endif