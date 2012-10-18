
/*
#include "subliminalRenderer.h"

void subliminalRenderer::setup(motionTracking *motionTracker)
{
	outputRenderer::setup( &motionTracker );

	// Default values for controllable parameters
	sizeMin          = 1;
	sizeMax          = 30;
	lifeMin          = 10;
	lifeMax          = 60;
	dragMin          = 0.95;
	dragMax          = 0.98;
	ofAvgArea        = 10;
	opFlowForceScale = 140;
	
	tracking->CTdoOpticalFlow = true;
	
	currentImage     = 0;
	fadeOut          = true;
	getColorRandomly = true;

	subliminalImage[0].loadImage("aSveso_1.jpg");
	subliminalImage[1].loadImage("aSveso_2.jpg");
	subliminalImage[2].loadImage("aSveso_3.jpg");
	subliminalImage[3].loadImage("aSveso_4.jpg");
	subliminalImage[4].loadImage("aSveso_5.jpg");
	subliminalImage[5].loadImage("trees.jpg");
}


void subliminalRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = true;
	particles.init( 60 );
}

void subliminalRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
	particles.clearParticles();
}


void subliminalRenderer::render()
{
	
	tracking->CTdoOpticalFlow = false;

	ofColor colorFromImage;

	int numContourPoints = tracking->motionContours.blobs.size();
	if( numContourPoints > 0 ){
		if( tracking->motionContours.blobs[0].pts.size() > 0 ){

			// EMIT A NUMER OF PARTICLES PROPORTIONAL TO 
			// THE NUMBER OF POINTS ON THE OUTLINE CONTOUR
			int rangeMin = ofRandom(2, 7);
			int rangeMax = ofMap( numContourPoints, 0, 1000, 10, 100 );
			int particlesPerFrame = ofRandom( rangeMin, rangeMax );

			for( int i=0; i<particlesPerFrame; i++ ){
					
				int randBlob  = (int)ofRandom( 0, numContourPoints );
				int randPoint = (int)ofRandom( 0, (tracking->motionContours.blobs[ randBlob ].pts.size())-1 );
				
				ofPoint point = tracking->motionContours.blobs[ randBlob ].pts[ randPoint ];
				
				// Average the local optical flow
				ofVec2f flowAtPoint = tracking->opticalFlow.averageFlowInRegion( point.x, point.y, ofAvgArea, ofAvgArea );

				Particle *p = new Particle( ofVec2f( ofRandom(-0.05, 0.05), ofRandom(-0.05, 0.05) ), //ofRandom(-0.1, 0.1 ),
													 //ofRandom( -0.1, 0.1) ),                     // acc

											ofVec2f( flowAtPoint.x * opFlowForceScale, 
													  flowAtPoint.y * opFlowForceScale ),  // vel

											ofVec3f( outputX + (point.x*outputScaleX), 
											         outputY + (point.y*outputScaleY) ),   // loc

											ofRandom( sizeMin, sizeMax )                   // size
											);


				// GET THE COLOUR RANDOMLY FROM THE IMAGE
				if( getColorRandomly ){
					colorFromImage = subliminalImage[currentImage].getColor( ofRandom(320), ofRandom(240) );
				}
				else{
					colorFromImage = subliminalImage[currentImage].getColor( point.x, point.y );
				}

				p->setColor( ofColor( colorFromImage.r, colorFromImage.g, colorFromImage.b, colorFromImage.a ) );
				
				p->setLifetime( ofRandom( lifeMin, lifeMax ) );
				p->setDrag(     ofRandom( dragMin, dragMax ) );

				particles.addParticle(p);
			}
		}
	}

	ofPushStyle();
	// Fade out if necessary
	if( fadeOut ){
		if( ofRandom( 0, 1 ) > 0.4 ){
			//ofSetColor(  ofColor( colorFromImage.r, colorFromImage.g, colorFromImage.b, 0.2 ) );
			ofSetColor( 0, 0, 0, 5 );
			ofFill();
			ofRect( outputX, outputY, outputW, outputH );
		}
	}
	ofPopStyle();

	particles.update();
	particles.render();

}
*/
