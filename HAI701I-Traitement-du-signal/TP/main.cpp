#include "wave.cpp"
#include <iostream>

double Char2Double(unsigned char val)
{
    double reel;
    reel = (double)val / 127.5;
    return reel - 1.0;
}

unsigned char Double2Char(double val)
{
    char caractere;
    val = (val + 1.0) * 127.5;
    val = val < 0.0 ? 0.0 : val;
    val = val > 255.0 ? 255.0 : val;
    return (unsigned char)floor(val);
}

int Creation_note(double freq, double freq_ech, int nb_ech, double *data_real, unsigned char *data_char) // Frequence de la note, plage frequentielle de l'échantillon, nombre d'echantillon, le tab qui stocke
{
    double alpha;
    int k;
    alpha = 2.0 * M_PI * freq / freq_ech;
    for (k = 0; k < nb_ech; k++)
    {
        data_real[k] = sin((double)k * alpha);
        data_char[k] = Double2Char(data_real[k]);
    }
    return 1;
}
// C'est a chaque lecture ou ecriture qu'on fait des conversion char -> double et double -> char pour optimiser la mémoire
int main()
{
    std::cout << "Starting main" << std::endl;

    // Varaibles pour le la
    double la = 440.0; // La fréquence du la;
    double freq = 44500.0;
    int N;
    double temps = 6.0;                               // Temps en secondes
    double *reel_data;                                //Tableau des datas réelles
    unsigned char *data;                              // Tableau des datas en char
    N = (int)floor(temps * freq);                     // Nombre d'échantillons
    reel_data = (double *)malloc(N * sizeof(double)); // Allouer la mémoire necessaire
    data = (unsigned char *)malloc(N * sizeof(data)); // Allouer la mémoire necessaire

    Creation_note(la, freq, N, reel_data, data); // On rempli reel_data avec les valeures créé pour la note

    Wave wave = Wave(data, N, 1, freq);

    wave.write("La1.wav");
    free(reel_data);
    free(data);
}