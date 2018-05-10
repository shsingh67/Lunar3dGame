
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //slider setup
//    gui.setup();
//    gui.add((gravity.setup("Gravity", 10, 1, 20)));
//    bHide = false;
    
    bWireframe = false;
    bDisplayPoints = false;
//    bAltKeyDown = false;
//    bCtrlKeyDown = false;
    bLanderLoaded = false;
    bTerrainSelected = true;
    cam.setDistance(10);
    cam.setNearClip(.1);
    cam.setFov(65.5);

    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableDepthTest();
    
    initLightingAndMaterials();
    moon.loadModel("geo/moon-houdini.obj");
    moon.setScaleNormalization(false);
    
    lander.loadModel("geo/lander.obj");
    lander.setScaleNormalization(false);

	thrust.loadSound("sounds/thrust.mp3");
	thrust.setLoop(true);
    
    // Create a lonely parcitle for and a thrust force to it
    ship.lifespan = 10000;
    ship.position.set(0,20,0);
    sys.add(ship);
    
    sys.addForce(&thruster);
    sys.addForce(new GravityForce(ofVec3f(0, -.162, 0)));
    //sys.addForce(&gravityForce);
    
    //set the type of explosion for the engine emitter
    engine.setLifespan(.7);
    engine.setParticleRadius(.04);
    engine.sys->addForce(new ImpulseRadialForce(150));
    engine.setOneShot(true);
    engine.setGroupSize(200);
    engine.setVelocity(ofVec3f(0,0,0));
    engine.sys->addForce(new GravityForce(ofVec3f(0, -6, 0)));

	cam.setPosition(20, 20, 20);
	cam.lookAt(ofVec3f(0,20,0));
	
}

//--------------------------------------------------------------
void ofApp::update(){
	landerX = lander.getPosition().x;
	landerY = lander.getPosition().y;
	landerZ = lander.getPosition().z;

    count++;
    
    sys.update();
    
    if(count == 65) {
    engine.sys->reset();
    engine.start();
    count = 0;
    }
    
    engine.update();
    
    engine.setPosition(sys.particles[0].position);
    lander.setPosition(sys.particles[0].position.x, sys.particles[0].position.y, sys.particles[0].position.z);

	if (currentCam == 1) {
		
	}
	if (currentCam == 2) {
		cam.setPosition(20, 20, 20);
		cam.lookAt(lander.getPosition());
	}
	if (currentCam == 3) {
		cam.setPosition(lander.getPosition());
		cam.lookAt(ofVec3f(landerX, 0, landerZ));
	}
	if (currentCam == 4) {
		cam.setPosition(ofVec3f(landerX+1, landerY+5, landerZ+1));
		cam.lookAt(ofVec3f(landerX+50, landerY, landerZ+50));
	}


}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
    
    if(!bHide) {
        gui.draw();
    }
    
    cam.begin();
    ofPushMatrix();
    ofSetColor(ofColor::dimGrey);
    sys.draw();
    engine.draw();
    
    if(bWireframe) {
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        moon.drawWireframe();
        
        //code for loading rover
        lander.drawWireframe();
    
    } else {
        ofEnableLighting();
        moon.drawFaces();
        //
        lander.drawFaces();
    }
    
    ofPopMatrix();
    cam.end();


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
    switch (key) {
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case OF_KEY_UP:
			thrust.play();
            thruster.add(ofVec3f(0, .5, 0));
            engine.sys->reset();
            engine.start();
            break;
        case OF_KEY_DOWN:
			thrust.play();
            thruster.add(ofVec3f(0, -.5, 0));
            break;
        case OF_KEY_LEFT:
			thrust.play();
            thruster.add(ofVec3f(-.5, 0, 0));
            break;
        case OF_KEY_RIGHT:
			thrust.play();
            thruster.add(ofVec3f(.3, 0, 0));
            break;
            
        case OF_KEY_LEFT_SHIFT:
			thrust.play();
            thruster.add(ofVec3f(0, 0, -5));
            break;
            
        case OF_KEY_RIGHT_SHIFT:
			thrust.play();
            thruster.add(ofVec3f(0, 0, 3));
            break;
        case 'h':
            break;
		case '1':
			currentCam = 1;
			break;
		case '2':
			currentCam = 2;
			break;
		case '3':
			currentCam = 3;
			break;
		case '4':
			currentCam = 4;
			break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    thruster.set(ofVec3f(0, 0, 0));
	thrust.stop();
   // engine.stop();
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

//---------------------------------------------------------------

void ofApp::initLightingAndMaterials() {
    
    static float ambient[] =
    { .5f, .5f, .5, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };
    
    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float lmodel_twoside[] =
    { GL_TRUE };
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}


