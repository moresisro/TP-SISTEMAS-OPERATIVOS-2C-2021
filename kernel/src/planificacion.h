/*
 * planificacion.h
 *
 *  Created on: 2 oct. 2021
 *      Author: utnso
 */

#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <futiles.h>
#include <sys/time.h>


//ESTRUCTURAS

typedef struct {
	char* nombre;
	uint32_t cantidad;
}t_registro_uso_recurso;

typedef struct {
	uint32_t PID;
	unsigned long real_anterior;
	double estimado_anterior;
	uint32_t tiempo_espera;
	int conexion;
	t_list* recursos_usados;
}PCB;

typedef enum{
	MATE_SUCCESS = 1,
	MATE_FREE_FAULT = -5,
	MATE_READ_FAULT = -6,
	MATE_WRITE_FAULT = -7
}MATE_RETURNS;

typedef struct {
    char* ip_memoria;
    char* puerto_memoria;
    char* ip_kernel;
    char* puerto_kernel;
    char* alg_plani;
    int estimacion_inicial;
    double alfa;
    char** dispositivos_IO;
    char** duraciones_IO;
    int tiempo_deadlock;
    int grado_multiprogramacion;
	int grado_multiprocesamiento;
}t_kernel_config;

typedef struct {
	PCB* lugar_PCB;
	sem_t sem_exec;
	int bit_de_ocupado;
	struct timeval timeValBefore;
	struct timeval timeValAfter;
}t_procesador;

typedef struct {
	char* nombre;
	int value;
	t_list* cola_bloqueados;
}t_semaforo_mate;

typedef struct {
	char* nombre;
	uint32_t rafaga;
	sem_t sem;
	t_list* cola_espera;
}t_dispositivo;

//VARIABLES GLOBALES
t_list_iterator* iterador_lista_ready;
t_kernel_config CONFIG_KERNEL;
t_log* LOGGER;

//SEMAFOROS
sem_t sem_algoritmo_planificador_largo_plazo;
sem_t sem_cola_ready;
sem_t sem_grado_multiprogramacion;
sem_t sem_grado_multiprocesamiento;

//LISTAS
t_list* LISTA_NEW;
t_list* LISTA_READY;
t_list* LISTA_EXEC;
t_list* LISTA_BLOCKED;
t_list* LISTA_BLOCKED_SUSPENDED;
t_list* LISTA_READY_SUSPENDED;
t_list* LISTA_PROCESADORES;
t_list* LISTA_SEMAFOROS_MATE;
t_list* LISTA_DISPOSITIVOS_IO;

//MUTEXES
pthread_mutex_t mutex_lista_new;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_lista_exec;
pthread_mutex_t mutex_lista_blocked;
pthread_mutex_t mutex_lista_blocked_suspended;
pthread_mutex_t mutex_lista_ready_suspended;
pthread_mutex_t mutex_lista_procesadores;
pthread_mutex_t mutex_lista_semaforos_mate;
pthread_mutex_t mutex_lista_dispositivos_io;

//HILOS
pthread_t planificador_largo_plazo;
pthread_t planificador_corto_plazo;
pthread_t administrador_deadlock;

void algoritmo_planificador_largo_plazo();
void algoritmo_planificador_mediano_plazo_ready_suspended();
void algoritmo_planificador_mediano_plazo_blocked_suspended();
void algoritmo_planificador_corto_plazo();
void correr_dispatcher(PCB* pcb);
void correr_algoritmo_deadlock();
PCB* algoritmo_SJF();
PCB* algoritmo_HRRN();
void* minimum(PCB*, PCB*);
bool criterio_remocion_lista(void* pcb);
void ejecutar(t_procesador*);
void recibir_peticion_para_continuar(int conexion);
void dar_permiso_para_continuar(int conexion);
void avisar_finalizacion_por_deadlock(int conexion);
void init_sem(t_procesador*);
void wait_sem(t_procesador*);
void post_sem(t_procesador*);
void destroy_sem(t_procesador*);
void call_IO(t_procesador*);
void matar_dispositivo_io();
void matar_procesador();
void ejecutar_io(t_dispositivo*);
void memalloc(t_procesador*);
void memread(t_procesador*);
void memfree(t_procesador*);
void memwrite(t_procesador*);
void mate_close(t_procesador*);
int valor_matriz (int, int, t_list*);
int cantidad_de_columnas_matriz();
void matar_algoritmo_deadlock();
void algoritmo_deteccion_deadlock();
void avisar_suspension_a_memoria(PCB* pcb);
void avisar_dessuspension_a_memoria(PCB* pcb);
void avisar_close_a_memoria(PCB* pcb);

#endif /* PLANIFICACION_H_ */
