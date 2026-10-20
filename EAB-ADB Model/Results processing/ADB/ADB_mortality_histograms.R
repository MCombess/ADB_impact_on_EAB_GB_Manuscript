

rm(list = ls())

library(dplyr)
library(ggplot2)
library(terra)
library(patchwork)

#Ok, first put together the mortality data

#Read in the relevant data (rds files generated through ADB_base_layer_processing.R file):

jlm <- readRDS("Juv_ash_now_Low_R.rds")
mlm <- readRDS("Mat_ash_now_Low_R.rds")
alm <- readRDS("All_ash_now_Low_R.rds")


jmm <- readRDS("Juv_ash_now_median_R.rds")
mmm <- readRDS("Mat_ash_now_median_R.rds")
amm <- readRDS("All_ash_now_median_R.rds")


jhm <- readRDS("Juv_ash_now_High_R.rds")
mhm <- readRDS("Mat_ash_now_High_R.rds")
ahm <- readRDS("All_ash_now_High_R.rds")


jlmp <- ggplot(jlm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(jlm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jlm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jlm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),plot.title = element_text(hjust = 0.5, face = "bold", size = 10.5, margin = margin(b = -3)),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = "Juvenile",subtitle = expression(bold("a.")), y="",x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
jlmp

mlmp <- ggplot(mlm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(mlm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mlm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mlm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),plot.title = element_text(hjust = 0.5, face = "bold", size = 10.5, margin = margin(b = -3)),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = "Mature",subtitle = expression(bold("b.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
mlmp

almp <- ggplot(alm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(alm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(alm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(alm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),plot.title = element_text(hjust = 0.5, face = "bold", size = 10.5, margin = margin(b = -3)),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = "All",subtitle = expression(bold("c.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
almp

jmmp <- ggplot(jmm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(jmm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jmm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jmm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("d.")), y="",x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
jmmp

mmmp <- ggplot(mmm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(mmm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mmm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mmm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("e.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
mmmp

ammp <- ggplot(amm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(amm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(amm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(amm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("f.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
ammp

jhmp <- ggplot(jhm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(jhm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jhm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(jhm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("g.")), y="",x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
jhmp

mhmp <- ggplot(mhm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(mhm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mhm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(mhm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("h.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
mmmp

ahmp <- ggplot(ahm, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(ahm$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(ahm$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(ahm$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.subtitle = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(subtitle = expression(bold("i.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
ahmp




# Add row labels as axis titles on leftmost plot
jlmp <- jlmp + ylab("Low ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))
jmmp <- jmmp + ylab("Median ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))
jhmp <- jhmp + ylab("High ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))


combined <- jlmp+mlmp+almp+jmmp+
  mmmp+ammp+jhmp+mhmp+ahmp+plot_layout(ncol = 3)+theme(plot.margin = margin(0,0,0,0))

combined


#ggsave("") save plot





