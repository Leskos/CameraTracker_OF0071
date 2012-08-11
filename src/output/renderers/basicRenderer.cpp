#include "basicRenderer.h"

void basicRenderer::setup(cameraTracking &cameraTracker)
{
	outputRenderer::setup( cameraTracker );
	useOpticalFlow = true;
	tracking->CTdoOpticalFlow = useOpticalFlow;
}


void basicRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
}

void basicRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
}


void basicRenderer::render()
{
	tracking->setOutContourColour( 0, 255, 255 );

	tracking->drawContours( outputX, outputY, outputW, outputH );
	tracking->drawOpticalFlow( outputX, outputY, outputW );
}
