#include <Opal/Core/application.hpp>
#include <Opal/Core/private/application_p.hpp>
#include <Opal/Util/SassEmbedded/sass.hpp>
#include <QDir>
#include <QDirListing>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QObject>

Opal::Core::ApplicationPrivate::~ApplicationPrivate() {}
Opal::Core::ApplicationPrivate::ApplicationPrivate(Core::Application* q)
    : q_ptr(q)
    , m_styleSheetCSSValid(false)
    , m_styleSheetPathCSSValid(false) {
    QObject::connect(&m_styleSheetPathWatcher, &QFileSystemWatcher::fileChanged, q_ptr, &Opal::Core::Application::updateGeneratedStyleSheet);
}

void Opal::Core::ApplicationPrivate::initStylePathWatcher() {
    if(m_styleSheetPathWatcher.directories().size() > 0 || m_styleSheetPathWatcher.files().size() > 0) {
        m_styleSheetPathWatcher.removePaths(m_styleSheetPathWatcher.files() + m_styleSheetPathWatcher.directories());
    }

    if(!q_ptr->styleSheetPathValid()) {
        return;
    }

    QFileInfo info(m_styleSheetPath);
    QDirListing fileList(info.absoluteDir().absolutePath(), QStringList() << "*.css" << "*.scss" << "*", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive | QDirListing::IteratorFlag::IncludeHidden | QDirListing::IteratorFlag::FollowDirSymlinks);
    for(auto it : fileList) {
        m_styleSheetPathWatcher.addPath(it.absoluteFilePath());
    }

    QDirListing dirList(info.absoluteDir().absolutePath(), QDirListing::IteratorFlag::DirsOnly | QDirListing::IteratorFlag::Recursive | QDirListing::IteratorFlag::IncludeHidden | QDirListing::IteratorFlag::FollowDirSymlinks);
    for(auto it : dirList) {
        m_styleSheetPathWatcher.addPath(it.absoluteFilePath());
    }
}

void Opal::Core::ApplicationPrivate::updateStyleSheets() {
    initStylePathWatcher();

    QString css;

    Util::SassEmbedded::Sass sass;
    Util::SassEmbedded::CompilationOutput output;
    if(q_ptr->styleSheetPathValid()) {
        if((output = sass.compile(m_styleSheetPath)).data.has_value()) {
            css += output.data.value() + "\n";
            m_styleSheetPathCSSValid = true;
        }
        else {
            qWarning() << "Couldn't compile SCSS from path: " << m_styleSheetPath << "\n"
                       << output.errorMessage;

            m_styleSheetPathCSSValid = false;
        }
    }
    else {
        m_styleSheetPathCSSValid = true;
    }

    if(!m_styleSheet.isEmpty()) {
        if((output = sass.compileString(m_styleSheet)).data.has_value()) {
            css += output.data.value();
            m_styleSheetCSSValid = true;
        }
        else {
            qWarning() << "Couldn't compile stylesheet SCSS" << "\n"
                       << output.errorMessage;
            m_styleSheetCSSValid = false;
        }
    }
    else {
        m_styleSheetCSSValid = true;
    }

    if(m_generatedCSS != css) {
        m_generatedCSS = css;
        q_ptr->reloadGeneratedStyleSheet();
    }
}

Opal::Core::Application::~Application() {}
Opal::Core::Application::Application(int argc, char** argv)
    : QApplication(argc, argv)
    , d_ptr(new Opal::Core::ApplicationPrivate(this)) {}

void Opal::Core::Application::setStyleSheet(QString scss) {
    d_ptr->m_styleSheet = scss;
    updateGeneratedStyleSheet();
}

void Opal::Core::Application::setStyleSheetPath(QString path) {
    d_ptr->m_styleSheetPath = path;
    d_ptr->initStylePathWatcher();

    updateGeneratedStyleSheet();
}

bool Opal::Core::Application::styleSheetPathValid() const {
    QFileInfo info(d_ptr->m_styleSheetPath);

    // must be an absolute file path
    return info.exists() &&
           info.isAbsolute() &&
           info.isFile() &&
           (QStringList() << "scss" << "css").contains(info.suffix());
}

QString Opal::Core::Application::styleSheet() const { return d_ptr->m_styleSheet; }
bool Opal::Core::Application::styleSheetCSSValid() const { return d_ptr->m_styleSheetCSSValid; }

QString Opal::Core::Application::styleSheetPath() const { return d_ptr->m_styleSheetPath; }
bool Opal::Core::Application::styleSheetPathCSSValid() const { return d_ptr->m_styleSheetPathCSSValid; }

QString Opal::Core::Application::generatedStyleSheet() const { return d_ptr->m_generatedCSS; }
void Opal::Core::Application::reloadGeneratedStyleSheet() { QApplication::setStyleSheet(d_ptr->m_generatedCSS); }
void Opal::Core::Application::updateGeneratedStyleSheet() { d_ptr->updateStyleSheets(); }