#include "opencv2/core/core.hpp"

class AbstractTracker {

    public:

        //Returns the next desired frame
        virtual cv::Mat* getNextFrame() = 0;

};
