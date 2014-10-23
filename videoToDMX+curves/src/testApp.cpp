#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    gui.setup("module");
    
    gui.add(nbModuleX.set("nb module X", 5, 1, 20));
    gui.add(nbModuleY.set("nb module Y", 5, 1, 20));
    gui.add(startPoint.set("start corner", 2, 0, 4));
    gui.add(snake.set("snake at end", true));
    
    gui.add(nbSubModuleX.set("nb submodule X", 8, 1, 20));
    gui.add(nbSubModuleY.set("nb submodule Y", 4, 1, 20));
    gui.add(subStartPoint.set("submodule start corner", 2, 0, 4));
    gui.add(subSnake.set("submodule snake at end", true));
    
    gui.add(nbPixelX.set("nb pixel X", 2, 1, 20));
    gui.add(nbPixelY.set("nb pixel Y", 2, 1, 20));
    gui.add(pxlStartPointOnPair.set("pixel start corner on pair line", 3, 0, 4));
    gui.add(pxlStartPointOnImpair.set("pixel start corner on impair line", 0, 0, 4));
    gui.add(pxlSnake.set("pixel snake at end", true));

    gui.add(reset.set("reset", false));
    
    gui.add(processTime.set("artnet process time", 0.02, 0., 0.1));
    gui.add(reconnectArtnet.set("reconnect artnet", false));
    
    gui.add(reset_lut_bool.set("reset lut", false));
    gui.add(load_lut_bool.set("load lut", false));
    gui.add(save_lut_bool.set("save lut", false));
    
    gui.add(draw_bool.set("draw", false));
    
    gui.add(isActive.set("active", true));
    
    gui.add(port_light.set("port light", 12345, 111111, 15000));
    gui.add(addr_light.set("addr light", "11.0.0.45"));
    gui.add(update_light.set("update light connection", false));
    
    syphon.setup();
    syphon.setApplicationName("VDMX5");
    syphon.setServerName("ledWall");
    
    fbo.allocate(syphon.getWidth(), syphon.getHeight());

    artnet.setup("10.0.0.1");
    
    oldtime = ofGetElapsedTimef();
    
    redCurve.setup();
    greenCurve.setup();
    blueCurve.setup();
    
    load_lut_bool = true;
    
    isActive = true;
    
    oscSend.setup(addr_light, port_light);
    oscUpdate = false;
    
    ofSetFrameRate(60);
    
    resetModules();
}

void testApp::resetModules(){
    for (int i=0; i<modules.size(); i++) {
        modules[i].clear();
    }
    modules.clear();
    
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
            modPosX = nbModuleX-1;
            modPosY = 0;
            incX = -1;
            incY = 1;
            break;
        case 2 :
            modPosX = 0;
            modPosY = nbModuleY-1;
            incX = 1;
            incY = -1;
            break;
        case 3 :
            modPosX = nbModuleX-1;
            modPosY = nbModuleY-1;
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
    
    
    for (int i=0; i<nbModuleX*nbModuleY; i++) {
        Module m;
        m.setup(i+1, modPosX, modPosY, "10.0.0."+ofToString(i+50));
        m.nbSubModuleX = nbSubModuleX;
        m.nbSubModuleY = nbSubModuleY;
        m.startPoint = subStartPoint;
        m.snake = subSnake;
        
        m.resetModule(nbPixelX, nbPixelY, pxlStartPointOnPair, pxlStartPointOnImpair, pxlSnake);
        
        modules.push_back(m);
        
//        ofLog(OF_LOG_NOTICE, "did create module "+ofToString(i));
        
        
        modPosX += incX;
        if (modPosX >= nbModuleX || modPosX < 0){
            modPosY += incY;
            if (snake){
                incX = -incX;
                modPosX += incX;
            }else{
                if (startPoint ==1 || startPoint == 3){
                    modPosX = nbModuleX-1;
                }else{
                    modPosX = 0;
                }
            }
        }
    }
    
    fbo.allocate(nbModuleX*nbSubModuleX*nbPixelX, nbModuleY*nbSubModuleY*nbPixelY);
    syphonPxl.allocate(nbModuleX*nbSubModuleX*nbPixelX, nbModuleY*nbSubModuleY*nbPixelY, OF_PIXELS_RGBA);
////    fbo.readToPixels(syphonPxl);
//    
    for (int i=0; i<modules.size(); i++) {
        modules[i].buildCorrespTable(nbModuleX*nbSubModuleX*nbPixelX, nbModuleX*nbSubModuleY*nbPixelY);
//        modules[i].buildCorrespTable();
    }
}

void testApp::saveLut(){
    redCurve.save("redLut");
    greenCurve.save("greenLut");
    blueCurve.save("blueLut");
    save_lut_bool = false;
}

void testApp::resetLut(){
//    redCurve.setup();
//    greenCurve.setup();
//    blueCurve.setup();
    reset_lut_bool = false;
}

void testApp::loadLut(){
    redCurve.load("redLut");
    greenCurve.load("greenLut");
    blueCurve.load("blueLut");
    load_lut_bool = false;
}

//--------------------------------------------------------------
void testApp::update(){
    if (reset) {
        resetModules();
        reset = false;
    }
    
    if (reconnectArtnet){
        artnet.setup("10.0.0.1");
        reconnectArtnet = false;
    }
    
    if (isActive){
        fbo.begin();
        syphon.draw(0, 0, fbo.getWidth(), fbo.getHeight());
        fbo.end();
        fbo.readToPixels(syphonPxl);
        
        if (ofGetElapsedTimef()-oldtime>processTime){
            if (!reset){
                itMods = modules.begin();
                itModsEnd = modules.end();
                while(itMods != itModsEnd){
                    itMods->process(syphonPxl, artnet, redCurve, greenCurve, blueCurve);
                    itMods++;
                }
                oscUpdate = true;
            }
            oldtime = ofGetElapsedTimef();
        }
        
        if (oscUpdate){
            int count = 0;
            ofVec3f medium = ofVec3f(0., 0., 0.);
            
            itMods = modules.begin();
            itModsEnd = modules.end();
            while(itMods != itModsEnd){
                medium += itMods->getMediumColor();
                itMods++;
                count ++;
            }

            medium /= count;
            
            ofxOscMessage m;
            m.setAddress("/leds/r");
            m.addFloatArg(medium.x/255.);
            oscSend.sendMessage(m);
            m.clear();
            
            m.setAddress("/leds/g");
            m.addFloatArg(medium.y/255.);
            oscSend.sendMessage(m);
            m.clear();
            
            m.setAddress("/leds/b");
            m.addFloatArg(medium.z/255.);
            oscSend.sendMessage(m);
            m.clear();
            
            m.setAddress("/mur/luminance");
            m.addFloatArg((medium.x+medium.y+medium.z)/(3*255.));
            oscSend.sendMessage(m);
            m.clear();

    //        ofLog (OF_LOG_NOTICE, "osc updated "+ofToString(medium.x)+", "+ofToString(medium.y)+", "+ofToString(medium.z));
            oscUpdate = false;
        }
    }
    else{
        itMods = modules.begin();
        itModsEnd = modules.end();
        while(itMods != itModsEnd){
            itMods->sendBlack(artnet);
            itMods++;
        }
    }
    
    if (reset_lut_bool){
        resetLut();
    }
    if (load_lut_bool){
        loadLut();
    }
    if (save_lut_bool){
        saveLut();
    }
    if (update_light) {
        update_light = false;
        oscSend.setup(addr_light, port_light);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofClear(0);
    
    if (draw_bool){
    
        ofPushMatrix();
        
        ofTranslate(20, 20);
        
        syphon.draw(0, 0, syphon.getWidth(), syphon.getHeight());
        ofImage test ;
        test.setFromPixels(syphonPxl);
        test.draw(0, syphon.getHeight(), test.getWidth(), test.getHeight());
        
        ofTranslate(0, syphon.getHeight()+test.getHeight());
        
        itMods = modules.begin();
        itModsEnd = modules.end();
        while(itMods != itModsEnd){
            itMods->draw();
            itMods++;
        }
        
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(10, 450);
        redCurve.draw();
        
        ofTranslate(275, 0);
        greenCurve.draw();
        
        ofTranslate(275, 0);
        blueCurve.draw();
        
        ofPopMatrix();
        
    }
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 5, 15);
    gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' ') {
        isActive = !isActive;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}