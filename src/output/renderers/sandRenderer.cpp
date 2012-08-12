#include "sandRenderer.h"

void sandRenderer::setup(cameraTracking &cameraTracker)
{
	outputRenderer::setup( cameraTracker );

	// Default values for controllable parameters
	sizeMin          = 10;
	sizeMax          = 50;
	lifeMin          = 40;
	lifeMax          = 70;
	dragMin          = 0.85;
	dragMax          = 0.9;
	ofAvgArea        = 10;
	opFlowForceScale = 140;
	
	tracking->CTdoOpticalFlow = false;
}


void sandRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = false;
}

void sandRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
	particles.clearParticles();
}


void sandRenderer::render()
{
	
	tracking->CTdoOpticalFlow = false;

	int numContourPoints = tracking->motionContours.blobs.size();

	if( numContourPoints > 0 ){
		if( tracking->motionContours.blobs[0].pts.size() > 0 ){

			// EMIT A NUMER OF PARTICLES PROPORTIONAL TO 
			// THE NUMBER OF POINTS ON THE OUTLINE CONTOUR
			int particlesPerFrame = ofRandom( 0, ofMap( numContourPoints, 0, 1000, 1, 5000 ) );

			for( int i=0; i<particlesPerFrame; i++ ){
					
				int randBlob  = (int)ofRandom( 0, numContourPoints );
				int randPoint = (int)ofRandom( 0, (tracking->motionContours.blobs[ randBlob ].pts.size())-1 );
				
				ofPoint point = tracking->motionContours.blobs[ randBlob ].pts[ randPoint ];
				
				// Average the local optical flow
				ofVec2f flowAtPoint = tracking->opticalFlow.averageFlowInRegion( point.x, point.y, ofAvgArea, ofAvgArea );

				Particle *p = new Particle( ofVec2f( ofRandom(-0.1,0.1),
													 ofRandom(0.90, 1.2)),                // acc
   
											ofVec2f( ofRandom(-1, 1), 
													 ofRandom( 5, 8) ),                   // vel

											ofVec3f( outputX + (point.x*outputScaleX), 
											         outputY + (point.y*outputScaleY) ),  // loc

											ofRandom( sizeMin, sizeMax )                  // size
											);

				p->setColor( ofColor( ofRandom(200,255), ofRandom(100,180), ofRandom(180, 220) ) );
				p->setLifetime( ofRandom( lifeMin, lifeMax ) );
				p->setDrag(     ofRandom( dragMin, dragMax ) );

				particles.addParticle(p);
			}
		}
	}
	particles.run();
	particles.render();

}
