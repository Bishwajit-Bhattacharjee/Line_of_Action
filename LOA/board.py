import pygame

from .constants import *
from .checker import Checker

class Board:
    dir_arr = [(0, 1), (0, -1), (-1, 0), (1, 0), (1, 1), (-1, 1),(1, -1), (-1, -1)]

    def __init__(self):
        self.board = []
        self.white_left = self.black_left = ROW - 2
        self.create_board()

    def is_inside(self, row, col):
        return 0 <= row < ROW and 0 <= col < COL

    def get_upto_border(self, checker, dir):
        cnt = 0
        x, y = checker.row, checker.col

        while self.is_inside(x + dir[0], y + dir[1]) :
            x += dir[0]
            y += dir[1]
            cnt += (self.board[x][y] is not None)
        return cnt
       

    def num_checker_in_a_line(self, checker, dir):
        neg_dir = (-dir[0], -dir[1])

        return self.get_upto_border(checker, dir) + \
            self.get_upto_border(checker, neg_dir) + 1


    def can_go_in_this_dir(self, checker, dir):
        # return None if not possible,
        # otherwise return the pos
        move_cnt = self.num_checker_in_a_line(checker, dir)
        print("dir : {} , cnt : {}".format(dir, move_cnt))
        itr = move_cnt
        x, y = checker.row , checker.col
        # check n - 1 adjacent moves
        while itr > 1:
            x, y = x + dir[0], y + dir[1]
            
            if not self.is_inside(x, y):
                return None
            else :
                is_opponent = self.board[x][y] is not None and \
                    self.board[x][y].color != checker.color

                if is_opponent:
                    return None
        
            itr -= 1

        assert(itr == 1)

        x, y = x + dir[0], y + dir[1]

        if not self.is_inside(x, y):
            return None
        if self.board[x][y] is None : 
            return (x, y)
        if self.board[x][y].color == checker.color: # cannot sit on my guti 
            return None     
        return (x, y)


        

    def get_valid_moves(self, checker):
        valid_moves = {}

        for dir in self.dir_arr :
            whr = self.can_go_in_this_dir(checker, dir) 
            if whr is not None:
                valid_moves[whr] = 1

        return valid_moves

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

        self.board[row][col] = None
        x, y = piece.row, piece.col
        self.board[x][y] , self.board[row][col] = self.board[row][col], self.board[x][y]
        piece.move(row, col)


    def get_checker(self, row, col):
        return self.board[row][col]