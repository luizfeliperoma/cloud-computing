#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include "TAD.h"

void Moldura(int CI, int LI, int CF, int LF, int CorT, int CorF)
{
	int i;
	textcolor(CorT);
	textbackground(CorF);
	gotoxy(CI,LI);
	printf("%c",201);
	gotoxy(CF,LI);
	printf("%c",187);
	gotoxy(CI,LF);
	printf("%c",200); 
	gotoxy(CF,LF);
	printf("%c",188);
	
	for(i=CI+1; i<CF; i++)
	{
		gotoxy(i,LI);
		printf("%c",205);
		
		gotoxy(i,LF);
		printf("%c",205);
	}
	
	for(i=LI+1; i<LF; i++)
	{
		gotoxy(CI,i);
		printf("%c",186);
		
		gotoxy(CF,i);
		printf("%c",186);
	}
	textcolor(7);
	textbackground(0);
}

void LimparTela(void)
{
	for(int i=2;i<30;i++)
	{
		gotoxy(2,i);printf("                                                                                                                    ");
	}
}

void LimparUT(void)
{
	gotoxy(3,29);printf ("                                                                                                                  ");
}

void Limpar(int c,int l)
{
	gotoxy(c,l);printf("  ");
}

//MOLDURA GERAL
void MolduraServ(int quant,int ut)
{
	gotoxy(55,2); printf("SERVIDORES");
	gotoxy(3,29); printf("ALTERAR UT [U]");
	gotoxy(89,29); printf("PROCESSO CHEGA A CADA %d UT's", ut);
	int CI=3, LI=5, CF=24, LF=28, CorF=0, CP=13;
	
	for(int i=0;i<quant;i++)
	{
		textcolor(GREEN);
		gotoxy(CP,4); printf("[");
		CP++;
		textcolor(WHITE);
		gotoxy(CP,4); printf("%d",i+1);
		CP++;
		textcolor(GREEN);
		gotoxy(CP,4); printf("]");
		CP++;
		textcolor(WHITE);
		CP+=20;
	}
		
	for(int i=quant;i<5;i++)
	{
		textcolor(RED);
		gotoxy(CP,4); printf("[");
		CP++;
		textcolor(WHITE);
		gotoxy(CP,4); printf("%d",i+1);
		CP++;
		textcolor(RED);
		gotoxy(CP,4); printf("]");
		CP++;
		textcolor(WHITE);
		CP+=20;
	}
	
	for(int i=0;i<quant;i++)
	{
		Moldura(CI,LI,CF,LF,GREEN,CorF); 
		CI+=23;
		CF+=23;
	}
	
  	for(int i=quant;i<5;i++)
  	{
  		Moldura(CI,LI,CF,LF,RED,CorF); 
		CI+=23;
		CF+=23;
  	}

}

//MOLDURA REQUISICAO
void MolduraProcesso(int CI, int LI, TpTarefas tarefa, int color)
{
    textcolor(color);

    gotoxy(CI,LI);printf("%c",218);
    gotoxy(CI+19,LI);printf("%c",191);
    gotoxy(CI,LI+4);printf("%c",192);
    gotoxy(CI+19,LI+4);printf("%c",217);

    for(int col=CI+1;col<CI+19;col++)
    {
        gotoxy(col,LI);printf("%c",196);
        gotoxy(col,LI+4);printf("%c",196);
    }

    for(int lin = LI+1; lin < LI+4; lin++)
    {
        gotoxy(CI,lin);printf("%c",179);
        gotoxy(CI+19,lin);printf("%c",179);
    }

    if(stricmp(tarefa.TipoReq,"Tempo Real") == 0)
        textcolor(LIGHTRED);
    else if(stricmp(tarefa.TipoReq,"Requisicao API") == 0)
        textcolor(LIGHTCYAN);
    else
        textcolor(LIGHTGREEN);

    int tam = strlen(tarefa.TipoReq);
    int pos = CI + ((22-tam)/2);
    
    gotoxy(CI+1, LI+1);
    printf("                  "); 
    gotoxy(pos,LI+1);printf("%s", tarefa.TipoReq);

    textcolor(WHITE);

    gotoxy(CI+1,LI+2);
    printf("                  ");
    gotoxy(CI+1,LI+2);printf("ID:%s", tarefa.Id);

    gotoxy(CI+1,LI+3);
    printf("                  ");
    gotoxy(CI+1,LI+3);printf("UT:%d", tarefa.ut);

    gotoxy(CI+12,LI+3);printf("%dKB", tarefa.tamanhoKB);

    textcolor(WHITE);
}

//MODULRA REQUISICAO CONSUMIDA/FILA DE TAREFAS DE CADA SERVIDOR
void ExibirServidor(DescServidor descServ)
{
    int x, y;
    TpServidor *atual;
    TpTarefas *auxProc;
    int cont;
	
	atual = descServ.inicio;
	
    while(atual != NULL)
    {
        y = 6;       
        cont = 0;  

        switch(atual->IdServidor)
        {
            case 1: x = 4; 
				break;
            case 2: x = 27; 
				break;
            case 3: x = 50; 
				break;
            case 4: x = 73; 
				break;
            case 5: x = 96; 
				break;
        }

        if(atual->status == 1 || atual->Executando == 1)
        {
            if(atual->Executando == 1 && atual->tarefaExecutando != NULL)
            {
                MolduraProcesso(x,y,*(atual->tarefaExecutando),GREEN);
                cont++;
            }

            auxProc = atual->descritorLista.inicio;
            while(auxProc != NULL && cont < 4) 
            {
                MolduraProcesso(x,y+(cont * 5),*auxProc,WHITE);
                auxProc = auxProc->prox;
                cont++;
            }
            
            if(auxProc != NULL)
            {
                textcolor(WHITE);
                gotoxy(x+9,y+20);printf("+");
            }
            else
            {
                gotoxy(x + 9, y + 20);printf("   ");
            }
        }

        while(cont < 4)
        {
            for(int i=0;i<5;i++)
            {
                gotoxy(x,y+(cont*5)+i);
				printf("                    "); 
            }
            cont++;
        }

        atual = atual->prox;
    }
}

//MUDO A COR VERDE/VERMELHO QUANDO ATIVA/DESATIVA O SERVIDOR
void pintarServ(DescServidor desc,int tecla,int encerrar)
{
	TpServidor *aux;
	int CI,CF;
	
	aux = desc.inicio;
	
	while(aux != NULL && aux->IdServidor != tecla)
        aux = aux->prox;
	
	switch(tecla)
	{
		case 1:
			CI=3;
			CF=24;
			break;
		case 2:
			CI=26;
			CF=47;
			break;
		case 3:
			CI=49;
			CF=70;
			break;
		case 4:
			CI=72;
			CF=93;
			break;
		case 5:
			CI=95;
			CF=116;
			break;
	}
	
	if(aux->status == 1 && encerrar == 0)
	{
		Moldura(CI,5,CF,28,GREEN,0); 
		textcolor(GREEN);
		gotoxy(CI+10,4);printf("[");
		gotoxy(CI+12,4);printf("]");
	}
	else
	{
		Moldura(CI,5,CF,28,RED,0); 
		textcolor(RED);
		gotoxy(CI+10,4);printf("[");
		gotoxy(CI+12,4);printf("]");
	}
	textcolor(WHITE);
}

void alterarUT(int &ut)
{
	int novaUt;
	LimparUT();
	gotoxy(3,29);printf("DIGITE A NOTA UT: ");
	gotoxy(22,29);scanf("%d",&novaUt);
	
	while(novaUt <= 0)
	{
		gotoxy(30,29);printf("DIGITE UMA UT MAIOR QUE 0!");
		Limpar(22,29);
		gotoxy(3,29);printf("DIGITE A NOTA UT: ");
		gotoxy(22,29);scanf("%d",&novaUt);
	}
	LimparUT();
	ut = novaUt;
	gotoxy(3,29); printf("ALTERAR UT [U]");
	gotoxy(89,29); printf("PROCESSO CHEGA A CADA %d UT's", ut);
}

void relatorio(DescServidor desc)
{
	LimparTela();
	gotoxy(53,3);printf("RELATORIO");
	
	gotoxy(5,9);printf("QUANTIDADE DE REQUISICOES");
	gotoxy(50,9);printf("TEMPO MEDIO ESPERA");
	gotoxy(88,9);printf("REQUISICOES NAO PROCESSADAS");
	
	gotoxy(5,11);printf("Tempo Real: %d",desc.procTempoReal);
	gotoxy(5,12);printf("Requisicao API: %d",desc.procAPI);
	gotoxy(5,13);printf("Background: %d",desc.procBackground);
	
	gotoxy(50,11);
	if(desc.procTempoReal>0)
		printf("Tempo Real: %.2f",(float)desc.esperaTempoReal/desc.procTempoReal);
	else
		printf("Tempo Real: 0.00");
	
	gotoxy(50,12);
	if(desc.procAPI)
		printf("Requisicao API: %.2f",(float)desc.esperaAPI/desc.procAPI);
	else
		printf("Requisicao API: 0.00");
		
	gotoxy(50,13);
	if(desc.procBackground)
		printf("Background: %.2f",(float)desc.esperaBackground/desc.procBackground);
	else
		printf("Background: 0.00");
	
	TpServidor *aux;
	aux = desc.inicio;
	int totalTar=0;
	
	while(aux!=NULL)
	{
		totalTar += aux->descritorLista.quant;
		
		if(aux->Executando == 1)
			totalTar++;
		
		aux = aux->prox;
	}
	
	gotoxy(88,11);printf("%d",totalTar);
	
	gotoxy(35,23);printf("PRESSIONE QUALQUER TECLA PARA ENCERRAR O PROGRAMA");
}

void InicioServ(int quantServidores, int ut)
{	
	LimparTela();
	DescServidor descServ;
	TpServidor *servidorMenor;
	TpTarefas tarefa;
		
	MolduraServ(quantServidores, ut);
	char tecla;
	int tempo=0, encerrar = 0;
	FILE *Ptr = fopen("Configuracao.txt", "r");
	
	iniciarSer(descServ,quantServidores);
		
	do
	{
		if(!kbhit())
		{
			if(tempo%ut == 0 && !feof(Ptr) && !encerrar)
			{
				fscanf(Ptr,"%[^,],%d,%[^,],%[^,],%d\n",tarefa.TipoReq,&tarefa.ut,tarefa.Id,tarefa.Ip,&tarefa.tamanhoKB);
				
				if(stricmp(tarefa.TipoReq,"Tempo Real") == 0)
					tarefa.prio = 1;
				else 
					if(stricmp(tarefa.TipoReq,"Requisicao API") == 0)
						tarefa.prio = 2;
					else
						tarefa.prio = 3;
				
				tarefa.utChegada=tempo;
		
				servidorMenor = buscaMenor(descServ);
				inserirTarefa(servidorMenor->descritorLista,tarefa);
			}
			
			ExibirServidor(descServ);
			atualizarServidores(descServ,tempo,encerrar);
			
			Sleep(2000);
			tempo++;
		}
		else
		{
			tecla = getch();
			if(tecla == 27)
				encerrar = 1;
			else
				if(tecla >= 49 && tecla <= 53)
				{
					estadoServidor(descServ,tecla-48,encerrar);
					pintarServ(descServ,tecla-48,encerrar);
				}
				else
					if(toupper(tecla) == 'U')
						alterarUT(ut);
		}
		
	}while((!encerrar && (!feof(Ptr) || !VerificaFim(descServ))) || (encerrar && Executando(descServ)));
	
	relatorio(descServ);
	getch();
	fclose(Ptr);
}

int main(void)
{
	int quantServidores=0, ut;
	char serv;
	
	Moldura(1,1,118,30,9,3);
	
	gotoxy(50,3); printf("FILAS DE SERVIDORES");
	
	do
	{
		gotoxy(35,8); printf("Quant. de Servidores para serem utilizados: (1-5)");
		gotoxy(42,10); printf("[A]1    [B]2    [C]3    [D]4    [E]5");
		fflush(stdin);
		serv = toupper(getch());
		
		switch(serv)
		{
			case 'A':
				quantServidores=1;
				break;
			case 'B':
				quantServidores=2;
				break;
			case 'C':
				quantServidores=3;
				break;
			case 'D':
				quantServidores=4;
				break;
			case 'E':
				quantServidores=5;
				break;
			
			default: 
				gotoxy(48,13); printf("Digite uma letra valida!");
				break;
		}
	
	}while(quantServidores==0);
	
	gotoxy(25,13); printf("Digite o intervalo (em UT) para que novas tarefas sejam solicitadas:");
	gotoxy(25,14);scanf("%d", &ut);
	
	while(ut<=0)
	{
		gotoxy(40,19);printf("Digite um intervalo maior que 0!");
		Limpar(25,14);
		gotoxy(25,13); printf("Digite o intervalo (em UT) para que novas tarefas sejam solicitadas:");
		gotoxy(25,14);scanf("%d", &ut);
	}
	
	InicioServ(quantServidores, ut);
		
	return 0;
}
