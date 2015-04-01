#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();

	// TuioCursorEvents is a sensor with newAgent event
	TuioCursorEvents = python.getObject("TuioCursorEvents","Tuio");

	// creating a System
	GA_system = python.getObject("ofSystem","ofsystem")(ofxPythonObject::makeTuple() << TuioCursorEvents);

	// registering to RecognizerStick newAgent events
	gestureagents.registerNewAgent(GA_system,"RecognizerStick",this,&ofApp::NewAgentStick);

	// registering to TuioCursorEvents newAgent events from Tuio module
	gestureagents.registerNewAgent(GA_system,"Tuio","TuioCursorEvents",this,&ofApp::NewAgentPaint);

	// registering to RecognizerTap newAgent events
	gestureagents.registerNewAgent(GA_system,"RecognizerTap",this,&ofApp::NewAgentTap);

	// registering to RecognizerDoubleTap newAgent events
	gestureagents.registerNewAgent(GA_system,"RecognizerDoubleTap",this,&ofApp::NewAgentDoubleTap);


	canvas.allocate(ofGetWidth(),ofGetHeight());
	canvas.begin();
	ofClear(0);
	canvas.end();

	buttonpos.set(400, 400);
	buttonradius = 50;

	ofSetWindowTitle("ofxGestureAgents: Painting Example");

}

//--------------------------------------------------------------
void ofApp::update(){
	GA_system.attr("update")();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	canvas.draw(0,0);
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(3);
	ofCircle(buttonpos, buttonradius);
	ofPopStyle();

	ofPushStyle();
	ofSetColor(0);
	int n = 1;
	ofDrawBitmapString("Available gestures:",10,15*n++);
	ofDrawBitmapString("  Tap",10,15*n++);
	ofDrawBitmapString("  DoubleTap (inside the circle)",10,15*n++);
	ofDrawBitmapString("  Stick",10,15*n++);
	ofDrawBitmapString("  Free Drawing",10,15*n++);
	ofPopStyle();
}

void ofApp::NewAgentStick(ofxPythonObject & agent)
{
	//we are interested on newStick events
	gestureagents.registerEvent(agent,"newStick",this,&ofApp::NewStick);
}


void ofApp::NewStick(ofxPythonObject & agent)
{
	ofVec3f pos1(
		agent.attr("pos1").asVector()[0].asFloat(),
		agent.attr("pos1").asVector()[1].asFloat());
	ofVec3f pos2(
		agent.attr("pos2").asVector()[0].asFloat(),
		agent.attr("pos2").asVector()[1].asFloat());
	ofPushStyle();
	ofSetLineWidth(3);
	canvas.begin();
	ofLine(pos1,pos2);
	canvas.end();
	ofPopStyle();
}


void ofApp::NewAgentPaint(ofxPythonObject & agent)
{
	//we are interested on updateCursor events
	gestureagents.registerEvent(agent,"updateCursor",this,&ofApp::NewPaint);
}


void ofApp::NewPaint(ofxPythonObject & agent)
{
	ofVec3f pos(
		agent.attr("pos").asVector()[0].asFloat(),
		agent.attr("pos").asVector()[1].asFloat());
	ofPushStyle();
	ofSetColor(255, 100, 100);
	canvas.begin();
	ofCircle(pos,10);
	canvas.end();
	ofPopStyle();
}

void ofApp::NewAgentTap(ofxPythonObject & agent)
{
	//we are interested on newTap events
	gestureagents.registerEvent(agent,"newTap",this,&ofApp::NewTap);
}


void ofApp::NewTap(ofxPythonObject & agent)
{
	ofVec3f pos(
		agent.attr("pos").asVector()[0].asFloat(),
		agent.attr("pos").asVector()[1].asFloat());
	ofPushStyle();
	ofSetColor(0, 100, 200);
	canvas.begin();
	ofCircle(pos,15);
	canvas.end();
	ofPopStyle();
}

void ofApp::NewAgentDoubleTap(ofxPythonObject & agent)
{
	ofVec3f pos(
		agent.attr("pos").asVector()[0].asFloat(),
		agent.attr("pos").asVector()[1].asFloat());
	if(pos.distance(buttonpos) < buttonradius)
	{
		//we are interested on newDoubleTap events inside the button
		gestureagents.registerEvent(agent,"newDoubleTap",this,&ofApp::NewDoubleTap);
	}
}


void ofApp::NewDoubleTap(ofxPythonObject & agent)
{
	ofPushStyle();
	ofSetColor(0, 100, 200);
	canvas.begin();
	ofClear(0);
	canvas.end();
	ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	mouse_agent.attr("xpos") = ofxPythonObject::fromFloat(x);
	mouse_agent.attr("ypos") = ofxPythonObject::fromFloat(y);
	mouse_agent.attr("pos") = ofxPythonObject::makeTuple() << mouse_agent.attr("xpos") << mouse_agent.attr("ypos");
	mouse_agent.attr("updateCursor")(mouse_agent);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// We create the mouse agent
	mouse_agent = python.getObject("CursorAgent","Tuio")(TuioCursorEvents);

	mouse_agent.attr("sessionid") = ofxPythonObject::fromFloat(0);
	mouse_agent.attr("xpos") = ofxPythonObject::fromFloat(x);
	mouse_agent.attr("ypos") = ofxPythonObject::fromFloat(y);
	mouse_agent.attr("xmot") = ofxPythonObject::fromFloat(0);
	mouse_agent.attr("ymot") = ofxPythonObject::fromFloat(0);
	mouse_agent.attr("mot_accel") = ofxPythonObject::fromFloat(0);
	mouse_agent.attr("pos") = ofxPythonObject::makeTuple() << mouse_agent.attr("xpos") << mouse_agent.attr("ypos");

	//send the event
	TuioCursorEvents.attr("newAgent")(mouse_agent);
	mouse_agent.attr("newCursor")(mouse_agent);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	mouse_agent.attr("xpos") = ofxPythonObject::fromFloat(x);
	mouse_agent.attr("ypos") = ofxPythonObject::fromFloat(y);
	mouse_agent.attr("pos") = ofxPythonObject::makeTuple() << mouse_agent.attr("xpos") << mouse_agent.attr("ypos");
	mouse_agent.attr("removeCursor")(mouse_agent);
	mouse_agent.attr("finish")();
	mouse_agent = ofxPythonObject::_None();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	w = ofGetWidth();
	h = ofGetHeight();
	if(w > canvas.getWidth() or h > canvas.getHeight())
	{
		ofFbo fbo;
		fbo.allocate(ofGetWidth(),ofGetHeight());
		fbo.begin();
		ofClear(0);
		canvas.draw(0,0);
		fbo.end();
		canvas = fbo;
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
