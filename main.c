#include <stdio.h>
#include <time.h>

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

void consultar_saldo(int linha_usuario, long *cpf, int *senha, float *moedas){
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL)
        printf("erro na abertura do arquivo para leitura (conferir saldo).\n");

    fscanf(fp, "%ld", cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
    fscanf(fp, "%d", senha);   // senha (descartavel, igual o cpf)
    for (int k = 0; k < 4; k++) // saldo: reais, btc, eth, xrp (ficam da 3 a 6 linha)
        fscanf(fp, "%f", &moedas[k]);

    printf("Saldo atual: R$ %.2f\n", moedas[0]);
    printf("Bitcoin: %.6f BTC\n", moedas[1]);
    printf("Ethereum: %.4f ETH\n", moedas[2]);
    printf("Ripple: %.2f XRP\n", moedas[3]);

    fclose(fp);
}

void adicionar_linha_extrato(int linha_usuario, char operacao, float valor, char *tipo, float cotacao, float taxa, float *moedas){
    char nome[20];
    sprintf(nome, "user%dextrato.txt", linha_usuario);
    FILE *fp = fopen (nome, "a");
    if(fp == NULL){
        printf("Erro na abertura do arquivo user%dextrato.txt.\n", linha_usuario);
        return;}

    time_t tempo_atual;
    char tempo[80];
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(tempo, sizeof(tempo), "%d/%m/%Y %H:%M:%S", info_tempo);

    fprintf(fp,"%s %c %16.8f %s CT: %10.2f TX: %3.2f R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %7.2f\n", tempo, operacao, valor, tipo, cotacao, taxa, moedas[0], moedas[1], moedas[2], moedas[3]);
    printf("Nova linha adicionada ao extrato:\n");
    printf("%s %c %16.8f %s CT: %10.2f TX: %3.2f R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %7.2f\n", tempo, operacao, valor, tipo, cotacao, taxa, moedas[0], moedas[1], moedas[2], moedas[3]);

    fclose(fp);
}

void depositar_fundos(int linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    float valor_deposito;
    while(1){
        printf("Digite o valor do deposito em reais: ");
        scanf("%f", &valor_deposito);
        if (valor_deposito < 0)
            printf("Digite um valor valido. Caso deseje cancelar o deposito, digite 0.\n");
        else if (valor_deposito == 0){
            printf("Deposito cancelado.\n");
            return;
            break;}
        else{
            printf("R$ %.2f adicionado a conta.\n", valor_deposito);
            break;}
    }

    moedas[0] += valor_deposito;

    //sobrescrever todos os dados mudando o saldo atual
    char nome[20];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "w");

    if (fp == NULL) {
        printf("erro ao abrir o arquivo para escrita (sobrescrever dados).\n");
        return;}
    fprintf(fp, "%ld\n", cpf);
    fprintf(fp, "%d\n", senha);
    for (int k = 0; k < 4; k++)
        fprintf(fp, "%.6f\n", moedas[k]);
    fclose(fp);

    char operacao = '+'; //chamar funcao de extrato
    char tipo[4] = " R$";
    float taxa = 0.0;
    float cotacao = 0.0;
    adicionar_linha_extrato(linha_usuario, operacao, valor_deposito, tipo, cotacao, taxa, moedas);
}

void consultar_extrato(int linha_usuario){
    char nome[20];
    sprintf(nome, "user%dextrato.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo para leitura (extrato).\n");
        return 1;
    };

    int linhas_maximo = 40;
    int tamanho_linha = 200;
    char extrato[linhas_maximo][tamanho_linha];
    int i = 0;

    while (fgets(extrato[i], tamanho_linha, fp) != NULL){
        i++;
    }
    if (i == 0)
        printf("Extrato vazio.\n");
    else{
        printf("Extrato:\n");
        for (int j; j < i; j++)
            printf("%s", extrato[j]);
    }

}

void sacar_fundos(linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    float valor_saque;
    while(1){
        printf("Digite o valor do saque em reais: ");
        scanf("%f", &valor_saque);
        float saldo_atualizado = moedas[0] - valor_saque;
        if (saldo_atualizado < 0){
            printf("O saldo nao pode ficar negativo. Insira um valor ate %.2f ou digite 0 para sair.\n", moedas[0]);
            continue;
        }else if(valor_saque == 0){
            printf("Saque cancelado.\n");
            return;
            break;
        }else{
            printf("R$ %.2f removido da conta.\n", valor_saque);
            break;
        }
    }

    moedas[0] -= valor_saque;

    char nome[20]; //sobrescrever todos os dados mudando o saldo atual
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "w");

    if (fp == NULL) {
        printf("erro ao abrir o arquivo para escrita (sobrescrever dados).\n");
        return;}
    fprintf(fp, "%ld\n", cpf);
    fprintf(fp, "%d\n", senha);
    for (int k = 0; k < 4; k++)
        fprintf(fp, "%.6f\n", moedas[k]);
    fclose(fp);

    char operacao = '-'; //chamar funcao de extrato
    char tipo[4] = " R$";
    float taxa = 0.0;
    float cotacao = 0.0;
    adicionar_linha_extrato(linha_usuario, operacao, valor_saque, tipo, cotacao, taxa, moedas);
}

int main(void){
    long cpf_login;
    int senha_login;
    int linha_usuario; //responsavel por localizar o numero de usuario. se linha_usuario = 2, usa o user2.txt

    printf("Boas vindas ao Exchange de Criptomoedas.\nPor favor, insira seu CPF e senha.\n");
    
    while(1){
        printf("CPF: "); //cpf valido
        scanf("%ld", &cpf_login);

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
    float saldo_atual, btc, eth, xrp;
    float *moedas[4] = {&saldo_atual, &btc, &eth, &xrp};
    long cpf;
    int senha;

    int i = 1;
    while(i >= 1 && i <= 8) { //switch responsavel por controlar o codigo inteiro (apos login)
        printf("\nSelecione a operacao desejada ou digite 8 para ver o menu novamente: ");
        scanf("%d", &i);
            
        switch(i) {
            case 1:
                printf("\n1. Consultar saldo:\n");
                consultar_saldo(linha_usuario, &cpf, &senha, moedas);
                break;
            case 2:
                printf("\n2. Consultar extrato:\n");
                consultar_extrato(linha_usuario);
                break;
            case 3:
                printf("\n3. Depositar fundos:\n");
                depositar_fundos(linha_usuario);
                break;
            case 4:
                printf("\n4. Sacar fundos:\n");

                int senha_novamente;
                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    sacar_fundos(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 5:
                printf("\n5. Compra de criptomoedas\n");

                int senha_novamente;
                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    // sacar_fundos(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 6:
                printf("\n6. Venda de criptomoedas\n");

                int senha_novamente;
                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    // sacar_fundos(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 7:
                printf("\n7. Atualizar cotacao de criptomoedas\n");
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