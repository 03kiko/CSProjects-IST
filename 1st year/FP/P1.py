#---P1-22/23---

# 1 --Justificação de textos--
def limpa_texto(texto):
    """
    
    devolve um texto limpo.
    
    Argumentos:
        texto: cadeia de carateres
    No fundo, remove todos os carateres brancos e elimina repetições
    de espaços seguidos e espaços iniciais e finais.
    
    limpa_texto: cad. carateres --> cad. carateres
    """

    texto=texto.split()
    return ' '.join(texto)


def corta_texto(texto, largura):
    """
    Devolve um tuplo constituído por 2 cadeias de carateres, que correspondem ao texto cortado em 2.

    Argumentos:
        texto:cadeia de carateres
        largura:inteiro (positivo)
    No fundo, o texto é cortado em 2 partes, consoante a largura
    recebida em que a primeira parte tem um comprimento máximo
    igual à largura, e a outra contém o resto do texto.

    corta_texto: cad. carateres x inteiro --> tuplo
    """
    
    if len(texto)==largura or ' ' not in texto:
        return (texto,'') 
        #Texto tem tamanho igual à largura ou é vazio ou contém apenas uma palavra
    
    elif texto[largura-1]==' ':
        return (texto[:largura-1],texto[largura:])
        #O carater onde o texto tem tamanho igual à largura é um espaço

    elif texto[largura]==' ': 
        return (texto[:largura],texto[largura+1:])
        #O carater onde o texto tem tamanho igual à largura constitui o fim de
        #uma palavra.
    
    elif texto[largura]!=' ':
        texto_cortado=texto[:largura]
        #O carater onde o texto tem tamanho igual à largura é uma letra e não
        #constitui o fim de uma palvra.    

        for i in range(largura-1,0,-1): #Procura-se o espaço anterior mais próximo
            if texto_cortado[i]==' ':
                i_espaco_mais_proximo=i
                break
        
        return (texto[:i_espaco_mais_proximo],texto[i_espaco_mais_proximo+1:])
        #O texto é cortado no espaço anterior mais próximo de modo a não cortar
        #uma palavra.

def insere_espacos(texto, largura):
    """
    Devolve um novo texto com comprimento igual à largura, inserindo espaços.

    Argumentos:
        texto:cadeia de carateres
        largura: inteiro (positivo)
    No fundo, insere-se espaços entre as palavras do texto de modo
    uniforme da esquerda para a direita até ter o tamanho pretendido.
    Se o texto for apenas uma palavra, então os espaços são
    adicionados no fim.

    insere_espacos: cad. carateres x inteiro --> cad. carateres
    """      

    if ' ' in texto: #O texto tem mais de uma palavra
        lista_espacos_a_adicionar=[0]*(len(texto)-len(texto.replace(' ','')))
        #É criada uma lista de tamanho igual ao número de espaços iniciais cujos
        #elementos são zero.
        soma=0
        
        while soma!=(largura-len(texto)): 
            
            for i in range(len(lista_espacos_a_adicionar)):
                soma=0
                lista_espacos_a_adicionar[i]+=1
                
                for elementos in lista_espacos_a_adicionar:
                    soma+=elementos
                
                if soma==largura-len(texto):
                    break
        #A cada elemento da lista adiciona-se +1 até que a soma dos elementos da
        #lista seja igual à diferença entre a largura e o comprimento do texto,
        #verificando sempre esta condição após cada adição de "+1".

        texto=texto.split()
        texto_final=''
        iterador_lista=iter(lista_espacos_a_adicionar)
        
        for i in range(len(texto)):
            
            if texto[i]!=texto[-1]:
                texto_final+=texto[i]+' '*(next(iterador_lista)+1)
            
            else:
                texto_final+=texto[i]
        #O texto final é formado adicionando-se ora uma palavra ora o número de
        #espaços necessários (elemento da lista +1), até se chegar à palavra
        #final onde se apenas adiciona esta.
    
    else: #O texto tem apenas uma palavra.
        texto_final=texto.ljust(largura)
    return texto_final


def justifica_texto(texto, largura):
    """
    Devolve um tuplo de cadeias de carateres justificadas.

    Argumentos:
        texto: cadeia de carateres (não vazia, com palavras de tamanho menor que
                                    a largura)
        largura: inteiro (positivo)
    Aplicam-se funções anteriores (limpa_texto, corta_texto, insere_espacos)
    de modo justificá-lo. É também verificado os argumentos recebidos,
    gerando um ValueError com a mensagem:"justifica_texto: argumentos
    inválidos" em caso de argumento(s) errados.
    
    justifica_texto: cad. carateres x inteiro --> tuplo
    """ 
    
    if not isinstance(texto,str) or texto=='' or not isinstance(largura,int) or largura<0:
        raise ValueError ('justifica_texto: argumentos invalidos')
        #Verifica se o texto de entrada é uma cadeia de carateres, não vazio e
        #se a largura é um inteiro positivo.
    
    texto_verifica_palavras=texto.split()
    for palavras in texto_verifica_palavras:
        if len(palavras)>largura:
                raise ValueError ('justifica_texto: argumentos invalidos')
                #Verifica se todas as palavras do texto têm um comprimento menor do que a
                #largura recebida
    
    texto=limpa_texto(texto)
        
    if len(texto)>largura:
        texto=corta_texto(texto,largura)
        resto_do_texto=texto[-1] 
        i=0
        while len(resto_do_texto)>largura:
            texto=texto[:i+1]+corta_texto(resto_do_texto,largura)
            resto_do_texto=texto[-1]
            i+=1
        #O texto é todo cortado, pelo que cada parte do texto tem um comprimento
        #máximo igual à largura.
        texto_final=()
            
        for i in range(len(texto)):
                
            if texto[i]!=texto[-1]:
                texto_espacos=insere_espacos(texto[i],largura)
                texto_final+=(texto_espacos,)
            #São inseridos espaços de modo a que todos os elementos tenham um
            #comprimento igual à largura.
            else:
                texto_final+=(texto[i].ljust(largura),)
            #É a última cadeia do tuplo, logo os espaços são inseridos no fim.
    else:
        texto_final=(texto.ljust(largura),)
        #O tamanho do texto é inferior à largura (o tuplo tem um único elemento),
        #logo os espaços são adicionados no fim
    return texto_final





# 2 --Método de Hondt--
def calcula_quocientes(votos_apurados, numero_de_deputados):
    """
    Devolve um dicionário com quocientes por ordem crescente calculados através do método de Hondt para cada partido.
    
    Argumentos:
        votos_apurados: dicionário (chaves correspondem a partidos, tem pelo
                                    menos um partido)
        numero_de_deputados: inteiro (positivo)
    No fundo, é feita uma divisão sucessiva (1,2,...,número de deputados)
    dos votos iniciais de cada partido, devolvendo-se um dicionário com as
    mesmas chaves do argumento, em que cada valor é a lista com
    essas divisões.

    calcula_quocientes: dicionário x inteiro --> dicionário
    """

    quociente={}
    div_sucessiva=[]
    
    for partido,votos in votos_apurados.items():
        divisor=1
        
        while divisor<=numero_de_deputados: 
            div_sucessiva+=[votos/divisor]
            divisor+=1
        
        quociente[partido]=div_sucessiva
        div_sucessiva=[]
    
    return quociente


def atribui_mandatos(votos_apurados, numero_de_deputados):
    """
    Devolve uma lista ordenada em que cada elemento corresponde a um deputado eleito, contendo o nome partido que o obteve.

    Argumentos:
        votos_apurados: dicionário (chaves correspondem a partidos, tem pelo
                                    menos um partido)
        numero_de_deputados: inteiro (positivo)
    Os quocientes são calculados com recurso à função anterior e
    são comparados, um mandato é atribuído em função da ordem crescente
    destes quocientes, quando se dão divisões iguais os mandatos são
    atribuídos por ordem crescente aos partidos menos votados.
    atribui_mandatos: dicionário x inteiro --> lista
    """
    
    quociente=calcula_quocientes(votos_apurados,numero_de_deputados)
    mandatos=[]
    divisoes=[]
    
    for lista_divisoes in quociente.values():
        for div2 in lista_divisoes:
            divisoes.append(div2)
    divisoes=sorted(divisoes,reverse=True)
    #junta-se numa só lista e ordena-se todas as divisões sucessivas do maior
    #para o menor
    d_anterior=0
    for d in divisoes:
        numero_de_correncias=divisoes.count(d)
        if numero_de_correncias>=2: #quando existe quocientes iguais
            
            if d==d_anterior:
                continue 

            d_anterior=d
            quocientes_iguais=[]
            for partido,lista_por_partido in quociente.items():
                
                if d in lista_por_partido:
                    quocientes_iguais.append(partido)
                #armazena na lista os partidos cujos quocientes são iguais

            quocientes_iguais_par_partido_votos=[]
            for partido,votos in votos_apurados.items():
                
                if partido in quocientes_iguais:
                    quocientes_iguais_par_partido_votos.append((partido,votos))
            quocientes_iguais_par_partido_votos=sorted(quocientes_iguais_par_partido_votos,key=lambda x:x[1]) 
            #lista ordenada por número de votos com pares partido/votos dos partidos
            #com quocientes iguais

            for i in quocientes_iguais_par_partido_votos:
                mandatos.append(i[0])  
                if len(mandatos)==numero_de_deputados:
                    break
        else:
            
            for partido,lista_por_partido in quociente.items():
                  
                if d in lista_por_partido:
                    mandatos.append(partido)
        
        if len(mandatos)==numero_de_deputados:
            break
    
    
    return mandatos


def obtem_partidos(eleicoes_num_territorio):
    """
    Devolve uma lista com os partidos presentes no território.

    Argumentos:
        eleicoes_num_territorio: dicionário
    No fundo, a função procura partidos em todos os círculos eleitorais do
    território e devolve numa lista com os partidos que participaram nas
    eleições por ordem alfabética. 

    obtem_partidos: dicionário --> lista
    """
  
    partidos=[]
    
    for circulo in eleicoes_num_territorio.keys():
        votos_num_circulo=eleicoes_num_territorio[circulo]['votos']
        
        for partido in votos_num_circulo.keys():
            
            if partido not in partidos:
                partidos.append(partido)
    
    return sorted(partidos)


def obtem_resultado_eleicoes(eleicoes_num_territorio):
    """
    Devolve uma lista com o resultado das eleições num território.
    
    Argumentos:
        eleicoes_num_territorio: dicionário
    No fundo, aplica-se o Método de Hondt recorrendo às funções
    atribui_mandatos e obtem_partidos de modo obter os mandatos de cada
    círculo eleitoral e chegar ao resultado das eleições. Para validar os
    argumentos recebidos recorre-se à função auxiliar "validar_arg" gerando
    ValueError com a mensagem: "obtem_resultados_eleicoes: argumentos
    inválidos" em caso de argumentos errados.

    obtem_resultado_eleicoes: dicionário --> lista
    """
    
    def validar_arg(eleicoes_num_territorio):
        if eleicoes_num_territorio=={} or not isinstance(eleicoes_num_territorio,dict):
            raise ValueError ('obtem_resultado_eleicoes: argumento invalido')
            #Verifica se o argumento é nao vazio e é um dicionário

        for circulo in eleicoes_num_territorio:
            
            if (not isinstance (circulo,str) or eleicoes_num_territorio[circulo]=='' or
                    not isinstance(eleicoes_num_territorio[circulo],dict) or
                    len(eleicoes_num_territorio[circulo])!=2 or
                    'votos' not in eleicoes_num_territorio[circulo] or
                    'deputados' not in eleicoes_num_territorio[circulo] or
                    not isinstance(eleicoes_num_territorio[circulo]['deputados'],int) or
                    not isinstance(eleicoes_num_territorio[circulo]['votos'],dict) or
                    eleicoes_num_territorio[circulo]['deputados']<1 or
                    len(eleicoes_num_territorio[circulo]['votos'])<1):

                raise ValueError ('obtem_resultado_eleicoes: argumento invalido')
                #Verifica se cada nome de um círculo eleitoral é uma cadeia de carateres
                #não vazia, se em cada círculo existe um dicionário com um comprimento
                #igual a 2, com as chaves "votos" e "deputados" e que estas são
                #respetivamente um dicionário com pelo menos um partido e um inteiro
                #maior ou igual a 1.

            for partido in eleicoes_num_territorio[circulo]['votos']:
                    
                    if (not isinstance(partido,str) or partido=='' or
                        not isinstance(eleicoes_num_territorio[circulo]['votos'][partido],int)):
                    
                        raise ValueError ('obtem_resultado_eleicoes: argumento invalido')
                        #Verifica se o nome dos partidos em cada círculo eleitoral são cadeias de
                        #carateres não vazias, e se os votos obtidos por cada partido são inteiros.

                    if eleicoes_num_territorio[circulo]['votos'][partido]<=0:
                        raise ValueError ('obtem_resultado_eleicoes: argumento invalido')
                        #Verifica se todos os partidos em cada círculo eleitoral receberam votos

    validar_arg(eleicoes_num_territorio)
    
    partidos=obtem_partidos(eleicoes_num_territorio)
    
    eleicoes_resultado=[]
    for partido in partidos:
        eleicoes_por_circulo=[]    
        for circulo in eleicoes_num_territorio:
            mandatos_por_circulo=atribui_mandatos(eleicoes_num_territorio[circulo]['votos'],
                                                  eleicoes_num_territorio[circulo]['deputados'])
            #São atribuídos os mandatos em cada círculo
            
            if partido in eleicoes_num_territorio[circulo]['votos'].keys():
                eleicoes_por_circulo.append([partido,
                                             mandatos_por_circulo.count(partido),
                                             eleicoes_num_territorio[circulo]['votos'][partido]])
        #É armazenada na lista os votos obtidos, nome do partido e número de
        #deputados obtidos, em que cada elemento da lista corresponde ao resultado
        #desse partido em cada círculo eleitoral.

        soma_mandatos=0
        soma_votos=0
        for i in range(len(eleicoes_por_circulo)):
            soma_mandatos+=eleicoes_por_circulo[i][1]
            soma_votos+=eleicoes_por_circulo[i][2]
        eleicoes_resultado.append((partido,soma_mandatos,soma_votos))
        #Para cada partido são somados os votos e mandatos que obteve nos
        #diversos círculos.
    
    eleicoes_resultado=sorted(eleicoes_resultado,key=lambda x: (x[1],x[2]),reverse=True)
    #O resultado final das eleições é ordenado de acordo com o número
    #de mandatos, e em caso de empate, conforme o número de votos obtidos.
    return eleicoes_resultado





# 3 --Solução de sistemas de equações--
def produto_interno(tuplo1, tuplo2):
    """
    Devolve o produto interno de dois vetores.

    Argumentos:
        tuplo1: tuplo (constituído por números inteiros ou reais)
        tuplo2: tuplo (constituído por números inteiros ou reais)
    No fundo, é feito o produto interno entre dois tuplos, que representam
    vetores, com igual dimensão. O resultado do produto interno é
    apresentado como um número real.

    produto_interno: tuplo x tuplo --> real
    """
    
    produto=0.0
    
    for i in range(len(tuplo1)):
        produto+=tuplo1[i]*tuplo2[i]
    
    return produto


def verifica_convergencia(matriz, vetor_c, sol_atual_x,precisao):
    """
    Devolve True ou False consoante se verifique que o valor absoluto do erro de todas as equações é inferior à precisão recebida.
    
    Argumentos:
        matriz: tuplo (é quadrada, os elementos são tuplos e representam as
                       linhas da matriz e cada linha contém os seus coeficientes)
        vetor_c:tuplo (tamanho igual à matriz)
        sol_atual_x: tuplo (tamanho igual à matriz)
        precisão: real
    No fundo, calcula-se o erro fazendo o valor absoluto da diferença 
    entre o produto interno dos coeficientes de cada linha e a solução
    atual de x e a constante respetiva dessa linha. Se o erro for inferior
    à precisão em todas as linhas é devolvido True, senão é devolvido False. 
    
    verifica_convergencia: tuplo x tuplo x tuplo x real --> booleano
    """
    
    contador=0
    
    for i_linha in range(len(matriz)):
        fi_x=produto_interno(matriz[i_linha],sol_atual_x)
    
        if abs(fi_x-vetor_c[i_linha])<precisao:
            contador+=1    
    
    return contador==len(matriz)


def retira_zeros_diagonal(matriz,constantes):
    """
    Devolve um tuplo cujos elementos são a matriz e o vetor das constantes mas com as linhas/números trocadas(os) retirando-se eventuais 0s das diagonais da matriz. 
    
    Argumentos:
        matriz: tuplo (é quadrada, os elementos são tuplos e representam as
                       linhas da matriz e cada linha contém os seus coeficientes)
        vetor_c:tuplo (tamanho igual à matriz)
    No fundo, esta devolve uma nova matriz com as mesmas linhas da recebida
    no argumento, mas com as linhas trocadas de modo a que não existam 0s nas
    diagonais. O mesmo acontece com o vetor das constantes que acompanha
    a reordenação.
    
    retira_zeros_diagonal: tuplo x tuplo --> tuplo x tuplo
    """

    matriz_lista=list(matriz)
    constantes_lista=list(constantes)
    
    for i_linha in range(len(matriz_lista)):
        
        if matriz_lista[i_linha][i_linha]==0:
            
            for j_linha in range(len(matriz_lista)):
                
                if matriz_lista[j_linha][i_linha]!=0 and matriz_lista[i_linha][j_linha]!=0:
                    matriz_lista[i_linha],matriz_lista[j_linha]=matriz_lista[j_linha],matriz_lista[i_linha]
                    constantes_lista[i_linha]=constantes_lista[j_linha]
                    constantes_lista[j_linha]=constantes[i_linha]
                    break
        
        #A troca de linhas é feita da seguinte forma: ao encontrar
        #um 0 na coluna "i" correspondente à diagonal de uma linha "i", esta é
        #trocada pela primeira linha "j" que encontrar, procurando desde o início
        #da matriz, que não contenha um 0 na coluna "i", sempre que na linha "i" 
        #não exista um 0 na coluna "j".

    return (tuple(matriz_lista),tuple(constantes_lista))



def eh_diagonal_dominante(matriz):
    """
    Devolve True caso esta seja diagonal dominante, caso contrário devolve False.

    Argumentos:
        matriz: tuplo (é quadrada, os elementos são tuplos e representam as
                       linhas da matriz e cada linha contém os seus coeficientes)
    No fundo, verifica-se se para todas as linhas se o valor
    absoluto do coeficiente da diagonal da linha é maior ou igual que a
    soma dos restantes coeficientes da linha.Se isso se verificar então,
    a matriz é diagonal dominante. 
    
    eh_diagonal_dominante: tuplo --> booleano
    """
    
    contador=0
    for linha in range(len(matriz)):

        soma=0
        
        for items_excepto_diagonal in range(len(matriz[linha])):
            
            if matriz[linha][linha]!=matriz[linha][items_excepto_diagonal]:
                soma+=abs(matriz[linha][items_excepto_diagonal])
        
        if abs(matriz[linha][linha])>=soma:
            contador+=1
    
    return len(matriz)==contador


def resolve_sistema(matriz,constantes,precisao):
    """
    Devolve um tuplo que é a solução do sistema aplicando o método de Jacobi.

    Argumentos:
        matriz: tuplo (é quadrada, os elementos são tuplos e representam as
                       linhas da matriz e cada linha contém os seus coeficientes)
        vetor_c:tuplo (tamanho igual à matriz)
        precisao: real (positivo)
    No fundo, esta função recorre às funções "eh_diagonal_dominante",
    "retira_zero_diagonal","verifica_convergencia" e "produto_interno e 
    aplica o método de Jacobi para resolver o sistema. A função auxiliar
    "validar_argumentos", gera ValueError com a mensagem: "resolve_sistema:
    argumentos inválidos" em caso de existirem argumentos errados. Também se
    a matriz não for diagonal dominante gera-se ValueError com a mensagem:
    "resolve_sistema: matriz nao diagonal dominante".

    resolve_sistema: tuplo x tuplo x real --> tuplo
    """

    def validar_argumentos(matriz,constantes,precisao):
        if (matriz==() or 
            constantes==() or
            not isinstance(matriz,tuple) or
            not isinstance(constantes,tuple)):

            raise ValueError ('resolve_sistema: argumentos invalidos') 
            #Verifica se a matriz é um tuplo não vazio e se as constantes são tuplos.

        for linha in matriz: 
            
            if not isinstance(linha,tuple): 
                raise ValueError ('resolve_sistema: argumentos invalidos')
                #Verifica se cada linha da matriz é um tuplo.

            if len(matriz)!=len(linha): 
                raise ValueError ('resolve_sistema: argumentos invalidos')             
                #Verifica se a matriz é quadrada.
            
            for coeficiente in linha:
        
                if not isinstance(coeficiente,(int,float)): 
                    raise ValueError ('resolve_sistema: argumentos invalidos')
                    #Verifica se os coeficientes da matriz são reais ou inteiros.

        for constante in constantes:
            
            if not isinstance(constante,(int,float)):
                raise ValueError ('resolve_sistema: argumentos invalidos')                
                #Verifica se as constantes são reais ou inteiras.
        
        if len(matriz)!=len(constantes):
            raise ValueError ('resolve_sistema: argumentos invalidos')         
            #Verifica se existe o mesmo número de linhas na matriz que constantes.
        
        if not isinstance(precisao,float) or precisao<=0:
            raise ValueError ('resolve_sistema: argumentos invalidos')
            #Verifica se a precisão é um real positivo.

    validar_argumentos(matriz,constantes,precisao)
    
    zeros_retirados=retira_zeros_diagonal(matriz,constantes)
    matriz,constantes=zeros_retirados[0],zeros_retirados[1]

    
    if not eh_diagonal_dominante(matriz):
        raise ValueError ('resolve_sistema: matriz nao diagonal dominante')

    x=(0,)*len(matriz) #solução inicial igual a 0
    x_solucao=list(x)

    while not verifica_convergencia(matriz,constantes,x_solucao,precisao):
        
        for linha in range(len(matriz)):
            x_solucao[linha]=(x_solucao[linha] +
            (constantes[linha]-produto_interno(matriz[linha],x))/matriz[linha][linha])
        #Enquanto não se verificar a convergencia é feita uma nova estimativa
        #para a solução.

        x=tuple(x_solucao)
    
    return x