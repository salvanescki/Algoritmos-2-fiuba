import sys
import seis_grados
KBN_INFINITO = -1

#Auxiliares
def leer_archivo_inicial(nombre):
  return seis_grados.grafo_crear(nombre)

#Funciones principales
def camino_hasta_KB(grafo, origen):
    camino = seis_grados.camino(grafo, origen, "Bacon Kevin")
    for paso in camino:
        print("{0} actuo con {1} en {2}", paso[0], paso[1], paso[2])

def bacon_number(grafo, llegada):
    n = seis_grados.a_pasos_de(grafo, "Bacon Kevin", llegada)
    print("{0} tiene un Kevin Bacon Number igual a {1}", llegada, n)

def bacon_number_mayor_a_6(grafo):
    i = 6
    lista = seis_grados.actores_a_distancia(grafo, "Bacon Kevin", i)
    while lista:
        print("Con KBN igual a {}: {} actores", i, len(lista))
        i+=1
        lista = seis_grados.actores_a_distancia(grafo, "Bacon Kevin", i)
    if len(lista) == 0:
        print("No hay actores con KBN igual a {}")
    

def bacon_number_infinito(grafo):
    lista = seis_grados.actores_a_distancia(grafo, "Bacon Kevin", KBN_INFINITO)
    if(len(lista) == 0):
        print("No existen actores con KBN infinito")
    print("Los actores con un Bacon Number infinito son {}", len(lista))

def KBN_promedio(grafo):
    raise NotImplementedError

def similares_a_KB(grafo, n):
    lista_similares = seis_grados.similares(grafo, "Bacon Kevin", n)
    print('Los {0} actores más similares KB son {1}', n, lista_similares)

def popularidad_contra_KB(grafo, actor):
    if not actor:
        print("El actor no existe")
        return None
    actor_popularidad = seis_grados.popularidad(grafo,actor)
    KB_popularidad = seis_grados.popularidad(grafo, "Bacon Kevin")
    popularidad = (actor_popularidad * 100) / popularidad_contra_KB
    print("{} es un {}% de lo popular que es Kevin Bacon", actor, popularidad)


def cantidad_peliculas(grafo):
    print('El dataset contiene {} peliculas'.format(cantidad_peliculas(grafo)))

def cantidad_actores(grafo):
    print('El dataset contiene {} actores'.format(cantidad_actores(grafo)))

#Interfaz y Main
def interfaz(grafo):
    line = input()
    while(line):
        datos = line.rstrip("\n").split(' ', 1)
        if len(datos) == 2:
            actor = datos[1].strip("'")
        if datos[0] == 'camino_hasta_KB':
            camino_hasta_KB(grafo, actor)
        elif datos[0] == 'bacon_number':
            bacon_number(grafo, actor)
        elif datos[0] == 'bacon_number_mayor_a_6':
            print('bacon_number_mayor_a_6')
        elif datos[0] == 'bacon_number_infinito':
            print('bacon_number_infinito')
        elif datos[0] == 'KBN_promedio':
            print('KBN_promedio')
        elif datos[0] == 'similares_a_KB':
            similares_a_KB(grafo, actor)
        elif datos[0] == 'popularidad_contra_KB':
            popularidad_contra_KB(grafo, actor)
        elif datos[0] == 'cantidad_peliculas':
            cantidad_peliculas(grafo)
        elif datos[0] == 'cantidad_actores':
            cantidad_actores(grafo)
        else:
            return 1
        line = input()

def main():
    print("Comienzo grafo")
    nombre = sys.argv[1]
    grafo = leer_archivo_inicial(nombre)
    print("Terminado grafo, comienzo interfaz")
    interfaz(grafo)

#Ejecucion
main()