#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "boxWithHandles.h"
#include "ofxSimpleGuiToo.h"
#include "ofVectorMath.h"
#include "ofxOpenCv.h"
#include "ParticleSystem.h"

#include "basicRenderer.h"
#include "bubbleRenderer.h"
#include "sandRenderer.h"
#include "pathRenderer.h"


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

		// CAMERA TRACKING
		cameraTracking      cameraTracker;

		ParticleSystem      mouseParticles;

		int numOutputs;
		int currOutput;
		outputRenderer *output[3];

		bool                drawImages;
		bool                autoBg;
			
};

#endif
