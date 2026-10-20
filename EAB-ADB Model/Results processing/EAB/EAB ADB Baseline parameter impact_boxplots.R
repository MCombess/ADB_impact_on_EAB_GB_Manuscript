

#Examining impact of adjusted parameters in the model examining impacts of ADB on EAB (see table 2 in paper)
#Simply change input file for different model runs to examine different model parameters 
#this script uses larval mortality scenario runs as an example

#See Table 2 in paper for full description.
#Run 1 = Baseline ADB impact
#Run 2 = No ADB impact (see other R files 'EAB ADB baseline vs no ADB_p1.R' and 'EAB ADB baseline vs no ADB_p2.R')
#Run 3 = Half baseline EAB attraction to ADB stressed tress
#Run 4 = Quarter baseline EAB attraction to ADB stressed tress
#Run 5 = No difference in EAB attraction to ADB stressed tress
#Run 6 = Larval mortality scenario 1
#Run 7 = Larval mortality scenario 2
#Run 9 = EAB arrival in 2030, not 2025
#Run 10 = Low ADB rate
#Run 11 = High ADB rate

rm(list=ls())

library(dplyr)
library(ggplot2)


#Read in the output file from C++
#Baseline scenario of ADB impact on EAB#
Run1 <- read.table("DataForOptJuneRun1.txt")

#Larval mortality Runs
Run3 <- read.table("CDataForOptJuneRun6.txt")
Run4 <- read.table("DataForOptJuneRun7.txt")
#Column 1 = iteration number
#Column 2 = Y of arrival cell
#Column 3 = X of arrival cell
#Column 4 = arrival prob in the cell
#Column 5 = Initial ash tree number
#Column 6 = Row
#Column 7 = Column
#Columns 8-16 = average larvae per tree
#Columns 17-25 = number of ash the cell
#Columns 26-34 = number of 'healthy' ash in the cell
#Columns 35-43 = I trees
#Columns 44-52 = S trees
#Columns 53-61 = D trees

totally <- 1

df1 <- cbind(Run1,totally)
df3 <- cbind(Run3, totally)
df4 <- cbind(Run4, totally)

rm(Run1,Run3, Run4)

#Number of cells colonised each year
df1y1 <- df1 %>% group_by(V1) %>% filter(V8>0.0) %>% mutate (numcells = sum(totally))
df3y1 <- df3 %>% group_by(V1) %>% filter(V8>0.0) %>% mutate (numcells = sum(totally))
df4y1 <- df4 %>% group_by(V1) %>% filter(V8>0.0) %>% mutate (numcells = sum(totally))

df1y2 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(totally))
df3y2 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(totally))
df4y2 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(totally))



df1y3 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(totally))
df3y3 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(totally))
df4y3 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(totally))

df1y4 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(totally))
df3y4 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(totally))
df4y4 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(totally))


df1y5 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(totally))
df3y5 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(totally))
df4y5 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(totally))


df1y6 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(totally))
df3y6 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(totally))
df4y6 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(totally))


df1y7 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(totally))
df3y7 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(totally))
df4y7 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(totally))

df1y8 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(totally))
df3y8 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(totally))
df4y8 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(totally))

df1y9 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(totally))
df3y9 <- df3 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(totally))
df4y9 <- df4 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(totally))

df1y1 <- df1y1[,c(1,63)]
df3y1 <- df3y1[,c(1,63)]
df4y1 <- df4y1[,c(1,63)]

df1y2 <- df1y2[,c(1,63)]
df3y2 <- df3y2[,c(1,63)]
df4y2 <- df4y2[,c(1,63)]

df1y3 <- df1y3[,c(1,63)]
df3y3 <- df3y3[,c(1,63)]
df4y3 <- df4y3[,c(1,63)]

df1y4 <- df1y4[,c(1,63)]
df3y4 <- df3y4[,c(1,63)]
df4y4 <- df4y4[,c(1,63)]

df1y5 <- df1y5[,c(1,63)]
df3y5 <- df3y5[,c(1,63)]
df4y5 <- df4y5[,c(1,63)]

df1y6 <- df1y6[,c(1,63)]
df3y6 <- df3y6[,c(1,63)]
df4y6 <- df4y6[,c(1,63)]

df1y7 <- df1y7[,c(1,63)]
df3y7 <- df3y7[,c(1,63)]
df4y7 <- df4y7[,c(1,63)]

df1y8 <- df1y8[,c(1,63)]
df3y8 <- df3y8[,c(1,63)]
df4y8 <- df4y8[,c(1,63)]

df1y9 <- df1y9[,c(1,63)]
df3y9 <- df3y9[,c(1,63)]
df4y9 <- df4y9[,c(1,63)]

df1y1 <- df1y1[!duplicated(df1y1), ]
df1y2 <- df1y2[!duplicated(df1y2), ]
df1y3 <- df1y3[!duplicated(df1y3), ]
df1y4 <- df1y4[!duplicated(df1y4), ]
df1y5 <- df1y5[!duplicated(df1y5), ]
df1y6 <- df1y6[!duplicated(df1y6), ]
df1y7 <- df1y7[!duplicated(df1y7), ]
df1y8 <- df1y8[!duplicated(df1y8), ]
df1y9 <- df1y9[!duplicated(df1y9), ]

df3y1 <- df3y1[!duplicated(df3y1), ]
df3y2 <- df3y2[!duplicated(df3y2), ]
df3y3 <- df3y3[!duplicated(df3y3), ]
df3y4 <- df3y4[!duplicated(df3y4), ]
df3y5 <- df3y5[!duplicated(df3y5), ]
df3y6 <- df3y6[!duplicated(df3y6), ]
df3y7 <- df3y7[!duplicated(df3y7), ]
df3y8 <- df3y8[!duplicated(df3y8), ]
df3y9 <- df3y9[!duplicated(df3y9), ]

df4y1 <- df4y1[!duplicated(df4y1), ]
df4y2 <- df4y2[!duplicated(df4y2), ]
df4y3 <- df4y3[!duplicated(df4y3), ]
df4y4 <- df4y4[!duplicated(df4y4), ]
df4y5 <- df4y5[!duplicated(df4y5), ]
df4y6 <- df4y6[!duplicated(df4y6), ]
df4y7 <- df4y7[!duplicated(df4y7), ]
df4y8 <- df4y8[!duplicated(df4y8), ]
df4y9 <- df4y9[!duplicated(df4y9), ]

df1y1$year <- 1
df1y2$year <- 2
df1y3$year <- 3
df1y4$year <- 4
df1y5$year <- 5
df1y6$year <- 6
df1y7$year <- 7
df1y8$year <- 8
df1y9$year <- 9

df3y1$year <- 1
df3y2$year <- 2
df3y3$year <- 3
df3y4$year <- 4
df3y5$year <- 5
df3y6$year <- 6
df3y7$year <- 7
df3y8$year <- 8
df3y9$year <- 9

df4y1$year <- 1
df4y2$year <- 2
df4y3$year <- 3
df4y4$year <- 4
df4y5$year <- 5
df4y6$year <- 6
df4y7$year <- 7
df4y8$year <- 8
df4y9$year <- 9

#Calculated the number of cells invaded by year#

df1f <- rbind(df1y1,df1y2,df1y3, df1y4,df1y5, df1y6,df1y7, df1y8,df1y9)
rm(df1y1,df1y2,df1y3, df1y4,df1y5, df1y6,df1y7, df1y8,df1y9, df1)

df3f <- rbind(df3y1,df3y2,df3y3, df3y4,df3y5, df3y6,df3y7, df3y8,df3y9)
rm(df3y1,df3y2,df3y3, df3y4,df3y5, df3y6,df3y7, df3y8,df3y9, df3)

df4f <- rbind(df4y1,df4y2,df4y3, df4y4,df4y5, df4y6,df4y7, df4y8,df4y9)
rm(df4y1,df4y2,df4y3, df4y4,df4y5, df4y6,df4y7, df4y8,df4y9, df4)

names(df1f)[1] <- "Realisation"
names(df3f)[1] <- "Realisation"
names(df4f)[1] <- "Realisation"

df1f$Scenario <- "Baseline mortality"
df3f$Scenario <- "Mortality scenario 1"
df4f$Scenario <- "Mortality scenario 2"

df <- rbind(df1f,df3f,df4f)

#Boxplots through time
df$numcells <- as.integer(df$numcells)
df$year <- df$year-1
#
df$Scenario <- factor(df$Scenario,
                      levels = c(
                        "Baseline mortality",
                        "Mortality scenario 1",
                        "Mortality scenario 2"
                      ))

#Boxplot
eabplotarrival <- ggplot(df, aes(x = factor(year), y = numcells, fill = Scenario)) +
  theme_bw() +
  geom_boxplot(outlier.shape = NA, coef = 10, alpha = 0.3) +
  geom_point(aes(color = Scenario), 
             position = position_dodge(width = 0.75),
             size = 0.8, alpha = 0.5) +
  scale_y_continuous(
    limits = c(0, 1050),
    breaks = seq(0, 1000, by = 100)
  ) +  labs(x = "Years after EAB arrival",
       y = expression("1 km"^2*" cells with EAB"), title = expression(bold("a."))) +
  scale_fill_manual(
    name = "Scenario",
    values = c(
      "Baseline mortality" = "#F8766D",
      "Mortality scenario 1"              = "green3",
      "Mortality scenario 2"              = "purple"
    ),
    labels = c(
    "Baseline mortality" = " \nBaseline\nmortality\n ",
      "Mortality scenario 1"              = " \nMortality\nscenario 1\n ",
      "Mortality scenario 2"              = " \nMortality\nscenario 2\n "
    )
  ) +
  scale_color_manual(
    name = "Scenario",
    values = c(
      "Baseline mortality" = "#F8766D",
      "Mortality scenario 1"              = "green3",
      "Mortality scenario 2"              = "purple"
    ),
    labels = c(
      "Baseline mortality" = " \nBaseline\nmortality\n ",
      "Mortality scenario 1"              = " \nMortality\nscenario 1\n ",
      "Mortality scenario 2"              = " \nMortality\nscenario 2\n "
    )
  ) +guides(
    fill = guide_legend(),
    color = guide_legend()
  ) +
  theme(
    text = element_text(size = 10),
    legend.title = element_blank(),
    plot.title = element_text(hjust = -0.05, vjust = 2)
  )
eabplotarrival


#############################    Plotting the difference compared to the ADB baseline model     ##########################

ndf3 <- df1f
ndf3$Scenario <- "Mortality scenario 1"
ndf3$numcells <- df3f$numcells-df1f$numcells
ndf3$year <- ndf3$year-1

ndf4 <- df1f
ndf4$Scenario <- "Mortality scenario 2"
ndf4$numcells <- df4f$numcells-df1f$numcells
ndf4$year <- ndf4$year-1


ndf <- rbind(ndf3,ndf4)
ndf$Scenario <- factor(ndf$Scenario,
  levels = c("Mortality scenario 1", "Mortality scenario 2")
)

#Plot
eabplotarrivaldiff <- ggplot(ndf, aes(x = factor(year), y = numcells, fill = Scenario, color = Scenario)) +
  theme_bw() +
  geom_boxplot(outlier.shape = NA, coef = 10, alpha = 0.3,color="black") +
  geom_point(position = position_dodge(width = 0.75),
             size = 0.8, alpha = 0.5) +
  scale_y_continuous(
    limits = c(-300, 700),
   breaks = seq(-300, 700, by = 100)
  ) +  labs(x = "Years after EAB arrival",
       y = expression("1 km"^2*" cells colonised by EAB vs baseline survival"),
       title = expression(bold("b."))) +
  scale_fill_manual(
    values = c(
      "Mortality scenario 1" = "green3",
      "Mortality scenario 2" = "purple"
    )
  ) +
  scale_color_manual(
    values = c(
      "Mortality scenario 1" = "green3",
      "Mortality scenario 2" = "purple"
    )
  ) +guides(
    fill = guide_legend(),
    color = guide_legend()
  ) +
  theme(
    text = element_text(size = 10),
    legend.title = element_blank(),
    plot.title = element_text(hjust = -0.05, vjust = 2),legend.position = "none"
  )

eabplotarrivaldiff

###### Combining plots with shared legend ######

library(patchwork)
library(cowplot)
legend <- get_legend(eabplotarrival)

p1 <- eabplotarrival + theme(legend.position = "none")
p2 <- eabplotarrivaldiff + theme(legend.position = "none")

combined <- plot_grid(p1, p2, nrow = 2, rel_widths = c(1, 0.05, 1), align = "v")

final_plot <- plot_grid(combined, legend, ncol = 2, rel_widths = c(1, 0.2))
final_plot

#ggsave("")

#############################       Summary statistics      ##############################################

#Individual runs (i.e. not compared to the ADB baseline model)#
summary_df <- df %>%
  group_by(Scenario, year) %>%
  summarise(
    median_numcells = median(numcells, na.rm = TRUE),
    lower_quartile = quantile(numcells, 0.25, na.rm = TRUE),
    upper_quartile = quantile(numcells, 0.75, na.rm = TRUE),
    lower_95 = quantile(numcells, 0.025, na.rm = TRUE),
    upper_95 = quantile(numcells, 0.975, na.rm = TRUE),
    min_numcells = min(numcells, na.rm = TRUE),
    max_numcells = max(numcells, na.rm = TRUE)
  ) %>%
  ungroup()
print(summary_df)

write.csv(summary_df, "Survival_summary.csv", row.names = FALSE) #Summary statistics
write.csv(df, "Survival_all.csv", row.names = FALSE) #Save all data to quickly replot


#Compared to the ADB baseline model#
summary_dfcompare <- ndf %>%
  group_by(Scenario,year) %>%
  summarise(
    median_numcells = median(numcells, na.rm = TRUE),
    lower_quartile = quantile(numcells, 0.25, na.rm = TRUE),
    upper_quartile = quantile(numcells, 0.75, na.rm = TRUE),
    lower_95 = quantile(numcells, 0.025, na.rm = TRUE),
    upper_95 = quantile(numcells, 0.975, na.rm = TRUE),
    min_numcells = min(numcells, na.rm = TRUE),
    max_numcells = max(numcells, na.rm = TRUE)
  ) %>%
  ungroup()
print(summary_dfcompare)

write.csv(summary_dfcompare, "Survival_diff_summary.csv", row.names = FALSE) #Summary statistics
write.csv(ndf, "Survival_diff_all.csv", row.names = FALSE) #Save all data to quickly replot








