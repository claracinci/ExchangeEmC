#include <stdio.h>

int conferirCPF(long long cpf, int *linha_usuario){// ponteiro dentro de uma função permite que a
    long long cpf_busca;                           // variável dentro de outra função seja modificada
    *linha_usuario = 1;

    FILE *fp = fopen("usuarios.txt", "r");

    if(fp == NULL){
            printf("erro na abertura do arquivo usuarios.txt para leitura.\n");
            return 1;
    };
    
    while (fscanf(fp, "%lld", &cpf_busca) != EOF){
        if (cpf_busca == cpf){
            return 1; // cpf valido
            break;
        }else{
            (*linha_usuario)++;
}
    };

    fclose(fp);
}

int conferir_senha(int senha, int linha_usuario){
    int senha_busca;
    int linha_busca = 0;

    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL){
        printf("erro na abertura do arquivo para leitura (conferir senha).\n");
        return 1;
    };

    while (fscanf(fp, "%d", &senha_busca) != EOF){
        linha_busca++;        // armazena a senha correta, já que a 2ª linha
        if (linha_busca == 2) // no user[].txt, contem a senha do usuario.
            break;
    }

    if (senha == senha_busca)
        return 1;
    
    fclose(fp);
};

void menu(){
    printf("---------------\n");
    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar fundos\n");
    printf("4. Sacar fundos\n");
    printf("5. Compra de criptomoedas\n");
    printf("6. Venda de criptomoedas\n");
    printf("7. Atualizar cotacoes\n");
    printf("8. Menu\n");
    printf("0. Sair\n");
    printf("---------------\n");
};

void consultar_saldo(int linha_usuario){
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL){
        printf("erro na abertura do arquivo para leitura (conferir saldo).\n");
        return 1;
    };

    long cpf;
    int senha;
    float moedas[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    fscanf(fp, "%lld", &cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
    fscanf(fp, "%d", &senha);   // senha (descartavel, igual o cpf)
    for (int k = 0; k < 4; k++) // saldo: reais, btc, eth, xrp (ficam da 3 a 6 linha)
        fscanf(fp, "%f", &moedas[k]);

    printf("Saldo atual: R$ %.2f\n", moedas[0]);
    printf("Bitcoin: %.6f BTC\n", moedas[1]);
    printf("Ethereum: %.4f ETH\n", moedas[2]);
    printf("Ripple: %.2f XRP\n", moedas[3]);

    fclose(fp);
}

void depositar_fundos(int linha_usuario){
    printf(".");
}

int main(void){
    long cpf_login;
    int senha_login;
    int linha_usuario; //responsavel por localizar o numero de usuario. se linha_usuario = 2, usa o user2.txt

    printf("Boas vindas ao Exchange de Criptomoedas.\nPor favor, insira seu CPF e senha.\n");
    
    while(1){
        printf("CPF: "); //cpf valido
        scanf("%lld", &cpf_login);

        if (conferirCPF(cpf_login, &linha_usuario)) // ponteiro dentro de uma função (conferirCPF) permite que
            break;                                  // a variável dentro de outra função (main) seja modificada
        else{
            printf("Insira um CPF valido.\n");}
            continue;
    }

    while(1){ //senha valida
        printf("Senha: ");
        scanf("%d", &senha_login);


        if (conferir_senha(senha_login, linha_usuario)){
            printf("Login efetuado com sucesso!\n");
            break;
        }else{
            printf("Senha incorreta. Insira sua senha novamente.\n");
            continue;
        }
    }

    menu(); //printa o menu

    int i = 1;
    while(i >= 1 && i <= 8) { //switch responsavel por controlar o codigo inteiro (apos login)
        printf("Selecione a operacao desejada ou digite 8 para ver o menu novamente: ");
        scanf("%d", &i);
            
        switch(i) {
            case 1:
                printf("1. Consultar saldo:\n");
                consultar_saldo(linha_usuario);
                break;
            case 2:
                printf("2. Consultar extrato:\n");
                // consultar_extrato();
                break;
            case 3:
                printf("3. Depositar fundos:\n");
                depositar_fundos(linha_usuario);
                break;
            case 4:
                printf("4. Sacar fundos:\n");
                // sacar_fundos();
                break;
            case 5:
                printf("5. Compra de criptomoedas\n");
                // compra_criptomoedas();
                break;
            case 6:
                printf("6. Venda de criptomoedas\n");
                // venda_criptomoedas();
                break;
            case 7:
                printf("7. Atualizar cotacao de criptomoedas\n");
                // float btc, eth, xrp;
                // int selecao = 0;
                // criar_arquivo_cotacao("cotacao_criptomoedas.txt");
                // ler_arquivo_cotacao("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                // while (1) {
                //     printf("Selecione 1 para atualizar a cotacao ou 0 para sair: ");
                //     scanf("%d", &selecao);
                    
                //     if (selecao == 1) {
                //         atualizar_moeda("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                //         ler_arquivo_cotacao("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                //     } else if (selecao == 0)
                //         break;
                //     else 
                //         printf("Insira um digito valido.\n");
                // }
                break;
            case 8:
                menu();
                break;
            default: //0 ou qualquer outro numero diferente de 1 a 8 finaliza o switch
                printf("Saindo...\n");
                break;
        } //fim switch
    }//fim while
};