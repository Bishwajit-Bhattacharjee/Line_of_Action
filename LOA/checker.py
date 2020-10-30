import pygame

from .constants import *

class Checker:
    RADIUS = SQ_SIZE // 2 - 15 
    OUTLINE = 2

    def __init__(self, row, col, color):
        self.row = row
        self.col = col
        self.color = color
        self.x = 0
        self.y = 0
        self.calc_cord()
    
    def calc_cord(self):
        self.x = SQ_SIZE * self.col + SQ_SIZE // 2
        self.y = SQ_SIZE * self.row + SQ_SIZE // 2
    

    def draw(self, surf):        
        pygame.draw.circle(surf, BLACK , (self.x, self.y), self.RADIUS + self.OUTLINE)
        pygame.draw.circle(surf, self.color , (self.x, self.y), self.RADIUS)

    def move(self,row, col):
        self.row = row 
        self.col = col
        self.calc_cord()
