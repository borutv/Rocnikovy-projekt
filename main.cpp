#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

Point detectFace( Mat frame, Point priorCenter );

CascadeClassifier face_cascade, eyes_cascade, eyes_cascade_no_glasses;

String display_window = "Display";
String face_window = "Face View";

int default_z = 0;
bool isInit = false;

int main() {
    
    VideoCapture cap(0); // zachytime obraz z hlavnej kamery
    Mat frame;
    Point pCenter(0, 0);
    
    face_cascade.load("/Users/borut/Desktop/OpenCVTest/OpenCVTest/haarcascade_frontalface_alt.xml"); // tvar
    eyes_cascade_no_glasses.load("/Users/borut/Desktop/OpenCVTest/OpenCVTest/haarcascade_eye.xml"); // oci
    eyes_cascade.load("/Users/borut/Desktop/OpenCVTest/OpenCVTest/haarcascade_eye_tree_eyeglasses.xml"); // oci aj okuliare
    
    namedWindow(face_window,
                CV_WINDOW_AUTOSIZE |
                CV_WINDOW_FREERATIO |
                CV_GUI_EXPANDED);
    
    while(cap.read(frame)) {
        
        pCenter = detectFace(frame, pCenter);
        
        if(waitKey(30) >= 0)
            break;
    }
    return 0;
}

Mat outputFrame(Mat frame, Point center, int w, int h) {
    
    int x = (center.x - w/2);
    int y = (center.y - 3*h/5);
    
    if(x + w > frame.size().width - 2 || x < 0 ||
       y + h > frame.size().height - 2 || y < 0 &&
       frame.size().width > 16 &&
       frame.size().height > 16)
        return frame(Rect(5, 5, 10, 10));
    
    // vrati obldznik tvare
    return frame(Rect(x, y, w, h));
}

// najde tvar nazaklade oci
Point faceFromEyes(Point priorCenter, Mat face) {
    
    std::vector<Rect> eyes;
    int avg_x = 0;
    int avg_y = 0;
    
    // pokusim sa najst oci ak nevidim tvar
    eyes_cascade.detectMultiScale(face, eyes, 1.1, 2,
                                  0 |CASCADE_SCALE_IMAGE, Size(30, 30));
    for(size_t j = 0; j < eyes.size(); j++) {
        
        // hladam stred oci
        Point eye_center(priorCenter.x + eyes[j].x + eyes[j].width/2,
                         priorCenter.y + eyes[j].y + eyes[j].height/2);
        
        // priemerny stred oci
        avg_x += eye_center.x;
        avg_y += eye_center.y;
    }
    
    // pouzivam priemernu polohu oci
    if(eyes.size() > 0) {
        priorCenter.x = avg_x / eyes.size();
        priorCenter.y = avg_y / eyes.size();
    }
    
    return priorCenter;
}

int roundUp(int numToRound, int multiple) {
    
    if (multiple == 0)
        return numToRound;
    
    int remainder = abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;
    if (numToRound < 0)
        return -(abs(numToRound) - remainder);
    return numToRound + multiple - remainder;
}

// Detekujem tvar a zobrazim
Point detectFace(Mat frame, Point priorCenter) {
    
    std::vector<Rect> faces;
    Mat frame_gray, frame_lab, output, temp;
    int h = frame.size().height - 1;
    int w = frame.size().width - 1;
    int minNeighbors = 2;
    bool faceNotFound = false;
    
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    
    // detekuj tvar s open source kaskadami
    face_cascade.detectMultiScale(frame_gray, faces,
                                  1.1, minNeighbors,
                                  0|CASCADE_SCALE_IMAGE, Size(30, 30));
    
    for(size_t i = 0; i < faces.size(); i++) {
        
        // najdem stred tvare
        Point center(faces[i].x + faces[i].width/2,
                     faces[i].y + faces[i].height/2);
        
        h = roundUp(faces[i].height, frame.size().height / 4);
        w = 3 * h / 5;
        
        if(priorCenter.x == 0) {
            priorCenter = center;
            temp = outputFrame(frame, center, w, h);
            break;
        }
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        
        // v kazdej tvary najdem oci
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
        for( size_t j = 0; j < eyes.size(); j++ )
        {
            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            if(!isInit){
                isInit = true;
                default_z = radius;
            }
            if(isInit && abs(default_z-radius > 7)){
                default_z = radius;
            }
            cout<<"eye" << j+1 <<" x:"<< center.x << endl;
            cout<<"eye" << j+1 <<" y:" << center.y << endl;
            cout<<"eye" << j+1 <<" z:" << default_z -(radius-default_z) <<endl;
            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        // overim ci to je rovnaky uzivatel
        if(abs(center.x - priorCenter.x) < frame.size().width / 6 &&
           abs(center.y - priorCenter.y) < frame.size().height / 6) {
            
            // overim ci sa uzivatel dostatocne pohol aby som aktualizoval data
            if(abs(center.x - priorCenter.x) < 7 &&
               abs(center.y - priorCenter.y) < 7){
                center = priorCenter;
            }
            
            
            center.x = (center.x + 2*priorCenter.x) / 3;
            center.y = (center.y + 2*priorCenter.y) / 3;
            priorCenter = center;
            
            // ukaz okno iba s tvarou
            temp = outputFrame(frame, center, w, h);
            
            break;
            
        } else {
            faceNotFound = true;
        }
    }
    
    if(faceNotFound) {
        
        // najdi tvar nazaklade oci
        Rect r(priorCenter.x, priorCenter.y, w, h);
        if(priorCenter.x + w > frame_gray.size().width - 2 &&
           priorCenter.y + h > frame_gray.size().height - 2){
            
            priorCenter = faceFromEyes(priorCenter, frame_gray(r));
            
            
            temp = outputFrame(frame, priorCenter, w, h);
        }
    }
    
    
    if(temp.size().width > 2)
        output = temp;
    else
        output = frame;
    
    imshow(face_window, output);
    
    if(output.size().width > 2)
        // vykresli elipsu okolo tvare
        ellipse(frame, priorCenter, Size(w/2, h/2),
                0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);
    

    imshow( display_window, frame );
    
    return priorCenter;
}
