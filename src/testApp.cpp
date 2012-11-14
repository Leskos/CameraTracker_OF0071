#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(void)
{
	// Draw app fullscreen over 2 monitors 
	ofSetWindowPosition( 0, 0 );
	ofSetWindowShape( ofGetScreenWidth()*2, ofGetScreenHeight() );
	
	ofBackground( 0, 0, 0 );
	ofSetBackgroundAuto( true );
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	

	// DO NOT CHANGE - this messes stuff up at other resolutions 
	//                (###=> FIX THIS <=###) 
	camX = 320;
	camY = 240;

	drawImages    = true;
	drawDebugInfo = true;

	vidWarpBox.setup( 20, 220, camX*2, camY*2 );
	
	motionTracker.setup( camX, camY );

	mouseParticles.init( 100 );

	vector<string> presetFiles;

	// Setup the GUI contents
	gui.addTitle( "IMG PROCESSING", 30);
	gui.addSlider("Blur Amount",     motionTracker.CTblur,   1, 30);
	gui.addSlider("Threshold",       motionTracker.CTthresh, 1, 255);
	gui.addToggle("Flip Horizontal", motionTracker.CTflipHorizontal);
	gui.addToggle("Flip Vertical",   motionTracker.CTflipVertical);

	gui.addTitle( "OPTICAL FLOW", 30 );
	gui.addToggle( "Do Optical Flow", motionTracker.CTdoOpticalFlow );
	gui.addSlider( "OF Cols Step", motionTracker.OFcolsStep, 1, 15 );
	gui.addSlider( "OF Rows Step", motionTracker.OFrowsStep, 1, 15 );
	gui.addSlider( "threshold",    motionTracker.OFactivityThresh, 1, 20 );

	gui.addTitle( "OUTPUT", 30);
	gui.addToggle( "Draw Images", drawImages);

	gui.currentPage().setXMLName("motionTracker_gui_settings.xml");
	gui.currentPage().setName("Tracking Options");
	gui.currentPage().get
	gui.loadFromXML();
	
	
	// Initialise renderer
	// -----------------------------------------
	output.setup( motionTracker );
	output.setOutputArea( 40+camX*2, 220, camX*2, camY*2 );

	gui.addPage("Default_Renderer");
	gui.setPage("Default_Renderer");
	gui.currentPage().setXMLName("default_renderer_settings.xml");

	gui.addTitle(  "Rendering");
	gui.addToggle( "Fade To Black", output.fadeToBlack  );
	gui.addToggle( "Redraw Screen", output.redrawScreen );

	gui.addTitle ( "Particles");
	gui.addToggle( "Draw Particles", output.useParticles  );
	gui.addSlider( "Life min", output.pLifeMin,   1, 100  );
	gui.addSlider( "Life max", output.pLifeMax,  10, 300  );
	gui.addSlider( "Size min", output.pSizeMin,   1, 100  );
	gui.addSlider( "Size max", output.pSizeMax,   2, 300  );
	gui.addSlider( "Drag min", output.pDragMin, 0.7, 0.95 );
	gui.addSlider( "Drag max", output.pDragMax, 0.8, 1.0  );
	gui.addSlider( "Colour range",    output.pRandColRange, 1, 50 );
	gui.addSlider( "Colour interval", output.pRandColInterval, 1, 50 );

	gui.addTitle ( "Optical Flow");
	gui.addSlider( "Averaging area", output.opFlowAvgArea,    1, 15  );
	gui.addSlider( "Force scale",    output.opFlowForceScale, 0, 100 );

	gui.addTitle ( "Outlines");
	gui.addToggle( "Draw Outlines", output.usePaths );
	gui.addSlider( "Max Path Vertices", output.pathMaxVertices,  5, 150  );
	gui.addSlider( "Path Smoothing",    output.pathSmoothAmount, 0, 10  );

	gui.addTitle( "Colour" );
	gui.addSlider( "Saturation",  output.pSat, 30, 255 );
	gui.addSlider( "Brightness",  output.pBri, 30, 255 );
	gui.addSlider( "Cycle Speed", output.pHueCycleSpeed, 1, 100 );

	//gui.addTitle ( "Images" );
	//gui.addSlider( "Image Number",       output.currentImg, 0, 5      );
	//gui.addToggle( "Randomised Colours", output.colourFromImgRandomly );


	rendererLastChanged = ofGetElapsedTimeMillis();
	presetLastChanged   = ofGetElapsedTimeMillis();
	rendererTimeout = 5;
	presetTimeout = 2;

	
	//gui.addColorPicker("Outline col");
	//gui.addColorPicker("Motion  col");
	gui.setPage("Tracking Options");
	gui.setDefaultKeys(false);
	gui.setAutoSave(false);

	update();
}




//--------------------------------------------------------------
void testApp::update()
{
	// Perform camera tracking
	
	output.update();

	motionTracker.doTracking( vidWarpBox.dstPoints[3].x/2, vidWarpBox.dstPoints[3].y/2,
						      vidWarpBox.dstPoints[2].x/2, vidWarpBox.dstPoints[2].y/2,
						      vidWarpBox.dstPoints[1].x/2, vidWarpBox.dstPoints[1].y/2,
						      vidWarpBox.dstPoints[0].x/2, vidWarpBox.dstPoints[0].y/2);
	mouseParticles.update();


	// TODO
	/*
	int rendererRunningFor = (ofGetElapsedTimeMillis() - rendererLastChanged)/1000;
	if( rendererRunningFor > rendererTimeout ){
		int presetsToUse[4] = {1,3,4,5};
		changeOutputRenderer( presetsToUse[(int)ofRandom(0,4)] );
		loadRandomPreset();
	}
	int presetRunningFor   = (ofGetElapsedTimeMillis() - presetLastChanged)/1000;
	if( presetRunningFor > presetTimeout ){
		loadRandomPreset();
	}
	*/

}


//--------------------------------------------------------------
void testApp::draw()
{
	
	// Draw our report string
	if( drawDebugInfo ){
		char reportStr[1024];
		sprintf( reportStr, "FPS: %f \nOutline points : %i\nMotion Points : %i\nCurrent Output : %s", ofGetFrameRate(), motionTracker.getOutContourLength(), motionTracker.getMotContourLength(), output.fileName);
		ofSetHexColor(0xffffff);
		ofDrawBitmapString(reportStr, 10, 20);
	}
	

	// Draw processing stages if required
	if( drawImages )
	{
		motionTracker.drawProcessingImages();
		ofSetHexColor(0xffffff);
		vidWarpBox.draw();
	}

	output.draw();
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

		// TOGGLE AUTO CLEAR BACKGROUND
		case 'a':
			output.redrawScreen = !output.redrawScreen;
			break;
		
		// TOGGLE DEBUG IMAGES
		case 'i':
			drawImages = !drawImages;
			break;

		// TOGGLE OPTICAL FLOW
		case 'o':
			motionTracker.CTdoOpticalFlow = !motionTracker.CTdoOpticalFlow;
			break;
		

		// TOGGLE DEBUG INFO
		case 'd':
			drawDebugInfo = !drawDebugInfo;
			output.showDebugInfo = drawDebugInfo;
			break;

		// LEARN BACKGROUND
		case 'b':
			gui.setPage( "Tracking Options" );
			motionTracker.learnBackGround();
			gui.saveToXML();
			drawImages = false;
			break;
		
		// RESET WARPING BOX
		case 'r':
			for( int i=0; i<4; i++)
			{
				vidWarpBox.dstPoints[i].x = vidWarpBox.srcPoints[i].x;
				vidWarpBox.dstPoints[i].y = vidWarpBox.srcPoints[i].y;
			}
			break;
		
		

		// INCREASE THRESHOLD
		case '+':
			motionTracker.CTthresh ++;
			if ( motionTracker.CTthresh > 255)
			{
				motionTracker.CTthresh = 255;
			}
			break;
		
		// DECREASE THRESHOLD
		case '-':
			motionTracker.CTthresh --;
			if (motionTracker.CTthresh < 0)
			{
				motionTracker.CTthresh = 0;
			}
			break;
		
		// OUTPUT TO FIRST SCREEN FULLSCREEN
		case '1':
			output.setOutputArea( 0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
			break;

		// OUTPUT TO SECOND SCREEN FULLSCREEN
		case '2':
			output.setOutputArea( ofGetScreenWidth(), 0, ofGetScreenWidth(), ofGetScreenHeight() );
			break;

		// OUTPUT TO FIRST SCREEN SMALL
		case '3':
			output.setOutputArea( 20+camX*2, 220, camX*2, camY*2 );
			break;
	
		// LOAD RANDOM PRESET FROM XML
		case 'l':
			loadRandomPreset();
			break;

		
		// SAVE GUI SETTINGS AS PRESET TO XML
		case 's':{

			string newFilename  = "./presets/";
			newFilename        += gui.currentPage().name;
			newFilename        += " - ";
			newFilename        += getTimeString();
			newFilename        += ".xml";

			cout << "\nNEW NAME "  << newFilename;

			gui.currentPage().setXMLName( newFilename );
			gui.currentPage().saveToXML();
			break;
		 }

		// DISPLAY TRACKING SETTINGS
		case 't':
			gui.setPage("TrackingOptions");
			//motionTracker.showSettings();
			break;

		case ',':
			// PRESET MANAGER

		case '.':
			// PRESET MANAGER
			break;

		

		// TOGGLE FULLSCREEN
		case 'f':
			ofToggleFullscreen();
			break;
		
			

	}
}


void testApp::changeOutputRenderer( string fileName ){

	// Preset manager's jonb
	
	rendererLastChanged = ofGetElapsedTimeMillis();
	presetLastChanged   = ofGetElapsedTimeMillis();


	/*
	output.releaseResources();

	currOutput = newOutput;
	output[currOutput]->initialiseResources();
	motionTracker.doTracking( vidWarpBox.dstPoints[3].x/2, vidWarpBox.dstPoints[3].y/2,
						      vidWarpBox.dstPoints[2].x/2, vidWarpBox.dstPoints[2].y/2,
						      vidWarpBox.dstPoints[1].x/2, vidWarpBox.dstPoints[1].y/2,
						      vidWarpBox.dstPoints[0].x/2, vidWarpBox.dstPoints[0].y/2 );

	float prob = ofRandom(0,1);

	switch( currOutput ){

		case 0 : gui.setPage("Tracking_Options");
				 break;
		
		case 1 : gui.setPage("Bubble_Renderer");
				 if( prob > 0.95 ){
					 autoBg = false;
				 }
				 else{
					 autoBg = true;
				 }
				 presetTimeout   = 3.0;
				 rendererTimeout = 1.80;
				 break;

		case 2 : gui.setPage("Smoke_Renderer");
				 break;

		case 3 : gui.setPage("Path_Renderer");
			     if( prob > 0.6 ){
					 autoBg = false;
				 }
				 else{
					 autoBg = true;
				 }
				 presetTimeout   = 1.5;
				 rendererTimeout = 1.20;
				 break;

		case 4 : gui.setPage("Sand_Renderer");
			     if( prob > 0.9 ){
					 autoBg = false;
				 }
				 else{
					 autoBg = true;
				 }
				 presetTimeout = 3.0;
				 rendererTimeout = 1.80;
				 break;

		case 5 : gui.setPage("Subliminal_Renderer");
			     if( prob > 0.1 ){
					 autoBg = false;
				 }
				 else{
					 autoBg = true;
				 }
				 presetTimeout = 2.0;
				 rendererTimeout = 30.00;
				 break;
	}
	
    ofSetBackgroundAuto( autoBg );
	rendererLastChanged = ofGetElapsedTimeMillis();
	presetLastChanged   = ofGetElapsedTimeMillis();
	*/
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

//--------------------------------------------------------------
string testApp::getTimeString(){

	time_t curr;
	tm     local;
	time( &curr );
	local =*(localtime(&curr));

	ostringstream convert;   // stream used for the conversion
   
	convert << local.tm_year;
	convert << "_";
	convert << local.tm_mon;
	convert << "_";
	convert << local.tm_mday;
	convert << "_";
	convert << local.tm_hour;
	convert << "_";
	convert << local.tm_min;
	convert << "_";
	convert << local.tm_sec;
	
	return convert.str();
}


void testApp::loadRandomPreset(){

	dir.listDir("/presets/");
	dir.sort();

	vector<int> validPresets;
	validPresets.clear();
	size_t found;

	// Store the indexes of all files in /presets/ directory 
	// whose filename contains the name of our current output
	if( dir.size() ){
		for( int i=0; i<dir.size(); i++ ){
			found = dir.getName(i).find( gui.currentPage().name );
			if( found != string::npos ){
				validPresets.push_back(i);
			}
		}
		// Print all matching files
		cout << "\nFOUND " << validPresets.size() << " " << gui.currentPage().name << " PRESETS : ";
		for( int i=0; i<validPresets.size(); i++ ){
			cout << "\n " << dir.getName( validPresets.at(i) );
		}

		// If we have presets to choose from
		if( validPresets.size() != 0 ){
			
			// Select one at random
			string randomPreset = "./presets/" + dir.getName( validPresets.at( ofRandom( 0, validPresets.size()-1 ) ) );

			// Format it's filename appropriately
			found = randomPreset.find(".bak");
			if( found != string::npos ){
				string removeTrailing = randomPreset.substr(0,found);
				randomPreset = removeTrailing;
			}
			// And load the preset
			gui.currentPage().setXMLName( randomPreset );
			gui.currentPage().loadFromXML();

			presetLastChanged = ofGetElapsedTimeMillis();
		}

				
	}
}