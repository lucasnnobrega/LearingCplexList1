/*

// Itens em um container, são itens inteiros e positivos portando int+
dvar int+ X1; // Item 1
dvar int+ X2; // Item 2
dvar int+ X3; // Item 3
dvar int+ X4; // Item 4
dvar int+ X5; // Item 5
dvar int+ X6; // Item 6

// Funçao Objetivo
// Maximizar o valor da carga
maximize
  10*X1 + 4*X2 + 5*X3 + 9*X4 + 8*X5 + 6*X6;

subject to{
	// Restrição de peso
	9*X1 + 2*X2 + 6*X3 + 7*X4 + 5*X5 + 7*X6 <= 90; 
	
	// Restrição de Volume
	8*X1 + 5*X2 + 3*X3 + 6*X4 + 6*X5 + 4*X6 <= 50;
	
	X1 <= 5; // Existem 5 copias do item 1
	X2 <= 3; // Existem 5 copias do item 2
	// Existem ilimitadas copias do X3, X4, X5, portanto não é necessario declarar como restrição
}

*/

#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

int main()
{

  // Creating a envirioment
  IloEnv env;

  // Decision variables
  IloNumVar I1(env, 0, 5, ILOINT);
  IloNumVar I2(env, 0, 3, ILOINT);
  IloNumVar I3(env, 0, +IloInfinity, ILOINT);
  IloNumVar I4(env, 0, +IloInfinity, ILOINT);
  IloNumVar I5(env, 0, +IloInfinity, ILOINT);
  IloNumVar I6(env, 0, +IloInfinity, ILOINT);

  // Objective Function
  IloObjective obj(env, 10 * I1 + 4 * I2 + 5 * I3 + 9 * I4 + 8 * I5 + 6 * I6, IloObjective::Maximize);

  // Constraints and ranges
  // Weight Restrition
  IloRange r1(env, 0, 9 * I1 + 2 * I2 + 6 * I3 + 7 * I4 + 5 * I5 + 7 * I6, 90);
  // Volume Restrition
  IloRange r2(env, 0, 8 * I1 + 5 * I2 + 3 * I3 + 6 * I4 + 6 * I5 + 4 * I6, 50);

  /**************************************/

  //               MODEL

  /**************************************/

  // Creating a model
  IloModel model(env);
  // Adding obj function
  model.add(obj);
  // Constrains and ranges
  model.add(r1);
  model.add(r2);

  // Creating a CPLEX solver
  IloCplex cplex(env);
  cplex.extract(model);
  // Or
  //IloCplex cplex(model);

  cplex.out() << endl;
  cplex.solve();
  cplex.out() << endl;
  cplex.out() << "solution status = " << cplex.getStatus() << endl;

  cplex.out() << endl;
  cplex.out() << "cost   = " << cplex.getObjValue() << endl;

  cplex.out() << "I1 = " << cplex.getValue(I1) << endl;
  cplex.out() << "I2 = " << cplex.getValue(I2) << endl;
  cplex.out() << "I3 = " << cplex.getValue(I3) << endl;
  cplex.out() << "I4 = " << cplex.getValue(I4) << endl;
  cplex.out() << "I5 = " << cplex.getValue(I5) << endl;
  cplex.out() << "I6 = " << cplex.getValue(I6) << endl;

  env.out();

  return 0;
}