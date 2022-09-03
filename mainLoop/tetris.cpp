// handle input
// if up then rotate (have to do a lot of checking for if can rotate in available space)
// if left and not at left bound then move left
// if right and not at right bound then move right
// if down then straight down
// also have a flag to not move things too fast

// check collision
// if collided at bottom (or actually, any part, because yeah that's annoying) then:
//   put that piece in place
//   spawn new piece
//   increase score by 1
// if collided at top of screen: return false;

// if a whole line is filled:
//   clear line
//   move every piece down
//   score += 10 or something (max is 99 so maybe we don't even do that?)

// move piece down by 1 (controlled by speed flags of course)
// blink piece (also controlled by flag)