/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CMSIS_PLUS_POSIX_IO_IOREF_H_
#define CMSIS_PLUS_POSIX_IO_IOREF_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/types.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/diag/trace.h>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {


    template<class T = io>
    class ioref {
      static_assert(std::is_base_of<io, T>::value,"Must be a 'io' object");
    public:
      using iotype_t = T;
      using ioref_t = ioref<T>;

      ioref() : ptr_(nullptr) {}

      /*ioref(T* t) : ptr_(t)
      {
	if (t)
	  {
	    t->refInc();
	  }
      }*/

      ioref(io* t) : ptr_(nullptr)
      {
	assign(t);
      }

      ioref(const ioref_t& t) : ptr_(nullptr)
      {
	assign(t.ptr_);
      }

      template<class Ti = io>
      ioref(const ioref<Ti>& t) : ptr_(nullptr)
      {
	assign(t.getio());
      }

      ~ioref()
      {
	reset();
      }

      void
      reset ()
      {
	assign(nullptr);
      }

      io*
      getio () const
      {
      	return ptr_;
      }

      iotype_t*
      get () const
      {
	  return static_cast<iotype_t*>(ptr_);
      }

      ioref_t& operator= (const ioref_t& ptr)
      {
	assign(ptr.ptr_);
	return *this;
      }

      template<class Ti = io>
      ioref_t& operator= (const ioref<Ti>& ptr)
      {
      	assign(ptr.getio());
      	return *this;
      }


      ioref_t& operator= (io* ptr)
      {
	assign(ptr);
	return *this;
      }

      ioref_t& operator= (const std::nullptr_t ptr)
      {
      	assign(nullptr);
      	return *this;
      }

      iotype_t& operator* () const
      {
	return *static_cast<iotype_t*>(ptr_);
      }

      iotype_t* operator-> () const
      {
	return static_cast<iotype_t*>(ptr_);
      }

      bool operator== (std::nullptr_t ptr) const
      {
      	return (ptr_ == nullptr);
      }

      bool operator== (ioref_t ref) const
      {
	  return (ptr_ == ref.ptr_);
      }

      bool operator!= (std::nullptr_t ptr) const
      {
	  return (ptr_ != nullptr);
      }

      bool operator!= (ioref_t ref) const
      {
	return (ptr_ != ref.ptr_);
      }

    protected:
      void assign (io* nptr) {
	io* cptr  = ptr_;

	if (nptr != nullptr)
	  {
	    if ((static_cast<posix::iotype_t>(nptr->type_) & traits::io_type<T>::value) != 0)// || std::is_same<T,posix::io>::value)
	      {
		nptr->refInc ();
	      }
	    else
	      {
		nptr = nullptr; // Bad Type!
	      }
	  }
	if (cptr != nullptr)
	  {
	    cptr->refDec ();
	  }

	ptr_ = nptr;
      }

      io* ptr_;
    };
  }
}
#endif
#endif
