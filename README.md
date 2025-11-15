This is a connect 4 app made using c++ and QT. Originally designed using QT Creator.
Version update with working gui and comments for missing logic
-Moved game logic from game.h to game.cpp for highlighting cells
-Removed 104 event handlers to create a smart slot that parses the grid for array location using regex
-Check if parsed data is found and grabs column
-Have onGridCellClicked automatically call DropInColumn
-Create argument for HighlightCell to find button name and set color
-Changed board initialization from (8,14) to (8,13)
-Added third argument to dropPiece to return the row piece dropped to
