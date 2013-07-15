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
 * 28/06/2013   Wei Song
 *
 *
 */

#ifndef VCD_UTIL_H_
#define VCD_UTIL_H_

#include <string>
#include <iostream>
#include <gmpxx.h>
#include "crange.hpp"
#include "wave_db.hpp"

#define YYSTYPE vcd::vcd_token_type

namespace vcd {
  
  struct vcd_token_type {
    std::string              tStr;            /* string */ 
    char                     tValue;          /* a 4-value value */
    std::string              tBValue;         /* binary value vector */
    double                   tReal;           /* real value */
    int                      tType;           /* types to differentiate key words */
    mpz_class                tNum;            // number
    std::pair<std::string, CRange> tCId;      // complex id
  };

  enum time_unit_t { time_s, time_ms, time_us, time_ns, time_ps, time_fs};
  enum scope_type_t { scope_begin, scope_fork, scope_function, scope_module, scope_task};
  enum var_type_t { var_event, var_integer, var_parameter, var_real, 
                    var_reg, var_supply0, var_supply1, var_time, var_tri, var_triand, 
                    var_trior, var_trireg, var_tri0, var_tri1, var_wand, var_wire,
                    var_wor};
  enum sim_cmd_type { sim_all, sim_off, sim_on, sim_vars};

  class VCDLexer {
  public:
    VCDLexer(std::istream *);
    int lexer(vcd_token_type *);

  private:
    enum state_t {
      S_BEGIN = 0,
      S_COMMENT,
      S_SCOPE_DECL, S_SCOPE_TYPE,
      S_TIMESCALE_DECL, S_TIMESCALE_TIME,
      S_VAR_DECL, S_VAR_TYPE, S_VAR_WIDTH, S_VAR_ID, S_VAR_CID, S_VAR_RANGE,
      S_SIM_TIME, 
      S_VALUE_CHANGE_SCALAR, S_VALUE_CHANGE_VB, S_VALUE_CHANGE_VR, S_VALUE_CHANGE_ID
    };

    std::istream * istm;        // input stream
    std::string buf;
    unsigned int state;

    std::string next_token();   // get the next token
    int validate_token(const std::string&, vcd_token_type *); // analyse the token
    int conv_to_string(const std::string&, vcd_token_type *); // convert the token to a string
    int conv_to_dec(const std::string&, vcd_token_type *);    // convert the token to a decimal number
    int conv_to_real(const std::string&, vcd_token_type *);   // convert the token to a real number

  };

  std::string timeunit_stype(int);

}

#endif
