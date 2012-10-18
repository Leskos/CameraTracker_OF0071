/*#include "basicRenderer.h"

void basicRenderer::setup(motionTracking *motionTracker)
{
	outputRenderer::setup( motionTracker );
	tracking->CTdoOpticalFlow = true;
}


void basicRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = true;
}

void basicRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
}


void basicRenderer::render()
{
	tracking->CTdoOpticalFlow = true;

	tracking->setColorOutContours( ofColor( 0, 255, 255 ) );
	tracking->drawContours( outputX, outputY, outputW, outputH );
	tracking->drawOpticalFlow( outputX, outputY, outputW );
}
*/