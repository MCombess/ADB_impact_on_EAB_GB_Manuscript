
#Examining impact of adjusted parameters in the model examining impacts of ADB on EAB (see table 2 in paper)
#Simply change input file for different model runs to examine different model parameters 
#this script uses larval mortality scenario 1 (Run 6) run as an example

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

#Baseline ADB impact on EAB#
Run1 <- read.table("DataForOptJuneRun1.txt")

#Larval mortality scenario 1#
Run3 <- read.table("DataForOptJuneRun6.txt")

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
df2 <- Run3

rm(Run1,Run3)

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

df1y9$V6 <- as.numeric(df1y9$V6)
df1y9$V7 <- as.numeric(df1y9$V7)
df1y9$numcells <- df1y9$numcells/9993 #Probability cell invaded during 9993 runs
ymin <- ext(rpa)[3]
ymax <- ext(rpa)[4]
df1y9$V6b <- ymax - (df1y9$V6 - ymin)
v <- vect(df1y9, geom=c("V7","V6b"))

ra9 <- rasterize(v, rpa, field="numcells")
ra9 <- mask(ra9,rpa,maskvalue=-3.3,updatevalue=NA) #Remove the cells without anything
ra9 <- mask(ra9,rpa,maskvalue=0,updatevalue=NA) #Remove the cells with no ash


df2y9$V6 <- as.numeric(df2y9$V6)
df2y9$V7 <- as.numeric(df2y9$V7)
df2y9$numcells <- df2y9$numcells/9993 #Probability cell invaded during 9993 runs
ymin <- ext(rpa)[3]
ymax <- ext(rpa)[4]
df2y9$V6b <- ymax - (df2y9$V6 - ymin)
rm(v)
v <- vect(df2y9, geom=c("V7","V6b"))

rn9 <- rasterize(v, rpa, field="numcells")
rn9 <- mask(rn9,rpa,maskvalue=-3.3,updatevalue=NA) #Remove the cells without anything
rn9 <- mask(rn9,rpa,maskvalue=0,updatevalue=NA) #Remove the cells with no ash

newinvnoadb <- (rn9 > 0) #Ok, gets us the points where Run 6 gets invaded (TRUE/FALSE)
newinvadb <-  (ra9 > 0) #Ok, gets us the points where ADB-EAB scenario gets invaded (TRUE/FALSE)

newinvadb_df <- as.data.frame(newinvadb, xy=TRUE)
newinvnoadb_df <- as.data.frame(newinvnoadb, xy=TRUE)

newinvadb_df$last <- as.integer(newinvadb_df$last) 
newinvnoadb_df$last <- as.integer(newinvnoadb_df$last)

newinvadb_df <- newinvadb_df %>% filter(last>0) #Filters to the cells where basleline EAB_ADB scenario actually gets invaded: 
newinvnoadb_df <- newinvnoadb_df %>% filter(last>0) #Filters to the cells where Run 6 actually gets invaded: 

rn9df <- as.data.frame(rn9, xy=TRUE, na.rm = FALSE) %>% mutate(raster = "No ADB") #Making a dataframe for probability of invasion for Run 6 cells#
ra9df <- as.data.frame(ra9, xy=TRUE, na.rm = FALSE) %>% mutate(raster = "ADB") #Making a dataframe for probability of invasion for baseline EAB_ADB scenario cells#

diffdf <- ra9df
diffdf$last <- rn9df$last-ra9df$last #Probability of invasion for Run 6 minus probability of invasion in the baseline EAB_ADB scenario

#diffdfb <- na.omit(diffdf)

diffdfbpos <- diffdf %>% filter(last>0) ##Number of cells for Run 6 where invasion probability is increased compared to baseline EAB_ADB scenario: 
diffdfbneg <- diffdf %>% filter(last<0) ##Number of cells for Run 6 where invasion probability is decreased compared to baseline EAB_ADB scenario: 
max(diffdfbpos$last)
min(diffdfbpos$last)

#write.csv(diffdfbpos,file="Pos_diff_EAB_ADB_survival_Run6.csv")

d9p <- ggplot() +
  geom_tile(data=diffdfbpos, aes(x = x, y =y, fill = last)) +
  coord_fixed(xlim = c(37.5, 600.5), ylim = c(3.5, 947.5))+ # keeps aspect ratio square
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

#write.csv(diffdfbneg,file="Neg_diff_EAB_ADB_survival_Run6.csv")

min(diffdfbneg$last)
d9n <- ggplot() +
  geom_tile(data=diffdfbneg, aes(x = x, y =y, fill = last)) +
  coord_fixed(xlim = c(37.5, 600.5), ylim = c(3.5, 947.5)) +# keeps aspect ratio square
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


#write.csv(diffdfb,file="All_diff_EAB_ADB_survival_Run6.csv")

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
newinvnoadb <- (rn9 > 0) & (ra9 == 0) #Ok, gets us the points where Run 6 gets invaded, but baseline EAB_ADB scenario does not
newinvadb <-  (ra9 > 0) & (rn9 == 0) #Ok, gets us the points where baseline EAB_ADB scenario gets invaded but Run 6 does not

newinvadb_df <- as.data.frame(newinvadb, xy=TRUE)
newinvnoadb_df <- as.data.frame(newinvnoadb, xy=TRUE)

newinvadb_df$last <- as.integer(newinvadb_df$last) 
newinvnoadb_df$last <- as.integer(newinvnoadb_df$last)

newinvadb_df <- newinvadb_df %>% filter(last>0) #Filters to the cells where baseline EAB_ADB scenario actually gets invaded, but Run 6 doesn't
newinvnoadb_df <- newinvnoadb_df %>% filter(last>0) #Filters to the cells where Run 6 actually gets invaded, but baseline EAB_ADB doesn't

ra9dff <- na.omit(ra9df) #Probability of invasion for the baseline EAB_ADB scenario cells#
ra9dff <- ra9dff %>% filter(last>0) #Gives us the cells where we get invasion#

#Save the cells where new invasion occurs under baseline EAB_ADB scenario, but not during Run 6#
#write.csv(newinvadb_df,file="Run1vs6_Run1cellswithEABRun6not.csv")


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



rn9dff <- na.omit(rn9df) #Probability of invasion for Run 6#
rn9dff <- rn9dff %>% filter(last>0) #Gives us the cells where we get invasion during Run 6

#Save the cells where we get invasion during Run 6, but not the baseline EAB_ADB scenario#
#write.csv(newinvnoadb_df,file="Run6_probEABcells.csv")

#Save the probability of invasion for Run 6 cells#
#write.csv(rn9dff,file="Run1vs6_Run6cellswithEABRun1not.csv")

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



ggsave("combinedRun16.tiff", plot = combined, width = 18, height = 8.5,units="cm", dpi = 300)
ggsave("combinedlowerRun16.tiff", plot = combinedlower, width = 18, height = 8.5,units="cm", dpi = 300)


library(magick)

img1 <- image_read("combinedRun16.tiff")
img2 <- image_read("combinedlowerRun16.tiff")

combined_vert <- image_append(c(img1, img2), stack = TRUE)
combined_vert

image_write(combined_vert, "Run1vs6_survival_Map.tiff")

