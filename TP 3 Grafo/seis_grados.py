from grafo import Grafo
from collections import deque

###############################################
#AUXILIARES
###############################################

def pelicula_que_conecta(grafo, actor1, actor2):
    for w in grafo.obtener_adyacentes(actor1):
        if w.get_info() == actor2:
            return w.get_info()

def peliculas_de(grafo, actor):
    peliculas = []
    for w in grafo.obtener_adyacentes(actor):
        if w.get_info() not in peliculas:
            peliculas.append(w.get_info())
    return peliculas

def a_pasos_de(grafo, origen, llegada):
    padre, orden = grafo.bfs(origen)
    return orden[llegada]

def a_mas_pasos_de(grafo, origen, n):
    lista = []
    padre, orden = grafo.bfs(origen)
    for actor in orden:
        if orden >= n:
            lista.append(actor)
    return lista

###############################################
#FUNCIONES
###############################################

def grafo_crear(nombre_archivo):

    grafo = Grafo()

    peliculas = {}

    with open(nombre_archivo, encoding="utf8") as archivo:
        for linea in archivo:
            #Agregar vertice
            linea_act = linea.split(',')
            actor = linea_act[0]
            grafo.agregar_vertice(actor)
            #Agregar datos a peliculas{}
            for pelicula in linea_act:
                if pelicula == actor:
                    continue
                if pelicula not in peliculas:
                    peliculas[pelicula] = []
                else:
                    peliculas[pelicula].append(actor)
            #Agregar aristas
            for pelicula in peliculas:
                if pelicula in linea_act:
                    for actor_act in peliculas[pelicula]:
                        grafo.agregar_arista(actor_act, pelicula)

    return grafo



def camino(grafo, origen, llegada):
    camino = []
    llegada_act = llegada
    padre, orden = grafo.bfs(origen)
    if not padre[llegada_act]:
        return None
    while padre[llegada_act] != origen:
        camino.insert(0,(padre[llegada_act], llegada_act, pelicula_que_conecta(grafo, padre[llegada_act], llegada_act)))
    camino.reverse()
    return camino


def actores_a_distancia(grafo, origen, n):
    lista = []
    padre, orden = grafo.bfs(origen)
    for actor in orden:
        if orden[actor] == n:
            lista.append(actor)
    return lista


def popularidad(grafo, actor):
    popularidad = 0
    padre, orden = grafo.bfs(actor)
    for actor_act in orden:
        if orden[actor_act] == 2:
            popularidad += 1
    popularidad = popularidad*(peliculas_de(grafo, actor).len())
    return popularidad


def similares(grafo,origen, n):

    raise NotImplementedError

def cantidad_actores(grafo):
    actores = grafo.obtener_vertices()
    return len(actores)

def cantidad_aristas(grafo):
    aristas = grafo.obtener_aristas()