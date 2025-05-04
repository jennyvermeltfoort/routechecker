// Implementatie van klasse TankPlanner.

#ifndef TankPlannerCCVar
#define TankPlannerCCVar

#include "tankplanner.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "standaard.h"
using namespace std;

//*************************************************************************

TankPlanner::TankPlanner() {}  // constructor

//*************************************************************************

TankPlanner::~TankPlanner() { rman_verwoest(rman_handle); }  // destructor

//*************************************************************************

bool TankPlanner::leesIn(const char *invoernaam) {
    if (rman_handle != nullptr) {
        return false;
    }

    rman_handle = rman_maak(invoernaam);
    return (rman_handle != nullptr);
}  // leesIn

//*************************************************************************

void TankPlanner::drukAfInstantie() {
    if (rman_handle != nullptr) rman_print(rman_handle);
}  // drukAfInstantie

//*************************************************************************

int TankPlanner::losOpTD(int nrJokers) {
    return (rman_handle != nullptr) ? rman_los_op_td(rman_handle, nrJokers) : -1;
}  // losOpTD

//*************************************************************************

int TankPlanner::losOpBU(int nrJokers, vector<pair<int, bool> > &strategie) {
    if (rman_handle != nullptr) {
        int a = rman_los_op_bu(rman_handle, nrJokers);
        for (int i = 0; i < rman_handle->as - 1; i++) {
            strategie.push_back({rman_handle->oj[i], rman_handle->ok[i]});
        }
        return a;
    }

    return -1;
}  // losOpBU

//*************************************************************************

#endif
