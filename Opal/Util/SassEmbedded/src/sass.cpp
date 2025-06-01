#include <embedded_sass.pb.h>

#include <Opal/Util/SassEmbedded/sass.hpp>
#include <cctype>
#include <cstring>
#include <optional>
#include <string>

using namespace Opal::Util;

class SassEmbedded::SassPrivate {
public:
    Q_DECLARE_PUBLIC(SassEmbedded::Sass);
    SassEmbedded::Sass* q_ptr;

    int nextVersionID     = 0;
    int nextCompilationID = 1;

    QString sassProgram;
    QStringList sassProgramArgs;

    QProcess process;

    ~SassPrivate() {
        process.kill();
        process.waitForFinished();
    }

    SassPrivate(SassEmbedded::Sass* q, QString sassProgram, QStringList sassProgramArgs)
        : q_ptr(q)
        , sassProgram(sassProgram)
        , sassProgramArgs(sassProgramArgs)
        , process(q) {
        process.setProgram(sassProgram);
        process.setArguments(sassProgramArgs);

        process.start();

        if(!process.isOpen()) {
            qCritical("Sass failed to load with program: %s", sassProgram.toStdString().c_str());
        }
    }

public:
    static bool readVarint(QProcess& process, quint64& out) {
        QByteArray varintData;

        process.readAllStandardError();
        process.setReadChannel(QProcess::ProcessChannel::StandardOutput);

        qint64 lastResult = -1;
        // if most significant bit is not set then must continue, else break
        while(varintData.isEmpty() || (varintData.back() & 0x80) != 0) {
            if(varintData.size() >= 10) {
                qWarning("Failed to read length varint, too long");
                return false;
            }

            if(lastResult != 1 && !process.waitForReadyRead(1000)) {
                qWarning("Timed out while waiting for length byte");
                return false;
            }

            char c;
            qint64 readData = lastResult = process.read(&c, 1);
            varintData.push_back(c);

            if(readData == -1) {
                qWarning("Failed to read length varint, invalid return from read");
                return false;
            }
        }

        google::protobuf::io::CodedInputStream inputStream((const uint8_t*)varintData.data(), varintData.size());
        inputStream.ReadVarint64((uint64_t*)&out);

        return true;
    }

    static std::optional<sass::embedded_protocol::OutboundMessage> sendMessage(QProcess& process, const quint32& id, const sass::embedded_protocol::InboundMessage& message) {
        /*
            varints are up to 10 bytes
            ╔══════════╦══════════════════╗
            ║ varint   ║ Length           ║
            ╠══════════╬══════════════════╣
            ║ varint   ║ Compilation ID   ║
            ╠══════════╬══════════════════╣
            ║ protobuf ║ Protobuf Message ║
            ╚══════════╩══════════════════╝
        */

        if(!process.isOpen()) {
            qWarning("Process was not open to send message");
            return std::nullopt;
        }

        if(id == std::numeric_limits<uint32_t>::max()) {
            qWarning("Tried to use a reserved compilation id");
            return std::nullopt;
        }

        std::string data = message.SerializeAsString();

        // send the data
        {
            uint8_t compilationIDPacketBytes[10];
            memset(compilationIDPacketBytes, 0, sizeof(compilationIDPacketBytes));
            size_t compilationIDPacketLen = google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(id, compilationIDPacketBytes) - compilationIDPacketBytes;

            size_t dataLength = compilationIDPacketLen + data.size();
            if(dataLength > (std::pow(2, 53) - 1)) {
                qWarning("Data length was too high");
                return std::nullopt;
            }

            uint8_t lengthPacketBytes[10];
            memset(lengthPacketBytes, 0, sizeof(lengthPacketBytes));

            size_t lengthPacketLen = google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(dataLength, lengthPacketBytes) - lengthPacketBytes;

            process.write((const char*)lengthPacketBytes, lengthPacketLen);
            process.write((const char*)compilationIDPacketBytes, compilationIDPacketLen);
            process.write(data.c_str(), data.size());
        }

        // get the message
        {
            quint64 length;
            if(!readVarint(process, length)) {
                qWarning("Failed to get length varint");
                return std::nullopt;
            }

            QByteArray output;
            bool firstTry = true;
            while((quint64)output.size() != length) {
                if(firstTry ? !process.waitForReadyRead(50) : !process.waitForReadyRead(1000)) {
                    if(!firstTry) {
                        qWarning("Timed out while waiting for data byte");
                        return std::nullopt;
                    }
                }

                firstTry = false;

                QByteArray received = process.read(length - output.size());
                if(received.size() <= 0) {
                    qWarning("Failed to read data, invalid return from read");
                    return std::nullopt;
                }

                output += received;
            }

            uint32_t compilationID;
            google::protobuf::io::CodedInputStream dataInputStream((const uint8_t*)output.data(), output.size());
            if(!dataInputStream.ReadVarint32(&compilationID)) {
                qWarning("Failed to get compilation id varint\n");
                return std::nullopt;
            }

            std::string messageData;
            if(!dataInputStream.ReadString(&messageData, dataInputStream.BytesUntilLimit())) {
                qWarning("Failed to get message string\n");
                return std::nullopt;
            }

            sass::embedded_protocol::OutboundMessage msg;
            msg.ParseFromArray(messageData.data(), messageData.size());

            return msg;
        }
    }
};

constexpr char charToLower(const char c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

constexpr uint64_t hash(std::string_view str) {
    uint64_t hash = 0;
    for(char c : str) {
        hash = (hash * 131) + charToLower(c);
    }

    return hash;
}

constexpr uint64_t operator"" _hash(const char* str, size_t len) { return hash(std::string_view(str, len)); }

SassEmbedded::Sass::~Sass() {}
SassEmbedded::Sass::Sass(QString sassProgram, QStringList sassProgramArgs)
    : d_ptr(new SassEmbedded::SassPrivate(this, sassProgram, sassProgramArgs)) {}

SassEmbedded::CompilationOutput compilationOutputFromData(std::optional<sass::embedded_protocol::OutboundMessage> data) {
    SassEmbedded::CompilationOutput output;

    if(!data.has_value()) {
        output.errorMessage = "";
        output.errorType    = SassEmbedded::CompilationOutput::ErrorType::NO_PROCESS;

        return output;
    }

    auto compileResponse = data->compile_response();
    if(compileResponse.has_success()) {
        output.data = QString::fromStdString(compileResponse.success().css());
        return output;
    }

    switch(hash(compileResponse.failure().message())) {
    case "expected expression."_hash: output.errorType = SassEmbedded::CompilationOutput::ErrorType::EXPECTED_EXPRESSION; break;
    case "expected \";\"."_hash:      output.errorType = SassEmbedded::CompilationOutput::ErrorType::EXPECTED_SEMICOLON; break;
    case "expected \"{\"."_hash:      output.errorType = SassEmbedded::CompilationOutput::ErrorType::EXPECTED_LEFT_BRACE; break;
    case "expected \"}\"."_hash:      output.errorType = SassEmbedded::CompilationOutput::ErrorType::EXPECTED_RIGHT_BRACE; break;
    case "undefined variable."_hash:  output.errorType = SassEmbedded::CompilationOutput::ErrorType::UNDEFINED_VARIABLE; break;
    default:                          output.errorType = SassEmbedded::CompilationOutput::ErrorType::UNKNOWN; break;
    }

    output.errorMessage = QString::fromStdString(compileResponse.failure().formatted());
    return output;
}

SassEmbedded::CompilationOutput SassEmbedded::Sass::compileString(QString scss, CompileOptions options) {
    sass::embedded_protocol::InboundMessage msg;
    *msg.mutable_compile_request()->mutable_string()->mutable_source() = scss.toStdString();
    msg.mutable_compile_request()->set_style((sass::embedded_protocol::OutputStyle)options.style);

    return compilationOutputFromData(SassPrivate::sendMessage(d_ptr->process, d_ptr->nextCompilationID++, msg));
}

SassEmbedded::CompilationOutput SassEmbedded::Sass::compile(QString path, CompileOptions options) {
    sass::embedded_protocol::InboundMessage msg;
    *msg.mutable_compile_request()->mutable_path() = path.toStdString();
    msg.mutable_compile_request()->set_style((sass::embedded_protocol::OutputStyle)options.style);
    msg.mutable_compile_request()->set_source_map(options.generateSourceMap);

    return compilationOutputFromData(SassPrivate::sendMessage(d_ptr->process, d_ptr->nextCompilationID++, msg));
}

std::optional<SassEmbedded::VersionOutput> SassEmbedded::Sass::version() {
    sass::embedded_protocol::InboundMessage msg;
    msg.mutable_version_request()->set_id(d_ptr->nextVersionID++);

    auto data = SassPrivate::sendMessage(d_ptr->process, 0, msg);
    if(!data.has_value()) {
        return std::nullopt;
    }

    VersionOutput output;
    output.protocolVersion       = QString::fromStdString(data->version_response().protocol_version());
    output.compilerVersion       = QString::fromStdString(data->version_response().compiler_version());
    output.implementationVersion = QString::fromStdString(data->version_response().implementation_version());
    output.implementationName    = QString::fromStdString(data->version_response().implementation_name());

    return output;
}