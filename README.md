# Minesweeper
This is a recreation of the Windows version of Minesweeper using C++. The SFML Library is used to implement graphics. Most of the graphics are taken from Windows Minesweeper. The controls are simple: Left-Click to clear a tile, and Right-Click to flag or unflag a tile. 
# Features
This game supports three difficulties and a custom board editor, as shown below.

**Title Screen:**
![Title screen](https://i.imgur.com/aLH4eFT.png)
  &nbsp;
  &nbsp;
**Easy Board (9x9):**
![Easy board (9x9) with 10 bombs](https://i.imgur.com/EB016Eo.png)
  &nbsp;
  &nbsp;
**Medium Board (16x16):**
![Medium board (16x16) with 40 bombs](https://i.imgur.com/drF0fhx.png)
  &nbsp;
  &nbsp;
**Hard Board (30x16):**
![Hard board (30x16) with 100 bombs](https://i.imgur.com/g7JltwX.png)
  &nbsp;
  &nbsp;
**Custom board editor:**
![Custom board editor](https://i.imgur.com/YKoZWqG.png)
Supports a board from size 5x5 up to 100x100, and bombs covering from 5%  to 20% of the board.
To create a custom board press "Enter" in the custom board menu.
  &nbsp;
  When all the tiles have been cleared, the game ends and displays "You Win!". 
![You Win!](https://i.imgur.com/x6oe4d5.png)

If you Left-Click a tile hiding a bomb, you lose, and the text "Game Over!" is displayed. 
![Game Over!](https://i.imgur.com/8W3xGXV.png)
# Quick Start
1. Install codeblocks: https://www.codeblocks.org/downloads/
    - Make sure you have a compiler installed
3. Download code from github
	- `git clone https://github.com/danielchandg/minesweeper.git`
4. Navigate to `/bin/Debug` and run `minesweeper.exe`