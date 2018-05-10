#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ofxGui.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    
        ofEasyCam cam;

        ofxAssimpModelLoader moon, lander;
    
        bool bAltKeyDown;
        bool bCtrlKeyDown;
        bool bWireframe;
        bool bDisplayPoints;
        bool bPointSelected;
        bool bHide;
    
        bool bLanderLoaded;
        bool bTerrainSelected;
    
        void initLightingAndMaterials();

    
        Particle ship;
        ParticleEmitter engine;
        ParticleSystem sys;
        ThrusterForce thruster;
        GravityForce gravityForce;

		ofSoundPlayer thrust;
    
        ofxPanel gui;
        ofxFloatSlider gravity;

		float currentCam;
		float landerX;
		float landerY;
		float landerZ;
    
        int count = 0;
};
