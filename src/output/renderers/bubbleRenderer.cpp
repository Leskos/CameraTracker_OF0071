#include "bubbleRenderer.h"

void bubbleRenderer::setup(cameraTracking &cameraTracker)
{
	outputRenderer::setup( cameraTracker );

	// Default values for controllable parameters
	sizeMin          = 10;
	sizeMax          = 50;
	lifeMin          = 40;
	lifeMax          = 100;
	dragMin          = 0.9;
	dragMax          = 0.98;
	ofAvgArea        = 15;
	opFlowForceScale = 60;
	
	tracking->CTdoOpticalFlow = true;
}


void bubbleRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = true;
	particles.init();
}

void bubbleRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
	particles.clearParticles();
}


void bubbleRenderer::render()
{
	
	tracking->CTdoOpticalFlow = true;

	tracking->setColorOutContours( ofColor( 355, 0, 0 ) );
	//tracking->drawContours( outputX, outputY, outputW, outputH );
	//tracking->drawOpticalFlow( outputX, outputY, outputW );

	if( tracking->motionContours.blobs.size() > 0 ){
		if( tracking->motionContours.blobs[0].pts.size() > 0 ){
			
			for( int i=0; i<20; i++ ){
					
				int randBlob  = (int)ofRandom( 0, (tracking->motionContours.blobs.size()) );
				int randPoint = (int)ofRandom( 0, (tracking->motionContours.blobs[ randBlob ].pts.size())-1 );
				
				ofPoint point = tracking->motionContours.blobs[ randBlob ].pts[ randPoint ];
				
				// Average the local optical flow
				ofVec2f flowAtPoint = tracking->opticalFlow.averageFlowInRegion( point.x, point.y, ofAvgArea, ofAvgArea );

				Particle *p = new Particle( ofVec2f(0,0),    //ofxVec2f( flowAtPoint.x*0.02, 
											                  //		  flowAtPoint.y*0.02 ),                // acc

											ofVec2f( flowAtPoint.x * opFlowForceScale, 
													 flowAtPoint.y * opFlowForceScale ),                  // vel

											ofVec3f( outputX + (point.x*outputScaleX), 
											         outputY + (point.y*outputScaleY) ),  // loc

											ofRandom( sizeMin, sizeMax )
											);

				p->setLifetime( ofRandom( lifeMin, lifeMax ) );
				p->setDrag(     ofRandom( dragMin, dragMax ) );

				particles.addParticle(p);
			}
		}
	}
	particles.update();
	particles.render();

}
