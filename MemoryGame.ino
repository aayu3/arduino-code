/* Creation Crate Memory Game
Math the pattern of the LEDs!
Each time you get the pattern right, it gets longer and faster.
The game resets when you make a mistake.

Step 1: Create Constant Variables
Constant variables have static values. Once you set their values, you
can't change them*/

// These are the pins the buttons connect to
const int button1 = 2;
const int button2 = 3;
const int button3 = 4;
const int button4 = 5;

// These are the pins the LEDs connect to.
const int led1 = 7;
const int led2 = 8;
const int led3 = 9;
const int led4 = 10;

// This is the buzzer connection
const int buzzer = 12;

// These are the tones for the buzzer in Hertz
const int tones[] = {1900, 1600, 1300, 1000, 3200};

// These are the state of the buttons;
int buttonState[] = {0,0,0,0};
int lastButtonState[] = {0,0,0,0};
int buttonCounter[] = {0,0,0,0};
// 1 = button presed, 0 = button not pressed.

int gameOn = 0; // A new game or level starts when gameOn is 0.
int wait = 0; // This is used to tell the game to wait until the user inputs a pattern.

int currentLevel = 1; // This is the current level, and the number of presses in the level
int dlay = 500; // This is the amount of time to wait for the next button press (0.5)
int ledTime = 500; // This is the amount of time each LED will stay on when the button is pressed.
int n_levels = 10; // This is the number of levels until the game is won, once it is won it gets faster.
int pinAndTone = 0; // This variable will be used to determine which LED to turn and its corresponding buzzer tone.
int correct = 0; // This value must become 1 to go to the next level.
int speedFactor = 5; // This is the speed of the game and increases when a level is beaten.
int ledDelay = 200; //This is the aoutn of time taken before the next LED in the patter lights up (0.2) seconds.
// function to play tones on buzzer

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone*2) {
        digitalWrite(buzzer, HIGH); // Turns the buzzer on.
        delayMicroseconds(tone); // Creates the tone of the buzzer.
        digitalWrite(buzzer, LOW); // Turns the buzzer off.
        delayMicroseconds(tone);
    }
}

// Step 4: Initialize the inputs and outputs
// Here, we're telling the arduino that buttons are inputs and LEDs and the buzzer are outputs.
void setup() {
    randomSeed(analogRead(0)); //Used to generate random numbers.

    // Initialize inputs
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(button4, INPUT);

    // Initialize outputs
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(buzzer, OUTPUT);
}

// Step 5: Create the game loop
void loop() {
    int n_array[n_levels]; // n_array will store the randomized game pattern.
    int u_array[n_levels]; // stores the user input pattern

    int i;
    // This creates the random pattern
    if (gameOn == 0) {
        for (i = 0; i < n_levels; i++) {
            n_array[i] = 0;
            u_array[i] = 0;
            n_array[i] = random(0,4);
        }
        gameOn = 1;
    }

    if (wait == 0) { // Triggers if no action is required from the user
        delay (200);
        i = 0;
        for (i = 0; i < currentLevel; i++) {
            ledDelay = ledTime/(1+(speedFactor/n_levels)* (currentLevel-1));
            pinAndTone = n_array[i];
            digitalWrite(pinAndTone + 7, HIGH);
            playTone(tones[pinAndTone], ledDelay);
            digitalWrite(pinAndTone + 7, LOW);
            delay(100/speedFactor);
        }
        wait = 1;
    }
    i = 0;
    int buttonChange = 0; // buttonChange will be used to detect when a button is pressed.
    int j = 0; // j is the current position in the pattern
    while (j < currentLevel) {
        while (buttonChange == 0) {
            for (i = 0; i < 4; i++) {
                // This loop determines which button was pressed by the user
                buttonState[i] = digitalRead(i + 2);
                buttonChange += buttonState[i];
            }
        }
        // This turns on the corresponding LED to the button pressed, and calls the function "playTone" to pay the corresponding sound on the buzzer.
        for (i = 0; i < 4; i++) {
            if (buttonState[i] == HIGH) {
                digitalWrite(i + 7, HIGH);
                playTone(tones[i], ledTime);
                digitalWrite(i + 7, LOW);
                wait = 0;
                u_array[j] = 1;
                buttonState[i] = LOW;
                buttonChange = 0;
            }

            //This section checks if the button pressed by the user matches the game pattern
            if (u_array[j] == n_array[j]) {
                j++;
                correct = 1;
            } else {
                correct = 0;
                i = 4;
                j = currentLevel;
                wait = 0;
            } 
        }

        if (correct == 0) {
            // If the user makes a mistake these variables will be reset so that the game starts over.
            delay(300);
            i = 0;
            gameOn = 0;
            currentLevel  = 1;

            // These loops make all the LEDs blink twice and the buzzer beep twice when the user makes a mistake and loses the game.
            for (i = 0; i < 4; i++) {
                digitalWrite(i + 7, HIGH);
            }
            playTone(tones[4], ledTime);

            for (i = 0; i < 4; i++) {
                digitalWrite(i + 7, LOW);
            }
            delay (200);
            for ( i = 0; i < 4; i++) {
                digitalWrite(i + 7, HIGH);
            }
            playTone(tones[4], ledTime);
            for (i = 0; i < 4; i++) {
                digitalWrite(i + 7, LOW);
            }
            delay(500);
            gameOn = 0;
        }

        // If the user gets the sequence right, the game goes up one level.
        if (correct == 1) {
            currentLevel++;
            wait = 0;
        }

        // This section plays the victory song if the game is beaten.
        if (currentLevel == n_levels) {
            delay(500);
            int notes[] = {2,2,2,2,0,1,2,1,2};
            int note = 0;
            int tempo[] = {200, 200, 200, 400, 400, 400, 200, 200, 600};
            int breaks[] = {100, 100, 100, 200, 200, 200, 300, 100, 200};

            for (i = 0; i < 9; i++) {
                note = notes[i];
                digitalWrite(note + 7, HIGH);
                playTone(tones[note], tempo[i]);
                digitalWrite(note + 7, LOW);
                delay(breaks[i]);
            }

            gameOn = 0;
            currentLevel = 1;
            n_levels = n_levels + 2; // This adds two more levels to the game.
            speedFactor++; // increases speed by one.
        }
    }
}
