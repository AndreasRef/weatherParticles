#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackgroundHex(0x00000);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    boundsA.set(0, 0, ofGetWidth()-272, ofGetHeight());
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    box2d.createBounds(boundsA);
    
    loadHotelArrow();
    
    
    //Set initial values for global variables
    pColor.set(255);
    
    
    //LiquidFun particles
    //See http://bit.ly/1YYigGM & http://bit.ly/25lc2Ij for more on particle types
    particles.setParticleFlag(b2_tensileParticle);
    particles.loadImage("particle32.png");
    //particles.setup(<#b2World *b2world#>, <#int maxCount#>, <#float lifetime#>, <#float radius#>, <#float particleSize#>, <#ofColor color#>)
    particles.setup(box2d.getWorld(), 200000, 60.0, 6.0, 42.0,ofColor(255));
    
    //Osc
    oscReceiver.setup(12345);
    
    
    //DatGui
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    //gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->addFRM();
    gui->addTextInput("Total particles");
    //ofxDatGuiTextInput* myInput = new ofxDatGuiTextInput(string label, string value = "");
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //1) Particle controls
    gui->addToggle("WEATHER PARTICLE CONTROL", false)->setStripeColor(ofColor::fromHex(0x2fa1d6));
    gui->addToggle("MOUSE CREATION", TRUE)->setStripeColor(ofColor::fromHex(0x2fa1d6));
    gui->add2dPad("CREATION VELOCITY",ofRectangle(-200,-200,400,400))->setStripeColor(ofColor::fromHex(0x2fa1d6));
    gui->addButton("RESET VELOCITY")->setStripeColor(ofColor::fromHex(0x2fa1d6));
    
    s_particleLifeSpan = gui->addSlider("LIFESPAN", 1, 120, 20);
    s_creationAmount = gui->addSlider("CREATION AMOUNT", 1, 100, 20);
    s_creationRadius = gui->addSlider("CREATION RADIUS", 0, 200, 60);
    s_creationSpread = gui->addSlider("CREATION SPREAD", 0, 200, 20);
    s_particleRadius = gui->addSlider("PARTICLE RADIUS", 0.01, 1, 0.22); //0.22 number does not make a lot of sense, but is the number that (more or less) matches radius 6.0
    s_particleSize = gui->addSlider("PARTICLE SIZE", 5, 64, 32);
    
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    
    
    //2) Particle type
    vector<string> opts = {"Particle type: water", "Particle type: wall", "Particle type:  spring", "Particle type: elastic", "Particle type: viscous", "Particle type: powder","Particle type: tensile", "Particle type: colorMixing", "Particle type: barrier", "Particle type: staticPressure", "Particle type: reactive", "Particle type: repulsive"};
    gui->addDropdown("particle type", opts)->setStripeColor(ofColor::fromHex(0x77C4D3));
    gui->getDropdown("particle type")->select(4);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //3) Particle color & blend
    gui->addColorPicker("PARTICLE COLOR", ofColor::fromHex(0xeeeeee))->setStripeColor(ofColor::fromHex(0xDAEDE2));
    gui->addToggle("BLENDMODE ADD", true)->setStripeColor(ofColor::fromHex(0xDAEDE2));
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    
    
    //4) Gravity controls
    p_gravityPad = gui->add2dPad("GRAVITY", ofRectangle(-1,-1,2,2));
    gui->get2dPad("GRAVITY")->setStripeColor(ofColor::fromHex(0xEA2E49));
    gui->addButton("reset gravity")->setStripeColor(ofColor::fromHex(0xEA2E49));
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    
    //5) Weather parameters
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = gui->addFolder("weather conditions", ofColor::white);
    gui->getFolder("weather conditions")->setStripeColor(ofColor::fromHex(0x333745));
    folder->addSlider("temperature", -20, 50);
    folder->addSlider("windDirection", 0, 360, 0);
    folder->addSlider("windSpeed", 0, 100, 0);
    folder->addSlider("weatherType", 0, 100);
    folder->addSlider("rising", 0, 1);
    folder->addToggle("manual weather")->setStripeColor(ofColor::fromHex(0x333745));
    folder->addToggle("weather noise")->setStripeColor(ofColor::fromHex(0x333745));
    folder->expand();
    
    vector<string> codes = {"Weather code: clear", "Weather code: cloudy", "Weather code: rain", "Weather code: snow", "Weather code: storm", "Weather code: thunder"};
    gui->addDropdown("groupedWeatherCode", codes)->setStripeColor(ofColor::fromHex(0x333745));
    gui->getDropdown("groupedWeatherCode")->select(0);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //6) MISC
    gui->addToggle("HOTEL ARROW", true)->setStripeColor(ofColor::fromHex(0xF6F792));
    gui->addButton("clear objects")->setStripeColor(ofColor::fromHex(0xF6F792));
    
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
    
    gui->getTextInput("Total particles")->setText(ofToString(particles.getParticleCount()));
    
    particles.setParticleLifetime(s_particleLifeSpan ->getValue());
    particles.particleSystem->SetRadius(s_particleRadius->getValue());
    particles.particleSize = s_particleSize -> getValue();
    particles.setColor(ofColor(0, ofRandom( 0, 255 ), ofRandom( 0, 255 )));
    
    
    //Add attraction point to mouse
//    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
//    for(int i=0; i<circles.size(); i++) {
//        circles[i].get()->addAttractionPoint(mouse, 4.0);
//    }
    
    
    
    
    //Create particles from the top
    
    if (gui ->getToggle("MOUSE CREATION")->getChecked() == 0) {
        
        for (int i = 0; i < s_creationAmount->getValue(); i++) {
            float radius = ofRandom(s_creationRadius->getValue(), s_creationRadius->getValue() + s_creationSpread->getValue());
            
            float x = cos(ofRandom(PI * 2.0)) * radius + 500;
            float y = sin(ofRandom(PI * 2.0)) * radius + 50;

            //ofVec2f position = ofVec2f(x, y);
            ofVec2f position = ofVec2f(ofRandom(1000),ofRandom(100));
            ofVec2f velocity = ofVec2f(gui->get2dPad("CREATION VELOCITY")->getPoint());
            particles.createParticle(position, velocity);
        }
        
    }
    
    
    float noiseVal = 0;
    float amplitude = 1.0;
    float speed = 0.1;
    float noisePos = 1000;
    
    //If manual weather is off -> Get weather from Osc messages
    if (gui ->getToggle("MANUAL WEATHER")->getChecked() == 0) {
        while ( oscReceiver.hasWaitingMessages() ) { ofxOscMessage m; oscReceiver.getNextMessage( &m );
            if ( m.getAddress() == "/temperature" ) {
                ofLog(OF_LOG_NOTICE, "/temperature " + ofToString(m.getArgAsInt( 0 )));
                
                if (gui->getToggle("WEATHER NOISE") ->getChecked()==1) {
                    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed + noisePos )-amplitude/2;
                }
                
                gui->getSlider("temperature") -> setValue(m.getArgAsInt( 0 )+noiseVal);
                
            } else if ( m.getAddress() == "/windDirection" ) {
                ofLog(OF_LOG_NOTICE, "/windDirection " + ofToString(m.getArgAsInt( 0 )));
                gui->getSlider("windDirection") -> setValue(m.getArgAsInt( 0 ));
                
            } else if ( m.getAddress() == "/windSpeed" ) {
                ofLog(OF_LOG_NOTICE, "/windSpeed " + ofToString(m.getArgAsInt( 0 )));
                
                if (gui->getToggle("WEATHER NOISE") ->getChecked()==1) {
                    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed + 0 )-amplitude/2;
                }
                gui->getSlider("windSpeed") -> setValue(m.getArgAsInt( 0 )+noiseVal);
                
            } else if ( m.getAddress() == "/weatherType" ) {
                ofLog(OF_LOG_NOTICE, ofToString("/weatherType " + m.getArgAsString( 0 )));
                
                
            } else if ( m.getAddress() == "/rising" ) {
                ofLog(OF_LOG_NOTICE, "/rising " + ofToString(m.getArgAsInt( 0 )));
                gui->getSlider("rising") -> setValue(m.getArgAsInt( 0 ));
                
            } else if ( m.getAddress() == "/counter" ) {
                
                ofLog(OF_LOG_NOTICE, "/counter " + ofToString(m.getArgAsInt( 0 )));
            }
        }
    }
    
    
    if (gui ->getToggle("WEATHER PARTICLE CONTROL")->getChecked() == 1) {
        //Lerp towards new value
        gui->getSlider("PARTICLE SIZE") -> setValue(ofLerp(gui->getSlider("PARTICLE SIZE") ->getValue(), ofMap(gui->getSlider("temperature") ->getValue(),11.5,12.5,5,50),0.1));
    
        //Windspeed & winddirection -> Gravity
        float x = cos(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
        float y = sin(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
        float scalar = ofMap(gui->getSlider("windSpeed")->getValue(),0,100,0,1);
        
        gui ->get2dPad("GRAVITY")->setPoint(ofPoint(x*scalar,y*scalar));
        
        box2d.setGravity(gui ->get2dPad("GRAVITY")->getPoint());
    
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor(0), ofColor(63), OF_GRADIENT_LINEAR);
    
    //ofDrawRectangle(boundsA); //Debug rectangle
    
    ofSetHexColor(0x00ff00);
    ofNoFill();
    for (int i=0; i<edges.size(); i++) {
        edges[i].get()->draw();     //Draw hotel arrow
    }
    
    
    for(int i=0; i<circles.size(); i++) {
        ofFill();
        ofSetHexColor(0xf6c738);
        circles[i].get()->draw();
    }
    
    if (gui -> getToggle("BLENDMODE ADD") ->getChecked() == 1) ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    particles.draw();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLog(OF_LOG_NOTICE, ofToString(key));
    
    if(key == 't'){
        particles.loadImage("particle32.png");
    }
    
    if(key >= 48 && key <= 57){ //Key '0' has value 48, 1 has 49 etc
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
void ofApp::mouseDragged(int x, int y, int button) {
    
    
    if(ofGetMouseX() < ofGetWidth()-270 && gui -> getToggle("MOUSE CREATION") ->getChecked() == 1) {
        for (int i = 0; i < s_creationAmount->getValue(); i++) {
            float radius = ofRandom(s_creationRadius->getValue(), s_creationRadius->getValue() + s_creationSpread->getValue());
            
            float x = cos(ofRandom(PI * 2.0)) * radius + mouseX;
            float y = sin(ofRandom(PI * 2.0)) * radius + mouseY;
            ofVec2f position = ofVec2f(x, y);
            ofVec2f velocity = ofVec2f(gui->get2dPad("CREATION VELOCITY")->getPoint());
            //        ofColor color;
            //        int hue = int(ofGetFrameNum() / 4.0) % 255;
            //        color.setHsb(hue, 180, 200);
            //particles.setColor(color); //Hue sweep color
            //particles.setColor(pColor); //ColorPicker color
            //particles.setColor(ofColor(0, ofRandom( 0, 255 ), ofRandom( 0, 255 )));
            particles.createParticle(position, velocity);
        }
    }
}



//--------DatGui stuff-----------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
    
    if (e.target->getLabel() == "CLEAR OBJECTS") {
        
        for(int i = 0; i<particles.particleSystem->GetParticleCount(); i++) {
        particles.particleSystem->DestroyParticle(i);
        //particles.particleSystem->SetRadius(10);
        }
            
        for(int i=0; i<circles.size(); i++) {
            circles[i].get()->destroy();
        }
    }
    
    else if (e.target->getLabel() == "RESET GRAVITY") {
        box2d.setGravity(0,0);
        gui->get2dPad("GRAVITY")->reset();
    }
    else if(e.target->getLabel() == "RESET VELOCITY") {
        gui->get2dPad("CREATION VELOCITY")->reset();
    }
}


void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
    
     if (e.target->getLabel() == "HOTEL ARROW") {
         if (e.target -> getChecked() == 0) {
            edges.clear();
         } else if (e.target -> getChecked() == 1) {
          loadHotelArrow();
         }
    
     }
}



void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
}


void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
    
    if (e.target->getLabel() == "GRAVITY") {
        box2d.setGravity(e.x,e.y);
    } else if(e.target->getLabel()=="CREATION VELOCITY") {
        //
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    

    
    //Sloppy way to set particle flags, rewrite this
    string s =e.target->getLabel();
    
    if (s == "PARTICLE TYPE: WATER" || s == "WEATHER CODE: RAIN") {
        gui->getDropdown("PARTICLE TYPE")->select(0); //Rain selects water particle
        particles.setParticleFlag(b2_waterParticle );
    }
    else if (s == "PARTICLE TYPE: WALL") {
        particles.setParticleFlag(b2_wallParticle );
    }
    else if (s == "PARTICLE TYPE: SPRING") {
        particles.setParticleFlag(b2_springParticle );
    }
    else if (s == "PARTICLE TYPE: ELASTIC") {
        particles.setParticleFlag(b2_elasticParticle );
    }
    else if (s == "PARTICLE TYPE: VISCOUS" || s == "WEATHER CODE: CLOUDY") {
        gui->getDropdown("PARTICLE TYPE")->select(4); //Cloudy selects viscous particle
        particles.setParticleFlag(b2_viscousParticle );
    }
    else if (s == "PARTICLE TYPE: POWDER") {
        particles.setParticleFlag(b2_powderParticle );
    }
    else if (s == "PARTICLE TYPE: TENSILE" || s == "WEATHER CODE: CLEAR") {
        gui->getDropdown("PARTICLE TYPE")->select(6); //Clear selects tensile particle
        particles.setParticleFlag(b2_tensileParticle );
    }
    else if (s == "PARTICLE TYPE: COLORMIXING") {
        particles.setParticleFlag(b2_colorMixingParticle );
    }
    else if (s == "PARTICLE TYPE: BARRIER") {
        particles.setParticleFlag(b2_barrierParticle );
    }
    else if (s == "PARTICLE TYPE: STATICPRESSURE") {
        particles.setParticleFlag(b2_staticPressureParticle );
    }
    else if (s == "PARTICLE TYPE: REACTIVE" || s == "WEATHER CODE: SNOW") {
        gui->getDropdown("PARTICLE TYPE")->select(10); //Snow selects reactive particle
        particles.setParticleFlag(b2_reactiveParticle );
    }
    else if (s == "PARTICLE TYPE: REPULSIVE" || s == "WEATHER CODE: THUNDER") {
        gui->getDropdown("PARTICLE TYPE")->select(11); //Clear selects repulsive particle
        particles.setParticleFlag(b2_repulsiveParticle );
    }
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    
    if (e.target->getLabel() == "PARTICLE COLOR") {
        pColor.set(e.color);
    }
}


void ofApp::loadHotelArrow()
{
    ifstream f;
    f.open(ofToDataPath("lines.txt").c_str());
    vector <string> strLines;
    while (!f.eof()) {
        string ptStr;
        getline(f, ptStr);
        strLines.push_back(ptStr);
    }
    f.close();
    
    for (int i=0; i<strLines.size(); i++) {
        vector <string> pts = ofSplitString(strLines[i], ",");
        if(pts.size() > 0) {
            shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
            for (int j=0; j<pts.size(); j+=2) {
                if(pts[j].size() > 0) {
                    float x = ofToFloat(pts[j]);
                    float y = ofToFloat(pts[j+1]);
                    edge.get()->addVertex(x, y);
                }
            }
            edge.get()->create(box2d.getWorld());
            edges.push_back(edge);
        }
    }
    
}


