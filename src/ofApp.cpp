#include "ofApp.h"

string KISS_VID_FOLDER = "kiss_vids/";
int FPS = 15;
int MAX_FRAMES = 22;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(FPS);
    
    ofDirectory dir(KISS_VID_FOLDER);
    dir.allowExt("mp4");

    dir.listDir();

    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string vidPath = dir.getPath(i);
        
        ofVideoPlayer vidPlayer;
        vidPlayer.load(vidPath);
        vidPlayer.setSpeed(-1);
//        vidPlayer.setLoopState(OF_LOOP_PALINDROME);
//        vidPlayer.play();
        videos.push_back(vidPlayer);
        

        ofLog(OF_LOG_NOTICE) << "Loaded: " << vidPath << "; frameCount: " << ofToString(vidPlayer.getTotalNumFrames());
    }
//
    int randomVideoIndex = (int) ofRandom(videos.size());
    currVideo = videos[randomVideoIndex];
    currVideo.play();
    
    expectedFrame = -1;
    hasHandledSwitch = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (ofRandom(1.0) > 0.95) {
        switchVideo();
    }
    
    
    if (isPlayingBackward && currVideo.getCurrentFrame() <= 0) {
        currVideo.setSpeed(1);
        isPlayingBackward = false;
    }
    if (!isPlayingBackward && currVideo.getCurrentFrame() >= currVideo.getTotalNumFrames() - 1) {
        currVideo.setSpeed(-1);
        isPlayingBackward = true;
    }
    if (isPlayingBackward && nextVideo.getCurrentFrame() <= 0) {
        nextVideo.setSpeed(1);
        isPlayingBackward = false;
    }
    if (!isPlayingBackward && nextVideo.getCurrentFrame() >= nextVideo.getTotalNumFrames() - 1) {
        nextVideo.setSpeed(-1);
        isPlayingBackward = true;
    }
    


    currVideo.update();
    nextVideo.update();
    
    if (!hasHandledSwitch && abs(currVideo.getCurrentFrame() - nextVideo.getCurrentFrame()) < 4) {
        currVideo.stop();
        currVideo = nextVideo;
        
        hasHandledSwitch = true;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    currVideo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofLogNotice() << "Curr frame: " << ofToString(currVideo.getCurrentFrame());
    ofLogNotice() << "Next frame: " << ofToString(nextVideo.getCurrentFrame());

}

void ofApp::switchVideo() {
    int randomVideoIndex = (int) ofRandom(videos.size());
    nextVideo = videos[randomVideoIndex];
    
    int currFrame = currVideo.getCurrentFrame();
    float progress = (float)currFrame / (float)currVideo.getTotalNumFrames();
    ofLogNotice() << "CURR VIDEO frame on switch: " << ofToString(currFrame);

//
//    currVideo.stop();
//    currVideo = nextVideo;
    
    nextVideo.play();
    nextVideo.setPaused(true);
    nextVideo.setFrame(currFrame);
    
    hasHandledSwitch = false;
    expectedFrame = currFrame;
   
    
    
    if (isPlayingBackward) {
        nextVideo.setSpeed(-1);
//        currVideo.previousFrame();
    } else {
        nextVideo.setSpeed(1);
//        currVideo.nextFrame();
    }
    
//    currVideo.play();
    nextVideo.update();
//    currVideo.update();
//    currVideo.setFrame(currFrame);
}

void ofApp::skipFrames(int setFrames) {
    unsigned int currentFrame = currVideo.getCurrentFrame();
    int frames = setFrames - currentFrame;

    if(frames > 0) {
        for(unsigned int i = currentFrame; i < currentFrame + frames; i++) {
            currVideo.setPaused(false);
            currVideo.nextFrame();
            currVideo.update();
            currVideo.setPaused(true);
        }
    } else if(frames < 0) {
        for(unsigned int i = currentFrame; i > currentFrame + frames; i--) {
            currVideo.setPaused(false);
            currVideo.previousFrame();
            currVideo.update();
            currVideo.setPaused(true);
        }
    }
    
    currVideo.play();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key ==  ' ') {
        switchVideo();
    }
    
    if (key == OF_KEY_LEFT) {
        currVideo.setPaused(true);
        currVideo.setFrame(10);
        currVideo.play();
        ofLogNotice() << "New curr frame: " << ofToString(currVideo.getCurrentFrame());
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
