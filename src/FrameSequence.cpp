//
//  FrameSequence.cpp
//  kissMeInteractive
//
//  Created by Adam Cole on 10/21/22.
//

#include "FrameSequence.hpp"
FrameSequence::FrameSequence(){}
FrameSequence::FrameSequence(string imgFolder) {
    ofDirectory dir(imgFolder);
    
    dir.listDir();
    dir.sort();
    
    for(int i = 0; i < dir.size(); i++){
        string imagePath = dir.getPath(i);
        ofImage img;
        img.load(imagePath);
        sequence.push_back(img);
        ofLogNotice() << imagePath;
    }
    
    currFrame = 0;
    isPlayingBackward = false;
}

void FrameSequence::checkBoundary() {
    if (!isPlayingBackward && currFrame + 1 == sequence.size()) {
        isPlayingBackward = true;
    } else if (isPlayingBackward && currFrame == 0) {
        isPlayingBackward = false;
    }
}

void FrameSequence::nextFrame() {
    if (isPlayingBackward) {
        currFrame--;
    } else {
        currFrame++;
    }
    
    checkBoundary();
}

bool FrameSequence::getIsPlayingBackward() {
    return isPlayingBackward;
}

float FrameSequence::getProgress() {
    return (float)currFrame / (float)sequence.size();
}

int FrameSequence::getCurrFrame() {
    return currFrame;
}

ofImage FrameSequence::getCurrImage() {
    return sequence[currFrame];
}

void FrameSequence::setProgress(float progress, bool _isPlayingBackward) {
    
    currFrame = round(progress * (sequence.size() - 1));
    
    ofLogNotice() << "currFrame" << ofToString(currFrame) << "setProgress" << progress << ofToString(progress) << " _isPlayingBackward: " << ofToString(_isPlayingBackward);
    isPlayingBackward = _isPlayingBackward;
    
    checkBoundary();
}
