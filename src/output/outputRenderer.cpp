#include "outputRenderer.h"


//--------------------------------------------------------------------------
outputRenderer::outputRenderer(){

	fadeToBlack  = true;
	redrawScreen = true;
	
	useParticles = true;
	pSizeMin = 3;
	pSizeMax = 30;
	pLifeMin = 30;
	pLifeMax = 70;
	pDragMin = 0.90;
	pDragMax = 0.96;
	particles.init(20);

	pRandColRange = 25;
	pRandColInterval = 5;

	maxParticlesPerFrame = 40;
	
	hue   = 0;
	sat   = 140;
	bri   = 200;
	alpha = 10;
	hueCycleSpeed = 40;

	opFlowForceScale  = 20;
	opFlowAvgArea     = 5;
	useOpticalFlow    = true;

	usePaths          = true;
	pathMaxVertices   = 110;
	pathSmoothAmount  = 0;

	colourFromImg         = true; 
	colourFromImgRandomly = true;
	currentImg            = 1;

	showDebugInfo = false;
	fileName = "defaultRenderer";
}


//--------------------------------------------------------------------------
void outputRenderer::setup( motionTracking &motionTracker )
{
	tracking = &motionTracker;
	tracking->CTdoOpticalFlow = false;
	
	setOutputArea( 0, ofGetScreenWidth(), ofGetScreenWidth(), ofGetScreenHeight() );
}


//--------------------------------------------------------------------------
void outputRenderer::initialiseResources( void )
{
	cout << "\nINITIALISING OUTPUT RENDERER RESOURCES";
}


//--------------------------------------------------------------------------
void outputRenderer::releaseResources( void )
{
	cout << "\nRELEASING OUTPUT RENDERER RESOURCES";
}


//--------------------------------------------------------------------------
void outputRenderer::update(){

	
	useOpticalFlow = false;
	if( opFlowForceScale > 0 ){
		useOpticalFlow = true;
		tracking->CTdoOpticalFlow = useOpticalFlow;
	}

	ofSetBackgroundAuto( redrawScreen );

	colour.setHsb( fmodf(ofGetElapsedTimef()*hueCycleSpeed,255), sat, bri, alpha );

		
	if( tracking->motionContours.blobs.size() > 0 ){
			if( tracking->motionContours.blobs[0].pts.size() > 0 ){
				seedNewParticles();
			}
	}

}


//--------------------------------------------------------------------------
void outputRenderer::draw()
{
	// Draw the output of the tracking
	
	if( showDebugInfo ){
		tracking->setColorOutContours( ofColor( 200, 100, 100 ) );
		tracking->setColorMotContour(  ofColor( 100, 200, 200 ) );
		tracking->drawContours( outputX, outputY, outputW, outputH );
	}
	else{

		if( useParticles ){
			particles.update();
			particles.render();
		}

		if( usePaths ){
			updatePaths();
			drawPaths();
		}
	}
	
}


//--------------------------------------------------------------------------
void outputRenderer::seedNewParticles(){

	// Calculate how many particles to emit this frame, proportional to amount of motion present
	int particlesThisFrame = ofMap( tracking->getMotContourLength(), 0, 500, 0, maxParticlesPerFrame );

	for( int i=0; i<particlesThisFrame; i++ ){
		
		// Select a random point on the outline of the motion
		int randBlob  = (int)ofRandom( 0, (tracking->motionContours.blobs.size()) );
		int randPoint = (int)ofRandom( 0, (tracking->motionContours.blobs[ randBlob ].pts.size())-1 );
				
		ofPoint point = tracking->motionContours.blobs[ randBlob ].pts[ randPoint ];
					
		// Average the local optical flow
		ofVec2f flowAtPoint = tracking->opticalFlow.averageFlowInRegion( point.x, point.y, opFlowAvgArea, opFlowAvgArea );

		// Create a new particle
		Particle *p = new Particle( ofVec2f(0,0),    //ofxVec2f( flowAtPoint.x*0.02, 
													//		  flowAtPoint.y*0.02 ),                // acc
	
									ofVec2f( flowAtPoint.x * opFlowForceScale, 
										 	 flowAtPoint.y * opFlowForceScale ),                  // vel
	
									ofVec3f( outputX + (point.x*outputScaleX), 
											outputY + (point.y*outputScaleY) ),  // loc
	
									ofRandom( pSizeMin, pSizeMax ),

									ofColor::fromHsb( fmodf( colour.getHue()+ofRandom( 0, pRandColRange ) * pRandColInterval, 255 ), sat, bri, alpha ) 

									);
		p->setLifetime( ofRandom( pLifeMin, pLifeMax ) );
		p->setDrag( ofRandom( pDragMin, pDragMax ) );

		particles.addParticle(p);
	}
}


//--------------------------------------------------------------------------
void outputRenderer::setOutputArea(int x, int y, int w, int h)
{
	outputX = x;
	outputY = y;
	outputW = w;
	outputH = h;

	cout << "\nSetting Output Area To " << x << "," << y << "," << w << "," << h;

	outputScaleX = outputW / tracking->getImgX();
	outputScaleY = outputH / tracking->getImgY();
}


//--------------------------------------------------------------------------
void outputRenderer::setFileName( string newFileName ){
	fileName = newFileName;
}



//--------------------------------------------------------------------------
void outputRenderer::updatePaths(){

	simplePolylines.clear();
	simplePolylines.reserve( tracking->outlineContours.blobs.size() );

	simplePaths.clear();
	simplePaths.reserve(    tracking->outlineContours.blobs.size() );
		
	ofPolyline contour;
	ofPath     path;

	// Loop through all of the tracked blobs
	for ( int i = 0; i < tracking->outlineContours.blobs.size(); i++ ){

		// Create an ofPolyline from the blobs points
		contour = tracking->outlineContours.blobs[i].pts;
		contour.setClosed( true );

		float simplifyAmt = 1;
		contour.simplify( simplifyAmt );

		// Simplify the ofPolyline until it is composed of few enough vertices
		while( contour.size() > pathMaxVertices ){
			simplifyAmt += 0.5;				
			contour.simplify( simplifyAmt );
		}
		if( pathSmoothAmount > 0.1 ){
				contour = contour.getSmoothed( pathSmoothAmount );
		}


		// create an ofPath from the ofPolyline's vertices
		path = ofPath();

			vector<ofPoint> vertices = contour.getVertices();

			for( int i=0; i<vertices.size(); i++ ){
				path.lineTo( vertices.at(i).x, vertices.at(i).y );
			}
		
			path.setFillColor( colour );
			path.setStrokeColor( ofColor( 255, 255, 255, 10 ) );
			path.setStrokeWidth( 10 );
			path.setFilled( true );

		path.close();


		simplePaths.push_back( path );
		simplePolylines.push_back( contour );
	}
}


//--------------------------------------------------------------------------
void outputRenderer::drawPaths(){

	ofGetCurrentRenderer()->translate( outputX, outputY );
	ofGetCurrentRenderer()->scale( outputScaleX, outputScaleY );
	
	
	ofEnableAlphaBlending();
	for( int i = 0; i < simplePaths.size(); i++ ){
		simplePaths.at(i).setFillColor( colour );
		simplePaths.at(i).draw( );
	}
			
	ofGetCurrentRenderer()->scale( (1/outputScaleX), (1/outputScaleY) );
	ofGetCurrentRenderer()->translate( -outputX, -outputY );
}