//
//  SubModule.cpp
//  videoToDMX
//
//  Created by pach on 27/03/14.
//  www.creapach.fr
//
//

#include "SubModule.h"

void SubModule::setup(int id, int modx, int mody, int refx, int refy){
    idSub = id;
    posX = modx;
    posY = mody;
    refX = refx;
    refY = refy;
    
//    ofLog(OF_LOG_NOTICE, "  create submodule at module coord "+ofToString(posX)+" "+ofToString(posY)+" and real coord "+ofToString(refX)+" "+ofToString(refY));
}

void SubModule::update(){

}

void SubModule::draw(){
    ofDrawBitmapString("s"+ofToString(idSub), refX * 25+10, refY * 25 +10);
    
    ofPushMatrix();
    itPxl = pixels.begin();
    itPxlEnd = pixels.end();
    
    while(itPxl != itPxlEnd){
        itPxl->draw();
        itPxl ++;
    }
    
//    for (int i=0 ; i<pixels.size() ; i++){
//        pixels[i].draw();
//    }
    ofPopMatrix();

}

void SubModule::clear(){
    
}

//void SubModule::process(ofPixels& data, ofPixels ref){
//    itPxl = pixels.begin();
//    itPxlEnd = pixels.end();
//    
//    while(itPxl != itPxlEnd){
//        itPxl->process(data, ref);
//        itPxl ++;
//    }
//
////    for (int i=0 ; i<pixels.size() ; i++){
////        pixels[i].process(data, ref);
////    }
//}

void SubModule::buildCorrespTable(int* correspTable, int dataPt, int width, int height){
    itPxl = pixels.begin();
    itPxlEnd = pixels.end();
    
    while(itPxl != itPxlEnd){
        itPxl->buildCorrespTable(correspTable, dataPt, width, height);
        itPxl ++;
    }

}

//void SubModule::buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int x, int y){
//    itPxl = pixels.begin();
//    itPxlEnd = pixels.end();
//    
//    int nx = 0;
//    int ny = 0;
//    
//    while(itPxl != itPxlEnd){
//        itPxl->buildCorrespTable(correspTable, inW, inH, outW, outH, nx+x*nbPixelX, ny+y*nbPixelY);
//        nx ++;
//        if (nx>=nbPixelX){
//            nx = 0;
//            ny ++;
//        }
//        itPxl ++;
//    }
//    
//}
//
//int SubModule::buildCorrespTable(int* correspTable, int inW, int inH, int outW, int outH, int dataPt){
//    itPxl = pixels.begin();
//    itPxlEnd = pixels.end();
//    
//    int nx = 0;
//    int ny = 0;
//    
//    while(itPxl != itPxlEnd){
////        itPxl->buildCorrespTable(correspTable, inW, inH, outW, outH, nx+x*nbPixelX, ny+y*nbPixelY);
//        itPxl->buildCorrespTable(correspTable, inW, inH, outW, outH, dataPt);
//        dataPt ++;
//        nx ++;
//        if (nx>=nbPixelX){
//            nx = 0;
//            ny ++;
//        }
//        itPxl ++;
//    }
//    
//    return --dataPt;
//}



void SubModule::resetSubModule(){
    for (int i=0; i<pixels.size(); i++) {
        pixels[i].clear();
    }
    pixels.clear();
    
    int modPosX, modPosY;
    int incX, incY;
    
    switch(startPixel){
        case 0 :
            modPosX = 0;
            modPosY = 0;
            incX = 1;
            incY = 1;
            break;
        case 1 :
            modPosX = nbPixelX-1;
            modPosY = 0;
            incX = -1;
            incY = 1;
            break;
        case 2 :
            modPosX = 0;
            modPosY = nbPixelY-1;
            incX = 1;
            incY = -1;
            break;
        case 3 :
            modPosX = nbPixelX-1;
            modPosY = nbPixelY-1;
            incX = -1;
            incY = -1;
            break;
            
        default:
            modPosX = 0;
            modPosY = 0;
            incX = 1;
            incY = 1;
            break;
            
    }
    
    sizeX = nbPixelX;
    sizeY = nbPixelY;
    
    posX *= sizeX;
    posY *= sizeY;
    
    for (int i=0; i<nbPixelX*nbPixelY; i++) {
        Pixel p;
        p.setup(i+1, modPosX+posX, modPosY+posY, modPosX+refX, modPosY+refY);
        
        pixels.push_back(p);
        
        //        ofLog(OF_LOG_NOTICE, "   did create and push submodule "+ofToString(i));
        
        modPosX += incX;
        if (modPosX >= nbPixelX || modPosX < 0){
            modPosY += incY;
            if (snake){
                incX = -incX;
                modPosX += incX;
            }else{
                if (startPixel ==1 || startPixel == 3){
                    modPosX = nbPixelX-1;
                }else{
                    modPosX = 0;
                }
            }
        }
    }
    
}