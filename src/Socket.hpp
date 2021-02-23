// Ce document décrit le type des données reçues par le PMC :

struct genericPayload{
  int freq; // Si c'est 0, clk disabled
};

// freq : fréquence en Hz pour les périphériques GPIO et USART. Une socket
// par instance de GPIO et USART.

struct timerPayload{
  int freq[3];
};

// freq[3] : tableau de fréquence en Hz pour les 3 canaux du timer. La
// position dans le tableau correspond au numéro de canal de manière croissante.
// Une socket par instance de TIMER.
