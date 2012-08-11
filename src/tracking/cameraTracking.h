#ifndef CAMERA_TRACKING
#define CAMERA_TRACKING

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvOpticalFlowLK.h"


class cameraTracking{

	public :

		int  CTthresh;
		int	 CTblur;
		bool CTflipHorizontal;
		bool CTdoOpticalFlow;

		int   OFrowsStep;
		int   OFcolsStep;
		float OFactivityThresh;
	
		ofxCvContourFinder 	outlineContours;
		ofxCvContourFinder 	motionContours;

	    ofxCvOpticalFlowLK  opticalFlow;
	
		
		void setup( int camX, int camY);

		void doTracking( int x1, int y1, 
					     int x2, int y2,
						 int x3, int y3,
						 int x4, int y4 );
	
		void drawProcessingImages();
		void drawContours( int x, int y, int w, int h );
		void drawOpticalFlow( int x, int y, int w );

		void setOutContourColour( int r, int g, int b );
		void setMotContourColour( int r, int g, int b );
		void setOfColour(         int r, int g, int b );
		void setBgColour(         int r, int g, int b );

		void learnBackGround();

		void showSettings();

		int getNumPoints();

		int getImgX();

		int getImgY();


	private : 
	
		int imgX;
		int imgY;
		int halfX;
		int halfY;
		int processX; 
		int processY;

		ofVideoGrabber 		vidGrabber;

		// Seperate these into ImageProcessing class
		ofxCvColorImage		    colorImg;
		ofxCvGrayscaleImage     unwarpedGrayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayImagePrev;
		ofxCvGrayscaleImage 	grayThresh;
		ofxCvGrayscaleImage 	grayThreshPrev;
		ofxCvGrayscaleImage 	grayThreshDiff;
		ofxCvGrayscaleImage 	grayThreshDiffDiff;
	
		
		bool				learnBg;

		// OUTPUT COLOURS ### COULD BE STORED MORE EFFICIENTLY ###
		
		int outContourR;
		int outContourG;
		int outContourB;

		int motContourR;
		int motContourG;
		int motContourB;

		int ofR;
		int ofG;
		int ofB;

		int bgR;
		int bgG;
		int bgB;
};

#endif
