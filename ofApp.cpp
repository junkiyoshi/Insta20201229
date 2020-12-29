#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofSetLineWidth(3);

	this->cap.open("D:\\MP4\\Pexels Videos 2441067.mp4");
	this->cap_size = cv::Size(128, 72);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	int skip_frame = 24 * 120;
	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT) - skip_frame;
	this->cap.set(cv::CAP_PROP_POS_FRAMES, skip_frame);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}

	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5,  this->cap_size.height * -0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * 0.5,  this->cap_size.height * 0.5, 0));
	this->mesh.addVertex(glm::vec3(this->cap_size.width * -0.5, this->cap_size.height * 0.5, 0));
	
	this->mesh.addTexCoord(glm::vec3(0, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, 0, 0));
	this->mesh.addTexCoord(glm::vec3(this->cap_size.width, this->cap_size.height, 0));
	this->mesh.addTexCoord(glm::vec3(0, this->cap_size.height, 0));

	this->mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
	this->mesh.addIndex(0); mesh.addIndex(2); mesh.addIndex(3);

	for (int x = 40 + this->cap_size.width * 0.5; x <= ofGetWidth(); x += this->cap_size.width) {

		for (int y = 40 + this->cap_size.height * 0.5; y <= ofGetHeight() * 0.5; y += this->cap_size.height) {

			this->draw_location_list.push_back(glm::vec3(x, y, 0));
			this->draw_index_list.push_back(ofRandom(this->number_of_frames));
		}
	}

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->draw_location_list.size(); i++) {

		int n = int(ofMap(ofNoise(this->draw_location_list[i].x * 0.5625 * 0.005, this->draw_location_list[i].y * 0.005, ofGetFrameNum() * 0.0005), 0, 1, 0, this->number_of_frames * 3)) % this->number_of_frames;
		this->draw_index_list[i] = n;
	}

	for (int i = this->box2d_rect_list.size() - 1; i > -1; i--) {

		this->box2d_life_list[i] -= 3;
		if (this->box2d_life_list[i] < 0) {

			this->box2d_rect_list.erase(this->box2d_rect_list.begin() + i);
			this->box2d_frame_index_list.erase(this->box2d_frame_index_list.begin() + i);
			this->box2d_life_list.erase(this->box2d_life_list.begin() + i);
		}
	}

	if (ofGetFrameNum() % 12 == 0) {

		int location_index = ofRandom(this->draw_location_list.size());
		int frame_index = this->draw_index_list[location_index];

		auto box2d_rect = make_shared<ofxBox2dRect>();
		box2d_rect->setPhysics(0.5, 0.21, 0.4);
		box2d_rect->setup(this->box2d.getWorld(), this->draw_location_list[location_index].x, this->draw_location_list[location_index].y, this->cap_size.width, this->cap_size.height);

		this->box2d_rect_list.push_back(box2d_rect);
		this->box2d_frame_index_list.push_back(frame_index);
		this->box2d_life_list.push_back(512);
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->draw_location_list.size(); i++) {

		this->frame_list[this->draw_index_list[i]].copyTo(this->frame);
		this->image.update();

		ofPushMatrix();
		ofTranslate(this->draw_location_list[i]);

		ofSetColor(255);
		ofFill();
		this->image.bind();
		mesh.draw();
		this->image.unbind();

		ofSetColor(39);
		ofNoFill();
		ofDrawRectangle(this->cap_size.width * -0.5, this->cap_size.height * -0.5, this->cap_size.width, this->cap_size.height);

		ofPopMatrix();
	}

	for (int i = 0; i < this->box2d_rect_list.size(); i++) {

		this->frame_list[this->box2d_frame_index_list[i]].copyTo(this->frame);
		this->image.update();

		ofPushMatrix();
		ofTranslate(this->box2d_rect_list[i]->getPosition());
		ofRotate(this->box2d_rect_list[i]->getRotation());

		auto alpha = this->box2d_life_list[i] > 255 ? 255 : this->box2d_life_list[i];

		ofSetColor(255, alpha);
		ofFill();
		this->image.bind();
		mesh.draw();
		this->image.unbind();

		ofSetColor(39);
		ofNoFill();
		ofDrawRectangle(this->cap_size.width * -0.5, this->cap_size.height * -0.5, this->cap_size.width, this->cap_size.height);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}