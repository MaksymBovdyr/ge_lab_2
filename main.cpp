
#include <SFML/Graphics.hpp>


const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,
    sf::Keyboard::Z,
    sf::Keyboard::Up,
    sf::Keyboard::Down
};



//Parameters

const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; // ~60 FPS

sf::Vector2f ball_velocity;
bool is_player_serving = true;
float initial_velocity_x = 100.f; //horizontal velocity
float initial_velocity_y = 60.f; //vertical velocity

const float velocity_multiplaier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.

//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];

void load() {
    ball_velocity = { (is_player_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
}


void init() {
    for (sf::RectangleShape &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }

    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius / 2.f, ballRadius / 2.f);


	//reset position

    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
	paddles[1].setPosition(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f);

	ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);




}


void reset() {
    //reset positions
    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
    paddles[1].setPosition(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f);
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

    //reset ball velocity
    ball_velocity = { (is_player_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
	is_player_serving = !is_player_serving;
}


void update(float dt) {
    //handle paddle movement

    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(sf::Vector2f(0.f, direction * paddleSpeed * dt));

    float direction2 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }
    if (sf::Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }
    paddles[1].move(sf::Vector2f(0.f, direction2 * paddleSpeed * dt));

    if (paddles[0].getPosition().y - (paddleSize.y * 0.5) < 0) {
        paddles[0].setPosition(paddles[0].getPosition().x, paddleSize.y * 0.5);
    }
    else if (paddles[0].getPosition().y + (paddleSize.y * 0.5) > gameHeight) {
        paddles[0].setPosition(paddles[0].getPosition().x, gameHeight - (paddleSize.y * 0.5));
	}

    if (paddles[1].getPosition().y - (paddleSize.y * 0.5) < 0) {
        paddles[1].setPosition(paddles[1].getPosition().x, paddleSize.y * 0.5);
    }
    else if (paddles[1].getPosition().y + (paddleSize.y * 0.5) > gameHeight) {
        paddles[1].setPosition(paddles[1].getPosition().x, gameHeight - (paddleSize.y * 0.5));
    }

    //move ball
	ball.move(ball_velocity * dt);

    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) {
        //bottom wall
        ball_velocity.x *= velocity_multiplaier;
        ball_velocity.y *= -velocity_multiplaier;
        ball.move(sf::Vector2f(0.f, -10.f));
    }
    else if (by < 0) {
        //top wall
        ball_velocity.x *= velocity_multiplaier;
        ball_velocity.y *= -velocity_multiplaier;
        ball.move(sf::Vector2f(0.f, 10.f));
    }
    else if (bx > gameWidth) {
        //right wall
        reset();
    }
    else if (bx < 0) {
        //left wall
        reset();
    }
    else if (
        //ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        //ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5)) {
            
        // bounce off left paddle
        ball_velocity.x *= -velocity_multiplaier;
        ball_velocity.y *= velocity_multiplaier;
		ball.move(sf::Vector2f(10.f, 0.f));
            }
    else if (
        //ball is inline or behind paddle AND
        bx > gameWidth - paddleSize.x - paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        //ball is above bottom edge of paddle
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5)) {

        // bounce off right paddle
        ball_velocity.x *= -velocity_multiplaier;
        ball_velocity.y *= velocity_multiplaier;
		ball.move(sf::Vector2f(-10.f, 0.f));
    }





}

void render(sf::RenderWindow& window) {
	//Draw everything
    window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
}


int main () {
	//creating the window
    sf::RenderWindow window(sf::VideoMode({gameWidth, gameHeight}), "PONG!");

    //initialise and load

    load();

	init();

    while (window.isOpen()) {
        //calculate dt
		static sf::Clock clock;
		const float dt = clock.restart().asSeconds();

		window.clear();

        update(dt);
        render(window);
		//wait for the time_step finish before displaying the next frame
        sf::sleep(sf::seconds(time_step));

		//Wait for VSync
        window.display();
    }

    //Unload and shutdown

    return 0;






}


