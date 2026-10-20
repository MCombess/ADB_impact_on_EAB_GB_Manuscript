

#Processing Output files from the model, having already generated files in "EAB ADB baseline vs no ADB_p1.R" file#

rm(list=ls())

library(dplyr)
library(ggplot2)

df <- read.csv("Run1andRun2_Raw.csv") #generated file in "EAB ADB baseline vs no ADB_p1.R"


eabplotarrival <- ggplot(df, aes(x = factor(year-1), y = numcells, fill=Scenario)) +
  theme_bw()+
  geom_boxplot(outlier.shape = NA, coef = 10,alpha=0.3)+
  geom_point(aes(color = Scenario), 
             position = position_dodge(width = 0.75),
             size = 0.8, alpha = 0.5)+
  scale_y_continuous(breaks = seq(0, 1750, by = 100))+
  labs(x = "Years after EAB arrival",
       y = expression("1 km"^2*" cells with EAB"), title = expression(bold("a."))) +
  scale_fill_discrete(
    labels = c(
      "Most likely ADB interaction" = "Baseline\nADB\nimpact",
      "No ADB" = "No ADB"
    )
  ) +
  scale_color_discrete(
    labels = c(
      "Most likely ADB interaction" = "Baseline\nADB\nimpact",
      "No ADB" = "No ADB"
    )
  ) +
  theme(text = element_text(size = 10),legend.title = element_blank(),plot.title = element_text(hjust=-0.05,vjust=2))

eabplotarrival


ndf <- read.csv("Run1minusRun2_diff_Raw.csv") #generated file in "EAB ADB baseline vs no ADB_p1.R"

eabplotarrivaldiff <- ggplot(ndf, aes(x = factor(year), y = numcells)) +
  theme_bw()+
  geom_boxplot(outlier.shape = NA, coef = 10, fill="darkgreen",alpha=0.3)+  geom_point(color = "darkgreen", 
                                                                                       position = position_dodge(width = 0.75),
                                                                                       size = 0.8, alpha = 0.5)+
  scale_y_continuous(breaks = seq(-1000, 500, by = 100))+
  labs(x = "Years after EAB arrival",
       y = expression("Difference in 1 km"^2*" cells colonised by EAB"), title = expression(bold("b."))) +
  theme(text = element_text(size = 10),legend.title = element_blank(),plot.title = element_text(hjust=-0.05,vjust=2))

eabplotarrivaldiff

#########################Now the bottom panels of the plot with the relationship with number of ash in cells and in arrival location#

#Read in the output file from C++
#Most likely interacation scenario (baseline ADB impact on EAB)#
Run1 <- read.table("DataForOptJuneRun1.txt")

#No ADB#
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

totally <- 1

df1 <- cbind(Run1,totally)
df2 <- cbind(Run2, totally)

rm(Run1,Run2)
rm(df2)
####################################################

#Number of ash at each colonized event#
df1$ashcoly0 <- ifelse(df1$V8>0.0,df1$V5,0) #If EAB in the cell, how many ash (0y after first GB EAB arrival)
df1$ashcoly1 <- ifelse(df1$V8>0.0,df1$V17,0) #If EAB in the cell, how many ash (0y after first GB EAB arrival)
df1$ashcoly2 <- ifelse(df1$V9>0.0,df1$V18,0) #If EAB in the cell, how many ash (1y after first GB EAB arrival)
df1$ashcoly3 <- ifelse(df1$V10>0.0,df1$V19,0) #If EAB in the cell, how many ash (2y after first GB EAB arrival)
df1$ashcoly4 <- ifelse(df1$V11>0.0,df1$V20,0) #If EAB in the cell, how many ash (3y after first GB EAB arrival)
df1$ashcoly5 <- ifelse(df1$V12>0.0,df1$V21,0) #If EAB in the cell, how many ash (4y after first GB EAB arrival)
df1$ashcoly6 <- ifelse(df1$V13>0.0,df1$V22,0) #If EAB in the cell, how many ash (5y after first GB EAB arrival)
df1$ashcoly7 <- ifelse(df1$V14>0.0,df1$V23,0) #If EAB in the cell, how many ash (6y after first GB EAB arrival)
df1$ashcoly8 <- ifelse(df1$V15>0.0,df1$V24,0) #If EAB in the cell, how many ash (7y after first GB EAB arrival)

#Frequency - so we can tally how many cells EAB is in
df1$ashcoln1 <- ifelse(df1$V8>0.0,1,0) #if EAB present(0y after first GB EAB arrival) ashcoln1 = 1
df1$ashcoln2 <- ifelse(df1$V9>0.0,1,0) #if EAB present(1y after first GB EAB arrival) ashcoln2 = 1
df1$ashcoln3 <- ifelse(df1$V10>0.0,1,0) #if EAB present(2y after first GB EAB arrival) ashcoln3 = 1
df1$ashcoln4 <- ifelse(df1$V11>0.0,1,0) #if EAB present(3y after first GB EAB arrival) ashcoln4 = 1
df1$ashcoln5 <- ifelse(df1$V12>0.0,1,0) #if EAB present(4y after first GB EAB arrival) ashcoln5 = 1
df1$ashcoln6 <- ifelse(df1$V13>0.0,1,0) #if EAB present(5y after first GB EAB arrival) ashcoln6 = 1
df1$ashcoln7 <- ifelse(df1$V14>0.0,1,0) #if EAB present(6y after first GB EAB arrival) ashcoln7 = 1
df1$ashcoln8 <- ifelse(df1$V15>0.0,1,0) #if EAB present(7y after first GB EAB arrival) ashcoln8 = 1

#Now the ash loss compared to time point 0 (we don't use this in the paper)#
df1$lossy1 <- ifelse(df1$ashcoly1>0,df1$V5-df1$ashcoly1,0) #if EAB in the cell (0y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy2 <- ifelse(df1$ashcoly2>0,df1$V5-df1$ashcoly2,0) #if EAB in the cell (1y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy3 <- ifelse(df1$ashcoly3>0,df1$V5-df1$ashcoly3,0) #if EAB in the cell (2y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy4 <- ifelse(df1$ashcoly4>0,df1$V5-df1$ashcoly4,0) #if EAB in the cell (3y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy5 <- ifelse(df1$ashcoly5>0,df1$V5-df1$ashcoly5,0) #if EAB in the cell (4y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy6 <- ifelse(df1$ashcoly6>0,df1$V5-df1$ashcoly6,0) #if EAB in the cell (5y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy7 <- ifelse(df1$ashcoly7>0,df1$V5-df1$ashcoly7,0) #if EAB in the cell (6y after first GB EAB arrival) calculate ash loss at this timepoint
df1$lossy8 <- ifelse(df1$ashcoly8>0,df1$V5-df1$ashcoly8,0) #if EAB in the cell (7y after first GB EAB arrival) calculate ash loss at this timepoint


#Calculating ash loss at EAB arrival in cells when EAB arrives in them due to ADB (we don't use this in the paper)
df1$fl <- ifelse(df1$V8>0,(df1$V5-df1$V17)/df1$V5,
                 ifelse(df1$V9>0,(df1$V5-df1$V18)/df1$V5,
                        ifelse(df1$V10>0,(df1$V5-df1$V19)/df1$V5,
                               ifelse(df1$V11>0,(df1$V5-df1$V20)/df1$V5,
                                      ifelse(df1$V12>0,(df1$V5-df1$V21)/df1$V5,
                                             ifelse(df1$V13>0,(df1$V5-df1$V22)/df1$V5,
                                                    ifelse(df1$V14>0,(df1$V5-df1$V23)/df1$V5,
                                                           ifelse(df1$V15>0,(df1$V5-df1$V24)/df1$V5,NA))))))))


#But we will look at this for the arrival location: calculate the proportion of
#ash lost due to ADB where EAB lands in year 0
df1$fln <- ifelse(df1$V8>0,df1$V5-df1$V17,0)
df1$fln <- df1$fln/df1$V5

#Number of ash in the arrival cell
df1$flnb <- ifelse(df1$V8>0,df1$V17,0)

#Get the numbers of cells colonised by EAB:
df1 <- df1 %>% group_by(V1) %>% mutate(numcells=ifelse(V8>0| V9>0 | V10>0| V11>0 | V12>0 |
                                                         V13>0 | V14>0 | V15>0 | V16>0,sum(totally),0)) %>% ungroup()


#For each realisation, sum the ash an EAB has been has been present with,
#and then dividing by the number of cells (avash) - so it is: total ash/ total number of cells (i.e. average number of ash in cells EAB is present between y0-y7)
#also use sfl_ash (the number of ash in the arrival location). Would need to contruct model accounting for number of ash to see direct impact of removing ash
#but a positive relationship between number of ash and EAB spread tells us what impact reducing ash number (via removing ash) would have.

df1 <- df1 %>% group_by(V1) %>% mutate(avash = (sum(ashcoly1)+sum(ashcoly2)+sum(ashcoly3)+sum(ashcoly4)+
                                                  sum(ashcoly5)+sum(ashcoly6)+sum(ashcoly7)+sum(ashcoly8))/(sum(ashcoln1)+sum(ashcoln2)+sum(ashcoln3)+sum(ashcoln4)+
                                                                                                              sum(ashcoln5)+sum(ashcoln6)+sum(ashcoln7)+sum(ashcoln8)),
                                       avloss=mean(fl,na.rm=TRUE),
                                       sfl=sum(fln), sfl_ash=sum(flnb), 
                                       asharrival=sum(ashcoly0)) %>% ungroup()

#Filter the desired columns and then remove duplicates:
colnames(df1)
df1c <- df1[,c(1,91:96)]
df1c <- df1c[!duplicated(df1c), ]

names(df1c)[1] <- "Realisation"

#Plot of average ash number per cell on EAB spread in the 8th year after arrival
library(ggplot2)
pb <- ggplot(df1c, aes(x = avash, y = numcells)) +
  geom_point(color = "#F8766D", alpha=0.3,size=0.8) +
  labs(y=expression("1 km"^2*" cells with EAB"),x= expression("Average number of ash in cells (1 km"^2*") with EAB"), title = expression(bold("c."))) +
  theme_bw()+theme(text = element_text(size = 10),plot.title = element_text(hjust=-0.05,vjust=1.5))
pb 

#Plot of the number of ash in the arrival cell on EAB spread in the 8th year after arrival
library(ggplot2)
py0 <- ggplot(df1c, aes(y = numcells, x = sfl_ash)) +
  geom_point(color = "#F8766D", alpha=0.3,size=0.8) +
  labs(y=expression("1 km"^2*" cells with EAB"),x = expression("Number of ash in EAB arrival cell (1 km"^2*")"), title = expression(bold("d."))) +
  theme_bw()+theme(text = element_text(size = 10),plot.title = element_text(hjust=-0.05,vjust=2))
py0



#Save these dataframes so I can just replot#
write.csv(df1c, file="Fig4c_data.csv")

library(patchwork)
library(cowplot)
combined <- eabplotarrival + eabplotarrivaldiff +pb+py0+plot_layout(ncol = 2)+theme(
  plot.margin = margin(0,0,0,0))
combined

ggsave("Fig4_141026.tiff", plot = combined, width = 18, height = 16,units="cm", dpi = 300)








