import random
i=0
file=open("testandofila.txt","w")

while(i<900):
	chegouClienteTempo1=random.random();
	if(chegouClienteTempo1<0.3):
		tipo = int ((random.random()*10)%3);


		sorteioAgenciaOrigem=random.random();
		if (sorteioAgenciaOrigem<0.7):
			agOrigem = 3;
		else:
			agOrigem = int (random.random()*10)%3;
		if (agOrigem==3):
			valor = int ( random.random()*1000);
		else:
			valor = int ( random.random()*500);

		contaOrigem = int (( random.random()*1000)%10);
		agDestino = int (( random.random()*10)%4);
		contaDestino = int (( random.random()*1000)%10);
		valor = int ( random.random()*1000);
		tempo = i;
		file.write(str(tipo)+":");
		file.write(str(agOrigem)+":");
		file.write(str(contaOrigem)+":");
		file.write(str(agDestino)+":");
		file.write(str(contaDestino)+":");
		file.write(str(valor)+":");
		file.write(str(tempo)+"\n");
	chegouClienteTempo2=random.random();
	if(chegouClienteTempo2<0.5)and(i%2 == 0):
		tipo = int ((random.random()*10)%3);


		sorteioAgenciaOrigem=random.random();
		if (sorteioAgenciaOrigem<0.7):
			agOrigem = 3;
		else:
			agOrigem = int (random.random()*10)%3;
		if (agOrigem==3):
			valor = int ( random.random()*1000);
		else:
			valor = int ( random.random()*500);

		contaOrigem = int (( random.random()*1000)%10);
		agDestino = int (( random.random()*10)%4);
		contaDestino = int (( random.random()*1000)%10);
		valor = int ( random.random()*1000);
		tempo = i;
		file.write(str(tipo)+":");
		file.write(str(agOrigem)+":");
		file.write(str(contaOrigem)+":");
		file.write(str(agDestino)+":");
		file.write(str(contaDestino)+":");
		file.write(str(valor)+":");
		file.write(str(tempo)+"\n");
	chegouClienteTempo3=random.random();	
	if(chegouClienteTempo3<0.20)and(i%3 == 0):
		tipo = int ((random.random()*10)%3);


		sorteioAgenciaOrigem=random.random();
		if (sorteioAgenciaOrigem<0.7):
			agOrigem = 3;
		else:
			agOrigem = int (random.random()*10)%3;
		if (agOrigem==3):
			valor = int ( random.random()*1000);
		else:
			valor = int ( random.random()*500);

		contaOrigem = int (( random.random()*1000)%10);
		agDestino = int (( random.random()*10)%4);
		contaDestino = int (( random.random()*1000)%10);
		valor = int ( random.random()*1000);
		tempo = i;
		file.write(str(tipo)+":");
		file.write(str(agOrigem)+":");
		file.write(str(contaOrigem)+":");
		file.write(str(agDestino)+":");
		file.write(str(contaDestino)+":");
		file.write(str(valor)+":");
		file.write(str(tempo)+"\n");
	i+=1;
file.close();
