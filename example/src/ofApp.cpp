#include "ofApp.h"
#include "ofxPythonCallBack.h"

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();

	// TuioCursorEvents is a sensor with newAgent event
	TuioCursorEvents = python.getObject("TuioCursorEvents","Tuio");

	// creating a System
	GA_system = python.getObject("ofSystem","ofsystem")(ofxPythonObject::makeTuple() << TuioCursorEvents);

	// RecognizerStick class
	ofxPythonObject RecognizerStick = python.getObject("RecognizerStick","RecognizerStick");

	ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(newagentStick);
	ofxPythonObject pcb = CallBack2Python(cb);
	registerNewAgent = python.getObject("registerNewAgent","ofsystem");
	registerNewAgent(GA_system,RecognizerStick,pcb);
	ofAddListener(newagentStick,this,&ofApp::NewAgentStick);

	lines.setMode(OF_PRIMITIVE_LINES);

}

//--------------------------------------------------------------
void ofApp::update(){
	GA_system.attr("update")();
}

//--------------------------------------------------------------
void ofApp::draw(){
	lines.draw();
}

void ofApp::NewAgentStick(ofxPythonObject & agent)
{
	ofxPythonObject registerEvent = python.getObject("registerEvent","ofsystem");
	ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(newStick);
	ofxPythonObject pcb = CallBack2Python(cb);
	registerEvent(agent.attr("newStick"),pcb);
	ofAddListener(newStick,this,&ofApp::NewStick);
}


void ofApp::NewStick(ofxPythonObject & agent)
{
	ofVec3f pos1(
		agent.attr("pos1").asVector()[0].asFloat(),
		agent.attr("pos1").asVector()[1].asFloat());
	ofVec3f pos2(
		agent.attr("pos2").asVector()[0].asFloat(),
		agent.attr("pos2").asVector()[1].asFloat());
	lines.addVertex(pos1);
	lines.addVertex(pos2);
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
