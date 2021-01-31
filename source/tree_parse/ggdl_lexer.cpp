/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <graph2grid/tree_parse/ggdl_lexer.h>

#include <istream>
#include <memory>
#include <queue>

using namespace std;
using namespace spimpl;
using namespace zg2g;

// ---- utility functions ----

inline bool isWhitespace(char c)
{
    return c == ' ' || c == '\t';
}

inline bool isUpperLetter(char c)
{
    return c >= 'A' && c <= 'Z';
}

inline bool isLowerLetter(char c)
{
    return c >= 'a' && c <= 'z';
}

inline bool isLetter(char c)
{
    return isUpperLetter(c) || isLowerLetter(c);
}

inline bool isNumber(char c)
{
    return c >= '0' && c <= '9';
}

// ---- implementation ----

struct GgdlLexer::PImpl
{
    shared_ptr<istream> stream;
    queue<Token> tokens; // tokens buffer for peeking
    bool isEof = false;

public:
    PImpl(shared_ptr<istream> stream)
        : stream(stream)
    {
    }

    inline bool eof() const
    {
        return stream->eof() || stream->fail();
    }

    inline void pushIntoToken(char c, Token &token)
    {
        token.chars.push_back(c);
        stream->ignore();
    }

    bool isKeyword(string_view chars)
    {
        static constexpr string_view keywords[] = {
            "Component",
            "System",
            "SystemStack",
            "count",
            "size",
            "near",
            "clusterCount",
            "preferredDistance",
            "minDistance",
            "maxDistance",
        };

        return any_of(cbegin(keywords), cend(keywords), [chars](const string_view &keyword)
        { return chars.substr(chars.find_first_not_of(" \t")) == keyword; });
    }

    Token tokenize()
    {
        Token token;

        if (eof()) {
            isEof = true;
            token.type = Token::Type::Eof;
            return token;
        }

        char c;
        bool isWhitespaced = false; // leading whitespace appended or not
        bool isFirstChar = true;
        bool hasDigits = false;
        bool onlyDigits = true;
        while (!eof()) {
            c = static_cast<char>(stream->peek());
            if (c == -1) {
                stream->ignore();
                if (token.chars.empty()) {
                    isEof = true;
                    token.type = Token::Type::Eof;
                    return token;
                } else {
                    break;
                }
            }

            if (isWhitespace(c)) {
                if (!isWhitespaced) {
                    pushIntoToken(c, token);
                } else {
                    break;
                }
            } else {
                isWhitespaced = true;

                if (isNumber(c)) {
                    hasDigits = true;
                    pushIntoToken(c, token);
                } else if (isLetter(c)) {
                    onlyDigits = false;
                    pushIntoToken(c, token);
                } else if (isFirstChar) { // only handles 1 character syntax chars
                    pushIntoToken(c, token);
                    token.type = Token::Type::Syntax;
                    return token;
                } else { // syntax met after keyword/identifier/literal
                    break;
                }

                isFirstChar = false;
            }
        }

        if (onlyDigits) {
            token.type = Token::Type::Literal;
        } else if (isKeyword(token.chars)) {
            token.type = Token::Type::Keyword;
        } else {
            token.type = Token::Type::Identifier;
        }

        return token;
    }
};

GgdlLexer::GgdlLexer(shared_ptr<istream> stream)
    : impl(make_impl<PImpl>(move(stream)))
{
}

void GgdlLexer::setStream(shared_ptr<istream> stream)
{
    impl->isEof = false;
    impl->stream = stream;
}

Token &GgdlLexer::peek(int tokenNumber)
{
    const auto tokensToRead = static_cast<int>(tokenNumber - impl->tokens.size()) + 1;
    for (int i = 0; i < tokensToRead; ++i) {
        impl->tokens.push(impl->tokenize());
    }

    return impl->tokens.back();
}

Token GgdlLexer::consume()
{
    if (impl->tokens.empty()) {
        return impl->tokenize();
    } else {
        Token token = impl->tokens.front();
        impl->tokens.pop();
        return token;
    }
}

GgdlLexer &GgdlLexer::operator >>(Token &token)
{
    token = consume();
    return *this;
}

bool GgdlLexer::eof() const
{
    return impl->isEof;
}

GgdlLexer::operator bool() const
{
    return impl->isEof;
}
