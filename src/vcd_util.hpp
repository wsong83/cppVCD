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

#idndef VCD_UTIL_H_
#define VCD_UTIL_H_

#include <string>
#include <iostream>

namespace vcd {
  
  struct vcd_token_type {
    std::string              tStr;            /* string */ 
    char                     tValue;          /* a 4-value value */
    std::string              tBValue;         /* binary value vector */
    double                   tReal;           /* real value */
    int                      tType;           /* types to differentiate key words */
    unsigned long            tNum;            // number
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
    std::istream * istm;        // input stream
    std::string buf;
    unsigned int state;

    std::string next_token();   // get the next token
    int validate_token(const std::string&, vcd_token_type *); // analyse the token

  };

}

#endif
