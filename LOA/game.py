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
        pygame.display.update()


    def selection_handler(self, row, col):
        if self.selected is not None:
            self._move(row, col)
            self.selected = None
            return
        
        checker = self.board.get_checker(row, col)
        
        if checker is not None and checker.color == self.turn : 
            self.selected = checker
            self.valid_moves = self.board.get_valid_moves(row, col)
        

    def _move(self, row, col):
        self.board.move(self.selected, row, col)
        self._rotate_move()


    def _rotate_move(self):
        if self.turn == BLACK :
            self.turn = WHITE
        else :
            self.turn = BLACK
        

