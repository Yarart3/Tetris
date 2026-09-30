# Tetris

![Gameplay](docs/gameplay.gif)

A Tetris clone written in C++ with SDL2. It includes a playable normal mode, an automated test mode that replays a game from files, a level system with increasing speed, sound effects and a persistent high score table.

The project focuses on object-oriented design and manual memory management: the game logic is split into independent classes (board, piece, game, match) and the move and piece queues are implemented as custom linked lists instead of STL containers.

## Features

- All 7 classic tetrominoes (O, I, T, L, J, Z, S) with clockwise and counterclockwise rotation.
- Collision detection for movement and rotation against walls, floor and placed blocks.
- Line clearing with gravity for the rows above.
- Soft drop and hard drop.
- 10 difficulty levels: the fall speed increases every 1,000 points.
- Score system based on cleared lines, placed pieces and hard drops.
- Pause and resume during a game; the music pauses too.
- In-game volume panel with mute. The volume is saved between sessions.
- Everything runs inside the game window: main menu, controls and scores screens, and name entry after a game over.
- **Test mode**: loads an initial board, a fixed sequence of pieces and a list of moves from text files and plays them automatically, making the game logic reproducible and easy to verify.
- High score table saved to disk and sorted on load.
- Original chiptune rendition of *Korobeiniki* (public domain) as background music, and a game over sound.

## Tech Stack

| Area | Tools |
| --- | --- |
| Language | C++ |
| Graphics and input | SDL2, SDL2_image, SDL2_ttf, libpng |
| Audio | WinMM (MCI, with `PlaySound` as fallback) |
| IDE / build | Visual Studio 2019 or later (uses the installed default toolset), x86  |
| Platform | Windows |

## Architecture

```
main
 └── Tetris          menu, game loop, high scores
      └── Partida    match state: score, level, timing, input / test moves
           ├── Joc           game rules: spawn, move, rotate, drop, place
           │    ├── Tauler   21x12 board, collisions, line clearing
           │    ├── Figura   tetromino shape, position and rotation
           │    └── CuaFigura    linked-list queue of pieces (test mode)
           └── CuaMoviment       linked-list queue of moves (test mode)
```

- **Tetris**: runs the in-window menu, the frame loop with delta time (`SDL_GetPerformanceCounter`), pause and volume panel handling, the game over name entry and the high score list (`std::list` kept in descending order).
- **Partida**: updates the match every frame, applies the fall timer for the current level, reads keyboard input in normal mode or consumes the move queue in test mode, and computes the score.
- **Joc**: applies the rules of the game and coordinates the board with the active piece.
- **Tauler**: stores the board as a color matrix, validates moves and rotations, detects full rows and removes them. Overloads `<<` and `>>` to read and write the board from files.
- **Figura**: stores each piece as a 4x4 matrix and rotates it through matrix transposition plus row or column reversal.
- **GraphicManager**: singleton that loads and draws sprites, fonts (including centered text) and filled rectangles.
- **Audio**: static class that plays the music and the game over sound through MCI, which allows pausing, resuming and changing the volume. The volume and mute state are saved to `data/Games/config.txt`.

## Game Modes

**Normal mode**: the player controls the pieces with the keyboard.

**Test mode**: reads three files from `data/Games/`:

| File | Content |
| --- | --- |
| `partida.txt` | Initial piece and initial board state |
| `figures.txt` | Sequence of pieces to spawn |
| `moviments.txt` | Sequence of moves to execute (0 left, 1 right, 2 rotate clockwise, 3 rotate counterclockwise, 4 soft drop, 5 hard drop) |

## Controls

**Menu**

| Key | Action |
| --- | --- |
| ↑ / ↓ or W / S | Select option |
| Enter / Space | Confirm |
| 1–6 | Choose an option directly |
| Esc | Back to the menu from a sub-screen; in the main menu, jump to *Exit* |

**In game**

| Key | Action |
| --- | --- |
| ← / A | Move left |
| → / D | Move right |
| ↑ / W | Rotate clockwise |
| ↓ / S | Rotate counterclockwise |
| C | Soft drop |
| Space | Hard drop |
| P | Pause / resume (also pauses the music) |
| Esc | Back to menu |

**Volume** (in the menu and in game)

| Key | Action |
| --- | --- |
| V | Open / close the volume panel (pauses the game while open) |
| + / − | Volume up / down by 5% |
| ↑ / → and ↓ / ← | Volume up and down while the panel is open |
| M | Mute / unmute |

Pressing + / − or M without opening the panel shows it for two seconds without pausing the game.

**Game over**

| Key | Action |
| --- | --- |
| Letters, digits, `_`, `-` | Type your name (up to 12 characters; spaces become `_`) |
| Backspace | Delete the last character |
| Enter | Save the score |
| Esc | Skip without saving |

## Scoring

| Action | Points |
| --- | --- |
| Clear 1 line | 100 |
| Clear 2 lines | 150 |
| Clear 3 lines | 175 |
| Clear 4 lines | 200 |
| Place a piece | 10 |
| Hard drop | 30 |

## Project Structure

```
0. C++ Code/
   Logic Game/       game logic classes
   Graphic Lib/      SDL2 wrapper for video, keyboard, sprites, sound and fonts
1. Resources/
   data/Graphics/    board, background and block sprites
   data/Fonts/       FreeSans font
   data/Games/       test mode files and high scores (config.txt with the volume is created at runtime)
   music.wav         background music
   gameover.wav      game over sound
2. Platforms/
   0. Windows Desktop/   Visual Studio solution and SDL2 / libpng headers
docs/
   gameplay.gif      gameplay preview
```

## Build and Run

1. Clone the repository.
2. Open `2. Platforms/0. Windows Desktop/MP_Practica.sln` in Visual Studio 2019, 2022 or 2026 with the *Desktop development with C++* workload.
3. Select the platform and configuration (the SDL2 and libpng libraries are included in `extlibs/` and `Program/`).
4. Build the solution. The executable is generated in `Program/`, and a post-build step copies `data/` and the sound files next to it.
5. Run it with **F5** from Visual Studio or by opening `Program/MP_Practica.exe`.
6. The menu opens in the game window; no console is needed.

## Credits

The graphics wrapper in `Graphic Lib/` was provided as base material for the course. The game logic, class design, data structures, test mode, scoring and level system were implemented for this project.

## License

This project is available under the MIT License.

Tetris is a trademark of The Tetris Company. This is a non-commercial educational project.
