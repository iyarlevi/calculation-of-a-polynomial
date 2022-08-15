# calculation-of-a-polynomial
calculation of a polynomial with threads and with processes
polynoms calculator
authored by Iyar Levi

------------DESCRIPTION------------------------------------------------------------------------------
ex4.a:

The program get string as an input fron the user.
The string contain polynom and value.
The program send pieces of the polynom to threads and the threads will sum up the polynom
piece with the value. after all the threads return their answer, the program will print the result. 

ex4.b:

The program get string as an input fron the user.
The string contain polynom and value.
The program send pieces of the polynom to sons process and the sons will sum up the polynom
piece with the value. after all the sons return their answer, the program will print the result.
The program works with shared memory. 

------------functions--------------------------------------------------------------------------------

freeArgv - function that free 2D arrays.

numOfX - function that count how many X are in the input.

sumMonom - function that sum the value of the polynom after the deployment of value.


------------memory--------------------------------------------------------------------------

ex4.b use shared memory.
               

------------INPUT-------------------------------------------------------------------------

the input is a sentence that contain polynom, value.

------------OUTPUT------------------------------------------------------------------------
The output is the answer to the polynom after deploymend of the value.

after the input "done" - the program end.

