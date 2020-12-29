#pragma once
#include "ofMain.h"
#include "opencv2/opencv.hpp"
#include "ofxBox2d.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	cv::VideoCapture cap;
	cv::Size cap_size;
	int number_of_frames;
	vector<cv::Mat> frame_list;

	ofImage image;
	cv::Mat frame;
	ofMesh mesh;

	vector<glm::vec3> draw_location_list;
	vector<int> draw_index_list;

	ofxBox2d box2d;
	vector<shared_ptr<ofxBox2dRect>> box2d_rect_list;
	vector<int> box2d_frame_index_list;
	vector<float> box2d_life_list;
};