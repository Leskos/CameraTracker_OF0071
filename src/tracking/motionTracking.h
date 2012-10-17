#ifndef CAMERA_TRACKING
#define CAMERA_TRACKING

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvOpticalFlowLK.h"


class motionTracking{

	public :

		// 
		int  CTthresh;
		int	 CTblur;
		bool CTflipHorizontal;
		bool CTflipVertical;
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

		void setColorOutContours( ofColor newColor );
		void setColorMotContour(  ofColor newColor );
		void setColorOpFlow(      ofColor newColor );
		void setColorBg(          ofColor newColor );

		void learnBackGround();

		void showSettings();

		int getOutContourLength();
		int getMotContourLength();

		int getImgX();

		int getImgY();


	private : 
	
		int imgX;
		int imgY;

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

		// OUTPUT COLOURS 
		ofColor colorOutContours;
		ofColor colorMotContours;
		ofColor colorOpFlow;
		ofColor colorBg;
};

#endif
