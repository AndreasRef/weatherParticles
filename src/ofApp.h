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
    
    //LiquidFun & box2d
    ofxBox2d box2d;
    ofxBox2dParticleSystem particles;			      //    LiquidFun particle system
    vector <ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
    
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
    
    ofxDatGuiSlider* s_dragSpread;
    ofxDatGuiSlider* s_dragRadius;
    ofxDatGuiSlider* s_dragAmount;
    ofxDatGuiSlider* s_particleSize;
    
    ofxDatGui2dPad* p_gravityPad;

    
    //Global variables
    ofColor pColor;
    

};