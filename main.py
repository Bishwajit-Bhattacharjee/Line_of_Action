import os, sys
import pygame
os.environ["DISPLAY"] = ":0.0"

from LOA.game import Game
from LOA.constants import *

FPS = 60
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Line of Action')



def get_board_pos(pos):
    x, y = pos
    return y // SQ_SIZE, x // SQ_SIZE

def main(my_id):    

    cur_player = 1        
    run = True
    clock = pygame.time.Clock()
    game = Game(WIN)
    

    while run:
        clock.tick(FPS)

        winner = game.winner()
        if winner is not None:
            print(str(winner) + " won the match")
            run = False
            break

        if cur_player != my_id:
            # ask for move
            print("{}".format(cur_player), flush = True)

            # take input from the console
            fx, fy, tx, ty = map( input().split(), int) 
            
            game.ai_make_move(fx, fy, tx, ty)
            
            cur_player = my_id

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False
            
        else :

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False
                if event.type == pygame.MOUSEBUTTONDOWN:
                    pos = pygame.mouse.get_pos()
                    row, col = get_board_pos(pos)
                    move_completed = game.selection_handler(row, col)
                    if move_completed is not None: 
                        fx, fy, tx, ty = move_completed
                        print("{} {} {} {} {}".format(my_id, fx, fy, tx, ty), file = sys.stderr ,flush = True)
                        print("{} {} {} {} {}".format(my_id, fx, fy, tx, ty), flush = True)

                        if my_id == 1: cur_player = 2
                        else : cur_player = 1
                
        game.update()
    
if __name__ == '__main__':

    starting_player = int(sys.argv[1])    
    print("{}".format(starting_player), file = sys.stderr, flush = True)
    main(starting_player)