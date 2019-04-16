/*********************************************
 * Author: lucasnn
 * Creation Date: Apr 10, 2019 at 11:00:21 PM
 *********************************************

// Variaveis de Decisão
dvar boolean X1; // Presença ou não do jogador 1 no inicio da partida
dvar boolean X2; // Presença ou não do jogador 2 no inicio da partida
dvar boolean X3; // Presença ou não do jogador 3 no inicio da partida
dvar boolean X4; // Presença ou não do jogador 4 no inicio da partida
dvar boolean X5; // Presença ou não do jogador 5 no inicio da partida
dvar boolean X6; // Presença ou não do jogador 6 no inicio da partida
dvar boolean X7; // Presença ou não do jogador 7 no inicio da partida

// Função Objetivo
maximize 3 * X1 + 2 * X2 + 2 * X3 + 1 * X4 + 3 * X5 + 3 * X6 + 1 * X7;

// Sujeito A
subject to
{
    //X1 + X2 + X3 + X4 + X5 + X6 + X7

    // Primeira restrição
    // Pelo menos 2 no ataque
    0 * X1 + 0 * X2 + 1 * X3 + 1 * X4 + 1 * X5 + 1 * X6 + 1 * X7 >= 2;
    // Pelo menos 2 no centro
    0 * X1 + 1 * X2 + 0 * X3 + 1 * X4 + 0 * X5 + 1 * X6 + 0 * X7 >= 2;
    // Pelo menos 3 na defesa
    1 * X1 + 0 * X2 + 1 * X3 + 0 * X4 + 1 * X5 + 0 * X6 + 1 * X7 >= 3;

    // A média em cada um deve ser no mínimo 2 (>= 2*7)
    // Médias em Assistencias
    3 * X1 + 2 * X2 + 2 * X3 + 1 * X4 + 3 * X5 + 3 * X6 + 7 * X7 >= 14;
    // Médias em Arremesso
    3 * X1 + 1 * X2 + 3 * X3 + 3 * X4 + 3 * X5 + 1 * X6 + 2 * X7 >= 14;
    // Médias em Rebote
    1 * X1 + 3 * X2 + 2 * X3 + 3 * X4 + 3 * X5 + 2 * X6 + 2 * X7 >= 14;
    // Médias em Defesa
    3 * X1 + 2 * X2 + 2 * X3 + 1 * X4 + 3 * X5 + 3 * X6 + 1 * X7 >= 14;

    // Necessário 5 jogadores
    //X1 + X2 + X3 + X4 + X5 + X6 + X7 == 5; // Função de restrição identificada
    X1 + X2 + X3 + X4 + X5 + X6 + X7 >= 5;
    //X1 + X2 + X3 + X4 + X5 + X6 + X7 <= 5;

    // Se X3 começa a partida, então X6 não pode estar no time
    X3 + X6 <= 1;

    // Se X1 começa a partida, então X4 e X6 tambem devem estar no time
    2 * X1 - X2 - X3 <= 0;

    // Como não pode haver o caso do jogador 2 e do jogador 3 começarem a partida
    //X2 + X3 == 1; // Função de restrição identificada
    X2 + X3 >= 1;
    //X2 + X3 <= 1;
    // A soma deles deve ser menor ou igual a um
}

*/

//#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

int main()
{
    try{
    // Creating a envirioment
    IloEnv env;

    // Constants
    IloInt nPlayers = 7;
    IloNumArray rebote        (env, nPlayers, 1, 3, 2, 3, 3, 2, 2);
    IloNumArray arremesso     (env, nPlayers, 3, 1, 3, 3, 3, 1, 2);
    IloNumArray assistencia   (env, nPlayers, 3, 2, 2, 1, 3, 3, 3);
    IloNumArray defesa        (env, nPlayers, 3, 2, 2, 1, 3, 3, 1);

    IloNumArray positionA     (env, nPlayers, 0, 0, 1, 1, 1, 1, 1);
    IloNumArray positionC     (env, nPlayers, 0, 1, 0, 1, 0, 1, 0);
    IloNumArray positionD     (env, nPlayers, 1, 0, 1, 0, 1, 0, 1);

    // Decision variables
    IloNumVarArray players    (env, nPlayers, 0, 1, ILOBOOL);

    //Creating objective function
    IloExpr to_optimize(env);
    for (int i = 0; i < players.getSize(); ++i)
        to_optimize += defesa[i] * players[i];

    // Objective Function
    IloObjective obj(env, to_optimize, IloObjective::Maximize);

    // Constraints and ranges

    // means
    IloExpr sum_assistencia(env);
    IloExpr sum_arremesso(env);
    IloExpr sum_rebote(env);
    IloExpr sum_defesa(env);

    IloExpr sum_positionA(env);
    IloExpr sum_positionC(env);
    IloExpr sum_positionD(env);
    
    IloExpr sum_all(env);

    for (int i = 0; i < players.getSize(); ++i)
    {
        sum_assistencia += assistencia[i] * players[i];
        sum_arremesso += arremesso[i] * players[i];
        sum_rebote += rebote[i] * players[i];
        sum_defesa += defesa[i] * players[i];

        sum_positionA += positionA[i] * players[i];
        sum_positionC += positionC[i] * players[i];
        sum_positionD += positionD[i] * players[i];

        sum_all += players[i];
    }

    IloRange mean_assistencia_restrition (env, 0, sum_assistencia, 14);
    IloRange mean_arremesso_restrition   (env, 0, sum_arremesso,   14);
    IloRange mean_rebote_restrition      (env, 0, sum_rebote,      14);
    IloRange mean_defesa_restrition      (env, 0, sum_defesa,      14);
    IloRange max_players_restrition      (env, 0, sum_all,          5);

    IloRange sum_positionA_restrition    (env, 2, sum_positionA, IloInfinity);
    IloRange sum_positionC_restrition    (env, 2, sum_positionC, IloInfinity);
    IloRange sum_positionD_restrition    (env, 3, sum_positionD, IloInfinity);
    
    // Constrains

    // -3 < x3 + x5 <= 1
    IloRange r1(env, -3, players[2] + players[5], 1);
    
    // 2*x1 - x3 - x5 <= 0
    IloRange r2(env, -IloInfinity, 2* players[0] - players[3] - players[4], 0);
    
    // x2 + x3 == 1
    IloRange r3(env, 1, players[1] + players[2] , 1);

    /**************************************/
    //               MODEL
    /**************************************/

    // Creating a model
    IloModel model(env);
    // Adding obj function
    model.add(obj);
    
    // Add constrains and ranges to the model
    model.add(mean_assistencia_restrition);
    model.add(mean_arremesso_restrition);
    model.add(mean_rebote_restrition);
    model.add(mean_defesa_restrition);
    model.add(max_players_restrition);

    model.add(sum_positionA_restrition);
    model.add(sum_positionC_restrition);
    model.add(sum_positionD_restrition);

    model.add(r1);    
    model.add(r2);
    model.add(r3);


    // Creating a CPLEX solver
    IloCplex cplex(env);
    cplex.setParam(IloCplex::Param::Threads, 1);
    cplex.extract(model);
    // Or
    //IloCplex cplex(model);

    cplex.out() << endl;
    cplex.solve();
    cplex.out() << endl;
    cplex.out() << "solution status = " << cplex.getStatus() << endl;

    cplex.out() << endl << "number of threads = " << cplex.getParam(IloCplex::Param::Threads) << endl;
    cplex.out() << endl << "number of players = " << players.getSize() << endl;
   
    cplex.out() << "\e[32mcost\e[0m = " << cplex.getObjValue() << endl;

    for(int i; i < players.getSize(); ++i){
    //for(int i; i <= players.getSize(); ++i){
        cplex.out() << "x " << i << " = " << (cplex.getValue(players[i]) == 1) << " " << cplex.getValue(players[i]) << endl;
    }

    env.out();
    }
    catch( IloException &e){
        std::cout << "\n \n\e[31mError\e[0m" << endl << e << std::endl;
        e.end();
    }

    return 0;
}