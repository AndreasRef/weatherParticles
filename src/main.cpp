#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGlutWindow.h"

int main() {
	
//    ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 1280, 1400, OF_WINDOW);
    ofSetupOpenGL(1280, 1400, OF_WINDOW);
	ofRunApp(new ofApp());
	
}
