/*
PROJETO FINAL SI200A
GRUPO 05
Andrey Toshiro Okamura 213119
Max ...
Gustavo...
bom noitarde
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>


//codigo de disciplina apenas maiusculo. ex.: SI100





/*
ESTRUTURAS
*/
typedef struct disciplina{//formato "EB101,C�lculo I,6", "codigo,nome,creditos"

	char codigo[7];
	char nome[100];
	int creditos;
} Disciplina;

typedef struct prerequisito{ //formato "SI200,SI100", "cod disciplina,cod prerequisito"
	
	char codDisc[7];
	char codPrereq[7];
} Prereq;

typedef struct aluno{ //formato "111111,Andrey,andrey,andrey", "ra,nome,login,senha"

	int ra;
	char nome[100];
	char login[10];	//obs.: para padronizacao, definimos o maximo para 10 (pode ser mudado a qualquer hora)
	char senha[10];
} Aluno;

typedef struct vetprereq{
	char codigo[5];
} Vetprereq;

typedef struct alunodisc{//RA,C�digodaDisciplina,Semestre,Nota,Faltas

	int ra;
	char codigo[7];
	int semestre;
	int nota;
	float faltas;
}Alunodisc;


//char vetprereq[43][5];
/*
PROTOTIPOS
*/
Disciplina* leDisc();//le  todas as disciplinas
Disciplina* leDisc2();
Prereq* lePrereq();	//le  todos os pre requisitos
Aluno* leAluno();	//le todos os alunos
Alunodisc* leAlunodisc();
void newAluno();	//cria novo aluno; pede os dados e escreve no arquivo (append para apenas adicionar no final)//ou pode dar realloc, e salva ao sair
void saveAluno(Aluno novo);	//funcao usada imediatamente apos newAluno, libera a antiga memoria de Alunos e cria uma struct atualizada(?.. solucao imediata, ha possiveis otimizacoes) por causa do malloc

int login(Aluno *user);

void consultaDisc();
void exibePrereq(Disciplina d);
void matricula();
void saveMatricula(Alunodisc * novo,int contsave);
void alteraMatricula();

/*
VARIAVEIS GLOBAIS
*/

int topD=31;//31, numero total de disciplinas; o arquivo tem um erro no comeco, apresentando uns caracteres estranhos
int topP=0;//43, o mesmo que anterior, arquivo apenas para leitura! sera lido o top na primeira linha
int topAD=0;//vai ser atualizado quando entrar no leAlunoDisc
int topA=0;//igual em cima, para leAluno

	Disciplina * d;
	Disciplina * d2;
	Prereq * p;
	Aluno * a;
	Alunodisc * ad;

int logra;
char lognome[100];//dados salvos ao fazer login


int main(){
	int i;
	int op;

	d=leDisc();
	p=lePrereq();
	a=leAluno();
	ad=leAlunodisc();


    int erro; //variável que identifica se tudo ocorreu bem em funções que forem chamadas

    do{ //laço que repete o login até o usuário entrar com as credenciais corretas
        erro = login(a); //chamando a função de login
    }while(erro != 0);
    printf("login realizado com sucesso\n");
	do{
		printf("1: Cadastro de Alunos\n");
		printf("2: Consulta de Disciplinas\n");
		printf("3: Realizar Matricula\n");
		printf("4: Atualizar Nota e Falta\n");
	
		printf("0: Sair\n");
		scanf("%d",&op);
		
	
		switch(op){
		
			case 1:
				newAluno();
			break;
			
			case 2:
				consultaDisc();
			break;
			
			case 3:
				matricula();
			break;
			
			case 4:
				alteraMatricula();
			break;
			
			case 0:
				printf("Saindo .. \n");
			break;
			
			default:
			break;	
		}
	
	
	}while(op!=0);

	return 0;
}


/*
FUNCOES
*/
Disciplina* leDisc2(){//nao utilizada
	FILE* fp = fopen("Disciplina.txt","r");
	
	int top, i;

	Disciplina * aux;
	aux = (Disciplina*)malloc(sizeof(Disciplina)*topD);
	for(i=0;i<topD;i++)
	{
		fscanf(fp, "%[^,], %[^,], %d", aux[i].codigo, aux[i].nome, &aux[i].creditos);
	}
	fclose(fp);
	return aux;
}

Disciplina* leDisc(){
	FILE* fp = fopen("Disciplinas.txt","r");
	char aseparar[100];
	int top, i;
	char *cod, *nom, *cred; //codigo, nome, credito temporario
	char *saveptr;
	/*
	fgets(aseparar,100,fp);
	top=atoi(aseparar);
	printf("%s\n",aseparar);
	printf("%d\n",topD);
	topD=top;*/ //nao [e possivel, pois existem caracteres estranhos no arquivo
	Disciplina * aux;
	aux = (Disciplina*)malloc(sizeof(Disciplina)*topD);
	
	for(i=0;i<topD;i++)
	{
		fgets(aseparar,100,fp);

		cod = strtok_r(aseparar, ",", &saveptr);
		nom = strtok_r(NULL, ",", &saveptr);
		cred = strtok_r(NULL, "\n", &saveptr);

		fflush(stdin);
		
		strcpy(aux[i].codigo,cod);
		strcpy(aux[i].nome,nom);
		aux[i].creditos=atoi(cred);

		fflush(stdin);
		
		/*
		printf("Depois de strcpy, pela struct:\n");
		printf("codigo:%s\n",aux[i].codigo);
		printf("nome:%s\n",aux[i].nome);
		printf("creditos:%d\n",aux[i].creditos);
		printf("\n");
		*/
	}
	fclose(fp);
	return aux;
}

Prereq* lePrereq(){
	FILE* fp = fopen("Prerequisitos.txt","r");
	char aseparar[100];
	int top, i;
	char *codDisc, *codPrereq; //codigo da disciplina,codigo do prerequisito temporario
	char *saveptr;

	fgets(aseparar,100,fp);//esse trecho funciona
	top=atoi(aseparar);
	topP=top;

	Prereq * aux;
	aux = (Prereq*)malloc(sizeof(Prereq)*topP);
	
	for(i=0;i<topP;i++)
	{
		fgets(aseparar,100,fp);

		codDisc = strtok_r(aseparar, ",", &saveptr);
		codPrereq = strtok_r(NULL, "\n", &saveptr);



		strcpy(aux[i].codDisc,codDisc);
		strcpy(aux[i].codPrereq,codPrereq);

		/*
		printf("Depois de strcpy, pela struct:\n");
		printf("codigo disciplina:%s\n",aux[i].codDisc);
		printf("codigo prerequisito:%s\n",aux[i].codPrereq);
		printf("\n");
		*/
		
	}
	fclose(fp);
	return aux;
}

Aluno* leAluno(){
	FILE* fp = fopen("Alunos.txt","r");
	char aseparar[100];
	int top, i;
	char *ra, *nom, *log, *sen; //codigo, nome, credito temporario
	char *saveptr;

	fgets(aseparar,100,fp);
	top=atoi(aseparar);
	topA=top;
	
	Aluno * aux;
	aux = (Aluno*)malloc(sizeof(Aluno)*topA);
	
	for(i=0;i<top;i++)
	{
		fgets(aseparar,100,fp);

		ra = strtok_r(aseparar, ",", &saveptr);
		nom = strtok_r(NULL, ",", &saveptr);
		log = strtok_r(NULL, ",", &saveptr);
		sen = strtok_r(NULL, "\n", &saveptr);	

		aux[i].ra=atoi(ra);
		strcpy(aux[i].nome,nom);
		strcpy(aux[i].login,log);
		strcpy(aux[i].senha,sen);
		
		/*
		printf("Depois de strcpy, pela struct:\n");
		printf("RA:%d\n",aux[i].ra);
		printf("nome:%s\n",aux[i].nome);
		printf("login:%s\n",aux[i].login);
		printf("senha:%s\n",aux[i].senha);
		printf("\n");
		*/
	}
	fclose(fp);
	return aux;
}

Alunodisc* leAlunodisc(){
	FILE* fp = fopen("AlunosDisciplinas.txt","r");
	char aseparar[100];
	int top, i;
	char *ra, *cod, *sem, *notas, *fal; //codigo, nome, credito temporario
	char *saveptr;
	
	fgets(aseparar,100,fp);
	top=atoi(aseparar);
	topAD=top;
	
	Alunodisc * aux;
	aux = (Alunodisc*)malloc(sizeof(Alunodisc)*top);
	
	//i=1;
	for(i=0;i<top;i++)
	{

		fgets(aseparar,100,fp);

		ra = strtok_r(aseparar, ",", &saveptr);
		cod = strtok_r(NULL, ",", &saveptr);
		sem = strtok_r(NULL, ",", &saveptr);
		notas = strtok_r(NULL, ",", &saveptr);
		fal = strtok_r(NULL, "\n", &saveptr);

		fflush(stdin);
		aux[i].ra=atoi(ra);
		strcpy(aux[i].codigo,cod);
		aux[i].semestre=atoi(sem);
		aux[i].nota=atoi(notas);
		aux[i].faltas=atof(fal);
		

		fflush(stdin);
		
		
		/*
		printf("Depois de strcpy, pela struct:\n");
		printf("ra:%d\n",aux[i].ra);
		printf("codigo:%s\n",aux[i].codigo);
		printf("semestre:%d\n",aux[i].semestre);
		printf("nota:%d\n",aux[i].nota);
		printf("faltas:%d\n",aux[i].faltas);	
		printf("\n");*/

	}

	fclose(fp);
	return aux;
}

void newAluno(){
	Aluno aux;
	char ra[100];
	
	fflush(stdin);
	printf("Menu Cadastro de Alunos\n");
	printf("Digite o RA do Aluno: ");

	fgets(ra,101,stdin);
	aux.ra=atoi(ra);
	fflush(stdout);
	
	printf("ra=%d\n",aux.ra);
	printf("\nDigite o Nome do Aluno: ");
	fgets(aux.nome,101,stdin);
	
	int len=strlen(aux.nome); //where buff is your char array fgets is using
	if(aux.nome[len-1]=='\n')
   		aux.nome[len-1]='\0';
		//fflush(stdout);
	
	printf("\nDigite o login: ");
	fgets(aux.login,11,stdin);
	
	len=strlen(aux.login); //where buff is your char array fgets is using
	if(aux.login[len-1]=='\n')
   		aux.login[len-1]='\0';
		//fflush(stdout);
	
	printf("\nDigite a senha: ");
	fgets(aux.senha,11,stdin);
	
	len=strlen(aux.senha); //where buff is your char array fgets is using
	if(aux.senha[len-1]=='\n')
   		aux.senha[len-1]='\0';
		//fflush(stdin);
	
	printf("\nRA: %d",aux.ra);
	printf("\nNome: %s",aux.nome);
	printf("\nLogin: %s",aux.login);
	printf("\nSenha: %s\n",aux.senha);
	
	saveAluno(aux);

}

void saveAluno(Aluno novo){

	int top, i;
	Aluno aux;
	aux.ra = novo.ra;
	strcpy (aux.nome,novo.nome);
	strcpy (aux.login,novo.login);
	strcpy (aux.senha,novo.senha);

	FILE* fp = fopen("Alunos.txt","w");

	fprintf(fp,"%d\n",topA+1);

	for(i=0;i<topA;i++){
		fprintf(fp,"%d,%s,%s,%s\n",a[i].ra,a[i].nome,a[i].login,a[i].senha);	
		printf("%d,%s,%s,%s\n",a[i].ra,a[i].nome,a[i].login,a[i].senha);
	}
	
	fprintf(fp,"%d,%s,%s,%s\n",novo.ra,novo.nome,novo.login,novo.senha);
	printf("%d,%s,%s,%s\n",novo.ra,novo.nome,novo.login,novo.senha);
	topA++;	
	fflush(stdin);
	fclose(fp);
}

int login(Aluno *user){
    FILE *fp; //variável apontadora do arquivo
    char login[10], senha[10]; //strings que receberão do usuário o login e senha
    char nomeArq[100], loginArq[10], senhaArq[10]; //strings que salvarão dados puxados do arquivo
    int raArq; //inteiro que receberá os RAs do arquivo

    fp = fopen("Alunos.txt", "r"); //abrindo o arquivo para leitura

    if(fp == NULL){ //condição de finalização do programa caso o arquivo não seja aberto com sucesso
        puts("Impossivel abrir o arquivo!");
        exit(1);
    }

    //recebendo os dados de login e senha do usuário
    //puts("Por favor, insira seu usuario e senha:");
    printf("Usuario: ");
    scanf("%s", login);
    printf("Senha: ");
    scanf("%s", senha);

    /*laço que percorre o arquivo até encontrar o login e senha correspondente com o que o
    usuário digitou anteriormente. */
    while(fscanf(fp, "%d, %[^,], %[^,], %[^\n]", &raArq, nomeArq, loginArq, senhaArq)!= EOF){
        //comparando dados para autenticação
        if(strcmp(login, loginArq) == 0 && strcmp(senha, senhaArq) == 0){
            /* Jogando dados pegos do arquivo na variável global que será usada no
            decorrer do programa. */
			logra=raArq;
			strcpy(lognome,nomeArq);
            fclose(fp);

            return 0;
        }
    }

    //impresso em caso de falha de autenticação
    puts("Usuario nao encontrado! Tente novamente!");

    fclose(fp);

    return 1;
}

void consultaDisc(){
	int i;
	int ret;
	char cod[5];
	Disciplina *aux = d;
	
	printf ("Digite XX000 para sair\n");
	
	do{
		printf ("Digite a disciplina: ");
		scanf("%s",cod);
		if(strcmp(cod,"XX000")==0)
			break;
			
		for(i=0;i<topD;i++){
			
			ret = strcmp(cod, aux[i].codigo);
			if(ret==0){
				printf("Nome: %s\n",aux[i].nome);
				printf("Quantidade de Creditos: %d\n",aux[i].creditos);
				exibePrereq(aux[i]);
				break;
			}
		}
	}while(1);
}

void exibePrereq(Disciplina dd){
	int i,j,k;

	Disciplina *aux2 = d;
	Prereq* pre = p;

	int ret;
	int topVP=0;
	Vetprereq * aux;
	aux = (Vetprereq*)malloc(sizeof(Vetprereq)*43);
	
	for(i=0;i<topP;i++){

		if(strcmp(dd.codigo,pre[i].codDisc)==0){
			//if(strcmp(temp,pre[i].codDisc)==0){
			
			strcpy(aux[topVP].codigo,pre[i].codPrereq);
			//printf("%s\n",aux[topVP].codigo);
			
			
			
			for(j=0;j<topD;j++){
			
				ret = strcmp(aux[topVP].codigo,aux2[j].codigo);
				if(ret==0){
					printf("Pre-requisito: %s\n",aux2[j].nome);
				}
			}
			topVP++;
			continue;
		}

	}
}

void matricula(){

	char ssem[100];
	char op[7];
	int sem,ret,i,j,k,ok;
	int topVP;
	int verifdisc,verifsem;
	int contcred, contsave, contpre, contpre2;
	
	Disciplina *auxd = d;
	Alunodisc *auxad = ad;
	Vetprereq * auxvp;
	auxvp = (Vetprereq*)malloc(sizeof(Vetprereq)*43);
	Alunodisc save[32];
	
	verifsem=0;
	contsave=0;
	
	printf("Para sair, digite XX000\n");
	
	do{
		ok=0;
		verifsem=0;
	
		fflush(stdin);
		printf("Digite o semestre: ");
		fgets(ssem,101,stdin);

		sem=atoi(ssem);

		for(i=0;i<topAD;i++){
			if(logra==auxad[i].ra){		
				if(auxad[i].semestre>sem){
				verifsem++;
				break;
				}
			}
		}
		if(verifsem>0){
			printf("Nao e possivel matricular, semestre ja matriculado\n");
			ok++;
		}
		
	}while(ok>0);
	fflush(stdin);
	contcred=0;
	
	do{	
	
		printf ("Digite a disciplina: ");
		scanf("%s",op);
		if(strcmp(op,"XX000")==0)
			break;
			
		verifdisc=0;
		contpre=0;
		contpre2=0;
		for(i=0;i<topD;i++){//verifica se a disciplina existe
			ret = strcmp(op, auxd[i].codigo);
			if(ret==0){//se existir
				verifdisc++;
				contcred=contcred+auxd[i].creditos;
				
				
				if(contcred>=32){
					printf("creditos excedidos\n");
					contcred=0;
					contsave=0;
					continue;
				}	

				topVP=0;
				for(j=0;j<topP;j++){
			
					if(strcmp(auxd[i].codigo,p[j].codDisc)==0){
						strcpy(auxvp[topVP].codigo,p[j].codPrereq);
				
						for(k=0;k<topAD;k++){		
							ret = strcmp(auxvp[topVP].codigo,auxad[k].codigo);
							if(ret==0){
								if(auxad[k].nota>=5&&auxad[k].faltas<25){
									printf("Pre-requisito: %s foi atendido\n",auxad[k].codigo);
									contpre++;
									}else{
										printf("Cursou a disciplina %s, mas nota ou falta menor que permitido\n",auxad[k].codigo);
										contpre++;
										contpre2++;
									}
							} 
						}
						if(strcmp(auxvp[topVP].codigo,"0")==0){
							printf("sem pre requisito\n");	
							contpre++;
						}	
						topVP++;
					}
				}
				if(contpre==0){
					printf("Prerequisito nao foi cursado\n");
				}else if(contpre2==0){
					
					save[contsave].ra=logra;			
					strcpy(save[contsave].codigo,auxd[i].codigo);
					save[contsave].semestre=sem;
					save[contsave].nota=0.0;
					save[contsave].faltas=0.0;
					contsave++;		
					continue;
				}
			}
		}
		if(verifdisc==0){//se nao foi adicionado o contador, quer dizer que nao existe
			printf("Disciplina nao existe\n");
			continue;//vai para proxima disciplina
		}
			
	}while(1);

	
	printf("disciplinas matriculadas: \n");
	for(i=0;i<contsave;i++){

		printf("RA: %d\n",save[i].ra);
		printf("Codigo Disciplina: %s\n",save[i].codigo);
		printf("Semestre: %d\n",save[i].semestre);
		printf("Nota: %d\n",save[i].nota);
		printf("Faltas: %.3f\n",save[i].faltas);
	}
	printf("Transacao realizada com sucesso\n");
	saveMatricula(save,contsave);
}

void saveMatricula(Alunodisc * novo,int contsave){
	
	int top, i;
	Alunodisc aux;

	FILE* fp = fopen("AlunosDisciplinas.txt","w");

	fprintf(fp,"%d\n",topAD+1);

	for(i=0;i<topAD;i++){
		fprintf(fp,"%d,%s,%d,%d,%.3f\n",ad[i].ra,ad[i].codigo,ad[i].semestre,ad[i].nota,ad[i].faltas);	
	}
	for(i=0;i<contsave;i++){
		fprintf(fp,"%d,%s,%d,%d,%.3f\n",novo[i].ra,novo[i].codigo,novo[i].semestre,novo[i].nota,novo[i].faltas);
		topAD++;	
	}
	fflush(stdin);
	fclose(fp);
}
	
void alteraMatricula(){
	int sem;
	int i,j;
	
	int verif,verifdisc;
	verifdisc=0;
		
	int nota;
	float faltas;

	char ssem[100];
	char op[7];
		
		
	printf("Digite o semestre: ");
	fflush(stdin);
	fgets(ssem,101,stdin);
	sem=atoi(ssem);
	fflush(stdin);
	for(i=0;i<topAD;i++){
				
		if(ad[i].ra==logra&&ad[i].semestre==sem){
				
			for(j=0;j<topD;j++){
				if(strcmp(ad[i].codigo,d[j].codigo)==0){
					printf("%s - %s - Nota: %d, falta(%): %.3f\n",d[j].codigo,d[j].nome,ad[i].nota,ad[i].faltas);
				}
			}				
		}	
	}
	do{
		nota=11;
		faltas=111;
		printf("Digite o codigo da disciplina que deseja fazer alteracao: ");
		scanf("%s",op);
		if(strcmp(op,"XX000")==0)
			break;
			
		for(i=0;i<topD;i++){
			if(strcmp(op,d[i].codigo)==0){
				fflush(stdin);
				while(nota>10||nota<0){
					printf("Nota: ");
					scanf("%d",&nota);
				}
				fflush(stdin);
				while(faltas>100||faltas<0){
					printf("Faltas: ");
					scanf("%f",&faltas);	
				}
				verifdisc++;
			}
		}
		if(verifdisc==0){
			printf("Disciplina nao existente\n");
		}else{
			
			FILE* fp=fopen("AlunosDisciplinas.txt","w");
			fprintf(fp,"%d\n",topAD);
				
			for(i=0;i<topAD;i++){	
				if(ad[i].ra==logra&&ad[i].semestre==sem&&strcmp(op,ad[i].codigo)==0){
					fprintf(fp,"%d,%s,%d,%d,%.3f\n",ad[i].ra,ad[i].codigo,ad[i].semestre,nota,faltas);				
				}else{
					fprintf(fp,"%d,%s,%d,%d,%.3f\n",ad[i].ra,ad[i].codigo,ad[i].semestre,ad[i].nota,ad[i].faltas);
				}					
			}
			fclose(fp);
		}
						
	}while(strcmp(op,"XX000")!=0);	
}


