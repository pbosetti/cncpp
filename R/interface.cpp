// R Interface file
// See https://teuder.github.io/rcpp4everyone_en/140_dataframe.html
// [[Rcpp::plugins("cpp17")]]
#include <Rcpp.h>
#include <cncpp.hpp>
#include <iostream>
#include <map>

using namespace Rcpp;
using namespace std;
using namespace cncpp;

// Wrapper class
class CNCpp {
  map<Block::BlockType, string> block_types = {
    {Block::BlockType::RAPID, "RAPID"},
    {Block::BlockType::LINE, "LINE"},
    {Block::BlockType::CWA, "CWA"},
    {Block::BlockType::CCWA, "CCWA"},
    {Block::BlockType::NO_MOTION, "NO_MOTION"},
  };
  
public:
  CNCpp(List blocks, string machine) : 
    _blocks(blocks), _machine(machine), _prog(&_machine) {
    load(blocks);
  }
  
  string version() {
    return cncpp::version();
  }
  
  void reset() {
    _prog.clear();
  }
  
  DataFrame simulate() {
    NumericVector n, t_time, time, lambda, speed, acc, x, y, z;
    CharacterVector type;
    data_t dt = _machine.tq();
    size_t i = 0;
    
    for (auto &b : _prog) {
      if (b.type() == Block::BlockType::NO_MOTION) {
        continue;
      } else if (b.type() == Block::BlockType::RAPID) {
        type.push_back(block_types[b.type()]);
        Point pos = b.target();
        n.push_back(b.n());
        t_time.push_back(i++ * dt);
        time.push_back(0);
        lambda.push_back(1);
        speed.push_back(b.feedrate());
        acc.push_back(0);
        x.push_back(pos.x());
        y.push_back(pos.y());
        z.push_back(pos.z());
        continue;
      }
      b.walk([&](Block &b, data_t t, data_t l, data_t s) {
        type.push_back(block_types[b.type()]);
        Point pos = b.interpolate(l);
        n.push_back(b.n());
        t_time.push_back(i++ * dt);
        time.push_back(t);
        lambda.push_back(l);
        speed.push_back(s);
        acc.push_back(b.profile().current_acc);
        x.push_back(pos.x());
        y.push_back(pos.y());
        z.push_back(pos.z());
      });
    }
    
    DataFrame df = DataFrame::create(
      _["n"] = n,
      _["type"] = type,
      _["time"] = time,
      _["t_time"] = t_time,
      _["lambda"] = lambda,
      _["speed"] = speed,
      _["acc"] = acc,
      _["x"] = x,
      _["y"] = y,
      _["z"] = z
    );
    return df;
  }
  
  void program() {
    cout << _prog.desc(false) << endl;
  }
  
  void load_file(string file) {
    _prog.rewind();
    _prog.load(file);
  }
  
  void load(List blocks) {
    for (auto &b : blocks) {
      _prog << as<string>(b);
    }
  }
  
private:
  List _blocks;
  Machine _machine;
  Program _prog;
};


// R interface

RCPP_MODULE(cncpp) {
  using namespace Rcpp;

    class_<CNCpp>("CNCpp")
    .constructor<List, string>()
    .method("simulate", &CNCpp::simulate)
    .method("load", &CNCpp::load)
    .method("load_file", &CNCpp::load_file)
    .method("version", &CNCpp::version)
    .method("reset", &CNCpp::reset)
    .method("program", &CNCpp::program)
    ;
}