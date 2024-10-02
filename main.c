#include <stdio.h>
#include <stdlib.h>

long long cpf;
int senha;
int cpf_confirmado = 0;
int senha_confirmada = 0;
int linha_usuario = 0;

int criar_usuarios (char *nome, long long cpf, int senha){
    FILE *fp = fopen(nome, "w");

    if(fp == NULL){
            printf("Erro na abertura do arquivo para leitura.\n");
            return 1;
    };

    fprintf(fp, "%lld\n", cpf);
    fprintf(fp, "%d\n", senha);
    for (int a = 0; a < 4; a ++) // saldo, btc, eth, xrp = 0 (1 linha pra cada)
        fprintf(fp, "%f\n", 0);

    fclose(fp);
    return 0;
}

int usuarios_txt() {
    FILE *fp = fopen("usuarios.txt", "w");

    if(fp == NULL){
            printf("Erro na abertura do arquivo para leitura.\n");
            return 1;
    };

    // fprintf(fp, "%lld\n", 12345678901);
    // fprintf(fp, "%lld\n", 98765432100);
    // fprintf(fp, "%lld\n", 12345678987);
    fprintf(fp, "%lld\n", 123);
    fprintf(fp, "%lld\n", 456);
    fprintf(fp, "%lld\n", 111);


    fclose(fp);
    return 0;
}

int conferirCPF(long long cpf){ //tambem pega em qual linha do usuarios.txt o cpf está. se estiver na 2 = user2.txt
    FILE *fp = fopen("usuarios.txt", "r");

    if(fp == NULL){
            printf("Erro na abertura do arquivo para leitura.\n");
            return 1;
    };

    long long cpf_busca;
    linha_usuario = 1;

    while (fscanf(fp, "%lld", &cpf_busca) != EOF){
        if (cpf_busca == cpf){
            cpf_confirmado = 1;
            break;
        }
        else
        linha_usuario++;
    }

    fclose(fp);
    return 0;
}

int conferir_senha(int senha){
    char nome[10];

    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");

    if(fp == NULL){
        printf("Erro na abertura do arquivo para conferir senha.\n");
        return 1;
    };

    int linha_arquivo = 0;
    int senha_busca;

    while (fscanf(fp, "%d", &senha_busca) != EOF){
        linha_arquivo++;
        if (linha_arquivo == 2)
            break;
    }

    if (senha == senha_busca)
        senha_confirmada = 1;

    fclose(fp);
    return 0;
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

int consultar_saldo(){
 
}

int depositar_fundos() {
    float saldo_busca;
    float criptomoedas[3] = {0.0f, 0.0f, 0.0f};
    char linhas_extrato[20][100] = {0};

    // armazenar o tempo atual
    time_t tempo_atual;
    char buffer[80];
    
    // tempo atual
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", info_tempo);


    //ler dados antes
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }

    fscanf(fp, "%lld", &cpf);
    fscanf(fp, "%d", &senha);
    fscanf(fp, "%f", &saldo_busca);
    //criptomoeda
    for (int k = 0; k < 3; k++) {
        fscanf(fp, "%f", &criptomoedas[k]);
    }
    //extrato
    int total_linhas_extrato = 0;
    while (fgets(linhas_extrato[total_linhas_extrato], 100, fp) != NULL) {
        if (linhas_extrato[total_linhas_extrato][0] != '\n') {
            total_linhas_extrato++;
        }
    }
    fclose(fp);

    //valor deposito
    float valor_deposito;
    printf("Digite o valor do deposito em reais: ");
    scanf("%f", &valor_deposito);
    float saldo_atualizado = valor_deposito + saldo_busca;
    printf("R$ %.2f adicionado a conta.\n", valor_deposito);

    //sobrescrever todos os dados mudando o saldo atual
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp2 = fopen (nome, "w");
    if (fp2 == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    fprintf(fp2, "%lld\n", cpf);
    fprintf(fp2, "%d\n", senha);
    fprintf(fp2, "%.2f\n", saldo_atualizado);

    for (int k = 0; k < 3; k++) {
        fprintf(fp2, "%.6f\n", criptomoedas[k]);
    }

    for (int k = 0; k < total_linhas_extrato; k++) {
        fprintf(fp2, "%s", linhas_extrato[k]);
    }

    fclose(fp2);

    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp3 = fopen (nome, "a");
    if (fp3 == NULL) {
        printf("Erro ao abrir o arquivo para anexar o depósito.\n");
        return 1;
    }

    //nova linha de extrato
    // fprintf(fp3, "+ %.2f R$ CT: 0.0 TX: 0.0 R$: %.2f BTC: %.6f ETH: %.6f XRP: %.6f\n",
    //         valor_deposito, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    fprintf(fp3,"%s + %.2f R$ CT:      0.0 TX:   0.0 R$: %.2f BTC: %.6f ETH: %.6f XRP: %.6f\n", buffer,valor_deposito, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);

    fclose(fp3);

    return 0;
}

int sacar_fundos(){
    float saldo_busca;
    float criptomoedas[3] = {0.0f, 0.0f, 0.0f};
    char linhas_extrato[20][100] = {0};

    // armazenar o tempo atual
    time_t tempo_atual;
    char buffer[80];
    
    // tempo atual
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", info_tempo);


    //ler dados antes
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }

    fscanf(fp, "%lld", &cpf);
    fscanf(fp, "%d", &senha);
    fscanf(fp, "%f", &saldo_busca);
    //criptomoeda
    for (int k = 0; k < 3; k++) {
        fscanf(fp, "%f", &criptomoedas[k]);
    }
    //extrato
    int total_linhas_extrato = 0;
    while (fgets(linhas_extrato[total_linhas_extrato], 100, fp) != NULL) {
        if (linhas_extrato[total_linhas_extrato][0] != '\n') {
            total_linhas_extrato++;
        }
    }
    fclose(fp);

    float valor_saque;

    while (1){
    printf("Digite o valor do saque em reais: ");
    scanf("%f", &valor_saque);
    float saldo_atualizado = saldo_busca - valor_saque;
    if (saldo_atualizado < 0)
        printf("O saldo nao pode ficar negativo. Insira um valor valido.\n");
    else{
        printf("R$ %.2f removido da conta.\n", valor_saque);
        break;}
    }

    FILE *fp2 = fopen (nome, "w");

    if(fp2 == NULL){
        printf("Erro na abertura do arquivo de escritura do deposito.\n");
        return 1;
    };
    float saldo_atualizado = saldo_busca - valor_saque;

    //mantem cpf, senha, muda saldo
    fprintf(fp2, "%lld\n", cpf);
    fprintf(fp2, "%d\n", senha);
    fprintf(fp2, "%f\n", saldo_atualizado);

    for (int k = 0; k < 3; k++){
        fprintf(fp2, "%f\n", criptomoedas[k]);
    }
    for (int k = 0; k < 20; k++)
        fprintf(fp2, "%s", linhas_extrato[k]);
    fclose(fp2);

    FILE *fp3 = fopen (nome, "a");

    if(fp3 == NULL){
        printf("Erro na abertura do arquivo de escritura do deposito.\n");
        return 1;
    };

    fprintf(fp3,"%s - %.2f R$ CT:      0.0 TX:   0.0 R$: %.2f BTC: %.6f ETH: %.6f XRP: %.6f\n", buffer,valor_saque, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    // fprintf(fp3, "- %.2f R$ CT: 0.0 TX: 0.0 R$: %.2f BTC: %.6f ETH: %.6f XRP: %.6f", valor_saque, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);

    fclose(fp3);
    return 0;
    return 0;
}

int consultar_extrato(){
    char nome[10];

    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL){
        printf("Erro na abertura do arquivo para saque (leitura).\n");
        return 1;
    };

    char linha[100];
    int contador_linha = 0;//vai ler ate 20 lihnas
     while (fgets(linha, 100, fp) != NULL) {
        contador_linha++;
        if (contador_linha >= 7 && contador_linha < 27) {
            printf("%s", linha);
        }
    }
    fclose(fp);
    return 0;
}

int compra_criptomoedas(){
    int senha_validar = 1;
    while(1){
        printf("Insira sua senha novamente: ");
        scanf("%d", &senha_validar);
        if (senha_validar == senha)
            break;
        else
            printf("Senha incorreta.\n");
    }

    float bitcoin_atual;
    float ethereum_atual;
    float ripple_atual;

    time_t tempo_atual;
    char buffer[80];
    
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", info_tempo);

    FILE *fp3 = fopen ("cotacao_criptomoedas.txt", "r");
    
    if(fp3 == NULL){
        printf("Erro na abertura do arquivo para leitura da cotacao atual.\n");
        return 1;
    };

    fscanf(fp3, "%f", &bitcoin_atual);
    fscanf(fp3, "%f", &ethereum_atual);
    fscanf(fp3, "%f", &ripple_atual);
    fclose(fp3);
    
    float saldo_busca;
    float criptomoedas[3] = {0.0f, 0.0f, 0.0f};
    char linhas_extrato[20][100] = {0};

    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");

    if(fp == NULL){
        printf("Erro na abertura do arquivo de compra_criptomoedas(leitura).\n");
        return 1;
    };

    fscanf(fp, "%lld", &cpf);
    fscanf(fp, "%d", &senha);
    fscanf(fp, "%f", &saldo_busca);

    for (int k = 0; k < 3; k++) {
        fscanf(fp, "%f", &criptomoedas[k]);
    }

    int total_linhas_extrato = 0;
    while (fgets(linhas_extrato[total_linhas_extrato], 100, fp) != NULL) {
        if (linhas_extrato[total_linhas_extrato][0] != '\n') {
            total_linhas_extrato++;
        }
    }

    fclose(fp);

    int escolha;
    float cotacao_cripto_escolhida;
    float taxa_compra;
    const char* cripto_comprada;

    while (1) {
        printf("1. BTC | 2. ETH | 3. XRP\n");
        printf("Escolha a criptomoeda desejada: ");
        scanf("%d", &escolha);
        
        if (escolha == 1) {
            printf("1. Bitcoin escolhido. ");
            printf("Taxa de compra: 2%%\n");
            cotacao_cripto_escolhida = bitcoin_atual;
            taxa_compra = (float)2 / 100;
            cripto_comprada = "BTC";
            break;
        } else if (escolha == 2) {
            printf("2. Ethereum escolhido. ");
            printf("Taxa de compra: 1%%\n");
            cotacao_cripto_escolhida = ethereum_atual;
            taxa_compra = (float)1 / 100;
            cripto_comprada = "ETH";
            break;
        } else if (escolha == 3) {
            printf("3. Ripple escolhido. ");
            printf("Taxa de compra: 1%%\n");
            cotacao_cripto_escolhida = ripple_atual;
            taxa_compra = (float)1 / 100;
            cripto_comprada = "XRP";
            break;
        } else {
            printf("Insira um valor valido.\n");
        }
    }

    float valor_compra;
    float saldo_atualizado;
    float saldo_convertido_em_cripto;
    int digito;
    while (1) {
        printf("Digite o valor de compra em reais: ");
        scanf("%f", &valor_compra);
        
        // atualiza saldo: valor inserido - taxa de compra
        saldo_atualizado = saldo_busca - valor_compra - (valor_compra * taxa_compra);

        if (saldo_atualizado < 0) 
            printf("O saldo nao pode ficar negativo. Insira um valor valido.\n");
        else if (valor_compra < 0)
            printf("O valor de compra nao pode ser negativo. Para cancelar a compra, insira 0 no valor de compra.");
        else if (valor_compra == 0){
            printf("Compra cancelada.\n");
            break;}
        else {
                // valor de compra convertido em cripto
                saldo_convertido_em_cripto = (valor_compra - (valor_compra*taxa_compra)) / cotacao_cripto_escolhida;

                printf("R$ %.2f equivale a %.6f %s. Deseja confirmar a compra?\n1. Confirmar | 0. Cancelar a compra: ", valor_compra, saldo_convertido_em_cripto, cripto_comprada);
                scanf("%d", &digito);

                if (digito == 1) {
                    printf("Compra confirmada.\n");
                    criptomoedas[escolha-1] += saldo_convertido_em_cripto;
                    break;
                } else if (digito == 0) {
                    printf("Compra cancelada.\n");
                    break;
                } else {
                    printf("Insira um digito valido.\n");
                }
            }
        }
    fclose(fp);

    FILE *fp2 = fopen (nome, "w");

    if(fp2 == NULL){
        printf("Erro na abertura do arquivo de escritura da compra cripto.\n");
        return 1;
    };

    fprintf(fp2, "%lld\n", cpf);
    fprintf(fp2, "%d\n", senha);
    fprintf(fp2, "%f\n", saldo_atualizado);

    for (int k = 0; k < 3; k++){
        fprintf(fp2, "%f\n", criptomoedas[k]);
    }

    for (int k = 0; k < 20; k++)
        fprintf(fp2, "%s", linhas_extrato[k]);
    fclose(fp2);
    
    FILE *fp4 = fopen(nome, "a");

    if(fp4 == NULL){
        printf("Erro na abertura do arquivo de escritura da compra cripto.\n");
        return 1;
    };
    if (escolha == 1 && digito == 1)
        fprintf(fp4,"%s + %13.6f BTC CT: %10.2f TX: 0.20 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, bitcoin_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    if (escolha == 2 && digito == 1)
        fprintf(fp4,"%s + %13.6f ETH CT: %10.2f TX: 0.10 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, ethereum_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    if (escolha == 3 && digito == 1)
        fprintf(fp4,"%s + %13.6f XRP CT: %10.2f TX: 0.10 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, ripple_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);

    fclose(fp4);
    return 0;
}

int venda_criptomoedas(){
    int senha_validar = 1;
    while(1){
        printf("Insira sua senha novamente: ");
        scanf("%d", &senha_validar);
        if (senha_validar == senha)
            break;
        else
            printf("Senha incorreta.\n");
    }

    float bitcoin_atual;
    float ethereum_atual;
    float ripple_atual;

    time_t tempo_atual;
    char buffer[80];
    
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", info_tempo);

    FILE *fp3 = fopen ("cotacao_criptomoedas.txt", "r");
    
    if(fp3 == NULL){
        printf("Erro na abertura do arquivo para leitura da cotacao atual.\n");
        return 1;
    };

    fscanf(fp3, "%f", &bitcoin_atual);
    fscanf(fp3, "%f", &ethereum_atual);
    fscanf(fp3, "%f", &ripple_atual);
    fclose(fp3);
    
    float saldo_busca;
    float criptomoedas[3] = {0.0f, 0.0f, 0.0f};
    char linhas_extrato[20][100] = {0};

    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");

    if(fp == NULL){
        printf("Erro na abertura do arquivo de compra_criptomoedas(leitura).\n");
        return 1;
    };

    fscanf(fp, "%lld", &cpf);
    fscanf(fp, "%d", &senha);
    fscanf(fp, "%f", &saldo_busca);

    for (int k = 0; k < 3; k++) {
        fscanf(fp, "%f", &criptomoedas[k]);
    }

    int total_linhas_extrato = 0;
    while (fgets(linhas_extrato[total_linhas_extrato], 100, fp) != NULL) {
        if (linhas_extrato[total_linhas_extrato][0] != '\n') {
            total_linhas_extrato++;
        }
    }

    int escolha;
    float cotacao_cripto_escolhida;
    float taxa_compra;
    const char* cripto_comprada;

    while (1) {
        printf("1. BTC | 2. ETH | 3. XRP\n");
        printf("Escolha a criptomoeda desejada: ");
        scanf("%d", &escolha);
        
        if (escolha == 1) {
            printf("1. Bitcoin escolhido. ");
            printf("Taxa de venda: 3%%\n");
            cotacao_cripto_escolhida = bitcoin_atual;
            taxa_compra = (float)3 / 100;
            cripto_comprada = "BTC";
            // mudanca = criptomoedas[0];
            break;
        } else if (escolha == 2) {
            printf("2. Ethereum escolhido. ");
            printf("Taxa de compra: 2%%\n");
            cotacao_cripto_escolhida = ethereum_atual;
            taxa_compra = (float)2 / 100;
            cripto_comprada = "ETH";
            // mudanca = criptomoedas[1];
            break;
        } else if (escolha == 3) {
            printf("3. Ripple escolhido. ");
            printf("Taxa de compra: 1%%\n");
            cotacao_cripto_escolhida = ripple_atual;
            taxa_compra = (float)1 / 100;
            cripto_comprada = "XRP";
            // mudanca = criptomoedas[2];
            break;
        } else {
            printf("Insira um valor valido.\n");
        }
    }

    float valor_venda;
    float venda_menos_taxa;
    float saldo_atualizado;
    float saldo_convertido_em_cripto;
    int digito = 0;

    float *guardar_cripto = &criptomoedas[escolha-1];
    float *guardar_saldo = &saldo_busca;

    while (1) {
    printf("Digite o valor de venda em reais: ");
    scanf("%f", &valor_venda);

    venda_menos_taxa = valor_venda - (valor_venda * taxa_compra);
    saldo_convertido_em_cripto = venda_menos_taxa / cotacao_cripto_escolhida;

    if (valor_venda == 0) {
        printf("Venda cancelada.\n");
        break;
    } else if (valor_venda < 0) {
        printf("O valor de venda nao pode ser negativo. Insira um valor valido.\n");
        continue;
    } else if (*guardar_cripto < saldo_convertido_em_cripto) {
        printf("O saldo em criptomoedas nao pode ficar negativo. Insira um valor valido.\n");
        continue;
    } else {
        printf("R$ %.2f equivale a %.6f %s. Deseja confirmar a venda?\n1. Confirmar | 0. Cancelar a venda: ",
               valor_venda, saldo_convertido_em_cripto, cripto_comprada);
        scanf("%d", &digito);

        if (digito == 1) {
            printf("Venda confirmada.\n");
            *guardar_cripto -= saldo_convertido_em_cripto;
            saldo_atualizado = *guardar_saldo + venda_menos_taxa;
            break;
        } else if (digito == 0) {
            printf("Venda cancelada.\n");
            break;
        } else {
            printf("Insira um dígito válido.\n");
        }
    }
}

    fclose(fp);

    FILE *fp2 = fopen (nome, "w");

    if(fp2 == NULL){
        printf("Erro na abertura do arquivo de escritura da compra cripto.\n");
        return 1;
    };

    fprintf(fp2, "%lld\n", cpf);
    fprintf(fp2, "%d\n", senha);
    fprintf(fp2, "%f\n", saldo_atualizado);

    for (int k = 0; k < 3; k++){
        fprintf(fp2, "%f\n", criptomoedas[k]);
    }

    for (int k = 0; k < 20; k++)
        fprintf(fp2, "%s", linhas_extrato[k]);
    fclose(fp2);
    
    FILE *fp4 = fopen(nome, "a");

    if(fp4 == NULL){
        printf("Erro na abertura do arquivo de escritura da compra cripto.\n");
        return 1;
    };

    if (escolha == 1 && digito == 1)
        fprintf(fp4,"%s - %13.6f BTC CT: %10.2f TX: 0.30 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, bitcoin_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    if (escolha == 2 && digito == 1)
        fprintf(fp4,"%s - %13.6f ETH CT: %10.2f TX: 0.20 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, ethereum_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);
    if (escolha == 3 && digito == 1)
        fprintf(fp4,"%s - %13.6f XRP CT: %10.2f TX: 0.10 R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %6.2f\n", buffer, saldo_convertido_em_cripto, ripple_atual, saldo_atualizado, criptomoedas[0], criptomoedas[1], criptomoedas[2]);

    fclose(fp4);
    return 0;
}

int criar_arquivo_cotacao(){
 
}

int ler_arquivo_cotacao() {
    
}

void atualizar_moeda() {
   
}

int main(void){
    // por enquanto, criando os 3 usuarios.txt bem no começo.

    // criar_usuarios("user1.txt",(long long)12345678901, 123456);
    // criar_usuarios("user2.txt",(long long)98765432100, 123123);
    // criar_usuarios("user3.txt", (long long)12345678987, 321321);
    criar_usuarios("user1.txt",(long long)123, 123);
    criar_usuarios("user2.txt",(long long)456, 456);
    criar_usuarios("user3.txt", (long long)111, 111);

    //cria usuarios.txt com cpf dos 3 usuarios.
    usuarios_txt();

    printf("Boas vindas ao Exchange de Criptomoedas.\nPor favor, insira seu CPF e senha.\n");

        //cpf valido
        while(1){
        printf("CPF: ");
        scanf("%lld", &cpf);
        conferirCPF(cpf);

        if (cpf_confirmado == 1){
            break;
        }
        else
            printf("Insira um CPF valido.\n");
        }

        //senha valida
        while(1){
        printf("Senha: ");
        scanf("%d", &senha);
        conferir_senha(senha);

        if (senha_confirmada == 1){
            printf("Login confirmado!\n");
            break;
        }
        else
            printf("Senha incorreta. Insira sua senha novamente.\n");
        }
        if(senha_confirmada==1 && cpf_confirmado==1)
        menu();

        int i = 0;

        while(i>=0 && i<=8){
            printf("Selecione a operacao desejada ou digite 8 para ver o menu novamente: ");
            scanf("%d", &i);

            switch(i){
                case 1:
                    printf("1. Consultar saldo:\n");
                    consultar_saldo();
                    break;
                case 2:
                    printf("2. Consultar extrato:\n");
                    consultar_extrato(linha_usuario);
                    break;

                case 3:
                    printf("3. Depositar fundos:\n");
                    depositar_fundos();
                    break;
                case 4:
                    printf("4. Sacar fundos:\n");
                    sacar_fundos();
                    break;
                case 5:
                    printf("5. Compra de criptomoedas\n");
                    compra_criptomoedas();
                    break;
                case 6:
                    printf("6. Venda de criptomoedas\n");
                    venda_criptomoedas();
                    break;
                case 7:
                    printf("Atualizar cotacao de criptomoedas\n");
                    float btc, eth, xrp;
                    int selecao = 0;
                    criar_arquivo_cotacao("cotacao_criptomoedas.txt");
                    ler_arquivo_cotacao("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                    while (1) {
                        printf("Selecione 1 para atualizar a cotacao ou 0 para sair: ");
                        scanf("%d", &selecao);

                        if (selecao == 1) {
                            atualizar_moeda("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                            ler_arquivo_cotacao("cotacao_criptomoedas.txt", &btc, &eth, &xrp);
                        }else if (selecao == 0)
                            break;
                        else 
                            printf("Insira um digito valido.\n");

                    }
                    break;
                case 8:
                    menu();
                    break;
                    default:
                    printf("saindo...\n");
            //         //quando sair, zerar senha e cpf
                    // break;
        };//final do switch
    };//final while (pra inserir o i)
}//final int main(void)