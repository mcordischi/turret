#include "MovingAlgorithm.h"
#include "CameraControl.h"

class SearchingAlgorithm {
    private:
        MovingAlgorithm* move;
        CameraControl* control;

    public:

        SearchingAlgorithm(MovingAlgorithm*,CameraControl*);

        bool identifyItem(char* picPath);
};
