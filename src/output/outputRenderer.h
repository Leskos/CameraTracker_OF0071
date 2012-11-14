#pragma once

#include "ofMain.h"
#include "motionTracking.h"
#include "ParticleSystem.h"

class outputRenderer
{
	public :

	// Base functionality

		int outputX, outputY, outputW, outputH;
		
		string fileName;

		outputRenderer();

		void setup( motionTracking &motionTracker );
		void update();
		void draw();

		void initialiseResources( void );
		void releaseResources();
		
		void setOutputArea( int x, int y, int w, int h );
	    void setFileName( string newFileName );
		
		bool fadeToBlack;
		bool redrawScreen;
		
		bool showDebugInfo;


	// Particle generation parameters
		bool useParticles;

		float pSizeMin, pSizeMax;
		float pLifeMin, pLifeMax;
		float pDragMin, pDragMax;

		int maxParticlesPerFrame;

		ofColor particleColour;
		float pHue;
		float pSat;
		float pBri;
		float pHueCycleSpeed;

		int pRandColRange;
		int pRandColInterval;

		
	// Optical flow parameters	
	// ### TODO #####
	// Move ofxOpticalFlowLK into this class
		float opFlowForceScale;  
		float opFlowAvgArea;     
		
	// For rendering paths
		bool    usePaths;
		int     pathMaxVertices;
		float   pathSmoothAmount;
	
	// For images
		bool colourFromImg;
		bool colourFromImgRandomly;
		int  currentImg;
		

	protected :
		
		motionTracking *tracking;

		float outputScaleX, outputScaleY;
		
		bool useOpticalFlow;

   		ParticleSystem particles;
		void seedNewParticles();
		
		void updatePaths();
		void drawPaths();
		vector<ofPolyline>  simplePolylines;
		vector<ofPath>      simplePaths;
};