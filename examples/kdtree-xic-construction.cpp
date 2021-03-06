/* $Id: example-xic-construction.cpp 1 2010-10-30 01:14:03Z mkirchner $
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

#include <array>
#include <iostream>
#include <fstream>
#include <vector>

#include "spatial/kd_tree.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "fbi/connectedcomponents.h"

#include "example-xic-construction.h"
#include "example-xic-construction-opts.h"

int main(int argc, char* argv[])
{
  using namespace fbi;
  using namespace boost::posix_time;

  ProgramOptions options;
  if (!parseProgramOptions(argc, argv, options)) {
    return 0;
  }

  std::vector<Centroid> centroids = parseFile(options);

  double mzWindowPpm = 2.0;
  double snWindow = 2.1;


  ptime start = microsec_clock::universal_time();
  // construct kd-tree
  typedef std::array<double, 2> KeyType;
  typedef double MappedType;
  typedef ssrc::spatial::kd_tree<KeyType, MappedType> KdTree;
  KdTree kdtree;
  typedef std::vector<Centroid>::iterator CI;

  for (CI && i = centroids.begin(), && end = centroids.end(); i != end; ++i) {
    kdtree.insert(KeyType{{i->mz_, i->sn_}}, i->abundance_);
  }
  kdtree.optimize();
  typedef KdTree::const_iterator KCI;
  std::vector<std::set<std::size_t> > adjList(centroids.size());

  for (KCI && i = kdtree.begin(); !i.end_of_range(); ++i) {
    // construct the range query
    const KeyType llh = {{ i->first[0] * (1 - mzWindowPpm * 1E-6),
      i->first[1] - snWindow - 0.3 }};
    const KeyType urh = {{ i->first[0] * (1 + mzWindowPpm * 1E-6),
      i->first[1] + snWindow + 0.3}};
    // run range query
    const KCI & b = kdtree.begin(llh, urh);
    const std::size_t k = std::distance(b, i);
    std::size_t l = 0;
    // store connectivity
    for (KCI j = b; !j.end_of_range(); ++j, ++l) {
        adjList[k].insert(l);
        adjList[l].insert(k);
    }
  }
  kdtree.clear();
  
  ptime end = microsec_clock::universal_time();
  time_duration td = end - start;
  std::cout << centroids.size() << "\t" << 
    td.total_seconds() << std::endl;

  std::vector<std::size_t> labels;
  findConnectedComponents(adjList, labels); 

  std::ofstream ofs(options.outputfileName_.c_str());
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  for (std::size_t i = 0;i < centroids.size();++i) {
    Centroid& c = centroids[i];
    ofs << c.rt_ << "\t" << c.mz_ << "\t" << c.sn_ << "\t" 
      << c.abundance_ << "\t" << labels[i] << "\n";
  }
  return 0;
}


