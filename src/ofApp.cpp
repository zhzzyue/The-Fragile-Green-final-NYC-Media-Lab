#include "ofApp.h"

#include <iostream>

ofSoundPlayer 		bgsong;
float avgSound;
float* fftSmoothed;
float* val;
int nBandsToGet;
int byteData;
ofFbo fbo;
ofColor color;
float n;



void ofApp::setup(){

    ofSetWindowShape(1024,768);
    
    ofSetWindowTitle("ofxSyphon Green");
    
    mainOutputSyphonServer.setName("Green Screen Output");
    individualTextureSyphonServer.setName("Green Texture Output");
    mClient.set("","Simple Server");
     tex.allocate(200, 100, GL_RGBA);
    //sound
    waterSound.load("water.mp3");
    birdSound.load("bird.mp3");
    cicadeSound.load("Cicada.mp3");
    grassSound.load("grass.mp3");
    //sound loop
    waterSound.setLoop(true);
    birdSound.setLoop(true);
    cicadeSound.setLoop(true);
    grassSound.setLoop(true);

    //General setup of look of window.
    
    
    ofSetColor(0);
    font.load("Verdana.ttf", 48);
    font1.load("Verdana.ttf", 24);

    //serial port setup. using COM3 for Windows port.
    //Also using baud rate 9600, same in Arduino sketch.
    serial.setup("/dev/cu.HC-05-DevB", 9600);

    
    bgsong.load("bg music.mp3");
    bgsong.setLoop(true);
    

    
    while (! bgsong.isLoaded()); // spin wheels till loaded
    nBandsToGet = 512;  // up to 512
    
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    //
        bgsong.play();
    ofSetFrameRate(60); // Limit the speed of our program to 60 frames per second
    ofBackground(0);

    
    state = 0;
    
    //    for (curAgent; curAgent < MAX_AGENTS; curAgent++) {  // only enable if we disable mouse gravity
    //        Agents[curAgent].start(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()));
    //    }
}

//-------------------------------------------------------------------------------------
void ofApp::update(){
    
    
    //Simple if statement to inform user if Arduino is sending serial messages.
    if (serial.available() < 0) {
        msg = "Arduino Error";
    }
    else {
        //While statement looping through serial messages when serial is being provided.
        while (serial.available() > 0) {
            //byte data is being writen into byteData as int.
            byteData = serial.readByte();
            
            //byteData is converted into a string for drawing later.
//            msg = "cm: " + ofToString(byteData);
        }
    }
    
    //state & dist setting
//    if(byteData < 97){
////        ofSetColor(0);
//        state = 0; //if nothing is detected
//    
//    }else
        if(byteData == 97){
        ofSetColor(255, 0, 0);//red
        state = 1;
        
    }else if(byteData == 98){
        ofSetColor(0,255,0);//green
        state = 2;
    }else if(byteData == 99){
        ofSetColor(0, 0, 255);//blue
        state = 3;
    }else if(byteData == 100){
        ofSetColor(255, 255, 0);//yellow
        state = 4;
    }
    
//    cout << state << endl;



    
    
    ofSoundUpdate();
    val = ofSoundGetSpectrum(nBandsToGet);
    for (int i = 0;i < nBandsToGet; i++){
        fftSmoothed[i] *= 0.96f;
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];

    }
    

}



//-------------------------------------------------------------------------------------
using namespace std;		// this is so we can use cout

#define MAX_AGENTS 500		// how many agents we can have max
agent Agents[MAX_AGENTS];	// storage (memory) space for all the unique details of every agent

// constructor
agent::agent(){
    
    
    
    velDir.set(0, ofRandom(.5, 2));	// init random speed
    // cout<<fftSmoothed[1]*10<<endl;
    velDir.rotate(ofRandom(135,360));

}
//-------------------------------------------------------------------------------------

// update all variables in an agent
void agent::update(agent *arr){  // influence the main vector
    

//    velDir.set(0, fftSmoothed[1]*10);	// init random speed
//// cout<<fftSmoothed[1]*10<<endl;
//     velDir.rotate(ofRandom(135,360));

    

    
    
    ofVec2f noVec(-1,-1);
    
    if (!active)
        return;
    
    // Where are agents near you headed? Align with that.
    float alignDelta = calcAlignment(arr, MAX_AGENTS);	// set up aligment
    velDir.rotate(alignDelta*.09);			// apply alignment delta (weighted)
    
    
    float ChangeAngle = velDir.angle(mouse - location);
    velDir.rotate(ChangeAngle * 0.06);
    
    /*
     set mouse gravity here
     weight by .06
     }
     */
    float distance = mouse.distance(location);
    if (distance <= 60) {
        velDir.rotate(ChangeAngle * -10);
        location.x+=ofRandom(-10,10);
        location.y+=ofRandom(-10,10);
        
    }
    // For agents nearby, get centroid, head that way
    ofVec2f middle = calcCohesion(arr, MAX_AGENTS);	// get centroid of nearby mass of agents
    if (middle != noVec) {
        float alignMouse = velDir.angle(middle - location);	// delta between centroid & location gives us a VECTOR
        velDir.rotate(alignMouse*.02);			// apply centroid gravity (weighted)
    }
    
    // For too-close agents try to move away from them
    ofVec2f tooClose = calcDispersion(arr, MAX_AGENTS);			// get avg heading of too-close agents
    if (tooClose != noVec) {
        float tooCloseDelta;
        if (velDir.angle((tooClose - location)) < 0) 			// if it is on our left side
            tooCloseDelta = velDir.angle((tooClose - location).rotate(90));	// move opposite to mass
        else
            tooCloseDelta = velDir.angle((tooClose - location).rotate(-90));  // move opposite to mass
        velDir.rotate(tooCloseDelta*.1);					// apply centroid gravity (weighted)
    }
    
    location += velDir;	// move to current location
}
//-------------------------------------------------------------------------------------
void agent::draw(){

    
    // if it moves off the board, move it to opposite side
    if (location.x > ofGetWidth()) location.x = 0;
    if (location.x < 0) location.x = ofGetWidth();
    if (location.y > ofGetHeight()) location.y = 0;
    if (location.y < 0) location.y = ofGetHeight();
    
//    p1 += location;	// move to current location
//    p2 += location;
//    p3 += location;
//      ofDrawTriangle(p1, p2, p3);
    

    
    ofSetColor(255,255,153,50);
    ofDrawCircle(location.x, location.y, ofRandom(2,10));
    
    ofSetColor(102,255,153,200);
    ofDrawCircle(location.x, location.y, 2);
    
    
    
    
    
//    ofDrawCircle(mouse, 10);
}
//-------------------------------------------------------------------------------------

void agent::start(float x, float y){
    active = true; // turn this one on!
    location.set(x,y);
}


// Get relative angle from me -180 - 180
float agent::getRelAngle(ofVec2f neighbor){
    float angle = velDir.angle(neighbor);
    return (angle);
}

//-------------------------------------------------------------------------------------

// get mouse location for gravity
void agent::calcGravityWell(float x,float y){
    mouse.set(x,y);
}

// average & return neighbor's VECTORS
float agent::calcAlignment(agent *arr, int max){
    float sum = 0;  // sum of angles
    int sumNum = 0; // num of angles summed
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        if (distance < 150 && distance > 0 && arr[i].active) { 	// if a neighbor is close & active
            //cout  << " Dist: " << distance << "\n";
            sum += getRelAngle(arr[i].velDir);			// add its angle to sum
            sumNum++;							// count how many are summed
        }
    }
    if (sumNum > 0) {   // avoid div by 0
        return(sum/sumNum);
    } else
        return(0);
}

// average and return neighbor's LOCATIONS
ofVec2f agent::calcCohesion(agent *arr, int max){
    ofVec2f centroid, nearArr[MAX_AGENTS];				// centroid & arr of Neighbors near us
    int lenArr = 0;
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        
        if (distance < 70 && distance > 20 && arr[i].active) { 	// if a neighbor is close & active
            nearArr[lenArr] = arr[i].location;			// add its location to array
            lenArr++;							// count how many are added
        }
    }
    
    if (lenArr > 0) {
        centroid.average(nearArr, lenArr);	// average the location of all neighbor agents
        return (centroid);
    } else {
        ofVec2f noVec(-1,-1);
        return (noVec);  			// no nearby agents
    }
}

// average and return too-close neighbor's LOCATIONS
ofVec2f agent::calcDispersion(agent *arr, int max){
    ofVec2f centroid, nearArr[MAX_AGENTS];		// centroid & arr of Neighbors near us
    int lenArr = 0;
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        
        if (distance < 20 && arr[i].active) { 	// if a neighbor is close & active
            nearArr[lenArr] = arr[i].location;	// add its location to array
            lenArr++;					// count how many are added
        }
    }
    
    if (lenArr > 0) {
        centroid.average(nearArr, lenArr);		// average the location of all too-close neighbor agents
        return (centroid);
    } else {
        ofVec2f noVec(-1,-1);
        return (noVec);				// no nearby agents
    }
}




int curAgent = 0;			// what agent we are dealing with


//--------------------------------------------------------------
void ofApp::draw(){
    
    //draw state == 1
    if(state == 1){
      bool waterPlay = waterSound.isPlaying();
        if (waterPlay == false){
            waterSound.play();
            mouseMoved(825, 750);
        }
    } else {
        waterSound.stop();

    }
    //draw state == 2
    if(state == 2){
        bool cicadePlay = cicadeSound.isPlaying();
        if (cicadePlay == false){
            cicadeSound.play();
            mouseMoved(625, 750);
        }
    } else {
        cicadeSound.stop();

    }
    //draw state == 3
    if(state == 3){
        bool birdPlay = birdSound.isPlaying();
        if (birdPlay == false){
            birdSound.play();
            mouseMoved(410, 750);

        }
    } else {
        birdSound.stop();
    }
    //draw state == 4
    if(state == 4){
        bool grassPlay = grassSound.isPlaying();
        if (grassPlay == false){
            grassSound.play();
            mouseMoved(200, 750);
        }
    } else {
        grassSound.stop();
    }


    

    //drawing the string version pf byteData on oF window.
    font.drawString("The Fragile Green", 50, 400);
    font1.drawString("-Spirits are likely hiding in natural world", 50, 450);
//    font1.drawString("-Go Catch the Fireflies", 50, 450);
    font1.drawString("Go Catch the Fireflies", 325, 150);
    
    //printing byteData into console.
//    cout << state << endl;
    
    
    
    
    
    
//-------------------Agents Color---------------------------------
        for (curAgent; curAgent < MAX_AGENTS; curAgent++) {  // only enable if we disable mouse gravity
            Agents[curAgent].start(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()));
        }
    
    
        for (int i=0; i < MAX_AGENTS; i++) {
            Agents[i].update(Agents);
        }
    int alpha = 150;
    
    ofColor aqua(0, 252, 255, alpha);
    ofColor green(34, 139, 34, alpha);
    ofColor inbetween = aqua.getLerped(green, ofRandom(1.0));  // linear interpolation between colors color
    
    for (int i=0; i < MAX_AGENTS; i++) {
        ofSetColor(inbetween);
        Agents[i].draw();
    }
    

    
    mClient.draw(50, 50);
    
    mainOutputSyphonServer.publishScreen();
    individualTextureSyphonServer.publishTexture(&tex);
    

    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key){
        case 'f':
        ofToggleFullscreen();
        break;
    
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for (int i=0; i < MAX_AGENTS; i++) {
        Agents[i].calcGravityWell(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    Agents[curAgent].start(x, y);
    curAgent++;
    if (curAgent >= MAX_AGENTS) {  // make sure it does not go over
        curAgent = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
