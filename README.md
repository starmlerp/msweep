# msweep
## description
This is a little code to recreate the traditional minesweeper game in a terminal window using ncurses library
I have writen it in one evening in around 3 hours of time, out of boredom
Code is messy, and i do not really intend to provide any further maintenance for it, as i am only uploading it for archival purposes
## options
`-w`: set width of the minefield
`-h`: set height of the minefield
`-b`: set number of bombs in the field
`-d`: set a template preset, 1 - 3, corresponding to a difficulty:
  > `1` easy: `-w 9 -h 9 -b 10`
  > `2` medium: `-w 16 -h 16 -b 40`
  > `3` hard: `-w 16 -h 30 -b 99`
  
  the code expects each option and its arguments to be whitespace separated
