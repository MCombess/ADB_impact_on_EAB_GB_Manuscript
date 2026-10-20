

#Processing Output files from the model#

rm(list=ls())

library(dplyr)
library(ggplot2)

#Read in the output file from C++
#Most likely interacation scenario (Baseline ADB impact on EAB)#
Run1 <- read.table("DataForOptJuneRun1.txt")

#With no ash dieback#
Run2 <- read.table("DataForOptJuneRun2.txt")

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

#Organising data#
totally <- 1

df1 <- cbind(Run1,totally)
df2 <- cbind(Run2, totally)

rm(Run1,Run2)

#Number of cells colonised each model year
df1y1 <- df1 %>% group_by(V1) %>% filter(V8>0.0) %>% mutate (numcells = sum(totally))
df2y1 <- df2 %>% group_by(V1) %>% filter(V8>0.0) %>% mutate (numcells = sum(totally))

df1y2 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(totally))
df2y2 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(totally))


df1y3 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(totally))
df2y3 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(totally))


df1y4 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(totally))
df2y4 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(totally))


df1y5 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(totally))
df2y5 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(totally))


df1y6 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(totally))
df2y6 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(totally))


df1y7 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(totally))
df2y7 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(totally))


df1y8 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(totally))
df2y8 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(totally))

df1y9 <- df1 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(totally))
df2y9 <- df2 %>% group_by(V1) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(totally))

df1y1 <- df1y1[,c(1,63)]
df2y1 <- df2y1[,c(1,63)]

df1y2 <- df1y2[,c(1,63)]
df2y2 <- df2y2[,c(1,63)]

df1y3 <- df1y3[,c(1,63)]
df2y3 <- df2y3[,c(1,63)]

df1y4 <- df1y4[,c(1,63)]
df2y4 <- df2y4[,c(1,63)]

df1y5 <- df1y5[,c(1,63)]
df2y5 <- df2y5[,c(1,63)]

df1y6 <- df1y6[,c(1,63)]
df2y6 <- df2y6[,c(1,63)]

df1y7 <- df1y7[,c(1,63)]
df2y7 <- df2y7[,c(1,63)]

df1y8 <- df1y8[,c(1,63)]
df2y8 <- df2y8[,c(1,63)]

df1y9 <- df1y9[,c(1,63)]
df2y9 <- df2y9[,c(1,63)]

df1y1 <- df1y1[!duplicated(df1y1), ]
df1y2 <- df1y2[!duplicated(df1y2), ]
df1y3 <- df1y3[!duplicated(df1y3), ]
df1y4 <- df1y4[!duplicated(df1y4), ]
df1y5 <- df1y5[!duplicated(df1y5), ]
df1y6 <- df1y6[!duplicated(df1y6), ]
df1y7 <- df1y7[!duplicated(df1y7), ]
df1y8 <- df1y8[!duplicated(df1y8), ]
df1y9 <- df1y9[!duplicated(df1y9), ]

df2y1 <- df2y1[!duplicated(df2y1), ]
df2y2 <- df2y2[!duplicated(df2y2), ]
df2y3 <- df2y3[!duplicated(df2y3), ]
df2y4 <- df2y4[!duplicated(df2y4), ]
df2y5 <- df2y5[!duplicated(df2y5), ]
df2y6 <- df2y6[!duplicated(df2y6), ]
df2y7 <- df2y7[!duplicated(df2y7), ]
df2y8 <- df2y8[!duplicated(df2y8), ]
df2y9 <- df2y9[!duplicated(df2y9), ]

df1y1$year <- 1
df1y2$year <- 2
df1y3$year <- 3
df1y4$year <- 4
df1y5$year <- 5
df1y6$year <- 6
df1y7$year <- 7
df1y8$year <- 8
df1y9$year <- 9

df2y1$year <- 1
df2y2$year <- 2
df2y3$year <- 3
df2y4$year <- 4
df2y5$year <- 5
df2y6$year <- 6
df2y7$year <- 7
df2y8$year <- 8
df2y9$year <- 9

#Ok, so I've now calculated the number of cells invaded by year#

df1f <- rbind(df1y1,df1y2,df1y3, df1y4,df1y5, df1y6,df1y7, df1y8,df1y9)
rm(df1y1,df1y2,df1y3, df1y4,df1y5, df1y6,df1y7, df1y8,df1y9, df1)

df2f <- rbind(df2y1,df2y2,df2y3, df2y4,df2y5, df2y6,df2y7, df2y8,df2y9)
rm(df2y1,df2y2,df2y3, df2y4,df2y5, df2y6,df2y7, df2y8,df2y9, df2)

names(df1f)[1] <- "Realisation"
names(df2f)[1] <- "Realisation"

df1f$Scenario <- "Most likely ADB interaction"
df2f$Scenario <- "No ADB"

df <- rbind(df1f,df2f) #We save this to then use in the R script "EAV ADB baseline vs no ADB_p2.R"

#Long-winded way, but read again and organise for map plot and histograms#
Run1 <- read.table("DataForOptJuneRun1.txt")

#With no ash dieback#
Run2 <- read.table("DataForOptJuneRun2.txt")

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


df1 <- Run1
df2 <- Run2

rm(Run1,Run2)

df1$present <- 1
df2$present <- 1

df1y1 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0) %>% mutate (numcells = sum(present))
df2y1 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0) %>% mutate (numcells = sum(present))

df1y2 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(present))
df2y2 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0) %>% mutate (numcells = sum(present))


df1y3 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(present))
df2y3 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0) %>% mutate (numcells = sum(present))


df1y4 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(present))
df2y4 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0) %>% mutate (numcells = sum(present))


df1y5 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(present))
df2y5 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0) %>% mutate (numcells = sum(present))

df1y6 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(present))
df2y6 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0) %>% mutate (numcells = sum(present))


df1y7 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(present))
df2y7 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0) %>% mutate (numcells = sum(present))


df1y8 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(present))
df2y8 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0) %>% mutate (numcells = sum(present))

df1y9 <- df1 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(present))
df2y9 <- df2 %>% group_by(V6, V7) %>% filter(V8>0.0|V9>0.0|V10>0.0|V11>0.0|V12>0.0|V13>0.0|V14>0.0|V15>0.0|V16>0.0) %>% mutate (numcells = sum(present))


library(terra)

#Read in the ash density map - so we can filter out cells with no ash#
pa <- as.matrix(read.table("treeDensity_GB.txt"))
rpa <- rast(pa)

df2y9$V6 <- as.numeric(df2y9$V6)
df2y9$V7 <- as.numeric(df2y9$V7)
df2y9$numcells <- df2y9$numcells/9993 #Probability cell invaded during 9993 runs
ymin <- ext(rpa)[3]
ymax <- ext(rpa)[4]
df2y9$V6b <- ymax - (df2y9$V6 - ymin)
v <- vect(df2y9, geom=c("V7","V6b"))

ra9 <- rasterize(v, rpa, field="numcells")
ra9 <- mask(ra9,rpa,maskvalue=-3.3,updatevalue=NA) #Remove the cells without anything
ra9 <- mask(ra9,rpa,maskvalue=0,updatevalue=NA) #Remove the cells with no ash


df1y9$V6 <- as.numeric(df1y9$V6)
df1y9$V7 <- as.numeric(df1y9$V7)
df1y9$numcells <- df1y9$numcells/9993 #Probability cell invaded during 9993 runs
ymin <- ext(rpa)[3]
ymax <- ext(rpa)[4]
df1y9$V6b <- ymax - (df1y9$V6 - ymin)
rm(v)
v <- vect(df1y9, geom=c("V7","V6b"))

rn9 <- rasterize(v, rpa, field="numcells")
rn9 <- mask(rn9,rpa,maskvalue=-3.3,updatevalue=NA) #Remove the cells without anything
rn9 <- mask(rn9,rpa,maskvalue=0,updatevalue=NA) #Remove the cells with no ash

newinvnoadb <- (rn9 > 0) #Ok, gets us the points where ADB-EAB run gets invaded (TRUE/FALSE)
newinvadb <-  (ra9 > 0) #Ok, gets us the points where no ADB scenario gets invaded (TRUE/FALSE)

newinvadb_df <- as.data.frame(newinvadb, xy=TRUE)
newinvnoadb_df <- as.data.frame(newinvnoadb, xy=TRUE)

newinvadb_df$last <- as.integer(newinvadb_df$last) 
newinvnoadb_df$last <- as.integer(newinvnoadb_df$last)

newinvadb_df <- newinvadb_df %>% filter(last>0) #Filters to the cells where no ADB scenario actually gets invaded:
newinvnoadb_df <- newinvnoadb_df %>% filter(last>0) #Filters to the cells where basleline EAB_ADB scenario actually gets invaded:

rn9df <- as.data.frame(rn9, xy=TRUE, na.rm = FALSE) %>% mutate(raster = "No ADB") #Making a dataframe for probability of invasion for ADB-EAB model cells#
ra9df <- as.data.frame(ra9, xy=TRUE, na.rm = FALSE) %>% mutate(raster = "ADB") #Making a dataframe for probability of invasion for no ADB scenario cells#

diffdf <- ra9df
diffdf$last <- rn9df$last-ra9df$last #Probability of invasion for ADB-EAB model minus probability of invasion in the no ADB scenario

#diffdfb <- na.omit(diffdf)
 
diffdfbpos <- diffdf %>% filter(last>0) ##Number of cells for ADB-EAB scenario where invasion probability is increased compared to no ADB scenario:
diffdfbneg <- diffdf %>% filter(last<0) ##Number of cells for ADB-EAB scenario where invasion probability is decreased compared to no ADB scenario: 


#write.csv(diffdfbpos,file="Pos_diff_EAB_ADB_baseline.csv")

d9p <- ggplot() +
  geom_tile(data=diffdfbpos, aes(x = x, y =y, fill = last)) +
  coord_fixed(xlim = c(37.5, 600.5), ylim = c(3.5, 947.5))+
  scale_fill_gradient(
    low  = "#d4bfee",
    high = "#5e3c8f",
    limits = c(0, 0.026),
    breaks = c(0, 0.008, 0.016, 0.024) 
    )+
  theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -3.5,size=10),axis.title = element_blank(),
        axis.text = element_blank(),
        axis.ticks = element_blank(),
        panel.grid = element_blank(),
        panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
        legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position="right")+
  labs(fill = "Difference in\nprobability of EAB", title = expression(bold("c.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4, 
      barheight = 0.5,
      title.position = "top",
      title.vjust = 2,
      title.hjust = 0.5
    )
  )
d9p

#write.csv(diffdfbneg,file="Neg_diff_EAB_ADB_baseline.csv")

d9n <- ggplot() +
  geom_tile(data=diffdfbneg, aes(x = x, y =y, fill = last)) +
  coord_fixed(xlim = c(37.5, 600.5), ylim = c(3.5, 947.5)) +
  scale_fill_gradient(
    high  = "#fac9b3",
    low = "#d6604d",
    limits = c(-0.026, 0),
    breaks = c(-0.024, -0.016, -0.008,0)
  ) +
  theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -7,size=10),axis.title = element_blank(),
        axis.text = element_blank(),
        axis.ticks = element_blank(),
        panel.grid = element_blank(),
        panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
        legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position="right")+
  labs(fill = "Difference in\nprobability of EAB", title = expression(bold("d.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4,
      barheight = 0.5,
      title.position = "top",
      title.hjust = 0.5,
      title.vjust = 2
    )
  )
d9n

diffdfb <- na.omit(diffdf)

#write.csv(diffdfb,file="All_diff_EAB_ADB_baseline.csv")


bp <- ggplot(diffdfb, aes(x = last)) +
  geom_histogram(bins = 300, fill = "orange", color = "black") +
  geom_point(aes(y=0),shape=4,colour="red",size=2)+
  geom_vline(xintercept = min(diffdfb$last), color = "red", linetype = "solid", size = 1, alpha=0.8) +
  geom_vline(xintercept = max(diffdfb$last), color = "red", linetype = "solid", size = 1, alpha=0.8) +
  geom_vline(xintercept = median(diffdfb$last), color = "red", linetype = "solid", size = 1, alpha=0.8) +
  geom_vline(xintercept = quantile(diffdfb$last, 0.75), color = "red", linetype = "dashed", size = 1, alpha=0.8) +
  geom_vline(xintercept = quantile(diffdfb$last, 0.25), color = "red", linetype = "dashed", size = 1, alpha=0.8) +
  theme_bw()+
  theme(text=element_text(size=11),plot.title = element_text(hjust=0.05),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1))+
  labs(title = "", y="Frequency",x="Difference in probability of EAB following arrival ")

#ggsave("")
bp

#9993 times simulated and arrived: get probability presence
library(patchwork)

rn9[is.na(rn9)] <- 0
ra9[is.na(ra9)] <- 0
newinvnoadb <- (rn9 > 0) & (ra9 == 0) #Ok, gets us the points where ADB - EAB baseline gets invaded, but no ADB scenario does not
newinvadb <-  (ra9 > 0) & (rn9 == 0) #Ok, gets us the points where no ADB scenario gets invaded but ADB - EAB baseline does not

newinvadb_df <- as.data.frame(newinvadb, xy=TRUE)
newinvnoadb_df <- as.data.frame(newinvnoadb, xy=TRUE)

newinvadb_df$last <- as.integer(newinvadb_df$last) 
newinvnoadb_df$last <- as.integer(newinvnoadb_df$last)

newinvadb_df <- newinvadb_df %>% filter(last>0) #Filters to the cells where ADB - EAB baseline gets invaded, but no ADB scenario does not
newinvnoadb_df <- newinvnoadb_df %>% filter(last>0) #Filters to the cells where no ADB scenario gets invaded but ADB - EAB baseline does not

ra9dff <- na.omit(ra9df) #Probability of invasion for cells for the no ADB scenario
ra9dff <- ra9dff %>% filter(last>0) #Gives us the cells where we get invasion in the no ADB scenario

#Save the cells where ADB - EAB baseline gets invaded, but no ADB scenario does not
#write.csv(newinvadb_df,file="Run2vs1_Run2cellswithEABRun1not.csv")

a9p <- ggplot() +
geom_tile(data=ra9dff, aes(x = x, y =y, fill = last)) +
  geom_point(data=newinvadb_df, aes(x = x, y =y),colour = "brown", size = 0.2)+
  coord_fixed() +
  scale_fill_gradient(
    low = "lightblue",
    high = "darkblue",
    limits = c(0, 0.06),breaks = c(0, 0.02, 0.04, 0.06)
  ) +   theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -7,size=10),axis.title = element_blank(),
              axis.text = element_blank(),
              axis.ticks = element_blank(),
              panel.grid = element_blank(),legend.position = "right",
              panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
              legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "Probability of EAB\nfollowing arrival", title = expression(bold("b.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4,
      barheight = 0.5,
      title.position = "top",
      title.hjust = 0.5,title.vjust = 2
    )
  )
a9p

rn9dff <- na.omit(rn9df) #Probability of invasion for Run 1 (ADB scenario)#
rn9dff <- rn9dff %>% filter(last>0) #Gives us the cells where we get invasion during Run 1 (ADB scenario)

#Save the cells where no ADB scenario gets invaded but ADB - EAB baseline does not
#write.csv(newinvnoadb_df,file="Run1_probEABcells.csv")

n9p <- ggplot() +
  geom_tile(data=rn9dff, aes(x = x, y =y, fill = last)) +
  geom_point(data=newinvnoadb_df, aes(x = x, y =y),colour = "brown", size = 0.2)+
  coord_fixed() +
  scale_fill_gradient(
    low = "lightblue",
    high = "darkblue",
    limits = c(0, 0.06),breaks = c(0, 0.02, 0.04, 0.06)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -3.5,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "Probability of EAB\nfollowing arrival", title = expression(bold("a.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4,
      barheight = 0.5,
      title.position = "top",
      title.hjust = 0.5,
      title.vjust = 2
    )
  )
n9p


combined <- n9p + a9p + plot_layout(ncol = 2) & 
  theme(legend.position = "right", plot.margin = margin(0, 0, 0, 0))

combinedlower <- d9p + d9n + plot_layout(ncol = 2) & 
  theme(legend.position = "right", plot.margin = margin(0, 0, 0, 0))

combined <- n9p + a9p + plot_layout(ncol = 2, guides = "collect") & 
  theme(legend.position = "right", plot.margin = margin(0, 0, 0, 0),
        legend.box.margin = margin(0, 0, 0, 0),
        legend.margin = margin(0, 0, 0, 0))
combinedlower <- d9p + d9n + plot_layout(ncol = 2, guides = "collect") & 
  theme(legend.position = "right", plot.margin = margin(0, 0, 0, 0),
        legend.box.margin = margin(0, 0, 0, 0),
        legend.margin = margin(0, 0, 0, 0))



ggsave("combinedRun1.tiff", plot = combined, width = 18, height = 8.5,units="cm", dpi = 300)
ggsave("combinedlowerRun1.tiff", plot = combinedlower, width = 18, height = 8.5,units="cm", dpi = 300)


library(magick)

img1 <- image_read("combinedRun1.tiff")
img2 <- image_read("combinedlowerRun1.tiff")

combined_vert <- image_append(c(img1, img2), stack = TRUE)
combined_vert

image_write(combined_vert, "Run1_Map.tiff")


##########Saving appropriate data###################

write.csv(df,file="Run1andRun2_Raw.csv") #We will use this file in "EAB ADB baseline vs no ADB_p2.R" file

#Now the summary statistics#
ndf <- df1f

ndf$numcells <- df1f$numcells-df2f$numcells
ndf$year <- ndf$year-1

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

write.csv(summary_df, "Run1andRun2_Summary.csv", row.names = FALSE)


summary_dfcompare <- ndf %>%
  group_by(year) %>%
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

write.csv(summary_dfcompare, "Run1minusRun2_diff_Summary.csv", row.names = FALSE)

write.csv(ndf, "Run1minusRun2_diff_Raw.csv", row.names = FALSE)  #We will use this file in "EAB ADB baseline vs no ADB_p2.R" file





