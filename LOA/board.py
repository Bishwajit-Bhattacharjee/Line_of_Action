import pygame

from .constants import ROW, COL, BLACK, WHITE, YELLOW_BACK, YELLOW_TOP, SQ_SIZE
from .checker import Checker

class Board:
    
    def __init__(self):
        self.board = []
        self.white_left = self.black_left = ROW - 2
        self.create_board()

    def draw(self, surf):
        self.draw_grids(surf)

        for cur_row in self.board:
            for checker in cur_row :
                if checker is not None:
                    checker.draw(surf)

    def draw_grids(self, surf):
        colors = [YELLOW_BACK, YELLOW_TOP]
        surf.fill(BLACK)

        for row in range(ROW):
            for col in range(COL):
                cur_col = colors[(col + row) % 2]
                pygame.draw.rect(surf, \
                    cur_col, \
                        (row * SQ_SIZE, col * SQ_SIZE, SQ_SIZE, SQ_SIZE)
                )
                
    def create_board(self):

        for row in range(ROW):
            self.board.append([])
            for col in range(COL):
                if row == 0 or row + 1 == ROW: # BLACK ROWS
                    if col > 0 and col < COL - 1: 
                        # first and last cols are empty
                        self.board[row].append(Checker(row, col, BLACK))
                    else :
                        self.board[row].append(None)
                else: # every other rows contain white
                    if col == 0 or col + 1 == COL: 
                        self.board[row].append(Checker(row, col, WHITE))
                    else : 
                        self.board[row].append(None)


    def move(self, piece, row, col):
        x, y = piece.row, piece.col
        self.board[x][y] , self.board[row][col] = self.board[row][col], self.board[x][y]
        piece.move(row, col)


    def get_checker(self, row, col):
        return self.board[row][col]