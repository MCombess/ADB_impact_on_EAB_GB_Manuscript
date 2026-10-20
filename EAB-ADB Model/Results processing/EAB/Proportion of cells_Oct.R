
#Examining differences in cells colonised by EAB for each scneario output#

#See Table 1 in paper for full description.
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


#205423 cells containing ash#

#This is for the baseline ADB scenario#

48755/205423 #An increase in probability of EAB colonisation for 0.2373395 cells with ash (EAB-ADB Baseline vs No ADB model)

115836/205423 #A decrease in probability of EAB colonisation for 0.5638901 cells with ash (EAB-ADB Baseline vs No ADB model)

a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(190387/205423) #0.07319531 of cells not invaded by EAB during no ADB model
b
#Now for the other scenarios#

#####################   Run 6    #####################################

114765/205423 #An increase in probability of EAB colonisation for 0.5586765 cells with ash (Run 6 vs EAB-ADB Baseline)

47704/205423 #A decrease in probability of EAB colonisation for 0.2322233 cells with ash (Run 6 vs EAB-ADB Baseline)


a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(187404/205423) #0.08771657 of cells not invaded by EAB during Run 6
b
#####################   Run 7    #####################################

152622/205423 #An increase in probability of EAB colonisation for 0.7429645 cells with ash (Run 7 vs EAB-ADB Baseline)

17703/205423 #A decrease in probability of EAB colonisation for 0.08617828 cells with ash (Run 7 vs EAB-ADB Baseline)


a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(189877/205423) #0.07567799 of cells not invaded by EAB during Run 7
b

#####################   Run 4    #####################################

83812/205423 #An increase in probability of EAB colonisation for 0.4079972 cells with ash (Run 4 vs EAB-ADB Baseline)

75801/205423 #A decrease in probability of EAB colonisation for 0.3689996 cells with ash (Run 4 vs EAB-ADB Baseline)


a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(185984/205423) #0.09462913 of cells not invaded by EAB during Run 4
b

#####################   Run 3    #####################################


81132/205423 #An increase in probability of EAB colonisation for 0.3949509 cells with ash (Run 3 vs EAB-ADB Baseline)

78495/205423 #A decrease in probability of EAB colonisation for 0.382114 cells with ash (Run 3 vs EAB-ADB Baseline)


a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(185967/205423) #0.09471189 of cells not invaded by EAB during Run 3
a
b
#####################   Run 5    #####################################


90807/205423 #An increase in probability of EAB colonisation for 0.4420488 cells with ash (Run 5 vs EAB-ADB Baseline)

69333/205423 #A decreas in probability of EAB colonisation for 0.3375133 cells with ash (Run 5 vs EAB-ADB Baseline)

a
a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(186622/205423) #0.09152334 of cells not invaded by EAB during Run 5
b

#####################   Run 9    #####################################

63134/205423 #An increase in probability of EAB colonisation for 0.3073366 cells with ash (Run 9 vs EAB-ADB Baseline)

95386/205423 #A decrease in probability of EAB colonisation for 0.4643394 cells with ash (Run 9 vs EAB-ADB Baseline)

a
a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(183436/205423) # 0.1070328 of cells not invaded by EAB during Run 9
b

#####################   Run 11    #####################################

102335/205423 #An increase in probability of EAB colonisation for 0.4981672 cells with ash (Run 11 vs EAB-ADB Baseline)

59620/205423 #A decrease in probability of EAB colonisation for 0.2902304 cells with ash (Run 11 vs EAB-ADB Baseline)

a
a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(188251/205423) # 0.08359337 of cells not invaded by EAB during Run 11
b


#####################   Run 12    #####################################

53185/205423 #An increase in probability of EAB colonisation for 0.2589048 cells with ash (Run 12 vs EAB-ADB Baseline)

104816/205423 #A decrease in probability of EAB colonisation for 0.5102447 cells with ash (Run 12 vs EAB-ADB Baseline)

a
a <- 1-(185661/205423) #0.0962015 of cells not invaded by EAB during EAB-ADB Baseline
b <- 1-(182439/205423) # 0.1118862 of cells not invaded by EAB during Run 12
b


