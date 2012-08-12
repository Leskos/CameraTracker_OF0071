#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(void)
{
	// Draw app fullscreen over 2 monitors 
	ofSetWindowPosition( 0, 0 );
	ofSetWindowShape( ofGetScreenWidth()*2, ofGetScreenHeight() );
	
	ofBackground( 0, 0, 0 );
	autoBg = true;
	ofSetBackgroundAuto( autoBg );
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	

	// DO NOT CHANGE - this messes stuff up at other resolutions 
	//                (###=> FIX THIS <=###) 
	camX = 320;
	camY = 240;

	drawImages    = true;
	drawDebugInfo = true;

	vidWarpBox.setup( 20, 220, camX*2, camY*2 );
	
	cameraTracker.setup( camX, camY );

	mouseParticles.init( 10, ofVec2f(50,50) );

	// Setup the GUI contents
	gui.addTitle( "IMG PROCESSING", 30);
	gui.addSlider("Blur Amount",     cameraTracker.CTblur,   1, 30);
	gui.addSlider("Threshold",       cameraTracker.CTthresh, 1, 255);
	gui.addToggle("Flip Horizontal", cameraTracker.CTflipHorizontal);
	gui.addToggle("Flip Vertical",   cameraTracker.CTflipVertical);

	gui.addTitle( "OPTICAL FLOW", 30 );
	gui.addToggle( "Do Optical Flow", cameraTracker.CTdoOpticalFlow );
	gui.addSlider( "OF Cols Step", cameraTracker.OFcolsStep, 1, 15 );
	gui.addSlider( "OF Rows Step", cameraTracker.OFrowsStep, 1, 15 );
	gui.addSlider( "threshold",    cameraTracker.OFactivityThresh, 1, 20 );

	gui.addTitle( "OUTPUT", 30);
	gui.addToggle( "Draw Images", drawImages);

	gui.currentPage().setXMLName("cameraTracker_gui_settings.xml");
	gui.currentPage().setName("Tracking Options");
	gui.saveToXML();

	currOutput = 0;
	numOutputs = 6;

	// Initialise basic renderer
	// -----------------------------------------
	output[0] = new basicRenderer;
	output[0]->setup( cameraTracker );
	output[0]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Basic Renderer");
	gui.setPage("Basic Renderer");
	gui.currentPage().setXMLName("basic_renderer_settings.xml");
	gui.addTitle("Nothing Here Yet");

	// Initialise bubble renderer
	// -----------------------------------------
	output[1] = new bubbleRenderer;
	output[1]->setup( cameraTracker );
	output[1]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Bubble Renderer");
	gui.setPage("Bubble Renderer");
	gui.currentPage().setXMLName("bubble_renderer_settings.xml");
	if ( bubbleRenderer * r = dynamic_cast<bubbleRenderer*>( output[1] ) ){			
		gui.addTitle("Random Parameters");
		gui.addSlider( "Life min", r->lifeMin, 1,  100 );
		gui.addSlider( "Life max", r->lifeMax, 10, 300 );
		gui.addSlider( "Size min", r->sizeMin, 1,  100 );
		gui.addSlider( "Size max", r->sizeMax, 2,  300 );
		gui.addSlider( "Drag min", r->dragMin, 0.7, 0.95 );
		gui.addSlider( "Drag max", r->dragMax, 0.8, 1.0 );
		gui.addTitle("Optical Flow Stuff");
		gui.addSlider( "Averaging area", r->ofAvgArea,          1, 40 );
		gui.addSlider( "Force scale",    r->opFlowForceScale, 10, 300 );
	}

	// Initialise smoke renderer
	// -----------------------------------------
	output[2] = new smokeRenderer;
	output[2]->setup( cameraTracker );
	output[2]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Sand Renderer");
	gui.setPage("Sand Renderer");
	gui.addTitle("Nothing Here Yet");

	// Initialise path renderer
	// -----------------------------------------
	output[3] = new pathRenderer;
	output[3]->setup( cameraTracker );
	output[3]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Path Renderer");
	gui.setPage("Path Renderer");
	gui.currentPage().setXMLName("path_renderer_gui_settings.xml");
	gui.addTitle("Simplification Settings");
	if ( pathRenderer * r = dynamic_cast<pathRenderer*>( output[3] ) ){	
		gui.addSlider( "Max Vertices", r->maxVertices, 5, 100 );
		gui.addSlider( "Smoothing",    r->smoothAmt,   0, 10  );
	}

	// Initialise sand renderer
	// -----------------------------------------
	output[4] = new sandRenderer();
	output[4]->setup( cameraTracker );
	output[4]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Sand Renderer");
	gui.setPage("Sand Renderer");
	gui.currentPage().setXMLName("sand_renderer_gui_settings.xml");
	if ( sandRenderer * r = dynamic_cast<sandRenderer*>( output[4] ) ){			
		gui.addTitle("Random Parameters");
		gui.addSlider( "Life min", r->lifeMin, 1,  100 );
		gui.addSlider( "Life max", r->lifeMax, 10, 300 );
		gui.addSlider( "Size min", r->sizeMin, 1,  100 );
		gui.addSlider( "Size max", r->sizeMax, 2,  300 );
		gui.addSlider( "Drag min", r->dragMin, 0.7, 0.95 );
		gui.addSlider( "Drag max", r->dragMax, 0.8, 1.0 );
		gui.addTitle("Optical Flow Stuff");
		gui.addSlider( "Averaging area", r->ofAvgArea,          1, 40 );
		gui.addSlider( "Force scale",    r->opFlowForceScale, 10, 300 );
	}

	// Initialise subliminal renderer
	// -----------------------------------------
	output[5] = new subliminalRenderer();
	output[5]->setup( cameraTracker );
	output[5]->setOutputArea( 40+camX*2, 220, camX*2, camY*2 );
	gui.addPage("Subliminal Renderer");
	gui.setPage("Subliminal Renderer");
	gui.currentPage().setXMLName("subliminal_renderer_settings.xml");
	if ( subliminalRenderer * r = dynamic_cast<subliminalRenderer*>( output[5] ) ){			
		gui.addTitle("Particles");
		gui.addSlider( "Life min", r->lifeMin, 1,  100 );
		gui.addSlider( "Life max", r->lifeMax, 10, 300 );
		gui.addSlider( "Size min", r->sizeMin, 1,  100 );
		gui.addSlider( "Size max", r->sizeMax, 2,  300 );
		gui.addSlider( "Drag min", r->dragMin, 0.7, 0.95 );
		gui.addSlider( "Drag max", r->dragMax, 0.8, 1.0 );
		gui.addTitle("Optical Flow");
		gui.addSlider( "Averaging area", r->ofAvgArea,          1, 40 );
		gui.addSlider( "Force scale",    r->opFlowForceScale, 10, 300 );
		gui.addTitle("Images");
		gui.addSlider( "Image Number", r->currentImage, 0, 6 );
		gui.addToggle( "Fade Out", r->fadeOut );
		gui.addToggle( "Randomised Colours", r->getColorRandomly );

	}
	
	
	//gui.addColorPicker("Outline col");
	//gui.addColorPicker("Motion  col");
	gui.setPage("Tracking Options");
	gui.setDefaultKeys(false);
	gui.setAutoSave(false);
}




//--------------------------------------------------------------
void testApp::update()
{
	// Perform camera tracking
	cameraTracker.doTracking( vidWarpBox.dstPoints[3].x/2, vidWarpBox.dstPoints[3].y/2,
						      vidWarpBox.dstPoints[2].x/2, vidWarpBox.dstPoints[2].y/2,
						      vidWarpBox.dstPoints[1].x/2, vidWarpBox.dstPoints[1].y/2,
						      vidWarpBox.dstPoints[0].x/2, vidWarpBox.dstPoints[0].y/2);
	mouseParticles.run();
}


//--------------------------------------------------------------
void testApp::draw()
{

	// Draw our report string
	if( drawDebugInfo ){
		char reportStr[1024];
		sprintf( reportStr, "FPS: %f \nOutline points : %i\nMotion Points : %i\nCurrent Output : %i", ofGetFrameRate(), cameraTracker.getOutContourLength(), cameraTracker.getMotContourLength(), currOutput );
		ofSetHexColor(0xffffff);
		ofDrawBitmapString(reportStr, 10, 20);
	}
	

	// Draw processing stages if required
	if( drawImages )
	{
		cameraTracker.drawProcessingImages();
		ofSetHexColor(0xffffff);
		vidWarpBox.draw();
	}

	output[currOutput]->render();
	mouseParticles.render();
	gui.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
	switch (key)
	{
		// TOGGLE GUI
		case ' ':
			gui.toggleDraw();
			break;
		
		// TOGGLE FULLSCREEN
		case 'f':
			ofToggleFullscreen();
			break;
		
		// TOGGLE SHOW IMAGES
		case 'i':
			
			cameraTracker.doTracking( vidWarpBox.dstPoints[3].x/2, vidWarpBox.dstPoints[3].y/2,
									  vidWarpBox.dstPoints[2].x/2, vidWarpBox.dstPoints[2].y/2,
									  vidWarpBox.dstPoints[1].x/2, vidWarpBox.dstPoints[1].y/2,
									  vidWarpBox.dstPoints[0].x/2, vidWarpBox.dstPoints[0].y/2 );	

			drawImages = !drawImages;
			break;

		// TOGGLE OPTICAL FLOW
		case 'o':
			cameraTracker.CTdoOpticalFlow = !cameraTracker.CTdoOpticalFlow;
			break;
			
		// RESET WARPING BOX
		case 'r':
			for( int i=0; i<4; i++)
			{
				vidWarpBox.dstPoints[i].x = vidWarpBox.srcPoints[i].x;
				vidWarpBox.dstPoints[i].y = vidWarpBox.srcPoints[i].y;
			}
			break;
		
		// LEARN BACKGROUND
		case 'b':
			cameraTracker.learnBackGround();
			break;
		
		// TOGGLE AUTO CLEAR BACKGROUND
		case 'a':
			autoBg = !autoBg;
			ofSetBackgroundAuto( autoBg );
			break;

		// INCREASE THRESHOLD
		case '+':
			cameraTracker.CTthresh ++;
			if ( cameraTracker.CTthresh > 255)
			{
				cameraTracker.CTthresh = 255;
			}
			break;
		
		// DECREASE THRESHOLD
		case '-':
			cameraTracker.CTthresh --;
			if (cameraTracker.CTthresh < 0)
			{
				cameraTracker.CTthresh = 0;
			}
			break;
		
		// OUTPUT TO FIRST SCREEN FULLSCREEN
		case '1':
			for( int i=0; i < numOutputs; i++ ){				
				output[ i ]->setOutputArea( 0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
			}
			break;

		// OUTPUT TO SECOND SCREEN FULLSCREEN
		case '2':
			for( int i=0; i < numOutputs; i++ ){				
				output[ i ]->setOutputArea( ofGetScreenWidth(), 0, ofGetScreenWidth(), ofGetScreenHeight() );
			}
			break;

		// OUTPUT TO FIRST SCREEN SMALL
		case '3':
			for( int i=0; i < numOutputs; i++ ){				
				output[ i ]->setOutputArea( 20+camX*2, 220, camX*2, camY*2 );
			}
			break;
	
		// LOAD GUI SETTINGS FROM XML
		case 'x':
			gui.currentPage().loadFromXML();
			break;

		// LOAD GUI SETTINGS FROM XML
		case 'z':
			gui.currentPage().saveToXML();
			break;

		// DISPLAY TRACKING SETTINGS
		case 's':
			gui.setPage("TrackingOptions");
			//cameraTracker.showSettings();
			break;

		case 'k':
			if( currOutput == 0 ){
				changeOutputRenderer( numOutputs-1 );
			}
			else{
				changeOutputRenderer( (currOutput-1) % numOutputs );
			}
			break;

		case 'l':
			changeOutputRenderer( (currOutput+1) % numOutputs );
			break;
		case 'd':
			drawDebugInfo = !drawDebugInfo;
			break;
			

	}
}


void testApp::changeOutputRenderer( int newOutput ){

	output[currOutput]->releaseResources();

	currOutput = newOutput;
	output[currOutput]->initialiseResources();
	cameraTracker.doTracking( vidWarpBox.dstPoints[3].x/2, vidWarpBox.dstPoints[3].y/2,
						      vidWarpBox.dstPoints[2].x/2, vidWarpBox.dstPoints[2].y/2,
						      vidWarpBox.dstPoints[1].x/2, vidWarpBox.dstPoints[1].y/2,
						      vidWarpBox.dstPoints[0].x/2, vidWarpBox.dstPoints[0].y/2 );
	switch( currOutput ){
		case 0 : gui.setPage("Tracking Options");
				 break;
		case 1 : gui.setPage("Bubble Renderer");
				 break;
		case 2 : gui.setPage("Tracking Options");
				 break;
		case 3 : gui.setPage("Path Renderer");
				 break;
		case 4 : gui.setPage("Sand Renderer");
				 break;
		case 5 : gui.setPage("Subliminal Renderer");
				 break;
	}
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	vidWarpBox.mouseDragged( x, y );

	Particle *p = new Particle( ofVec2f( x, y) );
	mouseParticles.addParticle(p);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	vidWarpBox.mousePressed( x, y );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	vidWarpBox.mouseReleased( );
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}


