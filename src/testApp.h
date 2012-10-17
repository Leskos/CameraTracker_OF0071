#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofVectorMath.h"
#include "ofxOpenCv.h"

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

		ofxSimpleGuiToo     gui;

      	boxWithHandles      vidWarpBox;

		motionTracking      motionTracker;

		ParticleSystem      mouseParticles;

		int numOutputs;
		int currOutput;
		outputRenderer *output[6];

		ofDirectory dir;
		vector<ofImage> imageFiles;

		bool                drawImages;
		bool                drawDebugInfo;

		bool                autoBg;

		void changeOutputRenderer( int newOutput );

		string getTimeString();


		float rendererLastChanged;
		float presetLastChanged;

		int rendererTimeout;
		int presetTimeout;

		void loadRandomPreset();
		void loadRandomRenderer();
			
};

#endif
