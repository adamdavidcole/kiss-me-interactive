#pragma once

#include "ofMain.h"
#include "FrameSequence.hpp"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void exit();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        FrameSequence* currSequence;
        vector<FrameSequence*> sequences;
        
        vector<vector<FrameSequence*>*> sequenceBuckets;
        vector<FrameSequence*>* currSequenceBucket;

        int totalSequences;
        void switchSequence();
        void jumpCut();
        void switchDirection();
        
        int currFPS;
        
        float changeSequenceProbability;
        float jumpCutProbability;
        int intensityBucketsToUseCount;
    
        int drawX;
        int drawY;
        int drawWidth;
        int drawHeight;
        void updateDrawCoords();
    
        ofSerial serial;
        string sensorValue;
        int byteData;
        void readSensorValue();
    
};
