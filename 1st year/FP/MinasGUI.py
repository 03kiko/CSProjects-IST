from P2 import *
import pygame
import sys
import random

pygame.init()
pygame.display.set_caption('Minesweeper')

# Colors
black = (0, 0, 0)
white = (255, 255, 255)
colors = {str(i + 1): color for i, color in enumerate([(255, 0, 0), (0, 255, 0), (0, 0, 255),
                                              (255, 255, 0), (255, 0, 255), (0, 255, 255), (255, 127, 0), (255, 0, 127), (127, 0, 255)])}

# minesweeper stuff
letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
max_col, max_line = 'R', 14
gerador = cria_gerador(32, random.randint(0, 1000000))
minas = 40

# window, screen, grid sizes
height = 680
cell_size = (height - 200) // (max_line + 1)
size = width, height = cell_size*(ord(max_col) - ord('A') + 2), (max_line + 1)*cell_size + 200
grid = (width // cell_size, (height - 200) // cell_size)
cell_count = (grid[0] - 1) * (grid[1] - 1)
for i in range(grid[1]):
    num = str(i + 1)
    if num not in colors:
        colors[num] = colors[str((i % 9) + 1)]

screen = pygame.display.set_mode(size)
font = pygame.font.SysFont("Arial", cell_size - 8)
big_font = pygame.font.SysFont("Arial", 72, bold=True)

states = []


for i in range(grid[0]-1):
    colors[letters[i]] = colors[str((i % 9) + 1)]


def draw_text(screen, text, center):
    text_rect = text.get_rect()
    text_rect.center = center
    screen.blit(text, text_rect)


def draw_char_in_cell(screen, char, cell):
    center = (cell[0] * cell_size + cell_size // 2,
              cell[1] * cell_size + cell_size // 2)
    color = colors.get(char) or white
    draw_text(screen, font.render(char, True, color), center)


class State:
    def __init__(self, screen):
        self.screen = screen

    def handle_event(self, event):
        pass

    def draw(self):
        pass


class End(State):
    def __init__(self, screen, mensagem):
        super().__init__(screen)
        self.mensagem = mensagem

    def handle_event(self, event):
        if event.type == pygame.KEYDOWN:
            states.clear()
            states.append(Game(self.screen))

    def draw(self):
        pygame.draw.rect(self.screen, black, (0, height // 2 - 90, width, 200))

        text = big_font.render(self.mensagem, True, white)
        draw_text(self.screen, text, (width // 2, height // 2))

        text = font.render(
            "Pressiona qualquer tecla para recomeçar", True, white)
        draw_text(self.screen, text, (width // 2, height // 2 + 50))


class Game(State):
    def __init__(self, screen):
        super().__init__(screen)
        self.campo = cria_campo(max_col, max_line)
        self.colocadas = False
        self.marcadas = 0

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            x //= cell_size
            y //= cell_size

            if x == 0 or y == 0:
                return

            coord = cria_coordenada(letters[x - 1], y)

            if self.colocadas:
                if event.button == 1:
                    if not turno_jogador_gamepy(self.campo, 'L' + '\n' + coordenada_para_str(coord) + '\n'):
                        states.append(End(self.screen, "Perdeste! ;("))
                elif event.button == 3:
                    turno_jogador_gamepy(self.campo, 'M' + '\n' + coordenada_para_str(coord) + '\n')
                    self.marcadas  = len(obtem_coordenadas(self.campo, 'marcadas'))
            else:
                coloca_minas(self.campo, coord, gerador, minas)
                turno_jogador_gamepy(self.campo, 'L' + '\n' + coordenada_para_str(coord) + '\n')
                self.colocadas = True

            if jogo_ganho(self.campo):
                states.append(End(self.screen, "Vitória!!!!!!!!"))

    def draw(self):
        # draw a grid
        for x in range(1, grid[0]):
            pygame.draw.line(self.screen, white,
                             (x * cell_size, cell_size), (x * cell_size, height-200))
            pygame.draw.line(self.screen, white, ((x + 1) *
                             cell_size - 1, cell_size), ((x + 1) * cell_size - 1, height-200))
            if x != 0:
                draw_char_in_cell(self.screen, letters[x - 1], (x, 0))

        for y in range(1, grid[1]):
            pygame.draw.line(self.screen, white,
                             (cell_size, y * cell_size), (width, y * cell_size))
            pygame.draw.line(self.screen, white, (cell_size, (y + 1)
                             * cell_size - 1), (width, (y + 1) * cell_size - 1))
            if y != 0:
                draw_char_in_cell(self.screen, str(y), (0, y))


        # draw the field    
        campo_str = campo_para_str(self.campo).split('\n')
        for y in range(2, len(campo_str)-1):
            for x in range(3, len(campo_str[y])-1):
                if campo_str[y][x] != ' ':
                    draw_char_in_cell(self.screen, campo_str[y][x], (x-2, y-1))
        
        # draw the flags
        text = big_font.render(f"Bandeiras: {self.marcadas}/{minas}", True, white)
        draw_text(self.screen, text, (width // 2, 580))


class Menu(State):
    def handle_event(self, event):
        if event.type == pygame.KEYDOWN:
            states.pop()
            states.append(Game(self.screen))

    def draw(self):
        text = big_font.render("Minesweeper", True, white)
        draw_text(self.screen, text, (width // 2, height // 2))

        text = font.render(
            "Pressiona qualquer tecla para começar", True, white)
        draw_text(self.screen, text, (width // 2, height // 2 + 50))


### AUXILIAR CODE NECESSARY TO REPLACE STANDARD INPUT 
class ReplaceStdIn:
    def __init__(self, input_handle):
        self.input = input_handle.split('\n')
        self.line = 0

    def readline(self):
        if len(self.input) == self.line:
            return ''
        result = self.input[self.line]
        self.line += 1
        return result

class ReplaceStdOut:
    def __init__(self):
        self.output = ''

    def write(self, s):
        self.output += s
        return len(s)

    def flush(self):
        return 


def turno_jogador_gamepy(mapa, input_jogo):
    oldstdin = sys.stdin
    sys.stdin = ReplaceStdIn(input_handle=input_jogo)
    
    oldstdout, newstdout = sys.stdout,  ReplaceStdOut()
    sys.stdout = newstdout

    try:
        res = turno_jogador(mapa)
        # text = newstdout.output
        return res #, text
    except ValueError as e:
        raise e
    finally:
        sys.stdin = oldstdin
        sys.stdout = oldstdout


## START GAME
states.append(Menu(screen))

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit(0)
        elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            pygame.quit()
            sys.exit(0)
        states[-1].handle_event(event)

    screen.fill(black)
    for state in states:
        state.draw()
    pygame.display.flip()
