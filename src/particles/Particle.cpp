// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A simple Particle class

#include "Particle.h"

//--------------------------------------------------------------
// One constructor
Particle::Particle(ofVec2f a, ofVec2f v, ofVec2f l, float _radius) {

	acc = a;
	vel = v;;
	loc = l;
	prevLoc = loc;
	radius  = _radius;

	drag = ofRandom( 0.93, 0.97 );

	timer =  ofRandom( 20, 50 );

	color = ofColor( ofRandom( 0, 255 ), ofRandom( 0, 255 ), ofRandom( 0, 255 ) );

}

//--------------------------------------------------------------
// Another constructor (the one we are using here)
Particle::Particle( ofVec2f l ) {

	acc.set( ofRandom(-0.03,0.03), ofRandom(0,-0.1) );
	vel.set( ofRandom(-0.01,0.01),ofRandom(-0.01,0.01));
	loc = l;
	prevLoc = loc;
	radius  = ofRandom( 5, 25 );
	timer   = 100.0;
	color   = ofColor( ofRandom( 0, 255 ), ofRandom( 0, 255 ), ofRandom( 0, 255 ) );
}

//--------------------------------------------------------------
// Method to update location
void Particle::update() { 

	vel.x *= drag;
	vel.y *= drag;
	
	vel     += acc;
	prevLoc  = loc;
	loc     += vel;
	timer   -= 1.0;
}

//--------------------------------------------------------------
// Method to display
void Particle::render() {
	ofEnableAlphaBlending();
	ofSetColor( color ,timer);
	ofFill();
	//ofSetLineWidth( 3 );
	//ofLine( loc.x, loc.y, prevLoc.x, prevLoc.y );
	ofEllipse(loc.x,loc.y,radius,radius);
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
// Is the particle still useful?
bool Particle::dead() {
	if (timer <= 0.0) {
		return 1;
	} else {
		return 0;
	}
}

void Particle::setColor( ofColor _color ){
	color = _color;
}

void Particle::setDrag(float _drag){
	drag = _drag;
}

void Particle::setLifetime( float _timer ){
	timer = _timer;
}


