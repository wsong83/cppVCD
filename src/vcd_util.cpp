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
 * Helper classes for the Value Change Dump (VCD) parser
 * 30/06/2013   Wei Song
 *
 *
 */

#include "vcd_util.hpp"

using namespace vcd;

vcd::VCDLexer(std::istream * i) 
  : istm(i), state(0) { }

int vcd::lexer(vcd_token_type * rv) {
  while(true) {
    if(buf.empty()) {
      if(istm->eof()) return 0;
      else std::get_line(*istm, buf);
    }

    string token = next_token();
    if(token.empty()) continue;
    else {
      return validate_token(token, rv);
    }
  }
}

string vcd::next_token() {
  // get rid of starting blanks
  buf.erase(0, buf.find_first_not_of(' '));
  if(buf.empty()) return buf;

  unsigned int loc = buf.find_first_of(' ');
  string t = buf.substr(0, loc);
  buf.erase(0, loc);
  return t;
}

int vcd::validate_token(const string& t, vcd_token_type * tt) {
  const enum state_t {
    S_BEGIN = 0,
    S_SCOPE_DECL, S_SCOPE_TYPE,
    S_TIMESCALE_DECL, S_TIMESCALE_TIME,
    S_VAR_DECL, S_VAR_TYPE, S_VAR_WIDTH, S_VAR_ID, S_VAR_CID,
    S_SIM_DECL, S_SIM_TYPE, 
    S_SIM_TIME, 
    S_VALUE_CHANGE
  }

  switch(state) {
  case S_BEGIN: {
    if(t == "$scope")     { state = S_SCOPE_DECL;     return VCDScope;     }
    if(t == "$timescale") { state = S_TIMESCALE_DECL; return VCDTimeScale; }
    if(t == "$var")       { state = S_VAR_DECL;       return VCDVar;       }
    if(t == "$dumpall")   { state = S_SIM_DECL; tt->tType = sim_all;  return VCDDumpAll;  }
    if(t == "$dumpoff")   { state = S_SIM_DECL; tt->tType = sim_off;  return VCDDumpOff;  }
    if(t == "$dumpon")    { state = S_SIM_DECL; tt->tType = sim_on;   return VCDDumpOn;   }
    if(t == "$dumpvars")  { state = S_SIM_DECL; tt->tType = sim_vars; return VCDDumpVars; }
    if(t[0] == '#')       { state = S_SIM_TIME; buf.insert(0, t, 1, t.size()-1); return '#'; }
  case S_SCOPE_DECL: 
  case S_SCOPE_TYPE:
  case S_TIMESCALE_DECL:
  case S_TIMESCALE_TIME:
  case S_VAR_DECL:
  case S_VAR_TYPE:
  case S_VAR_WIDTH:
  case S_VAR_ID:
  case S_VAR_CID:
  case S_SIM_DECL:
  case S_SIM_TYPE:
  case S_SIM_TIME: 
  case S_VALUE_CHANGE:
  default:
    assert(0 == "wrong lexer state");
    return 0;
  }
}




}
