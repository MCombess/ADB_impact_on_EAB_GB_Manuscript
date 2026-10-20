
#####Script estimating the q value in the ADB compartmental model based on the data in Timmermann et al., 2017 Figure 2c and 2d###

#From this data we can see (1) dominant trees (mature ash in our model) around 22% are dead and 20% are heavily stressed approximately 10 years
#after disease arrival (going with a 2006 disease arrival); (2) intermediate trees (juvenile trees in our model) around 42% are dead and 26% 
#are heavily stressed approximately 10 years after disease arrival (going with a 2006 disease arrival)



#We use the compartmental model structure defined in Figure 1 in the paper, and apply this to the above data to estimate q. q and elevated
#mortality for highly stressed vs lightly stressed trees are not separately identifiable - here we will say that highly stressed trees
#trees die at 3 times the rate of lightly stressed trees (while keeping overall mortality rate unchanged). The q value will adjust accordingly
#depending on this value and fit to the Timmermann et al., 2017 data regardless.


#Parameters we will fix from our model (representing median ADB):
beta <- 0.5 #transition of asymptomatic susceptible trees to lightly symptomatic
dead_removed <- 3 #(dead trees removed after 3 years - only include this so we can visualise impacts on our model later)
h_set <- 3.0 #elevated mortality rate multiplier for lightly stressed trees vs highly stressed trees (will adjust q so doesn't matter)
resistant <- 0.037 #(median value from Schertler et al., 2026)
obs_years <- 10L #will need to run the model for 10 years to match to Timmermann et al., 2017 data
c_juv <- 3.125 #juvenile mortality multiplier (Marcais et al., 2017)


################## ADB comaprtmental model function ###########

run_adb <- function(gamma, q, years = obs_years, resist = resistant, h = h_set){
  
  #defining starting conditions and variables
  alpha <- gamma/q
  H <- 0.99 #Healthy trees
  I <- 0.01 #lightly stressed trees
  S <- 0 #heavily stressed trees
  D <- 0 #recently dead trees (within the last 3 years)
  R <- 0 #removed trees
  
  #Now the model structure#
  for(t in seq_len(years)){
    #First, the adjusted gamma by stress category whilst keeping overall gamma the same
    den <- I + h*S
    g_I <- gamma * (I+S)/den
    g_S <- h*g_I
    
    dying <- g_I*I + g_S*S #total death
    avail_H <- max(H - resistant,0) #only susceptible trees transition in the ADB model out of asymptomatic
    
    H_new <- H-beta*avail_H
    I_new <- I+beta*avail_H-alpha*I-g_I*I
    S_new <- S+alpha*I-g_S*S
    D_new <- D+dying
    
    H <- H_new
    I <- I_new
    S <- S_new
    D <- D_new
    
  }
  c(H = H, I = I, S = S, dead = D, g_I=g_I, g_S=g_S)
  
}

########### Function fitting gamma and q values to the data (we will only take the q value across to our model) ####################

fit_q <- function(target_dead, target_S, h = h_set, resist = resistant, years = obs_years){
  
  objective <- function(p){
    X <- run_adb(p[1], p[2], years, resistant, h)
    (X["dead"] / target_dead - 1)^2 + (X["S"] / target_S - 1)^2
  }
  
  o <- optim(c(0.05,1), objective, method = "L-BFGS-B",
             lower = c(0.005,0.2), upper = c(0.3, 3))
  
  X <- run_adb(o$par[1], o$par[2], years, resistant, h)
  list(gamma = o$par[1], q = o$par[2], alpha_ratio = 1/o$par[2],
       dead=unname(X["dead"]), S = unname(X["S"]),
       g_I=unname(X["g_I"]), g_S=unname(X["g_S"]),
       err = o$value)
}


############### Fitting to data from Timmermann et al., 2017 ################

obs <- data.frame(
  dead = c(0.22,0.42),
  heavily = c(0.2, 0.26)
)

fits <- lapply(seq_len(nrow(obs)), function(i)
  fit_q(obs$dead[i],obs$heavily[i]))

fits[[1]] #Gives q value of 0.724 for mature ash
fits[[2]] #Gives q value of 0.712 for juvenile ash



