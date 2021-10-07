#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "TransformObject.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
	BaseObject();
	glm::vec3 trans, scale;
	float	rotation;
	bool	bSelected;
	ofVec3f head;
	ofVec3f left;
	glm::mat4 getMatrix() {
		glm::mat4 tran = glm::translate(glm::mat4(1.0), trans);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);

		glm::mat4 T = tran * rot * scale;
		return T;
	}
	void setPosition(ofVec3f);

	

};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	
	float age();
	void setImage(ofImage);
	
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	//void update();
	//ofPoint pos;
	bool haveImage;
	float width, height;
	
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	vector<Sprite> sprites;
	
};














//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	Emitter() {
		if (!sys)
			sys = new SpriteSystem();
	}
	void draw();
	void start();
	void stop();
	void setupSpeed(float);
	void setLifespan(float);
	void setVelocity(ofVec3f);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
	void integrate();
	SpriteSystem *sys = NULL;
	float rate;
	ofVec3f velocity = ofVec3f(0, 0, 0);
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height, childWidth,childHeight;
	void move();
	int speed;
	void sine();

	ofVec3f acceleration = ofVec3f(0, 0, 0);
	ofVec3f force = ofVec3f(0, 0, 0);
	ofVec3f vel = ofVec3f(0, 0, 0);
	float mass;
	float damping = .99;
	float angularForce = 0;
	float angularVelocity = 0.0;
	float angularAcceleration = 0.0;


};


class ofApp : public ofBaseApp {

public:
	


	void setup();
	void update();
	void draw();
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool firing = false;
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	string game_state;
	//	vector<Emitter *> emitters;
	//	int numEmitters;
	void checkCollision();
	Emitter  *turret ;
	Emitter *enemy;
	Emitter *enemyT;

	ParticleEmitter explosion;
	TurbulenceForce *turbForce;
	GravityForce *gravityForce;
	ImpulseRadialForce *radialForce;


	Sprite T;
	Sprite E;
	//Emitter turret;
	ofImage background;
	ofImage turretImage;
	ofImage start_screen;
	ofImage end_screen;
	ofImage bulletImage;
	ofImage targetImage;
	ofImage invaderImage;
	ofSoundPlayer bullet;
	ofSoundPlayer explode;
	ofVec3f mouse_last;
	bool imageLoaded;
	int score;
	ofSoundPlayer bgm;
	ofSoundPlayer gg;
	ofxFloatSlider rate;
	ofxFloatSlider leftEnemyFiringSpeed;
	ofxFloatSlider rightEnemyFiringSpeed;
	ofxFloatSlider life;
	ofxFloatSlider lifespan;
	ofxFloatSlider speed;
	ofxVec3Slider velocity;
	ofxFloatSlider leftEnemyRate;
	ofxFloatSlider leftEnemyLife;
	ofxVec3Slider leftEnemyVelocity;

	ofxFloatSlider rightEnemyRate;
	ofxFloatSlider rightEnemyLife;
	ofxVec3Slider rightEnemyVelocity;
	ofxToggle parabola;
	ofxToggle sine;
	ofxToggle circle;
	int prevKey = -9999999999;
	ofxLabel screenSize;
	void ofApp::animateTurret();
	string playerState = "idle";
	ofxPanel gui;
	ofSoundPlayer w;
	ofTrueTypeFont font;
	bool bHide = true;
	bool gameOver = false;
	







};
