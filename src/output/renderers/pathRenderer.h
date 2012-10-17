#include "outputRenderer.h"
#include "ParticleSystem.h"
#include "ofMain.h"

class pathRenderer : public outputRenderer{
	
	public :

		virtual void setup( motionTracking &motionTracker );
		virtual void initialiseResources();
		virtual void render();
		virtual void releaseResources();
		
		int   maxVertices;
		float smoothAmt;

	private :

		void updateSimplePaths();

		vector<ofPolyline>  simplePolylines;
		vector<ofPath>      simplePaths;

};