#include "forced_spring.hpp"
#include "spring.hpp"

#include <cmath>
#include <fmt/format.h>
#include <iostream>
#include <rang.hpp>

class Point {
public:
  double x;
  double y;
};

void test_optional() {
  // Why we want to use optionals?
  // Because we can embed in a single type
  // both the value and the existence of that value.
  // Meaning that we can check wether the values exist
  // and only in that case do some stuff.
  //
  // Another use is as return of a function, if the function
  // fails and cannot compute a value, we can return an optional
  // allowing to check if the function was successful and only
  // in that case do some stuff with it.
  //
  // In this test we will use it to set the new machine position
  // only if a new coordinate is available.
  std::optional<double> new_x;
  std::optional<double> new_y;

  // Note that setting to 0.0 or setting to std::nullopt
  // are DIFFERENT. In one case we are setting a valid value of zero,
  // in the second, we are setting that the value does not exist.
  new_x = 0.0;
  // these two lines do the same thing
  new_y = std::nullopt;
  new_y.reset();

  // We want to set the new machine positions if
  // the optionals have already a value inside.
  double machine_x = 10.0;
  double machine_y = 3.0;

  // Check if new_x has already a value, if yes,
  // then set new machine x
  if (new_x) {
    machine_x = new_x.value();
  }
  // set new machine y only if it has the value
  if (new_y) {
    machine_y = new_y.value();
  }
}

// The main function can be called with two arguments:
// 1. argc: number of arguments used to start this executable from the terminal
// 2. argv: the strings of the arguments themself.
//
// the program is called like: ./build/examples/example_6 arg1 arg2
// so in this case argc will be 3, and argv will contain 3 strings:
// - [0]: ./build/examples/example_6
// - [1]: arg1
// - [2]: arg2
//
// We can use these arguments to do some stuff in our program
int main(int argc, char *argv[]) {
  std::cout << argc << " ";
  for (size_t i = 0; i < argc; i++) {
    std::cout << argv[i] << ", ";
  }
  std::cout << std::endl;

  // Default values for our output and config folders.
  std::string out_folder = "./out/";
  std::string configs_folder = "./configs/";

  // Do we have more than one argument?
  // Remember that the first is always the program name itself
  if (argc > 1) {
    // Assuming that the first argument will be the output folder
    out_folder = argv[1];
  }
  if (argc > 2) {
    // Assuming that the second argument is the configs folder
    configs_folder = argv[2];
  }

  // If I have a vector, how can I sort it?
  Vec vec = {0, 10, 2, 6, 20};
  std::sort(vec.begin(), vec.end());

  // If I have a vector of points, how can I sort it?
  // std::sort will have no way to understand if we want to
  // sort by the x or y coordinates. We need to specify
  // how to sort it. We can use a lambda function!
  // Here we will create a function and we will pass it to std::sort
  //
  // the lambda functions will have a capture (specified with []), some
  // arguments (specified as always with ()) and the function code itself
  // (specified inside {}). So the difference from a normal function is only the
  // capture.
  //
  // We can capture some variables from the current scope by copy or by
  // reference. We can specify single variables or we can say "capture all I
  // need automatically".
  // - capturing by copy:
  //     [configs_folder](int dummy_argument) -> int {
  //       std::cout << configs_folder << std::endl;
  //       return dummy_argument;
  //     };
  // - capturing by reference:
  //     [&configs_folder](int dummy_argument) -> int {
  //       std::cout << configs_folder << std::endl;
  //       return dummy_argument;
  //     };
  // - capture all i need by copy:
  //     [=](int dummy_argument) -> int {
  //       std::cout << configs_folder << std::endl;
  //       return dummy_argument;
  //     };
  // - capture all i need by reference:
  //     [&](int dummy_argument) -> int {
  //       std::cout << configs_folder << std::endl;
  //       return dummy_argument;
  //     };
  std::vector<Point> points(10);
  // The sort function is a lambda which sorts by the x coordinate.
  // It returns a bool (specified with -> bool) meaning is a is before b.
  std::sort(points.begin(), points.end(),
            [](Point a, Point b) -> bool { return a.x < b.x; });
  // Here we are sorting by the closest point to the origin!
  std::sort(points.begin(), points.end(), [](Point a, Point b) -> bool {
    return std::hypot(a.x, a.y) < std::hypot(b.x, b.y);
  });

  Point p0 = {10, 10};
  // Here we want to sort by the closest point to p0. We
  // NEED to CAPTURE p0, in this case we are doing it by reference.
  std::sort(points.begin(), points.end(), [&](Point a, Point b) -> bool {
    return std::hypot(a.x - p0.x, a.y - p0.y) <
           std::hypot(b.x - p0.x, b.y - p0.y);
  });

  // The lambda functions can be assigned to variables, which can be used 
  // to call the lambda.
  // Here I am showing how the lambdas passed in input to std::sort, can be
  // assigned to a variable of type std::function.
  //
  // std::function is a templated type, the first element is the
  // return type (in this case bool), then inside the parenthesis we specify 
  // the types of the arguments.
  std::function<bool (Point a, Point b)> sort_function = [](Point a, Point b) -> bool {
    return a.x < b.x;
  };
  // We can call the lambda with:
  bool are_sorted = sort_function(points[0], points[1]);


  // I can use fmt to format a string
  std::cout << fmt::format("Simple format") << std::endl;
  // Here I am chosing to format the string to be 40 characters wide
  // and to center (with ^) the string "CUSTOM MODELS".
  std::cout << fmt::format("{:^40}", "CUSTOM MODELS") << std::endl;
  // Here another centered string
  std::cout << fmt::format("{:^40}", "v1.0") << std::endl;
  std::cout << rang::bg::cyan << fmt::format("{:^40}", "") << rang::bg::reset
            << std::endl;
  // With the rang library we can change the background color (with bg), or the
  // foreground color (with fg) or even the style of the text (italic/bold)
  std::cout << rang::fg::green << "Starting simulation" << rang::fg::reset
            << std::endl;

  // Instantiate our models
  Spring spring("spring");
  ForcedSpring fspring_1("constant_force");
  ForcedSpring fspring_2("sinusoidal_force");

  try {
    spring.load_config(configs_folder);
  } catch (std::runtime_error &e) { // catch all and only the runtime_error that maybe occurred in the
                                  // try scope
    std::cout << "Runtime error: " << rang::fg::red << e.what() << rang::fg::reset << std::endl;
  } catch (std::exception &e) { // catch all the other exceptions
    std::cout << "Exception: " << rang::fg::red << e.what() << rang::fg::reset << std::endl;
  }
  // We maybe loaded the configuration,
  // we want to still save it.
  spring.save_config(configs_folder);

  // fspring_1.load_config(configs_folder);
  // fspring_2.load_config(configs_folder);

  // Start the log in the out_folder
  spring.start_log(out_folder);
  fspring_1.start_log(out_folder);
  fspring_2.start_log(out_folder);

  // Set their initial conditions
  spring.set_x0({1.0, 0.0});
  fspring_1.set_x0({0.0, 0.0});
  fspring_2.set_x0({0.0, 0.0});

  // Performs the actual simulation.
  // Too large integration steps will generate drifts in the solution.
  double tf = 20.0;
  double dt = 0.01;
  for (double t = 0.0; t < tf; t += dt) {
    // No inputs
    spring.step(dt, {});
    // One constant force
    fspring_1.step(dt, {1.0});
    // Constant force + sinusoid
    fspring_2.step(dt, {1.0 + std::sin(t * 2 * M_PI * 10.0)});

    // Using again the lambda to pass an external solver.
    // In this case is a simple Euler.
    fspring_2.step(dt, {1.0 + std::sin(t * 2 * M_PI * 10.0)},
                   [](double dt, Vec states, Vec x_dot) {
                     for (size_t i = 0; i < states.size(); i++) {
                       states[i] = states[i] + dt * x_dot[i];
                     }
                   });

    // Using the same csv_row function to print to the terminal instead of
    // a file.
    // spring.csv_row(std::cout);
  }

  std::cout << rang::fg::green << "Simulation finished" << rang::fg::reset
            << std::endl;

  return 0;
}
