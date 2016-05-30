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
    creationLimitY = ofGetHeight();
    radiusNoise = false;
    
    //LiquidFun particles
    //See http://bit.ly/1YYigGM & http://bit.ly/25lc2Ij for more on particle types
    particles.setParticleFlag(b2_tensileParticle);
    particles.loadImage("particle32.png");
    //particles.setup(<#b2World *b2world#>, <#int maxCount#>, <#float lifetime#>, <#float radius#>, <#float particleSize#>, <#ofColor color#>)
    particles.setup(box2d.getWorld(), 200000, 60.0, 6.0, 42.0,ofColor(255));
    
    //Osc
    oscReceiver.setup(12345);
    
    
    //DatGui
    
    //1) Framerate and particles
    ofColor bg= ofColor::fromHex(0x0c542c);
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    //gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->addFRM()->setBackgroundColor(bg);
    gui->addTextInput("Total particles")->setBackgroundColor(bg);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //2) Global toogles
    bg= ofColor::fromHex(0x93790b);
    ofColor stripe = ofColor::fromHex(0xffd00b);
    
    gui->addToggle("WEATHER PARTICLE CONTROL", false)->setBackgroundColor(bg);
    gui->addToggle("MOUSE CREATION", TRUE)->setBackgroundColor(bg);
    gui->addToggle("manual weather",false)->setBackgroundColor(bg);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //3) Particle controls
    bg= ofColor::fromHex(0x113a4d);
    stripe = ofColor::fromHex(0x2fa1d6);
    
    gui->add2dPad("VELOCITY",ofRectangle(-200,-200,400,400))->setStripeColor(stripe);
    gui->get2dPad("VELOCITY")->setBackgroundColor(bg);
    
    gui->addButton("RESET VELOCITY")->setStripeColor(stripe);
    gui->getButton("RESET VELOCITY")->setBackgroundColor(bg);
    
    s_particleLifeSpan = gui->addSlider("LIFESPAN", 1, 120, 20);
    s_particleLifeSpan-> setBackgroundColor(bg);
    
    s_creationAmount = gui->addSlider("CREATION AMOUNT", 1, 100, 20);
    s_creationAmount-> setBackgroundColor(bg);
    
    s_creationRadius = gui->addSlider("CREATION RADIUS", 0, 200, 60);
    s_creationRadius-> setBackgroundColor(bg);
    
    s_creationSpread = gui->addSlider("CREATION SPREAD", 0, 200, 20);
    s_creationSpread-> setBackgroundColor(bg);
    
    s_particleRadius = gui->addSlider("PARTICLE RADIUS", 0.10, 0.6, 0.22); //0.22 number does not make a lot of sense, but is the number that (more or less) matches radius 6.0
    s_particleRadius-> setBackgroundColor(bg);
    
    s_particleSize = gui->addSlider("PARTICLE SIZE", 5, 64, 32);
    s_particleSize-> setBackgroundColor(bg);
    
    
    vector<string> opts = {"Particle type: water", "Particle type: wall", "Particle type:  spring", "Particle type: elastic", "Particle type: viscous", "Particle type: powder","Particle type: tensile", "Particle type: colorMixing", "Particle type: barrier", "Particle type: staticPressure", "Particle type: reactive", "Particle type: repulsive"};
    gui->addDropdown("particle type", opts)->setStripeColor(stripe);
    gui->getDropdown("particle type")->setBackgroundColor(bg);
    gui->getDropdown("particle type")->select(4);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //4) Particle color & blend
    stripe = ofColor::fromHex(0xDAEDE2);
    bg= ofColor::fromHex(0x4d5450);
    
    gui->addColorPicker("PARTICLE COLOR", ofColor::fromHex(0xeeeeee))->setStripeColor(stripe);
    gui->getColorPicker("PARTICLE COLOR")->setBackgroundColor(bg);
    gui->addToggle("HSB CYCLE", true)->setStripeColor(stripe);
    gui->getToggle("HSB CYCLE")->setBackgroundColor(bg);
    gui->addSlider("BLENDHSB",0.0,1.0,0.1)->setBackgroundColor(bg);
    gui->getSlider("BLENDHSB")->setStripeColor(stripe);
    gui->addSlider("HSB SPEED",0.1,100,10)->setBackgroundColor(bg);
    gui->getSlider("HSB SPEED")->setStripeColor(stripe);
    gui->addToggle("BLENDMODE ADD", true)->setStripeColor(stripe);
    gui->getToggle("BLENDMODE ADD")->setBackgroundColor(bg);
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    
    //5) Gravity controls
    stripe = ofColor::fromHex(0xEA2E49);
    bg= ofColor::fromHex(0x54111b);
    
    p_gravityPad = gui->add2dPad("GRAVITY", ofRectangle(-1,-1,2,2));
    p_gravityPad->setStripeColor(stripe);
    p_gravityPad->setBackgroundColor(bg);
    gui->addButton("reset gravity")->setStripeColor(stripe);
    gui->getButton("reset gravity")->setBackgroundColor(bg);
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    
    //6) Weather parameters
    stripe = ofColor::fromHex(0x2fa1d6);
    bg= ofColor::fromHex(0x5e6680);
    
    gui->addSlider("temperature", -20, 50)->setBackgroundColor(bg);
    
    gui->addSlider("windDirection", 0, 360, 0)->setBackgroundColor(bg);
    
    gui->addSlider("windSpeed", 0, 100, 0)->setBackgroundColor(bg);
    
    gui->addToggle("rising")->setBackgroundColor(bg);
    gui->getToggle("rising")->setStripeColor(stripe);
    
    gui->addToggle("weather noise")->setBackgroundColor(bg);
    gui->getToggle("weather noise")->setStripeColor(stripe);
    
    
    
    vector<string> codes = {"Weather code: clear", "Weather code: cloudy", "Weather code: rain", "Weather code: snow", "Weather code: storm", "Weather code: thunder"};
    d_weather = gui->addDropdown("groupedWeatherCode", codes);
    d_weather->setStripeColor(ofColor::fromHex(0x2fa1d6));
    d_weather->select(0);
    d_weather->setBackgroundColor(ofColor::fromHex(0x5e6680));
    
    
    
    
    gui->addBreak(); gui->addBreak(); gui->addBreak();
    
    //7) MISC
    stripe = ofColor::fromHex(0xF6F792);
    bg= ofColor::fromHex(0x7f804b);
    
    gui->addToggle("HOTEL ARROW", true)->setStripeColor(stripe);
    gui->getToggle("HOTEL ARROW")->setBackgroundColor(bg);
    gui->addButton("CLEAR OBJECTS")->setStripeColor(stripe);
    gui->getButton("CLEAR OBJECTS")->setBackgroundColor(bg);
    
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
    
    
    
    if (gui->getToggle("HSB CYCLE")->getChecked() == 1) {
        
        if (ofRandom(1)>gui->getSlider("BLENDHSB")->getValue()) {
            pColor.set(gui->getColorPicker("PARTICLE COLOR")->getColor());
            
        } else {
        
        int hue = int(ofGetFrameNum() / gui->getSlider("HSB SPEED")->getValue()) % 255;
        pColor.setHsb(hue, 180, 200);
            
        }
    }
    particles.setColor(ofColor(pColor));
    
    
    
    //Create particles from the top
    if (gui ->getToggle("MOUSE CREATION")->getChecked() == 0) {
        
        for (int i = 0; i < s_creationAmount->getValue(); i++) {
            float radius = ofRandom(s_creationRadius->getValue(), s_creationRadius->getValue() + s_creationSpread->getValue());
            
            float x = cos(ofRandom(PI * 2.0)) * radius + 500;
            float y = sin(ofRandom(PI * 2.0)) * radius + 50;
            
            //ofVec2f position = ofVec2f(x, y);
            //ofVec2f position = ofVec2f(ofRandom(1000),ofRandom(100));
            ofVec2f position = ofVec2f(ofRandom(1000),ofRandom(creationLimitY)); //totally random position
            ofVec2f velocity = ofVec2f(gui->get2dPad("VELOCITY")->getPoint());
            particles.createParticle(position, velocity);
        }
        
    }
    
    
    float noiseVal = 0;
    float amplitude = 1.0;
    float speed = 0.1;
    float noisePos = 1000;
    
    
    //Fast noise
    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed*7 + noisePos )-amplitude/4;
    
    //radiusNoise
    if (radiusNoise == true) {
        s_particleRadius->setValue(0.01 + noiseVal);
        
    }
    
    
    //If manual weather is off -> Get weather from Osc messages
    if (gui ->getToggle("MANUAL WEATHER")->getChecked() == 0) {
        while ( oscReceiver.hasWaitingMessages() ) { ofxOscMessage m; oscReceiver.getNextMessage( &m );
            if ( m.getAddress() == "/temperature" ) {
                ofLog(OF_LOG_NOTICE, "/temperature " + ofToString(m.getArgAsInt( 0 )));
                
                if (gui->getToggle("WEATHER NOISE") ->getChecked()==1) {
                    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed + noisePos )-amplitude/2;
                }
                
                gui->getSlider("temperature") -> setValue(m.getArgAsInt( 0 )+noiseVal);
                
                if (gui->getToggle("Weather Particle Control")->getChecked()==1) {
                    gui->getSlider("PARTICLE SIZE") -> setValue(ofMap(gui->getSlider("temperature") ->getValue(),-15,50,5,50));
                }
                
                
                
            } else if ( m.getAddress() == "/windDirection" ) {
                ofLog(OF_LOG_NOTICE, "/windDirection " + ofToString(m.getArgAsInt( 0 )));
                
                if (gui->getToggle("WEATHER NOISE") ->getChecked()==1) {
                    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed + noisePos/4 )-amplitude/4;
                }
                
                gui->getSlider("windDirection") -> setValue(m.getArgAsInt( 0 )+noiseVal);
                
            } else if ( m.getAddress() == "/windSpeed" ) {
                ofLog(OF_LOG_NOTICE, "/windSpeed " + ofToString(m.getArgAsInt( 0 )));
                
                if (gui->getToggle("WEATHER NOISE") ->getChecked()==1) {
                    noiseVal = amplitude*ofNoise( ofGetElapsedTimef() * speed + noisePos/2 )-amplitude/2;
                }
                gui->getSlider("windSpeed") -> setValue(m.getArgAsInt( 0 )+noiseVal);
                
                if (gui->getToggle("Weather Particle Control")->getChecked()==1) {
                    
                    float x = cos(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
                    float y = sin(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
                    float scalar = ofMap(gui->getSlider("windSpeed")->getValue(),0,100,0,1);
                    gui ->get2dPad("GRAVITY")->setPoint(ofPoint(x*scalar,y*scalar));
                    box2d.setGravity(gui ->get2dPad("GRAVITY")->getPoint());
                    
                    p_gravityPad->setPoint(ofPoint(x*scalar,y*scalar));
                    box2d.setGravity(p_gravityPad->getPoint());
                    
                    
                }
                
            }
            
            
            else if ( m.getAddress() == "/groupedWeatherCode" ) {
                ofLog(OF_LOG_NOTICE, ofToString("/groupedWeatherCode " + ofToString(m.getArgAsInt( 0 ))));
                //
                d_weather->select(m.getArgAsInt(0));
                
                //right mapping from weatherCodes to particle types
                int mapping[] = {6,4,0,10,9,11};
                
                gui->getDropdown("particle type")->select(mapping[m.getArgAsInt(0)]);
                
                weatherType(m.getArgAsInt(0));
                
            }
            
            
            else if ( m.getAddress() == "/rising" ) {
                ofLog(OF_LOG_NOTICE, "/rising " + ofToString(m.getArgAsInt( 0 )));
                gui->getToggle("rising") -> setChecked(m.getArgAsInt( 0 ));
                //gui->getSlider("rising") -> setValue(m.getArgAsInt( 0 ));
                
            } else if ( m.getAddress() == "/counter" ) {
                
                ofLog(OF_LOG_NOTICE, "/counter " + ofToString(m.getArgAsInt( 0 )));
            }
            
        }
    }
    
    
    if (gui ->getToggle("WEATHER PARTICLE CONTROL")->getChecked() == 1) {
        
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackgroundGradient(ofColor(0), ofColor(63), OF_GRADIENT_LINEAR);
    ofBackground(0);
    //ofDrawRectangle(boundsA); //Debug rectangle
    
    ofSetColor(255, 40);
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
            ofVec2f velocity = ofVec2f(gui->get2dPad("VELOCITY")->getPoint());
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
        gui->get2dPad("VELOCITY")->reset();
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
    
    if (e.target->getLabel() == "HSB CYCLE") {
        if (gui->getToggle("HSB CYCLE")->getChecked() == 0) {
            pColor.set(gui->getColorPicker("PARTICLE COLOR")->getColor());
        }
    }
}



void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
    
    if (e.target->getLabel() == "TEMPERATURE") {
        gui->getSlider("PARTICLE SIZE") -> setValue(ofMap(gui->getSlider("TEMPERATURE") ->getValue(),-15,50,5,50));
    }
    if (e.target->getLabel()=="WINDDIRECTION" || e.target->getLabel()=="WINDSPEED") {
        
        float x = cos(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
        float y = sin(ofDegToRad(gui->getSlider("windDirection")->getValue()+90));
        float scalar = ofMap(gui->getSlider("windSpeed")->getValue(),0,100,0,1);
        gui ->get2dPad("GRAVITY")->setPoint(ofPoint(x*scalar,y*scalar));
        box2d.setGravity(gui ->get2dPad("GRAVITY")->getPoint());
    }
}


void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
    
    if (e.target->getLabel() == "GRAVITY") {
        box2d.setGravity(e.x,e.y);
    } else if(e.target->getLabel()=="VELOCITY") {
        //
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
    string s =e.target->getLabel();
    
    if (e.target ==  d_weather) {

        if (s== "WEATHER CODE: CLEAR") weatherType(0);
        if (s== "WEATHER CODE: CLOUDY") weatherType(1);
        if (s== "WEATHER CODE: RAIN") weatherType(2);
        if (s== "WEATHER CODE: SNOW") weatherType(3);
        if (s== "WEATHER CODE: STORM") weatherType(4);
        if (s== "WEATHER CODE: THUNDER") weatherType(5);

    }
    
    
    //Sloppy way to set particle flags, rewrite this
    //string s =e.target->getLabel();
    
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
    else if (s == "PARTICLE TYPE: STATICPRESSURE" || s=="WEATHER CODE: STORM") {
        gui->getDropdown("PARTICLE TYPE")->select(9); //Storm selects static pressure particle
        particles.setParticleFlag(b2_staticPressureParticle );
    }
    else if (s == "PARTICLE TYPE: REACTIVE" || s == "WEATHER CODE: SNOW") {
        gui->getDropdown("PARTICLE TYPE")->select(10); //Snow selects reactive particle
        particles.setParticleFlag(b2_reactiveParticle );
    }
    else if (s == "PARTICLE TYPE: REPULSIVE" || s == "WEATHER CODE: THUNDER") {
        gui->getDropdown("PARTICLE TYPE")->select(11); //Thunder selects repulsive particle
        particles.setParticleFlag(b2_repulsiveParticle );
    }
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    
    if (e.target->getLabel() == "PARTICLE COLOR") {
        pColor.set(e.color);
        gui->getToggle("HSB CYCLE")->setChecked(false);
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

void ofApp:: weatherType(int index) {
//    ofLog(OF_LOG_NOTICE, ofToString("/weatherTypeFunction " + ofToString(index)));
    
    
    switch(index){
        case 0: //Clear weather
            
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0xF7E359));
            s_particleLifeSpan ->setValue(120);
            s_creationAmount ->setValue(1);
            s_particleRadius->setValue(0.22);
            creationLimitY = ofGetHeight();
            radiusNoise = false;

            gui->get2dPad("VELOCITY")->reset();
            break;
            
        case 1: //Cloudy weather
            
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0x5D84CC));
            s_particleLifeSpan ->setValue(120);
            s_creationAmount ->setValue(3);
            s_particleRadius->setValue(0.22);
            creationLimitY = ofGetHeight();
            radiusNoise = false;
            
            gui->get2dPad("VELOCITY")->reset();
            
            break;
            
        case 2: //Rain weather
            
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0x0095FF));
            s_particleLifeSpan ->setValue(30);
            s_creationAmount ->setValue(8);
            s_particleRadius->setValue(0.22);
            creationLimitY = ofGetHeight()/4.0;
            radiusNoise = false;
         
            break;
            
        case 3: //Snow weather
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0xF1FDFF));
            s_particleLifeSpan ->setValue(10);
            s_creationAmount ->setValue(22);
            s_particleRadius->setValue(0.2);
            creationLimitY = ofGetHeight()/3.0;
            radiusNoise = false;
            break;
            
        case 4: //Storm weather
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0x1054DB));
            s_particleLifeSpan ->setValue(30);
            s_creationAmount ->setValue(4);
            s_particleRadius->setValue(0.22);
            creationLimitY = ofGetHeight();
            radiusNoise = true;

            break;
            
        case 5: //Thunder weather
            gui->getColorPicker("PARTICLE COLOR")->setColor(ofColor::fromHex(0x00215A));
            s_particleLifeSpan ->setValue(40);
            s_creationAmount ->setValue(4);
            s_particleRadius->setValue(0.22);
            creationLimitY = ofGetHeight();
            radiusNoise = true;

            break;
            
        default:
            
            break;
    }
    
    pColor.set(gui->getColorPicker("PARTICLE COLOR")->getColor());
    
    
    
}

