#include "outputRenderer.h"

void outputRenderer::setup( motionTracking &motionTracker )
{
	tracking = &motionTracker;
	setOutputArea( 0, ofGetScreenWidth(), ofGetScreenWidth(), ofGetScreenHeight() );
	useOpticalFlow = false;
	tracking->CTdoOpticalFlow = useOpticalFlow;
}


void outputRenderer::initialiseResources( void )
{
	cout << "\nINITIALISING OUTPUT RENDERER RESOURCES";
}


void outputRenderer::releaseResources( void )
{
	cout << "\nRELEASING OUTPUT RENDERER RESOURCES";
}


void outputRenderer::render()
{
	// Draw the output of the tracking
	
	//tracking->drawContours( outputX, outputY, outputW, outputH );
	//tracking->drawOpticalFlow( outputX, outputY, outputW );
}


void outputRenderer::setOutputArea(int x, int y, int w, int h)
{
	outputX = x;
	outputY = y;
	outputW = w;
	outputH = h;

	outputScaleX = outputW / tracking->getImgX();
	outputScaleY = outputH / tracking->getImgY();
}


