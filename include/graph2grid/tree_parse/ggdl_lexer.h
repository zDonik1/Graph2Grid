/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <memory>
#include <string>

#include <spimpl.h>

namespace zg2g {

struct Token
{
    enum class Type { Keyword, Identifier, Literal, Syntax, Eof };

public:
    Type type;
    std::string chars;
    int indent = -1;
};

class GgdlLexer
{
    struct PImpl;
    spimpl::impl_ptr<PImpl> impl;

public:
    explicit GgdlLexer(std::shared_ptr<std::istream> stream);

    void setStream(std::shared_ptr<std::istream> stream);

    Token &peek(int tokenNumber = 0);
    Token consume();

    GgdlLexer &operator >>(Token &token);

    bool eof() const;
    operator bool() const;
};

} // namespace zg2g
