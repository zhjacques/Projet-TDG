//
// Created by gatsi on 07/12/2024.
//

#ifndef PROJET_TDG_GRAPHE_H
#define PROJET_TDG_GRAPHE_H


#define MAX_NODES 20

typedef struct {
    int adjacency_matrix[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][50];
    int node_count;
} Graph;

// Lecture et affichage
int read_graph(const char *filename, Graph *graph);
void display_graph(const Graph *graph);

// Vérification de connexité
int is_connected(const Graph *graph);

// Recherche de sommets particuliers
void find_special_nodes(const Graph *graph);

// Exploration DFS
void dfs(const Graph *graph, int node, int visited[]);

// Calcul des niveaux trophiques
void calculate_trophic_levels(const Graph *graph, int levels[]);

// Simulation de la disparition d'une espèce
void simulate_extinction(Graph *graph, const char *species_name);

// Dynamique des populations
void simulate_population_dynamics(Graph *graph, int initial_populations[], int time_steps);



#endif //PROJET_TDG_GRAPHE_H
