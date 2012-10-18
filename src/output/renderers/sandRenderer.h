#include "outputRenderer.h"
#include "ParticleSystem.h"
#include "ofMain.h"

class sandRenderer : public outputRenderer{
	
	public :

		virtual void setup( motionTracking *motionTracker );
		virtual void initialiseResources();
		virtual void render();
		virtual void releaseResources();

		// Controllable settings for the renderer
		float sizeMin, sizeMax;
		float lifeMin, lifeMax;
		float dragMin, dragMax;
		float ofAvgArea;
		float opFlowForceScale;

	private :
		ParticleSystem particles;
};