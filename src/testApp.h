#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

#include "ParticleSystem.h"
#include "boxWithHandles.h"

#include "basicRenderer.h"
#include "bubbleRenderer.h"
#include "smokeRenderer.h"
#include "pathRenderer.h"
#include "sandRenderer.h"
#include "subliminalRenderer.h"


class testApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();

		void keyPressed( int key );
		void mouseMoved(    int x, int y );
		void mouseDragged(  int x, int y, int button );
		void mousePressed(  int x, int y, int button );
		void mouseReleased( int x, int y, int button );
		void windowResized( int w, int h );

private:
		
	//  SIZE OF VARIOUS INPUTS & OUTPUTS

		int camX;
		int camY;
	
		int processX;
		int processY;
		
		int halfX;
		int halfY;

		int outputX;
		int outputY;
		int outputW;
		int outputH;

	// PROGRAM COMPONENTS

		ofxSimpleGuiToo     gui;

      	boxWithHandles      vidWarpBox;

		motionTracking      motionTracker;

		ParticleSystem      mouseParticles;

		outputRenderer output;

		ofxOscReceiver OSCinput;
		ofxOscSender   OSCoutput;

		void processOSCinput();
		void processOSCoutput();

		ofDirectory dir;

		bool                drawImages;
		bool                drawDebugInfo;


	// COULD BE SEPERATED INTO OUTPUTMANAGER

		float rendererLastChanged;
		float presetLastChanged;

		int rendererTimeout;
		int presetTimeout;

		void loadRandomPreset();
		void loadRandomRenderer();
		
		void changeOutputRenderer( string fileName );
		
		string getTimeString();
};

#endif
