#ifndef __HV_TRACKER_H_
#define __HV_TRACKER_H_


#include "AbstractCameraControl.h"
#include "HorizontalTracker.h"
#include "opencv2/core/core.hpp"


#define UP_STATE 0
#define DW_STATE 2

class HVTracker : public HorizontalTracker{
    protected:
        int verticalState;
    public:

        HVTracker(AbstractCameraControl*);

        //Returns the next desired frame
        void nextStepOnTrack();


 };

#endif //__HV_TRACKER_H_
