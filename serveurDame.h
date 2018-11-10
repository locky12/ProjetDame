//Prototypes

// Permet de lire un message d'un client
int read_player(int socket_player, char *buffer);
// Permet d'envoyer un message à un client
void send_player(int socket_player,char *buffer);
// Permet d'envoyer un message à plusieurs clients.
void send_all_player(int *listPlayer,int playerSend ,char *buffer,int size);
// Accepte un client.
int accept_player (int socket_ecoute);
// Permet de lier l'adresse serveur au socket d'écoute
// retourne le socket_ecoute.
int listen_net (int port);

void server(int port);
