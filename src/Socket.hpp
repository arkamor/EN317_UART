// Ce document décrit le type des données reçues par le PMC :

typedef struct genericPayload{
  int freq;
  int MCK;
}GenericPayload;

// freq : fréquence en Hz pour les périphériques GPIO et USART. Une socket
// par instance de GPIO et USART.
// MCK : fréquence de l'horloge MCK en Hz

typedef struct timerPayload{
  int freq[3];
  int MCK;
}TimerPayload;

// freq[3] : tableau de fréquence en Hz pour les 3 canaux du timer. La
// position dans le tableau correspond au numéro de canal de manière croissante.
// Une socket par instance de TIMER.
// MCK : fréquence de l'horloge MCK en Hz
