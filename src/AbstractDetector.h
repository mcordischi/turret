
#ifndef __ABS_DETECTOR_H_
#define __ABS_DETECTOR_H_

/**
*	Image recognition Interface
**/
class AbstractDetector{
    public:
        virtua bool identifyItem(char* picPath) = 0;
};

#endif // __ABS_DETECTOR_H_