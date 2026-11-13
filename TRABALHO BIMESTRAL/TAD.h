//Structs
struct TpTarefas {
	char TipoReq[100],Id[100],Ip[20]; 
	int ut, utChegada, tamanhoKB, prio; 
	TpTarefas *prox;
};

struct DescTarefas {
	TpTarefas *inicio, *fim;
	int quant;
};

struct TpServidor {
	int Executando, status, IdServidor; 
	TpServidor *ant, *prox;
	TpTarefas *tarefaExecutando;
	DescTarefas descritorLista;
};

struct DescServidor {
	TpServidor *inicio, *fim;
	int quant,procTempoReal,procAPI,procBackground,esperaTempoReal,esperaAPI,esperaBackground;
};

void iniciarSer(DescServidor &dServ,int quantServ);
void criarServ(DescServidor &desc, int idServ);
void iniciarDescTar(DescTarefas &dTar);
TpTarefas* novaCaixa(TpTarefas tarefa);
TpServidor* buscaMenor(DescServidor descServ);
void inserirTarefa(DescTarefas &descTarefa,TpTarefas tarefa);
TpTarefas* consomeTarefa(DescTarefas &desc);
void RedistribuirReq(DescServidor &descServ, TpServidor *ServDesligado);
void estadoServidor(DescServidor &descServ, int IdServ, int encerrar);
void atualizarServidores(DescServidor &desc, int ut);
char Executando(DescServidor descServ);
char VerificaFim(DescServidor descServ);

void iniciarSer(DescServidor &dServ,int quantServ)
{
	dServ.quant = 0;
	for(int i=0;i<quantServ;i++)
		criarServ(dServ,i+1);	
}

void criarServ(DescServidor &desc, int idServ)
{
	TpServidor *Novo;
	Novo = new TpServidor;
	
	Novo->IdServidor = idServ; 
    Novo->status = 1;
    
	Novo->Executando = 0;
	Novo->ant=Novo->prox = NULL;
	Novo->tarefaExecutando = NULL;
	
	Novo->descritorLista.inicio = Novo->descritorLista.fim = NULL; 
	Novo->descritorLista.quant = 0;
	
	desc.procTempoReal = 0;
	desc.procAPI = 0;
	desc.procBackground = 0;

	desc.esperaTempoReal = 0;
	desc.esperaAPI = 0;
	desc.esperaBackground = 0;
	
	if(desc.quant == 0)
		desc.inicio=desc.fim = Novo;
	else
	{
		Novo->ant = desc.fim;
		desc.fim->prox = Novo;
		desc.fim = Novo;
	}
	
	desc.quant++;
}

void iniciarDescTar(DescTarefas &dTar)
{
	dTar.quant = 0;
	dTar.inicio=dTar.fim = NULL;
}

TpTarefas* novaCaixa(TpTarefas tarefa)
{
	TpTarefas *NovaTarefa;
	NovaTarefa = new TpTarefas;
	strcpy(NovaTarefa->TipoReq,tarefa.TipoReq);
	NovaTarefa->prio = tarefa.prio;
 	NovaTarefa->ut = tarefa.ut;
 	NovaTarefa->utChegada = tarefa.utChegada;
 	strcpy(NovaTarefa->Id,tarefa.Id);
 	strcpy(NovaTarefa->Ip,tarefa.Ip);
	NovaTarefa->tamanhoKB = tarefa.tamanhoKB;
	NovaTarefa->prox = NULL;
	
	return NovaTarefa;
}

//BUSCA SERVIDOR COM A MENOR FILA DE REQUISICOES
TpServidor* buscaMenor(DescServidor descServ)
{
	TpServidor *aux, *menor;
	
	aux = descServ.inicio;
	
	while(aux != NULL && aux->status == 0)
        aux = aux->prox;
    
	menor = aux;	
	
	if(aux != NULL)
	{
		aux = aux->prox;
		
		while(aux != NULL)
		{
			if(aux->status == 1)
			{
				if(aux->descritorLista.quant < menor->descritorLista.quant || (menor->Executando == 1 && aux->Executando == 0))
				{
					menor = aux;
				}
			}
			
			aux = aux->prox;
		}
	}
	
	return menor;
}

//INSERE TAREFA COM PRIORIDADE NA MENOR FILA
void inserirTarefa(DescTarefas &descTarefa,TpTarefas tarefa)
{	
	TpTarefas *pontTarefa;
	
	pontTarefa = novaCaixa(tarefa);
	
	if(descTarefa.quant == 0)
		descTarefa.inicio=descTarefa.fim=pontTarefa;
	else
		if(pontTarefa->prio < descTarefa.inicio->prio)
		{
			pontTarefa->prox=descTarefa.inicio;
			descTarefa.inicio = pontTarefa;
		}
		else
		{
			if(tarefa.prio >= descTarefa.fim->prio)
			{
				descTarefa.fim->prox = pontTarefa;
				descTarefa.fim = pontTarefa;
			}
			else
			{
				//BuscaSequencial()
				TpTarefas *atual, *ant;
				atual=descTarefa.inicio;
				while(tarefa.prio>atual->prox->prio)
					atual = atual->prox;
					
				pontTarefa->prox = atual->prox;
				atual->prox = pontTarefa;
			}
		}
		
	descTarefa.quant++;
}

//RETORNA A PRIMEIRA TAREFA DA LISTA DO SERVIDOR (PARA SER CONSUMIDA POR ELE)
TpTarefas* consomeTarefa(DescTarefas &desc)
{
	TpTarefas *aux;
	
	aux = desc.inicio;
	desc.inicio = aux->prox;
	
	aux->prox = NULL;
	if(desc.inicio == NULL)
		desc.fim = NULL;
	
	desc.quant--;
	
	return aux;
}

//QUANDO UM SERVIDOR É ENCERRADO REDESTRIBUI AS TAREFAS PARA O SERVIDOR COM A MENOR FILA DE REQUISICOES
void RedistribuirReq(DescServidor &descServ, TpServidor *ServDesligado)
{
	TpTarefas *req;
    TpServidor *destino;

    ServDesligado->status = 0; 

    while(ServDesligado->descritorLista.inicio != NULL)
    {
        req = consomeTarefa(ServDesligado->descritorLista);
        destino = buscaMenor(descServ);

        if(destino != NULL)
            inserirTarefa(destino->descritorLista, *req);
            
        delete req; 
    }
}

//QUANDO O USUÁRIO ATIVA/DESATIVA O SERVIDOR (SE FOR O ÚLTIMO NÃO DESATIVA, SE O USUÁRIO APERTOU ESC NÃO ATIVA MAIS NENHUM)
void estadoServidor(DescServidor &descServ, int IdServ, int encerrar)
{
 	TpServidor *atual, *aux;
 	
    int cont = 0;

    atual = descServ.inicio;

    while(atual != NULL && atual->IdServidor != IdServ)
        atual = atual->prox;
    
    if(atual != NULL && atual->IdServidor == IdServ)
    {
        if(atual->status == 1)
        {
            aux = descServ.inicio;

            while(aux != NULL)
            {
                if(aux->status == 1)
                    cont++;

                aux = aux->prox;
            }

            if(cont > 1)
            {
                RedistribuirReq(descServ, atual);
                atual->status = 0;
            }
        }
        else
        	if(encerrar == 0)
            	atual->status = 1;
    }
    else
        criarServ(descServ, IdServ);

}

//DECREMENTA UT DA REQUISICAO QUE ESTÁ SENDO CONSUMIDA E CONSOME A PRÓXIMA
void atualizarServidores(DescServidor &desc, int ut, int encerrar)
{
TpServidor *aux;
    aux = desc.inicio;
    
    while(aux != NULL)
    {
        if(aux->status == 1 || (aux->status == 0 && aux->Executando == 1)) 
        {
            if(aux->Executando == 1)
            {
                aux->tarefaExecutando->ut--;
                
                if(aux->tarefaExecutando->ut == 0)
                {
                    int espera = ut - aux->tarefaExecutando->utChegada;
                    switch(aux->tarefaExecutando->prio)
                    {
                        case 1:
                            desc.procTempoReal++;
                            desc.esperaTempoReal += espera;
                            break;
                        case 2:
                            desc.procAPI++;
                            desc.esperaAPI += espera;
                            break;
                        case 3:
                            desc.procBackground++;
                            desc.esperaBackground += espera;
                            break;
                    }
                    
                    delete aux->tarefaExecutando; 
                    aux->tarefaExecutando = NULL;
                    aux->Executando = 0; 
                }
            }
            
            if(encerrar == 0 && aux->Executando == 0 && aux->status == 1 && aux->descritorLista.quant > 0)
            {
                aux->tarefaExecutando = consomeTarefa(aux->descritorLista);
                aux->Executando = 1;
            }
        }
        aux = aux->prox;
    }
}

//VERIFICA SE TEM ALGUM SERVIDOR CONSUMINDO UMA REQUISICAO PARA ENCERRAR O PROGRAMA SEGURO
char Executando(DescServidor descServ)
{
    TpServidor *aux = descServ.inicio;

    while(aux != NULL)
    {
        if(aux->Executando == 1)
            return 1;

        aux = aux->prox;
    }

    return 0;
}

//SE CHEGAR NO FINAL DO ARQUIVO ELE CONTINUA EXECUTANDO ATÉ ACABAR AS REQUISICOES
char VerificaFim(DescServidor descServ)
{	
	TpServidor *atual;
	atual = descServ.inicio;
	while(atual != NULL)
	{
 		if(atual->Executando == 1)
			return 0;
		else
			if(atual->descritorLista.quant > 0)
				return 0;
				
		atual = atual->prox;
	}
	
	return 1;
}


