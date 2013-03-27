// MatDataTable class

#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "MatDataTable.h"
#include "SqliteDb.h"

#include "Env.h"
#include "CycException.h"

using namespace std;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
MatDataTable::MatDataTable() :
  mat_(""),
  elem_len_(0),
  ref_disp_(NULL),
  ref_kd_(NULL),
  ref_sol_(NULL)
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
MatDataTable::MatDataTable(string mat, vector<element_t> elem_vec, map<Elem, 
    int> elem_index, double ref_disp, double ref_kd, double 
    ref_sol) :
  mat_(mat),
  elem_vec_(elem_vec),
  elem_index_(elem_index),
  ref_disp_(ref_disp),
  ref_kd_(ref_kd),
  ref_sol_(ref_sol)
{
  elem_len_= elem_vec_.size();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
MatDataTable::~MatDataTable() {
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double MatDataTable::K_d(Elem ent){
  check_validity(ent);
  return data(ent, KD);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double MatDataTable::S(Elem ent){
  check_validity(ent);
  return data(ent, SOL);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double MatDataTable::D(Elem ent){
  check_validity(ent);
  return data(ent, DISP);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void MatDataTable::check_validity(Elem ent) { 
  map<Elem, int>::iterator it;
  it=elem_index_.find(ent);
  if (it==elem_index_.end()){
    stringstream err;
    err << "Element " << ent << " not valid";
    throw CycException(err.str());
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double MatDataTable::data(Elem ent, ChemDataType data) {
  double to_ret;
  switch( data ){
    case DISP :
      to_ret = elem_vec_[ent].D*rel(ent, DISP);
      break;
    case KD :
      to_ret = elem_vec_[ent].K_d*rel(ent, SOL);
      break;
    case SOL :
      to_ret = elem_vec_[ent].S*rel(ent, SOL);
      break;
    default : 
      throw CycException("The ChemDataType provided is not yet supported.");
  }
  return to_ret;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double MatDataTable::rel(Elem ent, ChemDataType data) {
  double to_ret;
  Elem h = 1;
  switch( data ){
    case DISP : 
      to_ret = elem_vec_[ent].D/elem_vec_[h].D;
      break;
    case KD :
      to_ret = elem_vec_[ent].K_d/elem_vec_[h].K_d;
      break;
    case SOL :
      to_ret = elem_vec_[ent].S/elem_vec_[h].S;
      break;
    default : 
      throw CycException("The ChemDataType provided is not yet supported.");
  }
  return to_ret;
}


