#include "smokeRenderer.h"

void smokeRenderer::setup(cameraTracking &cameraTracker)
{
	outputRenderer::setup( cameraTracker );
	tracking->CTdoOpticalFlow = true;
}


void smokeRenderer::initialiseResources()
{
	cout << "\nINITIALISING BUBBLE RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = true;
	particles.init( 60 );
}

void smokeRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
	particles.clearParticles();
}


void smokeRenderer::render()
{
	tracking->CTdoOpticalFlow = true;

	tracking->setColorOutContours( ofColor( 255, 0, 0 ) );
	
	//tracking->drawOpticalFlow( outputX, outputY, outputW );

		if( tracking->outlineContours.blobs.size() > 0 ){
		if( tracking->outlineContours.blobs[0].pts.size() > 0 ){
			
			for( int i=0; i<10; i++ ){
					
				int randBlob  = (int)ofRandom( 0, (tracking->outlineContours.blobs.size()) );
				int randPoint = (int)ofRandom( 0, (tracking->outlineContours.blobs[ randBlob ].pts.size())-1 );
				
				ofPoint point = tracking->outlineContours.blobs[ randBlob ].pts[ randPoint ];
				
				ofVec2f flowAtPoint = tracking->opticalFlow.flowInRegion( point.x, point.y, 20, 20 ) / 400;

				Particle *p = new Particle( ofVec2f( ofRandom(-0.1,0.1), ofRandom(-0.1,-0.2) ),              // acc

											ofVec2f( ofRandom( -2, 2 ), ofRandom( -6, -20 ) ),			                  // vel

											ofVec3f( outputX + (point.x*outputScaleX), 
											          outputY + (point.y*outputScaleY) ),  // loc

											ofRandom( 10, 70 )
											);
				p->setColor( ofColor( ofRandom(180,230), ofRandom(150,180), ofRandom(120,140), ofRandom(1,10) ) );

				particles.addParticle(p);
			}
		}
	}

	if( tracking->motionContours.blobs.size() > 0 ){
		if( tracking->motionContours.blobs[0].pts.size() > 0 ){
			
			for( int i=0; i<20; i++ ){
					
				int randBlob  = (int)ofRandom( 0, (tracking->motionContours.blobs.size()) );
				int randPoint = (int)ofRandom( 0, (tracking->motionContours.blobs[ randBlob ].pts.size())-1 );
				
				ofPoint point = tracking->motionContours.blobs[ randBlob ].pts[ randPoint ];
				
				//ofVec2f flowAtPoint  = tracking->opticalFlow.flowAtPoint( point.x, point.y );
				ofVec2f flowAtPoint = tracking->opticalFlow.flowInRegion( point.x, point.y, 20, 20 ) / 400;

				Particle *p = new Particle( ofVec2f(0,0),    //ofxVec2f( flowAtPoint.x*0.02, 
											                  //		  flowAtPoint.y*0.02 ),                // acc

											ofVec2f( flowAtPoint.x * 20, 
													  flowAtPoint.y * 20 ),                  // vel

											ofVec3f( outputX + (point.x*outputScaleX), 
											          outputY + (point.y*outputScaleY) ),  // loc

											ofRandom( 5, 50 )
											);
					
				/*
				Particle *p = new Particle( ofxVec2f( outputX + (point.x * outputScaleX), 
													  outputY + (point.y * outputScaleY) ) );
			    */
				p->setLifetime( ofRandom( 20, 90 ) );

				particles.addParticle(p);
			}
		}
	}
	particles.update();
	particles.render();

	//tracking->drawContours( outputX, outputY, outputW, outputH );

}
