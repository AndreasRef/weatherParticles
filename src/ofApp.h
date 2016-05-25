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
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void loadHotelArrow();
    
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
    ofxDatGuiSlider* s_creationSpread;
    ofxDatGuiSlider* s_creationRadius;
    ofxDatGuiSlider* s_creationAmount;
    ofxDatGuiSlider* s_particleRadius;
    ofxDatGuiSlider* s_particleSize;
    ofxDatGui2dPad* p_gravityPad;

    
    //Global variables
    ofColor pColor;

};