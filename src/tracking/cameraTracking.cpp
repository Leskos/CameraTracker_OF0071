#include "cameraTracking.h"


	/// ###=> SEPERATE THIS OUT INTO UTILS <=###
//--------------------------------------------------------------
void getQuadSubImage(   unsigned char * inputData, unsigned char * outputData,
                                int inW, int inH, int outW, int outH,
                                int x1, int y1, int x2, int y2,
                                int x3, int y3, int x4, int y4, int bpp) 
{
    for(int x=0;x<outW;x++) 
	{
        for(int y=0;y<outH;y++) 
		{
            float xlrp = x/(float)outW;
            float ylrp = y/(float)outH;
            int xinput = (x1*(1-xlrp)+x2*xlrp)*(1-ylrp) + (x4*(1-xlrp)+x3*xlrp)*ylrp;
            int yinput = (y1*(1-ylrp)+y4*ylrp)*(1-xlrp) + (y2*(1-ylrp)+y3*ylrp)*xlrp;
            int inIndex = (xinput + yinput*inW)*bpp;
            int outIndex = (x+y*outW)*bpp;
            memcpy((void*)(outputData+outIndex),(void*)(inputData+inIndex),sizeof(unsigned char)*bpp);
        }
    }
}

void cameraTracking::setup( int camX, int camY )
{
		imgX = camX;
		imgY = camY;

		halfX = imgX/2;
		halfY = imgY/2;

		processX = imgX;
		processY = imgY;

		// Initialise the vidGrabber
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(imgX,imgY);

		// Not sure whether this has much effect
		/// ###=> REMOVE IF UNNECESSARY <=###
		//vidGrabber.setDesiredFrameRate(20);
	
		// Allocate memory for all the images we will need
	    colorImg.allocate         ( imgX, imgY );
		unwarpedGrayImage.allocate( imgX, imgY );
		grayBg.allocate             ( processX, processY );
		grayImage.allocate          ( processX, processY );
		grayImagePrev.allocate      ( processX, processY );
		grayThresh.allocate         ( processX, processY );
		grayThreshPrev.allocate     ( processX, processY );
		grayThreshDiff.allocate     ( processX, processY );
		grayThreshDiffDiff.allocate ( processX, processY );
	
		opticalFlow.allocate( processX, processY );
		
		// Set the default threshold to 40
		CTthresh         = 10;
		CTblur           = 15;
		CTflipHorizontal = false;

		OFrowsStep       = 5;
		OFcolsStep       = 5;
		OFactivityThresh = 5;

		outContourR = 255;
		outContourG = 0;
		outContourB = 0;

		motContourR = 0;
		motContourG = 255;
		motContourB = 0;

		int ofR = 0;
		int ofG = 0;
		int ofB = 255;

		int bgR = 200;
		int bgG = 200;
		int bgB = 200;
}



void cameraTracking::drawProcessingImages()
{
	// draw the various image processing stages
	unwarpedGrayImage.draw (  20, 220, imgX*2, imgY*2 );
	grayBg.draw            (  20, 60, halfX, halfY    );
	grayImage.draw         ( 200, 60, halfX, halfY    );
	grayThresh.draw        ( 380, 60, halfX, halfY    );
	grayThreshDiff.draw    ( 560, 60, halfX, halfY    );

	// And label them
	ofDrawBitmapString("background",   50, 70+halfY);
	ofDrawBitmapString("current",     230, 70+halfY);
	ofDrawBitmapString("subtraction", 410, 70+halfY);
	ofDrawBitmapString("frameDiff",   590, 70+halfY);
}


void cameraTracking::drawContours( int x, int y, int w, int h )
{
	// Draw a background to display our contours
	ofNoFill();
	ofSetColor( bgR, bgG, bgB );
	ofRect( x, y, w, h );

	// Draw the outline contours in blue, and the frameDiff ones in purple
	outlineContours.draw( x, y, w, h );
	motionContours.draw(  x, y, w, h );
	
	// If we want access to individual blobs, use this :
	/*
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw( 20, 220 );
    }
	*/
}


void cameraTracking::drawOpticalFlow( int x, int y, int w )
{
	if( CTdoOpticalFlow )
	{
		glPushMatrix();
			glTranslatef( x, y, 0);
			//opticalFlow.drawScaled( ( w/processX ) );
		glPopMatrix();
	}

	if( true ){
		/*
		IplImage * velX = opticalFlow.getVelX();
		IplImage * velY = opticalFlow.getVelY();
		
		// Create an image for the output
		IplImage* out = cvCreateImage( cvGetSize(velX), IPL_DEPTH_8U, 3 );

		// Perform a Gaussian blur
		cvSmooth( velX, out, CV_GAUSSIAN, 11, 11 );

		float scale = w/processX;

		ofEnableAlphaBlending();
        ofSetColor( 200, 200, 0 );
        ofNoFill();

        int x, y, dx, dy;
		float xScaled = 0;
		float yScaled = 0;


        for ( y = 0; y < imgY; y+=OFrowsStep ){

			xScaled = 0;

                for ( x = 0; x < imgX; x+=OFcolsStep ){

                        dx = (int)cvGetReal2D( velX, y, x );
                        dy = (int)cvGetReal2D( velY, y, x );
	
						if( dx > CTthresh || dy > CTthresh ){
							ofLine( xScaled , yScaled , xScaled+dx, yScaled+dy);
						}
                        
						xScaled = (xScaled + scale*OFcolsStep);
                }
			yScaled = ( yScaled + scale*OFrowsStep );
        }
        ofDisableAlphaBlending();
		*/
	}
}


void cameraTracking::doTracking( int x1, int y1, 
								 int x2, int y2,
								 int x3, int y3,
								 int x4, int y4 )
{
	bool bNewFrame = false;

	// Read in frame from camera, and see if it is new
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();

	// If it is indeed a new frame
	if (bNewFrame)
	{
		// Read the pixels into colorImg
		colorImg.setFromPixels(vidGrabber.getPixels(), imgX,imgY);

	    // Then copy then to grayImage...
		unwarpedGrayImage = colorImg;
		grayImage = unwarpedGrayImage;

		// ...and warp them
		getQuadSubImage( unwarpedGrayImage.getPixels(), grayImage.getPixels(), 
			             imgX, imgY, imgX, imgY, 
						 x1, y1, x2, y2, x3, y3, x4, y4,
						 1 );
		
		grayImage.mirror(false, CTflipHorizontal);

		// If necessary, use this frame as the background 
		if ( learnBg )
		{
			grayBg = grayImage;		
			learnBg = false;
		}

		// in grayThresh, we store the differenced, blurred, then thresholded image
		grayThresh.absDiff(grayBg, grayImage);
		grayThresh.blurGaussian(CTblur);
		grayThresh.threshold(CTthresh);

		// in grayThreshDiff, we store the difference of grayThresh from last frame
		grayThreshDiff.absDiff( grayThresh, grayThreshPrev );
		grayThreshPrev = grayThresh;

		if( CTdoOpticalFlow )
		{
			opticalFlow.setCalcStep( OFrowsStep, OFcolsStep );
			opticalFlow.calc( grayImagePrev, grayImage, 5 );
		}
		grayImagePrev = grayImage;


		// For the motion differenced image, look for blobs of 20+ pixels
		motionContours.findContours( grayThreshDiff, 20, (imgX*imgY), 10, false );	// find holes
		// For the whole outlines, only track blobs of 200+ pixels
		outlineContours.findContours( grayThresh, 200, (imgX*imgY), 10, false );	// find holes

	}
}


int cameraTracking::getNumPoints()
{
	int nPoints = 0;	
	
	for( int i=0; i < outlineContours.nBlobs; i++ )
	{
		nPoints += outlineContours.blobs[i].nPts;
	}

	for( int i=0; i< motionContours.nBlobs; i++ )
	{
		nPoints += motionContours.blobs[i].nPts;
	}

	return nPoints;
}


void cameraTracking::learnBackGround()
{
	learnBg = true;
}

void cameraTracking::showSettings(){
	vidGrabber.videoSettings();
}


void cameraTracking::setOutContourColour( int r, int g, int b )
{
	outContourR = r;
	outContourG = g;
	outContourB = b;
}


void cameraTracking::setMotContourColour( int r, int g, int b )
{
	motContourR = r;
	motContourG = g;
	motContourB = b;
}

void cameraTracking::setOfColour( int r, int g, int b )
{
	//opticalFlow.setColour( r, g, b );
}

void cameraTracking::setBgColour( int r, int g, int b )
{
	bgR = r;
	bgG = g;
	bgB = b;
}

int cameraTracking::getImgX(){
	return imgX;
}

int cameraTracking::getImgY(){
	return imgY;
}