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

	if (sensores.nivel != 4) {
		if (!hayPlan) {
			cout << "Calculamos un nuevo plan" << endl;

			// Nivel 0
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			// Nivel 2
			c_state2.jugador.f = sensores.posF;
			c_state2.jugador.c = sensores.posC;
			c_state2.jugador.brujula = sensores.sentido;
			c_state2.colaborador.f = sensores.CLBposF;
			c_state2.colaborador.c = sensores.CLBposC;
			c_state2.colaborador.brujula = sensores.CLBsentido;
			c_state2.coste = 0;
			c_state2.bikini_jug = c_state2.zapatillas_jug = false;


			switch (sensores.nivel) {
				case 0: plan = anchuraSoloJugador_V3 (c_state, goal, mapaResultado);
					break;
				case 1: // Incluir aquí la llamada al alg. búsqueda del nivel 1
					cout << "Pendiente de implementar el nivel 1" << endl;
					break;
				case 2: plan = CosteUniforme(c_state2, goal, mapaResultado);
					// cout << "Pendiente de implementar el nivel 2" << endl;
					break;
				case 3: // Incluir aquí la llamada al alg. búsqueda del nivel 3
					cout << "Pendiente de implementar el nivel 3" << endl;
					break;
			}

			cout << "Tamano plan: " << plan.size() << endl;;

			if (plan.size() > 0) {
				cout << "Pintamos plan: ";
				if (sensores.nivel == 0) visualizarPlan(c_state, plan);
				else if (sensores.nivel == 2) visualizarPlan(c_state2, plan);
				hayPlan = true;
			}
		}

		if (hayPlan and plan.size() > 0) {
			accion = plan.front();
			plan.pop_front();
		}
		if (plan.size() == 0) {
			cout << "Se completó el plan" << endl;
			hayPlan = false;
		}

	}
	else {
		// Incluir aquí la solución al nivel 4
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

void ComportamientoJugador::visualizarPlan (const stateN2 &st, const list<Action> &plan) {
	anularMatriz(mapaConPlan);
	stateN2 cst = st;

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

//// NIVEL 0

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


list<Action> ComportamientoJugador::anchuraSoloJugador_V3 (const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node; 				// Nodo actual
	list<nodeN0> frontier;				// Lista de estados pendientes de explorar
	set<nodeN0> explored;				// Lista de estados ya explorados
	list<Action> plan;
	current_node.st = inicio;


	// Variable que determina si ya se ha encontrado un nodo que satisface la 
	// condicion de ser solucion.
	bool solutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);

	frontier.push_back(current_node);	

	// Mientras frontier no esté vacío y la solución no se haya encontrado
	while (!frontier.empty() and !solutionFound) {
		contador++;

		frontier.pop_front(); // Se saca el primer nodo de frontier
		explored.insert(current_node); // Se marca el nodo como explorado

		// cout << "______current_node ubicacion: " << current_node.st.jugador.f << ", " << current_node.st.jugador.c << endl;
		// cout << "______sentido: " << current_node.st.jugador.brujula << endl;

		// Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK); 

		// Si se ha llegado al objetivo
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c) {
			current_node = child_walk;
			solutionFound = true;
			// cout << "solutionfound" << endl;
		}
		// Si no se ha llegado a la solución y no encuentra el hijo en explorados
		else if (explored.find(child_walk) == explored.end()) {
			frontier.push_back(child_walk); // añade el nodo generado actWALK a frontier
		}

		// Si todavía no se ha encontrado la solución
		if (!solutionFound) {
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			
			// Si se ha llegado al objetivo
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c) {
				current_node = child_run;
				solutionFound = true;
				// cout << "solutionfound" << endl;
			}
			// Si no se encuentra el hijo en explorados
			else if (explored.find(child_run) == explored.end()) {
				frontier.push_back(child_run);
			}
		}

		// Si todavía no se ha encontrado la solución generamos hijos turnSR y turnL
		if (!solutionFound) {
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);

			if (explored.find(child_turnl) == explored.end()) {
				frontier.push_back(child_turnl);
			}


			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);

			if (explored.find(child_turnsr) == explored.end()) {
				frontier.push_back(child_turnsr);
			}
		}

		// Si no se ha encontrado la solución y abiertos sigue lleno
		if (!solutionFound and !frontier.empty()) {
			current_node = frontier.front(); // Saca el primero de abiertos

			// Mientras abiertos tenga nodos y current_node ya esté en explorados
			while (!frontier.empty() and explored.find(current_node) != explored.end()) {
				frontier.pop_front(); // Sacamos el primero de la lista de abiertos
				if (!frontier.empty()) {
					current_node = frontier.front(); // El nodo actual ahora es el primero de abiertos
				}
			}
		}

	} // fin while (!frontier.empty() and !solutionFound)

	if (solutionFound) {
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}


	// DEBUG
		//
		// plan = current_node.secuencia;
		// PintaPlan(current_node.secuencia);
		//
	// cout << endl; cout << "Resultados:" << endl;
	// cout << "--> Tamano secuencia: " << current_node.secuencia.size() << " pasos" << endl;
	// cout << "\tCurrent node ubicacion: " << current_node.st.jugador.f << ", " << current_node.st.jugador.c << endl;
	// cout << "--> Solucion encontrada?: " << (solutionFound ? "si" : "no") << endl;
	// cout << "--> Contador: " << contador << endl;

	return plan;
}


//// NIVEL 1
stateN1 ComportamientoJugador::applyN1 (const Action &a, const stateN1 &st, const vector<vector<unsigned char>> mapa){
	stateN1 st_result = st;
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
		case act_CLB_WALK:
			sig_ubicacion = nextCasilla(st.jugador);
			if (casillaTransitable(sig_ubicacion, mapa) and
					!(sig_ubicacion.f == st.jugador.f and sig_ubicacion.c == st.jugador.c)) {
						st_result.colaborador = sig_ubicacion;
					}
			break;
		case act_CLB_TURN_SR:
			st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1) % 8);
			break;
		// case act_CLB_STOP:

	}


	return st_result;
}


list<Action> ComportamientoJugador::anchuraColaboradorN1(const stateN1 & inicio, const ubicacion &final,
                  const vector<vector<unsigned char>> & mapa)
{
	nodeN1 current_node; 				// Nodo actual
	list<nodeN1> frontier;				// Lista de estados pendientes de explorar
	set<nodeN1> explored;				// Lista de estados ya explorados
	list<Action> plan;
	current_node.st = inicio;

	// Variable que determina si ya se ha encontrado un nodo que satisface la 
	// condicion de ser solucion.
	bool solutionFound = (current_node.st.colaborador.f == final.f and
						  current_node.st.colaborador.c == final.c);

	frontier.push_back(current_node);

	while (!frontier.empty() && !solutionFound) {
		frontier.pop_front();
		explored.insert(current_node);

		// Si el jugador ve al colaborador se crean los hijos del colaborador
		if (jugadorVeColaborador(current_node.st.jugador, current_node.st.colaborador))
		{
			// Generar hijo act_CLB_WALK
			nodeN1 child_clb_walk = current_node;
			child_clb_walk.st = applyN1(act_CLB_WALK, current_node.st, mapa);

			// Si se ha llegado a la solución
			if (child_clb_walk.st.colaborador.f == final.f and
					child_clb_walk.st.colaborador.c == final.c) {
				child_clb_walk.secuencia.push_back(act_CLB_WALK);
				current_node = child_clb_walk;
				solutionFound = true;
			}
			// Si no se ha llegado a la solución y el hijo act_CLB_WALK no está en explorados
			else if (explored.find(child_clb_walk) == explored.end()) {
				child_clb_walk.secuencia.push_back(act_CLB_WALK);
				frontier.push_back(child_clb_walk);
			}


			if (!solutionFound) {
				nodeN1 child_clb_sr = current_node;
				child_clb_sr.st = applyN1(act_CLB_TURN_SR, current_node.st, mapa);

				if (explored.find(child_clb_sr) == explored.end()) {
					child_clb_sr.secuencia.push_back(act_CLB_TURN_SR);
					frontier.push_back(child_clb_sr);
				}
			}
		} // fin if (jugadorVeColaborador)

		if (!solutionFound) {
			// Generar hijo actWALK
			nodeN1 child_walk = current_node;
			child_walk.st = applyN1(actWALK, current_node.st, mapa);

			if (explored.find(child_walk) == explored.end()) {
				child_walk.secuencia.push_back(actWALK);
				frontier.push_back(child_walk);
			}

			// Generar hijo actRUN
			nodeN1 child_run = current_node;
			child_run.st = applyN1(actRUN, current_node.st, mapa);
			
			// Si no está en explorados, se añade a frontier
			if (explored.find(child_run) == explored.end()) {
				child_run.secuencia.push_back(actRUN);
				frontier.push_back(child_run);
			}

			// Generar hijo actTURN_L
			nodeN1 child_turnl = current_node;
			child_turnl.st = applyN1(actTURN_L, current_node.st, mapa);

			// Si no está en explorados, se añade a frontier
			if (explored.find(child_turnl) == explored.end()) {
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			
			// Generar hijo actTURN_SR
			nodeN1 child_turnsr = current_node;
			child_turnsr.st = applyN1(actTURN_SR, current_node.st, mapa);

			// Si no está en explorados, se añade a frontier
			if (explored.find(child_turnsr) == explored.end()) {
				child_turnsr.secuencia.push_back(actTURN_SR);
				frontier.push_back(child_turnsr);
			}
		}

		// Si no se ha encontrado la solución y abiertos sigue lleno
		if (!solutionFound and !frontier.empty()) {
			current_node = frontier.front(); // Saca el primero de abiertos

			// Mientras abiertos tenga nodos y current_node ya esté en explorados
			while (!frontier.empty() and explored.find(current_node) != explored.end()) {
				frontier.pop_front(); // Sacamos el primero de la lista de abiertos
				if (!frontier.empty()) {
					current_node = frontier.front(); // El nodo actual ahora es el primero de abiertos
				}
			}
		}
	}

	if (solutionFound) {
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	// Si no recibe orden de jugador, meter la ultima accion del colaborador.
	// Acceder a stateN1 ultimaAccionColaborador

	return plan;
}

///// NIVEL 2 
stateN2 ComportamientoJugador::applyN2 (const Action &a, const stateN2 &st, const vector<vector<unsigned char>> mapa)
{
	stateN2 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	char tipo_casilla = mapa[st.jugador.f][st.jugador.c];

	switch (a) {
		case actWALK: // si prox casilla es transitable y no está ocupada por el colaborador
			sig_ubicacion = nextCasilla(st.jugador);
			if (casillaTransitable(sig_ubicacion, mapa) and
				!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c))
			{
				
				// AÑADIDO: segun el tipo de casilla, añade coste
				if (tipo_casilla == 'A') {
					if (!st.bikini_jug) st_result.coste += 100;
					else st_result.coste += 10;
				}
				else if (tipo_casilla == 'B') {
					if (!st.zapatillas_jug) st_result.coste += 50;
					else st_result.coste += 15;
				}
				else if (tipo_casilla == 'T') {
					st_result.coste += 2;
				}
				else {
					st_result.coste += 1;
				}

				// ver si ha pasado a una casilla bikini o de zapas
				char siguiente_tipo = mapa[st_result.jugador.f][st_result.jugador.c];
				if (siguiente_tipo == 'K') {
					st_result.bikini_jug = true;
					st_result.zapatillas_jug = false;
				}
				else if (siguiente_tipo == 'D') {
					st_result.bikini_jug = false;
					st_result.zapatillas_jug = true;
				}
				
				st_result.jugador = sig_ubicacion;
			}
			break;
		case actRUN: // si prox 2 casillas son transitables y no está ocupada por el colaborador
			sig_ubicacion = nextCasilla(st.jugador);
			if (casillaTransitable(sig_ubicacion, mapa) and	
				!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c))
			{
				sig_ubicacion2 = nextCasilla(sig_ubicacion);
				if (casillaTransitable(sig_ubicacion2, mapa) and
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c))
				{

					if (tipo_casilla == 'A') {
						if (!st.bikini_jug) st_result.coste += 150;
						else st_result.coste += 15;
					}
					else if (tipo_casilla == 'B') {
						if (!st.zapatillas_jug) st_result.coste += 75;
						else st_result.coste += 25;
					}
					else if (tipo_casilla == 'T') {
						st_result.coste += 3;
					}
					else {
						st_result.coste += 1;
					}	
						
						
						
					st_result.jugador = sig_ubicacion2;
				}
			}
			break;
		case actTURN_L:
			if (tipo_casilla == 'A') {
				if (!st.bikini_jug) st_result.coste += 30;
				else st_result.coste += 5;
			}
			else if (tipo_casilla == 'B') {
				if (!st.zapatillas_jug) st_result.coste += 7;
				else st_result.coste += 1;
			}
			else if (tipo_casilla == 'T') {
				st_result.coste += 2;
			}
			else {
				st_result.coste += 1;
			}

			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6) % 8);
			break;
		case actTURN_SR:
			if (tipo_casilla == 'A') {
				if (!st.bikini_jug) st_result.coste += 10;
				else st_result.coste += 2;
			}
			else if (tipo_casilla == 'B') {
				if (!st.zapatillas_jug) st_result.coste += 5;
				else st_result.coste += 1;
			}
			else if (tipo_casilla == 'T') {
				st_result.coste += 1;
			}
			else {
				st_result.coste += 1;
			}

			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1) % 8);
			break;
	}


	return st_result;
}



void ComportamientoJugador::insertarEnOrden(list<nodeN2> &l, nodeN2 &n)
{
	auto it = l.begin(); // iterador al principio de la lista l
	bool encontrado = false; // indicando que la posición de inserción no ha sido encontrada
	auto fin = l.rbegin(); // un iterador en reversa para la lista l

	// Comprobar si el coste del último nodo es menor o igual que el coste del nuevo
	if ((*fin).st.coste <= n.st.coste) {
		l.push_back(n); 
		// Se añade al final de la lista porque tiene
		// el mayor coste entre los nodos encontrados hasta ahora
	}
	// En otro caso ((*fin).st.coste > n.st.coste) se busca donde ponerlo
	else {
		// Mientras no se haya llegado al final de la lista y no se haya encontrado la posición
		while (it != l.end() && !encontrado)
		{
			// Si el coste es mayor o igual que el del nodo que se quiere añadir
			if ((*it).st.coste >= n.st.coste)
				encontrado = true; // Encontrada la posición de inserción
			else
				++it; // Se avanza al siguiente nodo
		}
		// Si se ha encontrado la posición de inserción
		if (encontrado)
			l.insert(it, n); // El nodo n se inserta en la posición apuntada por el iterador
		else
			l.push_back(n); // El nodo n se inserta al final (tiene más coste que los demás)
	}
}

// Comprueba si el nodo n está en la lista de nodos (si los estados son iguales)
list<nodeN2>::iterator ComportamientoJugador::estaEnLista (list<nodeN2> &l, const nodeN2 &n) {
	auto it = l.begin(); // Iterador apuntando al principio de la lista
	bool encontrado = false; // El nodo no se ha encontrado

	// Mientras que no llegue al final de la lista y el nodo no ha sido encontrado
	while (it != l.end() and !encontrado) { 
		// Si los estados son iguales
		if((*it).st == n.st){
			encontrado = true; // El nodo se ha encontrado
		}
		else{
			++it; // Se incrementa el iterador para seguir buscando
		}
	}

	return it;
}


list<Action> ComportamientoJugador::CosteUniforme (const stateN2 &inicio, const ubicacion &final,
                  const vector<vector<unsigned char>> &mapa)
{
	list<nodeN2> frontier;  // Nodos a ser explorados
	nodeN2 current_node; 	// Nodo actual
	set<nodeN2> explored; 	// Nodos explorados
	list<Action> plan; 		// Secuencia de acciones (plan)
	current_node.st = inicio;

	bool solutionFound = (current_node.st.jugador.f == final.f &&
						  current_node.st.jugador.c == final.c);
	frontier.push_front(current_node);

	// Mientras frontier no esté vacío y la solución no se haya encontrado
	while (!frontier.empty() and !solutionFound) {
		frontier.pop_front(); // Se saca el primer nodo de frontier
		explored.insert(current_node); // Se marca el nodo como explorado

		nodeN2 child_walk = current_node;
		child_walk.st = applyN2(actWALK, current_node.st, mapa); 

		// Si frontier no está vacío
		if (!frontier.empty()) { 
			auto itw = estaEnLista(frontier, child_walk); // comprueba si ya existe en frontier

			// si el nodo hijo no está en explorados y tampoco en frontier
			if (explored.find(child_walk) == explored.end() and itw == frontier.end()) { 
				child_walk.secuencia.push_back(actWALK); 
				insertarEnOrden(frontier, child_walk); // inserta en explorados
			}
			// si el nodo hijo ya está en frontier pero tiene un coste mayor que el nuevo coste calculado
			else if (itw != frontier.end() and ( itw->st.coste > child_walk.st.coste)) {
				child_walk.secuencia.push_back(actWALK); // actualiza la secuencia del nodo hijo con actwalk
				frontier.erase(itw); // elimina la versión antigua de frontier
				insertarEnOrden(frontier, child_walk); // inserta el nodo hijo actualizado a frontier
			}
		}
		// Si frontier estaba vacío y el nodo hijo no ha sido explorado todavía
		else if (explored.find(child_walk) == explored.end()) {
			child_walk.secuencia.push_back(actWALK);
			frontier.push_back(child_walk); // añade el nodo hijo con la accion actwalk en su secuencia
		}

		/////
		nodeN2 child_turnl = current_node;
		child_turnl.st = applyN2(actTURN_L, current_node.st, mapa);

		if (!frontier.empty()) {
			auto itl = estaEnLista(frontier, child_turnl);

			// si el nodo hijo no está en explorados y tampoco en frontier
			if (explored.find(child_turnl) == explored.end() and (itl == frontier.end())) {
				child_turnl.secuencia.push_back(actTURN_L);
				insertarEnOrden(frontier, child_turnl); // inserta en explorados
			}
			// si el nodo hijo ya está en frontier pero tiene un coste mayor que el nuevo coste calculado
			else if (itl != frontier.end() and (*itl).st.coste > child_turnl.st.coste) {
				child_turnl.secuencia.push_back(actTURN_L); // actualiza la secuencia del nodo hijo con actTURN_L
				frontier.erase(itl); // elimina la versión antigua de frontier
				insertarEnOrden(frontier, child_turnl); // inserta el nodo hijo actualizado a frontier
			}
		}
		// Si frontier estaba vacío y el nodo hijo no ha sido explorado todavía
		else if(explored.find(child_turnl) == explored.end()){
			child_turnl.secuencia.push_back(actTURN_L); // añade el nodo hijo con la accion actTURN_L en su secuencia
			frontier.push_back(child_turnl); 
		}
		/////

		nodeN2 child_turnsr = current_node;
		child_turnsr.st = applyN2(actTURN_SR, current_node.st, mapa);

		if (!frontier.empty()) {
			auto itsr = estaEnLista(frontier, child_turnsr);

			// si el nodo hijo no está en explorados y tampoco en frontier
			if (explored.find(child_turnsr) == explored.end() and (itsr == frontier.end())) {
				child_turnsr.secuencia.push_back(actTURN_SR);
				insertarEnOrden(frontier, child_turnsr); // inserta en explorados
			}
			// si el nodo hijo ya está en frontier pero tiene un coste mayor que el nuevo coste calculado
			else if (itsr != frontier.end() and (*itsr).st.coste > child_turnsr.st.coste) {
				child_turnsr.secuencia.push_back(actTURN_SR); // actualiza la secuencia del nodo hijo con actTURN_SR
				frontier.erase(itsr); // elimina la versión antigua de frontier
				insertarEnOrden(frontier, child_turnsr); // inserta el nodo hijo actualizado a frontier
			}
		}
		// Si frontier estaba vacío y el nodo hijo no ha sido explorado todavía
		else if(explored.find(child_turnsr) == explored.end()){
			child_turnl.secuencia.push_back(actTURN_SR); // añade el nodo hijo con la accion actTURN_SR en su secuencia
			frontier.push_back(child_turnsr); 
		}

		////


		if(!frontier.empty()) // por que?
			current_node = frontier.front();	

		if(current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c)
			solutionFound = true;

	} // fin while

	if (solutionFound) {
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}