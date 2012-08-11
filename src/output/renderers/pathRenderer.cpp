#include "pathRenderer.h"

void pathRenderer::setup(cameraTracking &cameraTracker)
{
	outputRenderer::setup( cameraTracker );

	maxVertices = 100;
	smoothAmt   = 0.01;

	initialiseResources();
}


void pathRenderer::initialiseResources()
{
	cout << "\nINITIALISING PATH RENDERER RESOURCES";
	tracking->CTdoOpticalFlow = false;
}

void pathRenderer::releaseResources()
{
	cout << "\nRELEASING BUBBLE RENDERER RESOURCES";
}


void pathRenderer::render()
{
	updateSimplePaths();

	float scaleX = outputW / tracking->getImgX();
	float scaleY = outputH / tracking->getImgY();

	ofGetCurrentRenderer()->translate( outputX, outputY );
	ofGetCurrentRenderer()->scale( scaleX, scaleY );
	
	for( int i = 0; i < simplePaths.size(); i++ ){
		simplePaths.at(i).draw( );
	}
	
	ofGetCurrentRenderer()->scale( (1/scaleX), (1/scaleY) );
	ofGetCurrentRenderer()->translate( -outputX, -outputY );
	
}

void pathRenderer::updateSimplePaths(){

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
		while( contour.size() > maxVertices ){
			simplifyAmt += 0.5;				
			contour.simplify( simplifyAmt );
		}
		if( smoothAmt > 0.1 ){
				contour = contour.getSmoothed( smoothAmt );
		}


		// create an ofPath from the ofPolyline
		path = ofPath();

			vector<ofPoint> vertices = contour.getVertices();

			for( int i=0; i<vertices.size(); i++ ){
				path.lineTo( vertices.at(i).x, vertices.at(i).y );
			}
		
			path.setFillColor( ofColor( 200, 100, 100, 100 ) );
			path.setStrokeColor( ofColor( 100, 100, 200 ) );
			path.setStrokeWidth( 10 );
			path.setFilled( true );

		path.close();


		simplePaths.push_back( path );
		simplePolylines.push_back( contour );
	}
}
