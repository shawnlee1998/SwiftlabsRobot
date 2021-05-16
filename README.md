# Swiftlabs Robot Navigator Assignment

- Cygwin version 3.2.0
- GCC version 10.2.0

## Build Command
```
gcc -o robot.exe robot.c

gcc -o robot robot.c
```

## Current Issues
- Getting input from console is blocking. Typing exit command does not close the producer thread immediately. Needs a return character before it finishes.
- An invalid command input of 7 characters prefixed with '+++' still is put in buffer unnecessarily.