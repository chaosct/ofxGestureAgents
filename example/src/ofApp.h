#pragma once

#include "ofMain.h"
#include "ofxPython.h"
#include "ofxGestureAgents.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofxPython python;
		ofxPythonObject mouse_agent;
		ofxPythonObject GA_system;
		ofxPythonObject TuioCursorEvents;

		void NewAgentStick(ofxPythonObject&);
		void NewStick(ofxPythonObject&);

		void NewAgentPaint(ofxPythonObject&);
		void NewPaint(ofxPythonObject&);

		void NewAgentTap(ofxPythonObject&);
		void NewTap(ofxPythonObject&);

		void NewAgentDoubleTap(ofxPythonObject&);
		void NewDoubleTap(ofxPythonObject&);

		ofVec2f buttonpos;
		ofFbo canvas;
		int buttonradius;

		ofxGestureAgents gestureagents;
};
