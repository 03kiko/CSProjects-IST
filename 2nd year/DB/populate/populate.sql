\copy clinica(nome, telefone, morada) FROM 'data/clinica.txt' DELIMITER ';' CSV;

\copy enfermeiro(nif, nome, telefone, morada, nome_clinica) FROM 'data/enfermeiro.txt' DELIMITER ';' CSV;

\copy medico(nif, nome, telefone, morada, especialidade) FROM 'data/medico.txt' DELIMITER ';' CSV;

\copy trabalha(nif, nome, dia_da_semana) FROM 'data/trabalha.txt' DELIMITER ';' CSV;

\copy paciente(ssn, nif, nome, telefone, morada, data_nasc) FROM 'data/paciente.txt' DELIMITER ';' CSV;

\copy consulta(ssn, nif, nome, data, hora, codigo_sns) FROM 'data/consulta.txt' DELIMITER ';' CSV;

\copy receita(codigo_sns, medicamento, quantidade) FROM 'data/receita.txt' DELIMITER ';' CSV;

\copy observacao(id, parametro, valor) FROM 'data/observacao.txt' DELIMITER ';' CSV;