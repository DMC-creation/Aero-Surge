
#include <GL\glut.h>
#include <iostream>
#include<math.h>
#include<string.h>
#include <time.h> //time manipulation
#include <sstream> //string manipulation
#include<mmsystem.h>
using namespace std;
const int WINDOW_WIDTH = 370;
const int WINDOW_HEIGHT = 700;
const int REFRESH_RATE = 16;

void update(int);
void restartGame();
float speed = 0.0f;
int healthCount = 3;
int score = 0;
bool gameOverSoundPlayed = false;
bool startSoundPlayed=false;

enum ScreenState {
    START_SCREEN,
    INSTRUCTIONS_SCREEN,
    GAME_SCREEN
};
ScreenState currentScreen = START_SCREEN;

void drawScene();

void playCrashSound() {
    PlaySound(TEXT("crash"), NULL, SND_FILENAME | SND_ASYNC );
}
void playLooseSound() {
    PlaySound(TEXT("loose"), NULL, SND_FILENAME | SND_ASYNC);
}
void playRocketSound() {
    PlaySound(TEXT("rocket"), NULL, SND_FILENAME | SND_ASYNC| SND_LOOP);
}
void playStartSound() {
    PlaySound(TEXT("start"), NULL, SND_FILENAME | SND_ASYNC| SND_LOOP);
}


float _move = 0.0f;
float obstacleSpeed1 = 0.0f;
float _obstacle1 = 0.0f;
int flag = 0;
int obstacleFlag = 0;
int random = 0;
float _stars;

void drawStartScreen() {

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.2, 0.5);

    string startText = "Start";
    for (char c : startText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glRasterPos2f(-0.2, 0.3);
    string instructionsText = "Instructions";
    for (char c : instructionsText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glRasterPos2f(-0.2, 0.1);
    string exitText = "Exit";
    for (char c : exitText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glutSwapBuffers();
}

void drawInstructionsScreen() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    // Draw "Instructions" at the top
    string title = "Instructions:";
    glRasterPos2f(-0.4, 0.8); // Adjust position as needed
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Draw the list of instructions
    string instructions = "\n\nPress 'L/R'arrow key to dodge.\n";
    instructions += "Press 'R' to restart.\n";
    instructions += "Press 'ESC' to exit the game.\n\n";

    // Set the initial position for drawing the list
    float xPos = -0.9f;
    float yPos = 0.6f;

    // Draw each character of the instructions string
    for (char c : instructions) {
        if (c == '\n') {
            yPos -= 0.1f; // Move to the next line
            glRasterPos2f(xPos, yPos);
        } else {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    glutSwapBuffers();
}

void drawText(const char *text, int length, float x, float y) {//text rendering in opengl
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];//used to store the current projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();//pushes to stack current identity matrix
	glLoadIdentity(); //reset the identity matrix
	glRasterPos2f(x, y);//position of Game over and score
	for(int i=0; i<length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r': // Restart game on pressing 'r'
        case 'R':
            restartGame();
            break;
        case 27: // ESC key to exit the game
            exit(0);
            break;
        default:
            break;
    }
}
void restartGame() {
    healthCount = 3;
    score = 0;
    speed = 0.0f;
    _move = 0.0f;
    obstacleSpeed1 = 0.0f;
    _obstacle1 = 0.0f;
    flag = 0;
    obstacleFlag = 0;
    random = 0;
    _stars = 0.0f;
    playRocketSound(); // Restart background sound
    gameOverSoundPlayed = false;
    startSoundPlayed=false;
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            if (score <= 5) {
                obstacleSpeed1 = 0.008f;
                speed = 0.01f; // plane speed
            } else if (score > 5 && score <= 10) {
                obstacleSpeed1 = 0.01f;
                speed = 0.013f;
            } else if (score > 10 && score <= 15) {
                obstacleSpeed1 = 0.012f;
                speed = 0.016f;
            } else if (score > 15 && score <= 20) {
                obstacleSpeed1 = 0.014f;
                speed = 0.019f;
            } else if (score > 20 && score <= 30) {
                obstacleSpeed1 = 0.016f;
                speed = 0.022f;
            } else if (score > 30 && score <= 40) {
                obstacleSpeed1 = 0.020f;
                speed = 0.028f;
            } else if (score > 40 && score <= 50) {
                obstacleSpeed1 = 0.024f;
                speed = 0.034f;
            } else if (score > 50) {
                obstacleSpeed1 = 0.028f;
                speed = 0.040f;
            }
            break;
        case GLUT_KEY_LEFT:
            if (score <= 5) {
                obstacleSpeed1 = 0.008;
                speed = -0.01f;
            } else if (score > 5 && score <= 10) {
                obstacleSpeed1 = 0.01;
                speed = -0.013f;
            } else if (score > 10 && score <= 15) {
                obstacleSpeed1 = 0.012f;
                speed = -0.016f;
            } else if (score > 15 && score <= 20) {
                obstacleSpeed1 = 0.014f;
                speed = -0.019f;
            } else if (score > 20 && score <= 30) {
                obstacleSpeed1 = 0.016f;
                speed = -0.022f;
            } else if (score > 30 && score <= 40) {
                obstacleSpeed1 = 0.020f;
                speed = -0.028f;
            } else if (score > 40 && score <= 50) {
                obstacleSpeed1 = 0.024f;
                speed = -0.034f;
            } else if (score > 50) {
                obstacleSpeed1 = 0.028f;
                speed = -0.040f;
            }
            break;
        default:
            break;
    }
}


//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void drawGameScreen() {
	srand (time(NULL));//if no seeding value same sequence of random number are produced >>> time(null) provides actual time
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1,0,0);
	glLoadIdentity(); //Reset the drawing perspective
	glMatrixMode(GL_MODELVIEW);
	_stars -= 0.001f;

	// Game Over and Score Display
	if(healthCount == 0) {
		// Game Over
		if (!gameOverSoundPlayed) {
            playLooseSound();
            Sleep(2300);
            gameOverSoundPlayed = true; // Mark that game over sound has been played
        }
        if (!startSoundPlayed) {
            playStartSound();
            startSoundPlayed = true; // Mark that game over sound has been played
        }

		glColor3d(1,1,0);//game over in yellow
		stringstream convert; // stringstream used for the conversion
		string text = "Game Over";//string which will contain the result
		//text = convert.str();
		drawText(text.data(), text.size(), -0.21f, 0.2f);

		convert << score;
		text = "Score: " + convert.str();//string which will contain the score
		drawText(text.data(), text.size(), -0.10f, -0.1f);
        glColor3d(1,1,1);
        string restartText = "Press 'R' to Restart";
        string exitText = "Press 'Esc' to Exit";
       drawText(restartText.data(), restartText.size(), -0.4f, -0.4f);
       drawText(exitText.data(), exitText.size(), -0.4f, -0.5f);
		obstacleSpeed1 = 0.0f;
		_stars = 0.0f;
		speed = 0.0f;
		_move = 0.0f;

	} else {
		glColor3d(1,1,0);//score color
		string text;//string which will contain the result
		stringstream convert; // stringstream used for the conversion
		convert << score;//add the value of Number to the characters in the stream
		text = convert.str();
		drawText(text.data(), text.size(), 0.0f, 0.4f);//game on score print
	}

	// Stars
	glColor3d(1,1,1);//white color
	glPointSize(4);
	glPushMatrix();//push into stack
	glTranslatef(0.0f, _stars, 0.0f);//along y axis
	glBegin(GL_POINTS);
		glVertex3f(0.5f, 1.99f, 0.0f);
		glVertex3f(-0.97f, 1.97f, 0.0f); //
		glVertex3f(0.1f, 1.977f, 0.0f);
		glVertex3f(0.7f, 1.97f, 0.0f);
		glVertex3f(-0.9f, 1.92f, 0.0f); //
		glVertex3f(-0.6f, 1.92f, 0.0f);
		glVertex3f(0.97f, 1.86f, 0.0f);
		glVertex3f(-0.2f, 1.82f, 0.0f);
		glVertex3f(0.55f, 1.83f, 0.0f);
		glVertex3f(-0.87f, 1.80f, 0.0f);

		float incre = 0.24f;
		for(int i=0; i<20; i++) {
		glVertex3f(0.5f, 1.99f-incre, 0.0f);
		glVertex3f(-0.97f, 1.97f-incre, 0.0f); //
		glVertex3f(0.1f, 1.977f-incre, 0.0f);
		glVertex3f(0.7f, 1.97f-incre, 0.0f);
		glVertex3f(-0.9f, 1.92f-incre, 0.0f); //
		glVertex3f(-0.6f, 1.92f-incre, 0.0f);
		glVertex3f(0.97f, 1.86f-incre, 0.0f);
		glVertex3f(-0.2f, 1.82f-incre, 0.0f);
		glVertex3f(0.55f, 1.83f-incre, 0.0f);
		glVertex3f(-0.87f, 1.80f-incre, 0.0f);
		incre += 0.25;
		}

		if(_stars <= -1) {
			_stars = 0.0f;
		}

	glEnd();
	glPopMatrix();


	if(obstacleFlag == 0) {
		random = rand() % 3 + 1;
	}
	if(random == 1 && (obstacleFlag == 0 || obstacleFlag == 1)) {
		obstacleFlag = 1;
		_obstacle1 -= obstacleSpeed1;
		//Obstacle Left
		glColor3d(0.8f, 0.4f, 0.0f);
		glPushMatrix();
		glTranslatef(0.0f, _obstacle1, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.9f, 0.0f);
			glVertex3f(-1.0f, 0.9f, 0.0f);
		glEnd();
		glPopMatrix();

		if(_obstacle1 <= -2) {
			obstacleFlag = 0;
			_obstacle1 = 0.0f;
			score++;					//Increment of Score
		}
	} else if(random == 2 && (obstacleFlag == 0 || obstacleFlag == 2)){
		obstacleFlag = 2;
		_obstacle1 -= obstacleSpeed1;
		//Obstacle Right
		glColor3d(0.8f, 0.4f, 0.0f);
		glPushMatrix();
		glTranslatef(0.0f, _obstacle1, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.9f, 0.0f);
			glVertex3f(1.0f, 0.9f, 0.0f);
		glEnd();
		glPopMatrix();

		if(_obstacle1 <= -2) {
			obstacleFlag = 0;
			_obstacle1 = 0.0f;
			score++;					//Increment of Score
		}
	} else if(random == 3 && (obstacleFlag == 0 || obstacleFlag == 3)){
		obstacleFlag = 3;
		_obstacle1 -= obstacleSpeed1;
		//Obstacle Middle
		glColor3d(0.8f, 0.4f, 0.0f);
		glPushMatrix();
		glTranslatef(0.0f, _obstacle1, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.5f, 1.0f, 0.0f);
			glVertex3f(0.5f, 0.9f, 0.0f);
			glVertex3f(1.0f, 0.9f, 0.0f);

			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f(-0.5f, 1.0f, 0.0f);
			glVertex3f(-0.5f, 0.9f, 0.0f);
			glVertex3f(-1.0f, 0.9f, 0.0f);
		glEnd();
		glPopMatrix();

		if(_obstacle1 <= -2) {
			obstacleFlag = 0;
			_obstacle1 = 0.0f;
			score++;					//Increment of Score
		}

	}


	if(healthCount == 3) {
	//Health Bar
		glColor3d(1,0,0);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.96f, 0.96f, 0.0f);
			glVertex3f(0.91f, 0.96f, 0.0f);
			glVertex3f(0.91f, 0.90f, 0.0f);
			glVertex3f(0.96f, 0.90f, 0.0f);

			glVertex3f(0.88f, 0.96f, 0.0f);
			glVertex3f(0.83f, 0.96f, 0.0f);
			glVertex3f(0.83f, 0.90f, 0.0f);
			glVertex3f(0.88f, 0.90f, 0.0f);

			glVertex3f(0.80f, 0.96f, 0.0f);
			glVertex3f(0.75f, 0.96f, 0.0f);
			glVertex3f(0.75f, 0.90f, 0.0f);
			glVertex3f(0.80f, 0.90f, 0.0f);
		glEnd();
		glPopMatrix();
	} else if(healthCount == 2) {
		glColor3d(1,0,0);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.96f, 0.97f, 0.0f);
			glVertex3f(0.91f, 0.97f, 0.0f);
			glVertex3f(0.91f, 0.91f, 0.0f);
			glVertex3f(0.96f, 0.91f, 0.0f);

			glVertex3f(0.88f, 0.97f, 0.0f);
			glVertex3f(0.83f, 0.97f, 0.0f);
			glVertex3f(0.83f, 0.91f, 0.0f);
			glVertex3f(0.88f, 0.91f, 0.0f);
		glEnd();
		glPopMatrix();
	} else if(healthCount == 1) {
		glColor3d(1,0,0);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.96f, 0.97f, 0.0f);
			glVertex3f(0.91f, 0.97f, 0.0f);
			glVertex3f(0.91f, 0.91f, 0.0f);
			glVertex3f(0.96f, 0.91f, 0.0f);
		glEnd();
		glPopMatrix();
	}


	// Player
	glColor3d(1,0,0);
    glPushMatrix();
	_move += speed;
	glTranslatef(_move, 0.0f, 0.0f);
    glBegin(GL_QUADS);

		glColor3d(0.8f, 0.4f, 0.0f);
		glVertex3f(0.0f, -0.63f, 0.0f);
        glVertex3f(0.0f, -0.63f, 0.0f);
        glVertex3f(0.17f, -0.7f, 0.0f);
        glVertex3f(-0.17f, -0.7f, 0.0f);

		glColor3d(00,0.0,0.8);
        glVertex3f(-0.1f, -0.6f, 0.0f);
        glVertex3f(0.1f, -0.6f, 0.0f);
        glVertex3f(0.1f, -0.75f, 0.0f);
        glVertex3f(-0.1f, -0.75f, 0.0f);

		glColor3d(0.8f, 0.4f, 0.0f);
		glVertex3f(0.0f, -0.55f, 0.0f);
        glVertex3f(0.0f, -0.55f, 0.0f);
        glVertex3f(0.05f, -0.60f, 0.0f);
        glVertex3f(-0.05f, -0.60f, 0.0f);

	glEnd();
    glPopMatrix();

	// Side Collision
	if(_move <= -0.83) {
		_move = -0.83;
	} else if(_move >= 0.83) {
		_move = 0.83;
	}

	//Obstacle Collision Left Bar
	if((_move > -1.0f && _move < 0.0f) && (_obstacle1 < -1.45f && _obstacle1 > -1.8f) && random == 1) {
		        playCrashSound();

        Sleep(1000);
        playRocketSound();
		healthCount--;
		obstacleFlag = 0;
		_obstacle1 = 0.0f;			// Decrement of Score
	} else if((_move < 1.0f && _move > 0.0f) && (_obstacle1 < -1.45f && _obstacle1 > -1.8f) && random == 2) {
	//Obstacle Collision Right Bar
		glutDisplayFunc(drawScene);
        playCrashSound();
		Sleep(1000);
        playRocketSound();
		healthCount--;
		obstacleFlag = 0;
		_obstacle1 = 0.0f;			// Decrement of Score
	} else if((_move > 0.4f || _move < -0.4f) && (_obstacle1 < -1.45f && _obstacle1 > -1.8f) && random == 3) {
	//Obstacle Collision Middle Bar
	        playCrashSound();
		Sleep(1000);
        playRocketSound();
		healthCount--;
		obstacleFlag = 0;
		_obstacle1 = 0.0f;			//Decrement of Score
	}

	glutSwapBuffers();
}
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch (currentScreen) {
        case START_SCREEN:
            drawStartScreen();
            break;
        case INSTRUCTIONS_SCREEN:
            drawInstructionsScreen();
            break;
        case GAME_SCREEN:
            drawGameScreen();
            break;
    }
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert mouse coordinates to OpenGL coordinates
        float xPos = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0 - 1.0;
        float yPos = 1.0 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2.0;

        switch (currentScreen) {
            case START_SCREEN:
                // Check if start button is clicked
                if (xPos >= -0.2 && xPos <= 0.2 && yPos >= 0.5 && yPos <= 0.6) {
                    currentScreen = GAME_SCREEN;
                    playRocketSound();
                }
                // Check if instructions button is clicked
                else if (xPos >= -0.2 && xPos <= 0.2 && yPos >= 0.3 && yPos <= 0.4) {
                    currentScreen = INSTRUCTIONS_SCREEN;
                }
                // Check if exit button is clicked
                else if (xPos >= -0.2 && xPos <= 0.2 && yPos >= 0.1 && yPos <= 0.2) {
                    exit(0);
                }
                break;
            case INSTRUCTIONS_SCREEN:
                // Check if anywhere is clicked to go back to the start screen
                currentScreen = START_SCREEN;
                break;
            case GAME_SCREEN:
                // No mouse interactions during game screen
                break;
        }
    }
}
void update(int value) {

	glutPostRedisplay(); //Notify GLUT that the` display has changed

	glutTimerFunc(REFRESH_RATE, update, 0); //Notify GLUT to call update again in 10 milliseconds
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Aero_Surge");
	glutDisplayFunc(drawScene);
	glutSpecialFunc(specialKeys); //Special Key Handler
	  glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    playStartSound();
	glutTimerFunc(10, update, 0); //Add a timer
	glutMainLoop();
	return 0;
}
