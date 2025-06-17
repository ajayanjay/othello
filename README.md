# Othello Game in C

![Alt text](./preview.png?raw=true "Preview")
A comprehensive implementation of the classic Othello (Reversi) board game written in C, featuring multiple AI difficulty levels, game saving/loading, replay system, and cross-platform compatibility.

## 🎯 Overview

This Othello implementation provides a complete gaming experience with:
- **Multiple Player Types**: Human players and AI opponents (Easy, Medium, Hard)
- **Advanced AI**: Minimax algorithm with alpha-beta pruning for challenging gameplay
- **Game Management**: Save/load functionality, undo/redo system
- **Replay System**: Record and playback game sessions
- **Scoreboard**: Track high scores with difficulty-based weighting
- **Cross-Platform**: Works on Windows and Linux

## 🚀 Features

### Game Modes
- **Human vs Human**: Classic two-player mode
- **Human vs AI**: Challenge AI opponents of varying difficulty
- **AI vs AI**: Watch AI opponents battle each other

### AI Difficulty Levels
- **Easy AI**: Random move selection with visual confirmation
- **Medium AI**: Basic strategy implementation
- **Hard AI**: Advanced minimax algorithm with alpha-beta pruning

### Game Features
- Interactive board display with move highlighting
- Real-time score tracking
- Game state persistence (save/continue)
- Comprehensive undo/redo system
- Replay recording and playback
- High score leaderboard

## 📋 Prerequisites

- **GCC Compiler**: Version supporting C99 standard
- **Make**: For building the project
- **Standard C Library**: Including `dirent.h` for directory operations

### Platform-Specific Requirements

**Linux:**
- `make`
- `gcc`

**Windows:**
- MinGW-w64 or similar GCC distribution
- Make utility (included with MinGW)

## 🛠️ Installation

### 1. Clone the Repository
```bash
git clone https://github.com/ajayanjay/othello.git
cd othello
```

### 2. Build the Project
```bash
make
```

### 3. Run the Game
```bash
make run
```

Or directly:
```bash
./build/othello      # Linux
./build/othello.exe  # Windows
```

## 🎮 How to Play

### Basic Rules
1. Othello is played on an 8×8 board with black (X) and white (O) pieces
2. Players alternate turns, with black moving first
3. Valid moves must capture at least one opponent piece by flanking
4. Captured pieces are flipped to the current player's color
5. Game ends when no valid moves remain for either player
6. Winner is determined by who has the most pieces

### Controls
- **Arrow Keys**: Navigate menu options or available moves
- **Enter**: Confirm selection or move
- **Z**: Undo last move (not available against Hard AI)
- **Y**: Redo undone move
- **Escape**: Exit to main menu (saving the game)

### Game Interface
```
  +-----------------+
1 | . . . . . . . . |
2 | . . . . . . . . |
3 | . . . . . . . . |
4 | . . . O X . . . |
5 | . . . X O . . . |
6 | . . . . . . . . |
7 | . . . . . . . . |
8 | . . . . . . . . |
  +-----------------+
    A B C D E F G H
```

## 📁 Project Structure

```
othello/
├── src/
│   ├── main.c             # Main program entry and menu system
│   ├── game.c             # Core game logic and mechanics
│   ├── replay.c           # Game replay functionality
│   ├── ai/
│   │   └── ai.c           # AI implementation and evaluation
│   ├── attribute/
│   │   ├── move.c         # Move structure and utilities
│   │   ├── piece.c        # Piece definitions and operations
│   │   ├── player.c       # Player types and behaviors
│   │   └── score.c        # Scoring and leaderboard system
│   ├── datastructure/
│   │   ├── array.c        # Dynamic array implementation
│   │   ├── deque.c        # Double-ended queue for undo/redo
│   │   ├── linked.c       # Linked list utilities
│   │   ├── nbtree.c       # N-ary tree for minimax
│   │   ├── octuple.c      # 8-directional linked board
│   │   └── stack.c        # Stack implementation
│   └── util/
│       ├── menu.c         # Menu system and user interface
│       └── storage.c      # File I/O and persistence
├── include/               # Header files
├── build/                 # Compilation output
├── gamedata/              # Game saves, replays, and scores
├── LICENSE
├── Makefile
└── README.md
```

## 🤖 AI Implementation

The AI system uses several sophisticated techniques:

### Minimax Algorithm
- **Depth-limited search** with configurable depth
- **Alpha-beta pruning** for performance optimization
- **Position evaluation** using multiple heuristics

### Evaluation Criteria
- **Corner Control**: Corners are highly valued (immovable positions)
- **Mobility**: Number of available moves
- **Piece Difference**: Material advantage
- **Positional Value**: Strategic board positions

### Tree Reuse
- **Persistent game tree** between moves
- **Tree reconnection** after opponent moves
- **Memory optimization** through selective tree pruning

## 💾 Save System

### Game State Persistence
- **Board state**: Complete 8×8 board configuration
- **Player information**: Types and current turn
- **Move history**: Full undo/redo stack
- **Game metadata**: Current player, scores

### Replay System
- **Move recording**: Complete game history
- **Playback controls**: Step through moves
- **File management**: Save, load, and delete replays

## 🏆 Scoring System

### Difficulty Multipliers
- **Human**: 1.0x base score
- **Easy AI**: 1.2x multiplier
- **Medium AI**: 1.5x multiplier
- **Hard AI**: 2.0x multiplier

### Leaderboard
- **Top 10 tracking**: Best scores preserved
- **Name entry**: 3-character player identification
- **Automatic sorting**: Highest scores first

## 🔧 Build Options

### Standard Build
```bash
make                # Build the project
make clean          # Clean build files
make rebuild        # Clean and rebuild
make run           # Build and run
```

### Debug Build
```bash
make CFLAGS="-Wall -Wextra -std=c99 -g -DDEBUG"
```

### Release Build
```bash
make CFLAGS="-Wall -Wextra -std=c99 -O3 -DNDEBUG"
```

## 🐛 Troubleshooting

### Common Issues

**Build Errors:**
- Ensure GCC and Make are properly installed
- Check that all source files are present
- Verify include paths in Makefile

**Runtime Issues:**
- Create gamedata directory if missing
- Check file permissions for save/load operations
- Ensure terminal supports ANSI escape sequences

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👥 Authors

- **Team Members**: Multiple contributors to different components
- **AI Implementation**: Advanced minimax with optimization
- **Game Engine**: Complete Othello rule implementation
- **UI/UX**: Terminal-based interactive interface

---

**Enjoy playing Othello!** 🎲
