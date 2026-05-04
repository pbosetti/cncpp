#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cncpp.hpp"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace py = pybind11;

namespace {

using MachineHandle = std::shared_ptr<cncpp::Machine>;
using PyBlockHandle = std::shared_ptr<struct PyBlock>;

MachineHandle machine_or_default(MachineHandle machine) {
  return machine ? std::move(machine) : std::make_shared<cncpp::Machine>();
}

opt_data_t optional_double(py::object value) {
  if (value.is_none()) {
    return std::nullopt;
  }
  return py::cast<data_t>(value);
}

void machine_load(cncpp::Machine &machine, std::string const &filename) {
  auto path = filename;
  machine.load(path);
}

void machine_load_dict(cncpp::Machine &machine, py::dict data) {
  auto json = py::module_::import("json");
  auto text = py::str(json.attr("dumps")(data));
  auto parsed = nlohmann::json::parse(std::string(text));
  machine.load(parsed);
}

py::object json_to_python(nlohmann::json const &data) {
  auto json = py::module_::import("json");
  return json.attr("loads")(data.dump());
}

py::object point_tuple_or_none(cncpp::Point const &point) {
  if (!point.is_complete()) {
    return py::none();
  }

  auto values = point.vec();
  return py::make_tuple(values[0], values[1], values[2]);
}

std::string block_type_name(cncpp::Block::BlockType type) {
  switch (type) {
  case cncpp::Block::BlockType::RAPID:
    return "RAPID";
  case cncpp::Block::BlockType::LINE:
    return "LINE";
  case cncpp::Block::BlockType::CWA:
    return "CWA";
  case cncpp::Block::BlockType::CCWA:
    return "CCWA";
  case cncpp::Block::BlockType::NO_MOTION:
    return "NO_MOTION";
  }
  return "UNKNOWN";
}

bool is_motion_type(cncpp::Block::BlockType type) {
  return type == cncpp::Block::BlockType::LINE ||
         type == cncpp::Block::BlockType::CWA ||
         type == cncpp::Block::BlockType::CCWA;
}

py::dict profile_summary(cncpp::Block::Profile const &profile) {
  py::dict output;
  output["a"] = profile.a;
  output["d"] = profile.d;
  output["f"] = profile.f;
  output["l"] = profile.l;
  output["fs"] = profile.fs;
  output["fe"] = profile.fe;
  output["dt_1"] = profile.dt_1;
  output["dt_m"] = profile.dt_m;
  output["dt_2"] = profile.dt_2;
  output["dt"] = profile.dt;
  return output;
}

py::dict block_summary(cncpp::Block const &block) {
  auto type = block.type();
  py::dict output;
  output["line"] = block.line();
  output["n"] = block.n();
  output["parsed"] = block.parsed();
  output["type_code"] = static_cast<int>(type);
  output["type"] = block_type_name(type);
  output["tool"] = block.tool();
  output["feedrate"] = block.feedrate();
  output["arc_feedrate"] = block.arc_feedrate();
  output["spindle"] = block.spindle();
  output["length"] = block.length();
  output["dt"] = block.dt();
  output["m"] = block.m();
  output["r"] = block.r();
  return output;
}

struct PyBlock {
  explicit PyBlock(std::string line)
      : block(std::move(line)), machine(std::make_shared<cncpp::Machine>()) {}

  PyBlock(std::string line, MachineHandle machine)
      : block(std::move(line)),
        machine(machine_or_default(std::move(machine))) {}

  PyBlock(std::string line, PyBlockHandle previous_block)
      : block(std::move(line), checked_previous(previous_block)->block),
        machine(previous_block->machine), previous(std::move(previous_block)) {}

  static PyBlockHandle checked_previous(PyBlockHandle const &previous_block) {
    if (!previous_block) {
      throw py::value_error("previous block must not be None");
    }
    return previous_block;
  }

  cncpp::Block block;
  MachineHandle machine;
  PyBlockHandle previous;
};

struct PyProgram {
  explicit PyProgram(MachineHandle machine)
      : machine(machine_or_default(std::move(machine))),
        program(this->machine.get()) {
    current = program.end();
  }

  PyProgram(std::string filename, MachineHandle machine) : PyProgram(machine) {
    program.load(filename);
    current = program.end();
  }

  MachineHandle machine;
  cncpp::Program program;
  cncpp::block_iterator current;
  bool started = false;
};

void require_interpolatable(PyBlock const &block) {
  if (!block.block.parsed()) {
    throw py::value_error("Block must be parsed before interpolation");
  }

  if (!is_motion_type(block.block.type())) {
    throw py::value_error("Only LINE, CWA and CCWA blocks can be interpolated");
  }

  if (block.block.prev == nullptr) {
    throw py::value_error(
        "Interpolation requires a block linked to a parsed previous block");
  }
}

PyBlock &block_parse(PyBlock &block, MachineHandle machine) {
  if (machine) {
    block.machine = std::move(machine);
  }
  block.block.parse(block.machine.get());
  return block;
}

py::dict block_lambda(PyBlock &block, data_t time) {
  data_t speed = 0.0;
  data_t lambda = block.block.lambda(time, speed);
  py::dict output;
  output["lambda"] = lambda;
  output["speed"] = speed;
  output["acceleration"] = block.block.profile().current_acc;
  return output;
}

cncpp::Point block_interpolate_lambda(PyBlock &block, data_t lambda) {
  require_interpolatable(block);
  return block.block.interpolate(lambda);
}

py::dict block_interpolate_time(PyBlock &block, data_t time) {
  require_interpolatable(block);
  data_t lambda = 0.0;
  data_t speed = 0.0;
  auto point = block.block.interpolate(time, lambda, speed);

  py::dict output;
  output["point"] = point;
  output["lambda"] = lambda;
  output["speed"] = speed;
  return output;
}

py::list block_walk(PyBlock &block) {
  require_interpolatable(block);

  py::list output;
  block.block.walk([&](cncpp::Block &current_block, data_t time, data_t lambda,
                       data_t speed) {
    auto point = current_block.interpolate(lambda);
    py::dict row;
    row["time"] = time;
    row["lambda"] = lambda;
    row["speed"] = speed;
    row["x"] = point.x();
    row["y"] = point.y();
    row["z"] = point.z();
    output.append(std::move(row));
  });
  return output;
}

PyBlockHandle make_parsed_block(std::string const &line, MachineHandle machine,
                                PyBlockHandle previous = nullptr) {
  auto block = previous == nullptr
                   ? std::make_shared<PyBlock>(line, std::move(machine))
                   : std::make_shared<PyBlock>(line, std::move(previous));
  block->block.parse(block->machine.get());
  return block;
}

PyBlockHandle program_block_at(PyProgram &program,
                               cncpp::block_iterator target_block) {
  PyBlockHandle block;
  for (auto it = program.program.begin(); it != program.program.end(); ++it) {
    block = make_parsed_block(it->line(), program.machine, block);
    if (it == target_block) {
      return block;
    }
  }
  return nullptr;
}

void rewind_program_cursor(PyProgram &program) {
  program.current = program.program.end();
  program.started = false;
}

PyProgram &program_load(PyProgram &program, std::string const &filename,
                        bool append) {
  program.program.load(filename, append);
  rewind_program_cursor(program);
  return program;
}

PyProgram &program_append(PyProgram &program, py::object lines) {
  if (py::isinstance<py::str>(lines)) {
    program.program << py::cast<std::string>(lines);
  } else {
    for (py::handle line : py::iter(lines)) {
      if (line.is_none()) {
        throw py::value_error("lines must not contain None");
      }
      program.program << py::cast<std::string>(line);
    }
  }

  rewind_program_cursor(program);
  return program;
}

PyProgram &program_reset(PyProgram &program) {
  program.program.reset();
  rewind_program_cursor(program);
  return program;
}

std::vector<std::string> program_lines(PyProgram const &program) {
  std::vector<std::string> output;
  output.reserve(program.program.size());
  for (auto const &block : program.program) {
    output.push_back(block.line());
  }
  return output;
}

std::vector<PyBlockHandle> program_blocks(PyProgram &program) {
  std::vector<PyBlockHandle> output;
  output.reserve(program.program.size());

  PyBlockHandle previous;
  for (auto const &block : program.program) {
    previous = make_parsed_block(block.line(), program.machine, previous);
    output.push_back(previous);
  }
  return output;
}

py::list program_summary(PyProgram const &program) {
  py::list output;
  for (auto const &block : program.program) {
    output.append(block_summary(block));
  }
  return output;
}

py::object program_current(PyProgram &program) {
  if (!program.started || program.current == program.program.end()) {
    return py::none();
  }

  auto block = program_block_at(program, program.current);
  return block == nullptr ? py::none() : py::cast(block);
}

py::object program_load_next(PyProgram &program) {
  if (!program.started) {
    program.current = program.program.begin();
    program.started = true;
  } else if (program.current != program.program.end()) {
    ++program.current;
  }

  if (program.current == program.program.end()) {
    return py::none();
  }

  auto block = program_block_at(program, program.current);
  return block == nullptr ? py::none() : py::cast(block);
}

PyProgram &program_rewind(PyProgram &program) {
  rewind_program_cursor(program);
  return program;
}

py::list program_walk(PyProgram &program) {
  py::list output;
  data_t total_time = 0.0;

  for (auto &block : program.program) {
    if (!is_motion_type(block.type())) {
      continue;
    }

    block.walk([&](cncpp::Block &current_block, data_t block_time,
                   data_t lambda, data_t speed) {
      auto point = current_block.interpolate(lambda);
      py::dict row;
      row["n"] = current_block.n();
      row["time"] = total_time;
      row["block_time"] = block_time;
      row["lambda"] = lambda;
      row["speed"] = speed;
      row["x"] = point.x();
      row["y"] = point.y();
      row["z"] = point.z();
      output.append(std::move(row));
      total_time += program.machine->tq();
    });
  }

  return output;
}

py::dict machine_summary(cncpp::Machine const &machine) {
  py::dict output;
  output["A"] = machine.A();
  output["tq"] = machine.tq();
  output["fmax"] = machine.fmax();
  output["error"] = machine.error();
  output["max_error"] = machine.max_error();
  output["zero"] = point_tuple_or_none(machine.zero());
  output["offset"] = point_tuple_or_none(machine.offset());
  output["setpoint"] = point_tuple_or_none(machine.setpoint());
  output["position"] = point_tuple_or_none(machine.position());
  return output;
}

} // namespace

PYBIND11_MODULE(_cncpp, module) {
  module.doc() = "Python wrappers for the CNCpp C++ CNC kernel";

  py::class_<cncpp::Point>(module, "Point")
      .def(py::init([](py::object x, py::object y, py::object z) {
             return cncpp::Point(optional_double(x), optional_double(y),
                                 optional_double(z));
           }),
           py::arg("x") = py::none(), py::arg("y") = py::none(),
           py::arg("z") = py::none())
      .def(
          "desc",
          [](cncpp::Point const &point, bool colored) {
            return point.desc(colored);
          },
          py::arg("colored") = false)
      .def(
          "reset",
          [](cncpp::Point &point) -> cncpp::Point & {
            point.reset();
            return point;
          },
          py::return_value_policy::reference_internal)
      .def("delta", &cncpp::Point::delta, py::arg("other"))
      .def("length", &cncpp::Point::length)
      .def(
          "modal",
          [](cncpp::Point &point, cncpp::Point const &other) -> cncpp::Point & {
            point.modal(other);
            return point;
          },
          py::arg("other"), py::return_value_policy::reference_internal)
      .def("is_complete", &cncpp::Point::is_complete)
      .def("vec", &cncpp::Point::vec)
      .def_property(
          "x", [](cncpp::Point const &point) { return point.x(); },
          [](cncpp::Point &point, data_t value) { point.x(value); })
      .def_property(
          "y", [](cncpp::Point const &point) { return point.y(); },
          [](cncpp::Point &point, data_t value) { point.y(value); })
      .def_property(
          "z", [](cncpp::Point const &point) { return point.z(); },
          [](cncpp::Point &point, data_t value) { point.z(value); })
      .def("__add__", &cncpp::Point::operator+, py::arg("other"))
      .def("__repr__",
           [](cncpp::Point const &point) { return point.desc(false); });

  py::class_<cncpp::Machine, MachineHandle>(module, "Machine")
      .def(py::init([](py::object filename) {
             auto machine = std::make_shared<cncpp::Machine>();
             if (!filename.is_none()) {
               machine_load(*machine, py::cast<std::string>(filename));
             }
             return machine;
           }),
           py::arg("filename") = py::none())
      .def(
          "desc",
          [](cncpp::Machine const &machine, bool colored) {
            return machine.desc(colored);
          },
          py::arg("colored") = false)
      .def("load", &machine_load, py::arg("filename"))
      .def("load_dict", &machine_load_dict, py::arg("data"))
      .def(
          "quantize",
          [](cncpp::Machine const &machine, data_t time) {
            data_t delta = 0.0;
            data_t quantized = machine.quantize(time, delta);
            py::dict output;
            output["time"] = quantized;
            output["delta"] = delta;
            return output;
          },
          py::arg("time"))
      .def_property_readonly("A", &cncpp::Machine::A)
      .def_property_readonly("tq", &cncpp::Machine::tq)
      .def_property_readonly("fmax", &cncpp::Machine::fmax)
      .def_property_readonly("error", &cncpp::Machine::error)
      .def_property_readonly("max_error", &cncpp::Machine::max_error)
      .def_property_readonly("zero", &cncpp::Machine::zero)
      .def_property_readonly("offset", &cncpp::Machine::offset)
      .def_property(
          "setpoint",
          [](cncpp::Machine const &machine) { return machine.setpoint(); },
          [](cncpp::Machine &machine, cncpp::Point const &point) {
            machine.setpoint(point);
          })
      .def_property(
          "position",
          [](cncpp::Machine const &machine) { return machine.position(); },
          [](cncpp::Machine &machine, cncpp::Point const &point) {
            machine.position(point);
          })
      .def(
          "set_setpoint",
          [](cncpp::Machine &machine, cncpp::Point const &point) {
            return machine.setpoint(point);
          },
          py::arg("point"))
      .def(
          "set_setpoint",
          [](cncpp::Machine &machine, data_t x, data_t y, data_t z) {
            return machine.setpoint(x, y, z);
          },
          py::arg("x"), py::arg("y"), py::arg("z"))
      .def(
          "set_position",
          [](cncpp::Machine &machine, cncpp::Point const &point) {
            return machine.position(point);
          },
          py::arg("point"))
      .def("data",
           [](cncpp::Machine const &machine) {
             return json_to_python(machine.data());
           })
      .def("summary", &machine_summary)
      .def("__repr__",
           [](cncpp::Machine const &machine) { return machine.desc(false); });

  py::enum_<cncpp::Block::BlockType>(module, "BlockType")
      .value("RAPID", cncpp::Block::BlockType::RAPID)
      .value("LINE", cncpp::Block::BlockType::LINE)
      .value("CWA", cncpp::Block::BlockType::CWA)
      .value("CCWA", cncpp::Block::BlockType::CCWA)
      .value("NO_MOTION", cncpp::Block::BlockType::NO_MOTION);

  py::class_<PyBlock, PyBlockHandle>(module, "Block")
      .def(py::init([](std::string line) {
             return std::make_shared<PyBlock>(std::move(line));
           }),
           py::arg("line"))
      .def(py::init([](std::string line, PyBlockHandle previous) {
             return std::make_shared<PyBlock>(std::move(line),
                                              std::move(previous));
           }),
           py::arg("line"), py::arg("previous"))
      .def(py::init([](std::string line, MachineHandle machine) {
             return std::make_shared<PyBlock>(std::move(line),
                                              std::move(machine));
           }),
           py::arg("line"), py::arg("machine"))
      .def("parse", &block_parse, py::arg("machine") = nullptr,
           py::return_value_policy::reference_internal)
      .def(
          "desc",
          [](PyBlock const &block, bool colored) {
            return block.block.desc(colored);
          },
          py::arg("colored") = false)
      .def("lambda_at", &block_lambda, py::arg("time"))
      .def("lambda_", &block_lambda, py::arg("time"))
      .def("interpolate", &block_interpolate_lambda, py::arg("lambda_"))
      .def("interpolate_time", &block_interpolate_time, py::arg("time"))
      .def("walk", &block_walk)
      .def("summary",
           [](PyBlock const &block) { return block_summary(block.block); })
      .def_property_readonly(
          "line", [](PyBlock const &block) { return block.block.line(); })
      .def_property_readonly(
          "n", [](PyBlock const &block) { return block.block.n(); })
      .def_property_readonly(
          "dt", [](PyBlock const &block) { return block.block.dt(); })
      .def_property_readonly(
          "type", [](PyBlock const &block) { return block.block.type(); })
      .def_property_readonly("type_name",
                             [](PyBlock const &block) {
                               return block_type_name(block.block.type());
                             })
      .def_property_readonly(
          "tool", [](PyBlock const &block) { return block.block.tool(); })
      .def_property_readonly(
          "feedrate",
          [](PyBlock const &block) { return block.block.feedrate(); })
      .def_property_readonly(
          "arc_feedrate",
          [](PyBlock const &block) { return block.block.arc_feedrate(); })
      .def_property_readonly(
          "spindle", [](PyBlock const &block) { return block.block.spindle(); })
      .def_property_readonly(
          "length", [](PyBlock const &block) { return block.block.length(); })
      .def_property_readonly(
          "target", [](PyBlock const &block) { return block.block.target(); })
      .def_property_readonly(
          "center", [](PyBlock const &block) { return block.block.center(); })
      .def_property_readonly(
          "delta", [](PyBlock const &block) { return block.block.delta(); })
      .def_property_readonly(
          "m", [](PyBlock const &block) { return block.block.m(); })
      .def_property_readonly("profile",
                             [](PyBlock const &block) {
                               return profile_summary(block.block.profile());
                             })
      .def_property_readonly(
          "parsed", [](PyBlock const &block) { return block.block.parsed(); })
      .def_property_readonly(
          "r", [](PyBlock const &block) { return block.block.r(); })
      .def_property_readonly("machine",
                             [](PyBlock const &block) { return block.machine; })
      .def_property_readonly(
          "previous", [](PyBlock const &block) { return block.previous; })
      .def("__repr__", [](PyBlock const &block) {
        if (!block.block.parsed()) {
          return "<cncpp.Block line='" + block.block.line() + "' (unparsed)>";
        }
        return block.block.desc(false);
      });

  py::class_<PyProgram, std::shared_ptr<PyProgram>>(module, "Program")
      .def(py::init([](MachineHandle machine) {
             return std::make_shared<PyProgram>(std::move(machine));
           }),
           py::arg("machine") = nullptr)
      .def(py::init([](std::string filename, MachineHandle machine) {
             return std::make_shared<PyProgram>(std::move(filename),
                                                std::move(machine));
           }),
           py::arg("filename"), py::arg("machine") = nullptr)
      .def(
          "desc",
          [](PyProgram const &program, bool colored) {
            return program.program.desc(colored);
          },
          py::arg("colored") = false)
      .def("load", &program_load, py::arg("filename"),
           py::arg("append") = false,
           py::return_value_policy::reference_internal)
      .def("append", &program_append, py::arg("lines"),
           py::return_value_policy::reference_internal)
      .def("extend", &program_append, py::arg("lines"),
           py::return_value_policy::reference_internal)
      .def("reset", &program_reset, py::return_value_policy::reference_internal)
      .def("rewind", &program_rewind,
           py::return_value_policy::reference_internal)
      .def("lines", &program_lines)
      .def("blocks", &program_blocks)
      .def("summary", &program_summary)
      .def("current", &program_current)
      .def("load_next", &program_load_next)
      .def("walk", &program_walk)
      .def_property_readonly("done",
                             [](PyProgram const &program) {
                               return program.started &&
                                      program.current == program.program.end();
                             })
      .def_property_readonly(
          "machine", [](PyProgram const &program) { return program.machine; })
      .def("__len__",
           [](PyProgram const &program) { return program.program.size(); })
      .def(
          "__lshift__",
          [](PyProgram &program, std::string const &line) -> PyProgram & {
            program.program << line;
            rewind_program_cursor(program);
            return program;
          },
          py::arg("line"), py::return_value_policy::reference_internal)
      .def("__repr__", [](PyProgram const &program) {
        return "<cncpp.Program blocks=" +
               std::to_string(program.program.size()) + ">";
      });
}
