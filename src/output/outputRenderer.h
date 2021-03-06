#pragma once
#include "ofMain.h"
#include "cameraTracking.h"

class outputRenderer
{
	public :

		int outputX, outputY, outputW, outputH;


		virtual void setup( cameraTracking &cameraTracker );
		virtual void render( void );
		void setOutputArea( int x, int y, int w, int h );

		
		virtual void initialiseResources( void );
		virtual void releaseResources();

	protected :
		
		bool useOpticalFlow;

		int rgbToHex( int r, int g, int b );

		float outputScaleX, outputScaleY;

		cameraTracking *tracking;



};