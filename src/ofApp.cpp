#include "ofApp.h"

string KISS_VID_FOLDER = "kiss_vids/";
string KISS_FRAME_SEQUENCE_FOLDER = "frameSequences/";
string KISS_FRAME_SEQUENCE_FOLDER_DEV = "frameSequences_dev";

bool IS_DEV = true;

int FPS_START = 15;
int MAX_FRAMES = 22;

//--------------------------------------------------------------
void ofApp::setup(){
    currFPS = FPS_START;
    ofSetFrameRate(currFPS);
    
    string frameSequenceFolder = IS_DEV ? KISS_FRAME_SEQUENCE_FOLDER_DEV :KISS_FRAME_SEQUENCE_FOLDER;
    
    ofDirectory dir(frameSequenceFolder);
    dir.listDir();
    
    for(int i = 0; i < dir.size(); i++){
        string imageFolder = dir.getPath(i);
        FrameSequence seq(imageFolder);
        sequences.push_back(seq);
    }
    
    currSequence = sequences[0];
    changeSequenceProbability = 0;
    jumpCutProbability = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (ofRandom(1.0) < changeSequenceProbability) {
        switchSequence();
    }
    
    if (ofRandom(1.0) < jumpCutProbability) {
        jumpCut();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    currSequence.nextFrame();
    ofImage img = currSequence.getCurrImage();
    
    img.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::switchSequence() {
    int randomSequenceIndex = (int) ofRandom(sequences.size());
    FrameSequence nextSequence = sequences[randomSequenceIndex];
    
    float currProgress = currSequence.getProgress();
    bool currIsPlayingBackward = currSequence.getIsPlayingBackward();
    
    ofLogNotice()  << "switchSequence: " << currProgress;
    
    nextSequence.setProgress(currProgress, currIsPlayingBackward);
    currSequence = nextSequence;
}

void ofApp::jumpCut() {
    float nextProgress = ofRandom(0, 0.9);
    bool isPlayingBackward = ofRandom(1.0) > 0.5;
    currSequence.setProgress(nextProgress, isPlayingBackward);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key ==  ' ') {
        switchSequence();
    }
    
    if (key == OF_KEY_LEFT) {
        currFPS--;
        ofSetFrameRate(currFPS);

    }
    
    if (key == OF_KEY_RIGHT) {
        currFPS++;
        ofSetFrameRate(currFPS);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    changeSequenceProbability = ofMap(x, 0, ofGetWidth(), 0, 1);
    jumpCutProbability = ofMap(y, 0, ofGetHeight(), 0, 1);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
