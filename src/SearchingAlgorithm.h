#include "MovingAlgorithm.h"
#include "CameraControl.h"

#ifndef __SEARCH_ALG_H_
#define __SEARCH_ALG_H_


class SearchingAlgorithm {
    private:
        MovingAlgorithm* move;
        CameraControl* control;

    public:

        SearchingAlgorithm(MovingAlgorithm*,CameraControl*);

        bool identifyItem(char* picPath);
};


#endif //__SEARCH_ALG_H_
