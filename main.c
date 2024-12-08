#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

int main(void) {
    Graph graph;
    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Charger un reseau et afficher ses listes d'adjacence\n");
        printf("2. Calculer et afficher les successeurs\n");
        printf("3. Calculer et afficher les predecesseurs\n");
        printf("4. Détecter les composantes connexes\n");
        printf("5. Analyser les sommets\n");
        printf("6. Calculer les niveaux trophiques\n");
        printf("7. Afficher les chaînes alimentaires\n");
        printf("8. Degradation\n");
        printf("9. Predation\n");
        printf("10. Estimation de l'importance relative d'une espèce au sein d'un reseau et simulation de la disparition d'une espece.\n");
        printf("11. Quitter\n");
        printf("Votre choix : ");
        int choix;
        scanf("%d", &choix);

        if (choix == 11) {
            break;
        }

        char filename[100];
        if (choix >= 1 && choix <= 10) {
            printf("Choisissez un réseau trophique à analyser :\n");
            printf("1. Foret\n2. Marin\n3. Mangrove\n");
            printf("Entrez votre choix : ");
            int reseau;
            scanf("%d", &reseau);

            switch (reseau) {
                case 1:
                    strcpy(filename, "../foret.txt");
                    break;
                case 2:
                    strcpy(filename, "../marin.txt");
                    break;
                case 3:
                    strcpy(filename, "../mangrove.txt");
                    break;
                default:
                    printf("Choix invalide.\n");
                    continue;
            }

            if (!read_graph(filename, &graph)) {
                printf("Erreur lors de la lecture du fichier.\n");
                continue;
            }

            if (choix == 1) {
                display_graph(&graph);
            } else if (choix == 2) {
                for (int i = 0; i < graph.node_count; i++) {
                    display_successors(&graph, i);
                }
            } else if (choix == 3) {
                for (int i = 0; i < graph.node_count; i++) {
                    display_predecessors(&graph, i);
                }
            } else if (choix == 4) {
                int components[MAX_NODES];
                int connected = is_connected(&graph, components);
                if (connected) {
                    printf("Le réseau est connexe.\n");
                } else {
                    printf("Le réseau n'est pas connexe. Composantes connexes :\n");
                    for (int i = 0; i < graph.node_count; i++) {
                        printf("Sommet %s dans la composante %d\n", graph.node_names[i], components[i]);
                    }
                }
            } else if (choix == 5) {
                find_special_nodes(&graph);
            } else if (choix == 6) {
                int levels[MAX_NODES];
                calculate_trophic_levels(&graph, levels);
                printf("\nNiveaux trophiques :\n");
                for (int i = 0; i < graph.node_count; i++) {
                    printf("%s : Niveau %d\n", graph.node_names[i], levels[i]);
                }
            } else if (choix == 7) {
                // Code pour afficher les chaînes alimentaires
            } else if (choix == 8) {
                float degra;
                printf("Entrer le pourcentage souhaité : ");
                scanf("%f", &degra);
                printf("\n Degradation : ");
                appliquer_degradation(&graph, degra);
            } else if (choix == 9) {
                float preda;
                printf("Entrer le pourcentage souhaité : ");
                scanf("%f", &preda);
                predation_consommation(&graph, preda);
                display_graph(&graph);
            } else if (choix == 10) {
                // Code pour l'estimation de l'importance et la simulation de la disparition
            }
        }
    }
    return 0;
}
