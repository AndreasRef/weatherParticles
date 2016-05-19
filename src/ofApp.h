#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxLiquidFun.h"
#include "ofxGui.h"

// -------------------------------------------------

class ofApp : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void resized(int w, int h);
    
    
    //LiquidFun & box2d
    ofxBox2d box2d;
    ofxBox2dParticleSystem particles;			      //    LiquidFun particle system
    vector <ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
    
    
    //DatGui
    ofxDatGui* gui;
    void refreshWindow();
    void toggleFullscreen();
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    uint tIndex;
    vector<ofxDatGuiTheme*> themes;
    
    
    
    
    //OF GUI
//    void clearButtonPressed();
//    
//    bool bHide;
//    ofxIntSlider particleType;
//    ofxIntSlider particlesPerDrag;
//    ofxIntSlider dragSpread;
//    ofxIntSlider dragRadius;
//    ofxFloatSlider particleLifeSpan;
//    ofxFloatSlider xGravity;
//    ofxFloatSlider yGravity;
//    ofxIntSlider particleSize;
//    
//    ofxButton clearButton;
//    ofxToggle blendModeAdd;
//    
//    
//    ofxColorSlider color;
//    
//    ofxPanel gui;
//    
//    vector<string> particleList;
};