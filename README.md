Thanks for coming to my code! This time enhanced by Dylan Marsh.

For compiling, we have a makefile that compiles all the code files into "scrabble". 
Now also when running, there are 5 arguments for each enhancement. Simply type 'y' to include one, 
or 'n' to exclude. The enhancements are in the order:
Help! - Better Invalid Input - Colour - Word Checking - 3/4 Player

Enhancements:
 - Help!: during your turn, type "help" to get a list of useful commands
 - Better Invalid Input: For each invalid input, the game will tell you what part of the input was invalid
 - Colour: adds colour to player names, the tiles in their hand, and the tiles they place on the board
 - Word Checking: checks the "web2" file if the word exists in the dictionary, placing will fail if word does not exist
 - 3/4 Player: gives the option to play with 3 or 4 players. 2 is not an option with this enhancment on

All save files are saved under the extension ".save", any other extensions will not be valid for loading.
Save files may only have letters or numbers in them - no special characters. All files are found under saveFiles/*.
A save file with more than 2 players is denoted with "#X player" at the top of the file, where X is the number
of players - 3 or 4.

Test expout files look a bit funky because of how colour is implemented, however it looks fine on the terminal.
This does not change when the enhancement is turned off, since the implementation attaches the default colour
escape code to anything without a designated colour.

Credits

Name: Alex Ly
Student ID: s3660743
Email: s3660743@student.rmit.edu.au

Name: Radiyah Islam
Student ID: s3866903
Email: s3866903@student.rmit.edu.au

Name: Sayed Walif Ali
Student ID: s3866910
Email: s3866910@student.rmit.edu.au

Name: Dylan Marsh
Student ID: s3784998
Email: s3784998@student.rmit.edu.au
