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
    
    
    //Set initial values for global variables - delete if not needed!
    gravityX = 0;
    gravityY = 0;
    
    //particleLifeSpan = 30;
    
    ofColor color;
    color.set(255);
    
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
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = gui->addFolder("white folder", ofColor::white);
    //folder->addSlider("LIFESPANSLIDER", 0, 60, particleLifeSpan);
    folder->addToggle("** toggle");
    folder->addColorPicker("** picker", ofColor::fromHex(0xFFD00B));
    // let's have it open by default. note: call this only after you're done adding items //
    folder->expand();
    
    gui->addBreak();
    
    s_particleLifeSpan = gui->addSlider("LIFESPAN", 0, 60, 20);
    s_dragAmount = gui->addSlider("DRAG AMOUNT", 1, 100, 20);
    s_dragRadius = gui->addSlider("DRAG RADIUS", 0, 200, 60);
    s_dragSpread = gui->addSlider("DRAG SPREAD", 0, 200, 20);
    
    // and a slider to adjust the gui opacity //
    gui->addSlider("datgui opacity", 0, 100, 100);
    
    // and a colorpicker //
    gui->addColorPicker("color picker", ofColor::fromHex(0xeeeeee));
    
    // add a wave monitor //
    // take a look inside example-TimeGraph for more examples of this component and the value plotter //
    gui->addWaveMonitor("wave\nmonitor", 3, .2);
    
    gui->addBreak();
    
    // add a dropdown menu //
    vector<string> opts = {"water", "wall", "spring", "elastic", "viscous", "powder","tensile", "colorMixing", "barrier", "staticPressure", "reactive", "repulsive"};
    gui->addDropdown("particle type", opts);
    gui->addBreak();
    
    
    // add a 2d pad //
    p_gravityPad = gui->add2dPad("GRAVITY", ofRectangle(-1,-1,2,2)); //upperleft.x, upperleft.y, width, height
    
    //gui->add2dPad(<#string label#>, ofRectangle bounds)
    
    
    // and a couple of simple buttons //
    gui->addButton("click");
    
    gui->addHeader(":: drag me to reposition ::");
    
    gui->addFooter();
    
    // once the gui has been assembled, register callbacks to listen for component specific events //
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    
    
    gui->setOpacity(gui->getSlider("datgui opacity")->getScale());
    //  gui->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    
    // finally let's load up the stock themes, press spacebar to cycle through them //
    themes = {  new ofxDatGuiTheme(true),
        new ofxDatGuiThemeSmoke(),
        new ofxDatGuiThemeWireframe(),
        new ofxDatGuiThemeMidnight(),
        new ofxDatGuiThemeAqua(),
        new ofxDatGuiThemeCharcoal(),
        new ofxDatGuiThemeAutumn(),
        new ofxDatGuiThemeCandy()};
    tIndex = 0;
    
    // launch the app //
//    mFullscreen = true;
//    refreshWindow();
    
    
    //GUI
//    clearButton.addListener(this, &ofApp::clearButtonPressed);
//    
//    gui.setup();
//    gui.setPosition(10,100);
//    
//    gui.add(particleType.setup("particleType", 0, 0, 11));
//    gui.add(particlesPerDrag.setup("particlesPerDrag", 20, 2, 100));
//    gui.add(dragRadius.setup("dragRadius", 20, 2, 100));
//    gui.add(dragSpread.setup("dragSpread", 20, 2, 100));
//    gui.add(particleLifeSpan.setup("particleLifeSpan", 5, 1, 60));
//    gui.add(xGravity.setup("xGravity", 0, -3, 3));
//    gui.add(yGravity.setup("yGravity", 0, -3, 3));
//    gui.add(particleSize.setup("particleSize", 42, 6, 80));
//    gui.add(blendModeAdd.setup("blendModeAdd", true));
//    gui.add(clearButton.setup("clear"));
//    
//    
//    bHide = false;
//    
//    gui.loadFromFile("settings.xml");
    
//    particleList = ofSplitString(
//                         "b2_waterParticle;b2_wallParticle;b2_springParticle;b2_elasticParticle;b2_viscousParticle;b2_powderParticle;b2_tensileParticle;b2_colorMixingParticle;b2_barrierParticle;b2_staticPressureParticle;b2_reactiveParticle;b2_repulsiveParticle", ";" );
//    
}

//--------------------------------------------------------------
//void ofApp::clearButtonPressed(){
//    //Don't know how to clear the particles yet...
//    ofLog(OF_LOG_NOTICE, "clear");
//    
//    for(int i=0; i<circles.size(); i++) {
//        
//        circles[i].get()->destroy();
//    }
//}


//--------------------------------------------------------------
void ofApp::update() {
    box2d.update();
    
    //DatGui updates test
    //s_dragSpread-> setValue(ofGetMouseX()*0.15);

   box2d.setGravity(gravityX,gravityY);
   particles.setParticleLifetime(s_particleLifeSpan ->getValue());

    
//
//            
//    
//    //particles.setRadius(6); //Does not have any effect?
//    
//    particles.particleSize = particleSize;
//    //particles.setColor(ofColor(0, ofRandom( 0, 255 ), ofRandom( 0, 255 )));
//    
}


//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackgroundGradient(ofColor(0), ofColor(63), OF_GRADIENT_LINEAR);
    
    for(int i=0; i<circles.size(); i++) {
        ofFill();
        ofSetHexColor(0xf6c738);
        circles[i].get()->draw();
    }
    
    
    //if (blendModeAdd) ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    particles.draw();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    string info = "";
    info += "Mouse Drag for particles\n";
    info += "Total Particles: "+ofToString(particles.getParticleCount())+"\n\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
//    info += particleList[particleType]+"\n";
    ofSetHexColor(0xffffff);
    ofDrawBitmapString(info, 30, 30);
    
    
//    if(!bHide){
//        gui.draw(); //Hide gui
//    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLog(OF_LOG_NOTICE, ofToString(key));
    
    if(key == 'h'){
//        bHide = !bHide;
    }
    else if(key == 's'){
//        gui.saveToFile("settings.xml");
    }
    else if(key == 'l'){
//        gui.loadFromFile("settings.xml");
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
    
    for (int i = 0; i < s_dragAmount->getValue(); i++) {
        float radius = ofRandom(s_dragRadius->getValue(), s_dragRadius->getValue() + s_dragSpread->getValue());
    
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
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
    if (e.target->is("datgui opacity")) {
        gui->setOpacity(e.scale);
    }
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
    
    if (e.target->getLabel() == "COLOR PICKER") {
    ofSetBackgroundColor(e.color);
    }
}




