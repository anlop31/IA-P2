#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct stateN0
{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x) const
  {
    if (jugador == x.jugador and
        colaborador.f == x.colaborador.f and
        colaborador.c == x.colaborador.c)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
};

struct nodeN0
{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &n) const
  {
    return (st == n.st);
  }

  bool operator<(const nodeN0 &b) const
  {
    if (st.jugador.f < st.jugador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula < b.st.jugador.brujula)
      return true;
    else
      return false;
  }
};

// Nivel 1 (anchura colaborador)
struct stateN1
{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  
  bool operator==(const stateN1 &x) const
  {
    if (jugador == x.jugador and colaborador == x.colaborador)
      return true;
    else
      return false;
  }
};

struct nodeN1
{
  stateN1 st;
  list<Action> secuencia;
  bool operator==(const nodeN1 &b) const {
    return (st == b.st);
  }

  bool operator<(const nodeN1 &b) const {
    if (st.jugador.f < b.st.jugador.f) 
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula < b.st.jugador.brujula)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula == b.st.jugador.brujula and
             st.colaborador.f < b.st.colaborador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula == b.st.jugador.brujula and
             st.colaborador.f == b.st.colaborador.f and
             st.colaborador.c < b.st.colaborador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula == b.st.jugador.brujula and
             st.colaborador.f == b.st.colaborador.f and
             st.colaborador.c == b.st.colaborador.c and
             st.colaborador.brujula < b.st.colaborador.brujula)
      return true;
    else
      return false;
  }
};

// Nivel 2 (dijsktra)
struct stateN2
{
  ubicacion jugador;
  bool zapatillas_jug;
  bool bikini_jug;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  int coste;

  bool operator==(const stateN2 &x) const
  {
    if (jugador == x.jugador and               // misma ubicación
        colaborador.f == x.colaborador.f and   // misma fila colaborador
        colaborador.c == x.colaborador.c and   // misma columna colaborador
        zapatillas_jug == x.zapatillas_jug and // zapatillas jugador
        bikini_jug == x.bikini_jug)
    { // bikini jugador
      return true;
    }
    else
    {
      return false;
    }
  }
};

struct nodeN2
{
  stateN2 st;
  list<Action> secuencia;

  bool operator==(const nodeN2 &n) const
  {
    return (st == n.st);
  }

  bool operator<(const nodeN2 &b) const
  {
    if (st.jugador.f < st.jugador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula < b.st.jugador.brujula)
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula == b.st.jugador.brujula and
             st.zapatillas_jug < b.st.zapatillas_jug) // también se tiene en cuenta las zapas
      return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and
             st.jugador.brujula == b.st.jugador.brujula and
             st.zapatillas_jug == b.st.zapatillas_jug and
             st.bikini_jug < b.st.bikini_jug) // también se tiene en cuenta el bikini
      return true;
    else
      return false;
  }
};

class ComportamientoJugador : public Comportamiento
{
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
  }
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  list<Action> AvanzaASaltosDeCaballo();

  bool casillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa);
  ubicacion nextCasilla(const ubicacion &pos);

  void anularMatriz(vector<vector<unsigned char>> &matriz);
  void PintaPlan(const list<Action> &plan);
  void visualizarPlan(const stateN0 &st, const list<Action> &plan);
  void visualizarPlan (const stateN1 &st, const list<Action> &plan);
  void visualizarPlan(const stateN2 &st, const list<Action> &plan);

  bool Find(const stateN0 &item, const list<stateN0> &lista);
  bool Find(const stateN0 &item, const list<nodeN0> &lista);

  bool Find (const stateN1 &item, const list<stateN1> &lista);
  bool Find (const stateN1 &item, const list<nodeN1> &lista);


  //// NIVEL 0: ANCHURA JUGADOR
  bool anchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
  list<Action> anchuraSoloJugador_V2(const stateN0 &inicio, const ubicacion &final,
                                     const vector<vector<unsigned char>> &mapa);
  list<Action> anchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final,
                                     const vector<vector<unsigned char>> &mapa);

  stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> mapa);

  //// NIVEL 1: ANCHURA COLABORADOR
  stateN1 applyN1(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> mapa);
  list<Action> anchuraColaboradorN1(const stateN1 &inicio, const ubicacion &final,
                                    const vector<vector<unsigned char>> &mapa);
  bool jugadorVeColaborador(const ubicacion & j, const ubicacion & s);

  //// NIVEL 2: COSTE UNIFORME JUGADOR
  list<Action> CosteUniforme(const stateN2 &inicio, const ubicacion &final,
                             const vector<vector<unsigned char>> &mapa);
  list<Action> CosteUniformeconSet(const stateN2 &inicio, const ubicacion &final,
                            const vector<vector<unsigned char>> &mapa);

  stateN2 applyN2(const Action &a, const stateN2 &st, const vector<vector<unsigned char>> mapa);
  list<nodeN2>::iterator estaEnLista(list<nodeN2> &l, const nodeN2 &n);
  void insertarEnOrden(list<nodeN2> &l, nodeN2 &n);                     
  bool Find (const stateN2 &item, const list<stateN2> &lista);
  bool Find (const stateN2 &item, const list<nodeN2> &lista);


private:
  // Declarar Variables de Estado
  list<Action> plan; // Almacena el plan en ejecución
  bool hayPlan;      // Si es true indica que se está siguiendo un plan
  stateN0 c_state;
  stateN1 c_state1;
  stateN2 c_state2;
  ubicacion goal;

  // debug
  int contador = 0;
};

#endif
