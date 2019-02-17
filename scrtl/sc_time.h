/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_time.h -- The time class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_TIME_H
#define SC_TIME_H

namespace sc_core {

class sc_simcontext;

enum sc_time_unit { SC_FS = 0, SC_PS, SC_NS, SC_US, SC_MS, SC_SEC };

// ----------------------------------------------------------------------------
//  CLASS : sc_time
//
//  The time class.
// ----------------------------------------------------------------------------

class sc_time {
public:
  // constructors

  sc_time() {}

private:
};
#endif
