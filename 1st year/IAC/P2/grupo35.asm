;############################################################################################################################
;#  Grupo-35
;#  João Maria Pereira Parreira Cota Teixeira Nº97226
;#  Francisco Miguel Martins Mónica Nº 106583
;#
;#  Projeto 2022-23 Versão Final
;############################################################################################################################

;############################################################################################################################
;#  Constantes 
;############################################################################################################################

; --- Endereços dos periféricos, máscaras e valor da 4ª linha do teclado ---
DISPLAYS            EQU 0A000H      ; endereço dos Displays de números (POUT-1)
TEC_LIN             EQU 0C000H      ; endereço das Linhas do teclado (POUT-2)
TEC_COL             EQU 0E000H      ; endereço das Colunas do teclado (PIN)
LINHA               EQU 8           ; linha a testar do teclado inicialmente (4ª linha, 1000b)
MASCARA             EQU 0FH         ; isola os 4 bits de menor peso
MASCARA_2           EQU 2           ; isola os 2 bits de menor peso

; --- Comandos MediaCenter ---
DEF_REMOVE_FUNDO    EQU 6040H       ; endereço do comando para eliminar a imagem de fundo e o aviso "No background selected"
DEF_FUNDO           EQU 6042H       ; endereço do comando para alterar a imagem de fundo
APAGA_PIXELS        EQU 6002H       ; endereço para apagar todos os pixeis do ecrã
DEF_LINHA    		EQU 600AH       ; endereço do comando para definir a linha do pixel a desenhar
DEF_COLUNA   		EQU 600CH       ; endereço do comando para definir a coluna do pixel a desenhar
DEF_PIXEL    		EQU 6012H       ; endereço do comando para desenhar um pixel
DEF_SOM             EQU 605AH       ; endereço do comando para adicionar um efeito sonoro
DEF_SOM_JOGO        EQU 605CH       ; endereço do comando para adicionar um efeito sonoro em ciclo
DEF_SOM_PAUSE       EQU 605EH       ; endereco do comando para pausar um som especifico
DEF_SOM_CONT        EQU 6060H       ; endereco do comando para continuar a reproducao de um som especifico
DEF_TERMINA_SOM     EQU 6066H       ; termina a reprodução do som especificado

; --- Estados de jogo ---
ESPERA_COMECO       EQU 0           ; há espera para (re)começar jogo
EM_JOGO             EQU 1           ; em jogo
EM_PAUSA            EQU 2           ; em pausa

; --- Dimensões e coordenada do pixel de referência do painel de instrumentos ---
LARGURA_PAINEL      EQU 15          ; largura do painel de instrumentos
ALTURA_PAINEL       EQU 5           ; altura do painel de intrumentos
LINHA_TOPO_PAINEL   EQU 27          ; linha superior do painel de instrumentos
COLUNA_PAINEL       EQU 25          ; coluna que delimita o lado esquerdo do painel de instrumentos

; --- Dimensões e coordenada do pixel de referência das luzes do painel ---
LARGURA_LUZES       EQU 7           ; largura das luzes do painel
ALTURA_LUZES        EQU 2           ; altura das luzes do painel
LINHA_LUZES_PAINEL  EQU 29          ; linha superior das luzes do painel
COLUNA_LUZES_PAINEL EQU 29          ; coluna mais à esquerda das luzes do painel

; --- Valor máximo para uma linha no ecrã e coordenada fora do ecrã ---
MAX_LINHA           EQU 31          ; valor máximo para uma linha (limite inferior ecrã)
COORDS_FORA_ECRA    EQU -1          ; usado para definir uma linha/coluna de uma sonda, em que indica que este não foi desenhado no ecrã (está "fora do ecrã")

; --- Alcance e coordenadas dos pixeis de referência das sondas
ALCANCE_SONDA       EQU 15          ; linha onde as sondas atingem o alcance máximo (12 movimentos)
LINHA_SONDAS        EQU 26          ; linha onde se encontram as sondas quando são disparadas
COLUNA_SONDA_ESQ    EQU 26          ; coluna onde se encontra a sonda que é disparada para a esquerda quando disparada
COLUNA_SONDA_MEIO   EQU 32          ; coluna onde se encontra a sonda que é disparada para a frente quando disparada
COLUNA_SONDA_DIR    EQU 37          ; coluna onde se encontra a sonda que é disparada para a direita quando disparada

; --- Dimensões de um asteroide
LARGURA_ASTEROIDE   EQU 5           ; largura dos asteroides
ALTURA_ASTEROIDE    EQU 5           ; altura dos asteroides

; --- Dimensões de um asteroide médio ---
LAR_ASTEROIDE_MEDIO EQU 3           ; largura do asteroide médio
ALT_ASTEROIDE_MEDIO EQU 3           ; altura do asteroide médio

; --- Dimensões de um asteroide pequeno ---
LAR_ASTEROIDE_PEQUENO EQU 1         ; largura do asteroide pequeno   
ALT_ASTEROIDE_PEQUENO EQU 1         ; altura do asteroide pequeno

; --- Colisão dos asteroides com sondas ---
HOUVE_COLISAO       EQU 1           ; indica que não houve colisão 
SEM_COLISAO         EQU 0           ; indica que houve colisão

; --- Linha e colunas possiíves para um asteroide quando evocados ---
LINHA_ASTEROIDES    EQU 0           ; linha onde aparecem os asteroides 
COLUNA_0            EQU 0           ; uma das colunas onde pode aparecer um asteroide (canto superior esquerdo)
COLUNA_30           EQU 30          ; uma das colunas onde pode aparecer um asteroide (meio do ecrã, parte superior)
COLUNA_59           EQU 59          ; uma das colunas onde pode aparecer um asteroide (canto superior esquerdo)

; --- Valores a somar às colunas dos asteroides para os mexer nas diversas direções ---
DIAG_ESQ            EQU -1          ; valor que representa a direção de se mover para a esquerda na diagonal (a somar à coluna)
FRENTE              EQU 0           ; valor que representa a direção de se mover em frente (a somar à coluna)
DIAG_DIR            EQU 1           ; valor que representa a direção de se mover para a direita na diagonal (a somar à coluna)

; --- Valores de colunas que delimitam o intervalo de valores onde pode haver uma potencial colisão ---
COLUNA_COLISAO_1    EQU 27          ; valor de coluna que delimita o intervalo de colisão com o painel de instrumentos (limite inferior)
COLUNA_COLISAO_2    EQU 37          ; valor da coluna que delimita o intervalo de colisão com o painel de instrumentos (limite superior)

; --- Cores ---
VERMELHO            EQU 0FF00H      ; cor vermelha em ARGB (opaco e vermelho no máximo, verde e azul a 0)
ROXO                EQU 0FF0FH      ; cor roxa em ARGB (opaco, vermelho e azul no máximo, verde a 0)
VERDE               EQU 0F0F0H      ; cor verde em ARGB (opaco e verde no máximo, azul e vermelho a 0)
CINZENTO            EQU 0F777H      ; cor cinzenta em ARGB (opaco no máximo, vermelho, verde e azul a 7)
AMARELO             EQU 0FFF0H      ; cor amarela em ARGB (opaco vermelho e verde no máximo, azul a 0)
AZUL                EQU 0F0FFH      ; cor azul claro em ARGB (opaco, verde e azul no máximo, vermelho a 0)
COR_INT_PAINEL      EQU 0FBB8H      ; cor do interior do painel em ARGB (opaco no máximo e verde e vermelho a B e azul a 8)

; --- Energia inicial, sem energia e fator de divisão ---
ENERGIA_INICIAL     EQU 0064H       ; energia inicial da nave (64H ou 100 em decimal)
SEM_ENERGIA         EQU 0           ; se a energia da nave chegar a este valor, então a nave ficou sem energia
FATOR               EQU 1000        ; fator de divisão para transformar hexadecimal em decimal

;#############################################################################################################################
;#  Dados
;#############################################################################################################################

    PLACE 1000H

    STACK 100H                      ; espaço reservado para a pilha do processo do programa principal
SP_prog_princ:                      ; endereço com que o SP deste processo é inicializado

    STACK 100H                      ; espaço reservado para a pilha do processo "teclado"
SP_teclado:                         ; endereço com que o SP deste processo é inicializado

    STACK 100H                      ; espaço reservado para a pilha do processo "varia_luzes"
SP_varia_luzes:                     ; endereço com que o SP do processo é inicializado

    STACK 100H                      ; espaço reservado para a pilha do processo "move_sondas"
SP_sondas:                          ; endereço com que o SP deste processo é inicializado 

    STACK 100H                      ; espaço reservado para a pilha do processo "diminui_energia"
SP_diminui_energia:                 ; endereço com que o SP deste processo é inicializado 

    STACK 100H                      ; espaço reservado para a pilha do processo "asteroides"
SP_asteroides:                      ; endereço com que o SP deste processo é inicialiazado

; Tabela das rotinas de interrupção
tab:
    WORD rot_int_0                  ; rotina da interrupção 0 (relógio asteroides)
    WORD rot_int_1                  ; rotina da interrupção 1 (relógio sondas)
    WORD rot_int_2                  ; rotina da interrupção 2 (relógio energia)
    WORD rot_int_3                  ; rotina da interrupção 3 (relógio nave)

evento_int:                         ; LOCKs para cada rotina de interrupção comunicar com os respetivos processos
    LOCK 0                          ; para a interrupção 0
    LOCK 0                          ; para a interrupção 1
    LOCK 0                          ; para a interrupção 2
    LOCK 0                          ; para a interrupção 3

estado_jogo:
    WORD 0                          ; indica o estado do jogo

energia_nave:
    WORD ENERGIA_INICIAL            ; armazena o valor da energia da nave em hexadecimal

tecla_lida:
    LOCK 0                          ; LOCK para o processo "teclado" comunicar aos outros processos a tecla carregada

; --- Sondas ---
DEF_SONDA_ESQ:                      ; sonda que é disparada para a diagonal esquerda
    WORD COORDS_FORA_ECRA           ; a linha onde se encontra a sonda
    WORD COORDS_FORA_ECRA           ; a coluna onde se encontra a sonda

    WORD ROXO                       ; cor do pixel da sonda

DEF_SONDA_MEIO:                     ; sonda que é disparada em frente
    WORD COORDS_FORA_ECRA           ; a linha onde se encontra a sonda
    WORD COORDS_FORA_ECRA           ; a coluna onde se encontra a sonda

    WORD ROXO                       ; cor do pixel da sonda

DEF_SONDA_DIR:                      ; sonda que é disparada para a direita
    WORD COORDS_FORA_ECRA           ; a linha onde se encontra a sonda
    WORD COORDS_FORA_ECRA           ; a coluna onde se encontra a sonda

    WORD ROXO                       ; cor do pixel da sonda

; --- Asteroides (valores inciais não são relevantes) ---
ASTEROIDE_1:
    WORD LINHA_ASTEROIDES           ; linha do pixel de referência do asteroide 
    WORD 0                          ; coluna do pixel de referência do asteroide

    WORD 0                          ; direção
    WORD 0                          ; tipo de asteroide
    WORD 0                          ; indica se o asteroide colidiu com alguma sonda

ASTEROIDE_2:
    WORD LINHA_ASTEROIDES           ; linha do pixel de referência do asteroide 
    WORD 0                          ; coluna do pixel de referência do asteroide

    WORD 0                          ; direção
    WORD 0                          ; tipo de asteroide
    WORD 0                          ; indica se o asteroide colidiu com alguma sonda

ASTEROIDE_3:
    WORD LINHA_ASTEROIDES           ; linha do pixel de referência do asteroide 
    WORD 0                          ; coluna do pixel de referência do asteroide

    WORD 0                           ; direção
    WORD 0                           ; tipo de asteroide  
    WORD 0                           ; indica se o asteroide colidiu com alguma sonda

ASTEROIDE_4:
    WORD LINHA_ASTEROIDES           ; linha do pixel de referência do asteroide 
    WORD 0                          ; coluna do pixel de referência do asteroide

    WORD 0                          ; direção
    WORD 0                          ; tipo de asteroide
    WORD 0                          ; indica se o asteroide colidiu com alguma sonda

; --- Pares coluna-direção
PARES_COL_DIR:                      ; tabela dos pares coluna-direção
    WORD COLUNA_0, DIAG_DIR         ; canto superior esquerdo, move-se para a sua diagonal direita
    WORD COLUNA_30, DIAG_DIR        ; parte superior no meio, move-se para a sua diagonal direita
    WORD COLUNA_30, DIAG_ESQ        ; parte superior no meio, move-se para a sua diagonal esquerda
    WORD COLUNA_30, FRENTE          ; parte superior no meio, move-se em frente/para baixo       
    WORD COLUNA_59, DIAG_ESQ        ; canto superior direito, move-se para a sua diagonal esquerda

; --- Definição de asteroides ---
DEF_ASTEROIDE_MINERAVEL:
    WORD LARGURA_ASTEROIDE          ; largura do asteroide
    WORD ALTURA_ASTEROIDE           ; altura do asteroide

; --- cores dos pixeis de cada linha ---

    WORD 0, VERDE, VERDE, VERDE, 0

    WORD VERDE, VERDE, VERDE, VERDE, VERDE

    WORD VERDE, VERDE, VERDE, VERDE, VERDE

    WORD VERDE, VERDE, VERDE, VERDE, VERDE

    WORD 0, VERDE, VERDE, VERDE, 0

DEF_ASTEROIDE_MEDIO:
    WORD LAR_ASTEROIDE_MEDIO        ; largura do asteroide médio
    WORD ALT_ASTEROIDE_MEDIO        ; altura do asteroide médio

; --- cores dos pixeis de cada linha ---

    WORD VERDE, VERDE, VERDE

    WORD VERDE, VERDE, VERDE

    WORD VERDE, VERDE, VERDE

DEF_ASTEROIDE_PEQUENO:
    WORD LAR_ASTEROIDE_PEQUENO      ; largura do asteroide pequeno
    WORD ALT_ASTEROIDE_PEQUENO      ; altura do asteroide pequeno

    WORD VERDE

DEF_ASTEROIDE_N_MINERAVEL:
    WORD LARGURA_ASTEROIDE          ; largura do asteroide
    WORD ALTURA_ASTEROIDE           ; altura do asteroide

; --- cores dos pixeis por linha ---

    WORD VERMELHO, 0, VERMELHO, 0, VERMELHO
    
    WORD 0, VERMELHO, VERMELHO, VERMELHO, 0
    
    WORD VERMELHO, VERMELHO, 0, VERMELHO, VERMELHO
    
    WORD 0, VERMELHO, VERMELHO, VERMELHO, 0
    
    WORD VERMELHO, 0, VERMELHO, 0, VERMELHO

DEF_EXPLOSAO:
    WORD LARGURA_ASTEROIDE          ; largura do asteroide
    WORD ALTURA_ASTEROIDE           ; altura do asteroide
    
; --- cores dos pixeis por linha ---
    
    WORD 0, AZUL, 0, AZUL, 0
    
    WORD AZUL, 0, AZUL, 0, AZUL
    
    WORD 0, AZUL, 0, AZUL, 0
    
    WORD AZUL, 0, AZUL, 0, AZUL
    
    WORD 0, AZUL, 0, AZUL, 0

; --- Definição do painel de instrumentos ---
DEF_PAINEL:
    WORD LARGURA_PAINEL             ; largura do painel
    WORD ALTURA_PAINEL              ; altura do painel

; --- cores dos pixeis de cada linha inicial (as luzes sofrem alterações) --- 
    
    WORD 0, 0, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, 0, 0
    
    WORD 0, VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL , VERMELHO, 0

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, CINZENTO, VERMELHO, VERDE, CINZENTO, VERDE, CINZENTO, CINZENTO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERDE, CINZENTO, VERMELHO, VERDE, AMARELO, AZUL, CINZENTO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

; --- Variação das luzes do painel ---
VARIACAO_LUZES_ATUAL:               ; guarda o endereço da variação de luzes atual no painel
    WORD DEF_VARIACAO_LUZES_1       ; 1ª variação

DEF_VARIACAO_LUZES_1:
    ; --- cores dos pixeis de cada linha ---
    
    WORD CINZENTO, VERMELHO, VERDE, AMARELO, CINZENTO, VERMELHO, CINZENTO
    
    WORD VERDE, AMARELO, VERMELHO, VERDE, AMARELO, AZUL, VERMELHO

DEF_VARIACAO_LUZES_2:
    ; --- cores dos pixeis de cada linha ---

    WORD AZUL, VERMELHO, CINZENTO, CINZENTO, CINZENTO, CINZENTO, AMARELO

    WORD VERDE, CINZENTO, VERMELHO, CINZENTO, CINZENTO, AZUL, CINZENTO

DEF_VARIACAO_LUZES_3:
    ; --- cores dos pixeis de cada linha ---

    WORD AZUL, VERMELHO, VERDE, AMARELO, VERDE, VERMELHO, AMARELO

    WORD VERDE, AMARELO, VERMELHO, VERDE, AMARELO, AZUL, VERMELHO

DEF_VARIACAO_LUZES_4:
    ; --- cores dos pixeis de cada linha ---

    WORD CINZENTO, VERMELHO, VERDE, CINZENTO, VERDE, CINZENTO, CINZENTO

    WORD VERDE, CINZENTO, VERMELHO, VERDE, AMARELO, AZUL, CINZENTO

DEF_VARIACAO_LUZES_5:
    ; --- cores dos pixeis de cada linha ---

    WORD AZUL, CINZENTO, VERDE, CINZENTO, CINZENTO, VERMELHO, AMARELO

    WORD CINZENTO, AMARELO, CINZENTO, VERDE, CINZENTO, CINZENTO, VERMELHO

DEF_VARIACAO_LUZES_6:
    ; --- cores dos pixeis de cada linha ---
    
    WORD CINZENTO, CINZENTO, CINZENTO, AMARELO, VERDE, VERMELHO, CINZENTO

    WORD CINZENTO, AMARELO, CINZENTO, CINZENTO, AMARELO, CINZENTO, VERMELHO

DEF_VARIACAO_LUZES_7:
    ; --- cores dos pixeis de cada linha ---

    WORD CINZENTO, CINZENTO, VERDE, AMARELO, VERDE, CINZENTO, CINZENTO

    WORD CINZENTO, CINZENTO, CINZENTO, VERDE, CINZENTO, CINZENTO, CINZENTO

;#############################################################################################################################
;#  Código
;#############################################################################################################################

    PLACE 0
inicio:
    MOV SP, SP_prog_princ           ; inicializa SP do programa principal
    MOV BTE, tab                    ; inicializa BTE (registo de Base da Tabela de Exceções)

    MOV R0, 0
    MOV R1, DISPLAYS                ; endereço dos Displays de números
    MOV [R1], R0                    ; inicia o display a zeros
    MOV [DEF_REMOVE_FUNDO], R0      ; remove mensagem de aviso do media center
    MOV [DEF_FUNDO], R0             ; escreve no ecrã a imagem de fundo de introdução (imagem número 0)
    MOV [DEF_SOM], R0               ; reproduz som de início de jogo

    EI0                             ; permite interrupções 0
    EI1                             ; permite interrupções 1
    EI2                             ; permite interrupções 2
    EI3                             ; permite interrupções 3
    EI                              ; permite interrupções (geral)

    CALL teclado                    ; cria o processo para varrer o teclado
    CALL varia_luzes                ; cria processo para variar as luzes do painel
    CALL move_sondas                ; cria processo para mover as sondas
    CALL diminui_energia            ; cria processo para diminuir a energia da nave periodicamente
    CALL asteroides                 ; cria processo para evocar e mover os asteroides durante o jogo

ciclo_processa_tecla_lida:
    CALL Q_TECLA                    ; processa tecla lida (lê variável LOCK na rotina pelo que não é necessário um YIELD aqui)
    JMP ciclo_processa_tecla_lida   ; continua ciclo

;#############################################################################################################################
;#  Q_TECLA: Rotina que avalia qual foi a tecla premida e processa a sua função
;#           utilizando rotinas complementares
;#
;#############################################################################################################################

Q_TECLA:
    PUSH R1                         ; guarda registos
    PUSH R4
    MOV R1, [tecla_lida]            ; obtém a tecla lida (bloqueia até que alguma tecla seja carregada)
eh_0:                               ; tecla '0' dispara uma sonda para a esquerda
    MOV R4, 0011H                   ; valor da tecla '0'
    XOR R4, R1                      ; compara o valor da tecla lida com o da tecla '0'
    JNZ eh_1                        ; valor da tecla não corresponde a '0', testa tecla seguinte
    CALL dispara_sonda_esq          ; dispara sonda para a esquerda
    JMP sai_q                       ; recupera registos e retorna
eh_1:                               ; tecla '1' dispara uma sonda para cima/em frente
    MOV R4, 0012H                   ; valor da tecla '1'
    XOR R4, R1                      ; compara o valor da tecla lida com o da tecla '1'
    JNZ eh_2                        ; valor da tecla não corresponde a '1', testa tecla seguinte
    CALL dispara_sonda_meio         ; dispara sonda para o meio/em frente
    JMP sai_q                       ; recupera registos e retorna
eh_2:                               ; tecla '2' dispara uma sonda para a direita
    MOV R4, 0014H                   ; valor da tecla '2'
    XOR R4, R1                      ; compara teclas
    JNZ eh_C                        ; teclas não correspondem, testa tecla seguinte
    CALL dispara_sonda_dir          ; dispara sonda para a direita
    JMP sai_q                       ; recupera registos e retorna
eh_C:                               ; tecla 'C' começa o jogo
    MOV R4, 0081H                   ; valor da tecla 'C'
    XOR R4, R1                      ; compara teclas
    JNZ eh_D                        ; valores não são iguais, testa tecla seguinte
    CALL comeca_jogo                ; (re)começa o jogo
    JMP sai_q                       ; recupera registos e retorna
eh_D:                               ; tecla 'D' pausa o jogo
    MOV R4, 0082H                   ; valor da tecla 'D'
    XOR R4, R1                      ; compara teclas
    JNZ eh_E                        ; tecla lida não corresponde à tecla 'D', testa tecla seguinte
    CALL pausa_jogo                 ; pausa/retoma o jogo
    JMP sai_q                       ; recupera registos e retorna
eh_E:                               ; tecla 'E' acaba o jogo
    MOV R4, 0084H                   ; valor da tecla 'E'
    XOR R4, R1                      ; compara os valores das teclas
    JNZ sai_q                       ; todas as teclas foram testadas, então recupera registos e retorna
    CALL termina_jogo               ; termina o jogo
sai_q:
    POP R4                          ; recupera registos
    POP R1
    RET

;#############################################################################################################################
;#  DISPARA_SONDA_ESQ: Rotina que dispara uma sonda para a esquerda
;#
;#############################################################################################################################

dispara_sonda_esq:
    PUSH R0                         ; guarda registos
    PUSH R2
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R2, [R0]                    ; lê o estado de jogo
    CMP R2, EM_JOGO                 ; verifica se é o estado de jogo correto
    JNZ sai_dispara_sonda_esq       ; se o estado não é o correto então retorna    
    MOV R0, DEF_SONDA_ESQ           ; endereço da sonda da esquerda (linha onde esta se encontra)
    MOV R2, [R0]                    ; obtém linha onde esta se encontra
    CMP R2, COORDS_FORA_ECRA        ; verifica se uma sonda nesta direção já foi disparada 
    JNZ sai_dispara_sonda_esq       ; uma sonda já foi disparada nesta direção, pelo que não se pode disparar uma nova até que esta desapareça
    MOV R2, LINHA_SONDAS            ; linha da sonda inicial
    MOV [R0], R2                    ; escreve linha inicial da sonda
    ADD R0, 2                       ; endereço da coluna onde se encontra a sonda
    MOV R2, COLUNA_SONDA_ESQ        ; coluna inicial
    MOV [R0], R2                    ; escreve coluna inicial da sonda
    MOV R2, DEF_SONDA_ESQ           ; endereço da sonda da esquerda (a rotina "desenha_sonda" usa este registo)
    CALL desenha_sonda              ; desenha a sonda (dispara-a)
    CALL diminui_energia_disparo    ; diminui energia da nave em 5%
    MOV R2, 1                       ; número do som a reproduzir
    MOV [DEF_SOM], R2               ; reproduz som de disparo
sai_dispara_sonda_esq:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  DISPARA_SONDA_MEIO: Rotina que dispara uma sonda para o meio/ em frente
;#
;#############################################################################################################################

dispara_sonda_meio:
    PUSH R0                         ; guarda registos
    PUSH R2  
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R2, [R0]                    ; lê o estado de jogo
    CMP R2, EM_JOGO                 ; verifica se é o estado de jogo correto (em jogo)
    JNZ sai_dispara_sonda_meio      ; se o estado não é o correto então retorna    
    MOV R0, DEF_SONDA_MEIO          ; endereço da linha onde se encontra a sonda do meio
    MOV R2, [R0]                    ; obtém linha da sonda
    CMP R2, COORDS_FORA_ECRA        ; verifica se uma sonda nesta direção já foi disparada
    JNZ sai_dispara_sonda_meio      ; se uma sonda já foi disparada nesta direção, então retorna (para disparar uma nova sonda tem de esperar que a anterior desapareça)
    MOV R2, LINHA_SONDAS            ; linha inicial da sonda
    MOV [R0], R2                    ; escreve linha inicial da sonda
    ADD R0, 2                       ; endereço da coluna da sonda
    MOV R2, COLUNA_SONDA_MEIO       ; coluna inicial da sonda
    MOV [R0], R2                    ; escreve coluna incial da sonda
    MOV R2, DEF_SONDA_MEIO          ; endereço da sonda do meio (a rotina "desenha_sonda" usa este registo)
    CALL desenha_sonda              ; desenha a sonda (dispara-a)
    CALL diminui_energia_disparo    ; diminui energia da nave em 5%    
    MOV R2, 1                       ; número do som a reproduzir
    MOV [DEF_SOM], R2               ; reproduz som de disparo
sai_dispara_sonda_meio:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  DISPARA_SONDA_DIR: Rotina que dispara uma sonda para a direita
;#
;#############################################################################################################################

dispara_sonda_dir:
    PUSH R0                         ; guarda registos
    PUSH R2   
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R2, [R0]                    ; lê o estado de jogo
    CMP R2, EM_JOGO                 ; verifica se é o estado de jogo correto (em jogo)
    JNZ sai_dispara_sonda_dir       ; se o estado não é o correto então retorna    
    MOV R0, DEF_SONDA_DIR           ; endereço da linha onde se encontra a sonda da direita
    MOV R2, [R0]                    ; obtém linha onde se encontra a sonda
    CMP R2, COORDS_FORA_ECRA        ; verifica se já foi disparada uma sonda nesta direção
    JNZ sai_dispara_sonda_dir       ; se uma sonda já foi disparada nesta direção, então retorna (para disparar uma nova sonda tem de esperar que a anterior desapareça)
    MOV R2, LINHA_SONDAS            ; linha inicial da sonda
    MOV [R0], R2                    ; escreve novo valor para a linha da sonda
    ADD R0, 2                       ; endereço da coluna da sonda
    MOV R2, COLUNA_SONDA_DIR        ; coluna inicial da sonda
    MOV [R0], R2                    ; escreve valor da coluna inicial
    MOV R2, DEF_SONDA_DIR           ; endereço da sonda da direita (a rotina "desenha_sonda" usa este registo)
    CALL desenha_sonda              ; desenha a sonda (dispara-a)
    CALL diminui_energia_disparo    ; diminui a energia da nave em 5%
    MOV R2, 1                       ; número do som a reproduzir
    MOV [DEF_SOM], R2               ; reproduz som de disparo
sai_dispara_sonda_dir:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  DIMINUI_ENERGIA_DISPARO: Rotina responsável por diminuir a energia da nave em 5% após cada disparo
;#
;#############################################################################################################################

diminui_energia_disparo:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R0, energia_nave            ; endereço da energia da nave
    MOV R1, [R0]                    ; obtém energia da nave
    MOV R2, 95                      ; diminui a energia em 5%
    MUL R1, R2
    MOV R2, 100
    DIV R1, R2
    MOV [R0], R1                    ; armazena/escreve novo valor de energia da nave
    CALL converte_energia           ; converte o valor de energia para decimal
    MOV R2, DISPLAYS                ; endereço do periférico dos Displays de números
    MOV [R2], R1                    ; escreve novo valor de energia nos Displays    
    CALL testa_sem_energia          ; verifica se a nave ficou sem energia
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  COMECA_JOGO: Rotina que começa o jogo
;#
;#############################################################################################################################

comeca_jogo:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    MOV R2, estado_jogo             ; endereço do estado de jogo
    MOV R0, [R2]                    ; lê o estado do jogo
    CMP R0, ESPERA_COMECO           ; verifica se o estado do jogo corresponde ao correto
    JNZ sai_comeca_jogo             ; se o estado do jogo não corresponde ao pretendido, então retorna
    MOV R2, energia_nave            ; endereço da energia da nave
    MOV R1, [R2]                    ; obtém energia da nave
    CALL converte_energia           ; converte a energia para decimal
    MOV R0, DISPLAYS                ; endereço dos Displays de números
    MOV [R0], R1                    ; escreve energia incial no display
    MOV R0, EM_JOGO
    MOV R2, estado_jogo             ; endereço do estado de jogo
    MOV [R2], R0                    ; altera o estado do jogo para (em jogo)
    MOV R0, 1
    MOV [DEF_FUNDO], R0             ; escreve no ecrã a imagem de fundo de jogo (imagem número 1)
    MOV R3, 6                       ; atribui a R3 a posicao do som
    MOV [DEF_SOM_JOGO], R3          ; reproduz som do jogo
    CALL desenha_painel             ; desenha_painel de instrumentos
    CALL escolhe_pos_inciais        ; escolhe posições, direção e tipo para os 4 asteroides iniciais
    CALL desenha_asteroides         ; desenha os 4 asteroides
sai_comeca_jogo:
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  ESCOLHE_POS_INICIAIS: Rotina que escolhe as posições, direções e tipos para
;#                        os 4 asteroides iniciais, assegurando que estes não
;#                        se sobrepõem 
;#
;#############################################################################################################################

escolhe_pos_inciais:
    PUSH R0                         ; guarda registos
    PUSH R1                         ; alguns dos registos guardados não são propriamente alterados diretamente pela rotina mas
    PUSH R2                         ; constituem saídas de outras rotinas que esta usa acabando por ser alterados
    PUSH R3
    PUSH R4
    PUSH R5
    CALL valor_aleatorio            ; calcula dois valores aleatórios, um deles é o endereço do par coluna-direção a não usar (R1)
    CALL obtem_enderecos_pares_usar ; calcula os 4 pares coluna-direção a usar (R2 a R5)
    MOV R0, ASTEROIDE_1             ; endereço do asteroide 1
    CALL inicia_dados_asteroide     ; inicia dados do asteroide 1
    MOV R0, ASTEROIDE_2             ; endereço do asteroide 2
    MOV R2, R3                      ; a rotina seguinte usa este registo
    CALL inicia_dados_asteroide     ; inicia dados do asteroide 2
    MOV R0, ASTEROIDE_3             ; endreço do asteroide 3
    MOV R2, R4                      ; a rotina seguinte usa este registo
    CALL inicia_dados_asteroide     ; inicia dados do asteroide 3
    MOV R0, ASTEROIDE_4             ; endereço do asteroide 4
    MOV R2, R5                      ; a rotina seguinte usa este registo
    CALL inicia_dados_asteroide     ; inicia dados do asteroide 4
    POP R5                          ; recupera registos
    POP R4
    POP R3
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  OBTEM_ENDERECOS_PARES_USAR: Rotina que obtém os endereços de 4 pares
;#                              coluna-direção diferentes do par recebido
;#                              (Estes 4 serão usados para evocar os 4
;#                               asteroides inciais)
;#
;#  ENTRADAS: R1 - endereço do par a não usar
;#
;#  SAÍDAS: R2 - endereço do 1º par
;#          R3 - endereço do 2º par
;#          R4 - endereço do 3º par
;#          R5 - endereço do 4º par
;#
;#############################################################################################################################

obtem_enderecos_pares_usar:
    PUSH R1                         ; guarda registos
    CALL obtem_par_seguinte         ; endereço do par seguinte ao par a não usar
    MOV R2, R1                      ; 1º par selecionado
    CALL obtem_par_seguinte         ; endereço do par seguinte ao 1º
    MOV R3, R1                      ; 2º par selecionado
    CALL obtem_par_seguinte         ; endereço do par seguinte ao 2º
    MOV R4, R1                      ; 3º par selecionado
    CALL obtem_par_seguinte         ; endereço do par seguinte ao 3º
    MOV R5, R1                      ; 4º par selecionado
    POP R1                          ; recupera registos
    RET

;#############################################################################################################################
;#  OBTEM_PAR_SEGUINTE: Rotina que obtém o endereço do par seguinte de um dado
;#                      endereço de um par da tabela coluna-direção 
;#
;#  ENTRADAS: R1 - endereço do par coluna-direção
;#
;#  SAÍDAS: R1 - endereço do par seguinte ao recebido
;#
;#############################################################################################################################

obtem_par_seguinte:
    PUSH R0                         ; guarda registos
    PUSH R3
    MOV R0, PARES_COL_DIR           ; endereço base da tabela de pares coluna-direção
    MOV R3, 16
    ADD R0, R3                      ; obtém endereço do último par (Cada par é composto por 4 Bytes)
    CMP R1, R0                      ; verifica se o par recebido é o último da tabela
    JZ  reinicia_par                ; se for o último volta-se ao par inicial
    MOV R3, 4
    ADD R1, R3                       ; como não é o último obtém endereço do par do seguinte
    JMP sai_par_seguinte
reinicia_par:
    MOV R1, PARES_COL_DIR           ; endereço do 1º par     
sai_par_seguinte:
    POP R3                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  INICIA_DADOS_ASTEROIDES: Rotina que incia dados de um determinado asteroide
;#                           recebendo um endereço de um dado par coluna-direção
;#                           e o endereço do asteroide
;#
;#  ENTRADAS: R0 - endereço do asteroide
;#            R2 - endereço do par coluna-direção
;#
;#############################################################################################################################

inicia_dados_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    ADD R0, 2                       ; endereço da coluna do asteroide
    MOV R1, [R2]                    ; obtém coluna do par coluna-direção
    MOV [R0], R1                    ; escreve valor da coluna
    ADD R0, 2                       ; endereço da direção do asteroide
    ADD R2, 2                       ; endereço do direção do par coluna-direção
    MOV R1, [R2]                    ; obtém direção (valor a somar à coluna para mover o asteroide)
    MOV [R0], R1                    ; escreve valor da direção
    ADD R0, 2                       ; endereço do tipo de asteroide
    CALL valor_aleatorio            ; gera dois valores, um deles entre 0 e 3 (R3)
    CMP R3, 0                       ; se o valor gerado for zero então o tipo do asteroide será minerável
    JNZ tipo_n_mineravel
    MOV R1, DEF_ASTEROIDE_MINERAVEL ; endereço do endereço do tipo de asteroide minerável
    JMP sai_dados_asteroide
tipo_n_mineravel:
    MOV R1, DEF_ASTEROIDE_N_MINERAVEL ; endereço do endereço do tipo de asteroide não minerável
sai_dados_asteroide:
    MOV [R0], R1                    ; escreve tipo de asteroide
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  PAUSA_JOGO: Rotina que pausa/retoma o jogo consoante o estado do jogo atual 
;#
;#############################################################################################################################

pausa_jogo:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R0, estado_jogo             ; endereço do estado do jogo
    MOV R1, [R0]                    ; obtém estado de jogo
    CMP R1, ESPERA_COMECO           ; se o jogo ainda não começou, este não pode ser pausado
    JZ sai_pausa_jogo
    CMP R1, EM_JOGO                 ; se o jogo estiver a decorrer então o jogo será pausado
    JNZ retoma_jogo                 ; se não estiver em jogo então significa que está em pausa pelo que o jogo será retomado
    MOV R1, EM_PAUSA
    MOV [R0], R1                    ; altera o estado de jogo para em pausa
    MOV R4, 6                       ; atribui a R4 a posicao do som
    MOV [DEF_SOM_PAUSE], R4         ; pausa o som do jogo
    MOV R1, 2
    MOV [DEF_SOM], R1               ; reproduz som de pausa de jogo
    MOV [APAGA_PIXELS], R1          ; apaga todos os pixeis do ecrã
    MOV R1, 2
    MOV [DEF_FUNDO], R1             ; seleciona a imagem de fundo de pausa
    JMP sai_pausa_jogo
retoma_jogo:
    MOV R1, EM_JOGO
    MOV [R0], R1                    ; altera o estado de jogo para "em_jogo"
    MOV R1, 1
    MOV [DEF_FUNDO], R1             ; seleciona fundo 1 novamente
    MOV R4, 6
    MOV [DEF_SOM_CONT], R4          ; endereço para retomar o som pausado
    CALL desenha_painel             ; desenha painel de instrumentos
    CALL desenha_asteroides         ; volta a desenhar os asteroides
    MOV R2, DEF_SONDA_ESQ           ; volta a desenhar as sondas
    CALL desenha_sonda
    MOV R2, DEF_SONDA_MEIO
    CALL desenha_sonda
    MOV R2, DEF_SONDA_DIR
    CALL desenha_sonda
sai_pausa_jogo:    
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  TERMINA_JOGO: Rotina que termina o jogo, quando este é terminado pelo
;#                jogador usando a tecla 'E'
;#
;#############################################################################################################################

termina_jogo:
    PUSH R0
    PUSH R1
    PUSH R3
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R0]                    ; obtém estado de jogo 
    CMP R1, ESPERA_COMECO           ; verifica se o jogo começou
    JZ sai_termina_jogo             ; se o jogo não começou então não é possível terminá-lo
    MOV R1, ESPERA_COMECO           ; estado de jogo inicial
    MOV [R0], R1                    ; altera o estado de jogo para o incial
    MOV R3, 6
    MOV [DEF_TERMINA_SOM], R3       ; termina o som do jogo
    MOV R1, 5
    MOV [DEF_SOM], R1               ; reproduz som de fim de jogo
    MOV [APAGA_PIXELS], R1          ; apaga pixeis do ecrã
    MOV R1, 3
    MOV [DEF_FUNDO], R1             ; seleciona imagem de fundo de fim de jogo 
    CALL reinicia_dados             ; reinicia dados do jogo
sai_termina_jogo:
    POP R3
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  REINICIA_DADOS: Rotina que reinicia os dados do jogo (energia, posições de 
;#                  sondas e de asteroides)
;#
;#############################################################################################################################

reinicia_dados:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R2, DEF_SONDA_ESQ           ; endereço da sonda que é disparada para a esquerda
    CALL reinicia_dados_sonda       ; reinicia dados da sonda que é disparada para a esquerda
    MOV R2, DEF_SONDA_MEIO          ; endereço da sonda que é disparada em frente
    CALL reinicia_dados_sonda       ; reinicia dados da sonda que é disparada em frente
    MOV R2, DEF_SONDA_DIR           ; endereço da sonda que é disparada para a direita
    CALL reinicia_dados_sonda       ; reinicia dados da sonda que é disparada para a direita
    MOV R0, energia_nave            ; endereço da energia da nave
    MOV R1, ENERGIA_INICIAL         ; valor inicial de energia da nave
    MOV [R0], R1                    ; atualiza a energia da nave para o seu valor inicial
    CALL reinicia_dados_asteroides  ; reinicia dados dos asteroides
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  REINICIA_DADOS_SONDA: Rotina que reinicia os dados de uma dada sonda
;#
;#  ENTRADAS: R2 - endereço da sonda a reiniciar
;#                              
;#############################################################################################################################

reinicia_dados_sonda:
    PUSH R1                         ; guarda registos
    PUSH R2
    MOV R1, COORDS_FORA_ECRA
    MOV [R2], R1                    ; altera o valor da linha da sonda para indicar que esta não foi disparada
    ADD R2, 2                       ; endereço da coluna da sonda
    MOV [R2], R1                    ; altera o valor da coluna da sonda para indicar que esta não foi disparada
    POP R2                          ; recupera registos
    POP R1
    RET

;#############################################################################################################################
;#  REINICIA_DADOS_ASTEROIDES: Rotina que reinicia os dados dos asteroides
;#                              
;#############################################################################################################################

reinicia_dados_asteroides:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    MOV R1, LINHA_ASTEROIDES        ; linha incial dos asteroides
    MOV R3, 8
    MOV R2, SEM_COLISAO
    MOV R0, ASTEROIDE_1             ; endereço do asteroide 1
    MOV [R0], R1                    ; reinicia valor da linha para o asteroide 1
    ADD R0, R3                      ; endereço da variável que indica que houve colisão com sonda
    MOV [R0], R2                    ; altera a variável para como não houve colisão
    MOV R0, ASTEROIDE_2             ; endereço do asteroide 2
    MOV [R0], R1                    ; reinicia valor da linha para o asteroide 2
    ADD R0, R3                      ; endereço da variável que indica que houve colisão com sonda
    MOV [R0], R2                    ; altera a variável para como não houve colisão
    MOV R0, ASTEROIDE_3             ; endereço do asteroide 3
    MOV [R0], R1                    ; reinicia valor da linha para o asteroide 3
    ADD R0, R3                      ; endereço da variável que indica que houve colisão com sonda
    MOV [R0], R2                    ; altera a variável para como não houve colisão
    MOV R0, ASTEROIDE_4             ; endereço do asteroide 4
    MOV [R0], R1                    ; reinicia valor da linha para o asteroide 4  
    ADD R0, R3                      ; endereço da variável que indica que houve colisão com sonda
    MOV [R0], R2                    ; altera a variável para como não houve colisão
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  Processo
;#
;#  TECLADO: Processo responsável pelo varrimento do teclado (deteta quando
;#           se carrega numa tecla)
;#
;#############################################################################################################################

PROCESS SP_teclado                  ; inicialização do SP do processo do teclado

teclado:
    MOV R2, TEC_LIN                 ; endereço do periférico das linhas
    MOV R3, TEC_COL                 ; endereço do periférico das colunas
    MOV R5, MASCARA                 ; máscara para isolar os 4 bits de menor peso ao ler as colunas do teclado
    MOV R6, LINHA                   ; começa a testar a 4ª linha

le_teclado:
    WAIT                            ; ciclo bloqueante
    MOV R1, R6                      ; testa a 4ª linha (inicialmente)
    MOVB [R2], R1                   ; escreve no periférico das linhas a linha a testar
    MOVB R0, [R3]                   ; lê do periférico das colunas
    AND R0, R5                      ; elimina bits para além dos bits 0-3
    CMP R0, 0                       ; verifica se há alguma tecla premida
    JZ proxima_linha                ; se nehuma tecla foi premida nesta linha, repete a leitura noutra linha
    SHL R1, 4                       ; coloca linha no nibble high (os primeiros 4 dígitos binários ficam a zeros)
    OR R1, R0                       ; junta coluna (coloca o resultado da coluna no nibble low de R1)
    MOV [tecla_lida], R1            ; escreve na variável LOCK a tecla carregada (linha e coluna)

ha_tecla:                           ; ciclo que espera até que nehuma tecla esteja premida
    YIELD                           ; ciclo bloqueante
    MOV R1, R6                      ; testa-se a linha da tecla premida (R1 foi alterado)
    MOVB [R2], R1                   ; escreve no periférico de saída (linhas)
    MOVB R0, [R3]                   ; lê do periférico das colunas
    AND R0, R5                      ; elimina bits para além dos 0-3
    CMP R0, 0                       ; verifica se ainda há alguma tecla premida
    JNZ ha_tecla                    ; se ainda houver uma tecla premida, continua à espera
    JMP  le_teclado                 ; volta a ler o teclado

proxima_linha:
    SHR R6, 1                       ; diminui o valor para ler outra linha (divide por 2)
    JNZ le_teclado                  ; o loop continua até que o valor da linha seja zero, nesse caso volta-se a testar a 4ª linha
    MOV R6, LINHA                   ; testa 4ª linha novamente
    JMP le_teclado                  ; continua a leitura

;#############################################################################################################################
;#  Processo
;#
;#  MOVE_SONDAS: Processo responsável por mover as sondas no ecrã
;#
;#############################################################################################################################

PROCESS SP_sondas                   ; inicialização do SP das sondas

move_sondas:
    MOV R0, evento_int
    MOV R1, [R0 + 2]                ; lê o LOCK correspondente à interrupção das sondas
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R0]                    ; obtém estado de jogo
    CMP R1, EM_JOGO                 ; verifica se o estado de jogo é o correto
    JNZ move_sondas                 ; se não for o correto não move as sondas
    CALL apaga_sondas               ; apaga as sondas que foram disparadas
    CALL move_sonda_esq             ; move a sonda da esquerda
    CALL move_sonda_meio            ; move a sonda do meio
    CALL move_sonda_dir             ; move sonda da direita
    JMP move_sondas                 ; continua ciclo

;#############################################################################################################################
;#  APAGA_SONDAS: Rotina que apaga as sondas que estejam no ecrã
;#
;#############################################################################################################################

apaga_sondas:
    PUSH R2                         ; guarda registos
    MOV R2, DEF_SONDA_ESQ           ; endereço da linha da sonda que foi disparada para a esquerda
    CALL apaga_sonda                ; apaga a sonda que que é disparada para a esquerda (caso esta tenha sido disparada)
    MOV R2, DEF_SONDA_MEIO          ; endereço da linha da sonda que foi disparada em frente
    CALL apaga_sonda                ; apaga a sonda que que é disparada em frente (caso esta tenha sido disparada)
    MOV R2, DEF_SONDA_DIR           ; endereço da linha da sonda que foi disparada para a direita
    CALL apaga_sonda                ; apaga a sonda que que é disparada para a direita (caso esta tenha sido disparada)
    POP R2                          ; recupera registos
    RET

;#############################################################################################################################
;#  MOVE_SONDA_ESQ: Rotina que move a sonda que foi disparada para a esquerda
;#
;#############################################################################################################################

move_sonda_esq:
    PUSH R0                         ; guarda registos
    PUSH R2
    MOV R0, DEF_SONDA_ESQ           ; endereço da linha onde se encontra a sonda que é disparada para a esquerda
    MOV R2, [R0]                    ; obtém valor da linha
    CMP R2, COORDS_FORA_ECRA        ; verifica se uma sonda foi disparada nesta direção
    JZ sai_move_sonda_esq           ; se a sonda não foi disparada então retorna
    SUB R2, 1                       ; novo valor para a linha da sonda
    CALL testa_alcance_sonda        ; verifica se a sonda atingiu o alcance máximo
    CMP R2, COORDS_FORA_ECRA        ; se o valor em R2 for igual a COORDS_FORA_ECRA então significa que a sonda atingiu o alcance máximo
    JZ sai_move_sonda_esq           ; se a sonda atingiu o alcance máximo então retorna    
    MOV [R0], R2                    ; escreve novo valor da linha da sonda
    ADD R0, 2                       ; endereço da coluna da sonda
    MOV R2, [R0]                    ; obtém valor da coluna onde se encontra a sonda
    SUB R2, 1                       ; novo valor da coluna da sonda
    MOV [R0], R2                    ; escreve novo valor da coluna da sonda
    MOV R2, DEF_SONDA_ESQ           ; endereço da linha onde se encontra a sonda (registo é usado pela rotina "desenha_sonda")
    CALL desenha_sonda              ; desenha a sonda na nova posição
sai_move_sonda_esq:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  MOVE_SONDA_MEIO: Rotina que move a sonda que foi disparada em frente
;#
;#############################################################################################################################

move_sonda_meio:
    PUSH R0                         ; guarda registos
    PUSH R2
    MOV R0, DEF_SONDA_MEIO          ; endereço da linha onde se encontra a sonda que é disparada para a direita
    MOV R2, [R0]                    ; obtém valor da linha
    CMP R2, COORDS_FORA_ECRA        ; verifica se uma sonda foi disparada nesta direção
    JZ sai_move_sonda_meio          ; se a sonda não foi disparada então retorna
    SUB R2, 1                       ; novo valor para a linha da sonda
    CALL testa_alcance_sonda        ; verifica se a sonda atingiu o alcance máximo
    CMP R2, COORDS_FORA_ECRA        ; se o valor em R2 for igual a COORDS_FORA_ECRA então significa que a sonda atingiu o alcance máximo
    JZ sai_move_sonda_meio          ; se a sonda atingiu o alcance máximo então retorna    
    MOV [R0], R2                    ; escreve novo valor da linha da sonda
    MOV R2, DEF_SONDA_MEIO          ; endereço da linha onde se encontra a sonda (registo é usado pela rotina "desenha_sonda")
    CALL desenha_sonda              ; desenha a sonda na nova posição
sai_move_sonda_meio:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  MOVE_SONDA_DIR: Rotina que move a sonda que foi disparada para a direita
;#
;#############################################################################################################################

move_sonda_dir:
    PUSH R0                         ; guarda registos
    PUSH R2
    MOV R0, DEF_SONDA_DIR           ; endereço da linha onde se encontra a sonda que é disparada para a direita
    MOV R2, [R0]                    ; obtém valor da linha
    CMP R2, COORDS_FORA_ECRA        ; verifica se uma sonda foi disparada nesta direção
    JZ sai_move_sonda_dir           ; se a sonda não foi disparada então retorna
    SUB R2, 1                       ; novo valor para a linha da sonda
    CALL testa_alcance_sonda        ; verifica se a sonda atingiu o alcance máximo
    CMP R2, COORDS_FORA_ECRA        ; se o valor em R2 for igual a COORDS_FORA_ECRA então significa que a sonda atingiu o alcance máximo
    JZ sai_move_sonda_dir           ; se a sonda atingiu o alcance máximo então retorna
    MOV [R0], R2                    ; escreve novo valor da linha da sonda
    ADD R0, 2                       ; endereço da coluna da sonda
    MOV R2, [R0]                    ; obtém valor da coluna onde se encontra a sonda
    ADD R2, 1                       ; novo valor da coluna da sonda
    MOV [R0], R2                    ; escreve novo valor da coluna da sonda
    MOV R2, DEF_SONDA_DIR           ; endereço da linha onde se encontra a sonda (registo é usado pela rotina "desenha_sonda")
    CALL desenha_sonda              ; desenha a sonda na nova posição
sai_move_sonda_dir:
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  TESTA_ALCANCE_SONDA: Rotina que testa se uma sonda atingiu o alcance máximo
;#
;#  ENTRADAS: R0 - endereço da linha onde se encontra a sonda a testar
;#            R2 - valor da linha da sonda a testar
;#
;#  SAÍDAS: R2 - valor da linha recebido ou valor (COORDS_FORA_ECRA) a indicar
;#               que a sonda atingiu o alcance máximo
;#
;#############################################################################################################################

testa_alcance_sonda:
    PUSH R0                         ; guarda registos
    PUSH R1
    MOV R1, ALCANCE_SONDA           ; valor da linha correspondente ao alcance máximo
    CMP R2, R1                      ; compara os valores das duas linhas
    JGE sai_testa_alcance           ; se o valor da linha da sonda for maior ou igual, então a sonda ainda não atingiu o alcance máximo
    MOV R1, COORDS_FORA_ECRA        ; linha que corresponde ao alcance máximo das sondas
    MOV [R0], R1                    ; escreve novo valor da linha indicando que a sonda passou a estar como não disparada
    ADD R0, 2                       ; endereço da coluna onde se encontra a sonda
    MOV [R0], R1                    ; escreve novo valor da coluna indicando que a sonda passou a estar como não disparada
    MOV R2, R1                      ; valor a retornar que indica que a sonda atingiu o alcance máximo
sai_testa_alcance:
    POP R1                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  APAGA_SONDA: Rotina que apaga uma sonda do ecrã caso esta tenha sido disparada
;#
;#  ENTRADAS: R2 - endereço da linha onde se encontra a sonda que se pretende
;#                 apagar do ecrã
;#
;#############################################################################################################################

apaga_sonda:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R5                        
    MOV R0, [R2]                    ; obtém linha onde se encontra a sonda
    CMP R0, COORDS_FORA_ECRA        ; se a sonda não foi disparada então retorna
    JZ sai_apaga_sonda              
    ADD R2, 2                       ; endereço da coluna onde se encontra a sonda
    MOV R1, [R2]                    ; obtém coluna onde se encontra a sonda
    MOV R5, 0                       ; cor do pixel a desenhar: sem cor (apaga o pixel)
    CALL escreve_pixel              ; apaga o pixel do ecrã
sai_apaga_sonda:
    POP R5                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  DESENHA_SONDA: Rotina que desenha uma sonda no ecrã
;#
;#  ENTRADAS: R2 - endereço da linha onde se encontra a sonda que se pretende
;#                 desenhar
;#
;#############################################################################################################################

desenha_sonda:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R5
    MOV R0, [R2]                    ; obtém linha onde se encontra a sonda
    ADD R2, 2                       ; endereço da coluna onde se encontra a sonda 
    MOV R1, [R2]                    ; obtém coluna onde se encontra a sonda
    ADD R2, 2                       ; endereço da cor da sonda
    MOV R5, [R2]                    ; obtém cor da sonda
    CALL escreve_pixel              ; desenha a sonda
    POP R5                          ; recupera registos
    POP R2
    POP R1
    POP R0                          
    RET

;#############################################################################################################################
;#  Processo
;#
;#  VARIA_LUZES: Processo responsável por variar as luzes do painel (vai
;#               redesenhando cada variação de luzes)
;#
;#############################################################################################################################

PROCESS SP_varia_luzes              ; inicialização do SP das luzes do painel

varia_luzes:
    MOV R0, evento_int
    MOV R1, [R0 + 6]                ; lê o LOCK correspondente à interrupção da nave
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R0]                    ; obtém estado de jogo
    CMP R1, EM_JOGO                 ; se o jogo não estiver a decorrer não varia as luzes
    JNZ varia_luzes
    CALL desenha_luzes              ; desenha variação das luzes do painel
    CALL variacao_luzes_seguinte    ; escreve próxima variação de luzes no endereço da variação atual
    JMP varia_luzes                 ; continua ciclo

;#############################################################################################################################
;#  DESENHA_LUZES: Rotina que desenha no ecrã as luzes do painel de
;#                 instrumentos
;#
;#############################################################################################################################

desenha_luzes:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R0, LINHA_LUZES_PAINEL      ; linha superior das luzes do painel
    MOV R1, COLUNA_LUZES_PAINEL     ; coluna mais à esquerda das luzes do painel
    MOV R2, [VARIACAO_LUZES_ATUAL]  ; obtém o endereço correspondente à variação de luzes atual (primeira cor desta variação)
    SUB R2, 2                       ; obtém endereço anterior à variação pretendida (a rotina "desenha_linha" usa este endereço)
    MOV R3, LARGURA_LUZES           ; obtém a largura das luzes do painel
    MOV R4, ALTURA_LUZES            ; obtém a altura das luzes do painel
L3:
    CMP R4, 0                       ; verifica se já desenhou todas as linhas
    JZ sai_desenha_luzes            ; condição de paragem
    CALL desenha_linha              ; desenha a linha
    ADD R0, 1                       ; linha seguinte
    SUB R4, 1                       ; subtrai 1 ao número de linhas po desenhar
    JMP L3                          ; continua ciclo
sai_desenha_luzes:
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#   VARIACAO_LUZES_SEGUINTE: Rotina que atualiza a variação de luzes para a
;#                            seguinte, escrevendo no endereço da variação atual
;#                            o endereço correspondente a essa nova variação
;#
;#############################################################################################################################

variacao_luzes_seguinte:
    PUSH R1                         ; guarda registos
    MOV R1, [VARIACAO_LUZES_ATUAL]  ; endereço da variação de luzes atual
    MOV R0, DEF_VARIACAO_LUZES_7    ; endereço da última variação de luzes
    CMP R1, R0                      ; verifica se corresponde à última variação
    JZ reinicia_variacao            ; se sim, volta à variação número 1
    MOV R2, 28
    ADD R1, R2                      ; o conjunto de luzes é composto por 14 luzes pelo que são 28 Bytes (R1 passa a ser o endereço da variação seguinte)
    JMP sai_luzes_seguinte          ; sai da rotina
reinicia_variacao:
    MOV R1, DEF_VARIACAO_LUZES_1    ; endereço correspondente à variação de luzes 1
sai_luzes_seguinte:
    MOV [VARIACAO_LUZES_ATUAL], R1  ; escreve nova variação de luzes
    POP R1                          ; recupera registos
    RET

;#############################################################################################################################
;#  Processo
;#
;#  DIMINUI_ENERGIA: Processo responsável por decrementar a energia da nave em 3% a cada 3 segundos
;#
;#############################################################################################################################

PROCESS SP_diminui_energia

diminui_energia:
    MOV R0, evento_int
    MOV R1, [R0 + 4]                ; lê o LOCK correspondente à interrupção da energia
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R0]                    ; obtém estado de jogo
    CMP R1, EM_JOGO                 ; se o jogo não estiver a decorrer não varia as luzes
    JNZ diminui_energia
    MOV R0, energia_nave            ; endereço da energia da nave
    MOV R1, [R0]                    ; obtém energia da nave
    MOV R2, 97                      ; diminui a energia em 3%
    MUL R1, R2
    MOV R2, 100
    DIV R1, R2
    MOV [R0], R1                    ; escreve novo valor de energia
    CALL converte_energia           ; converte a energia para decimal
    MOV R0, DISPLAYS                ; endereço do periférico dos Displays de números
    MOV [R0], R1                    ; escreve novo valor de energia nos Displays
    CALL testa_sem_energia          ; verifica se a nave ficou sem energia
    JMP diminui_energia             ; continua ciclo

;#############################################################################################################################
;#  TESTA_SEM_ENERGIA: Rotina que testa se a nave ficou sem energia, se a nave
;#                     ficou sem energia então acaba o jogo
;#
;#  ENTRADAS: R1 - valor de energia em decimal a verificar
;#
;#############################################################################################################################

testa_sem_energia:
    PUSH R1                         ; guarda registos
    PUSH R2
    CMP R1, SEM_ENERGIA             ; verifica se a nave ainda tem uma energia superior a 0
    JGT sai_testa_energia           ; se a energia ainda é superior não há problema
    MOV R1, ESPERA_COMECO           ; endereço do estado de jogo
    MOV [estado_jogo], R1           ; altera o estado de jogo para o inicial
    MOV [APAGA_PIXELS], R1          ; apaga pixeis do ecrã
    MOV R1, 5                       ; imagen número 5
    MOV R2, 6
    MOV [DEF_TERMINA_SOM], R2       ; termina a reproducao do som do jogo
    MOV [DEF_FUNDO], R1             ; seleciona a imagem número de fim de jogo por ficar sem energia
    MOV [DEF_SOM], R1               ; reproduz som de fim de jogo
    CALL reinicia_dados             ; reinicia dados do jogo 
sai_testa_energia:
    POP R2                          ; recupera registos
    POP R1
    RET

;#############################################################################################################################
;#  CONVERTE_ENERGIA: Rotina que converte o valor de energia em hexadecimal para decimal
;#
;#  ENTRADAS: R1 - energia em hexadecimal
;#
;#  SAÍDAS: R1 - energia convertida para decimal            
;#
;#############################################################################################################################

converte_energia:
    PUSH R0                         ; guarda registos
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R5
    MOV R5, 10                      ; utilizado para obter o próximo fator de divisão
    MOV R2, FATOR                   ; fator de divisão (inicialmente é 1000)
    MOV R4, 0                       ; resultado inicial
ciclo_converte:
    MOD R1, R2                      ; valor a converter em cada iteração
    DIV R2, R5                      ; prepara próximo fator de divisão
    CMP R2, 0                       ; verifica se o fator de divisão chegou a 0
    JZ sai_converte                 ; se o fator chegou a 0, então sai do ciclo (tem-se o resultado pretendido)
    MOV R3, R1                      ; registo auxiliar (DIV afeta o 1º operando)
    DIV R3, R2                      ; mais um dígito do valor em decimal
    SHL R4, 4                       ; deslocação para se obter um espaço para o novo dígito
    OR R4, R3                       ; junta novo dígito
    JMP ciclo_converte              ; continua ciclo
sai_converte:
    MOV R1, R4                      ; coloca resultado em R1
    POP R5                          ; recupera registos
    POP R4
    POP R3
    POP R2
    POP R0
    RET

;#############################################################################################################################
;#  Processo
;#
;#  ASTEROIDES: Processo responsável por evocar asteroides e movê-los
;#
;#############################################################################################################################

PROCESS SP_asteroides

asteroides:
    MOV R0, evento_int
    MOV R1, [R0]                    ; lê o LOCK correspondente à interrupção dos asteroides
    MOV R0, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R0]                    ; obtém estado de jogo
    CMP R1, EM_JOGO                 ; verifica se o estado de jogo é o correto
    JNZ asteroides                  ; se não for o correto não move/evoca os asteroides
    CALL apaga_asteroides           ; apaga os asteroides no ecrã
    MOV R0, ASTEROIDE_1             ; endereço do asteroide 1 (endereço da linha do seu pixel de referência)
    CALL move_asteroide             ; move/evoca asteroide número 1
    MOV R0, ASTEROIDE_2             ; endereço do asteroide 2 (endereço da linha do seu pixel de referência)
    CALL move_asteroide             ; move/evoca asteroide número 2
    MOV R0, ASTEROIDE_3             ; endereço do asteroide 3 (endereço da linha do seu pixel de referência)
    CALL move_asteroide             ; move/evoca asteroide número 3
    MOV R0, ASTEROIDE_4             ; endereço do asteroide 4 (endereço da linha do seu pixel de referência)
    CALL move_asteroide             ; move/evoca asteroide número 4
    CALL desenha_asteroides         ; desenha os 4 asteroides nas suas novas posições
    JMP asteroides                  ; continua ciclo

;#############################################################################################################################
;#  APAGA_ASTEROIDES: Rotina que apaga os asteroides que estejam no ecrã
;#
;#############################################################################################################################

apaga_asteroides:
    PUSH R3                         ; guarda registos
    MOV R3, ASTEROIDE_1             ; endereço do asteroide 1 (linha do pixel de referência do asteroide)
    CALL remove_asteroide           ; apaga asteroide 1
    MOV R3, ASTEROIDE_2             ; endereço do asteroide 2 (linha do pixel de referência do asteroide)
    CALL remove_asteroide           ; apaga asteroide 2
    MOV R3, ASTEROIDE_3             ; endereço do asteroide 3 (linha do pixel de referência do asteroide)
    CALL remove_asteroide           ; apaga asteroide 3
    MOV R3, ASTEROIDE_4             ; endereço do asteroide 4 (linha do pixel de referência do asteroide)
    CALL remove_asteroide           ; apaga asteroide 4
    POP R3                          ; recupera registos
    RET

;#############################################################################################################################
;#  REMOVE_ASTEROIDE: Rotina que obtém posição do pixel de referência do
;#                    asteroide e o seu tipo e apaga-o do ecrã
;#
;#  ENTRADAS: R3 - endereço do asteroide a remover
;#
;#############################################################################################################################

remove_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    MOV R0, [R3]                    ; obtém linha do pixel de referência do asteroide
    ADD R3, 2                       ; endereço da coluna do pixel de referência do asteroide
    MOV R1, [R3]                    ; obtém coluna do pixel de referência
    ADD R3, 4                       ; endereço do tipo de asteroide
    MOV R2, [R3]                    ; obtém tipo de asteroide
    CALL apaga_asteroide            ; apaga asteroide
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  MOVE_ASTEROIDE: Rotina que move um determinado asteroide no ecrã
;#  
;#  ENTRADAS: R0 - endereço de um dos asteroides
;#
;#############################################################################################################################

move_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R3
    PUSH R4
    PUSH R7
    CALL desenvolve_explosao        ; verifica se anteriormente houve alguma asteroide que foi explodido/minerado e que precise de ser "desenvolvido"
    CMP R4, 1                       ; verifica se o asteroide foi desenvolvido ou não
    JZ sai_move_asteroide           ; se o asteroide foi desenvolvido então retorna (não o precisa de "mover")
    MOV R1, [R0]                    ; obtém valor da linha do pixel de referência do asteroide
    ADD R1, 1                       ; novo valor da linha do asteroide  
    ADD R0, 2                       ; endereço da coluna do asteroide
    MOV R3, [R0]                    ; obtém coluna do asteroide
    ADD R0, 2                       ; endereço da direção do asteroide
    MOV R4, [R0]                    ; obtém valor a somar à coluna para mover o asteroide na posição pretendida (direção)
    ADD R3, R4                      ; novo valor da coluna do asteroide
    SUB R0, 2                       ; endereço da coluna do asteroide
    CALL testa_colisoes             ; testa se houve alguma colisão (com o painel ou com uma sonda)
    CMP R7, 1                       ; verifica se foi detetada uma colisão
    JZ sai_move_asteroide           ; se colidiu então retorna
    MOV [R0], R3                    ; escreve novo valor da coluna do asteroide
    SUB R0, 2                       ; endereço da linha do asteroide
    MOV [R0], R1                    ; escreve novo valor da linha do asteroide 
    CALL testa_fora_ecra            ; testa se o asteroide saiu do ecrã   
sai_move_asteroide:
    POP R7                          ; recupera registos
    POP R4
    POP R3
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  DESENVOLVE_EXPLOSAO: Rotina que verifica se um dado asteroide colidiu
;#                       com uma sonda anteriormente e "desenvolve-o"
;#
;#  ENTRADAS: R0 - endereço de um asteroide
;#
;#  SAÍDAS: R4 - indica que desenvolveu uma explosão ou não (a 1 indica que sim, a 0 indica que não)
;#
;#############################################################################################################################

desenvolve_explosao:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R4, 0                       ; indica que não desenvolveu explosão
    MOV R1, 8
    ADD R0, R1                      ; endereço da variável que indica se o asteroide colidiu com uma sonda 
    MOV R1, [R0]                    ; obtém variável
    CMP R1, HOUVE_COLISAO           ; verifica se o asteroide colidiu com uma sonda
    JNZ sai_desenvolve_explosao     ; se não colidiu então retorna
    MOV R4, 1                       ; indica que desenvolveu explosão    
    SUB R0, 2                       ; endereço do tipo de asteroide
    MOV R1, [R0]                    ; obtém tipo de asteroide
    MOV R2, DEF_EXPLOSAO            ; endereço da explosão de um asteroide
    CMP R1, R2                      ; compara tipos de asteroide
    JNZ compara_medio               ; se este não for o tipo correto então compara com o tipo seguinte
    ADD R0, 2                       ; endereço da variável que indica que o asteroide colidiu com uma sonda
    MOV R1, SEM_COLISAO 
    MOV [R0],R1                     ; coloca a variável de colisão outra vez como sem colisão
    MOV R2, 8
    SUB R0, R2                      ; endereço do asteroide (linha do pixel de referência)
    CALL evoca_asteroide            ; evoca o asteroide numa nova posição
    JMP sai_desenvolve_explosao
compara_medio:
    MOV R2, DEF_ASTEROIDE_MEDIO     ; endereço de um asteroide médio
    CMP R1, R2                      ; compara os dois tipos de asteroide
    JNZ desenvolve_pequeno          ; se este não for o tipo correto então é o tipo seguinte
    SUB R0, 6                       ; endereço do asteroide (linha do pixel de referência)
    MOV R1, [R0]                    ; obtém linha de referência
    ADD R1, 1                       ; adiciona 1 ao valor da linha
    MOV [R0], R1                    ; escreve novo valor da linha
    ADD R0, 2                       ; endereço da coluna
    MOV R1, [R0]                    ; obtém coluna
    ADD R1, 1                       ; adiciona 1 ao valor da coluna
    MOV [R0], R1                    ; escreve novo valor da coluna
    ADD R0, 4                       ; endereço do tipo de asteroide
    MOV R2, DEF_ASTEROIDE_PEQUENO   ; tipo pequeno de asteroide
    MOV [R0], R2                    ; escreve novo tipo do asteroide
    JMP sai_desenvolve_explosao
desenvolve_pequeno:
    ADD R0, 2                       ; endereço da variável que indica que o asteroide colidiu com uma sonda
    MOV R1, SEM_COLISAO 
    MOV [R0],R1                     ; coloca a variável de colisão outra vez como sem colisão
    MOV R2, 8
    SUB R0, R2                       ; endereço do asteroide (linha do pixel de referência)
    CALL evoca_asteroide            ; evoca o asteroide numa nova posição
sai_desenvolve_explosao:
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  TESTA_COLISOES: Rotina que verifica se um dado asteroide colidiu
;#                  com algum outro boneco
;#
;#  ENTRADAS: R0 - endereço da coluna do asteroide
;#
;#  SAÍDAS: R7 - indica se houve colisão ou não (a 1 indica que sim, a 0 o contrário)
;#
;#############################################################################################################################

testa_colisoes:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R7, 0                       ; inicialmente indica que não houve colisão com uma sonda 
    CALL testa_colisao_com_sondas   ; verifica se o asteroide colidiu com alguma sonda
    CMP R7, 1                       ; verifica se a rotina anterior detetou alguma colisão
    JNZ testa_colisao_com_painel    ; se não houve colisão com alguma sonda testa se houve colisão com o painel  
    ADD R0, 4                       ; endereço do endereço do tipo de asteroide
    MOV R1, [R0]                    ; obtém tipo de asteroide
    MOV R2, DEF_ASTEROIDE_MINERAVEL
    CMP R1, R2                      ; verifica se o asteroide é do tipo minerável
    JNZ explode_asteroide           ; se não então explode asteroide
    MOV R2, 4                       ; número do som do meteoro minerável
    MOV [DEF_SOM], R2               ; reproduz som de colisão com asteroide minerável
    CALL aumenta_energia_colisao    ; aumenta a energia em 25%
    MOV R1, DEF_ASTEROIDE_MEDIO     ; endereço de um asteroide médio
    SUB R0, 4                       ; endereço da coluna do asteroide
    MOV R2, [R0]                    ; obtém coluna
    ADD R2, 1                       ; incrementa valor da coluna
    MOV [R0], R2                    ; escreve novo valor da coluna
    SUB R0, 2                       ; endereço da linha do asteroide
    MOV R2, [R0]                    ; obtém linha
    ADD R2, 1                       ; incrementa valor da linha
    MOV [R0], R2                    ; escreve novo valor da linha
    ADD R0, 6                       ; endereço do tipo de asteroide
    JMP altera_tipo_asteroide  
explode_asteroide:
    MOV R2, 3                       ; número do som da explosão
    MOV [DEF_SOM], R2               ; reproduz som de explosão  
    MOV R1, DEF_EXPLOSAO            ; endereço de um asteroide após explosão
altera_tipo_asteroide:
    MOV [R0], R1                    ; escreve novo tipo de asteroide
    ADD R0, 2                       ; endereço da variável que indica se o asteroide foi atingido por uma sonda
    MOV R1, HOUVE_COLISAO
    MOV [R0], R1                    ; altera variável para como houve colisão
    MOV R7, 1                       ; indica que houve colisão
    JMP sai_testa_colisoes
testa_colisao_com_painel:
    CALL testa_colisao_painel       ; verifica se o asteroide colidiu com o painel    
sai_testa_colisoes:
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  TESTA_COLISAO_COM_SONDAS: Rotina que verifica se um dado asteroide colidiu
;#                            com uma sonda
;#
;#  ENTRADAS: R1 - linha do pixel de referência do asteroide
;#            R3 - coluna do pixel de referência do asteroide
;#
;#  SAÍDAS: R7 - indica se houve colisão ou não (a 1 indica que sim, a 0 o contrário)
;# 
;#############################################################################################################################

testa_colisao_com_sondas:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R0, R1                      ; R0 passa a ter a linha do pixel de referência
    MOV R2, R3                      ; R2 passa a ter a coluna do pixel de referência
    ADD R1, 5                       ; R1 passa a ter a linha inferior do asteroide
    ADD R3, 5                       ; R3 passa a ter a coluna mais à direita do asteroide
    MOV R7, 0                       ; indica inicialmente que não houve colisões
    MOV R4, DEF_SONDA_ESQ           ; endereço da sonda que é disparada para a esquerda
    CALL testa_colisao_sonda        ; verifica colisão com a sonda disparada para a esquerda    
    MOV R4, DEF_SONDA_MEIO          ; endereço da sonda que é disparada em frente
    CALL testa_colisao_sonda        ; verifica colisão com a sonda disparada em frente
    MOV R4, DEF_SONDA_DIR           ; endereço da sonda que é disparada para a direita
    CALL testa_colisao_sonda        ; verifica colisão com a sonda disparada para a direita
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  TESTA_COLISAO_SONDA: Rotina que se um asteroide colidiu com uma
;#                       determinada sonda 
;#
;#  ENTRADAS: R4 - endereço da sonda a verificar
;#            R0 - linha do pixel de referência do asteroide
;#            R1 - linha inferior do asteroide
;#            R2 - coluna do pixel de referência do asteroide
;#            R3 - coluna mais à direita do asteroide
;#
;#  SAÍDAS: R7 - indica se houve colisão ou não (a 1 indica que sim, a 0 o contrário)
;#
;#############################################################################################################################

testa_colisao_sonda:
    PUSH R2                         ; guarda registos
    PUSH R4
    PUSH R5
    PUSH R6
    MOV R5, [R4]                    ; obtém linha da sonda
    CMP R5, COORDS_FORA_ECRA        ; verifica se a sonda foi disparada
    JZ sai_testa_colisao_sonda      ; se a sonda não foi disparada então não pode have colisão
    ADD R4, 2                       ; endereço da coluna da sonda
    MOV R6, [R4]                    ; obtém coluna da sonda
    ; verifica se a sonda está entre as linhas e colunas que delimitam o asteroide
    CMP R5, R0
    JLT sai_testa_colisao_sonda
    CMP R5, R1
    JGT sai_testa_colisao_sonda
    CMP R6, R2
    JLT sai_testa_colisao_sonda
    CMP R6, R3
    JGT sai_testa_colisao_sonda
    SUB R4, 2                       ; endereço da linha da sonda
    MOV R2, R4                      ; as duas rotinas que se seguem usam este registo
    CALL apaga_sonda                ; apaga sonda do ecrã
    CALL reinicia_dados_sonda
    MOV R7, 1                       ; indica que houve uma colisão
sai_testa_colisao_sonda:
    POP R6                          ; recupera registos
    POP R5
    POP R4
    POP R2
    RET

;#############################################################################################################################
;#  AUMENTA_ENERGIA_COLISAO: Rotina que aumenta energia da nave em 25% após
;#                           uma sonda atingir um asteroide minerável 
;#
;#############################################################################################################################

aumenta_energia_colisao:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R0, energia_nave            ; endereço da energia da nave
    MOV R1, [R0]                    ; obtém energia da nave
    MOV R2, 125                     ; aumenta energia da nave em 25%
    MUL R1, R2
    MOV R2, 100
    DIV R1, R2
    MOV [R0], R1                    ; escreve novo valor de energia da nave
    CALL converte_energia           ; converte o valor de energia para decimal
    MOV R2, DISPLAYS                ; endereço do periférico dos Displays de números
    MOV [R2], R1                    ; escreve novo valor de energia nos Displays  
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  TESTA_COLISAO_PAINEL: Rotina que testa se um determinado asteroide colidiu com o painel
;#  
;#  ENTRADAS: R1 - valor da linha do asteroide
;#            R3 - valor da coluna do asteroide
;#
;#  SAÍDAS: R7 - indica se houve colisão ou não (a 1 indica que sim, a 0 o contrário)
;#
;#############################################################################################################################

testa_colisao_painel:               ; existe colisão se a coluna de referência está entre "COLUNA_COLISAO_1" e "COLUNA_COLISAO_2" (1)
                                    ; ou se a coluna mais à direita do asteroide também estiver entre estes valores (2)
    PUSH R1                         ; guarda registos
    PUSH R2
    PUSH R3
    ADD R1, 5                       ; valor da linha inferior do asteroide
    MOV R2, LINHA_TOPO_PAINEL       ; valor da linha superior do painel
    CMP R1, R2                      ; verifica se a linha inferior do meteoro é a mesma que a do topo do painel
    JNZ sai_testa_colisao_painel    ; se as linhas não coincidem então não houve colisão
colisao_possivel_1:                 ; verifica primeiro caso de colisão
    MOV R2, COLUNA_COLISAO_1        ; valor de coluna que delimita o intervalo de colisão com o painel de instrumentos (limite inferior) 
    CMP R3, R2                      ; compara os valores das colunas
    JLT colisao_possivel_2          ; se o valor da coluna do pixel de referência é inferior ao limite inferior do intervalo então testa segundo caso de colisão
    MOV R2, COLUNA_COLISAO_2        ; valor de coluna que delimita o intervalo de colisão com o painel de instrumentos (limite superior)
    CMP R3, R2                      ; compara os valores das colunas
    JGT colisao_possivel_2          ; se o valor da coluna do pixel de referência é superior ao limite superior do intervalo então testa segundo caso de colisão
    CALL colisao_painel             ; termina o jogo por colisão com o painel
    JMP sai_testa_colisao_painel
colisao_possivel_2:                 ; verifica segundo caso de colisão
    ADD R3, 5                       ; valor da coluna mais à direita do asteroide
    MOV R2, COLUNA_COLISAO_1        ; valor de coluna que delimita o intervalo de colisão com o painel de instrumentos (limite inferior) 
    CMP R3, R2                      ; compara os valores
    JLT sai_testa_colisao_painel    ; se o valor da coluna do pixel de referência é inferior ao limite inferior do intervalo então não houve colisão
    MOV R2, COLUNA_COLISAO_2        ; valor de coluna que delimita o intervalo de colisão com o painel de instrumentos (limite superior)
    CMP R3, R2                      ; compara os valores
    JGT sai_testa_colisao_painel    ; se o valor da coluna do pixel de referência é superior ao limite superior do intervalo então não houve colisão
    CALL colisao_painel             ; termina o jogo por colisão com o painel
sai_testa_colisao_painel:
    POP R3                          ; recupera registos
    POP R2
    POP R1
    RET

;#############################################################################################################################
;#  COLISAO_PAINEL: Rotina que termina o jogo por se ter dado uma colisão entre
;#                  um asteroide e o painel de instrumentos
;#
;#  SAÍDAS: R7 - indica que houve uma colisão
;#
;#############################################################################################################################

colisao_painel:
    PUSH R1                         ; guarda registos
    PUSH R2
    MOV R7, 1                       ; indica que houve colisão
    MOV R1, ESPERA_COMECO           ; endereço do estado de jogo
    MOV [estado_jogo], R1           ; altera o estado de jogo para o inicial
    MOV [APAGA_PIXELS], R1          ; apaga pixeis do ecrã
    MOV R1, 4                       ; imagen número 4
    MOV [DEF_FUNDO], R1             ; seleciona a imagem número de fim de jogo por colisão com asteroide
    MOV R2, 6
    MOV [DEF_TERMINA_SOM], R2       ; termina a reproducao do som do jogo
    MOV R1, 5                       ; número do som de fim de jogo
    MOV [DEF_SOM], R1               ; reproduz som de fim de jogo
    CALL reinicia_dados             ; reinicia dados do jogo 
    POP R2                          ; recupera registos
    POP R1
    RET

;#############################################################################################################################
;#  TESTA_FORA_ECRA: Rotina que testa se um determinado asteroide saiu fora do ecrã
;#  
;#  ENTRADAS: R1 - valor da linha do asteroide
;#            R0 - endereço da linha do asteroide
;#
;#############################################################################################################################

testa_fora_ecra:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    MOV R2, MAX_LINHA               ; valor máximo para o valor de uma linha
    CMP R1, R2                      ; verifica se o valor da linha é maior que máximo valor para uma linha possível
    JLE sai_testa_fora_ecra         ; se o valor ainda não ultrapassou o valor máximo para uma linha então retorna
    CALL evoca_asteroide            ; evoca o asteroide numa nova posição
sai_testa_fora_ecra:
    POP R2                          ; recupera registos
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  EVOCA_ASTEROIDE - Rotina que evoca um asteroide numa determinada posição,
;#                    direção e tipo
;#
;#  ENTRADAS: R0 - endereço do asteroide (linha do pixel de referência deste)  
;#
;#############################################################################################################################    

evoca_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    CALL valor_aleatorio            ; gera um valor entre 0 e 3 (decide tipo do asteroide) e um endereço de um par coluna-direção
    MOV R2, LINHA_ASTEROIDES        ; linha inicial do asteroide
    MOV [R0], R2                    ; escreve novo valor da linha do asteroide
    MOV R2, [R1]                    ; obtém coluna gerada aleatoriamente
    ADD R0, 2                       ; endereço da coluna do asteroide
    MOV [R0], R2                    ; escreve novo valor para a coluna do asteroide
    ADD R0, 2                       ; endereço da direção do asteroide
    ADD R1, 2                       ; endereço da direção gerada aleatoriamente
    MOV R2, [R1]                    ; obtém direção gerada
    MOV [R0], R2                    ; escreve direção do asteroide
    ADD R0, 2                       ; endereço do tipo de asteroide
    CMP R3, 0                       ; se R3 for 0 então o tipo de asteroide será minerável, caso contrário será não minerável
    JNZ n_mineravel
    MOV R2, DEF_ASTEROIDE_MINERAVEL ; endereço do endereço do tipo de asteroide minerável
    JMP sai_evoca_asteroide
n_mineravel:
    MOV R2, DEF_ASTEROIDE_N_MINERAVEL ; endereço do endereço do tipo de asteroide não minerável
sai_evoca_asteroide:
    MOV [R0], R2                    ; escreve o tipo do asteroide
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  VALOR_ALEATORIO: Rotina que gera um valor aleatório entre 0 e 3 e um
;#                   endereço aleatório da tabela de pares coluna-direção
;#  
;#  SAÍDAS: R3 - valor aleatório entre 0 e 3
;#          R1 - valor do endereço de um par coluna-direção
;#
;#############################################################################################################################

valor_aleatorio:
    PUSH R0                         ; guarda registos
    PUSH R2
    MOV R0, TEC_COL                 ; endereço do periférico de saída (colunas)
    MOVB R1, [R0]                   ; lê do periférico das colunas
    SHR R1, 4                       ; deslocamento para a direita de 4 bits (4 bits aleatórios)
    MOV R3, R1                      ; registo auxiliar
    MOV R0, MASCARA_2               ; máscara para isolar os 2 bits de menor peso
    AND R3, R0                      ; isola os dois bits de menor peso (valor de 0 a 3 que decidirá o tipo do asteroide)
    MOV R2, 5                       
    MOD R1, R2                      ; obtém resto da divisão inteira por 5
    MOV R2, 4
    MUL R1, R2                      ; cada par são 2 WORDs (4 Bytes)
    MOV R0, PARES_COL_DIR           ; endereço base da tabela
    ADD R1, R0                      ; obtém um endereço da tabela
    POP R2                          ; recupera registos
    POP R0
    RET

;#############################################################################################################################
;#  DESENHA_ASTEROIDES: Rotina que desenha os 4 asteroides no ecrã
;#
;#############################################################################################################################

desenha_asteroides:
    PUSH R1                         ; guarda registos
    PUSH R3
    MOV R3, estado_jogo             ; endereço do estado de jogo
    MOV R1, [R3]                    ; obtém estado de jogo
    CMP R1, EM_JOGO                 ; verifica se o estado de jogo é o correto (pode ter se dado uma colisão com o painel)
    JNZ sai_desenha_asteroides      ; retorna caso o estado de jogo não seja o correto
    MOV R3, ASTEROIDE_1             ; endereço do asteroide 1
    CALL obtem_info_asteroide       ; obtém informação sobre o asteroide 1 e desenha-o no ecrã
    MOV R3, ASTEROIDE_2             ; endereço do asteroide 2
    CALL obtem_info_asteroide       ; obtém informação sobre o asteroide 2 e desenha-o no ecrã
    MOV R3, ASTEROIDE_3             ; endereço do asteroide 3
    CALL obtem_info_asteroide       ; obtém informação sobre o asteroide 3 e desenha-o no ecrã
    MOV R3, ASTEROIDE_4             ; endereço do asteroide 4
    CALL obtem_info_asteroide       ; obtém informação sobre o asteroide 4 e desenha-o no ecrã
sai_desenha_asteroides:
    POP R3                          ; recupera registos
    POP R1
    RET

;#############################################################################################################################
;#  OBTEM_INFO_ASTEROIDE: Rotina que obtém informação sobre o asteroide do
;#                        qual recebeu o endereço e desenha-o no ecrã 
;#
;#  ENTRADAS: R3 - endereço do asteroide
;#
;#############################################################################################################################

obtem_info_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    MOV R0, [R3]                    ; obtém linha
    ADD R3, 2                       ; endereço da coluna do asteroide
    MOV R1, [R3]                    ; obtém coluna
    ADD R3, 4                       ; endereço do tipo de asteroide
    MOV R2, [R3]                    ; obtém endereço do endereço do tipo de asteroide
    CALL desenha_asteroide          ; desenha o asteroide
    POP R3                          ; recupera registos
    POP R2
    POP R1
    POP R0
    RET

;#############################################################################################################################
;#  DESENHA_ASTEROIDE: Rotina que desenha um de asteroide do ecrã
;#
;#  ENTRADAS: R2 - endereço do tipo de asteroide a desenhar
;#            R0 - linha do pixel de referência do asteroide
;#            R1 - coluna do pixel de referência do asteroide
;#  
;#############################################################################################################################

desenha_asteroide:
    PUSH R0                         ; guarda registos                       
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R3, [R2]                    ; obtém largura do asteroide
    ADD R2, 2                       ; endereço da altura do asteroide
    MOV R4, [R2]                    ; obtém altura do asteroide
L0:
    CMP R4, 0                       ; verifica se já desenhou todas as linhas
    JZ sai_desenha_asteroide        ; condição de paragem
    CALL desenha_linha              ; desenha a linha do meteoro
    ADD R0, 1                       ; linha seguinte
    SUB R4, 1                       ; subtrai 1 ao número de linhas por desenhar
    JMP L0                          ; continua ciclo
sai_desenha_asteroide:
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0                        
    RET

;#############################################################################################################################
;#  APAGA_ASTEROIDE: Rotina que apaga um tipo de asteroide do ecrã
;#
;#  ENTRADAS: R2 - endereço do tipo de asteroide a desenhar
;#            R0 - linha do pixel de referência do asteroide
;#            R1 - coluna do pixel de referência do asteroide
;#
;#############################################################################################################################

apaga_asteroide:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4                         
    MOV R3, [R2]                    ; obtém largura do asteroide
    ADD R2, 2                       ; endereço da altura do asteroide
    MOV R4, [R2]                    ; obtém altura do asteroide
L1:
    CMP R4, 0                       ; verifica se todas as linhas já foram apagadas
    JZ sai_apaga_asteroide          ; condição de paragem
    CALL apaga_linha                ; apaga linha do asteroide
    ADD R0, 1                       ; linha seguinte
    SUB R4, 1                       ; subtrai 1 ao número de linhas por apagar
    JMP L1                          ; continua ciclo
sai_apaga_asteroide:
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0                         
    RET

;#############################################################################################################################
;#  DESENHA_PAINEL: Rotina que desenha o painel de instrumentos no ecrã
;#
;#############################################################################################################################

desenha_painel:
    PUSH R0                         ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R0, LINHA_TOPO_PAINEL       ; linha superior do painel
    MOV R1, COLUNA_PAINEL           ; coluna do lado esquerdo do painel
    MOV R2, DEF_PAINEL              ; endereço da largura do painel
    MOV R3, [R2]                    ; obtém largura do painel
    ADD R2, 2                       ; obtém endereço da altura do painel
    MOV R4, [R2]                    ; obtém altura do painel
L2:
    CMP R4, 0                       ; verifica se já desenhou todas as linhas
    JZ sai_desenha_painel           ; condição de paragem do ciclo 
    CALL desenha_linha              ; desenha a linha
    ADD R0, 1                       ; linha seguinte
    SUB R4, 1                       ; subtrai 1 ao número de linhas por desenhar
    JMP L2                          ; continua ciclo
sai_desenha_painel:
    POP R4                          ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0
    RET 


;#############################################################################################################################
;#  DESENHA_LINHA: Rotina que desenha uma linha no display 
;#   
;#  ENTRADAS: R3 - largura da linha a desenhar  R2- endereço anterior ao da 1ª cor dos pixeis
;#            R1 - coluna inicial do boneco a desenhar
;#
;#############################################################################################################################

desenha_linha:
    PUSH R1                         ; guarda registos
    PUSH R3
    PUSH R5
L4:
    CMP R3, 0                       ; verifica se já desenhou todos os pixeis da linha
    JZ sai_desenha_linha            ; condição de paragem do ciclo
    ADD R2, 2                       ; endereço da cor do pixel
    MOV R5, [R2]                    ; obtém cor do pixel a desenhar
    CALL escreve_pixel              ; escreve pixel
    ADD R1, 1                       ; soma 1 ao valor da coluna a desenhar
    SUB R3, 1                       ; subtrai 1 ao número de pixeis a desenhar na linha
    JMP L4                          ; continua ciclo
sai_desenha_linha:
    POP R5                          ; recupera registos
    POP R3
    POP R1                         
    RET

;#############################################################################################################################
;#  APAPGA_LINHA: Rotina que apaga uma linha de um determinado boneco
;#
;#  ENTRADAS: R1 - coluna inicial do boneco  R3 - largura da linha a apagar
;#
;#############################################################################################################################

apaga_linha:
    PUSH R1                         ; guarda registos
    PUSH R3
    PUSH R5                        
L5:
    CMP R3, 0                       ; verifica se já desenhou (apagou) todos os pixeis da linha
    JZ sai_apaga_linha              ; condição de paragem do ciclo
    MOV R5, 0                       ; obtém cor do pixel a desenhar
    CALL escreve_pixel              ; escreve pixel
    ADD R1, 1                       ; soma 1 ao valor da coluna a apagar
    SUB R3, 1                       ; subtrai 1 ao número de pixeis a apagar na linha
    JMP L5                          ; continua ciclo
sai_apaga_linha:
    POP R5                          ; recupera registos
    POP R3
    POP R1                         
    RET

;#############################################################################################################################
;#  ESCREVE_PIXEL: Rotina que desenha um pixel no display 
;#   
;#  ENTRADAS: R0 - linha do ecrã  R1 - coluna do ecrã  R5 - cor do pixel
;#     
;#############################################################################################################################

escreve_pixel:
	MOV  [DEF_LINHA], R0		    ; seleciona a linha
	MOV  [DEF_COLUNA], R1		    ; seleciona a coluna
	MOV  [DEF_PIXEL], R5		    ; altera a cor do pixel na linha e coluna selecionadas
	RET

;#############################################################################################################################
;#  ROT_INT_0: Rotina da interrupção 0, esta simplesmente escreve na variável
;#             LOCK que outros processos irão ler (indica apenas que a
;#             interrupção ocorreu)
;#
;#############################################################################################################################

rot_int_0:
    PUSH R1                         ; guarda registos
    MOV R1, evento_int
    MOV [R1 + 0], R0                ; escreve no LOCK (valor do registo é irrelevante)
    POP R1                          ; recupera registos
    RFE

;#############################################################################################################################
;#  ROT_INT_1: Rotina da interrupção 1, esta simplesmente escreve na variável
;#             LOCK que outros processos irão ler (indica apenas que a
;#             interrupção ocorreu)
;#
;#############################################################################################################################

rot_int_1:
    PUSH R1                         ; guarda registos
    MOV R1, evento_int
    MOV [R1 + 2], R0                ; escreve no LOCK (valor do registo é irrelevante)
    POP R1                          ; recupera registos
    RFE

;#############################################################################################################################
;#  ROT_INT_2: Rotina da interrupção 2, esta simplesmente escreve na variável
;#             LOCK que outros processos irão ler (indica apenas que a
;#             interrupção ocorreu)
;#
;#############################################################################################################################

rot_int_2:
    PUSH R1                         ; guarda registos
    MOV R1, evento_int
    MOV [R1 + 4], R0                ; escreve no LOCK (valor do registo é irrelevante)
    POP R1                          ; recupera registos
    RFE

;#############################################################################################################################
;#  ROT_INT_3: Rotina da interrupção 3, esta simplesmente escreve na variável
;#             LOCK que outros processos irão ler (indica apenas que a
;#             interrupção ocorreu)
;#
;#############################################################################################################################

rot_int_3:
    PUSH R1                         ; guarda registos
    MOV R1, evento_int
    MOV [R1 + 6], R0                ; escreve no LOCK (valor do registo é irrelevante)
    POP R1                          ; recupera registos
    RFE