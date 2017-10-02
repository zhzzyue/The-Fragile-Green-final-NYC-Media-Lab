#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxLeapMotion2.h"




class agent {
	public:
		agent();
		void update(agent *arr);
		void draw();
		void start(float X, float Y);
		void calcGravityWell(float x,float y);
	private:
		float getRelAngle(ofVec2f neighbor);
		float calcAlignment(agent *arr, int max);
		ofVec2f calcCohesion(agent *arr, int max);
		ofVec2f calcDispersion(agent *arr, int max);
		ofVec2f location;
		ofVec2f velDir; // velocity & direction for each step
		ofVec2f mouse;  //
    float n;


		bool active = false;	// if this agent is active
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    //sound effects
    ofSoundPlayer waterSound;
    ofSoundPlayer birdSound;
    ofSoundPlayer cicadeSound;
    ofSoundPlayer grassSound;
    

    
    //Custom variables for on screen string and font.
    string msg;
    
    
    ofTrueTypeFont font;
    ofTrueTypeFont font1;
    //New serial object.
    ofSerial serial;
    
    int state;
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    
    ofxSyphonClient mClient;
    
    ofTexture tex;
    //leapmotion
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    
    vector <int> fingersFound;
    ofEasyCam cam;

    
};
