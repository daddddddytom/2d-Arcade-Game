#include "ofApp.h"
//----------------------------------------------------------------------------------

// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial 
// game development, but it is useful for testing.  The "h" key will hide the GUI
// 
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU

BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rotation = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 60;
	height = 80;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

/*void Sprite::update() {
	velocity = velocity;
}*/

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}



//  Render the sprite
//
void Sprite::draw() {

	//ofSetColor(255, 255, 255, 255);
	ofPushMatrix();
	
	// draw image centered and add in translation amount
	//
	if (haveImage) {
		ofMultMatrix(getMatrix());
		image.draw(-width / 2.0 , -height / 2.0 );
		ofPopMatrix();
		
	}
	else {
		// in case no image is supplied, draw something.
		// 
		//ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
	
}



//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}





//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {
	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity.getNormalized()*100 / ofGetFrameRate();
		
		
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 10000;    // milliseconds
	started = false;
	
	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(0, 0, 0);
	drawable = true;
	width = 150;
	height = 150;
	childWidth = 10;
	childHeight = 10;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	
	
	ofPushMatrix();
	

	if (drawable) {

		if (haveImage) {
			ofMultMatrix(getMatrix());
			image.draw(-image.getWidth() / 2.0 , -image.getHeight() / 2.0 );
			


		}
	}

	ofPopMatrix();
	
	// draw sprite system
	//
	sys->draw();
	
}

void Emitter::setupSpeed(float _speed) {
	speed = _speed;

}


//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;
	float time = ofGetElapsedTimeMillis();
	/*if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		
		sprite.velocity = velocity;
		
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		sprite.birthtime = time;
		sprite.rotation = rotation;
		sprite.height = childHeight;
		sprite.width = childWidth;
		
		sys->add(sprite);
		lastSpawned = time;
	}*/
	
	sys->update();
	
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::integrate() {

	float dt = 1.0 / 60.0;
	trans += vel * dt;
	vel += acceleration * dt;
	vel *= damping;
	acceleration = (1 / mass) * force;
	angularVelocity += angularAcceleration * dt;
	angularVelocity *= damping;
	rotation += angularVelocity * dt;
	angularAcceleration = (1 / mass) * angularForce;
}



void Emitter::move() {
	//make the enemy sprites move parabola path
	for (int i = 0; i < sys->sprites.size(); i++) {
		sys->sprites[i].velocity.x += sin(sys->sprites[i].velocity.y);
		sys->sprites[i].velocity.y += 1;
		
		
	}
}

void Emitter::sine() {
	//make the enemy sprites move sinusoidal path
	for (int i = 0; i < sys->sprites.size(); i++) {
		sys->sprites[i].trans.x += sin(ofGetElapsedTimef())/5;
		sys->sprites[i].trans.y += cos(ofGetElapsedTimef())/5;

	}
	
}

























//--------------------------------------------------------------
void ofApp::setup() {
	game_state = "start";
	ofSetVerticalSync(true);
	bgm.load("sound/bgm.mpeg");
	gg.load("sound/gg.mp3");
	w.load("sound/win.mp3");
	start_screen.load("images/startScreen.png");
	background.load("images/background1.png");
	bullet.load("sound/firing.mp3");
	bulletImage.load("images/bullet.png");
	targetImage.load("images/target.png");
	invaderImage.load("images/inv.png");
	explode.load("sound/explode.mp3");
	font.load("font/Marlboro.ttf", 30);
	end_screen.load("images/endScreen.png");
	if (turretImage.load("images/player.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/player.png");
		ofExit();
	}

	score = 0;




		

	bgm.play();
		//initiallize emitters
		turret = new Emitter(new SpriteSystem());
		enemy = new Emitter(new SpriteSystem());
		enemyT = new Emitter(new SpriteSystem());
		//setting images
		turret->setImage(turretImage);
		enemy->setImage(invaderImage);
		enemyT->setImage(invaderImage);
		//initializing values
		//turret->drawable = true;
		enemy->drawable = true;
		enemyT->drawable = true;



		turret->height = 50;
		turret->mass = 1.0;
		turret->force = ofVec3f(0, 0, 0);

		enemy->trans = ofVec3f(ofGetWindowWidth() / 4, ofGetWindowHeight() / 2, 0);
		//enemy->trans = ofVec3f(100, ofGetWindowHeight() / 2, 0);
		enemyT->trans = ofVec3f(ofGetWindowWidth() - 100, ofGetWindowHeight() / 2, 0);

		enemy->mass = 2.0;
		enemyT->mass = 2.0;

		turret->setLifespan(100);

		enemy->rate = 3;
		enemyT->rate = 3;

		enemy->lifespan = 500;
		enemyT->lifespan = 500;

		turret->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
		turret->trans = (ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
		turret->head = glm::vec3(0, -1, 0);
		turret->left = glm::vec3(1, 0, 0);

		turret->setChildImage(bulletImage);


		

		enemyT->start();
		enemy->start();
		turret->start();
		turret->setupSpeed(3);

		//set up guis, including sliders and toggles
		gui.setup();
		gui.add(rate.setup("rate", 7, 7, 20));
	//	gui.add(lifespan.setup("player lifespan",10,5,100));
		//gui.add(lifespan.setup("life", -1, .1, 10));
		gui.add(speed.setup("speed", 3, .1, 10));
		//gui.add(velocity.setup("velocity", glm::vec3(0, 0, 0), ofVec3f(-1000, -1000, 0), ofVec3f(1000, 1000, 0)));

		gui.add(leftEnemyFiringSpeed.setup("left enemy fire speed", 50, 10, 500));

		gui.add(leftEnemyRate.setup("left enemy rate", 3, 0, 10));
		gui.add(leftEnemyLife.setup("left enemy lifespan", 10, .1, 10));
		//gui.add(leftEnemyVelocity.setup("left enemy velocity", glm::vec3(0, 200, 0), ofVec3f(-1000, -1000, 0), ofVec3f(1000, 1000, 0)));


		gui.add(rightEnemyFiringSpeed.setup("right enemy fire speed", 50, 10, 500));

		gui.add(rightEnemyRate.setup("right enemy rate", 3, 0, 10));
		gui.add(rightEnemyLife.setup("right enemy lifespan", 10, .1, 10));
		//gui.add(rightEnemyVelocity.setup("right enemy velocity", glm::vec3(0, 200, 0), ofVec3f(-1000, -1000, 0), ofVec3f(1000, 1000, 0)));

		gui.add(parabola.setup("parabola", false));
		gui.add(sine.setup("sine", false));
		gui.add(circle.setup("apply circular force", false));


		//initailize the enemy sprite velocity
		enemy->setVelocity(glm::vec3(0, 200, 0));
		enemyT->setVelocity(glm::vec3(0, 200, 0));



		turbForce = new TurbulenceForce(ofVec3f(-20, -20, 0), ofVec3f(20, 20, 0));
		gravityForce = new GravityForce(ofVec3f(0, -20, 0));
		radialForce = new ImpulseRadialForce(1000.0);

		explosion.sys->addForce(turbForce);
		explosion.sys->addForce(gravityForce);
		explosion.sys->addForce(radialForce);
		explosion.setVelocity(ofVec3f(200, 200, 0));
		explosion.setOneShot(true);
		explosion.setEmitterType(RadialEmitter);
		explosion.setGroupSize(20);
		explosion.setParticleRadius(5);
		explosion.setLifespan(1);
		explosion.setPosition(ofVec2f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));

	

}

//--------------------------------------------------------------
void ofApp::update() {
	explosion.update();
	if (game_state == "game") {

		turret->integrate();
		enemy->integrate();
		enemyT->integrate();
		

		

		

		//updating player emitter
		turret->setRate(rate);
		//turret->setLifespan(10);    // convert to milliseconds 
		turret->setVelocity(ofVec3f(velocity->x, velocity->y, 0));
		turret->setupSpeed(speed);
		turret->update();

		//updating the LHS enemy emitter
		enemy->update();
		//enemy->setLifespan(leftEnemyLife * 1000);
		enemy->setRate(leftEnemyRate);

		//updating the RHS enemy emitter
		enemyT->update();
		//enemyT->setLifespan(rightEnemyLife * 1000);
		enemyT->setRate(rightEnemyRate);




		//set two enemy emitters

		enemy->setVelocity(turret->trans - enemy->trans);
		enemyT->setVelocity(turret->trans - enemyT->trans);

		//enemy->setVelocity(ofVec3f(leftEnemyVelocity->x, leftEnemyVelocity->y, leftEnemyVelocity->z));
		//enemyT->setVelocity(ofVec3f(rightEnemyVelocity->x, rightEnemyVelocity->y, rightEnemyVelocity->z));



		//check bullet collisions
		checkCollision();


		//prevent player going outside
		if (turret->trans.y <= 0) {
			turret->setPosition(ofVec3f(turret->trans.x, 1, 0));
		}
		if (turret->trans.x <= 0) {
			turret->setPosition(ofVec3f(1, turret->trans.y, 0));
		}
		if (turret->trans.x >= ofGetWindowWidth()) {
			turret->setPosition(ofVec3f(ofGetWindowWidth() - 1, turret->trans.y, 0));
		}
		if (turret->trans.y >= ofGetWindowHeight()) {
			turret->setPosition(ofVec3f(turret->trans.x, ofGetWindowHeight() - 1, 0));
		}


		if (enemyT->trans.y >= ofGetWindowHeight()) {
			enemyT->setPosition(ofVec3f(enemyT->trans.x, ofGetWindowHeight() - 1, 0));
		}
		if (enemyT->trans.y <= 0) {
			enemyT->setPosition(ofVec3f(enemyT->trans.x,  1, 0));
		}

		float time = ofGetElapsedTimeMillis();
		//generating enemy sprites from the LHS emitter	
		if ((time - (*enemy).lastSpawned) > (1000.0 / enemy->rate)) {
			Sprite sprite;
			sprite.setImage(targetImage);
			sprite.velocity = (*enemy).velocity;
			sprite.lifespan = leftEnemyLife * 1000;
			sprite.setPosition((*enemy).trans);
			sprite.birthtime = time;
			sprite.width = enemy->childWidth;
			sprite.height = enemy->childHeight;
			(*enemy).sys->add(sprite);
			(*enemy).lastSpawned = time;
			//bullet.play();
		}
		//movements of the sprites generated by the LHS emitter
		for (int i = 0; i < enemy->sys->sprites.size(); i++) {
			enemy->sys->sprites[i].trans += (enemy->sys->sprites[i].velocity.getNormalized() * leftEnemyFiringSpeed / ofGetFrameRate());
		}

		//generating enemy sprites from the RHS emitter	
		if ((time - (*enemyT).lastSpawned) > (1000.0 / enemyT->rate)) {
			Sprite sprite;
			sprite.setImage(targetImage);
			sprite.velocity = (*enemyT).velocity;
			sprite.lifespan = rightEnemyLife * 1000;
			sprite.setPosition((*enemyT).trans);
			sprite.birthtime = time;
			sprite.width = enemyT->childWidth;
			sprite.height = enemyT->childHeight;
			(*enemyT).sys->add(sprite);
			(*enemyT).lastSpawned = time;
			//bullet.play();
		}

		//movements of the sprites generated by the RHS emitter
		for (int i = 0; i < enemyT->sys->sprites.size(); i++) {
			enemyT->sys->sprites[i].trans += (enemyT->sys->sprites[i].velocity.getNormalized() * rightEnemyFiringSpeed / ofGetFrameRate());
		}


		//shoot
		if (life > 0) {
			if ((time - (*turret).lastSpawned) > (1000.0 / turret->rate)) {
				Sprite sprite;
				sprite.setImage(bulletImage);
				sprite.velocity = (*turret).head * 100;
				sprite.lifespan = 2000;
				sprite.setPosition((*turret).trans);
				sprite.birthtime = time;
				sprite.width = turret->childWidth;
				sprite.height = turret->childHeight;
				(*turret).sys->add(sprite);
				(*turret).lastSpawned = time;
				bullet.play();
			}
		}

		for (int i = 0; i < turret->sys->sprites.size(); i++) {
			turret->sys->sprites[i].trans += turret->sys->sprites[i].velocity.getNormalized() * 400 / ofGetFrameRate();
		}

		//EXTRA CREDIT PART I: interesting moving paths

		if (parabola) {
			enemy->move();
			enemyT->move();
		}
		if (sine) {
			enemy->sine();
			enemyT->sine();
		}

		if (circle) {
			enemy->force = ofVec3f(cos(ofGetElapsedTimef()) * 50, sin(ofGetElapsedTimef()) * 50, 0);
			
		}
		else {
			enemy->force = ofVec3f(0, 0, 0);
		}


		animateTurret();

		if (gameOver) {
			gg.play();
			turret->stop();
			turret->drawable = false;
			//enemy->setRate(-1);
			enemy->stop();
			enemy->drawable = false;
			enemyT->stop();
			enemyT->drawable = false;
			turret->sys->sprites.clear();
			enemy->sys->sprites.clear();
			enemyT->sys->sprites.clear();
		}

		if (enemy->lifespan <= 0) {
			enemy->trans = ofVec3f(-1000, -1000, 0);
			enemy->stop();
			enemy->drawable = false;
			enemy->sys->sprites.clear();
		}

		if (enemyT->lifespan <= 0) {
			enemyT->trans = ofVec3f(-1000, -1000, 0);
			enemyT->stop();
			enemyT->drawable = false;
			enemyT->sys->sprites.clear();
		}


		if (enemyT->lifespan <= 0 && enemy->lifespan <= 0) {
			w.play();
			turret->stop();
			turret->drawable = false;
			turret->sys->sprites.clear();
			game_state = "win";
		}
	}
	
	

}


//--------------------------------------------------------------
void ofApp::draw() {
	
	if (game_state == "start") {
		
		start_screen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		
	}
	if (game_state == "game") {
		background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		if (turret->lifespan > 0) {
			turret->draw();
		}
		
		enemy->draw();
		enemyT->draw();
		turret->sys->draw();
		enemy->sys->draw();
		enemyT->sys->draw();
		if (!bHide) {
			gui.draw();
		}
		
		//display socre, life
		string lifeText;
		string scoreText;
		int l = turret->lifespan;
		lifeText += "Life: " + std::to_string(l);
		scoreText += "Score: " + std::to_string(score);
		//ofDrawBitmapString(scoreText, ofPoint(ofGetWindowWidth()/2, ofGetWindowHeight()-20));
		font.drawString(scoreText, ofGetWindowWidth() / 2 - font.stringWidth(scoreText) *2  , ofGetWindowHeight() - 20);

		font.drawString(lifeText, ofGetWindowWidth() / 2 + font.stringWidth(lifeText) , ofGetWindowHeight() - 20);
		
	}
	if (game_state == "end") {
		
		bgm.stop();
		
		string text = "GAME OVER";
		string scoreText;
		scoreText += "Score: " + std::to_string(score);
		end_screen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		font.drawString(text, ofGetWindowWidth() / 2 - font.stringWidth(text) / 2, ofGetWindowHeight() / 2 - font.stringHeight(text) / 2);
		font.drawString(scoreText, ofGetWindowWidth() / 2 - font.stringWidth(scoreText) / 2, ofGetWindowHeight() / 2 + 20);
	}

	if (game_state == "win") {
		bgm.stop();
		string text = "CONGRATS! YOU WIN";
		string scoreText;
		scoreText += "Score: " + std::to_string(score);
		end_screen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		font.drawString(text, ofGetWindowWidth() / 2 - font.stringWidth(text) / 2, ofGetWindowHeight() / 2 - font.stringHeight(text) / 2);
		font.drawString(scoreText, ofGetWindowWidth() / 2 - font.stringWidth(scoreText) / 2, ofGetWindowHeight() / 2 + 20);
	}


	explosion.draw();

	if (!bHide) { gui.draw(); }

	ofSetColor(255, 255, 255);
	
	
}
//Check collisions
void ofApp::checkCollision() {
		float collisionDistL = turret->childHeight / 2 + enemy->height / 2; //Collision distance with the left enemy emitter
		float collisionDistR = turret->childHeight / 2 + enemyT->height / 2;//Collision distance with the right enemy emitter
		float collisionDistC = turret->childHeight / 2 + enemy->childHeight / 2;// Collsion distance with the enemy sprites
		float collisionDistP = turret->height / 2 + enemy->childHeight / 2;
		float collisionDistP2 = turret->height / 2 + enemyT->childHeight / 2;
		float c1 = turret->height / 2 + enemy->height / 2;
		float c2 = turret->height / 2 + enemyT->height / 2;
		
		//collisions with the left enemy emitter
		for (int i = 0; i < turret->sys->sprites.size(); i++) {
			//convert glm::vec3 to ofVec3f to use distance() method
			ofVec3f player = ofVec3f(turret->sys->sprites[i].trans.x, turret->sys->sprites[i].trans.y, turret->sys->sprites[i].trans.z);
			
			ofVec3f emitter = ofVec3f(enemy->trans.x, enemy->trans.y, enemy->trans.z);
			//calculating distances
			
			float distanceE = player.distance(emitter);
			for (int j = 0; j< enemy->sys->sprites.size() ; j++) {
				ofVec3f invader = ofVec3f(enemy->sys->sprites[j].trans.x, enemy->sys->sprites[j].trans.y, enemy->sys->sprites[j].trans.z);
				float distance = player.distance(invader);
				//if distance<=collion distance, enemy sprite/bullet sprite disappears, update score and play sound
				if (distance <= collisionDistC) {
					enemy->sys->sprites[j].lifespan = 0;
					turret->sys->sprites[i].lifespan = 0;
					score += 1;
					explosion.setPosition(ofVec3f(turret->sys->sprites[i].trans));
					explosion.sys->reset();
					explosion.start();
					explode.play();
				}
				

				}
			if (distanceE <= collisionDistL) {
				turret->sys->sprites[i].lifespan = 0;
				enemy->lifespan -= 100;

				explosion.setPosition(ofVec3f(turret->sys->sprites[i].trans));
				explosion.sys->reset();
				explosion.start();

				explode.play();
			}

			}
		//collisions with the right enemy emitter
		for (int i = 0; i < turret->sys->sprites.size(); i++) {
			ofVec3f player = ofVec3f(turret->sys->sprites[i].trans.x, turret->sys->sprites[i].trans.y, turret->sys->sprites[i].trans.z);
			ofVec3f emitter = ofVec3f(enemyT->trans.x, enemyT->trans.y, enemyT->trans.z);
			float distanceE = player.distance(emitter);
			for (int j = 0; j < enemyT->sys->sprites.size(); j++) {
				//convert glm::vec3 to ofVec3f to use distance() method
			//	ofVec3f player = ofVec3f(turret->sys->sprites[i].trans.x, turret->sys->sprites[i].trans.y, turret->sys->sprites[i].trans.z);
				ofVec3f invader = ofVec3f(enemyT->sys->sprites[j].trans.x, enemyT->sys->sprites[j].trans.y, enemyT->sys->sprites[j].trans.z);
				//ofVec3f emitter = ofVec3f(enemyT->trans.x, enemyT->trans.y, enemyT->trans.z);
				float distance = player.distance(invader);
				float distanceE = player.distance(emitter);
				//if distance<=collion distance, enemy sprite/bullet sprite disappears, play sound
				if (distance <= collisionDistC) {
					enemyT->sys->sprites[j].lifespan = 0;
					turret->sys->sprites[i].lifespan = 0;
					score += 1;
					explosion.setPosition(ofVec3f(turret->sys->sprites[i].trans));
					explosion.sys->reset();
					explosion.start();

					explode.play();
				}
				

			}

			if (distanceE <= collisionDistL) {
				turret->sys->sprites[i].lifespan = 0;
				enemyT->lifespan -= 100;
				explosion.setPosition(ofVec3f(turret->sys->sprites[i].trans));
				explosion.sys->reset();
				explosion.start();

				explode.play();
			}

		}


		for (int i = 0; i < enemy->sys->sprites.size(); i++) {
			//for (int j = 0; j < enemyT->sys->sprites.size(); j++) {
				ofVec3f player = ofVec3f(turret->trans.x, turret->trans.y, turret->trans.z);
				ofVec3f invader = ofVec3f(enemy->sys->sprites[i].trans.x, enemy->sys->sprites[i].trans.y, enemy->sys->sprites[i].trans.z);
				//ofVec3f invader2 = ofVec3f(enemyT->sys->sprites[j].trans.x, enemyT->sys->sprites[j].trans.y, enemyT->sys->sprites[j].trans.z);
				ofVec3f emitter = ofVec3f(enemy->trans.x, enemy->trans.y, enemy->trans.z);
				ofVec3f emitter2 = ofVec3f(enemyT->trans.x, enemyT->trans.y, enemyT->trans.z);
				float distance = player.distance(invader);
				if (distance <= collisionDistP) {
					enemy->sys->sprites[i].lifespan = 0;
					turret->lifespan =  turret->lifespan - 1;
					cout << turret->lifespan << endl;;
					//gameOver = true;
					explosion.setPosition(ofVec3f(turret->trans));
					explosion.sys->reset();
					explosion.start();

					explode.play();
				//}
			}
		}

		for (int i = 0; i < enemyT->sys->sprites.size(); i++) {
			//for (int j = 0; j < enemyT->sys->sprites.size(); j++) {
			ofVec3f player = ofVec3f(turret->trans.x, turret->trans.y, turret->trans.z);
			ofVec3f invader = ofVec3f(enemyT->sys->sprites[i].trans.x, enemyT->sys->sprites[i].trans.y, enemyT->sys->sprites[i].trans.z);
			//ofVec3f invader2 = ofVec3f(enemyT->sys->sprites[j].trans.x, enemyT->sys->sprites[j].trans.y, enemyT->sys->sprites[j].trans.z);
			ofVec3f emitter = ofVec3f(enemy->trans.x, enemy->trans.y, enemy->trans.z);
			ofVec3f emitter2 = ofVec3f(enemyT->trans.x, enemyT->trans.y, enemyT->trans.z);
			float distance = player.distance(invader);
			if (distance <= collisionDistP2) {
				enemyT->sys->sprites[i].lifespan = 0;
				turret->lifespan = turret->lifespan - 1;
				cout << turret->lifespan << endl;;
				//gameOver = true;
				explosion.setPosition(ofVec3f(turret->trans));
				explosion.sys->reset();
				explosion.start();

				explode.play();
				//}
			}
		}

		ofVec3f t = ofVec3f(turret->trans.x, turret->trans.y, turret->trans.z);
		ofVec3f el = ofVec3f(enemy->trans.x, enemy->trans.y, enemy->trans.z);
		ofVec3f er = ofVec3f(enemyT->trans.x, enemyT->trans.y, enemyT->trans.z);
		float d1 = t.distance(el);
		float d2 = t.distance(er);
		if (d1 <= c1 || d2 <= c2) {
			turret->lifespan = turret->lifespan - 5;
			turret->trans = ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
			explode.play();
		}
		

		if (turret->lifespan <= 0) {
			explosion.setPosition(ofVec3f(turret->trans));
			explosion.sys->reset();
			explosion.start();
			gameOver = true;
			game_state = "end";
		}


		
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
	if (x == 0 || y == 0 || x == ofGetWindowWidth() || x == ofGetWindowHeight())
	{
		(*turret).trans = (*turret).trans;
	}
	else
		(*turret).trans = glm::vec3(x, y, 0);
	

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

void ofApp::animateTurret() {
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians((*turret).rotation), glm::vec3(0, 0, 1));
	glm::vec4 temp = glm::vec4(0, 1, 1, 1);
	glm::vec4 temp1 = glm::vec4(1, 0, 1, 1);


	temp = rot * temp;
	temp1 = rot * temp1;
	(*turret).head = glm::vec3(-temp.x, -temp.y, 0);
	(*turret).left = glm::vec3(temp1.x, temp1.y, 0);
	


	if (playerState == "moveUp") {
		turret->force = turret->head * turret->speed * 20;
	}
	if (playerState == "moveDown") {
		turret->force = turret->head * turret->speed * -20;
	}
	if (playerState == "moveLeft") {
		turret->force = turret->left * turret->speed * -20;
	}
	if (playerState == "moveRight") {
		turret->force = turret->left * turret->speed * 20;
	}
	if (playerState == "upRight") {
		turret->force = turret->head * turret->speed * 20 + turret->left * turret->speed * 20;
	}
	if (playerState == "downRight") {
		turret->force = turret->head * turret->speed * -20 + turret->left * turret->speed * 20;
	}
	if (playerState == "upLeft") {
		turret->force = turret->head * turret->speed * 20 + turret->left * turret->speed * -20;
	}
	if (playerState == "downLeft") {
		turret->force = turret->head * turret->speed * -20 + turret->left * turret->speed * -20;
	}
	if (playerState == "rotateRight") {
		turret->angularForce = 100;

	}
	if (playerState == "rotateLeft") {
		turret->angularForce = -100;
		
	}

	if (playerState == "idle") {
		turret->force = glm::vec3(0, 0, 0);
		turret->angularForce = 0;
	}

}



void ofApp::keyPressed(int key) {
	if (key == prevKey) {
		return;
	}
	prevKey = key;

	
	//movements

	if (key == OF_KEY_UP) {
		//playerState = "moveUp";
		
		
			up = true;
			//shoot when move
			/*if (firing) {
				float time = ofGetElapsedTimeMillis();
				(*turret).trans -= (*turret).head * (*turret).speed;
				if ((time - (*turret).lastSpawned) > (1000.0 / turret->rate)) {
					Sprite sprite;
					sprite.setImage(bulletImage);
					sprite.velocity = (*turret).velocity;
					sprite.lifespan = (*turret).lifespan;
					sprite.setPosition((*turret).trans);
					sprite.birthtime = time;
					sprite.width = turret->childWidth;
					sprite.height = turret->childHeight;
					(*turret).sys->add(sprite);
					(*turret).lastSpawned = time;
					bullet.play();
				}
			}*/
			//move diagonally
			if (left) {
				playerState = "upLeft";
			}
			else if (right) {
				playerState = "upRight";
			}
			else
				playerState = "moveUp";
		
		

		}
	
	if (key == OF_KEY_DOWN) {
		
			down = true;
			/*if (firing) {
				float time = ofGetElapsedTimeMillis();
				(*turret).trans += (*turret).head * (*turret).speed;
				if ((time - (*turret).lastSpawned) > (1000.0 / turret->rate)) {
					Sprite sprite;
					sprite.setImage(bulletImage);
					sprite.velocity = (*turret).head * 100;
					sprite.lifespan = (*turret).lifespan;
					sprite.setPosition((*turret).trans);
					sprite.birthtime = time;
					sprite.width = turret->childWidth;
					sprite.height = turret->childHeight;
					(*turret).sys->add(sprite);
					(*turret).lastSpawned = time;
					bullet.play();
				}
			}*/
			 if (left) {
				playerState = "downLeft";
			}
			else if (right) {
				playerState = "downRight";
			}
			else
				playerState = "moveDown";
		
	}
	if (key == OF_KEY_LEFT) {
		

			left = true;
			/*if (firing) {
				float time = ofGetElapsedTimeMillis();
				(*turret).trans -= (*turret).left * (*turret).speed;
				if ((time - (*turret).lastSpawned) > (1000.0 / turret->rate)) {
					Sprite sprite;
					sprite.setImage(bulletImage);
					sprite.velocity = (*turret).velocity;
					sprite.lifespan = (*turret).lifespan;
					sprite.setPosition((*turret).trans);
					sprite.birthtime = time;
					sprite.width = turret->childWidth;
					sprite.height = turret->childHeight;
					(*turret).sys->add(sprite);
					(*turret).lastSpawned = time;
					bullet.play();
				}
			}*/
			if (up) {
				playerState = "upLeft";
			}
			else if (down) {
				playerState = "downLeft";
			}
			else
				playerState = "moveLeft";
		
	}
		
	if (key == OF_KEY_RIGHT) {
		

			right = true;
			/*if (firing) {
				float time = ofGetElapsedTimeMillis();
				(*turret).trans += (*turret).left * (*turret).speed;
				if ((time - (*turret).lastSpawned) > (1000.0 / turret->rate)) {
					Sprite sprite;
					sprite.setImage(bulletImage);
					sprite.velocity = (*turret).velocity;
					sprite.lifespan = (*turret).lifespan;
					sprite.setPosition((*turret).trans);
					sprite.birthtime = time;
					sprite.width = turret->childWidth;
					sprite.height = turret->childHeight;
					(*turret).sys->add(sprite);
					(*turret).lastSpawned = time;
					bullet.play();
				}
			}*/
			if (up) {
				playerState = "upRight";
			}
			else if (down) {
				playerState = "downRight";
			}
			else {
				playerState = "moveRight";
			}
		
	}
		
	if (key == ' ') {
		if (game_state == "game") {
			//shoot
			firing = true;
			
			life = 7;
			
			/*if (right) {
				playerState = "moveRight";
			}
			else if (left) {
				playerState = "moveLeft";
			}
			else if (up) {
				playerState = "moveUp";
			}
			else if (down) {
				playerState = "moveDown";
			}
			else {
				
			}*/
			
		}
		

			
		
		}
	if (key == '.') {
		//rotating
		playerState = "rotateRight";
	
	}
		
	
	if (key == ',') {
		playerState = "rotateLeft";
	}

	if (key == 'h') {
		bHide = !bHide;
	}

	if (key == 'w') {
		enemyT->force = ofVec3f(0, -100, 0);
	}
	if (key == 's') {
		enemyT->force = ofVec3f(0, 100, 0);
	}

}



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {


	prevKey = -9999999999;
	//start page
	if (game_state == "start" && key == ' ') {
		game_state = "game";
	}
	

	switch (key) {
	case OF_KEY_LEFT:
		playerState = "idle";
		left = false;
		break;
	case OF_KEY_RIGHT:
		playerState = "idle";
		right = false;
		break;
	case OF_KEY_UP:
		playerState = "idle";
		up = false;
		break;
	case OF_KEY_DOWN:
		playerState = "idle";
		down = false;
		break;
		
	case ' ':
		firing = false;
		life = -1;
		playerState = "idle";
		break;
	case '.':
		playerState = "idle";
		break;
	case ',':
		playerState = "idle";
		break;
	case 'w':
		enemyT->force = ofVec3f(0, 0, 0);
		break;
	case 's':
		enemyT->force = ofVec3f(0, 0, 0);
		break;
	}
	

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

