grow <- 2
start <- 1
for(i in 4:10){
	n <- ceiling(start*grow^i)
	seed <- 0
	for(d in c(3,6,round(sqrt(n)))){
		for(s in 1:3){
			seed <- seed+1
			fn1  <- paste('tests/n',n,'d',d,'s',seed,'.txt',sep='')
			fn2  <- paste('dots/n',n,'d',d,'s',seed,'.dot',sep='')
			args <- paste(n,d,seed)
			cmd1 <- paste("./i386/rndgraph",      args,">",fn1)
			cmd2 <- paste("./i386/rndgraph --dot",args,">",fn2)
			print(cmd1)
			system(cmd1)
			if(n<100){print(cmd2);system(cmd2)}
		}
	}	
}