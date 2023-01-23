% 106583 Francisco Monica
:- set_prolog_flag(answer_write_options,[max_depth(0)]).
:-['dados.pl'],['keywords.pl'].


%---Qualidade_dos_Dados---%

/*
 O predicado "eventosSemSalas" encontra todos os IDs de eventos sem sala
 atribuida.

 Argumento:
  -Eventos: lista ordenada e sem elementos repetidos dos IDs de eventos sem sala.

 eventosSemSalas(-Eventos)*/
eventosSemSalas(Eventos):-
    findall(ID,(evento(ID,_,_,_,'semSala')),EventosDesordenados),
    sort(EventosDesordenados,Eventos).

/*
 O predicado "eventosSemSalasDiaSemana" encontra todos os IDs de eventos sem
 sala atribuida que ocorrem num determinado dia da semana.

 Argumentos (respetivamente):
  -DiaDaSemana: cadeia de carateres que diz respeito a um dia da semana;
  -Eventos: lista ordenada e sem elementos repetidos dos IDs de eventos sem sala
   que decorrem em "DiaDaSemana".

 eventosSemSalasDiaSemana(+DiaDaSemana,-Eventos)*/
eventosSemSalasDiaSemana(DiaDaSemana,Eventos):-
    eventosSemSalas(SemSala),
    findall(ID,(horario(ID,DiaDaSemana,_,_,_,_),
                member(ID,SemSala)),EventosDesordenados),
    sort(EventosDesordenados,Eventos).

/*
 O predicado "eventosSemSalasPeriodo" encontra todos os IDs de eventos sem
 sala atribuida que ocorrem num dado periodo ou periodos. Os eventos de
 disciplinas semetrais sao contabilizadas nos periodos a que lhe correspondem
 ('p1_2' e contabilizado tanto como p1 como p2).

 Argumentos (respetivamente):
  -ListaPeriodos: lista de periodos;
  -Eventos: lista ordenada e sem elementos repetidos dos IDs de eventos sem sala
   que ocorrem nos periodos de ListaPeriodos.

 eventosSemSalasPeriodo(+ListaPeriodos,-Eventos)*/
eventosSemSalasPeriodo([],[]):-!.
eventosSemSalasPeriodo([Periodo|Tail],Eventos):-
    eventosSemSalas(SemSala),
    findall(ID,(horario(ID,_,_,_,_,P),
                periodoAux(Periodo,P),
                member(ID,SemSala)),EventosPeriodo),
    % Recorre-se ao predicado auxiliar "periodoAux" de modo a tambem contabilizar os
    % eventos de disciplinas semestrais.
    eventosSemSalasPeriodo(Tail,EventosAux),
    append(EventosPeriodo,EventosAux,EventosDesordenados),
    sort(EventosDesordenados,Eventos).

/*
 O predicado auxiliar "periodoAux" permite contabilizar os eventos de
 disciplinas semestrais, em que o segundo argumento tera de ser igual
 ao primeiro ou igual ao semestre correpondente desse.

 Argumentos (respetivamente):
  -Periodo: cadeia de carateres que representa um periodo;
  -P: cadeia de carateres que representa um periodo.

 periodoAux(+Periodo,+P)*/
periodoAux(Periodo,P):-
    Periodo == 'p1', member(P,['p1','p1_2']);
    Periodo == 'p2', member(P,['p2','p1_2']);
    Periodo == 'p3', member(P,['p3','p3_4']);
    Periodo == 'p4', member(P,['p4','p3_4']).


%---Pesquisas_Simples---%

/*
 O predicado "organizaEventos" filtra os IDs conforme estes pertencem ou nao
 ao periodo pretendido.

 Argumentos:
  -ListaEventos: lista com os IDs a filtrar;
  -Periodo: cadeia de carateres que representa um periodo;
  -EventosNoPeriodo: lista ordenada e sem elementos repetidos dos IDs da
   ListaEventos filtrados, isto e, que ocorrem no Periodo.

 organizaEventos(+ListaEventos,+Periodo,-EventosNoPeriodo)*/
organizaEventos([],_,[]):-!.
organizaEventos([ID|Tail],Periodo,EventosNoPeriodo):-
    horario(ID,_,_,_,_,P),
    periodoAux(Periodo,P),
    % Recorre-se novamente ao predicado auxiliar "periodoAux" pela mesma razao
    % anterior.
    organizaEventos(Tail,Periodo,IDsAux),
    sort([ID|IDsAux],EventosNoPeriodo),!.
organizaEventos([_|Tail],Periodo,Rest):- organizaEventos(Tail,Periodo,Rest).

/*
 O predicado "eventosMenoresQue" encontra todos os IDs de eventos com duracao
 menor ou igual do que uma determinada duracao.

 Argumentos (respetivamente):
  -Duracao: float que representa uma duracao;
  -EventosMenoresQue: lista ordenada e sem elementos repetidos dos IDs
   de eventos com menor ou igual duracao do que o argumento Duracao.

 eventosMenoresQue(+Duracao,-EventosMenoresQue)*/
eventosMenoresQue(Duracao,EventosMenoresQue):-
    findall(ID,(horario(ID,_,_,_,D,_),D=<Duracao),EventosMenoresQueDesordenados),
    sort(EventosMenoresQueDesordenados,EventosMenoresQue).

/*
 O predicado "eventosMenoresQueBool" verifica se o evento com um determinado
 ID tem ou nao duracao menor do que uma determinada duracao.

 Argumentos (respetivamente):
  -ID: inteiro que diz respeito ao ID de um determinado evento;
  - Duracao: float que diz respeito a uma determinada duracao.

 eventosMenoresQueBool(+ID,+Duracao)*/
eventosMenoresQueBool(ID,Duracao):-
    horario(ID,_,_,_,D,_),D=<Duracao.

/*
 O predicado "procuraDisciplinas" procura todas as disciplinas de um
 determinado curso.

 Argumentos (respetivamente):
  -Curso: cadeia de carateres que diz respeito ao curso;
  -ListaDisciplinas:lista ordenada com o nome das disciplinas do curso pretendido.

 procuraDisciplinas(+Curso,-ListaDisciplinas)*/
procuraDisciplinas(Curso,ListaDisciplinas):-
    findall(Disciplina,(evento(ID,Disciplina,_,_,_),
                        turno(ID,Curso,_,_)),ListaDisciplinasDesordenadas),
    sort(ListaDisciplinasDesordenadas,ListaDisciplinas).

/*
 O predicado "organizaDisciplinas" organiza as disciplinas de um determinado
 curso de acordo com o semestre a que pertencem. Este falha se nao existir
 no curso "Curso" uma disciplina de "ListaDisciplinas".

 Argumentos (respetivamente):
  -ListaDisciplinas: lista de disciplinas a organizar;
  -Curso: cadeia de carateres que diz repeito ao curso das disciplinas;
  -Semestres: lista de duas listas, em que a primeira corresponde a lista de
   disciplinas do primeiro semestre e a segunda as do segundo semestre.

 organizaDisciplinas(+ListaDisciplinas,+Curso,-Semestres)*/
organizaDisciplinas(ListaDisciplinas,Curso,[Semestre1,Semestre2]):-
    semestreDiv(ListaDisciplinas,Curso,S1,S2),
    % A divisao das disciplinas e feita atraves do predicado auxiliar "semestreDiv".
    sort(S1,Semestre1),
    sort(S2,Semestre2),!.

/*
 O predicado auxiliar "SemestreDiv" divide as disciplinas de acordo com o
 semestre a que pertencem.

 Argumentos:
  - ListaDisciplinas: lista de disciplinas a organizar;
  -Curso: curso das disciplinas
  -S1: lista de disciplinas do primeiro semestre;
  -S2: lista de disciplinas do segundo semestre.

 semestreDiv(+ListaDisciplinas,+Curso,-S1,-S2)*/
semestreDiv([],_,[],[]):-!.
semestreDiv([Disciplina|Rest],Curso,[Disciplina|Tail],S2):-
    turno(ID,Curso,_,_),
    evento(ID,Disciplina,_,_,_),
    horario(ID,_,_,_,_,P),
    member(P,['p1','p2','p1_2']),
    semestreDiv(Rest,Curso,Tail,S2),!.
semestreDiv([Disciplina|Rest],Curso,S1,[Disciplina|Tail]):-
   turno(ID,Curso,_,_),
   evento(ID,Disciplina,_,_,_),
   horario(ID,_,_,_,_,P),
   member(P,['p3','p4','p3_4']),
   semestreDiv(Rest,Curso,S1,Tail).

/*
 O predicado "horasCurso" calcula o total de horas de um curso num determinado
 periodo de um ano.

 Argumentos (respetivamente):
  -Periodo: cadeia de carateres que diz respeito ao periodo;
  -Curso: cadeia de carateres que diz respeito ao curso;
  -Ano: cadeia de carateres que diz respeito ao ano;
  -TotalHoras: float que diz respeito ao total de horas obtido.

 horasCurso(+Periodo,+Curso,+Ano,-TotalHoras)*/
horasCurso(Periodo,Curso,Ano,TotalHoras):-
    findall(D,(horario(ID,_,_,_,D,P),once(turno(ID,Curso,Ano,_)),
               periodoAux(Periodo,P)),Duracoes),
        % Recorre-se novamente ao predicado "periodoAux" pela mesma razao anterior.
    sumlist(Duracoes,TotalHoras).

/*
 O predicado "evolucaoHorasCurso" cria uma lista de tuplos que demonstra a
 evolucao do numero de horas de um curso ao longo de cada periodo e ano.

 Argumentos (respetivamente):
  -Curso: cadeia de carateres que diz respeito ao curso;
  -Evolucao: lista ordenada de tuplos da forma (Ano, Periodo, NumHoras) que
   demonstra a evolucao de horas de um curso ao longo dos periodos de cada ano.

 evolucaoHorasCurso(+Curso,-Evolucao)*/
evolucaoHorasCurso(Curso,Evolucao):-
    findall((Ano, Periodo, TH),(member(Ano, [1, 2, 3]),
                                member(Periodo, ['p1', 'p2', 'p3', 'p4']),
                                horasCurso(Periodo, Curso, Ano, TH)), Evolucao).


%---Ocupacao_critica_de_salas---%

/*
 O predicado "OcupaSlot" determina o numero de horas sobrepostas entre um evento,
 e um slot. Se nao ha sobreposicao, entao o predicado falha.

 Argumentos (respetivamente):
  -HoraInicioDada: float que indica a hora cujo slot tem inicio;
  -HoraFimDada: float que indica a hora cujo slot tem fim;
  -HoraInicioEvento: float que indica a hora cujo evento tem inicio;
  -HoraFimEvento: float que indica a hora cujo evento tem fim;
  -Horas: float que indica o numero de horas em que o slot e o evento se sobrepoem.

 ocupaSlot(+HoraInicioDada,+HoraFimDada,+HoraInicioEvento,HoraFimEvento,-Horas)*/
ocupaSlot(HoraInicioDada,HoraFimDada,HoraInicioEvento,HoraFimEvento,Horas):-
    HoraInicioDada=<HoraInicioEvento,HoraFimDada>HoraInicioEvento,
        (HoraFimEvento>HoraFimDada, Horas is HoraFimDada-HoraInicioEvento;
         HoraFimEvento=<HoraFimDada, Horas is HoraFimEvento-HoraInicioEvento),!;
    HoraInicioEvento=<HoraInicioDada,HoraFimEvento>HoraInicioDada,
        (HoraFimDada>HoraFimEvento,Horas is HoraFimEvento-HoraInicioDada;
         HoraFimDada=<HoraFimEvento,Horas is HoraFimDada-HoraInicioDada),!.

/*
 O predicado "numHorasOcupadas" determina o numero de horas ocupadas num
 intervalo de tempo, num determinado periodo, tipos de sala e dia da semana.

 Argumentos (respetivamente):
  -Periodo: cadeia de carateres que indica o periodo;
  -TipoSala: cadeia de carateres que indica o tipo de salas;
  -DiaSemana: cadeia de carateres que indica o dia da semana;
  -HoraInicio: float que indica o inicio do intervalo de tempo;
  -HoraFim: float que indica o fim do intervalo de tempo;
  -SomaHoras: float que indica o numero de horas obtido.

 numHorasOcupadas(+Periodo,+TipoSala,+DiaSemana,+HoraInicio,+HoraFim,-SomaHoras)*/
numHorasOcupadas(Periodo,TipoSala,DiaSemana,HoraInicio,HoraFim,SomaHoras):-
    salas(TipoSala,ListaSalas),
    findall([HIE,HFE],(horario(ID,DiaSemana,HIE,HFE,_,P),
                       evento(ID,_,_,_,Sala),
                       member(Sala,ListaSalas),
                       periodoAux(Periodo,P)),Duracoes),
    maplist(sobrepostas(HoraInicio,HoraFim),Duracoes,Sobreposicoes),
    sumlist(Sobreposicoes,SomaHoras),!.
% Recorre-se novamente ao predicado auxiliar "periodoAux" pela mesma razao
% anterior, recorre-se ainda ao predicado auxiliar "sobrepostas" que obtem
% o numero de horas sobrepostas entre um evento e um dado intervalo de tempo.

/*
 O predicado auxiliar "sobrepostas" e utilizado para obter o numero de horas
 sobrepostas de cada evento em relacao a um intervalo de tempo.

 Argumentos (respetivamente):
  -HID: float que indica o inicio do intervalo de tempo;
  -HFD: float que indica o fim do intervalo de tempo;
  -Duracao: lista que contem as horas de inicio e fim de um evento;
  -Sobreposicao: float que indica o numero de horas sobrepostas do intervalo dado
   em relacao ao evento.

 sobrepostas(+HID,+HFD,+Duracao,-Sobreposicao)*/
sobrepostas(HID,HFD,[Inicio,Fim|_],Sobreposicao):-
    (ocupaSlot(HID,HFD,Inicio,Fim,SH);SH is 0),
    Sobreposicao is SH.

/*
 O predicado "ocupacaoMax" determina o numero de horas maximo possivel que um
 tipo de salas pode ser ocupado num intervalo de tempo.

 Argumentos (respetivamente):
  -TipoSala: cadeia de carateres que indica o tipo de salas;
  -HoraInicio: float que indica a hora cujo intervalo de tempo tem inicio;
  -HoraFim: float que indica a horas cujo intervalo de tempo tem fim;
  -Max: float que indica o numero de horas maximo obtido.

 ocupacaoMax(+TipoSala,+HoraInicio,+HoraFim,-Max)*/
ocupacaoMax(TipoSala,HoraInicio,HoraFim,Max):-
    salas(TipoSala,ListaSalas),
    length(ListaSalas,N),
    Max is (HoraFim-HoraInicio)*N.

/*
 O predicado "percentagem" permite determinar a percentagem da soma de horas em
 relacao ao numero de horas maximo.

 Argumentos:
  -SomaHoras: float que indica a soma de horas;
  -Max: float que indica o numero de horas maximo;
  -Percentagem: float que indica a percentagem obtida.

 percentagem(+SomHoras,+Max,-Percentagem)*/
percentagem(SomaHoras,Max,Percentagem):-
    Percentagem is 100*(SomaHoras/Max).

/*
 O predicado "ocupacaoCritica" encontra casos criticos num dado intervalo de
 tempo. Os eventos sao considerados casos criticos quando a percentagem de
 ocupacao for maior que o threshold definido.

 Argumentos (respetivamente):
  -HoraInicio: float que indica a hora cujo intervalo de tempo tem inicio;
  -HoraFim: float que indica a horas cujo intervalo de tempo tem fim;
  -Threshold: inteiro que indica a percentagem a partir da qual se considera
   um evento um caso critico;
  -Resultados: lista ordenada de tuplos do tipo
   casosCriticos(DiaSemana,TipoSala,Percentagem), que contem os diversos casos
   criticos encontrados.

 ocupacaoCritica(+HoraInicio,+HoraFim,+Threshold,-Resultados)*/
ocupacaoCritica(HoraInicio,HoraFim,Threshold,Resultados):-
    findall(S,salas(S,_),TipoSala),
    tipoSala(HoraInicio,HoraFim,TipoSala,Threshold,R),
    sort(R,Resultados),!.
    % Recorre-se ao predicado auxiliar "TipoSala" que obtem casos criticos de cada
    % tipo de sala.

/*
 O predicado auxiliar "tipoSala" permite obter casos criticos de cada tipo de sala.

 Argumentos (respetivamente):
  -HI: float que indica a hora cujo intervalo de tempo tem inicio;
  -HF: float que indica a hora cujo intervalo de tempo tem fim;
  -TipoSala: lista de todos os tipos de sala;
  -Threshold: inteiro que indica a percentagem a partir da qual se considera
   um evento um caso critico;
  -Result: lista por ordenar de tuplos do tipo
   casosCriticos(DiaSemana,TipoSala,Percentagem), que contem os diversos casos
   criticos encontrados.

 tipoSala(+HID.+HF,+TipoSala,+Threshold,-Result)*/
tipoSala(_,_,[],_,[]):-!.
tipoSala(HI,HF,[TipoSala|Tail],Threshold,Result):-
    ocupacaoMax(TipoSala,HI,HF,Max),
    findall(R,(member(Dia,[segunda-feira,terca-feira,quarta-feira,quinta-feira,
                           sexta-feira]),
               member(Periodo,['p1','p2','p3','p4']),
               numHorasOcupadas(Periodo,TipoSala,Dia,HI,HF,SH),
               percentagem(SH,Max,Percentagem),
               Percentagem>Threshold,
               P is ceiling(Percentagem),
               R=..[casosCriticos,Dia,TipoSala,P]),Res),
    tipoSala(HI,HF,Tail,Threshold,Aux),
    append(Res,Aux,Result).


%---And_now_for_something_completely_different---%

/*
 O predicado "ocupacaoMesa" senta as pessoas numa mesa com 8 lugares de acordo
 com as restricoes presentes.

 Argumentos (respetivamente):
  -ListaPessoas: lista de pessoas a sentar;
  -ListaRestricoes: lista de restricoes a cumprir;
  -OcupacaoMesa: lista de listas que representa as pessoas sentadas na mesa.

 ocupacaoMesa(+ListaPessoas,+ListaRestricoes,-OcupacaoMesa)*/
ocupacaoMesa(ListaPessoas,ListaRestricoes,OcupacaoMesa):-
    maplist(senta(OcupacaoMesa),ListaPessoas),
    satisfazRestricoes(ListaRestricoes,OcupacaoMesa).
    /*
     Recorre-se ao predicado auxiliar "senta" para sentar todas as pessoas na mesa e
     verifica-se atraves do predicado auxiliar "satisfazRestricoes" se todas as
     restricoes estao a ser cumpridas, caso nao estejam o predicado senta origina uma
     nova combinacao para a mesa, ate encotrar uma combinacao que cumpra todas as
     restricoes.*/

/*
 O predicado auxiliar "senta" permite sentar as pessoas na mesa procurando a
 combinacao que satisfaz todas as restricoes existentes.

 Argumentos (respetivamente):
  -OcupacaoMesa: lista de listas que representa as pessoas sentadas na mesa;
  -P: cadeia de carateres com o nome da pessoa a sentar.

 senta(-OcupacaoMesa,+P)*/
senta([[P,_,_],[_,_],[_,_,_]],P).
senta([[_,P,_],[_,_],[_,_,_]],P).
senta([[_,_,P],[_,_],[_,_,_]],P).
senta([[_,_,_],[P,_],[_,_,_]],P).
senta([[_,_,_],[_,P],[_,_,_]],P).
senta([[_,_,_],[_,_],[P,_,_]],P).
senta([[_,_,_],[_,_],[_,P,_]],P).
senta([[_,_,_],[_,_],[_,_,P]],P).

/*
 O predicado auxiliar "satisfazRestricoes permite verificar se as retricoes
 existentes sao cumpridas.

 Argumentos (respetivamente):
  -ListaRestricoes: lista de restricoes a cumprir;
  -OcupacaoMesa: lista de listas que representa as pessoas sentadas na mesa.

 satisfazRestricoes(+ListaRestricoes,+OcupacaoMesa)*/
satisfazRestricoes([],_):-!.
satisfazRestricoes([Restricao|Tail],OcupacaoMesa):-
    satisfazRestricoes(Tail,OcupacaoMesa),
    call(Restricao,OcupacaoMesa).

/*
 Os restantes predicados enumeram as possibilidades do que seria uma mesa ao
 cumprir determinadas restricoes.

 Restricoes possiveis:
  -cab1(P1,OcupacaoMesa): a pessoa P1 fica sentada na cabeceira 1;
  -cab2(P1,OcupacaoMesa): a pessoa P1 fica sentada na cabeceira 2;
  -honra(P1,P2,OcupacaoMesa): a pessoa P1 fica sentada numa das cabeceiras e P2 a
   direita desta;
  -lado(P1,P2,OcupacaoMesa): as pessoas P1 e P2 ficam sentadas lado a lado;
  -naoLado(P1,P2,OcupacaoMesa): as pessoas P1 e P2 nao ficam sentadas lado a lado;
  -frente(P1,P2,OcupacaoMesa): as pessoas P1 e P2 ficam frente a frente na mesa;
  -naoFrente(P1,P2,OcupacaoMesa): as pessoas P1 e P2 nao ficam frente a frente na
   mesa.
 Em todas estas restricoes a OcupacaoMesa representa a mesa com essa restricao
 aplicada.*/

cab1(X,[[_,_,_],[X,_],[_,_,_]]).
cab2(X,[[_,_,_],[_,X],[_,_,_]]).

honra(X,Y,[[_,_,_],[X,_],[Y,_,_]]).
honra(X,Y,[[_,_,Y],[_,X],[_,_,_]]).

lado(X,Y,[[X,Y|_],[_,_],[_,_,_]]).
lado(X,Y,[[Y,X|_],[_,_],[_,_,_]]).
lado(X,Y,[[_|R],[_,_],[_,_,_]]):-lado(X,Y,[R,[_,_],[_,_,_]]).
lado(X,Y,[[_,_,_],[_,_],[X,Y|_]]).
lado(X,Y,[[_,_,_],[_,_],[Y,X|_]]).
lado(X,Y,[[_,_,_],[_,_],[_|R]]):-lado(X,Y,[[_,_,_],[_,_],R]).

naoLado(X,Y,OcupacaoMesa):-
    \+lado(X,Y,OcupacaoMesa).

frente(X,Y,[[X|_],[_,_],[Y|_]]).
frente(X,Y,[[Y|_],[_,_],[X|_]]).
frente(X,Y,[[_|Lado1],[_,_],[_|Lado2]]):-frente(X,Y,[Lado1,[_,_],Lado2]).

naoFrente(X,Y,OcupacaoMesa):-
    \+frente(X,Y,OcupacaoMesa).
