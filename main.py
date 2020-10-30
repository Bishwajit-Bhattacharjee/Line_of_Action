import os
import pygame
os.environ["DISPLAY"] = ":0.0"

from LOA.game import Game
from LOA.constants import WIDTH, HEIGHT
from LOA.constants import *

FPS = 60
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Line of Action')

def get_board_pos(pos):
    x, y = pos
    return y // SQ_SIZE, x // SQ_SIZE

def main():    
    run = True
    clock = pygame.time.Clock()
    game = Game(WIN)
    
    while run:
        clock.tick(FPS)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                row, col = get_board_pos(pos)
                print("selected {} {}".format(row, col))
                game.selection_handler(row, col)

        game.update()
    
if __name__ == '__main__':
    main()