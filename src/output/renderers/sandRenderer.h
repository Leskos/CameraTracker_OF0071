#include "outputRenderer.h"
#include "ParticleSystem.h"
#include "ofMain.h"

class sandRenderer : public outputRenderer{
	
	public :

		virtual void setup( cameraTracking &cameraTracker );
		virtual void initialiseResources();
		virtual void render();
		virtual void releaseResources();

	private :
		ParticleSystem particles;
};