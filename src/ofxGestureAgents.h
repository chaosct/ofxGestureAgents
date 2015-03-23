#pragma once
#include "ofxPython.h"
#include "ofxPythonCallBack.h"

class ofxGestureAgents
{
public:
	ofxGestureAgents()
	{
		python.init();
	}

	typedef ofEvent<ofxPythonObject> event_type;

	template<class ListenerClass>
	void registerNewAgent(
		ofxPythonObject GA_system,
		string recognizer_module,
		string recognizer_class,
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&))
	{
		ofEvent<ofxPythonObject> & myevent = events[recognizer_module+"."+recognizer_class];
		ofxPythonObject Recognizer = python.getObject(recognizer_class,recognizer_module);
		ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(myevent);
		ofxPythonObject pcb = CallBack2Python(cb);
		ofxPythonObject py_registerNewAgent = python.getObject("registerNewAgent","ofsystem");
		py_registerNewAgent(GA_system,Recognizer,pcb);
		ofAddListener(myevent,listener,listenerMethod);
	}

	template<class ListenerClass>
	void registerNewAgent(
		ofxPythonObject GA_system,
		string recognizer_class, //module is the same as class
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&))
	{
		registerNewAgent(GA_system,recognizer_class,recognizer_class,listener,listenerMethod);
	}

	template<class ListenerClass>
	void registerEvent(
		ofxPythonObject agent,
		string event,
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&))
	{
		ofEvent<ofxPythonObject> & myevent = events[agent.attr("__class__").repr()+"."+event];
		ofxPythonObject py_registerEvent = python.getObject("registerEvent","ofsystem");
		ofxPythonCallBackEvent1Arg * cb = new ofxPythonCallBackEvent1Arg(myevent);
		ofxPythonObject pcb = CallBack2Python(cb);
		py_registerEvent(agent.attr(event),pcb);
		ofAddListener(myevent,listener,listenerMethod);
	}

protected:
	map<string, event_type> events;
	ofxPython python;

};
