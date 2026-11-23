
This is a port of the original 1990s VAX/VMS VT220 `curses` Othello
implementation (in `../vax-vms/`) to modern `ncurses` and C11 compilers,
keeping the core game logic and AI unchanged.

## Build & Run

```sh
cc -std=c11 -Wall -Wextra -O2 othello.c -lncurses -o othello
./othello
```

## Controls

- Arrow keys or `u`/`d`/`l`/`r`: move cursor

- `Enter`: place piece

- `q`: quit



