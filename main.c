//
// Created by gatsi on 07/12/2024.
//
#include <stdio.h>
#include <string.h>
#include "graphe.h"

int main() {
    Graph graph;
    char filename[100];
    int choice;

    printf("Choisissez un reseau trophique a analyser :\n");
    printf("1. Foret\n2. Marin\n3. Mangrove\n");
    printf("Entrez votre choix : ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\foret.txt");
            break;
        case 2:
            strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\marin.txt");
            break;
        case 3:
            strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\mangrove.txt");
            break;
        default:
            printf("Choix invalide.\n");
            return 1;
    }

    if (read_graph(filename, &graph)) {
        display_graph(&graph);
        printf("\n--- Analyse du Graphe ---\n");
        for (int i = 0; i < graph.node_count; i++) {
            display_successors(&graph, i);
            display_predecessors(&graph, i);
        }
        if (is_connected(&graph)) {
            printf("Le reseau est connexe.\n");
        } else {
            printf("Le reseau n'est pas connexe.\n");
        }
        char dot_filename[100];
        printf("\nEntrez le nom du fichier DOT a generer (par ex. reseau.dot) : ");
        scanf("%s", dot_filename);
        generate_dot_file(&graph, dot_filename);
        find_special_nodes(&graph);

        // Calcul des niveaux trophiques
        int levels[MAX_NODES];
        calculate_trophic_levels(&graph, levels);
        printf("\nNiveaux trophiques :\n");
        for (int i = 0; i < graph.node_count; i++) {
            printf("%s : Niveau %d\n", graph.node_names[i], levels[i]);
        }

        // Simulation de l'extinction
        char species_name[50];
        printf("\nEntrez le nom d'une espece a supprimer : ");
        scanf("%s", species_name);
        simulate_extinction(&graph, species_name);

        // Vérifier l'impact après extinction
        printf("\n--- Analyse apres extinction ---\n");
        if (is_connected(&graph)) {
            printf("Le reseau reste connexe.\n");
        } else {
            printf("Le reseau n'est plus connexe.\n");
        }
        find_special_nodes(&graph);
    } else {
        printf("Impossible de charger le graphe depuis %s.\n", filename);
    }

    return 0;

}
