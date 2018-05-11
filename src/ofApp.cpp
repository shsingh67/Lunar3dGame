
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
    
    // texture loading
    //
    ofDisableArbTex();     // disable rectangular textures
    
    // load textures
    //
    if (!ofLoadImage(particleTex, "images/dot.png")) {
        cout << "Particle Texture File: images/dot.png not found" << endl;
        ofExit();
    }
    
   //load shader
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
	thrust.loadSound("sounds/thrust.mp3");
	thrust.setLoop(true);
    
    // Create a lonely parcitle for and a thrust force to it
    ship.lifespan = 10000;
    ship.visible = false;
    ship.position.set(0,20,0);
    sys.add(ship);
    
    sys.addForce(&thruster);
    sys.addForce(new GravityForce(ofVec3f(0, -.162, 0)));
   
    //set the type of explosion for the engine emitter
    engine.setLifespan(.7);
    engine.setParticleRadius(10);
    engine.sys->addForce(new ImpulseRadialForce(110));
    engine.setOneShot(true);
    engine.setGroupSize(100);
    engine.setVelocity(ofVec3f(0,0,0));
    engine.sys->addForce(new GravityForce(ofVec3f(0, -6, 0)));

	cam.setPosition(20, 20, 20);
	cam.lookAt(ofVec3f(0,20,0));
    
    // setup for octTree
    boundingBox = meshBounds(moon.getMesh(0));
    bCollision = false;
    
    mesh = moon.getMesh(0); // getting the mesh from mars data
    int numIndex = mesh.getNumIndices();
    int numberOfVertex = mesh.getNumVertices();
    for(int i = 0; i < numberOfVertex;i++){
        indexList.push_back(i);
    }
    
    
    
    // cout << mesh.getVertex(indexList.at(100000));
    root.box = boundingBox;
    vector<Node> children1;
    root.children = children1;
    root.vertices = indexList;
    createOctree(root,12,1);
    cout<< "created octree"<<endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(!bCollision) {
    loadVbo();
    
	landerX = lander.getPosition().x;
	landerY = lander.getPosition().y;
	landerZ = lander.getPosition().z;

    count++;
    
    sys.update();

    if(!down) {
        if(count == 65) {
            engine.sys->reset();
            engine.start();
        }
        if(count >= 65) {
            count = 0;
        }
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
     bool val = collision(lander.getPosition(),root, 11, 1);
        
    } else {
        ship.setVelocity(ofVec3f(0,0,0));
        thruster.set(ofVec3f(0,0,0));
        engine.stop();
        //bCollision = false;
    }
 
}

//--------------------------------------------------------------
void ofApp::draw(){
    
   // loadVbo();
    ofBackground(ofColor::black);
    
    if(!bHide) {
        gui.draw();
    }
    
    cam.begin();
    ofPushMatrix();
    ofSetColor(ofColor::dimGrey);
    sys.draw();
   // engine.draw();
   

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
    
    glDepthMask(GL_FALSE);
    
    ofSetColor(255, 100, 90);
    
    // this makes everything look glowy :)
    //
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();

    
    // begin drawing in the camera
    //
    shader.begin();
    cam.begin();

    // draw particle emitter here..
    //
    particleTex.bind();
    vbo.draw(GL_POINTS, 0, (int)engine.sys->particles.size());
    particleTex.unbind();

    //  end drawing in the camera
    //
    cam.end();
    shader.end();

    ofDisablePointSprites();
    ofDisableBlendMode();
    ofEnableAlphaBlending();

    // set back the depth mask
    //
    glDepthMask(GL_TRUE);

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
            down = true;
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
    switch(key) {
        case OF_KEY_DOWN:
    down = false;
    break;
    }
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
//----------------------------------------------------------
// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
    if (engine.sys->particles.size() < 1) return;
    
    vector<ofVec3f> sizes;
    vector<ofVec3f> points;
    for (int i = 0; i < engine.sys->particles.size(); i++) {
        points.push_back(engine.sys->particles[i].position);
        sizes.push_back(ofVec3f(engine.sys->particles[i].radius));
    }
    // upload the data to the vbo
    //
    int total = (int)points.size();
    vbo.clear();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
//-----------------------
void ofApp::createOctree(Node& rootNode, int numLevels, int level){
    if (level >= numLevels) return;
    vector<Box> child;
    subDivideBox8(rootNode.box, child);
    for(int i = 0; i < child.size();i++){
        Box childBox = child.at(i);
        vector<Node> emptyChildren;
        vector<int> childVertices;
        int count = getMeshPointsInBox(mesh, rootNode.vertices, childBox, childVertices);
        
        if(count >= 30){
            Node* node = new Node(childBox,childVertices,emptyChildren);
            rootNode.addChild(*node);}
    }
    level++;
    for(int i = 0; i <rootNode.children.size();i++){
        createOctree(rootNode.children.at(i), numLevels, level);
    }
}
//------------------------
// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}
//------------------------
//draw a box from a "Box" class
//
void ofApp::drawBox(const Box &box) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
    float w = size.x();
    float h = size.y();
    float d = size.z();
    ofDrawBox(p, w, h, d);
}
//--------------------------
//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
// takes box, and returns 8 boxes in a vector
//
void ofApp::subDivideBox8(const Box &box, vector<Box> & boxList) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);
    
    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));
    
    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);
    
    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }
    
    
}
//-----------------------------// getMeshPointsInBox:  return an array of indices to points in mesh that are contained
//                      inside the Box.  Return count of points found;

int ofApp::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
                              Box & box, vector<int> & pointsRtn)
{
    int count = 0;
    for (int i = 0; i < points.size(); i++) {
        ofVec3f v = mesh.getVertex(points[i]);
        if (box.inside(Vector3(v.x, v.y, v.z))) {
            count++;
            pointsRtn.push_back(points[i]);
        }
    }
    return count;
}

//---------------------------
bool ofApp::collision(ofVec3f position,  Node node, int numLevels, int level ) {
    
    
    if (level >= numLevels) return;
    
    if(level == 10 ){
        if(node.box.inside(Vector3(position.x, position.y, position.z))){
            bCollision = true;
        } else {
        // bCollision = false;
        }
        
    }
    level++;
    
    for (int i = 0; i < node.children.size(); i++) {
        collision(position, node.children[i], numLevels, level);
    }
    return false;
}


