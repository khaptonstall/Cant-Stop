#Can't Stop Game
A branch of the REU-CAAR a the University of Maryland.

###Requires:
SDL2  
SDL2_image  
SDL2_ttf  

To install via Homebrew:
```
brew install SDL2 SDL2_image SDL2_ttf
```

###To play Can't Stop:

(1) Clone or download/unzip our repository.

(2)
In `main.cpp` make sure you have testing set to false:
```
const bool TESTING_MODE = false;
```

(3)
Choose which players will be playing (players found in Cant-Stop>source>players) and update inside `GameState.cpp`:
```
GameState::GameState() {
	player1 = new Human_Player();
	player2 = new monte_carlo_player(); // new Human_Player(), new probability_player(), etc.
	player1->name = "Player 1";
	player2->name = "Player 2";
}
```

(4)
Compile
```
cd Cant-Stop
make
./cantstop
```

###Team Members

[Kyle Haptonstall][kyle-github]

[Alejandro Huerta][alex-github]

[kyle-github]:  http://github.com/khaptonstall
[alex-github]:  http://github.com/ahuerta0686
