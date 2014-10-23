//
//  Pixel.h
//  videoToDMX
//
//  Created by pach on 29/03/14.
//  www.creapach.fr
//
//

#pragma once

#include "ofMain.h"

class Pixel {

public:
	void setup (int id, int modx, int mody, int refx, int refy);
	void update ();
	void draw ();
    void clear();
    
//    void process(ofPixels& data, ofPixels ref);
    void buildCorrespTable(int* correspTable, int dataPt, int width, int height);
//    void buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int x, int y);
//    void buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int & dataPt);
private:
    int idPxl;
    int posX;
    int posY;
    int refX;
    int refY;
};