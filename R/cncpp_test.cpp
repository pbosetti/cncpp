// [[Rcpp::plugins("cpp17")]]
#include <Rcpp.h>
#include <block.hpp>
#include <machine.hpp>
#include <iostream>


using namespace Rcpp;
using namespace std;
using namespace cncpp;

class cncpp_test {
public:
  string version() {
    return cncpp::version();
  }
  
  cncpp_test(string yml) {
    machine_yml = yml;
    machine = new Machine(machine_yml);
  }
  
  string machine_desc() {
    if (machine == nullptr) {
      Rcpp::stop("Machine is not initialized");
    }
    return machine->desc(false);
  }
  
  string machine_yml;
  Machine *machine = nullptr;
  
};


RCPP_MODULE(cncpp) {
  using namespace Rcpp;
  
  Rcpp::function("version", &cncpp::version);
  
  class_<cncpp_test>("cncpp_test")
    .constructor<string>()
    .method("version", &cncpp_test::version)
    .method("machine_desc", &cncpp_test::machine_desc)
    .field_readonly("machine_yml", &cncpp_test::machine_yml)
  ;
}