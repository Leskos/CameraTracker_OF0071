// Simple Particle System
// Based on code by
// Daniel Shiffman <http://www.shiffman.net>

// A class to describe a group of Particles
// An ArrayList is used to manage the list of Particles 

#ifndef _PARTICLE_SYSTEM
#define _PARTICLE_SYSTEM

#include "Particle.h"


class ParticleSystem {

	public:

		vector<Particle *> particles;     // A dynamic array for all the particles
		ofVec2f origin;				// An origin point for where particles are birthed
		
		ParticleSystem();

		void init();

		void update();
		
		void render();

		void addParticle(Particle *p);

		void addParticleAtLoc(ofVec2f loc);

		void setOrigin( ofVec2f newOrigin );

		// A method to test if the particle system still has particles
		bool dead();
		
		bool isParticleDead(Particle _particle);

		void clearParticles();

		float lastFrameTime;
  
};
#endif



