#include <stdio.h>
#include <stdlib.h>   
#include <math.h>     
#include <time.h> 

#define pi 3.14159265358979323846

typedef struct {
    double *posicion;
    double *velocidad;
    double *mejorPosicion;
    double fitness;
    double mejorFitness;
} Particula;

 //////////////////////////

typedef struct {
    int iteracion;
    double fitnessG;
    double *mejorPosicionGlobal;
}HistorialdeRegistroB;


double sphere(double *x, int dim){
    double sum=0.0;
    for(int i=0; i<dim; i++){
        sum+= pow(x[i], 2);
    }
    return sum;
}

double rastrigin(double *x, int dim) {
    double sum = 10.0 * dim;
    for(int i = 0; i < dim; i++) {
        sum += (pow(x[i], 2) - 10.0 * cos(2.0 * pi * x[i])); 
    }
    return sum;
}

double rosenbrock(double *x, int dim) {
    double sum = 0.0;
    for(int i = 0; i < dim - 1; i++) {
        double term1 = 100.0 * pow(x[i+1] - pow(x[i], 2), 2);
        double term2 = pow(1.0 - x[i], 2);
        sum += term1 + term2;
    }
    return sum;
}

double r1_r2(){
    return (double)rand()/RAND_MAX;
}

double rango(double min, double max){
    return min + r1_r2() * (max - min);
}





void PSO(double(*funcion)(double*, int), int dimensiones, int nParticulas, int iteraciones, double minRango, double maxRango){
    double c1=0.7, c2=0.3, w=0.8;

    Particula *enjambre = (Particula*)malloc(nParticulas * sizeof(Particula));

    if(enjambre == NULL){
        exit (1);
    }

    double *mejorPosicionGlobal = (double*)malloc(sizeof(double) * dimensiones);

    if(mejorPosicionGlobal == NULL){
        exit (1);
    }

    double mejorFitnessGlobal = INFINITY;


    double min = minRango;
    double max = maxRango;


    for(int i=0; i<nParticulas; i++){
        enjambre[i].posicion = (double*)malloc(sizeof(double) * dimensiones);
        enjambre[i].velocidad = (double*)malloc(sizeof(double) * dimensiones);
        enjambre[i].mejorPosicion = (double*)malloc(sizeof(double) * dimensiones);

        if((enjambre[i].posicion==NULL) || (enjambre[i].velocidad==NULL) || (enjambre[i].mejorPosicion==NULL)){
            exit (1);
        }

        for(int j=0; j<dimensiones; j++){
            enjambre[i].posicion[j]=rango(min, max);
            enjambre[i].velocidad[j]=r1_r2();
            enjambre[i].mejorPosicion[j]=enjambre[i].posicion[j];
        }

        enjambre[i].fitness=funcion(enjambre[i].posicion, dimensiones);
        enjambre[i].mejorFitness=enjambre[i].fitness;

        if(enjambre[i].fitness<mejorFitnessGlobal){
            mejorFitnessGlobal=enjambre[i].fitness;
            for(int j=0; j<dimensiones; j++){
                mejorPosicionGlobal[j] = enjambre[i].posicion[j];
            }
            
        }
    }



    FILE *archivo = fopen("historial.bin", "wb");
    if(archivo == NULL){
        exit (1);
    }

    for(int t=0; t<iteraciones; t++){
       for(int i = 0; i < nParticulas; i++){

            for(int j = 0; j < dimensiones; j++){

                enjambre[i].velocidad[j] = (w * enjambre[i].velocidad[j]) + (c1 * r1_r2() * (enjambre[i].mejorPosicion[j] - enjambre[i].posicion[j])) + (c2 * r1_r2() * (mejorPosicionGlobal[j] - enjambre[i].posicion[j]));

                enjambre[i].posicion[j] = enjambre[i].posicion[j] + enjambre[i].velocidad[j];
            } 

            enjambre[i].fitness = funcion(enjambre[i].posicion, dimensiones);

            if(enjambre[i].fitness < enjambre[i].mejorFitness){
                enjambre[i].mejorFitness = enjambre[i].fitness;
                for(int j = 0; j < dimensiones; j++){
                    enjambre[i].mejorPosicion[j] = enjambre[i].posicion[j];
                }
            }

            if(enjambre[i].fitness < mejorFitnessGlobal){
                mejorFitnessGlobal = enjambre[i].fitness;
                for(int j = 0; j < dimensiones; j++){
                    mejorPosicionGlobal[j] = enjambre[i].posicion[j];
                }
            }

        }

        HistorialdeRegistroB registro;
        registro.iteracion = t;
        registro.fitnessG = mejorFitnessGlobal;

        fwrite(&registro, sizeof(HistorialdeRegistroB), 1, archivo);
        fwrite(mejorPosicionGlobal, sizeof(double), dimensiones, archivo);

    }

    fclose(archivo);
    
    for(int i=0; i<nParticulas; i++){
        free(enjambre[i].posicion);
        free(enjambre[i].velocidad);
        free(enjambre[i].mejorPosicion);
    }

    free(enjambre);
    free(mejorPosicionGlobal);
}


void leerHistorial(int dimensiones) {
    FILE *archivo = fopen("historial.bin", "rb");
    if (archivo == NULL) {
        printf("Error");
        return;
    }

    HistorialdeRegistroB reg;
    double *posTemporales = (double*)malloc(sizeof(double) * dimensiones);

    printf("ESTO ES LO QUE SE ESTÁ GUARDANDO EN EL ARCHIVO BINARIOOO\n");
printf("%-12s | %-20s | %-s\n", "Iteracion", "Mejor Fitness G", "Mejor Posicion Global\n");
    printf("\n");

    while (fread(&reg, sizeof(HistorialdeRegistroB), 1, archivo) == 1) {
        fread(posTemporales, sizeof(double), dimensiones, archivo);

        printf("Iteracion %-3d | %-20.6f | [", reg.iteracion, reg.fitnessG);
        
        for (int j = 0; j < dimensiones; j++) {
            printf("%.4f", posTemporales[j]);
            if (j < dimensiones - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
    printf("\n");

    free(posTemporales);
    fclose(archivo);
}

int main(){

    srand(time(NULL));

    int dimensiones = 10;
    int particulas = 50;
    int iteraciones = 100;

    PSO(sphere, dimensiones, particulas, iteraciones, -100, 100);
    //PSO(rastrigin, dimensiones, particulas, iteraciones, -5.12, 5.12);
    //PSO(rosenbrock, dimensiones, particulas, iteraciones, -2.048, 2.048);

    leerHistorial(dimensiones);

    return 0;
}