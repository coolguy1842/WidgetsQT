#pragma once

#include <QObject>
#include <QProcess>
#include <QScopedPointer>
#include <optional>

namespace Opal::Util::SassEmbedded {

struct VersionOutput {
    QString protocolVersion;
    QString compilerVersion;
    QString implementationVersion;
    QString implementationName;
};

struct CompileOptions {
    enum OutputStyle {
        EXPANDED,
        COMPRESSED
    };

    OutputStyle style      = OutputStyle::COMPRESSED;
    bool generateSourceMap = false;
};

struct CompilationOutput {
    enum ErrorType {
        NONE = 0,
        NO_PROCESS,
        EXPECTED_EXPRESSION,
        EXPECTED_SEMICOLON,
        EXPECTED_LEFT_BRACE,
        EXPECTED_RIGHT_BRACE,
        UNDEFINED_VARIABLE,
        UNKNOWN
    };

    std::optional<QString> data;

    ErrorType errorType;
    QString errorMessage;
};

class SassPrivate;
class Sass : public QObject {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Sass);

public:
    Sass(QString sassProgram = "dart-sass", QStringList sassProgramArgs = { "--embedded" });
    ~Sass();

    CompilationOutput compile(QString path, CompileOptions options = {});
    CompilationOutput compileString(QString scss, CompileOptions options = {});

    std::optional<VersionOutput> version();

private:
    QScopedPointer<SassPrivate> d_ptr;
};

}  // namespace Opal::Util::SassEmbedded
