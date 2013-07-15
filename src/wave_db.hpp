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

#ifndef WAVE_DB_H_
#define WAVE_DB_H_

#include <string>
#include <map>
#include <vector>
#include <gmpxx.h>
#include <cassert>
#include "crange.hpp"

namespace vcd {

  class SigRecord {
  public:
    SigRecord() {}
    SigRecord(const std::string&, const CRange&, unsigned int);
    std::string sig_name;
    CRange range;
    unsigned int width;
    std::list<std::pair<mpz_class, std::string> > value;
    std::list<std::pair<mpz_class, double> > rvalue;

    void record_change(mpz_class, const std::string&);
    void record_change(mpz_class, const double&);
  };

  class WaveDB {
  public:
    WaveDB();

    void set_delimiter(char);
    void set_time_unit(unsigned int, const std::string&);
    void push_scope(const std::string&);
    void pop_scope();
    void set_time(mpz_class);
    void add_id(const std::string&, const std::string&, const CRange&, unsigned int width);
    template<typename VT>
    void add_change(const std::string& id, const VT& v) {
      assert(idDB.count(id));
      idDB[id].record_change(current_time, v);
    }

  private:
    char delimiter;
    std::list<std::string> current_scope;
    std::string hier;
    std::pair<unsigned int, std::string> time_unit;
    mpz_class current_time;
    std::map<std::string, SigRecord> idDB;    // store the ids in VCD 
    std::map<std::string, SigRecord> sigDB;   // store the signals in VCD
  };

}

#endif
