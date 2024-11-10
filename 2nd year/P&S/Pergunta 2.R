library(ggplot2)
theme_set(theme_light())

url <- 'https://web.tecnico.ulisboa.pt/~paulo.soares/pe/projeto/master.csv'
dados <- read.csv(url, check.names = FALSE)
ano <- 1986
grupo_etário <- "25-34 years"

dados_filtrados <- subset(dados, year == ano & age == grupo_etário)

ggplot(dados_filtrados) +
  geom_boxplot(aes(`sex`, `suicides/100k pop`, fill = sex)) +
  labs(title = paste0("Suicides per 100k population by sex for age group 25-34 in ", ano),
       x = "Sex",
       y = "Suicides per 100k population")