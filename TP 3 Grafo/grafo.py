###############################################
#CLASES
###############################################
from collections import deque

class Arista(object):

    def __init__(self, vertice, info = None):
        self.vertice = vertice
        self.info = info

    def __repr__(self):
        return str(self.info)

    def __str__(self):
        return str(self.info)

    def get_vertice(self):
        return self.vertice

    def get_info(self):
        return self.info

class Grafo (object):

    #Basicas

    def __init__(self):
        self.dicc = {}

    def __repr__(self):
        return str(self.dicc)

    def __str__(self):
        return str(self.dicc)

    #Funciones principales

    def agregar_vertice(self, vertice):
        self.dicc[vertice] = []

    def borrar_vertice(self, vertice):
        del self.dicc[vertice]

    def agregar_arista(self, vertice, arista):
        if(vertice in self.dicc):
            arista = Arista(vertice, arista)
            self.dicc[vertice].append(arista)

    def borrar_arista(self, vertice, arista):
        if(self.dicc[vertice].arista):
            self.dicc[vertice].remove(arista)

    def obtener_adyacentes(self, vertice):
        return self.dicc[vertice]

    def son_adyacentes(self, vertice1, vertice2):
        if vertice1 in self.dicc[vertice2]:
            return True
        return False

    def obtener_vertices(self):
        lista = []
        for key in self.dicc:
            lista.append(key)
        return lista

    def obtener_aristas(self):
        lista = []
        for key in self.dicc:
            for item in self.dicc[key]:
                if item not in lista:
                    lista.append(item)
        return lista

    def bfs(self, origen):
        visitados = {}
        padre = {}
        orden = {}
        q = deque()
        q.appendleft(origen)
        visitados[origen] = True
        orden[origen] = 0
        padre[origen] = None
        while q:
            v = q.pop()
            for wr in self.obtener_adyacentes(v):
                w = wr.get_vertice()
                if w not in visitados:
                    visitados[w] = True
                    padre[w] = v
                    orden[w] = orden[v] + 1
                    q.appendleft(w)
        return padre, orden