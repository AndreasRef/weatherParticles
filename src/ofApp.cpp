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
    
    //Set initial values for global variables
    gravityX = 0;
    gravityY = 0;
    blendModeAdd = true;
    
    ofColor color;
    color.set(255);
    
    pColor.set(255,255,255);
    
    //See https://google.github.io/liquidfun/API-Ref/html/b2_particle_8h.html for particle types
    //Also https://google.github.io/liquidfun/Programmers-Guide/html/md__chapter11__particles.html#pb
    particles.setParticleFlag(b2_tensileParticle);
    particles.loadImage("particle32.png");
    //particles.setup(<#b2World *b2world#>, <#int maxCount#>, <#float lifetime#>, <#float radius#>, <#float particleSize#>, <#ofColor color#>)
    particles.setup(box2d.getWorld(), 200000, 60.0, 6.0, 42.0, color);
    
    
    //DatGui
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->addFRM();
    gui->addBreak();
    
    vector<string> opts = {"water", "wall", "spring", "elastic", "viscous", "powder","tensile", "colorMixing", "barrier", "staticPressure", "reactive", "repulsive"};
    gui->addDropdown("particle type", opts);
    gui->addBreak();

    
    s_particleLifeSpan = gui->addSlider("LIFESPAN", 1, 60, 20);
    s_dragAmount = gui->addSlider("DRAG AMOUNT", 1, 100, 20);
    s_dragRadius = gui->addSlider("DRAG RADIUS", 0, 200, 60);
    s_dragSpread = gui->addSlider("DRAG SPREAD", 0, 200, 20);
    s_particleSize = gui->addSlider("PARTICLE SIZE", 5, 64, 32);
    
    gui->addToggle("BLENDMODE ADD", true);
    gui->addButton("clear objects");
    
    p_gravityPad = gui->add2dPad("GRAVITY", ofRectangle(-1,-1,2,2));
    gui->addButton("reset gravity");
    gui->addColorPicker("PARTICLE COLOR", ofColor::fromHex(0xeeeeee));
    
    
    gui->addHeader(":: drag me to reposition ::");
    gui->addFooter();
    
    // register callbacks to listen for component specific events //
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onToggleEvent(this, &ofApp::onToggleEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    
}



//--------------------------------------------------------------
void ofApp::update() {
    box2d.update();
    
    
    //s_dragSpread-> setValue(ofGetMouseX()*0.15);
    
    box2d.setGravity(gravityX,gravityY);
    particles.setParticleLifetime(s_particleLifeSpan ->getValue());
    particles.particleSize = s_particleSize -> getValue();
    
    //    //particles.setRadius(6); //Does not have any effect?
    //    //particles.setColor(ofColor(0, ofRandom( 0, 255 ), ofRandom( 0, 255 )));

}


//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackgroundGradient(ofColor(0), ofColor(63), OF_GRADIENT_LINEAR);
    
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
    ofSetHexColor(0xffffff);
    ofDrawBitmapString(info, 30, 30);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLog(OF_LOG_NOTICE, ofToString(key));
    
    if(key == 't'){
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
    
    for (int i = 0; i < s_dragAmount->getValue(); i++) {
        float radius = ofRandom(s_dragRadius->getValue(), s_dragRadius->getValue() + s_dragSpread->getValue());
        
        float x = cos(ofRandom(PI * 2.0)) * radius + mouseX;
        float y = sin(ofRandom(PI * 2.0)) * radius + mouseY;
        ofVec2f position = ofVec2f(x, y);
        ofVec2f velocity = ofVec2f(0, 0);
//        ofColor color;
//        int hue = int(ofGetFrameNum() / 4.0) % 255;
//        color.setHsb(hue, 180, 200);
        //particles.setColor(color);
        particles.setColor(pColor);
        particles.createParticle(position, velocity);
    }
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


//--------DatGui stuff-----------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
    
    if (e.target->getLabel() == "CLEAR OBJECTS") {
        
        //Don't know how to clear the particles yet...
        
        for(int i=0; i<circles.size(); i++) {
            
            circles[i].get()->destroy();
        }
    }
    
    else if (e.target->getLabel() == "RESET GRAVITY") {
//        gravityX = 0;
//        gravityY = 0;
        gui->get2dPad("GRAVITY")->reset();
        
    }
}


void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    if (e.target->is("BLENDMODE ADD")) blendModeAdd = !blendModeAdd;;
    cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
}



void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
}


void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
    gravityX = e.x;
    gravityY = e.y;
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    
    //Sloppy way to set particle flags, rewrite this
    string s =e.target->getLabel();
    
    if (s == "WATER") {
        particles.setParticleFlag(b2_waterParticle );
    }
    else if (s == "WALL") {
        particles.setParticleFlag(b2_wallParticle );
    }
    else if (s == "SPRING") {
        particles.setParticleFlag(b2_springParticle );
    }
    else if (s == "ELASTIC") {
        particles.setParticleFlag(b2_elasticParticle );
    }
    else if (s == "VISCOUS") {
        particles.setParticleFlag(b2_viscousParticle );
    }
    else if (s == "POWDER") {
        particles.setParticleFlag(b2_powderParticle );
    }
    else if (s == "TENSILE") {
        particles.setParticleFlag(b2_tensileParticle );
    }
    else if (s == "COLORMIXING") {
        particles.setParticleFlag(b2_colorMixingParticle );
    }
    else if (s == "BARRIER") {
        particles.setParticleFlag(b2_barrierParticle );
    }
    else if (s == "STATICPRESSURE") {
        particles.setParticleFlag(b2_staticPressureParticle );
    }
    else if (s == "REACTIVE") {
        particles.setParticleFlag(b2_reactiveParticle );
    }
    else if (s == "REPULSIVE") {
        particles.setParticleFlag(b2_repulsiveParticle );
    }
    
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    
    if (e.target->getLabel() == "PARTICLE COLOR") {
        //ofSetBackgroundColor(e.color);
        pColor.set(e.color);
    }
}




