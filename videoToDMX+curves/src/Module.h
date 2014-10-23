//
//  Module.h
//  videoToDMX
//
//  Created by pach on 27/03/14.
//  www.creapach.fr
//
//

#pragma once

#include "ofMain.h"

#include "SubModule.h"
#include "ofxArtnet.h"

#include "ofxCurvesTool.h"

class Module {

public:
    void setup(int id, int x, int y, string addr);
    void setupArtnet();
//	void setup ();
	void update ();
	void draw ();
    
    void clear();
    
    inline ofVec3f getMediumColor() {return mediumColor;};
    
    void process(ofPixels refImage, ofxArtnet& artnet, ofxCurvesTool redLut, ofxCurvesTool greenLut, ofxCurvesTool blueLut);
    
    void sendBlack(ofxArtnet & artnet);

    void buildCorrespTable(int outW, int outH);
    
//    void buildCorrespTable();
    
    void resetModule(int nbPxlX, int nbPxlY, int pxlStartPointOnPair, int pxlStartPointOnImpair, int pxlSnake);

    int nbSubModuleX;
    int nbSubModuleY;
    int startPoint;
    bool snake;

    // ArtNetPoll universe;
    int universeId;
    string artnetAddr;
    vector<SubModule> subModules;
    
    vector<SubModule>::iterator itSub;
    vector<SubModule>::iterator itSubEnd;
    
    int posX, posY;
    
    int sizeX, sizeY;
    
    ofPixels data;
//    unsigned char * rawdata;
    int* correspTable;
    int correspTableSize;
    
    ofVec3f mediumColor;
    int count;
};