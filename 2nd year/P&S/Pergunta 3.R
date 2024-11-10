library(ggplot2)
theme_set(theme_gray())

dados <- readxl::read_xlsx("/home/kiko/Documents/IST/PE/Projeto/electricity.xlsx", col_types = "text")

fonte <- "Renewables"
ano <- 2015
países <- c("IEA Total", "Italy", "Latvia")

dados_filtrados <- subset(dados, YEAR >= ano & PRODUCT == fonte & COUNTRY %in% países)
dados_filtrados$DATE <- as.Date(paste0(dados_filtrados$YEAR, "-", dados_filtrados$MONTH, "-01"), "%Y-%m-%d")

ggplot(dados_filtrados, aes(x = DATE, y = 100 *as.numeric(share), color = COUNTRY, group = COUNTRY)) +
  geom_point()+
  geom_line() +
  scale_y_continuous(limits = c(0, 100), expand = c(0, 0))+
  scale_x_date(date_breaks = "1 year",
               date_labels = "%Y") +
  labs(title = paste0("Monthly renewable electricity share evolution (", 2015, "-2022)"),
       x = "Time (years)",
       y = "Renewable electricity share (%)", color = "Country")