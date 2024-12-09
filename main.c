#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

int main(void) {
    Graph graph;
    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Charger un reseau et afficher ses listes d'adjacence\n");
        printf("2. Affichage du graphe\n");
        printf("3. Calculer et afficher les successeurs\n");
        printf("4. Calculer et afficher les predecesseurs\n");
        printf("5. Detecter les composantes connexes\n");
        printf("6. Analyser les sommets\n");
        printf("7. Calculer les niveaux trophiques\n");
        printf("8. Afficher les chaines alimentaires\n");
        printf("9. Degradation\n");
        printf("10. Predation\n");
        printf("11. Estimation de l'importance relative d'une espece au sein d'un reseau et simulation de la disparition d'une espece.\n");
        printf("12. Simuler la dynamique des populations\n");
        printf("13. Quitter\n");
        printf("Votre choix : ");
        int choix;
        scanf("%d", &choix);

        if (choix == 13) {
            break;
        }

        char filename[100];
        if (choix >= 1 && choix <= 12) {
            printf("Choisissez un reseau trophique a analyser :\n");
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
            }
            else if (choix == 2) {
                char dot_filename[100];
                printf("Entrez le nom du fichier DOT a afficher : ");
                scanf("%s", dot_filename);
                afficher_dot(dot_filename);
            }
            else if (choix == 3) {
                for (int i = 0; i < graph.node_count; i++) {
                    display_successors(&graph, i);
                }
            } else if (choix == 4) {
                for (int i = 0; i < graph.node_count; i++) {
                    display_predecessors(&graph, i);
                }
            } else if (choix == 5) {
                int components[MAX_NODES];
                int connected = is_connected(&graph, components);
                if (connected) {
                    printf("Le reseau est connexe.\n");
                } else {
                    printf("Le reseau n'est pas connexe. Composantes connexes :\n");
                    for (int i = 0; i < graph.node_count; i++) {
                        printf("Sommet %s dans la composante %d\n", graph.node_names[i], components[i]);
                    }
                }
            } else if (choix == 6) {
                find_special_nodes(&graph);
            } else if (choix == 7) {
                int levels[MAX_NODES];
                calculate_trophic_levels(&graph, levels);
                printf("\nNiveaux trophiques :\n");
                for (int i = 0; i < graph.node_count; i++) {
                    printf("%s : Niveau %d\n", graph.node_names[i], levels[i]);
                }
            } else if (choix == 8) {
                char species_name[50];
                printf("Entrez le nom de l'espece : ");
                scanf("%s", species_name);
                display_food_chains(&graph, species_name);
            } else if (choix == 9) {
                float degra;
                printf("Entrer le pourcentage souhaite : ");
                scanf("%f", &degra);
                printf("\n Degradation : ");
                appliquer_degradation(&graph, degra);
            } else if (choix == 10) {
                float preda;
                printf("Entrer le pourcentage souhaite : ");
                scanf("%f", &preda);
                predation_consommation(graph, preda);
            } else if (choix == 11) {
                printf("Entrez le nom de l'espece a simuler la disparition : ");
                char species_name[50];
                scanf("%s", species_name);
                simulate_extinction(&graph, species_name);
            } else if (choix == 12) {
                int initial_populations[MAX_NODES];
                printf("Entrez les populations initiales pour chaque espece :\n");
                for (int i = 0; i < graph.node_count; i++) {
                    printf("%s : ", graph.node_names[i]);
                    scanf("%d", &initial_populations[i]);
                }
                printf("Entrez le nombre d'etapes de temps pour la simulation : ");
                int time_steps;
                scanf("%d", &time_steps);
                simulate_population_dynamics(&graph, initial_populations, time_steps);
            }
        }
    }
    return 0;
}