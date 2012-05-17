/* $Id$
 *
 * Copyright (c) 2010 Buote Xu <buote.xu@gmail.com>
 * Copyright (c) 2010 Marc Kirchner <marc.kirchner@childrens.harvard.edu>
 *
 * This file is part of libfbi.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without  restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR  OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 #ifndef __LIBFBI_INCLUDE_FBI_TUPLE_H__
#define __LIBFBI_INCLUDE_FBI_TUPLE_H__

#include <fbi/config.h>
#if defined(__USE_VARIADIC_TEMPLATES__)
#include <tuple>
namespace fbi{
#if defined(__USE_TEMPLATE_ALIASES__)
template<std::size_t __i, typename _Tp>
using tuple_element = std::tuple_element<__i, _Tp>;
#else //else __USE_TEMPLATE_ALIASES__
template <std::size_t __i, typename _Tp>
struct tuple_element {
  typedef typename std::tuple_element<__i, _Tp>::type type;
};
#endif //endif __USE_TEMPLATE_ALIASES__
} //end namespace fbi
#else 
#include <boost/tuple/tuple.hpp>
namespace fbi{
template <int N, class T>
struct tuple_element {
  typedef typename boost::tuples::element<N,T>::type type;
};
} //end namespace fbi

#endif 


#endif