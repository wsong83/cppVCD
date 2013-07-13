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
 * VCD parser wrapper class
 * 13/07/2013   Wei Song
 *
 *
 */

#include "vcd_parser.hpp"

#include <fstream>
#include "vcd_util.hpp"
#include "vcd.hh"
#include "wave_db.hpp"


vcd::VCDParser::VCDParser(const std::string& fn)
  : vcd_file(fn) {}


bool vcd::VCDParser::parse(WaveDB * db)
{
  std::fstream vcd_file_handler;
  vcd_file_handler.open(vcd_file);

  VCDLexer * lexer = new VCDLexer(&vcd_file_handler);
  vcd_parser * parser = new vcd_parser(lexer, db);
  return 0 == parser->parse();
}
