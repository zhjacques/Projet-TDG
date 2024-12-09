#ifndef PROJET_TDG_GRAPHE_H
#define PROJET_TDG_GRAPHE_H


#define MAX_NODES 20



typedef struct {
    float biomasse_kg;
    float flux_W;
    float co2_emis;
    float co2_absorbe;
    float capacite_portage;
    float degradation_habitat; // Nouveau champ pour la dégradation de l'habitat
} NodeProperties;

typedef struct {
    int adjacency_matrix[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][50];
    int node_count;
    NodeProperties node_properties[MAX_NODES];  // Tableau des propriétés des nœuds
} Graph;

// Lecture et affichage
int read_graph(const char *filename, Graph *graph);
void display_graph(const Graph *graph);
void display_successors(const Graph *graph, int node);
void display_predecessors(const Graph *graph, int node);
// Afficher un fichier DOT dans la console
void afficher_dot(const char *dot_filename);

void estimate_species_importance(Graph *graph);
void appliquer_degradation(Graph *graph, float facteur_degradation);
void predation_consommation(Graph graph, float facteur_predation);
// Vérification de connexité
int is_connected(const Graph *graph, int components[]);
// Recherche de sommets particuliers
void find_special_nodes(const Graph *graph);
void print_ancestors(const Graph *graph, int node, int visited[], int level);
void display_food_chains(const Graph *graph, const char *species_name);
// Exploration DFS
void dfs(const Graph *graph, int node, int visited[], int component_id, int components[]);
// Calcul des niveaux trophiques
void calculate_trophic_levels(const Graph *graph, int levels[]);

// Simulation de la disparition d'une espèce
void simulate_extinction(Graph *graph, const char *species_name);

// Dynamique des populations
void simulate_population_dynamics(Graph *graph, int initial_populations[], int time_steps);

#endif //PROJET_TDG_GRAPHE_H
