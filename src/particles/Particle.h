// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A simple Particle class

#ifndef _PARTICLE
#define _PARTICLE

#include "ofMain.h"
#include "ofVectorMath.h"
#include "ofAddons.h"


class Particle {

	public:
	
		ofVec2f loc;
		ofVec2f vel;
		ofVec2f acc;

		ofVec2f prevLoc;

		float radius;
		float timer;

		ofColor color;

		float drag;


		// One constructor
		Particle(ofVec2f a, ofVec2f v, ofVec2f l, float r_);

		// Another constructor 
		Particle(ofVec2f l);

		// Method to update location
		void update( float timeDelta );

		// Method to display
		void render();

		// Is the particle still useful?
		bool dead();

		void setColor( ofColor _color );

		void setDrag( float _drag );

		void setLifetime(float _timer);
};
#endif


