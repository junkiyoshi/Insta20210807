#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	int span = 30;
	for (int x = -300; x <= 300; x += span) {

		for (int y = -300; y <= 300; y += span) {

			this->location_list.push_back(glm::vec3(x, y, 0));
			this->param_list.push_back(0);
			this->param_color_list.push_back(ofColor());
			this->word_index_list.push_back(0);
		}
	}

	for (auto& location : this->location_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				route_info.push_back(index);
			}
		}

		this->route_info_list.push_back(route_info);
	}

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	vector<ofColor> base_color_list;
	for (auto hex : hex_list) {

		color.setHex(hex);
		base_color_list.push_back(color);
	}

	for (int i = 0; i < 20; i++) {

		this->index_list.push_back((int)ofRandom(this->location_list.size()));
		this->color_list.push_back(base_color_list[(int)ofRandom(base_color_list.size())]);
	}

	this->font_size = 23;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->words = {

		u8"あ", u8"い", u8"う", u8"え", u8"お",
		u8"か", u8"き", u8"く", u8"け", u8"こ",
		u8"さ", u8"し", u8"す", u8"せ", u8"そ",
		u8"た", u8"ち", u8"つ", u8"て", u8"と",
		u8"な", u8"に", u8"ぬ", u8"ね", u8"の",
		u8"は", u8"ひ", u8"ふ", u8"へ", u8"ほ",
		u8"ま", u8"み", u8"む", u8"め", u8"も",
		u8"や", u8"ゆ", u8"よ",
		u8"ら", u8"り", u8"る", u8"れ", u8"ろ",
		u8"わ", u8"を", u8"ん",
	};
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 3 != 0) { 
	
		for (int i = 0; i < this->index_list.size(); i++) {

			this->word_index_list[this->index_list[i]] = (int)ofRandom(this->words.size());
		}
		return;
	}

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->param_list[next_index] <= 0) {

				this->param_list[next_index] = 45;
				this->param_color_list[next_index] = this->color_list[i];
				this->index_list[i] = next_index;
				this->word_index_list[next_index] = (int)ofRandom(this->words.size());
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->param_list[i] > 0) {

			ofFill();
			ofSetColor(ofMap(this->param_list[i], 0, 45, 239, 39));		
			this->font.drawString(this->words[this->word_index_list[i]], this->location_list[i].x - 15, this->location_list[i].y - 15);
		}

		if (this->param_list[i] > 0) { this->param_list[i] -= 1; }

		ofNoFill();
		ofSetColor(39);
		ofDrawRectangle(this->location_list[i] - glm::vec2(15, 15), 30, 30);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}