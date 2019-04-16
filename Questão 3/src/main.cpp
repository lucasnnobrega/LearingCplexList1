#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

int main()
{

    // Creating a envirioment
    IloEnv env;

    // Decision variables
    IloNumVar MilkPasteur(env, 50000, 60000, ILOFLOAT);
    IloNumVar Cheese(env, 0, 3500, ILOFLOAT);
    IloNumVar Yorgut(env, 0, 20000, ILOFLOAT);

    // Objective Function
    IloObjective obj(env, 0.07 * MilkPasteur + 1.04 * Cheese + 0.2 * Yorgut, IloObjective::Maximize);

    // Constraints and ranges
    IloRange r1(env, 0, MilkPasteur + (10 / 1) * Cheese + (2.5 / 1) * Yorgut, 100000);

    /**************************************/

    //               MODEL

    /**************************************/

    // Creating a model
    IloModel model(env);
    // Adding obj function
    model.add(obj);
    // Constrains and ranges
    model.add(r1);

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
    cplex.out() << "Milk Pasteur = " << cplex.getValue(MilkPasteur) << endl;
    cplex.out() << "Cheese = " << cplex.getValue(Cheese) << endl;
    cplex.out() << "Yorgut = " << cplex.getValue(Yorgut) << endl;

    return 0;
}