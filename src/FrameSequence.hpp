//
//  FrameSequence.hpp
//  kissMeInteractive
//
//  Created by Adam Cole on 10/21/22.
//

#ifndef FrameSequence_hpp
#define FrameSequence_hpp

#include <stdio.h>
#include "ofMain.h"

class FrameSequence {
public:
    FrameSequence();
    FrameSequence(string imgFolder);
    
    void nextFrame();
    ofImage getCurrImage();
    int getCurrFrame();
    float getProgress();
    bool getIsPlayingBackward();
    
    void setProgress(float progress, bool _isPlayingBackward);
    
private:
    vector<ofImage> sequence;
    int currFrame;
    bool isPlayingBackward;
    
    void checkBoundary();
    
};

#endif /* FrameSequence_hpp */
