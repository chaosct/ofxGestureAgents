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

	template<class ListenerClass>
	void registerNewAgent(
		ofxPythonObject GA_system,
		string recognizer_module,
		string recognizer_class,
		ListenerClass * listener,
		void (ListenerClass::*listenerMethod)(ofxPythonObject&))
	{
		ofEvent<ofxPythonObject> & myevent = newagentevents[recognizer_module+"."+recognizer_class];
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
	
protected:
	map<string,ofEvent<ofxPythonObject> > newagentevents;
	ofxPython python;

};
