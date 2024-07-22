import sys
import random

from search import (
    Problem,
    Node,
    astar_search,
    breadth_first_tree_search,
    depth_first_tree_search,
    greedy_search,
    recursive_best_first_search,
)

from numpy import (
    ndarray,
    array
)
from copy import deepcopy

class PipeManiaState:
    state_id = 0

    def __init__(self, board, connections_left, p_locked):
        self.board = board
        self.left = connections_left
        self.locked = p_locked
        self.id = PipeManiaState.state_id
        PipeManiaState.state_id += 1
        self.curr_coord = (0, 0)
        self.neighbour_locked = (set(), set(), set(), set()) # 0, 1, 2, 3 locked
        

    def __lt__(self, other):
        return self.id < other.id

    def no_solution(self):
        for s in self.neighbour_locked:
            s.clear()

    def go_to_next_coord(self):
        neighbour_locked = self.neighbour_locked
        for i in range(3, -1, -1): # picks one of the coords with the maximum neighbours locked
            if(len(neighbour_locked[i]) != 0):
                self.curr_coord = self.neighbour_locked[i].pop()
                break

    def reached_end(self) -> bool:
        return len(self.neighbour_locked[0]) == 0 and \
            len(self.neighbour_locked[1])  == 0 and \
            len(self.neighbour_locked[2]) == 0 and \
            len(self.neighbour_locked[3]) == 0 
    
    def deepcopy(self):
        copy = PipeManiaState(deepcopy(self.board), self.left, self.locked)
        copy.curr_coord = deepcopy(self.curr_coord)
        copy.neighbour_locked = deepcopy(self.neighbour_locked)
        return copy

    def point_and_lock(self, row: int, col: int, direction):
        def go_to_next_set(element):
            for j in range(3):
                if(element in self.neighbour_locked[j]):
                    self.neighbour_locked[j].remove(element)
                    self.neighbour_locked[j + 1].add(element)
                    break
        
        conns_left_before = self.board.connections_left_in_neighbourhood(row, col)
        self.board.set_value(row, col, Piece.lock(Piece.point(self.board.get_value(row, col), direction)))
        self.left += self.board.connections_left_in_neighbourhood(row, col) - conns_left_before
        self.locked += 1
        
        for i in range(4):
            if((row, col) in self.neighbour_locked[i]):
                self.neighbour_locked[i].remove((row, col))
                break

        neighbour_info = self.board.get_neighbour_info(row, col)
        empty = neighbour_info[2]
        locked = neighbour_info[1]
        offsets = (1, 0, 0, -1, -1, 0, 0, 1)
        mask = 0xF000
        for i in range(0, 4):
            if((empty & mask != 0) or (locked & mask)): #if empty/locked continue
                mask >>= 4
                continue
            go_to_next_set((row + offsets[2 * i], col + offsets[2 * i + 1]))
            mask >>= 4

    def point_and_lock_test(self, row: int, col: int, direction):
        self.board.set_value(row, col, Piece.lock(Piece.point(self.board.get_value(row, col), direction)))
    
    def infer(self, row: int, col: int) -> list:
        piece = self.board.get_value(row, col)
        info = self.board.get_neighbour_info(row, col)
        return Piece.infer(piece, info[0], info[1], info[2])

    def bfs_lock_processing(self, row: int, col: int):
        queue = [(row, col)]
        while queue:
            curr = queue.pop(0)
            piece = self.board.get_value(curr[0], curr[1])
            if (Piece.is_locked(piece)):
                continue
            info = self.board.get_neighbour_info(curr[0], curr[1])
            alignments = Piece.infer(piece, info[0], info[1], info[2])
            if (len(alignments) == 1):
                self.point_and_lock(curr[0], curr[1], alignments[0])
                if (curr[0] > 0):
                    queue.append((curr[0] - 1, curr[1]))
                if (curr[0] < self.board.get_height() - 1):
                    queue.append((curr[0] + 1, curr[1]))
                if (curr[1] > 0):
                    queue.append((curr[0], curr[1] - 1))
                if (curr[1] < self.board.get_width() - 1):
                    queue.append((curr[0], curr[1] + 1))
            elif (len(alignments) == 0):
                return True
        return False

    def dfs_lock_processing(self, row: int, col: int):
        stack = [(row, col)]
        while stack:
            curr = stack.pop()
            piece = self.board.get_value(curr[0], curr[1])
            if (Piece.is_locked(piece)):
                continue
            info = self.board.get_neighbour_info(curr[0], curr[1])
            alignments = Piece.infer(piece, info[0], info[1], info[2])
            if (len(alignments) == 1):
                self.point_and_lock(curr[0], curr[1], alignments[0])
                if (curr[0] > 0):
                    stack.append((curr[0] - 1, curr[1]))
                if (curr[0] < self.board.get_height() - 1):
                    stack.append((curr[0] + 1, curr[1]))
                if (curr[1] > 0):
                    stack.append((curr[0], curr[1] - 1))
                if (curr[1] < self.board.get_width() - 1):
                    stack.append((curr[0], curr[1] + 1))
            elif (len(alignments) == 0):
                return True
        return False

    def neighbourhood_bfs_lock_processing(self, row: int, col: int):
        queue = []
        if (row > 0):
            queue.append((row - 1, col))
        if (row < self.board.get_height() - 1):
            queue.append((row + 1, col))
        if (col > 0):
            queue.append((row, col - 1))
        if (col < self.board.get_width() - 1):
            queue.append((row, col + 1))
        while queue:
            curr = queue.pop(0)
            piece = self.board.get_value(curr[0], curr[1])
            if (Piece.is_locked(piece)):
                continue
            info = self.board.get_neighbour_info(curr[0], curr[1])
            alignments = Piece.infer(piece, info[0], info[1], info[2])
            if (len(alignments) == 1):
                self.point_and_lock(curr[0], curr[1], alignments[0])
                if (curr[0] > 0):
                    queue.append((curr[0] - 1, curr[1]))
                if (curr[0] < self.board.get_height() - 1):
                    queue.append((curr[0] + 1, curr[1]))
                if (curr[1] > 0):
                    queue.append((curr[0], curr[1] - 1))
                if (curr[1] < self.board.get_width() - 1):
                    queue.append((curr[0], curr[1] + 1))
            elif (len(alignments) == 0):
                return True
        return False

    def neighbourhood_dfs_lock_processing(self, row: int, col: int):
        stack = []
        if (row > 0):
            stack.append((row - 1, col))
        if (row < self.board.get_height() - 1):
            stack.append((row + 1, col))
        if (col > 0):
            stack.append((row, col - 1))
        if (col < self.board.get_width() - 1):
            stack.append((row, col + 1))
        while stack:
            curr = stack.pop()
            piece = self.board.get_value(curr[0], curr[1])
            if (Piece.is_locked(piece)):
                continue
            info = self.board.get_neighbour_info(curr[0], curr[1])
            alignments = Piece.infer(piece, info[0], info[1], info[2])
            if (len(alignments) == 1):
                self.point_and_lock(curr[0], curr[1], alignments[0])
                if (curr[0] > 0):
                    stack.append((curr[0] - 1, curr[1]))
                if (curr[0] < self.board.get_height() - 1):
                    stack.append((curr[0] + 1, curr[1]))
                if (curr[1] > 0):
                    stack.append((curr[0], curr[1] - 1))
                if (curr[1] < self.board.get_width() - 1):
                    stack.append((curr[0], curr[1] + 1))
            elif (len(alignments) == 0):
                return True
        return False

    def preprocess(self):
        for j in range(0, self.board.get_width()):
            self.bfs_lock_processing(0, j)
        for i in range(0, self.board.get_height()):
            self.bfs_lock_processing(i, self.board.get_width() - 1)
        for j in range(self.board.get_width() - 1, -1, -1):
            self.bfs_lock_processing(self.board.get_height() - 1, j)
        for i in range(self.board.get_height() - 1, -1, -1):
            self.bfs_lock_processing(i, 0)

    @staticmethod
    def parse_instance():
        state = PipeManiaState(Board.parse_instance(), 0, 0)
        for i in range(0, state.board.get_height()):
            for j in range(0, state.board.get_width()):
                state.left += Piece.connections_left(state.board.get_value(i, j), state.board.get_neighbour_orientations(i, j))
                if(i == 0 or i == state.board.get_height() - 1):
                    if(j == 0 or j == state.board.get_width() - 1):
                        state.neighbour_locked[2].add((i, j))
                    else:
                        state.neighbour_locked[1].add((i, j))  
                elif(j == 0 or j == state.board.get_width() - 1):
                    state.neighbour_locked[1].add((i, j)) 
                else:
                    state.neighbour_locked[0].add((i, j))
        return state


class Piece:
    # Each piece follows the following convention:
    # 0b 0 0 00 00 00
    # The groups from lo to hi are:
    # 1. Orientation id (0 -> right, 1 -> up, 2 -> left, 3 -> down)
    # 2. Type id (0 -> F, 1 -> B, 2 -> V, 3 -> L)
    # 3. Free bits (not in use)
    # 4. Is locked (0 -> no, 1 -> yes)
    # 5. Is marked (0 -> no, 1 -> yes)

    lt_ori = None
    lt_inv_ori = None
    lt_locked = None

    def invert_orientation(orientation):
        return (orientation >> 2) | ((orientation << 2) & 0b1100)

    @staticmethod
    def init():
        Piece.lt_ori = array(((0b0001, 0b0010, 0b0100, 0b1000),
           (0b1011, 0b0111, 0b1110, 0b1101),
           (0b0011, 0b0110, 0b1100, 0b1001),
           (0b0101, 0b1010, 0b0101, 0b1010)))
        Piece.lt_inv_ori = array(tuple(tuple(Piece.invert_orientation(x) \
                            for x in piece_ori) for piece_ori in Piece.lt_ori))
        Piece.lt_locked = (0b0000, 0b1111)

    def get_type(piece):
        return (piece & 0b1100) >> 2

    def get_orientation(piece):
        return Piece.lt_ori[(piece & 0b1100) >> 2][(piece & 0b0011)]

    def get_inv_orientation(piece):
        return Piece.lt_inv_ori[(piece & 0b1100) >> 2][(piece & 0b0011)]
    
    def is_F(piece) -> bool:
        return Piece.get_type(piece) == 0b00
    
    def is_B(piece) -> bool:
        return Piece.get_type(piece) == 0b01

    def is_V(piece) -> bool:
        return Piece.get_type(piece) == 0b10

    def is_L(piece) -> bool:
        return Piece.get_type(piece) == 0b11
    
    def is_pointing(piece, direction):
        return (piece & 0b0011) == direction
    
    def lock_status(piece):
        return Piece.lt_locked[(piece & 0b01000000) >> 6]
    
    def is_locked(piece):
        return (piece & 0b01000000) != 0
    
    def is_unlocked(piece):
        return (piece & 0b01000000) == 0
    
    def lock(piece):
        return piece | 0b01000000
    
    def unlock(piece):
        return piece & 0b10111111

    def mark(piece):
        return piece | 0b10000000
    
    def unmark(piece):
        return piece & 0b01111111
    
    def is_marked(piece):
        return (piece & 0b10000000) != 0
    
    def right():
        return 0

    def up():
        return 1

    def left():
        return 2

    def down():
        return 3

    def point(piece, direction):
        return (piece & 0b11111100) | (direction & 0b11)

    def connections_needed(orientation):
        total = orientation & 0b1 # Right
        total += (orientation >> 1) & 0b1 # Left
        total += (orientation >> 2) & 0b1 # Up
        total += (orientation >> 3) & 0b1 # Down
        return total
    
    def connections_done(piece, orientations):
        ori = Piece.get_orientation(piece)
        total = (ori & 0b1) & orientations
        total += ((ori >> 1) & 0b1) & (orientations >> 5)
        total += ((ori >> 2) & 0b1) & (orientations >> 10)
        total += ((ori >> 3) & 0b1) & (orientations >> 15)
        return total
    
    def connections_left(piece, orientations):
        return Piece.connections_needed(Piece.get_orientation(piece)) - \
            Piece.connections_done(piece, orientations)
    
    def infer(piece, neighbour_orientations, locked, empty) ->list:
        def bit_to_4bits(bit):
            bit = bit & 0b1
            return bit | (bit << 1) | (bit << 2) | (bit << 3)
        
        def grab_from_neighbours(neighbours, direction):
            return (neighbours >> (direction * 4)) & 0xF

        if (Piece.is_F(piece)):
            for direction in range(Piece.right(), Piece.down() + 1):
                if (Piece.connections_needed(grab_from_neighbours(neighbour_orientations, direction)) == 1):
                    empty |= 0xF << (direction * 4)

        to = Piece.down() if (not Piece.is_L(piece)) else Piece.up()
        res = []
        needed = neighbour_orientations & locked & 0b1000010000100001
        not_needed = (neighbour_orientations ^ 0xFFFF) & locked & 0b1000010000100001
        empty |= bit_to_4bits(not_needed)
        empty |= bit_to_4bits(not_needed >> 5) << 4
        empty |= bit_to_4bits(not_needed >> 10) << 8
        empty |= bit_to_4bits(not_needed >> 15) << 12
        for direction in range(Piece.right(), to + 1):
            ori = Piece.get_orientation(Piece.point(piece, direction))
            ori = (ori & 0x1) | (((ori >> 1) & 0b1) << 5) | (((ori >> 2) & 0b1) << 10) \
                | (((ori >> 3) & 0b1) << 15)
            good = (needed & ori) == needed
            good = good and (empty & ori) == 0
            if (good):
                res.append(direction)
        return res

    def parse_piece(piece: str):
        res = 0
        if (piece[0] == 'F'):
            res |= 0b00000000
        elif (piece[0] == 'B'):
            res |= 0b00000100
        elif (piece[0] == 'V'):
            res |= 0b00001000
        elif (piece[0] == 'L'):
            res |= 0b00001100
        else:
            raise RuntimeError(f"No such piece type exists {piece}")
        if (piece[1] == 'D'):
            res = Piece.point(res, Piece.right())
        elif (piece[1] == 'C'):
            res = Piece.point(res, Piece.up())
        elif (piece[1] == 'E'):
            res = Piece.point(res, Piece.left())
        elif (piece[1] == 'B'):
            res = Piece.point(res, Piece.down())
        elif (piece[1] == 'H'):
            res = Piece.point(res, Piece.right())
        elif (piece[1] == 'V'):
            res = Piece.point(res, Piece.up())
        else:
            raise RuntimeError(f"No such piece orientation exists {piece}")
        return res
    
    def to_string(piece):
        def decode_orientation(piece):
            if (Piece.is_pointing(piece, Piece.right())):
                return 'D'
            elif (Piece.is_pointing(piece, Piece.up())):
                return 'C'
            elif (Piece.is_pointing(piece, Piece.left())):
                return 'E'
            elif (Piece.is_pointing(piece, Piece.down())):
                return 'B'
            else:
                raise RuntimeError(f"Error decoding piece orientation {bin(piece)}")
        
        def decode_orientation_L(piece):
            if (Piece.is_pointing(piece, Piece.right()) or Piece.is_pointing(piece, Piece.left())):
                return 'H'
            elif (Piece.is_pointing(piece, Piece.up()) or Piece.is_pointing(piece, Piece.down())):
                return 'V'
            else:
                raise RuntimeError(f"Error decoding piece (L) orientation {bin(piece)}")

        t = '0'
        ori = '0'
        if (Piece.is_F(piece)):
            t = "F"
            ori = decode_orientation(piece)
        elif (Piece.is_B(piece)):
            t = "B"
            ori = decode_orientation(piece)
        elif (Piece.is_V(piece)):
            t = "V"
            ori = decode_orientation(piece)
        elif (Piece.is_L(piece)):
            t = "L"
            ori = decode_orientation_L(piece)
        else:
            raise RuntimeError(f"Error decoding piece type {bin(piece)}")
        return t + ori
    
class Board:
    """Representação interna de um tabuleiro de PipeMania."""
    def __init__(self, board):
        self.board = board
    
    def get_value(self, row: int, col: int):
        """Devolve o valor na respetiva posição do tabuleiro."""
        return self.board[row][col]

    def set_value(self, row: int, col: int, piece):
        self.board[row][col] = piece
    
    def get_neighbour_orientations(self, row: int, col: int):
        ori = 0
        if (row > 0):
            ori |= Piece.invert_orientation(Piece.get_orientation(self.board[row - 1][col])) << 4
        if (row < self.board.shape[0] - 1):
            ori |= Piece.invert_orientation(Piece.get_orientation(self.board[row + 1][col])) << 12
        if (col > 0):
            ori |= Piece.invert_orientation(Piece.get_orientation(self.board[row][col - 1])) << 8
        if (col < self.board.shape[1] - 1):
            ori |= Piece.invert_orientation(Piece.get_orientation(self.board[row][col + 1]))
        return ori
    
    def get_neighbour_info(self, row: int, col: int) -> tuple:
        ori = 0
        locked = 0
        empty = 0xFFFF
        if (row > 0):
            locked |= Piece.lock_status(self.board[row - 1][col]) << 4
            empty &= 0xFF0F
            ori |= Piece.get_inv_orientation(self.board[row - 1][col]) << 4
        if (row < self.board.shape[0] - 1):
            locked |= Piece.lock_status(self.board[row + 1][col]) << 12
            empty &= 0x0FFF
            ori |= Piece.get_inv_orientation(self.board[row + 1][col]) << 12
        if (col > 0):
            locked |= Piece.lock_status(self.board[row][col - 1]) << 8
            empty &= 0xF0FF
            ori |= Piece.get_inv_orientation(self.board[row][col - 1]) << 8
        if (col < self.board.shape[1] - 1):
            locked |= Piece.lock_status(self.board[row][col + 1])
            empty &= 0xFFF0
            ori |= Piece.get_inv_orientation(self.board[row][col + 1])
        return (ori, locked, empty)
    
    def connections_left_in_neighbourhood(self, row: int, col: int) -> int:
        conns_left = 0
        conns_left += Piece.connections_left(self.board[row, col],
                        self.get_neighbour_orientations(row, col))
        if (row - 1 >= 0):
            conns_left += Piece.connections_left(self.board[row - 1][col],
                                self.get_neighbour_orientations(row - 1, col))
        if (row + 1 < self.get_height()):
            conns_left += Piece.connections_left(self.board[row + 1][col], 
                                self.get_neighbour_orientations(row + 1, col))
        if (col - 1 >= 0):
            conns_left += Piece.connections_left(self.board[row][col - 1],
                                self.get_neighbour_orientations(row, col - 1))
        if (col + 1 < self.get_width()):
            conns_left += Piece.connections_left(self.board[row][col + 1],
                                self.get_neighbour_orientations(row, col + 1))
        return conns_left

    def get_width(self):
        return self.board.shape[1]

    def get_height(self):
        return self.board.shape[0]

    def print_board(self):
        for i in range(0, self.get_height()):
            for j in range(0, self.get_width() - 1):
                print(Piece.to_string(self.board[i][j]), end = "\t")
            print(Piece.to_string(self.board[i][self.get_width() - 1]), end = "\n")
    
    def print_board_done(self):
        for i in range(0, self.get_height()):
            for j in range(0, self.get_width() - 1):
                print(Piece.connections_done(self.get_value(i, j), self.get_neighbour_orientations(i, j)), end=" ")
            print(Piece.connections_done(self.get_value(i, self.get_width() - 1), self.get_neighbour_orientations(i, self.get_width() - 1)))

    def print_board_left(self):
        for i in range(0, self.get_height()):
            for j in range(0, self.get_width() - 1):
                print(Piece.connections_left(self.get_value(i, j), self.get_neighbour_orientations(i, j)), end=" ")
            print(Piece.connections_left(self.get_value(i, self.get_width() - 1), self.get_neighbour_orientations(i, self.get_width() - 1)))
    
    def print_board_locked(self):
        for i in range(0, self.get_height()):
            for j in range(0, self.get_width() - 1):
                print(Piece.is_locked(self.get_value(i, j)), end=" ")
            print(Piece.is_locked(self.get_value(i, self.get_width() - 1)))

    @staticmethod
    def parse_instance():
        """Lê o test do standard input (stdin) que é passado como argumento
        e retorna uma instância da classe Board."""
        read = []
        for line in sys.stdin:
            read.append(list(map(Piece.parse_piece, line.split())))
        return Board(array(read, dtype = 'uint8'))

class PipeMania(Problem):
    def __init__(self, initial: PipeManiaState):
        """O construtor especifica o estado inicial."""
        initial.preprocess()
        super().__init__(initial)

    def actions(self, state: PipeManiaState):
        """Retorna uma lista de ações que podem ser executadas a
        partir do estado passado como argumento."""
        if (state.reached_end()):
            return []
        return state.infer(state.curr_coord[0], state.curr_coord[1])
        
    def result(self, state: PipeManiaState, action):
        """Retorna o estado resultante de executar a 'action' sobre
        'state' passado como argumento. A ação a executar deve ser uma
        das presentes na lista obtida pela execução de
        self.actions(state)."""
        copy = state.deepcopy()
        copy.point_and_lock(copy.curr_coord[0], copy.curr_coord[1], action)
        if (copy.neighbourhood_bfs_lock_processing(copy.curr_coord[0], copy.curr_coord[1])):
            copy.no_solution()
        copy.go_to_next_coord()
        return copy

    def goal_test(self, state: PipeManiaState):
        """Retorna True se e só se o estado passado como argumento é
        um estado objetivo. Deve verificar se todas as posições do tabuleiro
        estão preenchidas de acordo com as regras do problema."""
        def is_single_scc(state):
            done = 0
            queue = [(0, 0)]
            while queue:
                curr = queue.pop(0)
                piece = state.board.get_value(curr[0], curr[1])
                if (Piece.is_marked(state.board.get_value(curr[0], curr[1]))):
                    continue
                done += 1
                state.board.set_value(curr[0], curr[1], Piece.mark(piece))
                ori = Piece.get_orientation(piece)
                if ((ori & 0b1) == 1 and (not Piece.is_marked(state.board.get_value(curr[0], curr[1] + 1)))):
                    queue.append((curr[0], curr[1] + 1))
                if (((ori >> 1) & 0b1) == 1 and (not Piece.is_marked(state.board.get_value(curr[0] - 1, curr[1])))):
                    queue.append((curr[0] - 1, curr[1]))
                if (((ori >> 2) & 0b1) == 1 and (not Piece.is_marked(state.board.get_value(curr[0], curr[1] - 1)))):
                    queue.append((curr[0], curr[1] - 1))
                if (((ori >> 3) & 0b1) == 1 and (not Piece.is_marked(state.board.get_value(curr[0] + 1, curr[1])))):
                    queue.append((curr[0] + 1, curr[1]))
            if (done == (state.board.get_width() * state.board.get_height())):
                return True
            for i in range(0, state.board.get_height()):
                for j in range(0, state.board.get_width()):
                    state.board.set_value(i, j, Piece.unmark(state.board.get_value(i, j)))
            return False
        
        while (not state.reached_end() and state.left != 0):
            infered = state.infer(state.curr_coord[0], state.curr_coord[1])
            if (len(infered) == 1):
                state.point_and_lock(state.curr_coord[0], state.curr_coord[1], infered[0])
                if (state.neighbourhood_bfs_lock_processing(state.curr_coord[0], state.curr_coord[1])):
                    state.no_solution()
            else:
                if (len(infered) == 0):
                    state.no_solution()
                break
            state.go_to_next_coord()
        if (state.left == 0):
            return is_single_scc(state)
        return False

    def h(self, node: Node):
        """Função heuristica utilizada para a procura A*."""
        return node.state.locked
    

def heuristic_depth_first_tree_search(problem):
    frontier = [Node(problem.initial)]  # Stack

    while frontier:
        node = frontier.pop()
        if problem.goal_test(node.state):
            return node
        children = node.expand(problem)
        if (children):
            children.sort(key = problem.h)
        frontier.extend(children)
    return None

if __name__ == "__main__":
    Piece.init()
    problem = PipeMania(PipeManiaState.parse_instance())
    #solution = depth_first_tree_search(problem)
    solution = heuristic_depth_first_tree_search(problem)
    #solution = greedy_search(problem)
    #solution = astar_search(problem)
    solution.state.board.print_board()
    #print(PipeManiaState.state_id)
