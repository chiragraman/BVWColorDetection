/* Copyright (C) 2011 Chirag Raman

This file is part of BVW_Colour_Detection.

BVW_Colour_Detection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BVW_Colour_Detection is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BVW_Colour_Detection.  If not, see <http://www.gnu.org/licenses/>.
*/

// BVW_Colour_Detection.cpp : Defines the entry point for the console application.

#include "cv.h"
#include "stdafx.h"



#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"




IplImage* GetThresholdedImage(IplImage* img){
	
	//Convert to HSV space
	IplImage* imgHSV = cvCreateImage(cvGetSize(img),8,3);
	cvCvtColor(img, imgHSV, CV_BGR2HSV);

	//Create the Thresholded Image
	IplImage* imgThresholded = cvCreateImage(cvGetSize(img), 8, 1);

	//Thresholding
	cvInRangeS(imgHSV, cvScalar(20, 100, 100),cvScalar(30, 255, 255),imgThresholded);

	//Smoothing
	 cvSmooth( imgThresholded, imgThresholded, CV_GAUSSIAN, 9, 9);
	 
	//release the temporary HSV Image and return the Thresholded Image
	cvReleaseImage(&imgHSV);
	return imgThresholded;

}

double GetAreaOfROI(IplImage* img){


	//Store the contours as a linked list
		CvSeq* contours;
		CvMemStorage* storage = cvCreateMemStorage(0);

		cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
		double area;
		double totalArea = 0.0;

		while(contours){
		
			area = fabs(cvContourArea(contours,CV_WHOLE_SEQ));
			totalArea += area;

			//Print the total area
			std::cout<<"Total Area: "<<totalArea<<std::endl;
			contours = contours->h_next;
		}

		return totalArea;

}

int sendToClient(int damage){

	return 0; 
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	//----------------------------------------------------
	//Initialize variables

	int iResult;
	int iSendResult;
	WSADATA wsaData;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	
	int recvbuflen = DEFAULT_BUFLEN;
	char *sendbuf = "Client: sending data test";
	char recvbuf[DEFAULT_BUFLEN] = "";
	//----------------------------------------------------


	//----------------------------------------------------
	// Initializing Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	    printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//----------------------------------------------------
	//Creating a Socket
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;		
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
    return 1;
	}

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	//Checking for errors

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
    return 1;
	}
	
	//-----------------------------------------------------

	//-----------------------------------------------------
	//Binding the socket
	// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	 freeaddrinfo(result);
	 //----------------------------------------------------

	 //----------------------------------------------------
	 //Listening on the socket
	 iResult = listen(ListenSocket, SOMAXCONN);
	 if ( iResult == SOCKET_ERROR ) {
	    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	 }
	//-----------------------------------------------------

	//-----------------------------------------------------
	// Accept a client socket
	wprintf(L"Waiting for client to connect...\n");

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	} else
        wprintf(L"Client connected.\n");

	// No longer need server socket
    closesocket(ListenSocket);
	//-----------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------------

	//OpenCV Stuff
	//Initialize capturing live feed from the camera
	CvCapture* capture = 0;
	capture = cvCaptureFromCAM(0);

	//Error Handling
	if(!capture){

		std::cout<<"Could not initialize camera feed capture...My honour has been compromised.. I have to kill myself"<<std::endl;
		return -1;

	}

	//Windows for display
	cvNamedWindow("Camera Feed");
	cvNamedWindow("Thresholded Image");
	
	//Infinite Loop for capturing frames from the camera feed
	while(true){
		
		//Capture the frame fromt the camera feed
		IplImage* frame = 0;
		frame = cvQueryFrame(capture);

		//If the frame capture fails, quit
		if(!frame)
			break;

		//The colour thresholded image
		IplImage* imgColourThresh = GetThresholdedImage(frame);

		//Display the images
		cvShowImage("Thresholded Image",imgColourThresh);
		cvShowImage("Camera Feed",frame);

		//After displaying the image, pause and check if a key is pressed
		int c = cvWaitKey(5);
		if(c!=-1){

			//If pressed, break out of the loop
			break;
		}

		GetAreaOfROI(imgColourThresh);

		cvReleaseImage(&imgColourThresh);
		
	}

	//Done Using the Camera
	cvReleaseCapture(&capture);
	return 0;
}

