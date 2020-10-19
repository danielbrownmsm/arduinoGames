# arduinoGames
programs for a small console consisting of an ATMega2560, 8x8 red LED matrix, and a jostick with button. Support for a passive buzzer for sound feedback coming soon

# List of Somewhat/Mostly Complete Games
 * Conway's Game of Life
 * 21/Blackjack
 * Tower Stack Blocks (like the ones you would see at an arcade)

# List of Programs That May or May Not be Implemented
 * Pong
 * Paint
 * Maze
 * An RPG (similar to Existential Crisis)
 * Snake
 * Some kind of racing game
 * Flappy bird
 * The Dino Game
 * Tic Tac Toe/Knots and Crosses
 * Tetris (if I get it working right)
 * Breakout
 * Yahtzee
 * Reaction-timer
 * Calculator
 * Canyon Runner
 * Battleship
 * Memory game (Simon says)

# Thoughts on how I might implement these 
because I've made little progress on doing anything and in general am kinda procrastinating right now
### Pong
 - bouncing ball
 - whenever it hits something negate the velocity direction (which one depends on what it hit)
 - paddle is fixed vertically (or horizontally, depending on how you look at it)
 - display score after each goal
### Paint
oof
 - blinky thing represents cursor
 - short press is toggle dot at that loc, long press opens menu
 - in menu can:
  - change size of brush
  - clear screen
  - save (output text to Serial and then save that in a file)
  - load (input text from file using gui that I haven't made yet)
  - change type of brush (normal, line, spray paint)
### Maze
 - generate a (solvable) maze
 - blinky thing is character
 - score is time taken to get through maze
### An RPG
 - Nothing yet
 ### Snake
  - no idea how to do the snakey part
  - apple is easy just blink at a random loc
### Racing Game
 - 2d
 - up is forward
 - left steers left
 - right steers right
 - blink the player
 - no opponets
 - score is time
 - racetracks increase in difficulty the higher you go
### Flappy bird
 - 1-wide columns
 - up flaps up
 - blink player
 - score is time (but higher is better)
### Dino Game
 - chrome://dino
### Tetris
 - the rules of tetris, pretty much
 - up holds
 - down goes down
 - left/right rotate shape
 - might have to mod shapes depending on how large they are b/c I have only 64 LEDs
 - if a whole row is filled it clears
 - score is score
### Tic Tac Toe
 - yeah graphics are hard
### Breakout
 - Breakout breakout = new Breakout(breakout_inits);
### Yahtzee
 - I mean some kind of dice game would work but Yahtzee?
 - how to display text? graphics?
### Reaction-timer
 - when X is displayed do Y or something (probably when screen turns red press button)
 - score is time * scale factor b/c no decimals
### Calculator
 - entering numbers is hard
 - selecting operator less so
 - Arduino doesn't have an `eval()` or `exec()` so will have to do something else
 - stack-based stuff seems promising (Reverse-Polish Notation or something like Whitespace)
### Canyon Runner
 - Width no more than 6 (off led on each side)
 - no less than 2 (so you can, you know, move)
 - instead of keeping track of the walls individually, keep track of the left (or right) wall and then the width, and draw the other wall at wall_x + width
 - score is time
 - should get smaller over time
 - also change more often over time
### Battleship
 - Battleship battleship = new Dreadfreakinought();
 - might require some modification b/c 8x8 instead of 10x10
 - hits are blinked slowly
 - cursor is blinked fastily
 - misses are solid
 - no indication if you've sunk the ship
 - score is either binary (win/lose, num wins vs num losses) or not (do it faster/less turns and lose less ships, get higher score)
### Memory Game
 - Basically the Simon Electronic Memory Game thing
 - Up, Down, Left, Right
 - Possibly middle (button press)

## Other thoughts
Will def need a `display_digits(99);` func.
