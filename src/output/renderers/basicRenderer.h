#include "outputRenderer.h"
#include "ParticleSystem.h"
#include "ofMain.h"

class basicRenderer : public outputRenderer{
	
	public :

		virtual void setup( motionTracking &motionTracker );
		virtual void initialiseResources();
		virtual void render();
		virtual void releaseResources();

	private :
};