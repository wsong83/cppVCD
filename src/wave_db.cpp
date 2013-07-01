/*
 * Copyright (c) 2013-2013 Wei Song <songw@cs.man.ac.uk> 
 *    Advanced Processor Technologies Group, School of Computer Science
 *    University of Manchester, Manchester M13 9PL UK
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

/* 
 * A waveform data base
 * 01/07/2013   Wei Song
 *
 *
 */

#include "wave_db.hpp"

using std::string;

vcd::WaveDB::WaveDB() 
  : delimiter('/'), time_unit("ns"), current_time(0) { }

void vcd::WaveDB::set_delimiter(char d) {
  delimiter = d;
}

void vcd::WaveDB::set_time_unit(const string& u) {
  time_unit = u;
}

void vcd::WaveDB::push_scope(const string& s) {
  if(current_scope.size() == 0)
    hier = s;
  else
    hier += "/" + s;
  current_scope.push_back(s);
}

void vcd::WaveDB::pop_scope() {
  if(current_scope.size() <= 1)  
    hier = "";
  else
    hier = hier.substr(0, hier.size() - current_scope.back().size() - 1);
  current_scope.pop_back();
}

void vcd::WaveDB::set_time(mpz_class t) {
  current_time = t;
}

void vcd::WaveDB::add_id(const std::string& id, const std::string& ref, const CRange& r, unsigned int width w) {
  idDB[id] = SigRecord(ref, r, w);
  sigDB[ref] += SigRecord(ref, r, w);
}

void vcd::WaveDB::add_change(const string& id, const string& v) {
  // find the signal record
  assert(idDB.count(id));
  SigRecord sig_record = idDB[id];
  
  // record the value in the signal database
  assert(sigDB.count(sig_record.sig_name));
  sigDB[sig_record.sig_name].record_change(sig_record, v);
}

void vcd::WaveDB::add_change(const string& id, double v) {
  // find the signal record
  assert(idDB.count(id));
  SigRecord sig_record = idDB[id];

  // record the value in the signal database
  assert(sigDB.count(sig_record.sig_name));
  sigDB[sig_record.sig_name].record_change(sig_record, v);
}
