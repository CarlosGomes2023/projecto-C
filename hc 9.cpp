#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <windows.h>
#include<time.h>
#include<stdbool.h>

typedef struct {
    int codigo;
    char nome[100];
    char morada[500];
    char bi_ou_nif[20];
    char telefone[20];
} Pessoa;

typedef struct {
    int numero_conta;
    double saldo;
    char estado[20];
    int codigo_cliente;
} Conta;

Conta contas[1000];
Pessoa clientes[1000];

char caminho[] = "C:\\Users\\igorv\\Downloads\\Banco de Dados\\BD.txt";

int qtdClientes = 0, qtdContas = 0;

void continuar() {
    char qualquertecla;
    printf("Pressione qualquer tecla para continuar\n");
    scanf(" %c", &qualquertecla);
    system("cls");
}
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}




void menuPrincipal() {
    printf("\n==============================================================================\n");
    printf("      SGB                       \n");
    printf("==============================================================================\n\n");
}

void terminar() {
    system("cls");
    printf("Programa Fechadondo");
    fflush(stdout);  
    for (int i = 0; i < 5; i++) {
        printf(".");
        Sleep(400);
    }
    system("cls");
    printf("Programa Terminado\n");
    exit(0);
}

void inicializacao() {
    printf("Abrindo o Programa");
    fflush(stdout);  
    for (int i = 0; i < 5; i++) {
        printf(".");
        Sleep(200);
    }
    system("cls");
}

void mostrarMenu() {
    printf("\t01-Cadastrar Cliente\n\t02-Listar Clientes\n\t03-Abertura de Conta\n\t04-Depositar dinheiro\n\t05-Levantar dinheiro\n\t06-Consultar Saldo\n\t07-Transferir Dinheiro\n\t08-Atualizar dados do Cliente\n\t09-Sair\n\n");
    printf("\tEscolha uma opcao: ");
}

bool cadastrar_cliente(Pessoa p) {

    while (true) {
        while (strlen(p.telefone) != 9 || p.telefone[0] != '9') {
            printf("Telefone invalido. O telefone deve ter 9 dígitos e iniciar em 9.\n");
            printf("Digite o seu telefone novamente: ");
            fgets(p.telefone, sizeof(p.telefone), stdin);
            p.telefone[strcspn(p.telefone, "\n")] = '\0';
        }

        int telefone_existente = -1;
        for (int i = 0; i < qtdClientes; i++) {
            if (strcmp(clientes[i].telefone, p.telefone) == 0) {
                telefone_existente = i;
                break;
            }
        }

        if (telefone_existente != -1) {
            printf("Numero de telefone ja cadastrado para o cliente %s.\n", clientes[telefone_existente].nome);
            printf("Por favor, utilize um número diferente.\n");
        } else {
            break; 
        }
    }

    while (true) {
        while (strlen(p.bi_ou_nif) != 14) {
            printf("Bilhete de identidade invalido. Deve ter 14 dígitos.\n");
            printf("Digite o seu bilhete de identidade novamente: ");
            fgets(p.bi_ou_nif, sizeof(p.bi_ou_nif), stdin);
            p.bi_ou_nif[strcspn(p.bi_ou_nif, "\n")] = '\0';
        }

  
        int bi_existente = -1;
        for (int i = 0; i < qtdClientes; i++) {
            if (strcmp(clientes[i].bi_ou_nif, p.bi_ou_nif) == 0) {
                bi_existente = i;
                break;
            }
        }

        if (bi_existente != -1) {
            printf("Bilhete de identidade ja cadastrado para o cliente %s.\n", clientes[bi_existente].nome);
            printf("Por favor, tente outro Bilhete.\n");
        } else {
            break;  
        }
    }

    p.codigo = qtdClientes + 1;
    clientes[qtdClientes] = p;
    qtdClientes++;
    return true;
}





int busca_conta_pelo_codigo_cliente(int codigo) {
    for (int i = 0; i < qtdContas; i++) {
        Conta c = contas[i];
        if (c.codigo_cliente == codigo) {
            return i;
        }
    }
    return -1;
}

void listar_clientes() {
    for (int i = 0; i < qtdClientes; i++) {
        Pessoa cliente = clientes[i];
        printf("\nDADOS PESSOAIS\n");
        printf("Codigo: %d\n", cliente.codigo);
        printf("Nome: %s\n", cliente.nome);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Identificacao: %s\n", cliente.bi_ou_nif);
        printf("Morada: %s\n", cliente.morada);
        printf("_");

        int posicao = busca_conta_pelo_codigo_cliente(cliente.codigo);

        if (posicao == -1) {
            continue;
        }

        Conta c = contas[posicao];
        printf("\nDADOS DA CONTA:\n");
        printf("Numero: %d\n", c.numero_conta);
        printf("Saldo: %.2f\n", c.saldo);
        printf("Estado: %s\n", c.estado);
        printf("Codigo cliente: %d\n", c.codigo_cliente);
        printf("_\n");
    }
}

int buscar_cliente(int codigo) {
    for (int i = 0; i < qtdClientes; i++) {
        Pessoa c = clientes[i];
        if (c.codigo == codigo) {
            return i;
        }
    }
    return -1;
}

bool abertura_de_conta(Pessoa p, int tipo_conta) {
    if (qtdContas >= 1000) {
        printf("Limite de contas atingido. Nao foi possivel abrir uma nova conta.\n");
        return false;
    }

    Conta c;
    c.numero_conta = qtdContas + 1;
    c.saldo = 0; 
    strcpy(c.estado, "Inativo");
    c.codigo_cliente = p.codigo;
    contas[qtdContas] = c;
    qtdContas++;
    return true;
}





int buscar_conta_pelo_numero_conta(int numero_conta) {
    for (int i = 0; i < qtdContas; i++) {
        Conta c = contas[i];
        if (c.numero_conta == numero_conta) {
            return i;
        }
    }
    return -1;
}

bool depositar_dinheiro(int numero_conta, double valor) {
    int posicao = buscar_conta_pelo_numero_conta(numero_conta);

    if (posicao == -1) {
        printf("Conta Inexistente.\n");
        return false;
    }

    Conta *c = &contas[posicao];
    c->saldo += valor;
    strcpy(c->estado, "Activo");

    return true;
}




bool levantar_dinheiro(int numero_conta, double valor) {
    int posicao = buscar_conta_pelo_numero_conta(numero_conta);

    if (posicao == -1) {
        printf("Conta inexistente.\n");
        return false;
    }

    Conta *c = &contas[posicao];

    if (valor <= 0) {
        printf("Valor invalido. Insira um valor positivo.\n");
        return false;
    }

    if (c->saldo < valor) {
        printf("Saldo insuficiente.\n");
        return false;
    }

    c->saldo -= valor;
    strcpy(c->estado, "Activo");

    return true;
}



double consultar_saldo(int numero_conta) {
    int posicao = buscar_conta_pelo_numero_conta(numero_conta);

    if (posicao == -1) {
        return -1;
    }

    return contas[posicao].saldo;
}

bool transferir_dinheiro(int numero_conta_enviar, int numero_conta_receber, double valor) {
    int posicao_enviar = buscar_conta_pelo_numero_conta(numero_conta_enviar);
    int posicao_receber = buscar_conta_pelo_numero_conta(numero_conta_receber);

    if (posicao_enviar == -1 || posicao_receber == -1) {
        printf("Conta nao encontrada.\n");
        return false;
    }

    Conta *conta_enviar = &contas[posicao_enviar];
    Conta *conta_receber = &contas[posicao_receber];

    if (valor <= 0) {
        printf("Valor invalido. Insira um valor positivo.\n");
        return false;
    }

    if (valor > conta_enviar->saldo) {
        printf("Saldo insuficiente para transferência.\n");
        return false;
    }

    conta_enviar->saldo -= valor;
    conta_receber->saldo += valor;

    printf("Transferencia realizada com sucesso!\n");
 

    return true;
}
bool atualizar_dados(Pessoa p, int codigo_cliente) {
    int posicao = buscar_cliente(codigo_cliente);

    if (posicao == -1) {
        return false;
    }

    p.codigo = codigo_cliente;
    clientes[posicao] = p;

    return true;
}

void salvar_dados_em_arquivo() {
    FILE *arquivo = fopen(caminho, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    for (int i = 0; i < qtdClientes; i++) {
        Pessoa p = clientes[i];
        fprintf(arquivo, "%d;%s;%s;%s;%s\n", p.codigo, p.nome, p.bi_ou_nif, p.telefone, p.morada);
    }

    for (int i = 0; i < qtdContas; i++) {
        Conta c = contas[i];
        fprintf(arquivo, "%d;%.2lf;%s;%d\n", c.numero_conta, c.saldo, c.estado, c.codigo_cliente);
    }

    fclose(arquivo);
}





char *limparVirgula(char *texto_com_virgula) {
    for (int i = 0; texto_com_virgula[i]; i++) {
        if (texto_com_virgula[i] == ',') {
            texto_com_virgula[i] = ' ';
        }
    }
    return texto_com_virgula;
}

void carregar_dados_do_arquivo() {
    FILE *arquivo = fopen(caminho, "r");

    if (arquivo == NULL) {
        printf("Arquivo de dados inexistente. Inicializando sem dados.\n");
        return;
    }

    while (fscanf(arquivo, "%d;%99[^;];%19[^;];%19[^;];%499[^\n]\n", &clientes[qtdClientes].codigo, clientes[qtdClientes].nome, clientes[qtdClientes].bi_ou_nif, clientes[qtdClientes].telefone, clientes[qtdClientes].morada) == 5) {
        qtdClientes++;
    }

    
    qtdContas = 0;

    while (fscanf(arquivo, "%d;%lf;%19[^;];%d\n", &contas[qtdContas].numero_conta, &contas[qtdContas].saldo, contas[qtdContas].estado, &contas[qtdContas].codigo_cliente) == 4) {
        qtdContas++;
    }

    fclose(arquivo);
}


int main() {
    inicializacao();
    carregar_dados_do_arquivo();

    int opcao;

    while (opcao != 9) {
        menuPrincipal();
        mostrarMenu();
        scanf("%d", &opcao);
        limparBuffer(); 
        
        system("cls");

        bool resposta = false;

        switch (opcao) {
            case 1: {
                Pessoa p;
                printf("\tCADASTRO DE CLIENTES\n");
            
                printf("Digite o seu nome: ");
                fgets(p.nome, sizeof(p.nome), stdin);
                p.nome[strcspn(p.nome, "\n")] = '\0';
                printf("Digite o seu telefone: ");
                fgets(p.telefone, sizeof(p.telefone), stdin);
                  p.telefone[strcspn(p.telefone, "\n")] = '\0';
                printf("Digite o seu bilhete: ");
                fgets(p.bi_ou_nif, sizeof(p.bi_ou_nif), stdin);
                p.bi_ou_nif[strcspn(p.bi_ou_nif, "\n")] = '\0';
                printf("Digite a sua morada: ");
                fgets(p.morada, sizeof(p.morada), stdin);
                 p.morada[strcspn(p.morada, "\n")] = '\0';

                resposta = cadastrar_cliente(p);
                printf("Cadastro Concluido\n\n");
                break;
            }
            case 2: {
                menuPrincipal();
                printf("\n\tCLIENTES CADASTRADOS\n ");
                listar_clientes();
                printf("\n\n");
                break;
            }
         case 3: {
    int codigo;
    menuPrincipal();
    printf("Digite o seu codigo pessoal: ");
    scanf("%d", &codigo);

    int posicao = buscar_cliente(codigo);

    if (posicao != -1) {
        int tipo_conta;
        printf("Insira o tipo de conta (0 para particular ou 1 para empresarial): ");
        scanf("%d", &tipo_conta);

        resposta = abertura_de_conta(clientes[posicao], tipo_conta);

        if (resposta) {
            printf("Conta Criada Com sucesso\n\n");
        } else {
            printf("Erro ao criar conta. Tente novamente.\n\n");
        }

       
        salvar_dados_em_arquivo();
    } else {
        printf("Codigo Inexistente. Tente novamente.\n\n");
    }
    break;
}



            case 4: {
    menuPrincipal();
    int numero_conta;
    double valor;

    int tentativas = 0;

    do {
        int codigo_cliente;
        printf("Insira o codigo do cliente para deposito: ");
        scanf("%d", &codigo_cliente);

        int posicao_cliente = buscar_cliente(codigo_cliente);

        if (posicao_cliente != -1) {
            printf("Cliente encontrado!\n");

            printf("Digite o numero de conta para deposito: ");
            scanf("%d", &numero_conta);

            printf("Digite o valor a depositar: ");
            scanf("%lf", &valor);

            if (valor <= 0) {
                printf("Deposito Invalido\n\n");
            } else {
                resposta = depositar_dinheiro(numero_conta, valor);
                printf("Concluido\n\n");
            }

            break;
        } else {
            printf("Codigo Inexistente. Tente novamente.\n\n");
            tentativas++;

    
            if (tentativas >= 2) {
                printf("Numero maximo de tentativas atingido. Voltando ao menu principal.\n\n");
                break;
            }
        }
    } while (1);  

    break;
}
            case 5: {
    menuPrincipal();
    int numero_conta;
    double valor;

    int tentativas = 0;

    do {
        printf("Digite o numero da conta: ");
        scanf("%d", &numero_conta);

        int posicao_conta = buscar_conta_pelo_numero_conta(numero_conta);

        if (posicao_conta != -1) {
            printf("Conta encontrada!\n");

            printf("Digite o valor a levantar: ");
            scanf("%lf", &valor);

            if (valor <= 0) {
                printf("Levantamento invalido\n\n");
            } else {
                resposta = levantar_dinheiro(numero_conta, valor);
                printf("concluido\n\n");
            }

            break;
        } else {
            printf("Numero de conta inexistente. Tente novamente.\n\n");
            tentativas++;

          
            if (tentativas >= 2) {
                printf("Numero maximo de tentativas atingido. Voltando ao menu principal.\n\n");
                break;
            }
        }
    } while (1);  

    break;
}

          case 6: {
    menuPrincipal();
    int numero_conta;

   
    int tentativas = 0;

    do {
        printf("Digite o numero da conta: ");
        scanf("%d", &numero_conta);

        int posicao_conta = buscar_conta_pelo_numero_conta(numero_conta);

        if (posicao_conta != -1) {
            printf("Conta encontrada!\n");

            double saldo = consultar_saldo(numero_conta);
            if (saldo != -1) {
                printf("''''''''''''''''''''''''''''''''\n");
                printf(" Saldo da conta: %.2f\n", saldo);
                printf("''''''''''''''''''''''''''''''''\n");
            }
            printf("\n\n");

            // Break out of the loop
            break;
        } else {
            printf("Numero de conta inexistente. Tente novamente.\n\n");
            tentativas++;

            
            if (tentativas >= 2) {
                printf("Numero maximo de tentativas atingido. Voltando ao menu principal.\n\n");
                break;
            }
        }
    } while (1); 

    break;
}

           case 7: {
    menuPrincipal();
    int numero_conta_enviar, numero_conta_receber;
    double valor;

    
    int tentativas = 0;

    do {
        printf("Digite o numero da conta a enviar: ");
        scanf("%d", &numero_conta_enviar);

        printf("Digite o numero da conta a receber: ");
        scanf("%d", &numero_conta_receber);

        int posicao_enviar = buscar_conta_pelo_numero_conta(numero_conta_enviar);
        int posicao_receber = buscar_conta_pelo_numero_conta(numero_conta_receber);

        if (posicao_enviar != -1 && posicao_receber != -1) {
            printf("Contas encontradas!\n");

           
            printf("Digite o valor a transferir: ");
            scanf("%lf", &valor);

            resposta = transferir_dinheiro(numero_conta_enviar, numero_conta_receber, valor);
            printf("\n\n");

            break;
        } else {
            printf("Numero de conta inexistente. Tente novamente.\n\n");
            tentativas++;

           
            if (tentativas >= 2) {
                printf("Numero maximo de tentativas atingido. Voltando ao menu principal.\n\n");
                break;
            }
        }
    } while (1); 
    break;
}

           case 8: {
    menuPrincipal();
    int codigo;


    int tentativas = 0;

    do {
        printf("Insira o seu codigo pessoal: ");
        scanf("%d", &codigo);

        int posicao = buscar_cliente(codigo);

        if (posicao != -1) {
            printf("Cliente encontrado!\n");

            Pessoa p;
            printf("Digite o seu nome: ");
            fgets(p.nome, sizeof(p.nome), stdin);
            printf("Digite o seu telefone: ");
            fgets(p.telefone, sizeof(p.telefone), stdin);
            printf("Digite o seu bilhete: ");
            fgets(p.bi_ou_nif, sizeof(p.bi_ou_nif), stdin);
            printf("Digite a sua morada: ");
            fgets(p.morada, sizeof(p.morada), stdin);

            resposta = atualizar_dados(p, codigo);
            printf("\n\n");

           
            break;
        } else {
            printf("Codigo inexistente. Tente novamente.\n\n");
            tentativas++;

           
            if (tentativas >= 2) {
                printf("Numero maximo de tentativas atingido. Voltando ao menu principal.\n\n");
                break;
            }
        }
    } while (1);  

    break;
}

            case 9: {
                terminar();
                break;
            }
        }

        if (resposta) {
            printf("Operacao realizada com sucesso!\n");
         salvar_dados_em_arquivo();
        }
    }

    return 0;
}