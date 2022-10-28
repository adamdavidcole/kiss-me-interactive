#include "ofApp.h"

string KISS_VID_FOLDER = "kiss_vids/";
string KISS_FRAME_SEQUENCE_FOLDER = "frameSequences/";
string KISS_FRAME_SEQUENCE_FOLDER_DEV = "frameSequences_dev";
vector<string> intensityFolders = {"low", "medium", "high" };

bool IS_DEV = false;
bool USE_SERIAL_INPUT = false;

int FPS_START = 15;
int MAX_FRAMES = 22;

//--------------------------------------------------------------
void ofApp::setup(){
    std::cout << "App is starting up" << std::endl;
    currFPS = FPS_START;
    ofSetFrameRate(currFPS);
    
    string frameSequenceFolder = IS_DEV ? KISS_FRAME_SEQUENCE_FOLDER_DEV :KISS_FRAME_SEQUENCE_FOLDER;
    
    ofLogNotice() << "Using frameSequenceFolder: " << frameSequenceFolder;
     
    
    for (int j = 0; j < 3; j++) {
        ofLogNotice() << "Begin new frame sequence bucket: " << ofToString(j);
        vector<FrameSequence*>* sequenceBucket = new vector<FrameSequence*>;
        string directory = frameSequenceFolder + "/" + intensityFolders[j];
        
        ofLogNotice() << "Intensity directory: " << directory;
        
        ofLogNotice() << directory;
        ofDirectory dir(directory);
        dir.listDir();
        
        ofLogNotice() << "Size of dir: " << dir.size();
        
        // Create bucket from all sequences in directory
        for(int i = 0; i < dir.size(); i++) {
            string imageFolder = dir.getPath(i);
            ofLogNotice() << "imageFolder" << imageFolder;
            FrameSequence* seq = new FrameSequence(imageFolder);
            ofLogNotice() << "Push to bucket";
            sequenceBucket->push_back(seq);
            ofLogNotice() << "Done pushing to bucket";
        }
        
       
        
        // Add sequences from previous bucket to this bucket
        if (j > 0) {
            vector<FrameSequence*> prevBucket = *sequenceBuckets[j-1];
            for (FrameSequence* seq : prevBucket) {
                sequenceBucket->push_back(seq);
            }
        }
        
        sequenceBuckets.push_back(sequenceBucket);
        ofLogNotice() << "Size of new bucket: " << sequenceBucket->size();
    }
    
   
   ofLogNotice() << "Completed loading all frame sequences";
    
   ofLogNotice() << "sequenceBucketsSize: " << sequenceBuckets.size();
    
    vector<FrameSequence*>* firstBucketPtr = sequenceBuckets[0];
    
    ofLogNotice() << "firstBucketPtr set";
    
    
    currSequence = firstBucketPtr->at(0);
    
    ofLogNotice() << "currSequence set";
    
    currSequenceBucket = firstBucketPtr;
    
    ofLogNotice() << "Initialized curr sequence";
    
   
    changeSequenceProbability = 0;
    jumpCutProbability = 0;
    


    
    updateDrawCoords();

     
    
    if (USE_SERIAL_INPUT) {
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
        for (ofSerialDeviceInfo device : deviceList) {
            ofLogNotice() << device.getDeviceName() << ", " << device.getDeviceID();
        }
        
        // this should be set to whatever com port your serial device is connected to.
        // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
        // arduino users check in arduino app....
        int baud = 9600;
        //    serial.setup("/dev/cu.usbmodem.1201 (Arduino Uno)", baud); //open the first device
        //serial.setup("COM4", baud); // windows example
        //serial.setup("tty.usbmodem11201", baud); // mac osx example
        serial.setup("/dev/ttyACM0", baud); //linux example
    }
   
  
}

void ofApp::exit() {
    if (sequenceBuckets.size() > 0) {
        for (vector<FrameSequence*>* bucket : sequenceBuckets) {
            for (FrameSequence* seq : *bucket) {
                delete seq;
            }
            delete bucket;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    ofLogNotice() << "update wtf";
    
    if (ofRandom(1.0) < changeSequenceProbability) {
        switchSequence();
    }

    if (ofRandom(1.0) < jumpCutProbability) {
        jumpCut();
    }

    if (USE_SERIAL_INPUT) {
        readSensorValue();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofLogNotice() << "drawz: " << ofToString(drawX) << ", drawY: " << drawY << ", drawWidth: " << drawWidth << ", drawHeight: " << drawHeight;
    
    currSequence->nextFrame();
    ofImage img = currSequence->getCurrImage();
    
    ofLogNotice() << "img width: " << img.getWidth();

    img.draw(drawX, drawY, drawWidth, drawHeight);
}

void ofApp::switchSequence() {
    int randomSequenceIndex = (int) ofRandom(currSequenceBucket->size());
    FrameSequence* nextSequence = currSequenceBucket->at(randomSequenceIndex);
    

    float currProgress = currSequence->getProgress();
    bool currIsPlayingBackward = currSequence->getIsPlayingBackward();
    
//    ofLogNotice()  << "switchSequence: " << currProgress;
    
    nextSequence->setProgress(currProgress, currIsPlayingBackward);
    currSequence = nextSequence;
}

void ofApp::jumpCut() {
    float nextProgress = ofRandom(0, 0.9);
    bool isPlayingBackward = ofRandom(1.0) > 0.5;
    currSequence->setProgress(nextProgress, isPlayingBackward);
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
    
    
    if (!USE_SERIAL_INPUT) {
        if (key == '1') currSequenceBucket = sequenceBuckets[0];
        if (key == '2') currSequenceBucket = sequenceBuckets[1];
        if (key == '3') currSequenceBucket = sequenceBuckets[2];
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------


void ofApp::mouseMoved(int x, int y ){
    if (!USE_SERIAL_INPUT) {
        changeSequenceProbability = ofMap(x, 0, ofGetWidth(), 0, 1);
        jumpCutProbability = ofMap(y, 0, ofGetHeight(), 0, 1);
    }
}

void ofApp::readSensorValue() {
    if (!USE_SERIAL_INPUT) return;
    
    if (serial.available() < 0) {
       sensorValue = "Arduino Error";
    } else {
       //While statement looping through serial messages when serial is being provided.
       while (serial.available() > 0) {
           //byte data is being writen into byteData as int.
           byteData = serial.readByte();
       
           //byteData is converted into a string for drawing later.
//           sensorValue = "value: " + ofToString(byteData);
       }
        
        float sesnorValueMapped = ofMap(byteData, 0, 255, .015, 0.21);
        ofLogNotice() << "Mapped sensor value: " << ofToString(sesnorValueMapped);
        
        changeSequenceProbability = sesnorValueMapped;
        jumpCutProbability = sesnorValueMapped;
        
        vector<FrameSequence*>* prevSequenceBucket = currSequenceBucket;
        
        if (sesnorValueMapped < 0.05) {
            currSequenceBucket = sequenceBuckets[0];
        } else if (sesnorValueMapped < 0.2)  {
            currSequenceBucket = sequenceBuckets[1];
        } else {
            currSequenceBucket = sequenceBuckets[2];
        }
        
        if (prevSequenceBucket != currSequenceBucket) {
            switchSequence();
        }
   }
   cout << sensorValue << endl; // output the sensorValue
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

void ofApp::updateDrawCoords() {
    int currWidth = ofGetWidth();
    int currHeight = ofGetHeight();
    int size = std::min(currWidth, currHeight);
    
    drawWidth = size;
    drawHeight = size;
    drawX = (currWidth / 2) - (size / 2);
    drawY = (currHeight / 2) - (size / 2);
}

void ofApp::windowResized(int w, int h){
    updateDrawCoords();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

