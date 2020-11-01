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
    

    def set_turn(self, turn):
        self.turn = turn    

    def update(self):
        self.board.draw(self.surf)
        self.draw_valid_moves()
        pygame.display.update()

    def winner(self):
        return self.board.winner(self.turn)


    def draw_valid_moves(self):
        if self.selected is None : 
            return

        x, y = self.selected.x, self.selected.y

        for move in self.valid_moves:
            row, col = move
            x1, y1 = \
                    col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE//2

            pygame.draw.line(self.surf, GREEN, 
                (x1, y1), (x, y), 4
            )
            pygame.draw.circle(self.surf, GREEN, (x1, y1), 10)
            pygame.draw.circle(self.surf, GREEN, (x, y), 10)


    def selection_handler(self, row, col):
        if self.selected is not None:
            moved = self._move(row, col)
            self.selected = None
            self.valid_moves = {}
            return moved
        
        checker = self.board.get_checker(row, col)
        
        if checker is not None and checker.color == self.turn : 
            self.selected = checker
            self.valid_moves = self.board.get_valid_moves(checker)

        return None

    def _move(self, row, col):

        if (row, col) in self.valid_moves:
            fx, fy = self.selected.row, self.selected.col
            self.board.move(fx, fy, row ,col)
            self._rotate_move()
            return (fx, fy, row, col) 
        return None

    def ai_make_move(self, fx, fy, tx, ty):
        
        from_checker = self.board.get_checker(fx, fy)
        assert(from_checker is not None and from_checker.color == self.turn)
        to_checker = self.board.get_checker(tx, ty)
        assert(to_checker is None or to_checker.color != self.turn)
                        
        self.board.move(fx, fy, tx, ty)        
        self._rotate_move()        

    def _rotate_move(self):
        if self.turn == BLACK :
            self.turn = WHITE
        else :
            self.turn = BLACK
        

