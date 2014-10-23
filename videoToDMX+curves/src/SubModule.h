//
//  SubModule.h
//  videoToDMX
//
//  Created by pach on 27/03/14.
//  www.creapach.fr
//
//

#pragma once

#include "ofMain.h"
#include "Pixel.h"

class SubModule {

public:
	void setup (int id, int modx, int mody, int refx, int refy);
	void update ();
	void draw ();
    void clear();
    
    void resetSubModule();
    
//    void process(ofPixels& data, ofPixels ref);
//    void buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int x, int y);
//    int buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int dataPt);
    
    void buildCorrespTable(int* correspTable, int dataPt, int width, int height);
    
    int idSub;
    int posX;
    int posY;
    int refX;
    int refY;
    
    int sizeX;
    int sizeY;
    
    int nbPixelX;
    int nbPixelY;
    int startPixel;
    bool snake;
    
    vector <Pixel> pixels;
    vector<Pixel>::iterator itPxl;
    vector<Pixel>::iterator itPxlEnd;
};