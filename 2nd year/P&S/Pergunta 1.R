library(ggplot2)
theme_set(theme_gray())

url <- 'https://web.tecnico.ulisboa.pt/~paulo.soares/pe/projeto/Paises_PIB_ICH.csv'
dados <- read.csv(url, check.names = FALSE)
continentes <- c("Europe", "Americas")
paises <- c("Lithuania", "Iceland", "United States", "Saint Lucia")

dados |>
  subset(Continent %in% continentes) |>
  ggplot() +
  geom_point(aes(`GDP`, `HCI`, color = Continent)) +
  geom_text(aes(`GDP`, `HCI`, label = ifelse(Country %in% paises, Country, "")), vjust = -1, hjust = 1) +
  scale_x_log10() +
  labs(title = "Human Capital Index vs GDP per capita adjusted for the cost of living",
       x = "GDP per capita adjusted for the cost of living (2023, internacional dollars)",
       y = "Human Capital Index (2020, values between 0 and 1)")
