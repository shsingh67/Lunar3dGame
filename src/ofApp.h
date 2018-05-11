
#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ofxGui.h"
#include "box.h"
#include "ray.h"

class Node {
public:
    Box box;
    vector<Node> children;
    vector<int> vertices;
    
    
    Node(){}
    Node(Box box1, vector<int> param3, vector<Node> child){
        box = box1;
        vertices = param3;
        children = child;
        
    }
    
    vector<Node> getChildren(){
        return children;
    }
    
    vector<int> getIndexList(){
        return vertices;
    }
    
    Box getBox(){
        return box;
    }
    
    void addChild(Node& node){
        children.push_back(node);
    }
    
    
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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void loadVbo();
        void drawBox(const Box &box);
        Box meshBounds(const ofMesh &);
        void subDivideBox8(const Box &b, vector<Box> & boxList);

    
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
    
        // textures
        //
        ofTexture  particleTex;
    
        // shaders
        //
        ofVbo vbo;
        ofShader shader;
    
    
        int count = 0;
        int down = false;
    
        //Tree
    Box boundingBox;
        Node root;
        void createOctree(Node& root,int numLevels,int level);
    
        ofMesh mesh;
    
        vector<ofVec3f> boxVertices(Box, vector<ofVec3f>);
        bool contains(Box,ofVec3f);
    
        vector<int> indexList;
        void draw(Node & node, int numLevels, int level);
        int getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
                           Box & box, vector<int> & pointsRtn);
        const float selectionRange = 4.0;
    
        bool intersect(Ray ray, Node node);
    
        bool collision(ofVec3f position,  Node node, int numLevels, int level);
    
        bool bCollision;
    
    
    
};

