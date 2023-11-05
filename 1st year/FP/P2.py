## ---P2Minas--- ##

# --TAD gerador-- #

# Gerador representado como dicionário.

# Construtores
def cria_gerador(bits,estado):
    """
    Devolve um gerador que armazena os argumentos recebidos (número de bits e seed).

    Argumentos:
        bits: inteiro (32 ou 64)
        estado: inteiro positivo
    Para além disso, verificam-se os argumentos, gerando um ValueError com a
    mensagem "cria_gerador: argumentos invalidos" em caso de argumentos
    incorretos.

    cria_gerador: int x int --> gerador
    """
    if (not isinstance(bits,int) or not isinstance(estado,int) or estado <= 0 or
        bits not in (32,64) or (bits == 32 and estado > 0xFFFFFFFF) or
       (bits == 64 and estado > 0xFFFFFFFFFFFFFFFF)):
        # O estado recebido nunca pode ser superior ao que o tamanho do seu
        # estado permite representar (se bits=32 então o estado tem de ser menor
        # que 0xFFFFFFFF, ou seja, 2**32-1).        
        raise ValueError ('cria_gerador: argumentos invalidos')
    return {'bits':bits,'estado':estado}


def cria_copia_gerador(gerador):
    """
    Devolve uma cópia nova do gerador recebido.

    Argumentos:
        gerador: gerador

    cria_copia_gerador: gerador --> gerador
    """
    return gerador.copy()


# Seletor
def obtem_estado(gerador):
    """
    Devolve o estado do gerador recebido.

    Argumentos:
        gerador: gerador
    
    obtem_estado: gerador --> int
    """
    return gerador['estado']


# Modificadores
def define_estado(gerador,estado):
    """
    Define o novo valor do estado do gerador recebido como sendo o "estado" recebido e devolve-o.

    Argumentos:
        gerador: gerador
        estado: inteiro 

    define_estado: gerador x int --> int
    """
    gerador['estado'] = estado
    return obtem_estado(gerador)


def atualiza_estado(gerador):
    """
    Atualiza o estado do gerador e devolve-o.
    
    Argumentos:
        gerador: gerador
    O estado do gerador recebido é atualizado de acordo com o algoritmo
    xorshift.
    
    atualiza_estado: gerador --> int
    """
    s = obtem_estado(gerador)
    if gerador['bits'] == 32:
        s ^= (s<<13) & 0xFFFFFFFF
        s ^= (s>>17) & 0xFFFFFFFF
        s ^= (s<<5)  & 0xFFFFFFFF
    else:
        s ^= (s<< 13) &  0xFFFFFFFFFFFFFFFF
        s ^= (s>>7)  &  0xFFFFFFFFFFFFFFFF
        s ^= (s<<17) &  0xFFFFFFFFFFFFFFFF
    return define_estado(gerador,s)


# Reconhecedor
def eh_gerador(argumento):
    """
    Devolve True ou False consoante seja um TAD gerador ou não.
    
    Argumentos:
        argumento: universal

    eh_gerador: universal --> booleano
    """
    return (type(argumento) == dict and len(argumento) == 2 and
            'bits' in argumento and 'estado' in argumento and
            type(argumento['bits']) == int and type(obtem_estado(argumento)) == int and
            obtem_estado(argumento) > 0 and argumento['bits'] in (32,64))


# Teste
def geradores_iguais(gerador1,gerador2):
    """
    Devolve True apenas se os 2 argumentos forem geradores e se forem iguais.
    
    Argumentos:
        gerador1: gerador
        gerador2: gerador

    geradores_iguais: gerador x gerador --> booleano
    """ 
    return (eh_gerador(gerador1) and eh_gerador(gerador2) and
            obtem_estado(gerador1) == obtem_estado(gerador2) and
            gerador1['bits'] == gerador2['bits'])
    

# Transformador
def gerador_para_str(gerador):
    """
    Devolve uma cadeia de carateres que representa o gerador recebido.

    Argumentos:
        gerador: gerador
    No fundo, devolve uma cadeia de carateres que representa o gerador
    recebido, seguindo o seguinte modelo: "xorshit__(s=__)" em que no
    primeiro espaço está o número de bits e no segundo o estado do gerador.

    gerador_para_str: gerador --> str
    """
    return 'xorshift{}(s={})'.format(gerador['bits'],obtem_estado(gerador))


# Funções de alto nível-TAD Gerador
def gera_numero_aleatorio(gerador,n):
    """
    Devolve um número aleatório.

    Argumentos:
        gerador: gerador
        n: inteiro
    No fundo, o estado do gerador recebido é atualizado e devolve-se um
    número aleatório no intervalo de 1 a "n", em que este é obtido através
    do estado do gerador.

    gera_numero_aleatorio: gerador x int --> int
    """
    atualiza_estado(gerador)
    return 1 + (obtem_estado(gerador) % n)


def gera_carater_aleatorio(gerador,carater):
    """
    Devolve um carater aleatório.

    Argumentos:
        gerador: gerador
        carater: cadeia de carateres
    No fundo, o estado do gerador recebido é atualizado e devolve-se um
    carater aleatório  no intervalo de "A" ao carater recebido, em que
    este é obtido através do estado do gerador.
    
    gera_carater_aleatorio: gerador x str --> str
    """
    atualiza_estado(gerador)
    return chr(ord('A') + (obtem_estado(gerador) % (ord(carater) - ord('A') + 1)))




# --TAD coordenada-- #

# Coordenada representada como tuplo.

# Construtor
def cria_coordenada(coluna,linha):
    """
    Devolve a coordenada correspondente à linha e coluna recebidas. 

    Argumentos:
        coluna: cadeia de carateres (A a Z)
        linha: inteiro (1 a 99)
    Para além disso, verificam-se ainda os argumentos recebidos, gerando um
    ValueError com a mensagem "cria_coordenada: argumentos invalidos", caso
    os argumentos sejam incorretos.
    
    cria_coordenada: str x int --> coordenada
    """
    if (not isinstance(coluna,str) or not isinstance(linha,int) or
        len(coluna) != 1 or not 'A' <= coluna <= 'Z' or not 1 <= linha <= 99):
            raise ValueError ('cria_coordenada: argumentos invalidos') 
    return (coluna,linha)


# Seletores
def obtem_coluna(coordenada):
    """
    Devolve a coluna da coordenada recebida.

    Argumentos:
        coordenada: coordenada

    obtem_coluna: coordenada --> str   
    """
    return coordenada[0]


def obtem_linha(coordenada):
    """
    Devolve a linha da coordenada recebida.

    Argumentos:
        coordenada: coordenada

    obtem_linha: coordenada --> int    
    """
    return coordenada[1]


# Reconhecedor
def eh_coordenada(arg):
    """
    Devolve True caso o argumento seja um TAD coordenada e False caso contrário.

    Argumentos:
        arg: universal

    eh_coordenada: universal --> booleano
    """
    return (type(arg) == tuple and len(arg) == 2 and
            type(obtem_coluna(arg)) == str and len(obtem_coluna(arg)) == 1 and
            type(obtem_linha(arg)) == int and 'A' <= obtem_coluna(arg) <= 'Z' and
            1 <= obtem_linha(arg) <= 99)


# Teste
def coordenadas_iguais(coordenada1,coordenada2):
    """
    Devolve True apenas se as duas coordenadas recebidas forem coordenadas e forem iguais, caso contrário devolve False.

    Argumentos:
        coordenada1: coordenada
        coordenada2: coordenada
    
    coordenadas_iguais: coordenada x coordenada --> booleano    
    """
    return (eh_coordenada(coordenada1) and eh_coordenada(coordenada2) and
            obtem_coluna(coordenada1) == obtem_coluna(coordenada2) and
            obtem_linha(coordenada1) == obtem_linha(coordenada2))


# Transformadores
def coordenada_para_str(coordenada):
    """
    Devolve a cadeia de carateres que representa a coordenada recebida.

    Argumentos:
        coordenada: coordenada
    A cadeia de carateres devolvida segue o modelo: "CLL", em que "C" é a
    coluna da mesma e "LL" a sua linha. No caso da linha ser apenas um
    algarismo, esta é representada como "C0L".
    
    coordenada_para_str: coordenada --> str
    """
    return '{}{:02d}'.format(obtem_coluna(coordenada),obtem_linha(coordenada))


def str_para_coordenada(coord_em_string):
    """
    Devolve a coordenada representada pela cadeia de carateres recebida.

    Argumentos:
        coord_em_string: cadeia de carateres
    A coordenada recebida deve ser do tipo "CLL" ou "C0L" em que C diz
    respeito a uma coluna ( de A a Z) e L a uma linha (de 1 a 99).

    str_para_coordenada: str --> coordenada
    """
    if coord_em_string[1] == '0':
        linha = coord_em_string[2]
    else:
        linha = coord_em_string[1:]
    return cria_coordenada(coord_em_string[0],int(linha))


# Funções de alto nível-TAD Coordenada
def obtem_coordenadas_vizinhas(coordenada):
    """
    Devolve um tuplo com as coordenadas vizinhas à coordenada recebida.

    Argumentos:
        coordenada: coordenada
    No tuplo, as coordenadas vizinhas à coordenada recebida estão ordenadas,
    começando-se pela coordenada na diagonal acima-esquerda e seguindo no
    sentido horário.
    
    obtem_coordenadas_vizinhas: coordenada --> tuplo
    """
    coords_vizinhas = ()
    # Coordenadas vizinhas na linha anterior à da recebida.
    for i in range(-1,2):
        coluna = chr(ord(obtem_coluna(coordenada)) + i)
        linha = obtem_linha(coordenada) - 1
        if not 'A' <= coluna <= 'Z' or not 1 <= linha <= 99:
            continue
        coord = cria_coordenada(coluna,linha)
        coords_vizinhas += (coord,)

    # Coordenada vizinha à direita da recebida.
    linha = obtem_linha(coordenada)
    coluna = chr(ord(obtem_coluna(coordenada)) + 1)    
    if 'A' <= coluna <= 'Z' and 1 <= linha <= 99:
        coord = cria_coordenada(coluna,linha)
        coords_vizinhas += (coord,)
    
    # Coordenadas vizinhas na linha seguinte à da recebida.
    for i in range(1,-2,-1):
        coluna = chr(ord(obtem_coluna(coordenada)) + i)
        linha = obtem_linha(coordenada) + 1
        if not 'A' <= coluna <= 'Z' or not 1 <= linha <= 99:
            continue
        coord = cria_coordenada(coluna,linha)
        coords_vizinhas += (coord,)

    # Coordenada vizinha à esquerda da recebida.
    linha = obtem_linha(coordenada)
    coluna = chr(ord(obtem_coluna(coordenada))-1)
    if 'A' <= coluna <= 'Z' and 1 <= linha <= 99:    
        coord=cria_coordenada(coluna,linha)
        coords_vizinhas += (coord,)

    return coords_vizinhas


def obtem_coordenada_aleatoria(coordenada,gerador):
    """
    Devolve uma coordenada aleatória, em que a coordenada recebida define a maior coluna e linha possíveis.

    Argumentos:
        coordenada: coordenada
        gerador: gerador
    A coordenada aleatória é gerada com recurso às funções
    "gera_numero_aleatorio" e "gera_carater_aleatorio", em que, em
    sequência, primeiro é gerada a coluna e depois a linha.

    obtem_coordenada_aleatoria: coordenada x gerador --> coordenada
    """
    nova_coluna = gera_carater_aleatorio(gerador,obtem_coluna(coordenada))
    nova_linha = gera_numero_aleatorio(gerador,obtem_linha(coordenada))
    return cria_coordenada(nova_coluna,nova_linha)




# --TAD parcela-- #

# Parcela representada como dicionário.

# Construtores
def cria_parcela():
    """
    Devolve uma parcela tapada sem mina escondida.

    cria_parcela: {}--> parcela
    """
    return {'estado': 'tapada','mina':False}


def cria_copia_parcela(parcela):
    """
    Devolve uma nova cópia da parcela recebida.

    Argumentos:
        parcela: parcela 
    
    cria_copia_parcela: parcela --> parcela
    """
    return parcela.copy()


# Modificadores
def limpa_parcela(parcela):
    """
    Modifica o estado da parcela para "limpa" e devolve-a.
    
    Argumentos:
        parcela: parcela
    
    limpa_parcela: parcela --> parcela
    """
    parcela['estado'] = 'limpa'
    return parcela


def marca_parcela(parcela):
    """
    Modifica o estado da parcela para "marcada" e devolve-a.

    Argumentos:
        parcela: parcela

    marca_parcela: parcela --> parcela
    """
    parcela['estado'] = 'marcada'
    return parcela


def desmarca_parcela(parcela):
    """
    Modifica o estado da parcela para "tapada" e devolve-a.

    Argumentos:
        parcela: parcela

    desmarca_parcela: parcela --> parcela
    """
    parcela['estado'] = 'tapada'
    return parcela


def esconde_mina(parcela):
    """
    Modifica a parcela, escondendo uma mina e devolve-a

    Argumentos:
        parcela: parcela

    esconde_mina: parcela --> parcela
    """
    parcela['mina'] = True
    return parcela


# Reconhecedores
def eh_parcela(argumento):
    """
    Devolve True caso o argumento seja um TAD parcela e False caso contrário.

    Argumentos:
        argumento: universal

    eh_parcela: universal --> booleano
    """
    return (type(argumento) == dict and len(argumento) == 2 and
            'estado' in argumento and 'mina' in argumento and
            type(argumento['estado']) == str and type(argumento['mina']) == bool and
            (argumento['estado'] == 'limpa' or argumento['estado'] == 'tapada' or
            argumento['estado'] == 'marcada'))


def eh_parcela_tapada(parcela):
    """
    Devolve True ou False consoante a parcela recebida seja tapada ou não.

    Argumentos:
        parcela: parcela
        
    eh_parcela_tapada: parcela --> booleano
    """
    return parcela['estado'] == 'tapada'


def eh_parcela_marcada(parcela):
    """
    Devolve True ou False consoante a parcela recebida seja marcada ou não.
    
    Argumentos:
        parcela: parcela

    eh_parcela_marcada: parcela --> booleano
    """
    return parcela['estado'] == 'marcada'


def eh_parcela_limpa(parcela):
    """
    Devolve True ou False consoante a parcela recebida seja limpa ou não.
    
    Argumentos:
        parcela: parcela
            
    eh_parcela_marcada: parcela --> booleano
    """
    return parcela['estado'] == 'limpa'


def eh_parcela_minada(parcela):
    """
    Devolve True ou False consoante a parcela recebida contenha uma mina ou não.
    
    Argumentos:
        parcela: parcela
            
    eh_parcela_marcada: parcela --> booleano
    """
    return parcela['mina']


# Teste
def parcelas_iguais(parcela1,parcela2):
    """
    Devolve True ou False consoante os argumentos sejam parcelas e sejam iguais.

    Argumentos:
        parcela1: parcela
        parcela2: parcela

    parcelas_iguais: parcela1 x parcela2 --> boolenao
    """    
    return (eh_parcela(parcela1) and eh_parcela(parcela2) and
            parcela1['estado'] == parcela2['estado'] and
            parcela1['mina'] == parcela2['mina'])


# Transformador
def parcela_para_str(parcela):
    """
    Devolve uma cadeia de carateres que representa a parcela recebida.

    Argumentos:
        parcela: parcela
    Se a parcela recebida for tapada devolve: "#", se for marcada: "@", se
    for limpa sem mina: "?" e se for limpa com mina: "X".

    parcela_para_str: parcela --> str
    """
    if eh_parcela_limpa(parcela) and eh_parcela_minada(parcela):
        return 'X'
    elif eh_parcela_marcada(parcela):
        return '@'
    elif eh_parcela_tapada(parcela):
        return '#'
    else:
        return '?'


# Função de alto nível-TAD Parcela.
def alterna_bandeira(parcela):
    """
    Marca ou desmarca a parcela recebida.

    Argumentos:
        parcela: parcela
    No fundo, desmarca-a se esta estiver marcada e marca-a se estiver tapada,
    devolvendo True. Em qualquer outro caso, não a modifica e devolve False.

    alterna_bandeira: parcela --> booleano
    """
    if eh_parcela_marcada(parcela):
        desmarca_parcela(parcela)
        return True
    if eh_parcela_tapada(parcela):
        marca_parcela(parcela)
        return True
    return False




# --TAD campo-- #

# Campo representado como dicionário.

# Construtores
def cria_campo(ultima_coluna,ultima_linha):
    """
    Devolve um campo do tamanho pretendido, em que todas as suas parcelas são tapadas e sem minas.

    Argumentos:
        ultima_coluna: cadeia de carateres (entre A e Z)
        ultima_linha: inteiro (entre 1 e 99)
    Para além disso, verificam-se a validade dos argumentos, gerando um
    ValueError com a mensagem "cria_campo: argumentos invalidos", em caso
    de argumentos incorretos.

    cria_campo: str x int --> campo
    """
    if (type(ultima_coluna) != str or len(ultima_coluna) != 1 or
        type(ultima_linha) != int or not ( 1<= ultima_linha <= 99 and
        'A' <= ultima_coluna <= 'Z')):
            raise ValueError ('cria_campo: argumentos invalidos')

    coordenadas = {}
    for c in range(ord('A'),ord(ultima_coluna) + 1):
        for l in range (1,ultima_linha + 1): # A cada coordenada é associada uma parcela
            coordenadas[cria_coordenada(chr(c),l)] = cria_parcela()
    return {'ultima_coluna':ultima_coluna,'ultima_linha':ultima_linha,
            'coordenadas':coordenadas}


def cria_copia_campo(campo):
    """
    Devolve uma nova copia do campo recebido.

    Argumentos:
        campo: campo
    
    cria_copia_campo: campo --> campo
    """
    copia_campo={}
    for key,value in campo.items():
        if key == 'coordenadas':
          copia_campo[key] = {}
          for coord,parcela in value.items():
            copia_campo[key][coord] = cria_copia_parcela(parcela)
        else:
            copia_campo[key] = value
    return copia_campo


# Seletores
def obtem_ultima_coluna(campo):
    """
    Devolve a última coluna do campo recebido.

    Argumentos:
        campo: campo

    obtem_ultima_coluna: campo --> str
    """
    return campo['ultima_coluna']


def obtem_ultima_linha(campo):
    """
    Devolve a última linha do campo recebido.
    
    Argumentos:
        campo: campo

    obtem_ultima_linha: campo --> int
    """
    return campo['ultima_linha']


def obtem_parcela(campo,coordenada):
    """
    Devolve a parcela correspondente à coordenada correspondida.

    Argumentos:
        campo: campo
        coordenada: coordenada

    obtem_parcela: campo x coordenada --> parcela
    """
    return campo['coordenadas'][coordenada]


def obtem_coordenadas(campo,estado):
    """
    Devolve um tuplo com as coordenadas que contêm parcelas com o estado recebido.

    Argumentos:
        campo: campo
        estado: cadeia de carateres

    obtem_coordenadas: campo x estado --> tuplo
    """
    coords_com_igual_estado = ()
    for l in range(1,obtem_ultima_linha(campo) + 1):
        for c in range(ord('A'),ord(obtem_ultima_coluna(campo)) + 1):
            coord = cria_coordenada(chr(c),l)
            parcela=obtem_parcela(campo,coord)
            if ((estado == 'tapadas' and eh_parcela_tapada(parcela)) or
                (estado == 'limpas' and eh_parcela_limpa(parcela)) or
                (estado == 'marcadas' and eh_parcela_marcada(parcela)) or
                (estado == 'minadas' and eh_parcela_minada(parcela))):
                    coords_com_igual_estado = coords_com_igual_estado + (coord,)
    return coords_com_igual_estado


def obtem_numero_minas_vizinhas(campo,coordenada):
    """
    Devolve o número de minas vizinhas à coordenada recebida.

    Argumentos:
        campo: campo
        coordenada: coordenada
    No fundo, devolve o número de parcelas vizinhas da parcela na coordenada
    recebida que escondem uma mina.

    obtem_numero_minas_vizinhas: campo x coordenada --> int
    """
    coord_vizinhas = obtem_coordenadas_vizinhas(coordenada)
    minas_vizinhas = 0
    for coord in coord_vizinhas:
        if (eh_coordenada_do_campo(campo,coord) and
            eh_parcela_minada(obtem_parcela(campo,coord))):
                minas_vizinhas += 1
    return minas_vizinhas


# Reconhecedores
def eh_campo(argumento):
    """
    Devolve True ou False consoante o argumento seja um TAD campo ou não.

    Argumentos:
        argumento: universal
    
    eh_campo: universal --> booleano
    """
    if (type(argumento) != dict or len(argumento) != 3 or
        'ultima_coluna' not in argumento or 'ultima_linha' not in argumento or
        'coordenadas' not in argumento or type(argumento['coordenadas']) != dict):
            return False
    for coord,parcela in argumento['coordenadas'].items():
        if not eh_coordenada(coord) or not eh_parcela(parcela):
            return False
    return (type(obtem_ultima_coluna(argumento)) == str and
            type(obtem_ultima_linha(argumento)) == int)


def eh_coordenada_do_campo(campo,coordenada):
    """
    Devolve True ou False consoante a coordenada recebida faça parte do campo recebido ou não.

    Argumentos:
        campo: campo
        coordenada: coordenada

    eh_coordenada_do_campo: campo x coordenada --> booleano
    """
    return (eh_coordenada(coordenada) and
            obtem_coluna(coordenada) <= obtem_ultima_coluna(campo) and
            obtem_linha(coordenada) <= obtem_ultima_linha(campo))


# Teste
def campos_iguais(campo1,campo2):
    """
    Devolve True ou False consoante os argumentos recebidos sejam campos e sejam iguais ou não.
    
    Argumentos:
        campo1: campo
        campo2: campo
    
    campos_iguais: campo1 x campo2 --> booleano
    """    
    if (not eh_campo(campo1) or not eh_campo(campo2) or
        not obtem_ultima_coluna(campo1) == obtem_ultima_coluna(campo2) or
        not obtem_ultima_linha(campo1) == obtem_ultima_linha(campo2)):
            return False
    return (obtem_coordenadas(campo1,'limpas') == obtem_coordenadas(campo2,'limpas') and
            obtem_coordenadas(campo1,'tapadas') == obtem_coordenadas(campo2,'tapadas') and
            obtem_coordenadas(campo1,'marcadas') == obtem_coordenadas(campo2,'marcadas') and
            obtem_coordenadas(campo1,'minadas') == obtem_coordenadas(campo2,'minadas'))


# Transformador
def campo_para_str(campo):
    """
    Devolve uma cadeia de carateres que representa o campo recebido.

    Argumentos:
        campo: campo
    
    campo_para_str: campo --> str
    """
    colunas = 'A'
    while colunas[-1] < obtem_ultima_coluna(campo):
        colunas += chr(ord(colunas[-1])+1)
    campo_str = '   {}\n  +{}+\n'.format(colunas,'-' * len(colunas))

    for l in range(1,obtem_ultima_linha(campo) + 1):
        parcela = ''
        for c in range(ord('A'),ord(obtem_ultima_coluna(campo)) + 1):        
            coords = cria_coordenada(chr(c),l)
            parcelas = obtem_parcela(campo,coords)
            if eh_parcela_limpa(parcelas) and not eh_parcela_minada(parcelas):
                # As parcelas limpas são transformadas em espaços brancos caso não tenham
                # minas vizinhas, caso contrário apresentam o número destas.
                if obtem_numero_minas_vizinhas(campo,coords) == 0:
                    parcela += ' '
                else:
                    parcela += str(obtem_numero_minas_vizinhas(campo,coords))
            else:
                # Parcelas marcadas, tapadas ou limpas e minadas são transformadas com
                # recurso à função "parcela_para_str".
                parcela += parcela_para_str(parcelas)
        # Após cada a conversão de cada linha, as parcelas são adicionadas ao
        # "campo_para_str".
        campo_str += '{:02d}|{}|\n'.format(l,parcela)
    return campo_str + '  +{}+'.format('-'*len(colunas))


# Funções de Alto nível-TAD Campo
def coloca_minas(campo,coordenada,gerador,n_minas):
    """
    Devolve um campo colocando neste "n_minas" minas.

    Argumentos:
        campo: campo
        coordenada: coordenada
        gerador: gerador
        n_minas: inteiro
    No fundo, são geradas coordenadas aleatórias, e se não coincidirem com a
    coordenada recebida, nem com as suas viznhas, nem já forem coordenadas
    com minas, são escondidas minas nestas.

    coloca_minas: campo x coordenada x gerador x int --> campo
    """
    contador = 0
    maior_coord = cria_coordenada(obtem_ultima_coluna(campo),obtem_ultima_linha(campo))
    while contador < n_minas:
        nova_coord = obtem_coordenada_aleatoria(maior_coord,gerador)
        parcela = obtem_parcela(campo,nova_coord)
        if (not eh_parcela_minada(parcela) and nova_coord != coordenada and
            nova_coord not in obtem_coordenadas_vizinhas(coordenada)):
                esconde_mina(parcela)
                contador += 1
    return campo


def limpa_campo(campo,coordenada):
    """
    Devolve um campo limpo.

    Argumentos:
        campo: campo
        coordenada: coordenada
    No fundo, limpa a parcela da coordenada recebida e se não houver minas
    vizinhas, limpa iterativamente todas as parcelas vizinhas tapadas. Se
    a parcela já estiver limpa, a operação não tem efeito.

    limpa_campo: campo x coordenada --> campo
    """
    if (not eh_parcela_limpa(obtem_parcela(campo,coordenada)) and
        eh_coordenada_do_campo(campo,coordenada)):
        # Se a parcela da coordenada recebida já estiver limpa, a operação não tem
        # efeito.
        limpa_parcela(obtem_parcela(campo,coordenada))
        if (obtem_numero_minas_vizinhas(campo,coordenada) == 0 and
            not eh_parcela_minada(obtem_parcela(campo,coordenada))):
            coords_vizinhas = obtem_coordenadas_vizinhas(coordenada)
            for c in coords_vizinhas:
                # Se nem a coordenada recebida nem as vizinhas esconderem minas, todas as
                # parcelas vizinhas tapadas são limpas iterativamente, chamando novamente
                # a função "limpa_campo".
                if (eh_coordenada_do_campo(campo,c) and
                    not eh_parcela_marcada(obtem_parcela(campo,c))):
                        limpa_campo(campo,c)
    return campo




# --Funções adicionais-- #
def jogo_ganho(campo):
    """
    Devolve True ou False consoante o jogo esteja ganho ou não.

    Argumentos:
        campo: campo
    O jogo é considerado ganho se todas as parcelas sem minas estiverem
    limpas.

    jogo_ganho: campo--> booleano
    """
    coords_minadas=obtem_coordenadas(campo,'minadas')
    coords_marcadas=obtem_coordenadas(campo,'marcadas')
    coords_tapadas=obtem_coordenadas(campo,'tapadas')
    return tuple(sorted(coords_minadas)) == tuple(sorted(coords_marcadas+coords_tapadas))


def turno_jogador(campo):
    """
    Recebe um campo e pede ao jogador uma ação e uma coordenada.

    Argumentos:
        campo: campo
    A ação do jogador pode ser limpar ou (des)marcar a coordenada que
    selecionar. Caso o jogador limpe uma parcela que esconda uma mina
    devolve False, e True caso contrário.

    turno_jogador: campo --> booleano
    """
    while True:
        acao = input('Escolha uma ação, [L]impar ou [M]arcar:')
        if acao in ('M','L'):
            break # Apenas aceita a ação recebida se esta for válida.
    
    while True:
        coord = input('Escolha uma coordenada:')
        if (len(coord) == 3 and 'A' <= coord[0] <= obtem_ultima_coluna(campo) and
           ((coord[1] == '0' and '1' <= coord[2] <= '9' and
           1 <= int(coord[2]) <= obtem_ultima_linha(campo)) or ('1' <= coord[1] <= '9' and
           '0' <= coord[2] <= '9' and 1 <= int(coord[1:]) <= obtem_ultima_linha(campo)))):
                break # Apenas aceita a coordenada recebida se esta for válida.
    
    if acao == 'L':
        if eh_parcela_minada(obtem_parcela(campo,str_para_coordenada(coord))):
            limpa_campo(campo,str_para_coordenada(coord))
            return False
        limpa_campo(campo,str_para_coordenada(coord))
    else:
        alterna_bandeira(obtem_parcela(campo,str_para_coordenada(coord)))
    return True


def minas(coluna,linha,n_minas,bits,estado):
    """
    Devolve True ou False consoante o jogador ganhe o jogo ou não.

    Argumentos:
        coluna: cad. carateres
        linha: inteiro
        n_minas: inteiro
        bits: inteiro
        estado: inteiro
    No fundo, esta permite jogar o jogo das minas, recorrendo às diversas
    funções e TADs definidos anteriormente. Para além disso, os argumentos
    recebidos são verificados.

    minas: str x int x int x int x int --> booleano
    """    
    if (type(coluna) != str or len(coluna) != 1 or type(linha) != int or
        type(n_minas) != int or type(bits) != int or type(estado) != int or
        estado <= 0 or not 'A' <= coluna <= 'Z' or not 1 <= linha <= 99 or
        n_minas <= 0 or bits not in (32,64) or (bits == 32 and estado > 0xFFFFFFFF) or
        (bits == 64 and estado > 0xFFFFFFFFFFFFFFFF) or
        (ord(coluna) - ord('A') + 1) * linha - 9 < n_minas):
            raise ValueError ('minas: argumentos invalidos')
            # A primeira jogada tem de ser segura independentemente da escolha da
            # coordenada inicial, logo é necessário que o campo tenha um tamanho mínimo
            # para colocar todas as minas. Tal é verificado pela condição: área do
            # campo - 9 < número de minas a colocar.
    
    gerador = cria_gerador(bits,estado)
    campo = cria_campo(coluna,linha)
    print(('   [Bandeiras {}/{}]\n' + campo_para_str(campo)).format(0,n_minas))

    while True:
        coord = input('Escolha uma coordenada:')
        # A primeira "ação" é obrigatória ser limpar, pelo que apenas é pedido a
        # coordenada ao jogador.
        if (len(coord) == 3 and 'A' <= coord[0] <= obtem_ultima_coluna(campo) and
            ((coord[1] == '0' and '1' <= coord[2] <= '9' and
            1 <= int(coord[2]) <= obtem_ultima_linha(campo)) or ('1' <= coord[1] <= '9' and
            '0' <= coord[2] <= '9' and 1 <= int(coord[1:]) <= obtem_ultima_linha(campo)))):
                break # Apenas aceita a coordenada recebida se for válida.
    
    coloca_minas(campo,str_para_coordenada(coord),gerador,n_minas)
    limpa_campo(campo,str_para_coordenada(coord))

    while not jogo_ganho(campo):
        n_bandeiras = len(obtem_coordenadas(campo,'marcadas'))
        print(('   [Bandeiras {}/{}]\n' + campo_para_str(campo)).format(n_bandeiras,n_minas))
        if not turno_jogador(campo):
            n_bandeiras = len(obtem_coordenadas(campo,'marcadas'))
            # O número de bandeiras é calculado denovo, pois o jogador pode ter
            # perdido ao tentar limpar uma parcela marcada que continha uma mina.
            print(('   [Bandeiras {}/{}]\n' + campo_para_str(campo)).format(n_bandeiras,n_minas))
            print('BOOOOOOOM!!!')
            return False
    print(('   [Bandeiras {}/{}]\n' + campo_para_str(campo)).format(n_bandeiras,n_minas))    
    print('VITORIA!!!')
    return True