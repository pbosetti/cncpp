// URL: https://github.com/mircodz/tojson/blob/master/tojson.hpp

#ifndef YAML2JSON_HPP
#define YAML2JSON_HPP

nlohmann::json parse_scalar(const YAML::Node &node) {
  int i;
  double d;
  bool b;
  std::string s;

  if (YAML::convert<int>::decode(node, i))
    return i;
  if (YAML::convert<double>::decode(node, d))
    return d;
  if (YAML::convert<bool>::decode(node, b))
    return b;
  if (YAML::convert<std::string>::decode(node, s))
    return s;

  return nullptr;
}

nlohmann::json yaml2json(const YAML::Node &root) {
  nlohmann::json j{};

  switch (root.Type()) {
  case YAML::NodeType::Null:
    break;
  case YAML::NodeType::Scalar:
    return parse_scalar(root);
  case YAML::NodeType::Sequence:
    for (auto &&node : root)
      j.emplace_back(yaml2json(node));
    break;
  case YAML::NodeType::Map:
    for (auto &&it : root)
      j[it.first.as<std::string>()] = yaml2json(it.second);
    break;
  default:
    break;
  }
  return j;
}

#endif /* YAML2JSON_HPP */
