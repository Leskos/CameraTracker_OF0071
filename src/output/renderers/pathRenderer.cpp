/*#include "pathRenderer.h"

void pathRenderer::setup(motionTracking *motionTracker)
{
	outputRenderer::setup( motionTracker );

	maxVertices = 100;
	smoothAmt   = 0.01;
	
	tracking->CTdoOpticalFlow = false;
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
	tracking->CTdoOpticalFlow = false;

	updateSimplePaths();

	float scaleX = outputW / tracking->getImgX();
	float scaleY = outputH / tracking->getImgY();

	ofGetCurrentRenderer()->translate( outputX, outputY );
	ofGetCurrentRenderer()->scale( scaleX, scaleY );
	
	// Draw the filled in paths
	for( int i = 0; i < simplePaths.size(); i++ ){
		simplePaths.at(i).draw( );
	}

	
	//
	// !!!! NOT WORKING !!! - Should draw connecting lines between points
	//
	ofSetColor( ofColor( 255, 0, 0 ) );
	ofSetLineWidth(3.0);
	for( int pointsJoined = 0 ; pointsJoined > 50;  ){

		int randomLineA  = ofRandom( 0, simplePolylines.size() );
		int randomLineB  = ofRandom( 0, simplePolylines.size() );
		int randomVertA  = ofRandom( 0, simplePolylines.at(randomLineA).getVertices().size() );
		int randomVertB  = ofRandom( 0, simplePolylines.at(randomLineB).getVertices().size() );

		ofPoint pointA = simplePolylines.at (randomLineA ).getVertices().at( randomVertA );
		ofPoint pointB = simplePolylines.at (randomLineA ).getVertices().at( randomVertA );
		
		//if( pointA.distance( pointB ) > 100 ){
		ofLine( pointA.x, pointA.y, pointB.x, pointB.y );
		pointsJoined += 1;
		//}
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


		// create an ofPath from the ofPolyline's vertices
		path = ofPath();

			vector<ofPoint> vertices = contour.getVertices();

			for( int i=0; i<vertices.size(); i++ ){
				path.lineTo( vertices.at(i).x, vertices.at(i).y );
			}
		
			path.setFillColor( ofColor( 200, 150, 100, 100 ) );
			path.setStrokeColor( ofColor( 255, 255, 255, 150 ) );
			path.setStrokeWidth( 5 );
			path.setFilled( true );

		path.close();


		simplePaths.push_back( path );
		simplePolylines.push_back( contour );
	}
}
*/