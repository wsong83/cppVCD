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

vcd::SigRecord::SigRecord(const std::string& sig_name, const CRange& range, unsigned int width)
  : sig_name(sig_name), range(range), width(width) {}

void vcd::SigRecord::record_change(mpz_class ctime, const std::string& val) {
  if(value.empty() || value.back().second != val)
    value.push_back(std::pair<mpz_class, string>(ctime, val));
}

void vcd::SigRecord::record_change(mpz_class ctime, const double& val) {
  if(rvalue.empty() || rvalue.back().second != val)
    rvalue.push_back(std::pair<mpz_class, double>(ctime, val));
}

vcd::WaveDB::WaveDB() 
  : delimiter('/'), time_unit(1, "ns"), current_time(0) { }

void vcd::WaveDB::set_delimiter(char d) {
  delimiter = d;
}

void vcd::WaveDB::set_time_unit(unsigned int v, const string& u) {
  time_unit.first = v;
  time_unit.second = u;
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

void vcd::WaveDB::add_id(const std::string& id, const std::string& ref, const CRange& r, unsigned int w) {
  idDB[id] = SigRecord(ref, r, w);
}

