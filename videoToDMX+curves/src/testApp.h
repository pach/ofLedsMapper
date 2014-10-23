#pragma once

#include "ofMain.h"
#include "Module.h"

#include "ofxGui.h"

#include "ofxSyphon.h"
#include "ofxArtnet.h"

#include "ofxCurvesTool.h"

#include "ofxOscSender.h"

//#define ADDR_LIGHT "11.0.0.45"
//#define ADDR_LIGHT "127.0.0.1"
//#define PORT_LIGHT 12344

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void saveLut();
    void loadLut();
    void resetLut();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofParameter<int> nbModuleX;
    ofParameter<int> nbModuleY;
    ofParameter<int> startPoint;
    ofParameter<bool> snake;
    
    ofParameter<int> nbSubModuleX;
    ofParameter<int> nbSubModuleY;
    ofParameter<int> subStartPoint;
    ofParameter<bool> subSnake;
    
    ofParameter<int> nbPixelX;
    ofParameter<int> nbPixelY;
    ofParameter<int> pxlStartPointOnPair;
    ofParameter<int> pxlStartPointOnImpair;
    ofParameter<bool> pxlSnake;
    
    ofParameter<bool> reset;
    
    ofParameter<bool> reset_lut_bool;
    ofParameter<bool> save_lut_bool;
    ofParameter<bool> load_lut_bool;
    
    ofParameter<int> port_light;
    ofParameter<string> addr_light;
    ofParameter<bool> update_light;
    
    ofParameter<bool> draw_bool;
    
    ofParameter<bool> isActive;
    
private:
    ofxOscSender oscSend;
    bool oscUpdate;
    
    vector<Module> modules;
    
    void resetModules();
    
    ofxPanel gui;
    
    ofxSyphonClient syphon;
    ofFbo fbo;
    ofPixels syphonPxl;
    
    vector<Module>::iterator itMods;
    vector<Module>::iterator itModsEnd;
    
    ofxArtnet artnet;
    
    double oldtime;
    ofParameter<float> processTime;
    
    ofParameter<bool> reconnectArtnet;
    
    
    ofxCurvesTool redCurve;
    ofxCurvesTool blueCurve;
    ofxCurvesTool greenCurve;
};
