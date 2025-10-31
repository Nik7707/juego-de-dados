#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// --- Función para limpiar pantalla multiplataforma (sin system) ---
void limpiarPantalla() {
    for (int i = 0; i < 40; i++) cout << "\n";
}

// --- Dibujo ASCII de los dados ---
void mostrarDado(int valor) {
    static const string dados[6][5] = {
        {"+-------+", "|       |", "|   o   |", "|       |", "+-------+"},
        {"+-------+", "| o     |", "|       |", "|     o |", "+-------+"},
        {"+-------+", "| o     |", "|   o   |", "|     o |", "+-------+"},
        {"+-------+", "| o   o |", "|       |", "| o   o |", "+-------+"},
        {"+-------+", "| o   o |", "|   o   |", "| o   o |", "+-------+"},
        {"+-------+", "| o   o |", "| o   o |", "| o   o |", "+-------+"}
    };
    for (int i = 0; i < 5; i++)
        cout << dados[valor - 1][i] << endl;
}

// --- Lanzar un dado ---
int lanzarDado() {
    return rand() % 6 + 1;
}

// --- Mostrar reglas ---
void mostrarReglas() {
    cout << "\n=== 📜 REGLAS DEL JUEGO DE DADOS ===\n";
    cout << "1. Cada jugador lanza dos dados por ronda.\n";
    cout << "2. La suma de ambos se añade a su puntuación total.\n";
    cout << "3. El jugador con más puntos al final gana.\n";
    cout << "4. Si varios empatan, todos ganan. 🤝\n";
    cout << "5. Se muestra el historial de lanzamientos al final.\n";
    cout << "====================================\n\n";
}

// --- Créditos finales ---
void mostrarCreditos() {
    cout << "\n====================================\n";
    cout << "🎮 JUEGO DE DADOS FINALIZADO 🎮\n";
    cout << "Desarrollado por: anonymous 😎\n";
    cout << "Versión: 5.0 (Con historial de lanzamientos)\n";
    cout << "Gracias por jugar 🎲 ¡Hasta la próxima!\n";
    cout << "====================================\n";
}

// --- Animación de lanzamiento ---
void animarLanzamiento() {
    cout << "\nLanzando dados";
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(400));
    }
    cout << "\n";
}

// --- Mostrar marcador dinámico ---
void mostrarMarcador(const vector<string>& nombres, const vector<int>& puntajes, int rondaActual, int totalRondas) {
    cout << "\n================== 🧾 MARCADOR ==================\n";
    cout << "Ronda " << rondaActual << " de " << totalRondas << "\n";
    cout << "-------------------------------------------------\n";
    cout << left << setw(15) << "Jugador" << "| Puntos\n";
    cout << "-------------------------------------------------\n";
    for (size_t i = 0; i < nombres.size(); ++i) {
        cout << left << setw(15) << nombres[i] << "| " << puntajes[i] << "\n";
    }
    cout << "=================================================\n";
}

// --- Mostrar historial de lanzamientos ---
void mostrarHistorial(const vector<string>& nombres, const vector<vector<string>>& historial) {
    cout << "\n===== 📖 HISTORIAL DE LANZAMIENTOS =====\n";
    for (size_t i = 0; i < nombres.size(); ++i) {
        cout << "\n🔹 " << nombres[i] << ":\n";
        for (const auto& registro : historial[i]) {
            cout << "   " << registro << endl;
        }
    }
    cout << "==========================================\n";
}

// --- Programa principal ---
int main() {
    srand(static_cast<unsigned int>(time(0)));

    int numJugadores;
    int rondas;
    int opcion;

    // --- Menú principal ---
    do {
        cout << "\n=== 🎲 MENU PRINCIPAL - JUEGO DE DADOS ===\n";
        cout << "1. Jugar\n";
        cout << "2. Ver reglas\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 2) {
            mostrarReglas();
        } else if (opcion == 3) {
            mostrarCreditos();
            return 0;
        }

    } while (opcion != 1);

    // --- Configuración de jugadores ---
    cout << "\nIngrese el número de jugadores (mínimo 2, máximo 6): ";
    cin >> numJugadores;
    if (numJugadores < 2) numJugadores = 2;
    if (numJugadores > 6) numJugadores = 6;

    vector<string> nombres(numJugadores);
    vector<int> puntajes(numJugadores, 0);
    vector<vector<string>> historial(numJugadores); // Guarda texto por jugador

    cout << "\n=== Registro de jugadores ===\n";
    for (int i = 0; i < numJugadores; i++) {
        cout << "Nombre del jugador " << i + 1 << ": ";
        cin >> nombres[i];
    }

    cout << "\n¿Cuántas rondas desean jugar? ";
    cin >> rondas;

    // --- Bucle principal del juego ---
    for (int ronda = 1; ronda <= rondas; ronda++) {
        limpiarPantalla();
        cout << "===== 🎯 RONDA " << ronda << " =====\n";

        for (int i = 0; i < numJugadores; i++) {
            cout << "\nTurno de " << nombres[i] << " 🎲\n";
            animarLanzamiento();

            int dado1 = lanzarDado();
            int dado2 = lanzarDado();
            int total = dado1 + dado2;

            mostrarDado(dado1);
            mostrarDado(dado2);

            cout << nombres[i] << " obtiene " << dado1 << " + " << dado2
                 << " = " << total << " puntos.\n";

            puntajes[i] += total;

            // Guardar historial
            string registro = "Ronda " + to_string(ronda) + ": (" + to_string(dado1) + " + " + to_string(dado2) + ") = " + to_string(total);
            historial[i].push_back(registro);

            // Mostrar marcador actualizado
            mostrarMarcador(nombres, puntajes, ronda, rondas);
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        cout << "\nFin de la ronda " << ronda << " ✅";
        mostrarMarcador(nombres, puntajes, ronda, rondas);
        this_thread::sleep_for(chrono::milliseconds(1500));
    }

    // --- Resultados finales ---
    limpiarPantalla();
    cout << "\n===== 🏁 RESULTADOS FINALES =====\n";
    mostrarMarcador(nombres, puntajes, rondas, rondas);

    int maxPuntaje = 0;
    for (int p : puntajes) if (p > maxPuntaje) maxPuntaje = p;

    cout << "\n🏆 GANADOR(ES): ";
    for (size_t i = 0; i < nombres.size(); i++) {
        if (puntajes[i] == maxPuntaje)
            cout << nombres[i] << " ";
    }
    cout << "\n";

    // --- Mostrar historial detallado ---
    mostrarHistorial(nombres, historial);

    mostrarCreditos();
    return 0;
}
