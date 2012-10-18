#include "motionTracking.h"


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


void motionTracking::setup( int camX, int camY )
{
		imgX = camX;
		imgY = camY;
		
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
		CTflipVertical   = false;

		OFrowsStep       = 5;
		OFcolsStep       = 5;
		OFactivityThresh = 5;

		colorOutContours = ofColor( 255,   0,   0 );
		colorMotContours = ofColor(   0, 255,   0 );
		colorOpFlow      = ofColor(   0,   0, 255 );
		colorBg          = ofColor( 100, 100, 100 );
}


void motionTracking::drawProcessingImages()
{
	// draw the various image processing stages
	int halfX = imgX/2;
	int halfY = imgY/2;

	unwarpedGrayImage.draw (  20, 220, imgX*2, imgY*2 );

	grayBg.draw            (  20,  60,  halfX, halfY    );
	grayImage.draw         ( 200,  60,  halfX, halfY    );
	grayThresh.draw        ( 380,  60,  halfX, halfY    );
	grayThreshDiff.draw    ( 560,  60,  halfX, halfY    );

	// And label them
	ofDrawBitmapString("background",   50, 70+halfY);
	ofDrawBitmapString("current",     230, 70+halfY);
	ofDrawBitmapString("subtraction", 410, 70+halfY);
	ofDrawBitmapString("frameDiff",   590, 70+halfY);
}


void motionTracking::drawContours( int x, int y, int w, int h )
{
	// Draw a background to display our contours
	ofFill();
	ofSetColor( colorBg.r, colorBg.g, colorBg.b, colorBg.a ); 
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


void motionTracking::drawOpticalFlow( int x, int y, int w )
{
	if( CTdoOpticalFlow )
	{
		glPushMatrix();
			glTranslatef( x, y, 0);
			//opticalFlow.draw( imgX*2, imgY*2 );
		glPopMatrix();
	}

}


void motionTracking::doTracking( int x1, int y1, 
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
		
		grayImage.mirror( CTflipVertical, CTflipHorizontal);

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
			opticalFlow.calc( grayImagePrev, grayImage, 1 );
		}
		grayImagePrev = grayImage;


		// For the motion differenced image, look for blobs of 20+ pixels
		motionContours.findContours( grayThreshDiff, 20, (imgX*imgY), 10, false );	// find holes
		// For the whole outlines, only track blobs of 200+ pixels
		outlineContours.findContours( grayThresh, 200, (imgX*imgY), 10, false );	// find holes

	}
}


int motionTracking::getOutContourLength()
{
	int nPoints = 0;	
	
	for( int i=0; i < outlineContours.nBlobs; i++ )
	{
		nPoints += outlineContours.blobs[i].nPts;
	}

	return nPoints;
}


int motionTracking::getMotContourLength()
{
	int nPoints = 0;	
	
	for( int i=0; i< motionContours.nBlobs; i++ )
	{
		nPoints += motionContours.blobs[i].nPts;
	}

	return nPoints;
}


void motionTracking::learnBackGround()
{
	learnBg = true;
}


void motionTracking::showSettings(){
	vidGrabber.videoSettings();
}


void motionTracking::setColorOutContours( ofColor newColor )
{
	colorOutContours = newColor;
}


void motionTracking::setColorMotContour( ofColor newColor )
{
	colorMotContours = newColor;
}


void motionTracking::setColorOpFlow( ofColor newColor )
{
	colorOpFlow = newColor;
}


void motionTracking::setColorBg( ofColor newColor )
{
	colorBg = newColor;
}


int motionTracking::getImgX(){
	return imgX;
}


int motionTracking::getImgY(){
	return imgY;
}
