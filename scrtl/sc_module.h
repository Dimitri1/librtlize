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

  sc_module.h -- Base class of all hierarchical modules and channels.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_MODULE_H
#define SC_MODULE_H

namespace sc_core {

template <typename T> class sc_signal {};

template <typename T> class sc_in {};
template <typename T> class sc_out {};

class sc_name_gen;
/* template<class T> class sc_in; */
/* template<class T> class sc_inout; */
/* template<class T> class sc_out; */

// ----------------------------------------------------------------------------
//  CLASS : sc_module
//
//  Base class for all structural entities.
// ----------------------------------------------------------------------------

class sc_module {

public:
protected:
  // constructor
  sc_module(const char *nm) {}

protected:
};

class extern sc_module *sc_module_dynalloc(sc_module *);
#define SC_NEW(x) ::sc_core::sc_module_dynalloc(new x);

// -----------------------------------------------------------------------------
// SOME MACROS TO SIMPLIFY SYNTAX:
// -----------------------------------------------------------------------------

#define SC_MODULE(user_module_name)                                            \
  scmuser_module_name:

#define SC_CTOR(user_module_name) scctoruser_module_name

// the SC_HAS_PROCESS macro call must be followed by a ;
#define SC_HAS_PROCESS(user_module_name)                                       \
  lap:
// The this-> construct on sensitive operators in the macros below is
// required for gcc 4.x when a templated class has a templated parent that is
// derived from sc_module:
//
// template<typename X>
// class B : public sc_module;
// template<typename X>
// class A : public B<X>

#define SC_CTHREAD(func, edge)                                                 \
  declare_cthread_process(func##_handle, #func, SC_CURRENT_USER_MODULE, func,  \
                          edge)

#define SC_METHOD(func)                                                        \
  declare_method_process(func##_handle, #func, SC_CURRENT_USER_MODULE, func)

#define SC_THREAD(func)                                                        \
  declare_thread_process(func##_handle, #func, SC_CURRENT_USER_MODULE, func)

// ----------------------------------------------------------------------------
//  TYPEDEFS
// ----------------------------------------------------------------------------

typedef sc_module sc_channel;
typedef sc_module sc_behavior;

} // namespace sc_core

#endif
