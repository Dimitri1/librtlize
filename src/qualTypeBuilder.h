// Dimitri Gerin 2019

#ifndef __QTypeBuilder__
#define __QTypeBuilder__

// restricted vhdl gramar class hierarchy
enum qt { scint, scuint, scbool };

static std::map<std::string, qt> sctypesStr = {
    {std::string("sc_int"), scint}, {std::string("sc_uint"), scuint}};

auto getParams(std::string strType) {

  qt type = scint;
  uint32_t dim = 0;

  while (1) {
    size_t found = strType.find("bool");
    if (found != std::string::npos) {
      type = scbool;
      break;
    }

    for (auto &i : sctypesStr) {

      size_t found = strType.find(i.first);
      if (found != std::string::npos) {

        found = found + i.first.size();

        auto temp = strType.substr(found, strType.size() - 1);

        while (temp.find('>') != std::string::npos) {
          temp.erase(std::find(temp.begin(), temp.end(), '>'));
        }

        while (temp.find('<') != std::string::npos) {
          temp.erase(std::find(temp.begin(), temp.end(), '<'));
        }

        while (temp.find(' ') != std::string::npos) {
          temp.erase(std::find(temp.begin(), temp.end(), ' '));
        }
        dim = std::atoi(temp.c_str());
        llvm::errs() << "temp => " << temp << "\n";
      }
    }

    break;
  }

  return std::make_tuple(type, dim);
}

#endif
