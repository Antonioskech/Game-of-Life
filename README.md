# Game of Life Project

## What is the Game of Life?
The **Game of Life**, also known simply as **Life**, was created by British mathematician **John Horton Conway** in 1970.

The universe of the Game of Life is an **infinite, two-dimensional, orthogonal grid** of square cells, where each cell is either **alive** or **dead** (also referred to as **populated** and **unpopulated**).  
Each cell interacts with its **eight neighbors** — the cells that are horizontally, vertically, or diagonally adjacent.

At each step of time, the following transitions occur:
- A **live** cell with **fewer than two** live neighbors **dies** (underpopulation).
- A **live** cell with **two or three** live neighbors **stays alive** (stable population).
- A **live** cell with **more than three** live neighbors **dies** (overpopulation).
- A **dead** cell with **exactly three** live neighbors becomes **alive** (reproduction).

---

## Game Features

1. **New Game**  
   Starts a new game with an empty grid of size **16×8**.

2. **Load File**  
   Prompts the user for a filename, then attempts to load a grid from it.  
   If successful, a new game starts with the loaded data.

3. **Step Forward**  
   Simulates a single step in the game and displays the updated grid along with the game menu.

4. **Resize Grid**  
   Displays the current grid size and prompts the user for new dimensions **X** and **Y**.  
   - If the grid shrinks, cells outside the new dimensions are lost.  
   - If it grows, new cells are initialized as dead.

5. **Toggle Cell**  
   Prompts the user for coordinates and toggles the state (alive/dead) of the specified cell.  
   - Optionally, if the coordinates are out of bounds (negative or too large), the grid can automatically expand following the resizing rules.

6. **Clear Grid**  
   Sets all cells to dead and displays the cleared grid along with the menu.

7. **Randomize Grid**  
   Prompts the user for a number **N** and randomizes the grid:  
   - Most cells become dead.  
   - Each cell has a 1-in-N chance of being alive.  
   (If **N=0**, all cells are dead. If **N=1**, all cells are alive.)

8. **Save to File**  
   Prompts the user for a filename and saves the current grid state into that file.

9. **End Game**  
   Exits the game and returns to the main menu.

---

## Example Screenshot

![Game of Life Example](https://github.com/Antonioskech/Game-of-Life/assets/147728161/0193cf0e-aed3-4c41-a0db-aa11d362bda8)




