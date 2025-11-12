#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// NÍVEL NOVATO: Mapa da Mansão com Árvore Binária
// ============================================================================

// Estrutura para representar uma sala
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função para criar uma nova sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Função para explorar as salas
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        printf("Você chegou a um beco sem saída!\n");
        return;
    }

    char escolha;
    printf("\n====================================\n");
    printf("Você está em: %s\n", salaAtual->nome);
    printf("====================================\n");

    if (salaAtual->esquerda != NULL) {
        printf("(e) Ir para esquerda: %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("(d) Ir para direita: %s\n", salaAtual->direita->nome);
    }
    printf("(s) Sair da exploração\n");
    printf("Escolha: ");
    scanf(" %c", &escolha);

    if (escolha == 'e' && salaAtual->esquerda != NULL) {
        explorarSalas(salaAtual->esquerda);
    } else if (escolha == 'd' && salaAtual->direita != NULL) {
        explorarSalas(salaAtual->direita);
    } else if (escolha == 's') {
        printf("Saindo da exploração...\n");
        return;
    } else {
        printf("Opção inválida!\n");
        explorarSalas(salaAtual);
    }
}

// ============================================================================
// NÍVEL AVENTUREIRO: Armazenamento de Pistas com BST
// ============================================================================

// Estrutura para representar uma pista
typedef struct Pista {
    char texto[100];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// Função para inserir uma pista na BST
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) {
        Pista* novaPista = (Pista*)malloc(sizeof(Pista));
        strcpy(novaPista->texto, texto);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        printf("✓ Pista coletada: %s\n", texto);
        return novaPista;
    }

    int comparacao = strcmp(texto, raiz->texto);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    } else {
        printf("Esta pista já foi coletada!\n");
    }

    return raiz;
}

// Função para listar pistas em ordem alfabética
void listarPistas(Pista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("  • %s\n", raiz->texto);
        listarPistas(raiz->direita);
    }
}

// ============================================================================
// NÍVEL MESTRE: Relacionamento de Pistas com Suspeitos via Hash
// ============================================================================

#define TAM_HASH 10

// Estrutura para lista encadeada de pistas de um suspeito
typedef struct NoPista {
    char pista[100];
    struct NoPista* proximo;
} NoPista;

// Estrutura para representar um suspeito
typedef struct Suspeito {
    char nome[50];
    NoPista* pistas;
    int contador;
    struct Suspeito* proximo;
} Suspeito;

// Tabela hash
typedef struct {
    Suspeito* tabela[TAM_HASH];
} TabelaHash;

// Função hash simples usando soma dos valores ASCII
int funcaoHash(const char* nome) {
    int hash = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        hash += nome[i];
    }
    return hash % TAM_HASH;
}

// Inicializar a tabela hash
void inicializarHash(TabelaHash* th) {
    for (int i = 0; i < TAM_HASH; i++) {
        th->tabela[i] = NULL;
    }
}

// Inserir associação entre pista e suspeito
void inserirHash(TabelaHash* th, const char* pista, const char* nomeSuspeito) {
    int indice = funcaoHash(nomeSuspeito);

    // Procurar suspeito na lista encadeada
    Suspeito* atual = th->tabela[indice];
    Suspeito* anterior = NULL;

    while (atual != NULL && strcmp(atual->nome, nomeSuspeito) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se o suspeito não existe, criar novo
    if (atual == NULL) {
        atual = (Suspeito*)malloc(sizeof(Suspeito));
        strcpy(atual->nome, nomeSuspeito);
        atual->pistas = NULL;
        atual->contador = 0;
        atual->proximo = NULL;

        if (anterior == NULL) {
            th->tabela[indice] = atual;
        } else {
            anterior->proximo = atual;
        }
    }

    // Adicionar pista ao suspeito
    NoPista* novaPista = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novaPista->pista, pista);
    novaPista->proximo = atual->pistas;
    atual->pistas = novaPista;
    atual->contador++;

    printf("✓ Pista \"%s\" associada ao suspeito %s\n", pista, nomeSuspeito);
}

// Buscar suspeito na tabela hash
Suspeito* buscarSuspeito(TabelaHash* th, const char* nome) {
    int indice = funcaoHash(nome);
    Suspeito* atual = th->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }

    return NULL;
}

// Listar todas as associações
void listarAssociacoes(TabelaHash* th) {
    printf("\n========================================\n");
    printf("RELATÓRIO DE SUSPEITOS E PISTAS\n");
    printf("========================================\n");

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* atual = th->tabela[i];

        while (atual != NULL) {
            printf("\nSUSPEITO: %s (Pistas: %d)\n", atual->nome, atual->contador);
            printf("Pistas encontradas:\n");

            NoPista* pistaAtual = atual->pistas;
            while (pistaAtual != NULL) {
                printf("  • %s\n", pistaAtual->pista);
                pistaAtual = pistaAtual->proximo;
            }

            atual = atual->proximo;
        }
    }
}

// Encontrar o suspeito mais provável
void encontrarSuspeitoMaisProvavel(TabelaHash* th) {
    char nomeMaisProvavel[50] = "";
    int maxPistas = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* atual = th->tabela[i];

        while (atual != NULL) {
            if (atual->contador > maxPistas) {
                maxPistas = atual->contador;
                strcpy(nomeMaisProvavel, atual->nome);
            }
            atual = atual->proximo;
        }
    }

    if (maxPistas > 0) {
        printf("\n========================================\n");
        printf("🔍 SUSPEITO MAIS PROVÁVEL: %s\n", nomeMaisProvavel);
        printf("   Total de pistas: %d\n", maxPistas);
        printf("========================================\n");
    } else {
        printf("\nNenhum suspeito encontrado!\n");
    }
}

// ============================================================================
// FUNÇÃO PRINCIPAL E MENU
// ============================================================================

void exibirMenu() {
    printf("\n========================================\n");
    printf("    DETECTIVE QUEST - MENU PRINCIPAL\n");
    printf("========================================\n");
    printf("1. Explorar Mansão (Nível Novato)\n");
    printf("2. Gerenciar Pistas (Nível Aventureiro)\n");
    printf("3. Investigar Suspeitos (Nível Mestre)\n");
    printf("4. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opção: ");
}

int main() {
    // Criar o mapa da mansão (Árvore Binária)
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    hall->esquerda->esquerda = criarSala("Escritório");
    hall->esquerda->direita = criarSala("Sala de Leitura");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Sótão");

    // Inicializar BST de pistas
    Pista* pistasColetadas = NULL;

    // Inicializar tabela hash de suspeitos
    TabelaHash tabelaSuspeitos;
    inicializarHash(&tabelaSuspeitos);

    int opcao;
    int subOpcao;
    char textoPista[100];
    char nomeSuspeito[50];

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      BEM-VINDO AO DETECTIVE QUEST      ║\n");
    printf("║   Resolva o mistério da mansão!        ║\n");
    printf("╚════════════════════════════════════════╝\n");

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: // Explorar Mansão
                printf("\n🏚️  Iniciando exploração da mansão...\n");
                explorarSalas(hall);
                break;

            case 2: // Gerenciar Pistas
                printf("\n========================================\n");
                printf("    GERENCIAMENTO DE PISTAS\n");
                printf("========================================\n");
                printf("1. Adicionar pista\n");
                printf("2. Listar pistas coletadas\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("Digite a pista: ");
                    getchar(); // Limpar buffer
                    fgets(textoPista, sizeof(textoPista), stdin);
                    textoPista[strcspn(textoPista, "\n")] = 0; // Remover \n
                    pistasColetadas = inserirPista(pistasColetadas, textoPista);
                } else if (subOpcao == 2) {
                    printf("\n📋 Pistas coletadas (em ordem alfabética):\n");
                    listarPistas(pistasColetadas);
                }
                break;

            case 3: // Investigar Suspeitos
                printf("\n========================================\n");
                printf("    INVESTIGAÇÃO DE SUSPEITOS\n");
                printf("========================================\n");
                printf("1. Associar pista a suspeito\n");
                printf("2. Listar todas as associações\n");
                printf("3. Encontrar suspeito mais provável\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);

                if (subOpcao == 1) {
                    printf("Digite a pista: ");
                    getchar();
                    fgets(textoPista, sizeof(textoPista), stdin);
                    textoPista[strcspn(textoPista, "\n")] = 0;

                    printf("Digite o nome do suspeito: ");
                    fgets(nomeSuspeito, sizeof(nomeSuspeito), stdin);
                    nomeSuspeito[strcspn(nomeSuspeito, "\n")] = 0;

                    inserirHash(&tabelaSuspeitos, textoPista, nomeSuspeito);
                } else if (subOpcao == 2) {
                    listarAssociacoes(&tabelaSuspeitos);
                } else if (subOpcao == 3) {
                    encontrarSuspeitoMaisProvavel(&tabelaSuspeitos);
                }
                break;

            case 4:
                printf("\n👋 Obrigado por jogar Detective Quest!\n");
                break;

            default:
                printf("\n❌ Opção inválida!\n");
        }

    } while (opcao != 4);

    // Liberar memória (simplificado - em produção seria mais completo)
    // Aqui você adicionaria funções para liberar toda a memória alocada

    return 0;
}
