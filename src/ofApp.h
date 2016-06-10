#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxLiquidFun.h"
#include "ofxOsc.h"


// -------------------------------------------------

class ofApp : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void drawFbo(); //Fbo alpha
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void loadHotelArrow();
    void weatherType(int index);
    
    //LiquidFun & box2d
    ofxBox2d box2d;
    ofxBox2dParticleSystem particles;			      //    LiquidFun particle system
    ofRectangle boundsA;
    vector <ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
    vector <shared_ptr<ofxBox2dEdge> >   edges;       //    Hotel arrow edges
    
    //Osc
    ofxOscReceiver oscReceiver;
    
    //DatGui
    ofxDatGui* gui;
    void refreshWindow();
    void toggleFullscreen();
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    ofxDatGuiSlider* s_particleLifeSpan;

    ofxDatGuiSlider* s_creationAmount;
    ofxDatGuiSlider* s_particleRadius;
    ofxDatGuiSlider* s_particleSize;
    
    ofxDatGui2dPad* p_gravityPad;
    ofxDatGuiDropdown* d_weather;
    
    ofxDatGuiDropdown* d_mode;
    

    //Global variables
    ofColor pColor;
    int creationLimitY;
    Boolean radiusNoise;
    int mode = 0;

    //Sound variables
    
    ofSoundPlayer sound;	//Sound sample
    
    const int N = 512;		//Number of bands in spectrum
    //const int N = 28;		//Number of bands in spectrum
    
    float spectrum[ 512 ];	//Smoothed spectrum values
    int bandBass = 2;		//Band index in spectrum, affecting Rad value
    int bandSnare = 100;	//Band index in spectrum, affecting Vel value  
    float time0 = 0;		//Time value, used for dt computing
    
    
    //Fading Fbo
    ofFbo rgbaFboFloat;
    
    
    //WindLines
    float a = 0;
    
    float randomC();
    
    
    //Fading trails
    float fadeSpeed = 2.0;
    float wind = 0.0;
    float amplitude = 0.5;
    int period = 125;
    
    
    int fadingTrailN = 300;
    float xPos[300];
    float yPos[300];
    float size[300];
    

};