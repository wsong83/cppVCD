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
#include <gmpxx.h>
#include "crange.h"

namespace vcd {

  class SigRecord {
  public:
    std::string sig_name;
    CRange range;
    std::string value;
    vector<double> rvalue;
  }

  class WaveDB {
  public:
    WaveDB();

    void set_delimiter(char);
    void set_time_unit(const string&);
    void push_scope(const std::string&);
    void pop_scope();
    void set_time(mpz_class);
    void add_id(const std::string&, const std::string&, const CRange&, unsigned int width);
    void add_change(const std::string&, const std::string&);
    void add_change(const std::string&, double);



  private:
    char delimiter;
    std::list<std::string> current_scope;
    std::string hier;
    std::string time_unit;
    mpz_class current_time;
    std::map<std::string, SigRecord> idDB;    // store the ids in VCD 
    std::map<std::string, SigRecord> sigDB;   // store the signals in VCD

    

  };

}

#endif
