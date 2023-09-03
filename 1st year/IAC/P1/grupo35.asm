; ############################################################################################################################
; #  Grupo-35
; #  Bogdan Prokopiuk Nº104016
; #  João Maria Pereira Parreira Cota Teixeira Nº97226
; #  Francisco Miguel Martins Mónica Nº 106583
; #
; #  Teclas:
; #       -> '1': movimenta a sonda uma linha para cima  
; #       -> '3': movimenta o asteroide uma linha na diagonal para baixo
; #       -> '5': incrementa uma unidade o valor nos Displays
; #       -> '6': diminui uma unidade o valor nos Displays
; #       -> 'C': inicia o jogo
; #       -> 'D': pausa o jogo
; #       -> 'E': acaba o jogo
; #   
; #  Projeto 2022-23 Versão Intermédia
; ############################################################################################################################

;#############################################################################################################################
;#   Constantes 
;#############################################################################################################################

DISPLAYS            EQU 0A000H     ; endereço dos Displays de números (POUT-1)
TEC_LINHA           EQU 0C000H     ; endereço das Linhas do teclado (POUT-2)
TEC_COLUNA          EQU 0E000H     ; endereço das Colunas do teclado (PIN)
LINHA               EQU 8          ; linha a testar do teclado inicialmente (4ª linha, 1000b)
MASCARA             EQU 0FH        ; isola os 4 bits de menor peso

DEF_REMOVE_FUNDO    EQU 6040H      ; endereço do comando para eliminar a imagem de fundo e o aviso "No background selected"
DEF_FUNDO           EQU 6042H      ; endereço do comando para alterar a imagem de fundo
APAGA_PIXELS        EQU 6002H      ; endereço para apagar todos os pixeis do ecrã
DEF_LINHA    		EQU 600AH      ; endereço do comando para definir a linha
DEF_COLUNA   		EQU 600CH      ; endereço do comando para definir a coluna
DEF_PIXEL    		EQU 6012H      ; endereço do comando para escrever um pixel
DEF_SOM             EQU 605AH      ; endereço do comando para adicionar um efeito sonoro

MIN_LINHA           EQU 0          ; limite superior do ecrã (linha 0)

LINHA_SONDA         EQU 26         ; linha inicial da sonda
COLUNA_SONDA        EQU 32         ; coluna inicial da sonda

LINHA_TOPO_PAINEL   EQU 27         ; linha superior do painel de instrumentos
COLUNA_PAINEL       EQU 25         ; coluna que delimita o lado esquerda do painel de instrumentos

LINHA_METEORO       EQU 0          ; linha inicial do meteoro (topo esquerdo do ecrã)
COLUNA_METEORO      EQU 0          ; coluna inicial do meteoro (topo esquerdo do ecrã)

LARGURA_PAINEL      EQU 15         ; largura do painel de instrumentos
ALTURA_PAINEL       EQU 5          ; altura do painel de instrumentos

LARGURA_METEORO     EQU 5          ; largura do meteoro
ALTURA_METEORO      EQU 5          ; altura do meteoro

VERMELHO            EQU 0FF00H     ; cor vermelha em ARGB (opaco e vermelho no máximo, verde e azul a 0)
ROXO                EQU 0FF0FH     ; cor roxa em ARGB (opaco, vermelho e azul no máximo, verde a 0)
VERDE               EQU 0F0F0H     ; cor verde em ARGB (opaco e verde no máximo, azul e vermelho a 0)
CINZENTO            EQU 0F777H     ; cor cinzenta em ARGB (opaco no máximo, vermelho, verde e azul a 7)
AMARELO             EQU 0FFF0H     ; cor amarela em ARGB (opaco vermelho e verde no máximo, azul a 0)
AZUL                EQU 0F0FFH     ; cor azul claro em ARGB (opaco, verde e azul no máximo, vermelho a 0)
COR_INT_PAINEL      EQU 0FBB8H     ; cor do interior do painel em ARGB (opaco no máximo e verde e vermelho a B e azul a 8)

ENERGIA_INICIAL     EQU 0100H      ; valor inicial da energia da nave 

;#############################################################################################################################
;#   Zona de Dados e inicialização da pilha
;#############################################################################################################################
	PLACE       1000H
pilha:
	STACK 100H			     ; espaço reservado para a pilha (200H bytes ou 100H words)
SP_inicial:				     ; O SP é inicializado com o endereço 1200H (O 1.º end. de retorno será armazenado em 11FEH (1200H-2) )

energia:
     WORD ENERGIA_INICIAL          ; armazenar no endereço 1200H a constante com o valor inicial de energia

DEF_METEORO_MINERAVEL:
     WORD LINHA_METEORO            ; linha onde se encontra o pixel de referência do meteoro 
     WORD COLUNA_METEORO           ; coluna onde se encontra o pixel de referência do  meteoro
     
     WORD LARGURA_METEORO          ; largura do meteoro
     WORD ALTURA_METEORO           ; altura do meteoro

; --- cores dos pixeis de cada linha ---

     WORD 0, VERDE, VERDE, VERDE, 0

     WORD VERDE, VERDE, VERDE, VERDE, VERDE

     WORD VERDE, VERDE, VERDE, VERDE, VERDE

     WORD VERDE, VERDE, VERDE, VERDE, VERDE

     WORD 0, VERDE, VERDE, VERDE, 0

DEF_SONDA:
     WORD LINHA_SONDA              ; linha onde se encontra a sonda
     WORD COLUNA_SONDA             ; coluna onde se encontra a sonda

     WORD ROXO                     ; cor do pixel da sonda

DEF_PAINEL:
     WORD LARGURA_PAINEL           ; largura do painel
     WORD ALTURA_PAINEL            ; altura do painel

; --- cores dos pixeis de cada linha --- 
    
    WORD 0, 0, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, VERMELHO, 0, 0
    
    WORD 0, VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL , VERMELHO, 0

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, CINZENTO, VERMELHO, VERDE, CINZENTO, VERDE, CINZENTO, CINZENTO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERDE, CINZENTO, VERMELHO, VERDE, AMARELO, AZUL, CINZENTO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

    WORD VERMELHO, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, COR_INT_PAINEL, VERMELHO

;#############################################################################################################################
;#   Início do programa
;#############################################################################################################################
     
     PLACE   0H
inicia_SP:
     MOV  SP, SP_inicial            ; inicializa SP para a palavra a seguir à última da pilha

;#############################################################################################################################
;#   Inicia o display de energia a zeros
;#############################################################################################################################

inicia_display_energia:
    MOV R1, 0
   MOV  R4, DISPLAYS               ; endereço do periférico dos displays
    MOV [R4], R1                   ; escreve o valor da coluna e linha a zeros no display

;#############################################################################################################################
;#   Inicia o display do media center com a imagem de fundo de introdução 
;#############################################################################################################################

inicia_media:
    MOV R1, 0        
    MOV [DEF_REMOVE_FUNDO], R1     ; remove a mensagem de aviso do media center
    MOV [DEF_FUNDO], R1            ; escreve no display a imagem de fundo de introdução (imagem número 0)

;#############################################################################################################################
;#   Loop de início de jogo (espera até que a tecla 'C' seja premida) 
;#############################################################################################################################

prepara_teclado:
    MOV  R2, TEC_LINHA             ; endereço do periférico das linhas
    MOV  R3, TEC_COLUNA            ; endereço do periférico das colunas
    MOV  R5, MASCARA               ; para isolar os 4 bits de menor peso, ao ler as colunas do teclado

espera_inicio: 
     MOV R1, LINHA                 ; testar a linha 4 onde está a tecla 'C'
     MOVB [R2], R1                 ; escrever no periférico de saída (linhas)
     MOVB R0, [R3]                 ; lê do periférico de entrada (colunas)
     AND  R0, R5                   ; elimina bits para além dos bits 0-3
     SHL  R1, 4                    ; coloca linha no nibble high (os primeiros 4 dígitos binários ficam a zeros)
     OR   R1, R0                   ; junta coluna (coloca o resultado da coluna no nibble low de R1)
     MOV  R6, 0081H                ; verifica se a tecla 'C' foi premida ('C' corresponde a 0081H) 
     XOR  R1, R6                   ; compara a tecla premida com a tecla 'C', se forem iguais inicia-se o jogo
     JZ ha_tecla_inicio            ; se a tecla for premida o jogo começa 
     JMP espera_inicio             ; senão então espera até o comando 'C'

ha_tecla_inicio:                   ; espera até que a tecla de inicio de jogo deixe de ser premida
     MOV R1, LINHA                 ; testa a linha 4 (R1 foi alterado)
     MOVB [R2], R1                 ; escreve no  periférico de saída (linhas)
     MOVB R0, [R3]                 ; lê do periférico de saída (colunas)
     AND R0, R5                    ; aplica a máscara (isola bits de menor peso)
     CMP R0, 0                     ; há tecla premida?
     JNZ ha_tecla_inicio           ; se a tecla de inicio de jogo continua premida, continua a esperar

;#############################################################################################################################
;#   Começa o jogo colocando a imagem de fundo do jogo e desenhando o painel de
;#   instrumentos, o meteoro e a sonda nas suas posições iniciais  
;#############################################################################################################################

comeca_jogo:
     MOV R1, ENERGIA_INICIAL       ; inicia o display com a energia no máximo
     MOV [DISPLAYS], R1            
     MOV R1, 1
     MOV [DEF_FUNDO], R1           ; escreve no display a tela de fundo do jogo (imagem de fundo número 1)     
     CALL desenha_painel           ; desenha no display o painel de instrumentos
     CALL desenha_sonda            ; desenha a sonda no display
     CALL desenha_meteoro          ; desenha o meteoro no topo esquerdo do ecrã
     MOV R6, LINHA                 ; começa a ler o teclado na 4ª linha

;#############################################################################################################################
;#   Lê o input das teclas e processa as suas funções 
;#############################################################################################################################          

leitura:
    MOV  R1, R6                    ; testa a linha 4
    MOVB [R2], R1                  ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]                  ; ler do periférico de entrada (colunas)
    AND  R0, R5                    ; elimina bits para além dos bits 0-3
    CMP  R0, 0                     ; há tecla premida?    
    JZ   proxima_linha             ; se nenhuma foi tecla premida, repete a leitura, mas noutra linha
    SHL  R1, 4                     ; coloca linha no nibble high (os primeiros 4 dígitos binários ficam a zeros)
    OR   R1, R0                    ; junta coluna (coloca o resultado da coluna no nibble low de R1)
    CALL Q_tecla                   ; identifica a tecla premida e processa a sua função

ha_tecla:                          ; espera-se até que nenhuma tecla esteja premida
     MOV R1, R6                    ; testa-se a linha da tecla processada (R1 foi alterado)
     MOVB [R2], R1                 ; escreve no periférico de saída (linhas)
     MOVB R0, [R3]                 ; lê do periférico de entrada (colunas)
     AND  R0, R5                   ; elimina bits para além dos bits 0-3
     CMP  R0, 0                    ; há tecla premida?     
     JNZ ha_tecla                  ; se ainda estiver uma tecla premida, espera até não haver
     JMP leitura                   ; volta a ler o teclado

proxima_linha:
    SHR R6, 1                      ; diminui o valor para poder analizar a linha seguinte (divide por 2)
    JNZ leitura                    ; o loop continua até que o valor da linha seja zero, nesse caso volta-se a testar a linha 4
    MOV R6, LINHA                  ; volta a testar a linha 4
    JMP leitura                    ; continua leitura

;#############################################################################################################################
;#   Q_TECLA: Rotina que avalia qual foi a tecla premida e processa a sua função
;#   utilizando rotinas complementares
;#
;#   ENTRADAS R1 - valor da tecla premida
;#############################################################################################################################

Q_tecla:
     PUSH R1                       ; guarda registos
     PUSH R2
     PUSH R4
eh_1:
     MOV R4, 0012H                 ; valor da tecla 1
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ eh_3                      ; se o valor da tecla não corresponder à tecla 1 testa outra tecla
     CALL move_sonda_cima          ; processa a tecla 1 (move sonda)
     JMP sai_q                     ; retorna para leitura do teclado após recuperar registos
eh_3:
     MOV R4, 0018H                 ; valor da tecla 3
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ eh_5                      ; se o valor da tecla não corresponder à tecla 3 testa outra tecla
     CALL move_meteoro_diagonal    ; processa função da tecla 3 (move asteroide na diagonal)
     JMP sai_q                     ; retorna para a leitura do teclado após recuperar registos
eh_5:
     MOV R4, 0022H                 ; valor da tecla 5
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ eh_6                      ; se o valor da tecla não corresponder à tecla 5 testa outra tecla
     CALL incrementa_energia       ; processa a tecla 5 (incrementar o valor no display de números)
     JMP sai_q                     ; recupera registos e retorna para leitura
eh_6:
     MOV R4, 0024H                 ; valor da tecla 6
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ eh_d                      ; se o valor da tecla não corresponder à tecla 6 testa outra tecla
     CALL decrementa_energia       ; processa a tecla 6 (diminuir o valor no display de números)
     JMP sai_q                     ; recupera registos e retorna para leitura
eh_d:
     MOV R4, 0082H                 ; valor da tecla D
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ eh_e                      ; se o valor da tecla não correponde à tecla 'D' testa outra tecla 
     CALL pausa_jogo               ; processa tecla 'D' (pausa o jogo)
     JMP sai_q                     ; recupera registos e retorna para leitura
eh_e:
     MOV R4, 0084H                 ; valor da tecla E
     MOV R2, R1                    ; copia valor da tecla lida para um registo auxiliar (XOR altera o valor do registo)
     XOR R2, R4                    ; compara os valores para reconhecer a tecla
     JNZ sai_q                     ; se a tecla não corresponde à tecla 'E', volta a ler o teclado
     JMP fim_jogo                  ; processa tecla (acaba jogo) 
sai_q:                             
     POP R4                        ; recupera registos
     POP R2
     POP R1
     RET

;#############################################################################################################################
;#   INCREMENTA_ENERGIA: Rotina que processa a função da tecla '5' (incrementar o valor nos displays uma unidade) 
;#############################################################################################################################

incrementa_energia:
     PUSH R1                       ; guarda registos
     PUSH R2
     PUSH R4
     MOV R2, energia               ; endereço da energia da nave
     MOV R1, [R2]                  ; lê do endereço a energia da nave
     MOV R4, DISPLAYS              ; endereço dos displays de energia
     ADD R1, 0001H                 ; incrementa o valor de energia em uma unidade
     MOV [R4], R1                  ; escreve novo valor nos displays 
     MOV [R2], R1                  ; escreve novo valor de energia no endereço respetivo                             
     POP R4                        ; recupera registos
     POP R2
     POP R1
     RET

;#############################################################################################################################
;#   DECREMENTA_ENERGIA: Rotina que processa a função da tecla '6' (decrementar o valor nos displays uma unidade) 
;#############################################################################################################################

decrementa_energia:
     PUSH R1                       ; guarda registos
     PUSH R2
     PUSH R4
     MOV R2, energia               ; endereço da energia da nave
     MOV R1, [R2]                  ; lê do endereço a energia da nave
     MOV R4, DISPLAYS              ; endereço dos displays de energia
     SUB R1, 0001H                 ; decrementa o valor de energia em uma unidade
     MOV [R4], R1                  ; escreve novo valor nos displays 
     MOV [R2], R1                  ; escreve novo valor de energia no endereço respetivo
     CMP R1, 0                     ; verifica se o valor da energia chegou a 0
     JZ sem_energia                ; verifica se o valor da energia chegou a 0     
     POP R4                        ; recupera registos
     POP R2
     POP R1
     RET

sem_energia:                       ; nave sem energia, acaba o jogo
     MOV R1, ENERGIA_INICIAL       ; reinicia o valor de energia
     MOV [energia], R1             ; guarda valor de energia no respetivo endereço
     JMP fim_jogo                  ; acaba jogo

;#############################################################################################################################
;#   Acaba o jogo quando a nave fica sem energia
;#############################################################################################################################

fim_jogo:
     MOV R1, 0
     MOV [APAGA_PIXELS], R1        ; apaga todos os pixels no ecrã
     MOV R1, 3
     MOV [DEF_FUNDO], R1           ; escreve no ecrã a imagem de fundo de game over (imagem número 3) 
     MOV R1, DEF_SONDA             ; endereço da linha onde se encontra a sonda
     MOV R2, LINHA_SONDA           ; linha inicial da sonda
     MOV [R1], R2                  ; reinicia a linha da sonda
     ADD R1, 2                     ; endereço da coluna onde se encontra a sonda
     MOV R2, COLUNA_SONDA          ; coluna inicial da sonda
     MOV [R1], R2                  ; reinicia a colunada sonda
     MOV R1, DEF_METEORO_MINERAVEL ; endereço da linha do pixel de referência do meteoro
     MOV R2, LINHA_METEORO         ; linha inicial do pixel de referência do meteoro
     MOV [R1], R2                  ; reinicia a posição do meteoro (linha)
     ADD R1, 2                     ; obtém endereço da coluna do pixel de referência do meteoro
     MOV R2, COLUNA_METEORO        ; coluna inicial do meteoro
     MOV [R1], R2                  ; reinicia posição do meteoro (coluna)
     MOV SP, SP_inicial            ; reinicia o stack pointer
     JMP prepara_teclado           ; espera que se volte a premir a tecla 'C' para recomeçar o jogo

;#############################################################################################################################
;#   PAUSA_JOGO: Esta rotina processa a tecla 'D' põe o jogo em pausa e se clicar novamente
;#               na tecla o jogo recomeça no ponto em que foi pausado 
;#############################################################################################################################

pausa_jogo:
     PUSH R1                       ; guarda registos 
     PUSH R2
     PUSH R4
     MOV R1, 0
     MOV [APAGA_PIXELS], R1        ; apaga todos os pixeis do ecrã 
     MOV R1, 2
     MOV [DEF_FUNDO], R1           ; seleciona o a imagem de fundo de pausa

ha_tecla_paused:                   ; espera que a tecla 'D' deixe de ser premida
     MOV R1, LINHA                 ; testa a linha 4
     MOV R2, TEC_LINHA             ; endereço do periférico de saída (linhas)
     MOVB [R2], R1                 ; escreve a linha a testar no periférico de saída (linhas)
     MOV R2, TEC_COLUNA            ; endereço do periférico de saída (colunas)
     MOVB R4, [R2]                 ; lê do periférico de entrada (colunas)
     MOV R2, MASCARA               ; máscara para isolar os bits de menor peso
     AND R4, R2                    ; aplica máscara
     CMP R4, 0                     ; há tecla premida?
     JNZ ha_tecla_paused           ; se ainda houver tecla premida continua à espera

espera_pausa:         
     MOV R1, LINHA                 ; testa linha 4
     MOV R2, TEC_LINHA             ; endereço do periférico de saída (linhas)
     MOVB [R2], R1                 ; escreve a linha a testar no periférico de saída (linhas)
     MOV R2, TEC_COLUNA            ; endereço do periférico das colunas
     MOVB R4, [R2]                 ; lê do periférico de entrada (colunas)
     MOV R2, MASCARA               ; máscara para isolar bits de menor peso
     AND R4, R2                    ; aplica máscara
     SHL R1, 4                     ; coloca linha no nibble high (os primeiros 4 dígitos binários ficam a zeros)
     OR R1, R4                     ; junta coluna (coloca o resultado da coluna no nibble low de R1)
     MOV R2, 0082H                 ; valor da tecla 'D'                    
     XOR R1, R2                    ; compara a tecla premida com a tecla 'D', se forem iguais regressa ao jogo
     JNZ espera_pausa              ; se a tecla 'D' não for premida, continua à espera  
     MOV R1, 1
     MOV [DEF_FUNDO], R1           ; escreve no display a tela de fundo do jogo (imagem de fundo número 1)
     CALL desenha_sonda            ; volta a desenhar a sonda
     CALL desenha_painel           ; volta a desenhar o painel de instrumentos
     CALL desenha_meteoro          ; volta a desenhar o meteoro
     POP R4                        ; recupera registos
     POP R2
     POP R1
     RET

;#############################################################################################################################
;#   DESENHA_PAINEL - Rotina que desenha no display o painel de instrumentos
;#############################################################################################################################

desenha_painel:
    PUSH R0                        ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    MOV R0, LINHA_TOPO_PAINEL      ; linha superior do painel
    MOV R1, COLUNA_PAINEL          ; coluna do lado esquerdo do painel
    MOV R2, DEF_PAINEL             ; endereço da largura do painel
    MOV R3, [R2]                   ; obtém largura do painel
    ADD R2, 2                      ; obtém endereço da altura do painel
    MOV R4, [R2]                   ; obtém altura do painel
L1:
     CMP R4, 0                     ; verifica se já desenhou todas as linhas
     JZ sai_desenha_painel         ; condição de paragem do ciclo 
     CALL desenha_linha            ; desenha a linha
     ADD R0, 1                     ; linha seguinte
     SUB R4, 1                     ; subtrai 1 ao número de linhas por desenhar
     JMP L1                        ; continua ciclo
sai_desenha_painel:
    POP R4                         ; recupera registos
    POP R3
    POP R2
    POP R1
    POP R0                          
    RET 

;#############################################################################################################################
;#   DESENHA_SONDA - Rotina que desenha a sonda no ecrã
;#############################################################################################################################

desenha_sonda:
    PUSH R0                        ; guarda registos
    PUSH R1
    PUSH R2
    PUSH R5                         
    MOV R2, DEF_SONDA              ; endereço da linha onde se encontra a sonda
    MOV R0, [R2]                   ; obtém linha onde se encontra a sonda
    ADD R2, 2                      ; endereço da coluna onde se encontra a sonda 
    MOV R1, [R2]                   ; obtém coluna onde se encontra a sonda
    ADD R2, 2                      ; endereço da cor da sonda
    MOV R5, [R2]                   ; obtém cor da sonda
    CALL escreve_pixel             ; desenha a sonda
    POP R5                         ; recupera registos
    POP R2
    POP R1
    POP R0                          
    RET
    
;#############################################################################################################################
;#   MOVE_SONDA_CIMA - Rotina que move a sonda para cima no display
;#############################################################################################################################

move_sonda_cima:
     PUSH R2                       ; guarda registos
     PUSH R0                        
     CALL apaga_sonda              ; apaga a sonda do ecrã
     MOV R2, DEF_SONDA             ; endereço da linha onde se encontra a sonda
     MOV R0, [R2]                  ; obtém a linha onde se encontra a sonda
     SUB R0, 1                     ; subtrai 1 ao valor da linha da sonda
     CALL limite_superior_sonda    ; testa se a sonda chegou ao limite superior do ecrã
     MOV [R2], R0                  ; escreve novo valor da linha da sonda
     CALL desenha_sonda            ; desenha a sonda na sua nova posição
     POP R0                        ; recupera registos
     POP R2                         
     RET

;#############################################################################################################################
;#   LIMITE_SUPERIOR_SONDA - Rotina que testa se a sonda chegou ao limite superior do ecrã
;#
;#   ENTRADAS: R0 - valor da linha da sonda a testar
;#   SAÍDAS: R0 - valor da linha depois de ser testado
;#############################################################################################################################

limite_superior_sonda:
     CMP R0, MIN_LINHA             ; compara o valor da linha da sonda com o do limite superior do ecrã
     JGE sai_testa_sonda           ; se o valor da linha está dentro do ecrã então não precisa de ser alterado
reset_sonda:
     MOV R0, LINHA_SONDA           ; altera o valor de R0, que depois irá escrever o novo valor no endereço respetivo 
sai_testa_sonda:
     RET                              

;#############################################################################################################################
;#   APAGA_SONDA - Rotina que apaga a sonda do ecrã
;#############################################################################################################################

apaga_sonda:
     PUSH R0                       ; guarda registos
     PUSH R1
     PUSH R2
     PUSH R5                        
     MOV R2, DEF_SONDA             ; endereço da linha onde se encontra a sonda
     MOV R0, [R2]                  ; obtém linha onde se encontra a sonda
     ADD R2, 2                     ; endereço da coluna onde se encontra a sonda
     MOV R1, [R2]                  ; obtém coluna onde se encontra a sonda
     MOV R5, 0                     ; cor do pixel a desenhar: sem cor (apaga o pixel)
     CALL escreve_pixel            ; apaga o pixel do ecrã
     POP R5                        ; recupera registos
     POP R2
     POP R1
     POP R0
     RET

;#############################################################################################################################
;#   DESENHA_METEORO - Rotina que desenha o meteoro no display
;#############################################################################################################################

desenha_meteoro:
     PUSH R0                       ; guarda registos
     PUSH R1
     PUSH R2
     PUSH R3
     PUSH R4                         
     MOV R2, DEF_METEORO_MINERAVEL ; endereço da linha do pixel de referência do meteoro
     MOV R0, [R2]                  ; obtém linha onde do pixel de referência do meteoro
     ADD R2, 2                     ; endereço da coluna do pixel de referência do meteoro
     MOV R1, [R2]                  ; obtém coluna do pixel de referência do meteoro
     ADD R2, 2                     ; obtém endereço da largura do meteoro
     MOV R3, [R2]                  ; obtém largura do meteoro
     ADD R2, 2                     ; endereço da altura do meteoro
     MOV R4, [R2]                  ; obtém altura do meteoro
L3:
     CMP R4, 0                     ; verifica se já desenhou todas as linhas
     JZ sai_desenha_meteoro        ; condição de paragem
     CALL desenha_linha            ; desenha a linha do meteoro
     ADD R0, 1                     ; linha seguinte
     SUB R4, 1                     ; subtrai 1 ao número de linhas por desenhar
     JMP L3                        ; continua ciclo
sai_desenha_meteoro:
     POP R4                        ; recupera registos
     POP R3
     POP R2
     POP R1
     POP R0                        
     RET

;#############################################################################################################################
;#   MOVE_METEORO_DIAGONAL - Rotina que move o metoro na diagonal no display
;#############################################################################################################################

move_meteoro_diagonal:
     PUSH R0                       ; guarda registos
     PUSH R1
     PUSH R2
     PUSH R3
     MOV R3, 0
     MOV [DEF_SOM], R3             ; reproduz som do metoro a mexer (som número 0)
     CALL apaga_meteoro            ; apaga meteoro do ecrã
     MOV R2, DEF_METEORO_MINERAVEL ; endereço da linha do pixel de referência do meteoro
     MOV R0, [R2]                  ; obtém linha do pixel de referência do meteoro
     ADD R2, 2                     ; endereço da coluna do pixel de referência do meteoro
     MOV R1, [R2]                  ; obtém coluna do pixel de referência do meteoro
     ADD R0, 1                     ; adiciona 1 ao valor da linha do pixel de referência do meteoro
     ADD R1, 1                     ; adiciona 1 ao valor da coluna do pixel de referência do meteoro
     CALL limite_inferior_meteoro  ; verifica se o meteoro colidiu com a nave
     MOV [R2], R1                  ; escreve novo valor da coluna do pixel de referência do meteoro
     SUB R2, 2                     ; obtém endereço da linha do pixel de referência do meteoro
     MOV [R2], R0                  ; escreve novo valor da linha do pixel de referência do meteoro
     CALL desenha_meteoro          ; desenha meteoro na sua nova posição
     POP R3                        ; recupera registos
     POP R2
     POP R1
     POP R0
     RET

;#############################################################################################################################
;#   LIMITE_INFERIOR_METEORO - Rotina que testa se o meteoro colidiu com a nave
;#   ENTRADAS: R0 - valor da linha do meteoro a testar      R1 - valor da coluna do meteoro
;#   SAÍDAS: R0 - valor da linha depois de ser testado      R1 - valor da coluna após teste
;#############################################################################################################################

limite_inferior_meteoro:
     PUSH R2                       ; guarda registos  
     PUSH R3
     MOV R3, LINHA_TOPO_PAINEL     ; linha superior do painel de instrumentos
     MOV R2, R0                    ; copia o valor da linha do meteoro 
     ADD R2, ALTURA_METEORO        ; adiciona altura do meteoro para que o registo passe a ser a linha inferior do meteoro 
     CMP R2, R3                    ; compara a linha inferior do meteoro com a linha superior do painel de instrumentos
     JLE sai_testa_meteoro         ; se o valor da linha está dentro do ecrã então não precisa de ser alterado
reset_meteoro:
     MOV R0, LINHA_METEORO         ; altera o valor para o valor inicial da linha do meteoro
     MOV R1, COLUNA_METEORO        ; altera o valor para o valor inicial da coluna do meteoro
sai_testa_meteoro:
     POP R3                        ; recupera registos
     POP R2
     RET

;#############################################################################################################################
;#   APAPGA_METEORO - Rotina que apaga o meteoro do display
;#############################################################################################################################

apaga_meteoro:
     PUSH R0                        ; guarda registos
     PUSH R1
     PUSH R2
     PUSH R3
     PUSH R4                         
     MOV R2, DEF_METEORO_MINERAVEL  ; endereço da linha do pixel de referência do meteoro
     MOV R0, [R2]                   ; obtém linha do pixel de referência do meteoro
     ADD R2, 2                      ; endereço da coluna do pixel de referência do meteoro
     MOV R1, [R2]                   ; obtém coluna do pixel de referência do meteoro
     ADD R2, 2                      ; obtém endereço da largura do meteoro
     MOV R3, [R2]                   ; obtém largura do meteoro
     ADD R2, 2                      ; endereço da altura do meteoro
     MOV R4, [R2]                   ; obtém altura do meteoro
L4:
     CMP R4, 0                      ; verifica se todas as linhas já foram apagadas
     JZ sai_apaga_meteoro           ; condição de paragem
     CALL apaga_linha               ; apaga linha do meteoro
     ADD R0, 1                      ; linha seguinte
     SUB R4, 1                      ; subtrai 1 ao número de linhas por apagar
     JMP L4                         ; continua ciclo
sai_apaga_meteoro:
     POP R4                         ; recupera registos
     POP R3
     POP R2
     POP R1
     POP R0                         
     RET

;#############################################################################################################################
;#   DESENHA_LINHA - Rotina que desenha uma linha no display 
;#   
;#   ENTRADAS: R3 - largura da linha a desenhar   R2- endereço anterior ao das cores dos pixeis
;#             R1 - coluna inicial do boneco a desenhar
;#############################################################################################################################

desenha_linha:
     PUSH R1                       ; guarda registos
     PUSH R3
     PUSH R5
L2:
     CMP R3, 0                     ; verifica se já desenhou todos os pixeis da linha
     JZ sai_desenha_linha          ; condição de paragem do ciclo
     ADD R2, 2                     ; endereço da cor do pixel
     MOV R5, [R2]                  ; obtém cor do pixel a desenhar
     CALL escreve_pixel            ; escreve pixel
     ADD R1, 1                     ; soma 1 ao valor da coluna a desenhar
     SUB R3, 1                     ; subtrai 1 ao número de pixeis a desenhar na linha
     JMP L2                        ; continua ciclo
sai_desenha_linha:
     POP R5                        ; recupera registos
     POP R3
     POP R1                         
     RET

;#############################################################################################################################
;#   APAPGA_LINHA - Rotina que apaga uma linha do meteoro
;#
;#   ENTRADAS: R1 - coluna inicial do meteoro     R3 - largura da linha a apagar
;#############################################################################################################################

apaga_linha:
     PUSH R1                       ; guarda registos
     PUSH R3
     PUSH R5                        
L5:
     CMP R3, 0                     ; verifica se já desenhou todos os pixeis da linha
     JZ sai_apaga_linha            ; condição de paragem do ciclo
     MOV R5, 0                     ; obtém cor do pixel a desenhar
     CALL escreve_pixel            ; escreve pixel
     ADD R1, 1                     ; soma 1 ao valor da coluna a apagar
     SUB R3, 1                     ; subtrai 1 ao número de pixeis a apagar na linha
     JMP L5                        ; continua ciclo
sai_apaga_linha:
     POP R5                        ; recupera registos
     POP R3
     POP R1                         
     RET

;#############################################################################################################################
;#   ESCREVE_PIXEL - Rotina que desenha um pixel no display 
;#   
;#   ENTRADAS: R0 - linha do ecrã  R1 - coluna do ecrã  R5 - cor do pixel     
;#############################################################################################################################

escreve_pixel:
	MOV  [DEF_LINHA], R0		; seleciona a linha
	MOV  [DEF_COLUNA], R1		; seleciona a coluna
	MOV  [DEF_PIXEL], R5		; altera a cor do pixel na linha e coluna selecionadas
	RET