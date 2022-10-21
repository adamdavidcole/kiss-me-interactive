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
    FrameSequence(string imgFolder);
    
    vector<ofImage> sequence;
};

#endif /* FrameSequence_hpp */
