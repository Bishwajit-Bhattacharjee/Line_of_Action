import pygame

from .board import Board
from .constants import *

class Game:

    def __init__(self, surf):
        self.surf = surf
        self.board = Board()
        self.selected = None
        self.valid_moves = {}
        self.turn = BLACK

    def update(self):
        self.board.draw(self.surf)
        self.draw_valid_moves()
        pygame.display.update()


    def draw_valid_moves(self):
        if self.selected is None : 
            return

        x, y = self.selected.x, self.selected.y

        for move in self.valid_moves:
            row, col = move
            x1, y1 = \
                    col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE//2

            pygame.draw.line(self.surf, BLUE, 
                (x1, y1), (x, y), 5
            )


    def selection_handler(self, row, col):
        if self.selected is not None:
            moved = self._move(row, col)
            self.selected = None
            self.valid_moves = {}
            return
        
        checker = self.board.get_checker(row, col)
        
        if checker is not None and checker.color == self.turn : 
            self.selected = checker
            self.valid_moves = self.board.get_valid_moves(checker)
        

    def _move(self, row, col):

        if (row, col) in self.valid_moves:
            self.board.move(self.selected,row , col)
            self._rotate_move()
            return True
        else:
            return False 


    def _rotate_move(self):
        if self.turn == BLACK :
            self.turn = WHITE
        else :
            self.turn = BLACK
        

