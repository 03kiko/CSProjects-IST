seed <- 2255
num_circuitos <- 9
reps <- 150
sinais <- 1:10

set.seed(seed)
probs_sinais <- sinais/55

avisos_sonoros <- 0
sistemas_n_desligados <- 0
for(i in 1:reps) {
  sistema <- sample(sinais, num_circuitos, replace = TRUE, prob = probs_sinais)
  aviso_sonoro <- 2 %in% sistema
  desligado <- 1 %in% sistema
  if(aviso_sonoro & !desligado) {
    avisos_sonoros <- avisos_sonoros + 1
  }
  if(!desligado) {
      sistemas_n_desligados <- sistemas_n_desligados + 1
  }
}

proporção <- avisos_sonoros / sistemas_n_desligados

print(proporção)