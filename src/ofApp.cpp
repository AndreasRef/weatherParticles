#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackgroundHex(0x00000);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    box2d.createBounds();
    
    ofColor color;
    color.set(255);
    
    //See https://google.github.io/liquidfun/API-Ref/html/b2_particle_8h.html for particle types
    //Also https://google.github.io/liquidfun/Programmers-Guide/html/md__chapter11__particles.html#pb
    particles.setParticleFlag(b2_tensileParticle);
    particles.loadImage("particle32.png");
    //particles.setup(<#b2World *b2world#>, <#int maxCount#>, <#float lifetime#>, <#float radius#>, <#float particleSize#>, <#ofColor color#>)
    particles.setup(box2d.getWorld(), 200000, 60.0, 6.0, 42.0, color);
    
    
    //GUI
    clearButton.addListener(this, &ofApp::clearButtonPressed);
    
    gui.setup();
    gui.setPosition(10,100);
    
    gui.add(particleType.setup("particleType", 0, 0, 11));
    gui.add(particlesPerDrag.setup("particlesPerDrag", 20, 2, 100));
    gui.add(dragRadius.setup("dragRadius", 20, 2, 100));
    gui.add(dragSpread.setup("dragSpread", 20, 2, 100));
    gui.add(particleLifeSpan.setup("particleLifeSpan", 5, 1, 60));
    gui.add(xGravity.setup("xGravity", 0, -3, 3));
    gui.add(yGravity.setup("yGravity", 0, -3, 3));
    gui.add(particleSize.setup("particleSize", 42, 6, 80));
    gui.add(blendModeAdd.setup("blendModeAdd", true));
    gui.add(clearButton.setup("clear"));
    
    
    bHide = false;
    
    gui.loadFromFile("settings.xml");
    
    particleList = ofSplitString(
                         "b2_waterParticle;b2_wallParticle;b2_springParticle;b2_elasticParticle;b2_viscousParticle;b2_powderParticle;b2_tensileParticle;b2_colorMixingParticle;b2_barrierParticle;b2_staticPressureParticle;b2_reactiveParticle;b2_repulsiveParticle", ";" );
    
}

//--------------------------------------------------------------
void ofApp::clearButtonPressed(){
    //Don't know how to clear the particles yet...
    ofLog(OF_LOG_NOTICE, "clear");
    
    for(int i=0; i<circles.size(); i++) {
        
        circles[i].get()->destroy();
    }
}


//--------------------------------------------------------------
void ofApp::update() {
    box2d.update();
    box2d.setGravity(xGravity,yGravity);
    particles.setParticleLifetime(particleLifeSpan);
    
    //ofLog(OF_LOG_NOTICE, particleList[particleType]);
    
    switch(particleType){
        case 0:
            particles.setParticleFlag(b2_waterParticle );
            break;
        case 1:
            particles.setParticleFlag(b2_wallParticle );
            break;
        case 2:
            particles.setParticleFlag(b2_springParticle);
            break;
        case 3:
            particles.setParticleFlag(b2_elasticParticle);
            break;
        case 4:
            particles.setParticleFlag(b2_viscousParticle);
            break;
        case 5:
            particles.setParticleFlag(b2_powderParticle);
            break;
        case 6:
            particles.setParticleFlag(b2_tensileParticle);
            break;
        case 7:
            particles.setParticleFlag(b2_colorMixingParticle);
            break;
        case 8:
            particles.setParticleFlag(b2_barrierParticle );
            break;
        case 9:
            particles.setParticleFlag(b2_staticPressureParticle);
            break;
        case 10:
            particles.setParticleFlag(b2_reactiveParticle );
            break;
        case 11:
            particles.setParticleFlag(b2_repulsiveParticle);
            break;
            
    }
    
    //particles.setRadius(6); //Does not have any effect?
    
    particles.particleSize = particleSize;
    //particles.setColor(ofColor(0, ofRandom( 0, 255 ), ofRandom( 0, 255 )));
    
}


//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor(0), ofColor(63), OF_GRADIENT_LINEAR);
    
    for(int i=0; i<circles.size(); i++) {
        ofFill();
        ofSetHexColor(0xf6c738);
        circles[i].get()->draw();
    }
    
    
    if (blendModeAdd) ofEnableBlendMode(OF_BLENDMODE_ADD);

    particles.draw();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    string info = "";
    info += "Mouse Drag for particles\n";
    info += "Total Particles: "+ofToString(particles.getParticleCount())+"\n\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    info += particleList[particleType]+"\n";
    ofSetHexColor(0xffffff);
    ofDrawBitmapString(info, 30, 30);
    
    
    if(!bHide){
        gui.draw(); //Hide gui
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLog(OF_LOG_NOTICE, ofToString(key));
    
    if(key == 'h'){
        bHide = !bHide;
    }
    else if(key == 's'){
        gui.saveToFile("settings.xml");
    }
    else if(key == 'l'){
        gui.loadFromFile("settings.xml");
    }
    else if(key == 't'){
        particles.loadImage("particle32.png");
    }
    
    else if(key >= 48 && key <= 57){ //Key '0' has value 48, 1 has 49 etc
        particles.loadImage("particle" + ofToString(key-48) + ".png");
    }
    
    if(key == 'c') {
        float r = ofRandom(30, 40);
        circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(0.9, 0.9, 0.1);
        circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
    //if (ofGetKeyPressed()) {
    
    for (int i = 0; i < particlesPerDrag; i++) {
        float radius = ofRandom(dragRadius, dragRadius + dragSpread);
        float x = cos(ofRandom(PI * 2.0)) * radius + mouseX;
        float y = sin(ofRandom(PI * 2.0)) * radius + mouseY;
        ofVec2f position = ofVec2f(x, y);
        ofVec2f velocity = ofVec2f(0, 0);
        ofColor color;
        int hue = int(ofGetFrameNum() / 4.0) % 255;
        color.setHsb(hue, 180, 200);
        particles.setColor(color);
        particles.createParticle(position, velocity);
    }
   // }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::resized(int w, int h){
}

