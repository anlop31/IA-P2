#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;


	if (!hayPlan) {
		// Invocar al método de búsqueda
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula == sensores.sentido;
		c_state.colaborador.f = sensores.CLBposF;
		c_state.colaborador.c = sensores.CLBposC;
		c_state.colaborador.brujula = sensores.CLBsentido;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;

		// hayPlan = anchuraSoloJugador(c_state, goal, mapaResultado);
		// if (hayPlan) cout << "Se encontro un plan" << endl;

		plan = anchuraSoloJugador_V2(c_state, goal, mapaResultado);
		visualizarPlan(c_state, plan);
		hayPlan = true;
	}
	if (hayPlan and plan.size() > 0) {
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0) {
		cout << "Se completo el plan\n";
		hayPlan = false;
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}


list<Action> ComportamientoJugador::AvanzaASaltosDeCaballo() {
	list<Action> secuencia;

	secuencia.push_back(actWALK);
	secuencia.push_back(actWALK);
	secuencia.push_back(actTURN_L);
	secuencia.push_back(actWALK);

	return secuencia;
}

/** primera aproximacion a la implementacion de la busqueda en anchura **/
bool ComportamientoJugador::anchuraSoloJugador (const stateN0 &inicio, const ubicacion &final,
						 const vector<vector<unsigned char>> &mapa)
{
	stateN0 current_state = inicio; 	// Estado actual
	list<stateN0> frontier;				// Lista de estados pendientes de explorar
	list<stateN0> explored;				// Lista de estados ya explorados
	frontier.push_back(current_state);	


	// Variable que determina si ya se ha encontrado un estado que satisface la 
	// condicion de ser solucion.
	bool solutionFound = (current_state.jugador.f == final.f and
						  current_state.jugador.c == final.c);

	while (!frontier.empty() and !solutionFound) {
		frontier.pop_front();
		explored.push_back(current_state);

		// Generar hijo actWALK
		stateN0 child_walk = apply(actWALK, current_state, mapa);

		if (child_walk.jugador.f == final.f and child_walk.jugador.c == final.c) {
			current_state = child_walk;
			solutionFound = true;

		}
		else if (!Find(child_walk, frontier) and !Find(child_walk, explored)) {
			frontier.push_back(child_walk);
		}

		if (!solutionFound) {
			// Generar hijo actRUN
			stateN0 child_run = apply(actRUN, current_state, mapa);
			
			if (child_run.jugador.f == final.f and child_run.jugador.c == final.c) {
				current_state = child_run;
				solutionFound = true;
			}
			else if (!Find(child_run, frontier) and !Find(child_run, explored)) {
				frontier.push_back(child_run);
			}
		}

		if (!solutionFound) {
			// Generar hijo actTURN_L
			stateN0 child_turnl = apply(actTURN_L, current_state, mapa);
			
			if (!Find(child_turnl, frontier) and !Find(child_turnl, explored)) {
				frontier.push_back(child_turnl);
			}

			// Generar hijo actTURN_SR
			stateN0 child_turnsr = apply(actTURN_SR, current_state, mapa);
			
			if (!Find(child_turnsr, frontier) and !Find(child_turnsr, explored)) {
				frontier.push_back(child_turnsr);
			}
		}

		if (!solutionFound and !frontier.empty()) {
			current_state = frontier.front();
		}

	}

	return solutionFound;
}

/* Devuelve si una ubicación en el mapa es transitable para el agente */
bool ComportamientoJugador::casillaTransitable (const ubicacion &x, const vector<vector<unsigned char>> &mapa) {
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
}

/* Devuelve la ubicación siguiente según el avance del agente */
ubicacion ComportamientoJugador::nextCasilla (const ubicacion &pos) {
	ubicacion next = pos;

	switch (pos.brujula)
	{
		case norte:
			next.f = pos.f - 1;
			break;
		case noreste:
			next.f = pos.f - 1;
			next.c = pos.c + 1;
			break;
		case este:
			next.c = pos.c + 1;
			break;
		case sureste:
			next.f = pos.f + 1;
			next.c = pos.c + 1;
			break;
		case sur:
			next.f = pos.f + 1;
			break;
		case suroeste:
			next.f = pos.f + 1;
			next.c = pos.c - 1;
			break;
		case oeste:
			next.c = pos.c - 1;
			break;
		case noroeste:
			next.f = pos.f - 1;
			next.c = pos.c - 1;
			break;
	}

	return next;
}

/* Devuelve el estado que se genera si el agente puede avanzar.
   Si no puede avanzar, devuelve como salida el mismo estado de entrada. */
stateN0 ComportamientoJugador::apply (const Action &a, const stateN0 &st, const vector<vector<unsigned char>> mapa) {
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;

	switch (a) {
		case actWALK: // si prox casilla es transitable y no está ocupada por el colaborador
			sig_ubicacion = nextCasilla(st.jugador);
			if (casillaTransitable(sig_ubicacion, mapa) and
					!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)) {
						st_result.jugador = sig_ubicacion;
					}
			break;
		case actRUN: // si prox 2 casillas son transitables y no está ocupada por el colaborador
			sig_ubicacion = nextCasilla(st.jugador);
			if (casillaTransitable(sig_ubicacion, mapa) and	
					!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)) {
						sig_ubicacion2 = nextCasilla(sig_ubicacion);
						if (casillaTransitable(sig_ubicacion2, mapa) and
							!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)) {
								st_result.jugador = sig_ubicacion2;
							}
					}
			break;
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6) % 8);
			break;
		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1) % 8);
			break;
	}


	return st_result;
}

/* Encuentra si el elemento item está en la lista */
bool ComportamientoJugador::Find (const stateN0 &item, const list<stateN0> &lista) {
	auto it = lista.begin();
	while ( it != lista.end() and !( (*it) == item ) ) {
		it++;
	}

	return ( !( it == lista.end() ) );
}

bool ComportamientoJugador::Find (const stateN0 &item, const list<nodeN0> &lista) {
	auto it = lista.begin();
	while ( it != lista.end() and !( it->st == item ) ) {
		it++;
	}

	return ( !( it == lista.end() ) );
}

void ComportamientoJugador::anularMatriz (vector<vector<unsigned char>> &matriz) {
	for (int i = 0; i < matriz.size(); i++) {
		for (int j = 0; j < matriz[0].size(); j++) {
			matriz[i][j] = 0;
		}
	}
}

void ComportamientoJugador::visualizarPlan (const stateN0 &st, const list<Action> &plan) {
	anularMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	
	while (it != plan.end()) {
		if ( (*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP) )
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = nextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = nextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = nextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = nextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) %  8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) %  8);
			break;
		case act_CLB_TURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.colaborador.brujula + 1) %  8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
	
}

void ComportamientoJugador::PintaPlan (const list<Action> &plan)
{
	auto it = plan.begin();
	
	while (it != plan.end())
	{
		if (*it == actWALK) cout << "W ";
		else if (*it == actRUN) cout << "R ";
		else if (*it == actTURN_SR) cout << "r ";
		else if (*it == actTURN_L) cout << "L ";
		else if (*it == act_CLB_WALK) cout << "cW ";
		else if (*it == act_CLB_TURN_SR) cout << "cr ";
		else if (*it == act_CLB_STOP) cout << "cS ";
		else if (*it == actIDLE) cout << "I ";
		else cout << "-_ ";

		it++;
	}

	cout << " (" << plan.size() << " acciones)" << endl;
}

list<Action> ComportamientoJugador::anchuraSoloJugador_V2 (const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node; 				// Nodo actual
	list<nodeN0> frontier;				// Lista de estados pendientes de explorar
	list<nodeN0> explored;				// Lista de estados ya explorados
	list<Action> plan;
	current_node.st = inicio;


	// Variable que determina si ya se ha encontrado un nodo que satisface la 
	// condicion de ser solucion.
	bool solutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);

	frontier.push_back(current_node);	


	while (!frontier.empty() and !solutionFound) {
		frontier.pop_front();
		explored.push_back(current_node);

		// Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);

		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c) {
			current_node = child_walk;
			solutionFound = true;

		}
		else if (!Find(child_walk.st, frontier) and !Find(child_walk.st, explored)) {
			frontier.push_back(child_walk);
		}

		if (!solutionFound) {
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c) {
				current_node = child_run;
				solutionFound = true;
			}
			else if (!Find(child_run.st, frontier) and !Find(child_run.st, explored)) {
				frontier.push_back(child_run);
			}
		}

		if (!solutionFound) {
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			
			if (!Find(child_turnl.st, frontier) and !Find(child_turnl.st, explored)) {
				frontier.push_back(child_turnl);
			}

			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			
			if (!Find(child_turnsr.st, frontier) and !Find(child_turnsr.st, explored)) {
				frontier.push_back(child_turnsr);
			}
		}

		if (!solutionFound and !frontier.empty()) {
			current_node = frontier.front();
		}

	}

	if (solutionFound) {
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}