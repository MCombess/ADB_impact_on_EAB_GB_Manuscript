

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


jlmp <- ggplot() +
  geom_tile(data=jlm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),plot.title = element_text(hjust = 0.4, face = "bold", size = 10.5, margin = margin(b = -1)),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(title = "Juvenile",fill = "", subtitle = expression(bold("a.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
jlmp

mlmp <- ggplot() +
  geom_tile(data=mlm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),plot.title = element_text(hjust = 0.4, face = "bold", size = 10.5, margin = margin(b = -1)),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(title = "Mature",fill = "", subtitle = expression(bold("b.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",   
      title.hjust = 0.5
    )
  )
mlmp

almp <- ggplot() +
  geom_tile(data=alm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),plot.title = element_text(hjust = 0.3, face = "bold", size = 10.5, margin = margin(b = -1)),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(title = "All",fill = "", subtitle = expression(bold("c.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
almp

jmmp <- ggplot() +
  geom_tile(data=jmm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("d.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
jmmp

mmmp <- ggplot() +
  geom_tile(data=mmm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("e.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
mmmp

ammp <- ggplot() +
  geom_tile(data=amm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",
    high = "yellow",
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("f.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
ammp

jhmp <- ggplot() +
  geom_tile(data=jhm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",   
    high = "yellow",
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("g.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,            
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
jhmp

mhmp <- ggplot() +
  geom_tile(data=mhm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",   
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("h.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
mhmp

ahmp <- ggplot() +
  geom_tile(data=ahm, aes(x = x, y =y, fill = lyr.1)) +
  coord_fixed() +
  scale_fill_gradient(
    low = "navy",    
    high = "yellow", 
    na.value = "transparent",
    limits = c(0, 1)
  ) + theme(plot.margin = margin(0, 0, 0, 0),plot.subtitle = element_text(hjust=0,vjust = 0,size=10),axis.title = element_blank(),
            axis.text = element_blank(),
            axis.ticks = element_blank(),
            panel.grid = element_blank(),
            panel.background = element_rect(fill = NA, color = NA),legend.position = "none",legend.title = element_text(size = 8.5),
            legend.text = element_text(size = 7,angle = 45, hjust = 1))+
  labs(fill = "", subtitle = expression(bold("i.")))+
  guides(
    fill = guide_colorbar(
      direction = "horizontal",  
      barwidth = 4,             
      barheight = 0.5,
      title.position = "top",    
      title.hjust = 0.5
    )
  )
ahmp



# Add row labels as axis titles on leftmost plot
jlmp <- jlmp + ylab("Low ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))
jmmp <- jmmp + ylab("Median ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))
jhmp <- jhmp + ylab("High ADB") + theme(axis.title.y = element_text(face = "bold", size = 10, angle = 90, hjust = 0.5,vjust=4))

combined <- jlmp+mlmp+almp+jmmp+
  mmmp+ammp+jhmp+mhmp+ahmp+plot_layout(ncol = 3,guides="collect")+theme(legend.position = "right",
                                                                        plot.margin = margin(0,0,0,0))

combined

#ggsave("") save plot


