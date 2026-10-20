
rm(list = ls())

library(dplyr)
library(ggplot2)
library(terra)
library(patchwork)

#Read in our reference ash density map, and remove the NA values, need to do this with the
#arrival map because there are some places in Scotland that have no ash in any of the categories
#because the arrival map doesn't align

#Keep cells with ash
pa <- as.matrix(read.table("treeDensity_GB.txt"))
rpa <- rast(pa)
rpa[rpa == -3.3 | rpa == 0] <- NA
plot(rpa)

#Align with ash layer  above
arrival <- as.matrix(read.table("arrival_GB3011.txt"))
rarrival <- rast(arrival)
rarrival[rarrival == -9999] <- NA
plot(rarrival)
newarr <- mask(rarrival,rpa)
plot(newarr)

#Read in the start and end data so we can mask where no ash are left#
Run1 <- as.matrix(read.table("ashstart0.txt"))
Run2 <- as.matrix(read.table("ashend0.txt"))

rashstart <- rast(Run1)
rashend <- rast(Run2)

rashstart[rashstart == -2933.7] <- NA
rashend[rashend == -2933.7] <- NA

start_masked <- mask(rashstart, newarr)
end_masked <- mask(rashend, newarr)

#Ok, now, read in the data from the ash dieback layers (change file names accordingly as desired)#
ashH <- as.matrix(read.table("adbTreeH0.txt"))
ashI <- as.matrix(read.table("adbTreeI0.txt"))
ashS <- as.matrix(read.table("adbTreeS0.txt"))
ashD <- as.matrix(read.table("adbTreeD0.txt"))
#ashR <- as.matrix(read.table("adbTreeMatR0.txt")) #Add for the mature/ juvenile

#Align with the reference raster layer:
rashH <- rast(ashH)
rashI <- rast(ashI)
rashS <- rast(ashS)
rashD <- rast(ashD)
#rashR <- rast(ashR) #Add for the mature and juvenile
end_zero <- start_masked
end_zero[end_zero == 0] <- NA

rashH_masked <- mask(rashH, end_zero)
rashI_masked <- mask(rashI, end_zero)
rashS_masked <- mask(rashS, end_zero)
rashD_masked <- mask(rashD, end_zero)
#rashR_masked <- mask(rashR, end_zero) #Add for the mature and juvenile


#Rdf <- as.data.frame(rashR_masked, xy = TRUE, na.rm = TRUE) #Add below for removed ash for juvenile and mature ash rather than below code
#Rdfb <- na.omit(Rdf) #Add below for removed ash for juvenile and mature ash rather than below code

ashdiff <- (rashstart - rashend)/rashstart #Calculating the proportion of removed ash (done with a separate removed file when doing for juvenile and mature ash)
ashdiff <- mask(ashdiff, end_zero) #Removing cells where we have no ash starting
plot(ashdiff)
Fdf <- as.data.frame(ashdiff, xy = TRUE, na.rm = TRUE)
Rdfb <- na.omit(Fdf)

#Save the mortality rds file to use in the R files: "ADB_mortality_histograms.R" and "ADB_mortality_maps.R"
#saveRDS(Rdfb, file = "")

ts <- c("min","5%","25%","50%","75%","95%","max")
sRdfb <- quantile(Rdfb$lyr.1, c(0, 0.05,0.25,0.5,0.75,0.95,1), na.rm = TRUE)
sRdfb <- as.data.frame(cbind(ts,sRdfb))
sRdfb[, 2] <- as.numeric(sRdfb[, 2])
write.csv(sRdfb, file="All_ash_now_High_Rstats.csv") #Change file name as appropriate for the summary statistics



#Check ok
plot(rashH_masked)
plot(rashI_masked)
plot(rashS_masked)
plot(rashD_masked)

#Fine, now plot a nice map of this, with four facets for each category:
Hdf <- as.data.frame(rashH_masked, xy = TRUE, na.rm = TRUE)
Hp <- ggplot() +
  geom_tile(data=Hdf, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",
    high = "yellow",
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -4,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position = "right")+
  labs(fill = "Proportion of ash", title = expression(bold("a.")))+
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
Hp


Idf <- as.data.frame(rashI_masked, xy = TRUE, na.rm = TRUE)
Ip <- ggplot() +
  geom_tile(data=Idf, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -7.5,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position = "right")+
  labs(fill = "Proportion of ash", title = expression(bold("b.")))+
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
Ip


Sdf <- as.data.frame(rashS_masked, xy = TRUE, na.rm = TRUE)
Sp <- ggplot() +
  geom_tile(data=Sdf, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -4,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position = "right")+
  labs(fill = "Proportion of ash", title = expression(bold("c.")))+
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
Sp

Ddf <- as.data.frame(rashD_masked, xy = TRUE, na.rm = TRUE)
Dp <- ggplot() +
  geom_tile(data=Ddf, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1),
    breaks = seq(0, 1, by = 0.25)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.title = element_text(hjust=0.05,vjust = -7.5,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1),legend.position = "right")+
  labs(fill = "Proportion of ash", title = expression(bold("d.")))+
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
Dp


combined <- Hp + Ip+Sp+Dp +plot_layout(ncol = 2,guides="collect")+theme(legend.position = "right",
                                                                        plot.margin = margin(0,0,0,0))

combined


#ggsave("") Save plot

##################Ok, the maps are done, now onto the histograms, and then save the summary data###################

#First, bind data with named columns#
Hdfb <- na.omit(Hdf)
Hp <- ggplot(Hdfb, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(Hdfb$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Hdfb$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Hdfb$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.title = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = expression(bold("a.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
Hp


Idfb <- na.omit(Idf)
Ip <- ggplot(Idfb, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(Idfb$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Idfb$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Idfb$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.title = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = expression(bold("b.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
Ip

Sdfb <- na.omit(Sdf)
Sp <- ggplot(Sdfb, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(Sdfb$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Sdfb$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Sdfb$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.title = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = expression(bold("c.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
Sp

Ddfb <- na.omit(Ddf)
Dp <- ggplot(Ddfb, aes(x = lyr.1)) +
  geom_histogram(bins = 1000, fill = "black", color = "black", drop = TRUE) +
  geom_vline(xintercept = quantile(Ddfb$lyr.1, 0.95, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Ddfb$lyr.1, 0.05, na.rm = TRUE), linetype = "dashed", color = "red", size = 1)+
  geom_vline(xintercept = quantile(Ddfb$lyr.1, 0.5, na.rm = TRUE), linetype = "solid", color = "red", size = 1)+
  theme_bw()+
  theme(axis.title.x = element_text(size = 9.5),plot.title = element_text(hjust=0,vjust = -1,margin = margin(b = 10),size=10),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10,angle = 45, hjust = 1)
  )+
  labs(title = expression(bold("d.")), y=expression(""),x="Proportion of ash")+
  scale_x_continuous(limits = c(0, 1), breaks = seq(0, 1, 0.2))+
  scale_y_continuous(labels = NULL)
Dp

#Ok, now combine the plots#

combined <- Hp + Ip +Sp+Dp+plot_layout(ncol = 2)+theme(
  plot.margin = margin(0,0,0,0))
combined

#ggsave("") Save plots



#Ok, now I want to save the RDS files and the summary statistics#

#Change file names accordingly
saveRDS(Hdfb, file = "All_ash_now_High_H.rds")
saveRDS(Idfb, file = "All_ash_now_High_I.rds")
saveRDS(Sdfb, file = "All_ash_now_High_S.rds")
saveRDS(Ddfb, file = "All_ash_now_High_D.rds")

#Now the summary statistics:
ts <- c("min","5%","25%","50%","75%","95%","max")

sHdfb <- quantile(Hdfb$lyr.1, c(0, 0.05,0.25,0.5,0.75,0.95,1), na.rm = TRUE)
sHdfb <- as.data.frame(cbind(ts,sHdfb))
sHdfb[, 2] <- as.numeric(sHdfb[, 2])

sIdfb <- quantile(Idfb$lyr.1, c(0, 0.05,0.25,0.5,0.75,0.95,1), na.rm = TRUE)
sIdfb <- as.data.frame(cbind(ts,sIdfb))
sIdfb[, 2] <- as.numeric(sIdfb[, 2])

sSdfb <- quantile(Sdfb$lyr.1, c(0, 0.05,0.25,0.5,0.75,0.95,1), na.rm = TRUE)
sSdfb <- as.data.frame(cbind(ts,sSdfb))
sSdfb[, 2] <- as.numeric(sSdfb[, 2])

sDdfb <- quantile(Ddfb$lyr.1, c(0, 0.05,0.25,0.5,0.75,0.95,1), na.rm = TRUE)
sDdfb <- as.data.frame(cbind(ts,sDdfb))
sDdfb[, 2] <- as.numeric(sDdfb[, 2])

#Change file names accordingly
write.csv(sHdfb, file="All_ash_now_High_Hstats.csv") 
write.csv(sIdfb, file="All_ash_now_High_Istats.csv")
write.csv(sSdfb, file="All_ash_now_High_Sstats.csv")
write.csv(sDdfb, file="All_ash_now_High_Dstats.csv")






