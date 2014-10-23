byte univers[] ={0x19}; // Vous pouvez choisir l'univers que vous voulez entre 0 et 255. // Univers 1 = 1 et non 0... enfin ca depend.
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x45, 0x7F}; // Inscrivez l'adresse MAC de votre carte.
byte ip[] = {10, 0, 0, 74}; // L'adresse IP que vous souhaitez lui adresser, la norme ART-NET précise qu'il faut rester dans un réseau de type 2.xxx.xxx.xxx ou 10.xxx.xxx.xxx.
unsigned char ip2[4] ={255, 255, 255, 255}; // NE PAS MODIFIER, ici on indique simplement au programme d'envoyer des réponses en broadcast afin que les autres machines détectent notre noeud.
byte gateway[] = {10, 0, 0, 1}; // On définie l'adresse IP de la passerelle du réseau, si vous passez dans un réseau du type 10.xxx.xxx.xxx n'oubliez pas de modifier celle-ci par 10.0.0.1.
byte subnet[] = {255, 0, 0, 0}; // NE PAS MODIFIER, c'est le masque du sous-réseau d'un réseau de classe A (ce qu'on utilise).
unsigned int localPort = 6454; // NE PAS MODIFIER, c'est le port utilisé par défaut par la norme ART-NET.

#define LOOP_TEMPO 40

// How many leds are in the strip?
#define NUM_LEDS 128

// Data pin that led data will be written out over
#define DATA_PIN 6

#define TEST_PIN 8




