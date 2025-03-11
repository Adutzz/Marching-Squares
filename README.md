# Marching-Squares
1) Main Function:
In the variable "command" I save the string input which tells me what
function I have to call and then enter a while loop which ends when the input
gives "command" the string "EXIT". If everything goes well, I free all the
remaining alocated memory with the function free_memory().

2)Read:
"READ" command gives input values to specific variables and checks through
errors() function if the input given is correct. If it is, i allocate memory
to matrix based on the input from "n" and "m" variables.

3)Write:
Write function prints the elements from matrix, if "GRID" was called before
write prints the same value 3 times for every element in matrix because in
function grid() we are doing the arithmetic mean of the RGB and the output
is smaller. Otherwise, it prints matrix normally.

4)Init_contur:
The init_contur() function has 2 uses. If it is called from the function
march(), it has the purpose to find the matrix that fits in certain cases
by looking for the value calculated in march() through "find" and comparing
it to every pattern init_contur() created. If "find" is equal to -1 then it
means that init_contur() was not called by march() and it was called in main()
by "command" and it prints all the possible patterns the matrix can take.

5)Grid:
Grid first does the arithmetic mean of the RGB values and then calls
directions() that adds in a temporary matrix the arithmetic mean of the
neigbours in a "X" shape for 1 in every 4 elements of the matrix.
Then it compares the value to a preset variable "sigma" and if it is lower
it is converted to 1, else the value becomes 0 so we can use march().
Because the output has to have N squared, squares, where N is width/4,
I saved another row and line that i did not print, so the output is correct
and I can use it after in march(). After everything I transfer the new
values back to the original matrix.

6)March:
This is the main function of the program that cannot be done without calling
grid() first, so if "command" did not request grid(), march() will do it
automatically. March sections the matrix given by squares and for every
square it calculates "find" based on the corners through a binary conversion
to decimal and scales up the matrix so that instead of the original square
it has the 4 by 4 matrix given by init_contur()

7)Resize:
The last function scales up the matrix given by 4, by taking every
element in the matrix and multiplicating it to be 4 x 12 from 1 x 3 
(RGB value). I do that in a auxiliar matrix "tmp" and in the end, i adjust
the memory alocated to "matrix" so i can copy all the elements from the
temporary matrix back to the original one.
