//
//  Pixel.cpp
//  videoToDMX
//
//  Created by pach on 29/03/14.
//  www.creapach.fr
//
//

#include "Pixel.h"

void Pixel::setup(int id, int modx, int mody, int refx, int refy){
    idPxl = id;
    posX = modx;
    posY = mody;
    refX = refx;
    refY = refy;
    
//    ofLog(OF_LOG_NOTICE, "    create pixel at module coord "+ofToString(posX)+" "+ofToString(posY)+" and real coord "+ofToString(refX)+" "+ofToString(refY));
}

void Pixel::update(){

}

void Pixel::draw(){
    ofDrawBitmapString("p"+ofToString(idPxl), refX * 25+20, refY * 25+20);

}

void Pixel::clear(){
    
}

//void Pixel::process(ofPixels& data, ofPixels ref){
//    int dataPt = posY*data.getWidth()*data.getNumChannels()+posX*data.getNumChannels();
//    int refPt = refY*ref.getWidth()*data.getNumChannels()+refX*data.getNumChannels();
//    
//    if(dataPt < data.getWidth()*data.getHeight()*data.getNumChannels()){
//        data[dataPt]   = ref[refPt];
//        data[dataPt+1] = ref[refPt+1];
//        data[dataPt+2] = ref[refPt+2];
//    }
//    else{
//        ofLog(OF_LOG_ERROR, "data out of bounds for "+ofToString(posX)+" "+ofToString(posY));
//        ofLog(OF_LOG_ERROR, "  data size : "+ofToString(data.getWidth())+" "+ofToString(data.getHeight()));
//    }
//}

void Pixel::buildCorrespTable(int* correspTable, int dataPt, int width, int height){
//    int refPt = posY*width+posX;
    int refPt = refY*width+refX;

    dataPt += idPxl-1;
    
//    ofLog(OF_LOG_NOTICE, "build corresp table for "+ofToString(idPxl)+" at "+ofToString(posX)+"-"+ofToString(posY)+" : "+ofToString(refX)+"-"+ofToString(refY)+" which is "+ofToString(dataPt)+"-"+ofToString(refPt));

    if(dataPt < width*height){
        correspTable[dataPt]   = refPt;
    }
    else{
        ofLog(OF_LOG_ERROR, "data out of bounds for "+ofToString(posX)+" "+ofToString(posY));
        ofLog(OF_LOG_ERROR, "  data size : "+ofToString(width)+" "+ofToString(height));
    }
}

//void Pixel::buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int & dataPt){
//    int refPt = posY*inW+posX;
//    //    int refPt = refY*outW+refX;
//    
//    ofLog(OF_LOG_NOTICE, "build corresp table for "+ofToString(idPxl)+" at "+ofToString(posX)+"-"+ofToString(posY)+" : "+ofToString(refX)+"-"+ofToString(refY)+" which is "+ofToString(dataPt)+"-"+ofToString(refPt));
//    
//    if(dataPt < inW*inH){
//        correspTable[dataPt]   = refPt;
//    }
//    else{
//        ofLog(OF_LOG_ERROR, "data out of bounds for "+ofToString(posX)+" "+ofToString(posY));
//        ofLog(OF_LOG_ERROR, "  data size : "+ofToString(inW)+" "+ofToString(inH));
//    }
//}
//
//void Pixel::buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int x, int y){
//    int dataPt = y*inW+x;
//    int refPt = posY*inW+posX;
////    int refPt = refY*outW+refX;
//    
//    ofLog(OF_LOG_NOTICE, "build corresp table for "+ofToString(idPxl)+" at "+ofToString(posX)+"-"+ofToString(posY)+" : "+ofToString(refX)+"-"+ofToString(refY)+" which is "+ofToString(dataPt)+"-"+ofToString(refPt));
//    
//    if(dataPt < inW*inH){
//        correspTable[dataPt]   = refPt;
//    }
//    else{
//        ofLog(OF_LOG_ERROR, "data out of bounds for "+ofToString(posX)+" "+ofToString(posY));
//        ofLog(OF_LOG_ERROR, "  data size : "+ofToString(inW)+" "+ofToString(inH));
//    }
//}
