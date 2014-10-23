//
//  Module.cpp
//  videoToDMX
//
//  Created by pach on 27/03/14.
//  www.creapach.fr
//
//

#include "Module.h"

void Module::setup(int id, int x, int y, string addr){
    posX = x;
    posY = y;
    universeId = id;
    artnetAddr = addr;
    
    correspTable = NULL;
    
//    ofLog(OF_LOG_NOTICE, "create module at coord "+ofToString(posX)+" "+ofToString(posY));
}


void Module::update(){

}

void Module::draw(){
    
    ofPushMatrix();
    
    
    ofImage test;
    test.setFromPixels(data);
//    test.draw(posX*25, posY*25);
   test.draw(posX*5, posY*5);
    
    ofPopMatrix();
}

void Module::clear(){
//    if (rawdata != NULL)
//        free(rawdata);
//    rawdata = NULL;
    
    if (correspTable != NULL){
        free(correspTable);
        correspTable = NULL;
    }
    
}
void Module::buildCorrespTable(int outW, int outH){
    
    clear();
    
    correspTableSize = data.getWidth()*data.getHeight();
    if (correspTable == NULL){
        correspTable = (int *) malloc(sizeof(int)*correspTableSize);
    }
    if (correspTable == NULL){
        ofLog(OF_LOG_ERROR,"correspTable not freed");
        ofExit();
    }
    
    for (int i=0 ; i<correspTableSize ; i++){
        correspTable[i]=0;
    }
    
    
    itSub = subModules.begin();
    itSubEnd = subModules.end();
    int dataPt = 0;
    while (itSub != itSubEnd) {
        itSub->buildCorrespTable(correspTable, dataPt, outW, outH);
        itSub ++;
        if (subModules.size()>0) {
            dataPt+=subModules[0].nbPixelX*subModules[0].nbPixelY;
        }
    }
    
    cout<<"debug corresp table for module "<<ofToString(universeId)<<" which is size "<<ofToString(correspTableSize)<<endl;
    for (int i=0 ; i<correspTableSize ; i++){
        cout<<ofToString(correspTable[i])<<" ";
    }
    cout<<endl;
}


void Module::process(ofPixels pxl, ofxArtnet& artnet, ofxCurvesTool redLut, ofxCurvesTool greenLut, ofxCurvesTool blueLut){
    
    count = 0;
    mediumColor = ofVec3f(0., 0., 0.);
    
    unsigned char * pxlData = pxl.getPixels();
//    unsigned char * rawdata = (unsigned char *)malloc(sizeof(unsigned char)*data.size());
    unsigned char * rawdata = data.getPixels();
    int size = data.size();
    for (int i=0 ; i<data.getWidth()*data.getHeight() ; i++){
        rawdata[i*data.getNumChannels()]=redLut[pxlData[correspTable[i]*pxl.getNumChannels()]];
        rawdata[i*data.getNumChannels()+1]=greenLut[pxlData[correspTable[i]*pxl.getNumChannels()+1]];
        rawdata[i*data.getNumChannels()+2]=blueLut[pxlData[correspTable[i]*pxl.getNumChannels()+2]];
        
        mediumColor.x += pxlData[correspTable[i]*pxl.getNumChannels()];
        mediumColor.y += pxlData[correspTable[i]*pxl.getNumChannels()+1];
        mediumColor.z += pxlData[correspTable[i]*pxl.getNumChannels()+2];
        count ++;
    }
    
    mediumColor.x /= count;
    mediumColor.y /= count;
    mediumColor.z /= count;
    
    artnet.sendDmx(artnetAddr, 0, universeId, rawdata, correspTableSize*data.getNumChannels());
}

void Module::sendBlack(ofxArtnet &artnet){
    unsigned char *rawdata = (unsigned char*) malloc(sizeof(unsigned char)*data.size());
    int size = data.size();
    for (int i=0; i<size; i++) {
        rawdata[i]=0;
    }
    artnet.sendDmx(artnetAddr, 0, universeId, rawdata, correspTableSize*data.getNumChannels());
    free(rawdata);
}

void Module::resetModule(int nbPxlX, int nbPxlY, int pxlStartPointOnPair, int pxlStartPointOnImpair, int pxlSnake){
    for (int i=0; i<subModules.size(); i++) {
        subModules[i].clear();
    }
    subModules.clear();
    
    int modPosX, modPosY;
    int incX, incY;
    
    switch(startPoint){
        case 0 :
            modPosX = 0;
            modPosY = 0;
            incX = 1;
            incY = 1;
            break;
        case 1 :
            modPosX = nbSubModuleX-1;
            modPosY = 0;
            incX = -1;
            incY = 1;
            break;
        case 2 :
            modPosX = 0;
            modPosY = nbSubModuleY-1;
            incX = 1;
            incY = -1;
            break;
        case 3 :
            modPosX = nbSubModuleX-1;
            modPosY = nbSubModuleY-1;
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
    
    sizeX = nbSubModuleX*nbPxlX;
    sizeY = nbSubModuleY*nbPxlY;
    
    posX *= sizeX;
    posY *= sizeY;
    
//    ofLog(OF_LOG_NOTICE, "module pos is "+ofToString(posX)+"-"+ofToString(posY));
    
    for (int i=0; i<nbSubModuleX*nbSubModuleY; i++) {
        SubModule m;
//        m.setup(i+1, modPosX, modPosY, modPosX*nbPxlX+posX, modPosY*nbPxlY+posY);
        m.setup(i+1, modPosX, modPosY, modPosX*nbPxlX+posX, modPosY*nbPxlY+posY);
        
        m.nbPixelX = nbPxlX;
        m.nbPixelY = nbPxlY;
        m.snake = pxlSnake;
        
        if (modPosY%2 == 0) {
            m.startPixel = pxlStartPointOnPair;
        }else{
            m.startPixel = pxlStartPointOnImpair;
        }
        
        m.resetSubModule();
        
        subModules.push_back(m);
        
//        ofLog(OF_LOG_NOTICE, "   did create and push submodule "+ofToString(i));
        
        modPosX += incX;
        if (modPosX >= nbSubModuleX || modPosX < 0){
            modPosY += incY;
            if (snake){
                incX = -incX;
                modPosX += incX;
            }else{
                if (startPoint ==1 || startPoint == 3){
                    modPosX = nbSubModuleX-1;
                }else{
                    modPosX = 0;
                }
            }
        }
    }
    
    
    data.allocate(sizeX, sizeY, OF_PIXELS_RGB);

    ofLog(OF_LOG_NOTICE, "data allocated at size "+ofToString(data.getWidth())+" "+ofToString(data.getHeight())+" which is "+ofToString(data.size()));
}