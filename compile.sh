#!/bin/bash
echo "Code should be -Wextra -pedantic -Wall clean on gcc, but not all compilers support those flags"
echo "On solaris you might want to change -O3 to -xO3..."
echo ""
echo "cc -O3 *.c -lpthread -o btcrack"
cc -O3 *.c -lpthread -o btcrack
