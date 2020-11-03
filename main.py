import os, sys
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = "hide"
os.environ["DISPLAY"] = ":0.0"

import pygame

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
    has_won = False 

    while run:
        clock.tick(FPS)

        winner = game.winner()

        if winner is not None:
            print(str(winner) + " won the match")
            has_won = True

        if not has_won and cur_player != my_id:
            # ask for move
            print(cur_player, flush = True)

            # take input from the console
            fx = int(input())
            fy = int(input())
            tx = int(input())
            ty = int(input())
            
            game.ai_make_move(fx, fy, tx, ty)
            
            cur_player = my_id

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False
            
        elif not has_won :

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
                        print(my_id, flush = True)
                        print(fx, flush = True)
                        print(fy, flush = True)
                        print(tx, flush = True)
                        print(ty, flush = True)

                        if my_id == 1: cur_player = 2
                        else : cur_player = 1

        if has_won :
            while True : 
                game.update()                
        game.update()
    
if __name__ == '__main__':

    starting_player = int(sys.argv[1])   
    print("", end="", flush= True)
    print("{}".format(starting_player), file = sys.stderr, flush = True)
    main(starting_player)