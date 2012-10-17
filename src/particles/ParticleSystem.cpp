// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A class to describe a group of Particles
// An ArrayList is used to manage the list of Particles 

#include "ParticleSystem.h"

//--------------------------------------------------------------
ParticleSystem::ParticleSystem() {
   
}


//--------------------------------------------------------------
void ParticleSystem::init( int targetFrameRate ){
	updateInterval = 1000/targetFrameRate;
	lastUpdate     = ofGetElapsedTimeMillis();
}


//--------------------------------------------------------------
void ParticleSystem::update() {

	float timeSinceUpdate = ofGetElapsedTimeMillis() - lastUpdate;
	
	while( timeSinceUpdate > updateInterval ){

		// Cycle through the ArrayList backwards b/c we are deleting
		for (int i = particles.size()-1; i >= 0; i--) {
			particles[i]->update( );
			if (particles[i]->dead()) {
				particles.erase(particles.begin() + i);
			}
		}
		lastUpdate += updateInterval;
		timeSinceUpdate -= updateInterval;
	}

	
}
//--------------------------------------------------------------
void ParticleSystem::render() {
	// Cycle through the ArrayList and render particles	
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->render();
	}
}


//--------------------------------------------------------------
void ParticleSystem::addParticle( Particle *p ) {
	particles.push_back( p );
}

//--------------------------------------------------------------
void ParticleSystem::addParticleAtLoc( ofVec2f loc ) {
	particles.push_back( new Particle( loc ) );
}

//--------------------------------------------------------------
void ParticleSystem::setOrigin( ofVec2f newOrigin ) {
	origin = newOrigin;
}

//--------------------------------------------------------------
// A method to test if the particle system still has particles
bool ParticleSystem::dead() {
	if (particles.empty()) {
		return 1;
	} else {
		return 0;
	}
}

void ParticleSystem::clearParticles(){
	particles.clear();
}





