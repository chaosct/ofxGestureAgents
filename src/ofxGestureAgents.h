#pragma once
#include "ofMain.h"
#include "ofxPython.h"
#include "ofxPythonCallBack.h"

class ofxGestureAgents
{
public:
	ofxGestureAgents();

	typedef ofEvent<ofxPythonObject> event_type;

	template<class ListenerClass>
	void registerNewAgent(
		ofxPythonObject GA_system,
		string recognizer_module,
		string recognizer_class,
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&));

	template<class ListenerClass>
	void registerNewAgent(
		ofxPythonObject GA_system,
		string recognizer_class, //module is the same as class
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&));

	template<class ListenerClass>
	void registerEvent(
		ofxPythonObject agent,
		string event,
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&));

protected:
	map<string, event_type> events;

};

// implementation details

template<class ListenerClass>
void ofxGestureAgents::registerNewAgent(
	ofxPythonObject GA_system,
	string recognizer_module,
	string recognizer_class,
	ListenerClass * listener,
	void (ListenerClass::*listenerMethod)(ofxPythonObject&))
{
	ofEvent<ofxPythonObject> & myevent = events[recognizer_module+"."+recognizer_class];
	ofxPythonObject Recognizer = ofxPython::getObject(recognizer_class,recognizer_module);
	ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(myevent);
	ofxPythonObject pcb = CallBack2Python(cb);
	ofxPythonObject py_registerNewAgent = ofxPython::getObject("registerNewAgent","ofsystem");
	py_registerNewAgent(GA_system,Recognizer,pcb);
	ofAddListener(myevent,listener,listenerMethod);
}

template<class ListenerClass>
void ofxGestureAgents::registerNewAgent(
	ofxPythonObject GA_system,
	string recognizer_class, //module is the same as class
	ListenerClass * listener,
	void (ListenerClass::*listenerMethod)(ofxPythonObject&))
{
	registerNewAgent(GA_system,recognizer_class,recognizer_class,listener,listenerMethod);
}

template<class ListenerClass>
void ofxGestureAgents::registerEvent(
	ofxPythonObject agent,
	string event,
	ListenerClass * listener,
	void (ListenerClass::*listenerMethod)(ofxPythonObject&))
{
	ofEvent<ofxPythonObject> & myevent = events[agent.attr("__class__").repr()+"."+event];
	ofxPythonObject py_registerEvent = ofxPython::getObject("registerEvent","ofsystem");
	ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(myevent);
	ofxPythonObject pcb = CallBack2Python(cb);
	py_registerEvent(agent.attr(event),pcb);
	ofAddListener(myevent,listener,listenerMethod);
}
