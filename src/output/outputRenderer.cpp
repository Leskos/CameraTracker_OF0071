#include "outputRenderer.h"


//--------------------------------------------------------------------------
outputRenderer::outputRenderer(){

	fadeToBlack  = true;
	redrawScreen = true;
	
	pSizeMin = 3;
	pSizeMax = 30;
	pLifeMin = 30;
	pLifeMax = 70;
	pDragMin = 0.90;
	pDragMax = 0.96;
	particles.init(20);

	maxParticlesPerFrame = 40;
	
	pHue = 0;
	pSat = 140;
	pBri = 200;
	pHueCycleSpeed = 40;

	opFlowForceScale  = 5;
	opFlowAvgArea     = 5;
	useOpticalFlow    = true;

	drawPaths         = true;
	pathMaxVertices   = 50;
	pathSmoothAmount  = 0;
	pathFillColour    = ofColor(   0, 255, 0 );

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

	particleColour.setHsb( fmodf(ofGetElapsedTimef()*pHueCycleSpeed,255), pSat, pBri );

	
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
		particles.update();
		particles.render();
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

									particleColour

									);
		p->setColor( ofColor::fromHsb( fmodf(particleColour.getHue()+ofRandom(0,6)*5,255), pSat, pBri ) );
		p->setLifetime( ofRandom( pLifeMin, pLifeMax ) );
		p->setDrag( ofRandom( pDragMin, pDragMax ) );

		particles.addParticle(p);
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
