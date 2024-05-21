#pragma once

#include <string>
#include <vector>

namespace Beam::Text::Parser {
class Ast {
  public:
    enum Type : unsigned char { AstTypeBlock, AstTypeExpression };

    explicit Ast(const Type& type): type(type) {}

    void addBlock(const std::vector<Ast>& block);

    std::vector<Ast>* getBlock() const;

    std::string getTypeAsString() const;

  private:
    const Type type;

    std::vector<Ast>* block;
};
} // namespace Beam::Text::Parser
