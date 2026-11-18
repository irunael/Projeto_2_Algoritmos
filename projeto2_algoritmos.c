// Nome: Anna Beatriz Fatala Pereira do Carmo - RA: 10755419
// Nome: Lana Victória Elimelek Esteves Cezar - RA: 10436422
// Nome: Pedro Henrique Alvarez - RA: 10769777

#include <stdio.h>
#include <string.h>

#define MAX 1000

int carregarLeituras(const char nomeArquivo[], int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int max){
    FILE *arquivo;
    int n;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) return -1;

    n = 0;
    while (n < max && fscanf(arquivo, "%d %d %d %d %f", &ids[n], &unidadeIds[n], &meses[n], &anos[n], &consumos[n]) == 5){
        n++;
    }

    fclose(arquivo);
    return n;
}

int carregarUnidades(const char nomeArquivo[], int unidadeIds[], char descricoes[][50], int max){
    FILE *arquivo;
    char linha[200], descricao[50];
    int n, id, inicio;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) return -1;

    n = 0;
    while (n < max && fgets(linha, sizeof(linha), arquivo) != NULL){
        inicio = 0;
        if ((unsigned char)linha[0] == 0xEF) inicio = 3; // pula o BOM de arquivos UTF-8

        if (sscanf(&linha[inicio], "%d;%49[^\r\n]", &id, descricao) == 2){
            unidadeIds[n] = id;
            strcpy(descricoes[n], descricao);
            n++;
        }
    }

    fclose(arquivo);
    return n;
}

int buscaLinear(int ids[], int n, int idProcurado){
    int i;

    for (i = 0; i < n; i++){
        if (ids[i] == idProcurado) return i;
    }

    return -1;
}

int buscaBinaria(int ids[], int n, int idProcurado){
    int inicio, fim, meio;

    inicio = 0;
    fim = n - 1;

    while (inicio <= fim){
        meio = (inicio + fim) / 2;
        if (ids[meio] == idProcurado) return meio;
        else if (ids[meio] < idProcurado) inicio = meio + 1;
        else fim = meio - 1;
    }

    return -1;
}

void trocarLeituras(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int i, int j){
    int auxInt;
    float auxFloat;

    auxInt = ids[i];
    ids[i] = ids[j];
    ids[j] = auxInt;

    auxInt = unidadeIds[i];
    unidadeIds[i] = unidadeIds[j];
    unidadeIds[j] = auxInt;

    auxInt = meses[i];
    meses[i] = meses[j];
    meses[j] = auxInt;

    auxInt = anos[i];
    anos[i] = anos[j];
    anos[j] = auxInt;

    auxFloat = consumos[i];
    consumos[i] = consumos[j];
    consumos[j] = auxFloat;
}

void bubbleSortLeituraPorId(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int n){
    int i, j;

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (ids[j] > ids[j + 1]) trocarLeituras(ids, unidadeIds, meses, anos, consumos, j, j + 1);
        }
    }
}

void insertionSortLeituraPorId(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int n){
    int i, j;

    for (i = 1; i < n; i++){
        j = i;
        while (j > 0 && ids[j - 1] > ids[j]){
            trocarLeituras(ids, unidadeIds, meses, anos, consumos, j - 1, j);
            j--;
        }
    }
}

void selectionSortLeituraPorId(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int n){
    int i, j, menor;

    for (i = 0; i < n - 1; i++){
        menor = i;
        for (j = i + 1; j < n; j++){
            if (ids[j] < ids[menor]) menor = j;
        }
        if (menor != i) trocarLeituras(ids, unidadeIds, meses, anos, consumos, i, menor);
    }
}

void quickSortLeituraPorId(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int inicio, int fim){
    int i, j, pivo;

    if (inicio >= fim) return;

    trocarLeituras(ids, unidadeIds, meses, anos, consumos, (inicio + fim) / 2, fim);
    pivo = ids[fim];
    i = inicio - 1;

    for (j = inicio; j < fim; j++){
        if (ids[j] <= pivo){
            i++;
            trocarLeituras(ids, unidadeIds, meses, anos, consumos, i, j);
        }
    }
    trocarLeituras(ids, unidadeIds, meses, anos, consumos, i + 1, fim);

    quickSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, inicio, i);
    quickSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, i + 2, fim);
}

void intercalarLeituras(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int inicio, int meio, int fim){
    int tIds[MAX], tUnidadeIds[MAX], tMeses[MAX], tAnos[MAX];
    float tConsumos[MAX];
    int i, j, k, origem;

    i = inicio;
    j = meio + 1;

    for (k = 0; k <= fim - inicio; k++){
        if (j > fim || (i <= meio && ids[i] <= ids[j])){
            origem = i;
            i++;
        }
        else{
            origem = j;
            j++;
        }
        tIds[k] = ids[origem];
        tUnidadeIds[k] = unidadeIds[origem];
        tMeses[k] = meses[origem];
        tAnos[k] = anos[origem];
        tConsumos[k] = consumos[origem];
    }

    for (k = 0; k <= fim - inicio; k++){
        ids[inicio + k] = tIds[k];
        unidadeIds[inicio + k] = tUnidadeIds[k];
        meses[inicio + k] = tMeses[k];
        anos[inicio + k] = tAnos[k];
        consumos[inicio + k] = tConsumos[k];
    }
}

void mergeSortLeituraPorId(int ids[], int unidadeIds[], int meses[], int anos[], float consumos[], int inicio, int fim){
    int meio;

    if (inicio >= fim) return;

    meio = (inicio + fim) / 2;
    mergeSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, inicio, meio);
    mergeSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, meio + 1, fim);
    intercalarLeituras(ids, unidadeIds, meses, anos, consumos, inicio, meio, fim);
}

float calcularConsumoUnidade(int unidadeIds[], float consumos[], int n, int unidadeId){
    int i;
    double total;

    total = 0;
    for (i = 0; i < n; i++){
        if (unidadeIds[i] == unidadeId) total = total + consumos[i];
    }

    return (float)total;
}

float calcularConsumoPeriodo(int meses[], int anos[], float consumos[], int n, int mesInicial, int anoInicial, int mesFinal, int anoFinal){
    int i, chave, chaveInicial, chaveFinal;
    double total;

    chaveInicial = anoInicial * 100 + mesInicial;
    chaveFinal = anoFinal * 100 + mesFinal;

    total = 0;
    for (i = 0; i < n; i++){
        chave = anos[i] * 100 + meses[i];
        if (chave >= chaveInicial && chave <= chaveFinal) total = total + consumos[i];
    }

    return (float)total;
}

int encontrarUnidadeMaiorConsumo(int unidadeIds[], float consumos[], int n){
    int i, melhorId;
    float total, maior;

    if (n <= 0) return -1;

    melhorId = unidadeIds[0];
    maior = calcularConsumoUnidade(unidadeIds, consumos, n, melhorId);

    for (i = 1; i < n; i++){
        total = calcularConsumoUnidade(unidadeIds, consumos, n, unidadeIds[i]);
        if (total > maior || (total == maior && unidadeIds[i] < melhorId)){
            maior = total;
            melhorId = unidadeIds[i];
        }
    }

    return melhorId;
}

int main(){
    int ids[MAX], unidadeIds[MAX], meses[MAX], anos[MAX];
    float consumos[MAX];
    int unidadeIdsCatalogo[MAX];
    char descricoes[MAX][50];
    int nLeituras, nUnidades, opcao, algoritmo, ordenado;
    int idBusca, indice, idUnidade, unidadeMaior;
    int mesInicial, anoInicial, mesFinal, anoFinal;
    float total;
    int i, c;

    nLeituras = carregarLeituras("leituras.txt", ids, unidadeIds, meses, anos, consumos, MAX);
    if (nLeituras == -1){
        printf("Erro ao abrir o arquivo leituras.txt.\n");
        printf("Encerrando o programa.\n");
        return 1;
    }

    nUnidades = carregarUnidades("unidades.csv", unidadeIdsCatalogo, descricoes, MAX);
    if (nUnidades == -1){
        printf("Erro ao abrir o arquivo unidades.csv.\n");
        printf("Encerrando o programa.\n");
        return 1;
    }

    ordenado = 0;

    do {
        printf("\n===== SISTEMA DE ANALISE DE CONSUMO DE ENERGIA =====\n\n");
        printf("1. Mostrar leituras ordenadas por ID\n");
        printf("2. Buscar leitura por ID\n");
        printf("3. Calcular consumo de uma unidade\n");
        printf("4. Calcular consumo em um periodo\n");
        printf("5. Identificar unidade com maior consumo\n");
        printf("6. Sair da aplicacao\n\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1){
            while ((c = getchar()) != '\n' && c != EOF);
            opcao = 0;
        }

        switch (opcao){
            case 1:
                if (ordenado == 0){
                    printf("\nEscolha o algoritmo de ordenacao:\n\n");
                    printf("1. Bubble Sort\n");
                    printf("2. Insertion Sort\n");
                    printf("3. Selection Sort\n");
                    printf("4. Quick Sort\n");
                    printf("5. Merge Sort\n\n");
                    printf("Opcao: ");
                    if (scanf("%d", &algoritmo) != 1){
                        while ((c = getchar()) != '\n' && c != EOF);
                        algoritmo = 0;
                    }

                    if (algoritmo == 1) bubbleSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, nLeituras);
                    else if (algoritmo == 2) insertionSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, nLeituras);
                    else if (algoritmo == 3) selectionSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, nLeituras);
                    else if (algoritmo == 4) quickSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, 0, nLeituras - 1);
                    else if (algoritmo == 5) mergeSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, 0, nLeituras - 1);
                    else{
                        printf("Opcao invalida. Tente novamente.\n");
                        break;
                    }
                    ordenado = 1;
                }

                printf("\nLeituras ordenadas por ID:\n\n");
                printf("ID | Unidade | %-26s | Mes | Ano  | Consumo (kWh)\n", "Descricao");
                printf("-----------------------------------------------------------------------\n");
                for (i = 0; i < nLeituras; i++){
                    indice = buscaLinear(unidadeIdsCatalogo, nUnidades, unidadeIds[i]);
                    printf("%-2d | %-7d | %-26s | %-3d | %-4d | %.2f\n", ids[i], unidadeIds[i], indice == -1 ? "-" : descricoes[indice], meses[i], anos[i], consumos[i]);
                }
                break;

            case 2:
                printf("\nDigite o ID da leitura: ");
                if (scanf("%d", &idBusca) != 1){
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }

                printf("\nEscolha o algoritmo de busca:\n\n");
                printf("1. Busca linear\n");
                printf("2. Busca binaria\n\n");
                printf("Opcao: ");
                if (scanf("%d", &algoritmo) != 1){
                    while ((c = getchar()) != '\n' && c != EOF);
                    algoritmo = 0;
                }

                if (algoritmo == 1){
                    indice = buscaLinear(ids, nLeituras, idBusca);
                }
                else if (algoritmo == 2){
                    if (ordenado == 0){
                        quickSortLeituraPorId(ids, unidadeIds, meses, anos, consumos, 0, nLeituras - 1);
                        ordenado = 1;
                    }
                    indice = buscaBinaria(ids, nLeituras, idBusca);
                }
                else{
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }

                if (indice == -1){
                    printf("\nLeitura nao encontrada.\n");
                }
                else{
                    i = buscaLinear(unidadeIdsCatalogo, nUnidades, unidadeIds[indice]);
                    printf("\nLeitura encontrada:\n\n");
                    printf("ID Leitura: %d\n", ids[indice]);
                    printf("Unidade: %d\n", unidadeIds[indice]);
                    printf("Descricao: %s\n", i == -1 ? "-" : descricoes[i]);
                    printf("Mes: %d\n", meses[indice]);
                    printf("Ano: %d\n", anos[indice]);
                    printf("Consumo: %.2f kWh\n", consumos[indice]);
                }
                break;

            case 3:
                printf("\nDigite o ID da unidade: ");
                if (scanf("%d", &idUnidade) != 1){
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }

                indice = buscaLinear(unidadeIdsCatalogo, nUnidades, idUnidade);
                if (indice == -1){
                    printf("\nUnidade nao encontrada.\n");
                    break;
                }

                total = calcularConsumoUnidade(unidadeIds, consumos, nLeituras, idUnidade);
                printf("\nConsumo da unidade:\n\n");
                printf("Unidade: %d\n", idUnidade);
                printf("Descricao: %s\n", descricoes[indice]);
                printf("Consumo total: %.2f kWh\n", total);
                break;

            case 4:
                mesInicial = 0;
                anoInicial = 0;
                mesFinal = 0;
                anoFinal = 0;

                printf("\nDigite o mes inicial: ");
                scanf("%d", &mesInicial);
                printf("Digite o ano inicial: ");
                scanf("%d", &anoInicial);
                printf("Digite o mes final: ");
                scanf("%d", &mesFinal);
                printf("Digite o ano final: ");
                scanf("%d", &anoFinal);

                if (mesInicial < 1 || mesInicial > 12 || mesFinal < 1 || mesFinal > 12 || anoInicial * 100 + mesInicial > anoFinal * 100 + mesFinal){
                    printf("\nPeriodo invalido.\n");
                    break;
                }

                total = calcularConsumoPeriodo(meses, anos, consumos, nLeituras, mesInicial, anoInicial, mesFinal, anoFinal);
                printf("\nConsumo no periodo:\n\n");
                printf("Periodo: %02d/%d ate %02d/%d\n", mesInicial, anoInicial, mesFinal, anoFinal);
                printf("Consumo total: %.2f kWh\n", total);
                break;

            case 5:
                unidadeMaior = encontrarUnidadeMaiorConsumo(unidadeIds, consumos, nLeituras);
                if (unidadeMaior == -1){
                    printf("\nNao ha leituras carregadas.\n");
                    break;
                }

                indice = buscaLinear(unidadeIdsCatalogo, nUnidades, unidadeMaior);
                total = calcularConsumoUnidade(unidadeIds, consumos, nLeituras, unidadeMaior);
                printf("\nUnidade com maior consumo:\n\n");
                printf("Unidade: %d\n", unidadeMaior);
                printf("Descricao: %s\n", indice == -1 ? "-" : descricoes[indice]);
                printf("Consumo total: %.2f kWh\n", total);
                break;

            case 6:
                printf("\nEncerrando o programa.\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 6);

    return 0;
}
