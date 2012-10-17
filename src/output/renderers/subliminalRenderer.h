#include "outputRenderer.h"
#include "ParticleSystem.h"
#include "ofMain.h"

class subliminalRenderer : public outputRenderer{
	
	public :

		virtual void setup( motionTracking &motionTracker );
		virtual void initialiseResources();
		virtual void render();
		virtual void releaseResources();

		// Controllable settings for the renderer
		float sizeMin, sizeMax;
		float lifeMin, lifeMax;
		float dragMin, dragMax;
		float ofAvgArea;
		float opFlowForceScale;
		
		int     currentImage;
		bool    fadeOut;
		bool    getColorRandomly;

	private :
		ParticleSystem particles;

		ofImage subliminalImage[6];
};